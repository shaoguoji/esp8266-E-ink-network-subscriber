/**
 *  @filename   :   epdif.c
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     July 7 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "bsp/epdif.h"
#include "driver/spi.h"
#include "epaper.h"
#include "osapi.h"

void EpdDigitalWriteCallback(int pin_num, int value) {
  if (value == HIGH) {
    GPIO_OUTPUT_SET(GPIO_ID_PIN(pin_num), 1); 
  } else {
    GPIO_OUTPUT_SET(GPIO_ID_PIN(pin_num), 0); 
  }
}

int EpdDigitalReadCallback(int pin_num) {
  GPIO_DIS_OUTPUT(pin_num);
  if (GPIO_INPUT_GET(pin_num) == 1) {
     return HIGH;
   } else {
     return LOW;
   } 
}

void EpdDelayMsCallback(unsigned int delaytime) {
  int times = delaytime / 65;
  int i;
  for (i = 0; i < times; i++)
  {
    os_delay_us(65000);
  }
  os_delay_us(delaytime%65*1000);
}

void EpdSpiTransferCallback(unsigned char data) {
  GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_PIN), 0);   
  Spi_Write_Byte(data);
  GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_PIN), 1);
}

int EpdInitCallback(void) {
  SPI_Init();
  
  PIN_FUNC_SELECT(EPAPER_DC_REG, EPAPER_DC_FUN);     //configure io to gpio mode
  PIN_FUNC_SELECT(EPAPER_RST_REG, EPAPER_RST_FUN);   //configure io to gpio mode
  PIN_FUNC_SELECT(EPAPER_BUSY_REG, EPAPER_BUSY_FUN); //configure io to gpio mode

  return 0;
}

