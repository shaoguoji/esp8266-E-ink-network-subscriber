/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"

#include "spi.h"
#include "epd2in9.h"
#include "epdif.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "mem.h"

#define COLORED      0
#define UNCOLORED    1

os_timer_t count_timer;
unsigned char *frame_buffer;
char time_string[] = {'0', '0', ':', '0', '0', '\0'};
unsigned long time_start_ms;
unsigned long time_now_s;

EPD epd;
Paint paint;

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR time_tick(void)
{
    time_now_s = (system_get_time()/1000 - time_start_ms) / 1000;
    time_string[0] = time_now_s / 60 / 10 + '0';
    time_string[1] = time_now_s / 60 % 10 + '0';
    time_string[3] = time_now_s % 60 / 10 + '0';
    time_string[4] = time_now_s % 60 % 10 + '0';

    Paint_SetWidth(&paint, 32);
    Paint_SetHeight(&paint, 96);
    Paint_SetRotate(&paint, ROTATE_90);

    Paint_Clear(&paint, UNCOLORED);
    Paint_DrawStringAt(&paint, 0, 4, time_string, &Font24, COLORED);
    EPD_SetFrameMemory(&epd, frame_buffer, 80, 72, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
    EPD_DisplayFrame(&epd);
}

void ICACHE_FLASH_ATTR
user_rf_pre_init(void)
{
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
user_init(void)
{
    os_printf("SDK version:%s\n", system_get_sdk_version());

    frame_buffer = (unsigned char*)os_malloc(EPD_WIDTH * EPD_HEIGHT / 8);
    if (EPD_Init(&epd, lut_full_update) != 0) {
        os_printf("e-Paper init failed\n");
        return;
    }

    Paint_Init(&paint, frame_buffer, epd.width, epd.height);
    Paint_Clear(&paint, UNCOLORED);

    /* For simplicity, the arguments are explicit numerical coordinates */
    /* Write strings to the buffer */
    Paint_DrawFilledRectangle(&paint, 0, 10, 128, 34, COLORED);
    Paint_DrawStringAt(&paint, 0, 14, "Hello world!", &Font16, UNCOLORED);
    Paint_DrawStringAt(&paint, 0, 34, "e-Paper Demo", &Font16, COLORED);

    /* Draw something to the frame buffer */
    Paint_DrawRectangle(&paint, 16, 60, 56, 110, COLORED);
    Paint_DrawLine(&paint, 16, 60, 56, 110, COLORED);
    Paint_DrawLine(&paint, 56, 60, 16, 110, COLORED);
    Paint_DrawCircle(&paint, 120, 90, 30, COLORED);
    Paint_DrawFilledRectangle(&paint, 16, 130, 56, 180, COLORED);
    Paint_DrawFilledCircle(&paint, 120, 160, 30, COLORED);

    /* Display the frame_buffer */
    EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
    EPD_DisplayFrame(&epd);
    EPD_DelayMs(&epd, 2000);

    /**
     *  there are 2 memory areas embedded in the e-paper display
     *  and once the display is refreshed, the memory area will be auto-toggled,
     *  i.e. the next action of SetFrameMemory will set the other memory area
     *  therefore you have to set the frame memory and refresh the display twice.
     */
    EPD_ClearFrameMemory(&epd, 0xFF);
    EPD_DisplayFrame(&epd);
    EPD_ClearFrameMemory(&epd, 0xFF);
    EPD_DisplayFrame(&epd);

    /* EPD_or partial update */
    if (EPD_Init(&epd, lut_partial_update) != 0) {
    os_printf("e-Paper init failed\n");
    return;
    }

    /**
     *  there are 2 memory areas embedded in the e-paper display
     *  and once the display is refreshed, the memory area will be auto-toggled,
     *  i.e. the next action of SetFrameMemory will set the other memory area
     *  therefore you have to set the frame memory and refresh the display twice.
     */
    EPD_SetFrameMemory(&epd, IMAGE_DATA, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_SetFrameMemory(&epd, IMAGE_DATA, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);

    time_start_ms = system_get_time()/1000;

    os_timer_disarm(&count_timer);
    os_timer_setfn(&count_timer, (os_timer_func_t *)time_tick, NULL);
    os_timer_arm(&count_timer, 500, 1);
}

