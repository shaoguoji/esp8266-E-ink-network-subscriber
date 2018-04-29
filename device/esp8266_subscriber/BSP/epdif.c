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

#include "epdif.h"
#include "main.h"
#include "spi.h"
#include "osapi.h"

// extern SPI_HandleTypeDef hspi1;

EPD_Pin epd_cs_pin = {
  // SPI_CS_GPIO_Port,
  EPAPER_CS_PIN,
};

EPD_Pin epd_dc_pin = {
  // RST_GPIO_Port,
  EPAPER_DC_PIN,
};

EPD_Pin epd_rst_pin = {
  // DC_GPIO_Port,
  EPAPER_RST_PIN,
};

EPD_Pin epd_busy_pin = {
  // BUSY_GPIO_Port,
  EPAPER_BUSY_PIN,
};

EPD_Pin pins[4];

void EpdDigitalWriteCallback(int pin_num, int value) {
  if (value == HIGH) {
    GPIO_OUTPUT_SET(GPIO_ID_PIN(pins[pin_num].pin), HIGH); 
  } else {
    GPIO_OUTPUT_SET(GPIO_ID_PIN(pins[pin_num].pin), LOW); 
  }
}

int EpdDigitalReadCallback(int pin_num) {
  GPIO_DIS_OUTPUT(pins[pin_num].pin);
  if (GPIO_INPUT_GET(pins[pin_num].pin) == HIGH) {
     return HIGH;
   } else {
     return LOW;
   } 
}

void EpdDelayMsCallback(unsigned int delaytime) {
  int count = delaytime / 65;
  int i;
  for (i = 0; i < count; i++)
  {
    os_delay_us(65000);
  }
  os_delay_us(delaytime%65*1000);
}

void EpdSpiTransferCallback(unsigned char data) {
  GPIO_OUTPUT_SET(GPIO_ID_PIN(pins[CS_PIN].pin), LOW);   
  Spi_Write_Byte(data);
  GPIO_OUTPUT_SET(GPIO_ID_PIN(pins[CS_PIN].pin), HIGH);
}

int EpdInitCallback(void) {
  pins[CS_PIN] = epd_cs_pin;
  pins[RST_PIN] = epd_rst_pin;
  pins[DC_PIN] = epd_dc_pin;
  pins[BUSY_PIN] = epd_busy_pin;

  SPI_Init();

  PIN_FUNC_SELECT(EPAPER_DC_REG, EPAPER_DC_FUN);     //configure io to gpio mode
  PIN_FUNC_SELECT(EPAPER_RST_REG, EPAPER_RST_FUN);   //configure io to gpio mode
  PIN_FUNC_SELECT(EPAPER_BUSY_REG, EPAPER_BUSY_FUN); //configure io to gpio mode

  return 0;
}

