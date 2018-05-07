#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "fonts.h"
#include "epdpaint.h"
#include "data_handle.h"

#include <stdarg.h>

#define COLORED      0
#define UNCOLORED    1

typedef struct {
  char city[50];
  char today_date[20];
  char today_temp[20];
  char today_air[20];
  char tomorrow_date[20];
  char tomorrow_temp[20];
  char tomorrow_air[20];
} Weather_Type;

typedef struct {
  char repo_name[100];
  char watch[20];
  char star[20];
  char fork[20];
} Github_Type;

typedef struct {
  char num[10];
  char now_worth[20];
  char now_limit[20];
  char now_datetime[20];
  char forecast_worth[20];
  char forecast_limit[20];
  char forecast_datetime[20];
} Fund_Type;

typedef struct {
  char style[10];
  int font_size;
} Config_Type;

int ets_vsprintf(char *str, const char *format, va_list argptr);

void Display_Init(void);
void Display_Welcome(void);
void Display_Weather(Weather_Type* data, Config_Type* conf);
void Display_Github(Github_Type* data, Config_Type* conf);
void Display_Fund(Fund_Type* data, Config_Type* conf);
void Display_Muti_Line_String(Paint* paint, const char* text, int x, int y, int width, int heigh, sFONT* font);
void Display_Reverse(Paint* paint);

void Display_Reflesh(Data_Type* data, Data_Type* conf);
void Display_HorizontalLine(Paint* paint, int x, int y, int line_width, int size);
void Display_VerticalLine(Paint* paint, int x, int y, int line_height, int size);

void Display_Log(char *str);
void EPAPER_LOG(char *format, ...);

#endif /* #ifndef __DISPLAY_H__ */
