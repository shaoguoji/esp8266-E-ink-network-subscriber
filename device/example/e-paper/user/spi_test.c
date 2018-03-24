/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2015 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "driver/spi.h"
#include "eagle_soc.h"
#include "osapi.h"

// Test spi master interfaces.
void ICACHE_FLASH_ATTR spi_master_test(void)
{
    // Init HSPI GPIO
    WRITE_PERI_REG(PERIPHS_IO_MUX, 0x105);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, 2);//configure io to spi mode
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, 2);//configure io to spi mode
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, 2);//configure io to spi mode
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, 2);//configure io to spi mode

    spi_master_init(HSPI);
    uint8_t sendData = 0x66;

    os_printf("\r\n =============   spi init master   ============= \r\n");

//  Test 8266 slave.Communication format: 1byte command + 1bytes address + x bytes Data.
    os_printf("\r\n Master send 8 bytes data...\r\n");
    spi_mast_byte_write(HSPI, sendData);
}

void ICACHE_FLASH_ATTR EpdSpiTransferCallback(void)
{
    spi_mast_byte_write(HSPI, 0x66);
}

void spi_interface_test(void)
{
    // Test spi interfaces.
    os_printf("\r\n =======================================================\r\n");
    os_printf("\t ESP8266 %s application \n\r", __func__);
    os_printf("\t\t SDK version:%s    \n\r", system_get_sdk_version());
    os_printf("\t\t Complie time:%s  \n\r", __TIME__);
    os_printf("\r\n =======================================================\r\n");

    spi_master_test();
}
