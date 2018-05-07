/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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

#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"

#include "display.h"
#include "data_handle.h"

#include "mqtt.h"
#include "wifi.h"
#include "config.h"
#include "debug.h"
#include "mem.h"

#include <string.h>
#include <stdio.h>

#define DEVICE_ID   "D446"

MQTT_Client mqttClient;
Data_Type sub_data;
Data_Type sub_config;

void wifiConnectCb(uint8_t status)
{
	if(status == STATION_GOT_IP){
		MQTT_Connect(&mqttClient);
	} else {
		MQTT_Disconnect(&mqttClient);
	}
}
void mqttConnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Connected\r\n");
	MQTT_Subscribe(client, "data", 2);
	MQTT_Subscribe(client, "config", 2);
	//MQTT_Subscribe(client, "/mqtt/topic/2", 2);

    char *data_text = "{\"device_id\":\"D446\",\"data_type\":2,\"data_content\":{\"repo_name\":\"----------\",\"watch\":\"---\",\"star\":\"---\",\"fork\":\"---\"}}";
    char *config_text = "{\"device_id\":\"D446\",\"data_type\":0,\"data_content\":{\"style\":\"light\",\"font_size\":1}}";

	MQTT_Publish(client, "data", data_text, strlen(data_text), 0, 0);
    MQTT_Publish(client, "config", config_text, strlen(config_text), 0, 0);
	//MQTT_Publish(client, "/mqtt/topic/1", "hello1", 6, 1, 0);
	//MQTT_Publish(client, "/mqtt/topic/2", "hello2", 6, 2, 0);

}

void mqttDisconnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Disconnected\r\n");
}

void mqttPublishedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Published\r\n");
}

void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
	char *topicBuf = (char*)os_zalloc(topic_len+1),
			*dataBuf = (char*)os_zalloc(data_len+1);

	MQTT_Client* client = (MQTT_Client*)args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	INFO("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);

    if (strcmp(topicBuf, "data") == 0) {
        data_parse(dataBuf, &sub_data);
    } else if (strcmp(topicBuf, "config") == 0) {
        config_parse(dataBuf, &sub_config);
    }
    
    if ((strcmp(sub_data.device_id, DEVICE_ID) == 0) && (sub_data.data_content != NULL) && (sub_config.data_content != NULL)) {
        Display_Reflesh(&sub_data, &sub_config);
    }
	
	os_free(topicBuf);
	os_free(dataBuf);
}

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR time_tick(void)
{
    // time_now_s = (system_get_time()/1000 - time_start_ms) / 1000;
    // time_string[0] = time_now_s / 60 / 10 + '0';
    // time_string[1] = time_now_s / 60 % 10 + '0';
    // time_string[3] = time_now_s % 60 / 10 + '0';
    // time_string[4] = time_now_s % 60 % 10 + '0';

    // Paint_SetWidth(&paint, 32);
    // Paint_SetHeight(&paint, 96);
    // Paint_SetRotate(&paint, ROTATE_90);

    // Paint_Clear(&paint, UNCOLORED);
    // Paint_DrawStringAt(&paint, 0, 4, time_string, &Font12, COLORED);
    // EPD_SetFrameMemory(&epd, frame_buffer, 80, 72, Paint_GetWidth(&paint), Paint_GetHeight(&paint));
    // EPD_DisplayFrame(&epd);
}

void ICACHE_FLASH_ATTR
user_rf_pre_init(void)
{
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
user_init(void)
{
    os_printf("SDK version:%s\n", system_get_sdk_version());
    
    Display_Init();
    Display_Welcome();

	CFG_Load();

	MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
	MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);

	MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);

	WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);

	INFO("\r\nSystem started ...\r\n");

    // EPD_DelayMs(&epd, 2000);
    // Display_Weather();
    // EPD_DelayMs(&epd, 2000);
    // Display_Github(&githubData);
    // EPD_DelayMs(&epd, 2000);
    // Display_Fund();
    
    // char *data_text = "";
    // //char *data_text = "{\"device_id\":\"D446\",\"data_type\":2,\"data_content\":{\"repo_name\":\"alibaba/AliOS-Things\",\"watch\":\"222\",\"star\":\"1147\",\"fork\":\"410\"}}";
    // //char *data_text = "";
    // char *config_text = "{\"device_id\":\"D446\",\"data_type\":0,\"data_content\":{\"style\":\"light\",\"font_size\":1}}";

    // Weather_Type wdata = {
    //     "Guangzhou", 
    //     "20180504", "21.0-25.0C", "PM25:63.0", 
    //     "TOMORROW", "22.0-27.0C", "aqi:77.0"
    // };

    // Github_Type gdata = {
    //     "alibaba/AliOS-Things", 
    //     "222", "1147", "410"
    // };

    // Fund_Type fdata = {
    //     "164906", 
    //     "1.3350", "-1.84%", "2018-04-25",
    //     "1.3454", "+0.78%", "2018-04-27"
    // };

    // data_parse(data_text, &data);
    // config_parse(config_text, &config);

    // Display_Welcome();
    // Display_Weather(&wdata, (Config_Type*)config.data_content);
    // Display_Fund(&fdata, (Config_Type*)config.data_content);
    // Display_Github(&gdata, (Config_Type*)config.data_content);

    // if (strcmp(data.device_id, DEVICE_ID) == 0) {
    //     Display_Reflesh(&data, &config);
    // }
    
    // time_start_ms = system_get_time()/1000;

    // os_timer_disarm(&count_timer);
    // os_timer_setfn(&count_timer, (os_timer_func_t *)time_tick, NULL);
    // os_timer_arm(&count_timer, 500, 1);
}

