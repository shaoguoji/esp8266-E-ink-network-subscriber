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
#include <stdio.h>
#include <stdarg.h>

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
    Paint_Clear(&paint, UNCOLORED);
    os_memcpy(frame_buffer, IMAGE_WELCOME_DATA, 1520);
    Paint_SetRotate(&paint, ROTATE_90);

    Paint_DrawStringAt(&paint, 115, 25, "--- Welcome ---", &Font16, COLORED);
    Paint_DrawStringAt(&paint, 175, 60, "ESP8266", &Font24, COLORED);
    Paint_DrawStringAt(&paint, 98, 87, "Network Subscriber", &Font16, COLORED);
    Paint_DrawStringAt(&paint, 198, 115, "--By shaoguoji", &Font12, COLORED);

    EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
}

void ICACHE_FLASH_ATTR Display_Weather(Weather_Type* data, Config_Type* conf)
{
    /* fontsize table */
    sFONT* sizeConfig[][3] = {
        {&Font12, &Font16, &Font24},   // fontsize of city
        {&Font8, &Font12, &Font16},    // fontsize of tempertrue and air
        {&Font12, &Font16, &Font16}    // fontsize data and title
    }; 

    Paint_Clear(&paint, UNCOLORED);

    /* copy icon data */
    os_memcpy(frame_buffer, IMAGE_WEATHER_DATA, 1120);
    Paint_SetRotate(&paint, ROTATE_90);
    /* border line */
    Display_HorizontalLine(&paint, 0, 1, 296, 4);
    Display_HorizontalLine(&paint, 0, 125, 296, 4);
    Display_VerticalLine(&paint, 0, 0, 128, 4);
    Display_VerticalLine(&paint, 292, 0, 128, 4);
    /* table line */
    Display_HorizontalLine(&paint, 72, 38, 224, 2);
    Display_HorizontalLine(&paint, 0, 64, 296, 2);
    Display_HorizontalLine(&paint, 0, 95, 296, 2);
    Display_VerticalLine(&paint, 72, 1, 128, 2);
    Display_VerticalLine(&paint, 183, 38, 91, 2);
    /* title text */
    Display_Muti_Line_String(&paint, "TEMP", 5, 67, 65, 25, sizeConfig[2][conf->font_size]);
    Display_Muti_Line_String(&paint, "AIR", 5, 98, 65, 25, sizeConfig[2][conf->font_size]);
    /* data text */
    Display_Muti_Line_String(&paint, data->city, 76, 6, 210, 30, sizeConfig[0][conf->font_size]);
    Display_Muti_Line_String(&paint, data->today_date, 76, 40, 105, 25, sizeConfig[2][conf->font_size]);
    Display_Muti_Line_String(&paint, data->tomorrow_date, 187, 40, 102, 25, sizeConfig[2][conf->font_size]);
    Display_Muti_Line_String(&paint, data->today_temp, 76, 68, 105, 25, sizeConfig[1][conf->font_size]);
    Display_Muti_Line_String(&paint, data->tomorrow_temp, 187, 68, 102, 25, sizeConfig[1][conf->font_size]);
    Display_Muti_Line_String(&paint, data->today_air, 76, 98, 105, 25, sizeConfig[1][conf->font_size]);
    Display_Muti_Line_String(&paint, data->tomorrow_air, 187, 98, 102, 25, sizeConfig[1][conf->font_size]);

    if (strcmp(conf->style, "dark") == 0) {
        Display_Reverse(&paint);
    }

    EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
}

void ICACHE_FLASH_ATTR Display_Github(Github_Type* data, Config_Type* conf)
{
    /* fontsize table */
    sFONT* sizeConfig[][3] = {
        {&Font12, &Font16, &Font24},   // fontsize of repo name
        {&Font12, &Font16, &Font20}    // fontsize of watch star and fork
    }; 

    Paint_Clear(&paint, UNCOLORED);

    /* copy icon data */
    os_memcpy(frame_buffer, IMAGE_GITHUB_DATA, EPD_WIDTH * EPD_HEIGHT / 8);
    Paint_SetRotate(&paint, ROTATE_90);
    /* data text */
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
    /* fontsize table */
    sFONT* sizeConfig[][3] = {
        {&Font16, &Font20, &Font24},   // fontsize of worth
        {&Font8, &Font12, &Font16},    // fontsize of date and num
        {&Font12, &Font16, &Font16}     // fontsize of limit and tile
    }; 
    
    Paint_Clear(&paint, UNCOLORED);
    
    /* copy icon data */
    os_memcpy(frame_buffer, IMAGE_FUND_DATA, 1120);
    Paint_SetRotate(&paint, ROTATE_90);
    /* border line */
    Display_HorizontalLine(&paint, 0, 1, 296, 4);
    Display_HorizontalLine(&paint, 0, 125, 296, 4);
    Display_VerticalLine(&paint, 0, 0, 128, 4);
    Display_VerticalLine(&paint, 292, 0, 128, 4);
    /* table line */
    Display_HorizontalLine(&paint, 72, 38, 224, 2);
    Display_HorizontalLine(&paint, 0, 64, 73, 2);
    Display_HorizontalLine(&paint, 0, 95, 296, 2);
    Display_VerticalLine(&paint, 72, 1, 128, 2);
    Display_VerticalLine(&paint, 183, 1, 128, 2);
    /* title text */
    Display_Muti_Line_String(&paint, "Net Worth", 75, 6, 105, 30, sizeConfig[2][conf->font_size]);
    Display_Muti_Line_String(&paint, "Forecast", 187, 6, 105, 30, sizeConfig[2][conf->font_size]);
    Display_Muti_Line_String(&paint, "Date", 5, 98, 65, 25, sizeConfig[2][conf->font_size]);
    /* data text */
    Display_Muti_Line_String(&paint, data->num, 4, 67, 66, 25, sizeConfig[2][conf->font_size]);
    Display_Muti_Line_String(&paint, data->now_worth, 75, 40, 105, 35, sizeConfig[0][conf->font_size]);
    Display_Muti_Line_String(&paint, data->forecast_worth, 187, 40, 105, 35, sizeConfig[0][conf->font_size]);
    Display_Muti_Line_String(&paint, data->now_limit, 105, 75, 80, 20, sizeConfig[2][conf->font_size]);
    Display_Muti_Line_String(&paint, data->forecast_limit, 217, 75, 80, 20, sizeConfig[2][conf->font_size]);    
    Display_Muti_Line_String(&paint, data->now_datetime, 73, 95, 110, 30, sizeConfig[1][conf->font_size]);
    Display_Muti_Line_String(&paint, data->forecast_datetime, 185, 95, 110, 30, sizeConfig[1][conf->font_size]);

    EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, epd.width, epd.height);
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

void ICACHE_FLASH_ATTR Display_HorizontalLine(Paint* paint, int x, int y, int line_width, int size)
{
    int i;
    for (i = 0; i < size; i++) {
       Paint_DrawHorizontalLine(paint, x, y+i, line_width, COLORED);     
    }
}

void ICACHE_FLASH_ATTR Display_VerticalLine(Paint* paint, int x, int y, int line_height, int size)
{
    int i;
    for (i = 0; i < size; i++) {
       Paint_DrawVerticalLine(paint, x+i, y, line_height, COLORED);     
    }
}

void ICACHE_FLASH_ATTR Display_Reverse(Paint* paint) 
{
    int i;
    for (i= 0; i < paint->height*paint->width/8; i++) {
        paint->image[i] ^= 0xff;
    }
}

void ICACHE_FLASH_ATTR Display_Log(char *text)
{
    static int lineNum = 0;
    char str[500];
    int i = 0;
    
    while (*text != '\0')
    {
        str[i] = *text++;
        i++;
        if (lineNum == 0) {
            os_memset(frame_buffer, 0x00, 1520);
        }
        if ((i >= 18)) { // new line
            str[i] = '\0';
            Paint_DrawStringAt(&paint, 3, 5+lineNum*8, str, &Font8, UNCOLORED);
            i = 0;
            lineNum = (lineNum+1) % 15;
        }
    }

    /* display last line */
    str[i] = '\0';
    Paint_DrawStringAt(&paint, 3, 5+lineNum*8, str, &Font8, UNCOLORED);
    lineNum = (lineNum+1) % 15;

    EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
}

void ICACHE_FLASH_ATTR EPAPER_LOG(char *format, ...)
{
    char buf[500] = {0};
    va_list p;

    va_start(p, format);
    ets_vsprintf(buf, format, p);
    va_end(p);

    Display_Log(buf);
}
