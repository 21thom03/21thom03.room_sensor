#include "main_config.h"

/**********************************
 * Functions declarations
**********************************/
const char *TAG = "Wi-Fi";

esp_err_t Wifi_connection(void);
esp_err_t Wifi_disconnection(void);

/**********************************
 * Functions code
**********************************/
esp_err_t Wifi_connection(void)
{
    wifi_init_config_t wifi_config;

    // wifi_config = WIFI_INIT_CONFIG_DEFAULT;

    esp_err_t ret = esp_wifi_init(&wifi_config);
    ret = esp_wifi_set_mode(WIFI_MODE_STA);
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

esp_err_t Wifi_disconnection(void)
{
    esp_err_t ret = esp_wifi_stop();
    ret = esp_wifi_restore();
    ret = esp_wifi_disconnect();
    if(!ret)
    ESP_LOGE(TAG, "Wi-Fi disconnection failed");
    else
    ESP_LOGI(TAG, "Wi-Fi disconnect with success");
    return ret;
}