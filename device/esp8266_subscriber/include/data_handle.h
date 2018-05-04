#ifndef __DATA_HANDLE_H__
#define __DATA_HANDLE_H__

#define CONFIG_DATA       0
#define WEATHER_DATA    1
#define GITHUB_DATA     2
#define FUND_DATA       3

#define DISPLAY_CONFIG  1

typedef struct {
    char *device_id;
    int data_type;
    void *data_content;
} Data_Type;


void data_parse(char *data_str, Data_Type* data);
void config_parse(char *conf_str, Data_Type* conf);

#endif /* #ifndef __DATA_HANDLE_H__ */