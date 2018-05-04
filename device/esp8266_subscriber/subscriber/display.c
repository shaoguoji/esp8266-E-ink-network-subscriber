#include "display.h"

#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "mem.h"

#include "spi.h"
#include "epd2in9.h"
#include "epdif.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "data_handle.h"

#include <string.h>

#define DEBUG 0

unsigned char *frame_buffer;
EPD epd;
Paint paint;

void ICACHE_FLASH_ATTR Display_Init(void)
{
    frame_buffer = (unsigned char*)os_malloc(EPD_WIDTH * EPD_HEIGHT / 8);
    if (EPD_Init(&epd, lut_full_update) != 0) {
        os_printf("e-Paper init failed\n");
        return;
    }

    Paint_Init(&paint, frame_buffer, epd.width, epd.height);
    Paint_Clear(&paint, UNCOLORED);

    EPD_ClearFrameMemory(&epd, 0xFF);
    EPD_DisplayFrame(&epd);
    EPD_ClearFrameMemory(&epd, 0xFF);
    EPD_DisplayFrame(&epd);

    /* EPD_or partial update */
    if (EPD_Init(&epd, lut_partial_update) != 0) {
        os_printf("e-Paper init failed\n");
        return;
    }
}

void ICACHE_FLASH_ATTR Display_Welcome(void)
{
    EPD_SetFrameMemory(&epd, IMAGE_WELCOME_DATA, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_SetFrameMemory(&epd, IMAGE_WELCOME_DATA, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
}

void ICACHE_FLASH_ATTR Display_Weather(Weather_Type* data, Config_Type* conf)
{
    EPD_SetFrameMemory(&epd, IMAGE_WEATHER_DATA, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_SetFrameMemory(&epd, IMAGE_WEATHER_DATA, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
}

void ICACHE_FLASH_ATTR Display_Github(Github_Type* data, Config_Type* conf)
{
    /* fontsize table */
    sFONT* sizeConfig[2][3] = {
        {&Font12, &Font16, &Font24},   // fontsize of repo name
        {&Font12, &Font16, &Font20}    // fontsize of watch star and fork
    }; 

    os_memcpy(frame_buffer, IMAGE_GITHUB_DATA, EPD_WIDTH * EPD_HEIGHT / 8);

    Paint_SetRotate(&paint, ROTATE_90);

    Display_Muti_Line_String(&paint, data->repo_name, 80, 5, 210, 60, sizeConfig[0][conf->font_size]);
    Display_Muti_Line_String(&paint, data->watch, 35, 81, 60, 30, sizeConfig[1][conf->font_size]);
    Display_Muti_Line_String(&paint, data->star, 130, 81, 60, 30, sizeConfig[1][conf->font_size]);
    Display_Muti_Line_String(&paint, data->fork, 225, 81, 60, 30, sizeConfig[1][conf->font_size]);

    if (strcmp(conf->style, "dark") == 0) {
        Display_Reverse(&paint);
    }

    EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
}

void ICACHE_FLASH_ATTR Display_Fund(Fund_Type* data, Config_Type* conf)
{
    EPD_SetFrameMemory(&epd, IMAGE_FUND_DATA, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_SetFrameMemory(&epd, IMAGE_FUND_DATA, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
}

void ICACHE_FLASH_ATTR Display_Reflesh(Data_Type* data, Data_Type* conf)
{
    switch (data->data_type)
    {
    case WEATHER_DATA:
        Display_Weather((Weather_Type*)data->data_content, (Config_Type*)conf->data_content);
        break;
    case GITHUB_DATA:
        Display_Github((Github_Type*)data->data_content, (Config_Type*)conf->data_content);
        break;
    case FUND_DATA:
        Display_Fund((Fund_Type*)data->data_content, (Config_Type*)conf->data_content);
        break;
    default:
        break;
    }
}

void ICACHE_FLASH_ATTR Display_Muti_Line_String(Paint* myPaint, const char* text, 
                                                int x, int y, int width, int heigh, sFONT* font)
{
    int lineCount = heigh / font->Height;
    int lineLength = width / font->Width;
    int lineNum = 0;
    int textLine = strlen(text)/lineLength+(strlen(text)%lineLength != 0);
    char str[60];
    int i = 0;

    if (DEBUG) {
        Paint_DrawFilledRectangle(&paint, x, y, x+width, y+heigh, COLORED);
    }

    /* vertical center display */
    if (textLine <= lineCount) { 
        y = y+(heigh-font->Height*textLine)/2;
    }

    /* horizontal center display */
    if (textLine == 1) { 
        x = x+(width-font->Width*strlen(text))/2;
    }

    while (*text != '\0')
    {
        str[i] = *text++;
        i++;
        if ((i >= lineLength)) { // new line
            str[i] = '\0';
            Paint_DrawStringAt(myPaint, x, y+lineNum*font->Height, str, font, DEBUG ? UNCOLORED : COLORED);
            if (lineNum+1 >= lineCount) { // overheigh
                break;
            }
            i = 0;
            lineNum++;
        }
    }
    
    /* display last line */
    str[i] = '\0';
    Paint_DrawStringAt(myPaint, x, y+lineNum*font->Height, str, font, DEBUG ? UNCOLORED : COLORED);
}

void ICACHE_FLASH_ATTR Display_Reverse(Paint* paint) 
{
    int i;
    for (i= 0; i < paint->height*paint->width/8; i++) {
        paint->image[i] ^= 0xff;
    }
}
