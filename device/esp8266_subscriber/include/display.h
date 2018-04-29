#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "fonts.h"
#include "epdpaint.h"

#define COLORED      0
#define UNCOLORED    1

typedef struct {
  char repoName[100];
  char watch[20];
  char star[20];
  char fork[20];
} Github_Type;

void Display_Init(void);
void Display_Welcome(void);
void Display_Weather(void);
void Display_Github(Github_Type* data);
void Display_Fund(void);
void Display_Muti_Line_String(Paint* paint, const char* text, int x, int y, int width, int heigh, sFONT* font);
void Display_Reverse(Paint* paint);


#endif /* #ifndef __DISPLAY_H__ */
