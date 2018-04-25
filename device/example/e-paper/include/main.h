#ifndef __MAIN_H__
#define __MAIN_H__

#include "os_type.h"
#include "gpio.h"

#define EPAPER_DIN_PIN      14
#define EPAPER_CLK_PIN      12
#define EPAPER_CS_PIN        13
#define EPAPER_DC_PIN        15
#define EPAPER_RST_PIN      3
#define EPAPER_BUSY_PIN    5

#define EPAPER_DIN_REG      PERIPHS_IO_MUX_MTMS_U  
#define EPAPER_CLK_REG      PERIPHS_IO_MUX_MTDI_U
#define EPAPER_CS_REG        PERIPHS_IO_MUX_MTCK_U
#define EPAPER_DC_REG        PERIPHS_IO_MUX_MTDO_U
#define EPAPER_RST_REG      PERIPHS_IO_MUX_U0RXD_U
#define EPAPER_BUSY_REG    PERIPHS_IO_MUX_GPIO5_U

#define EPAPER_DIN_FUN      FUNC_GPIO14
#define EPAPER_CLK_FUN      FUNC_GPIO12
#define EPAPER_CS_FUN        FUNC_GPIO13
#define EPAPER_DC_FUN        FUNC_GPIO15
#define EPAPER_RST_FUN      FUNC_GPIO3
#define EPAPER_BUSY_FUN    FUNC_GPIO5

#endif /* #ifndef __MAIN_H__ */