#ifndef __SPI_H__
#define __SPI_H__

#include "os_type.h"

#include "main.h"

//*****************************************************************************
//
// Make sure all of the definitions in this header have a C binding.
//
//*****************************************************************************

#ifdef __cplusplus
extern "C"
{
#endif

#define     SPI_MOSI_PIN        EPAPER_DIN_PIN
#define     SPI_CLK_PIN          EPAPER_CLK_PIN
#define     SPI_CS_PIN            EPAPER_CS_PIN

#define     SPI_MOSI_PIN_REG    EPAPER_DIN_REG 
#define     SPI_CLK_PIN_REG      EPAPER_CLK_REG 
#define     SPI_CS_PIN_REG        EPAPER_CS_REG 

#define     SPI_MOSI_PIN_FUN    EPAPER_DIN_FUN 
#define     SPI_CLK_PIN_FUN      EPAPER_CLK_FUN 
#define     SPI_CS_PIN_FUN        EPAPER_CS_FUN 

void SPI_Init(void);
void Spi_Write_Byte(uint8_t data);

#ifdef __cplusplus
}
#endif

#endif //  __SPI_H__