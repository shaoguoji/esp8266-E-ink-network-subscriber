#ifndef __SPI_H__
#define __SPI_H__

#include "os_type.h"
#include "gpio.h"

#include "epaper.h"

//*****************************************************************************
//
// Make sure all of the definitions in this header have a C binding.
//
//*****************************************************************************

#ifdef __cplusplus
extern "C"
{
#endif

// #define 	SPI_MISO_PIN	12
#define 	SPI_MOSI_PIN	EPAPER_DIN_PIN
#define 	SPI_CLK_PIN		EPAPER_CLK_PIN
#define 	SPI_CS_PIN		EPAPER_CS_PIN

// #define 	SPI_MISO_PIN_REG	PERIPHS_IO_MUX_MTDI_U
#define 	SPI_MOSI_PIN_REG	PERIPHS_IO_MUX_MTMS_U // PERIPHS_IO_MUX_MTDO_U
#define 	SPI_CLK_PIN_REG		PERIPHS_IO_MUX_MTDI_U // PERIPHS_IO_MUX_MTCK_U
#define 	SPI_CS_PIN_REG		PERIPHS_IO_MUX_MTCK_U // PERIPHS_IO_MUX_MTMS_U

// #define 	SPI_MISO_PIN_FUN	FUNC_GPIO12
#define 	SPI_MOSI_PIN_FUN	FUNC_GPIO13
#define 	SPI_CLK_PIN_FUN		FUNC_GPIO14
#define 	SPI_CS_PIN_FUN		FUNC_GPIO15

void SPI_Init(void);
void Spi_Write_Byte(uint8_t data);

#ifdef __cplusplus
}
#endif

#endif //  __SPI_H__
