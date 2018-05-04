#include "data_handle.h"
#include "cJSON.h"
#include "display.h"
#include "mem.h"
#include <string.h>

#include "osapi.h"

void ICACHE_FLASH_ATTR data_parse(char *data_str, Data_Type* data) 
{
    char *out = NULL;
    cJSON *json = NULL;
    cJSON *weather = NULL;
    cJSON *info = NULL; 

    if (data_str == NULL) {
        return;
    }

    json = cJSON_Parse(data_str);

    if (!json) { 
        os_printf("Error before: [%s]\n",cJSON_GetErrorPtr());
        return;
    } else {
        data->device_id = os_malloc(strlen(cJSON_GetObjectItem(json, "device_id")->valuestring)+1);
        strcpy(data->device_id, cJSON_GetObjectItem(json, "device_id")->valuestring);
        data->data_type = cJSON_GetObjectItem(json, "data_type")->valueint;

        switch (data->data_type)
        {
        case CONFIG_DATA:
            data->data_content = os_malloc(sizeof(Config_Type));

            strcpy((*(Config_Type*)(data->data_content)).style, cJSON_GetObjectItem(cJSON_GetObjectItem(json, "data_content"), "style")->valuestring);
            (*(Config_Type*)(data->data_content)).font_size = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "data_content"), "font_size")->valueint;
            break;
        case WEATHER_DATA:
            data->data_content = os_malloc(sizeof(Weather_Type));
            weather = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "data_content"), "weather");

            strcpy((*(Weather_Type*)(data->data_content)).city, cJSON_GetObjectItem(cJSON_GetObjectItem(json, "data_content"), "city")->valuestring);
            strcpy((*(Weather_Type*)(data->data_content)).today_date, cJSON_GetObjectItem(cJSON_GetArrayItem(weather, 0), "date")->valuestring);
            strcpy((*(Weather_Type*)(data->data_content)).today_temp, cJSON_GetObjectItem(cJSON_GetArrayItem(weather, 0), "temp")->valuestring);
            strcpy((*(Weather_Type*)(data->data_content)).today_air, cJSON_GetObjectItem(cJSON_GetArrayItem(weather, 0), "air")->valuestring);
            strcpy((*(Weather_Type*)(data->data_content)).tomorrow_date, cJSON_GetObjectItem(cJSON_GetArrayItem(weather, 1), "date")->valuestring);
            strcpy((*(Weather_Type*)(data->data_content)).tomorrow_temp, cJSON_GetObjectItem(cJSON_GetArrayItem(weather, 1), "temp")->valuestring);
            strcpy((*(Weather_Type*)(data->data_content)).tomorrow_air, cJSON_GetObjectItem(cJSON_GetArrayItem(weather, 1), "air")->valuestring);

            break;
        case GITHUB_DATA:
            data->data_content = os_malloc(sizeof(Github_Type));
            strcpy((*(Github_Type*)(data->data_content)).repo_name, cJSON_GetObjectItem(cJSON_GetObjectItem(json, "data_content"), "repo_name")->valuestring);
            strcpy((*(Github_Type*)(data->data_content)).watch, cJSON_GetObjectItem(cJSON_GetObjectItem(json, "data_content"), "watch")->valuestring);
            strcpy((*(Github_Type*)(data->data_content)).star, cJSON_GetObjectItem(cJSON_GetObjectItem(json, "data_content"), "star")->valuestring);
            strcpy((*(Github_Type*)(data->data_content)).fork, cJSON_GetObjectItem(cJSON_GetObjectItem(json, "data_content"), "fork")->valuestring);

            break;
        case FUND_DATA:
            data->data_content = os_malloc(sizeof(Fund_Type));
            info = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "data_content"), "info");

            strcpy((*(Fund_Type*)(data->data_content)).num, cJSON_GetObjectItem(cJSON_GetObjectItem(json, "data_content"), "num")->valuestring);
            strcpy((*(Fund_Type*)(data->data_content)).now_worth, cJSON_GetObjectItem(cJSON_GetArrayItem(info, 0), "worth")->valuestring);
            strcpy((*(Fund_Type*)(data->data_content)).now_limit, cJSON_GetObjectItem(cJSON_GetArrayItem(info, 0), "limit")->valuestring);
            strcpy((*(Fund_Type*)(data->data_content)).now_datetime, cJSON_GetObjectItem(cJSON_GetArrayItem(info, 0), "datetime")->valuestring);
            strcpy((*(Fund_Type*)(data->data_content)).forecast_worth, cJSON_GetObjectItem(cJSON_GetArrayItem(info, 1), "worth")->valuestring);
            strcpy((*(Fund_Type*)(data->data_content)).forecast_limit, cJSON_GetObjectItem(cJSON_GetArrayItem(info, 1), "limit")->valuestring);
            strcpy((*(Fund_Type*)(data->data_content)).forecast_datetime, cJSON_GetObjectItem(cJSON_GetArrayItem(info, 1), "datetime")->valuestring);

            break;
        default:
            break;
        }

		out=cJSON_Print(json);
		cJSON_Delete(json);
        os_printf("\nparse json data:\n");
		os_printf("%s\n",out);
	}
}

void ICACHE_FLASH_ATTR config_parse(char *conf_str, Data_Type* conf)
{
    char *out = NULL;
    cJSON *json = NULL;

    if (conf_str == NULL) {
        return;
    }

    data_parse(conf_str, conf);
}

