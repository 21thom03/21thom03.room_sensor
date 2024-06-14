#include "main_config.h"

#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1
const char *TAG = "WiFi station";

static EventGroupHandle_t s_wifi_event_group;

typedef struct {
    char ssid[32];
    char pass[64];
} wifi_id_t;

/**********************************
 * Functions code
**********************************/

esp_err_t wifi_init_sta(void)
{

    esp_err_t ret;
    wifi_config_t wifi_config;
    memset(&wifi_config, 0, sizeof(wifi_config));

    strncpy((char*)wifi_config.sta.ssid, WIFI_SSID, 32);
    strncpy((char*)wifi_config.sta.password, WIFI_PASSWORD, 64);

    // s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    ESP_LOGI(TAG, "Wi-Fi configurate in station mode succefully");
    ret = esp_wifi_start();

    // EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
    //     WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
    //     pdFALSE,
    //     pdFALSE,
    //     portMAX_DELAY);
    

    int i_connect = 1;
    do
    {
        ret = esp_wifi_connect();
        if(ret != ESP_OK)
        ESP_LOGE(TAG, "Connection failed, error : %s", esp_err_to_name(ret));
        i_connect++;
    } while ((ret != ESP_OK) && (i_connect < 5));
    
    if(ret == ESP_OK)
    {
        ESP_LOGI(TAG, "Wi-Fi connect with success, to SSID %s and PASSWORD: %s", WIFI_SSID, WIFI_PASSWORD);
        return ESP_OK;
    } else {
        ESP_LOGE(TAG, "Wi-Fi connection failed, to SSID: %s and PASSSWORD: %s, error message : %s",WIFI_SSID, WIFI_PASSWORD, esp_err_to_name(ret));
        return ret;
    }
}

esp_err_t wifi_disconnect(void)
{
    esp_err_t ret =  esp_wifi_disconnect();
    if(ret == ESP_OK)
    ESP_LOGI(TAG, "Wi-Fi disconnect with success");
    else
    ESP_LOGE(TAG, "Wi-Fi disconnection failed");

    esp_wifi_stop();
    esp_wifi_deinit();
    // vEventGroupDelete(s_wifi_event_group);
    return ret;
}