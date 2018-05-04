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

#include "display.h"
#include "data_handle.h"

#include <string.h>

#define DEVICE_ID   "D446"

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
    // time_now_s = (system_get_time()/1000 - time_start_ms) / 1000;
    // time_string[0] = time_now_s / 60 / 10 + '0';
    // time_string[1] = time_now_s / 60 % 10 + '0';
    // time_string[3] = time_now_s % 60 / 10 + '0';
    // time_string[4] = time_now_s % 60 % 10 + '0';

    // Paint_SetWidth(&paint, 32);
    // Paint_SetHeight(&paint, 96);
    // Paint_SetRotate(&paint, ROTATE_90);

    // Paint_Clear(&paint, UNCOLORED);
    // Paint_DrawStringAt(&paint, 0, 4, time_string, &Font12, COLORED);
    // EPD_SetFrameMemory(&epd, frame_buffer, 80, 72, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
    // EPD_DisplayFrame(&epd);
}

void ICACHE_FLASH_ATTR
user_rf_pre_init(void)
{
}

Data_Type data;
Data_Type config;

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
    
    Display_Init();

    // Display_Welcome();
    // EPD_DelayMs(&epd, 2000);
    // Display_Weather();
    // EPD_DelayMs(&epd, 2000);
    // Display_Github(&githubData);
    // EPD_DelayMs(&epd, 2000);
    // Display_Fund();
    
    char *data_text = "{\"device_id\":\"D446\",\"data_type\":2,\"data_content\":{\"repo_name\":\"alibaba/AliOS-Things\",\"watch\":\"222\",\"star\":\"1147\",\"fork\":\"410\"}}";
    char *config_text = "{\"device_id\":\"D446\",\"data_type\":0,\"data_content\":{\"style\":\"light\",\"font_size\":1}}";

    data_parse(data_text, &data);
    config_parse(config_text, &config);

    if (strcmp(data.device_id, DEVICE_ID) == 0) {
        Display_Reflesh(&data, &config);
    }
    
    // time_start_ms = system_get_time()/1000;

    // os_timer_disarm(&count_timer);
    // os_timer_setfn(&count_timer, (os_timer_func_t *)time_tick, NULL);
    // os_timer_arm(&count_timer, 500, 1);
}

