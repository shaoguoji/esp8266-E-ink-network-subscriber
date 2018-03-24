#ifndef __SPI_H__
#define __SPI_H__

#include "os_type.h"
#include "gpio.h"

//*****************************************************************************
//
// Make sure all of the definitions in this header have a C binding.
//
//*****************************************************************************

#ifdef __cplusplus
extern "C"
{
#endif

#define 	SPI_MISO_PIN	12
#define 	SPI_MOSI_PIN	13
#define 	SPI_CLK_PIN		14
#define 	SPI_CS_PIN		15

#define 	SPI_MISO_PIN_REG	PERIPHS_IO_MUX_MTDI_U
#define 	SPI_MOSI_PIN_REG	PERIPHS_IO_MUX_MTDO_U
#define 	SPI_CLK_PIN_REG		PERIPHS_IO_MUX_MTCK_U
#define 	SPI_CS_PIN_REG		PERIPHS_IO_MUX_MTMS_U

#define 	SPI_MISO_PIN_FUN	FUNC_GPIO12
#define 	SPI_MOSI_PIN_FUN	FUNC_GPIO13
#define 	SPI_CLK_PIN_FUN		FUNC_GPIO14
#define 	SPI_CS_PIN_FUN		FUNC_GPIO15

void SPI_Init(void);
void Spi_Write_Byte(uint8_t data);

#ifdef __cplusplus
}
#endif

#endif //  __SPI_H__
