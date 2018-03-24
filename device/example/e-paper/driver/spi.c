#include "driver/spi.h"

// Test spi master interfaces.
void ICACHE_FLASH_ATTR SPI_Init(void)
{
    // PIN_FUNC_SELECT(SPI_MISO_PIN_REG, SPI_MISO_PIN_FUN); //configure io to gpio mode
    PIN_FUNC_SELECT(SPI_MOSI_PIN_REG, SPI_MOSI_PIN_FUN); //configure io to gpio mode
    PIN_FUNC_SELECT(SPI_CLK_PIN_REG, SPI_CLK_PIN_FUN);   //configure io to gpio mode
    PIN_FUNC_SELECT(SPI_CS_PIN_REG, SPI_CS_PIN_FUN);     //configure io to gpio mode
}

void ICACHE_FLASH_ATTR Spi_Write_Byte(uint8_t data)
{
    u8 i;
	
	for(i = 0; i < 8; i++)
	{
		/* 写功能，上升沿写入数据 */
		GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CLK_PIN), 0);   //拉低时钟线
		if(data & 0x80)    //数据发送顺序为先高
		{
			GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_MOSI_PIN), 1);   //写1
		}
		else
		{
			GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_MOSI_PIN), 0);   //写0
		}
		GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CLK_PIN), 1);    //拉高时钟线
		data <<= 1;
	}
	
	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CLK_PIN), 0);        //拉低时钟线
}
