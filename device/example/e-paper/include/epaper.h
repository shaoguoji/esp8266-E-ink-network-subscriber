#ifndef __EPAPER_H__
#define __EPAPER_H__

#include "os_type.h"

#define EPAPER_DIN_PIN      14
#define EPAPER_CLK_PIN      12
#define EPAPER_CS_PIN       13
#define EPAPER_DC_PIN       15
#define EPAPER_RST_PIN      3
#define EPAPER_BUSY_PIN     5

#define EPAPER_DC_REG       PERIPHS_IO_MUX_MTDO_U
#define EPAPER_RST_REG      PERIPHS_IO_MUX_U0RXD_U
#define EPAPER_BUSY_REG     PERIPHS_IO_MUX_GPIO5_U

#define EPAPER_DC_FUN       FUNC_GPIO15
#define EPAPER_RST_FUN      FUNC_GPIO3
#define EPAPER_BUSY_FUN     FUNC_GPIO5

#endif /* #ifndef __EPAPER_H__ */