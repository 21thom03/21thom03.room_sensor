#include "main_config.h"

const char *TAG = "Wi-Fi";

typedef struct {
    char ssid[32];
    char pass[64];
} wifi_id_t;

/**********************************
 * Functions code
**********************************/
esp_err_t esp_Wifi_connection(void)
{

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    wifi_config_t wifi_config;
    memset(&wifi_config, 0, sizeof(wifi_config));

    strncpy((char*)wifi_config.sta.ssid, WIFI_SSID, 32);
    strncpy((char*)wifi_config.sta.password, WIFI_PASSWORD, 64);

    esp_err_t ret = esp_netif_init();
    ret = esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    ret = esp_wifi_init(&wifi_init_config);
    ret = esp_wifi_set_config(WIFI_MODE_STA, &wifi_config);
    if(!ret)
    {
        ESP_LOGE(TAG, "Error : %d, error message : %s", ret, esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "Wi-Fi configurate in station mode succefully");
    ret = esp_wifi_start();
    ret = esp_wifi_connect();
    if(!ret)
    {
        ESP_LOGE(TAG, "Wi-Fi connection failed, error message : %s", esp_err_to_name(ret));
        return ret;
    }
        ESP_LOGI(TAG, "Wi-Fi connect with success");
        return ESP_OK;
}

esp_err_t esp_Wifi_disconnection(void)
{
    esp_err_t ret =  esp_wifi_disconnect();
    if(!ret)
    ESP_LOGE(TAG, "Wi-Fi disconnection failed");
    else
    ESP_LOGI(TAG, "Wi-Fi disconnect with success");

    esp_wifi_stop();
    esp_wifi_deinit();
    return ret;
}