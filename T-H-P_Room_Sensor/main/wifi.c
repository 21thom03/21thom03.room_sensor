#include "main_config.h"

/**********************************
 * Constante
**********************************/
#define WIFI_CONNECTED_BIT      BIT0
#define WIFI_FAILED_BIT         BIT1
#define WIFI_DISCONNECTED_BIT   BIT2
#define WIFI_SCAN_NUMBER           8
#define RETRY_NUMBER               3

/**********************************
 * Event and states enum
**********************************/

enum {
    NETIF_STATE_NOT_INITED          = 0x10,
    NETIF_STATE_INIT                = 0x11,
    NETIF_STATE_LOOP_CREATE         = 0x12,
    NETIF_STATE_FAILED              = 0xE1,
    NETIF_STATE_ERROR               = 0xE2,

    WIFI_STATE_INIT                 = 0x20,
    WIFI_STATE_CREATE_DEFAULTS      = 0x21,
    WIFI_STATE_SET_HANDLERS         = 0x22,
    WIFI_STATE_INIT_SYSTEM,

    WIFI_STATE_DISCONNECT           = 0x24,

    WIFI_STATE_CONNECT_FAILED       = 0xE3,
    WIFI_STATE_CONNECT_SUCCESS      = 0x25,
    WIFI_STATE_CONNECT_TRY          = 0x26,
    WIFI_STATE_SWITCH               = 0x31,
    WIFI_STATE_SCAN_BOOT            = 0x41,
};

/**********************************
 * Variables
**********************************/
const char *TAG = "WiFi station";

/****Initialisation variables */
bool netif_init_b = false;
bool netif_loop_init = false;
bool wifi_init = false;
bool wifi_default_sta_inited = false;
bool wifi_event_inited = false;

/****Global variables */
esp_netif_t * pvWifi_sta;
esp_netif_t * pvWifi_ap;

uint16_t wifi_state_id = NETIF_STATE_NOT_INITED;
uint16_t next_wifi_state_id = NETIF_STATE_NOT_INITED;
int16_t old_wifi_state_id = -1;

esp_event_handler_instance_t instance_any_id;
esp_event_handler_instance_t instance_got_ip;
EventGroupHandle_t s_wifi_event_group;
wifi_config_t wifi_config;
wifi_ap_record_t wifi_scan_list[WIFI_SCAN_NUMBER];
wifi_scan_config_t config_wifi_scan = {
    .ssid = 0,
    .bssid = 0,
    .channel = 0,
    .show_hidden = false,
    .scan_type = WIFI_SCAN_TYPE_ACTIVE,
    // .scan_time = 
    // .home_chan_dwell_time =
};

//IP varibles
ip_event_got_ip_t* ip_event;

int retry_num = 0;

/**********************************
 * Functions code
**********************************/

int wifi_search_list(wifi_ap_record_t* scan_list, wifi_id_t* wifi_list)
{
    int wifi_find = -1;
    size_t scan_number = sizeof(&scan_list) / sizeof(wifi_ap_record_t);

    for(int wifi_i = 0; (wifi_i < WIFI_NUMBER) && (wifi_find < 0); wifi_i ++)
    {
        for(int scan_i = 0; (scan_i < (int)scan_number) && (wifi_find < 0); scan_i ++)
        {
            if(strcmp((const char *)scan_list[scan_i].ssid, (const char *)wifi_list[wifi_i].ssid) == 0)
            wifi_find = wifi_i;
        }
    }
    return wifi_find;
}

static void wifi_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    esp_err_t ret;
    if( event_base == WIFI_EVENT)
    {
        switch(event_id)
        {
            case WIFI_EVENT_WIFI_READY:
                ESP_LOGI(TAG, "Wifi ready to work!");
                break;

            case WIFI_EVENT_SCAN_DONE:
                break;

            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;

            case WIFI_EVENT_STA_STOP:
                break;
            case WIFI_EVENT_STA_CONNECTED:
                break;

            case WIFI_EVENT_STA_DISCONNECTED:
                if (retry_num < RETRY_NUMBER) {
                    esp_wifi_connect();
                    retry_num++;
                    ESP_LOGI(TAG, "retry to connect to the AP");
                } else {
                    xEventGroupSetBits(s_wifi_event_group, WIFI_FAILED_BIT);
                    // ESP_LOGE(TAG, "Connection to wifi with ssid : %s and password : %s, was failed",  );
                }
                break;

            /*****AP station event ******/
            case WIFI_EVENT_AP_START:

            case WIFI_EVENT_AP_STOP:

            case WIFI_EVENT_AP_STACONNECTED:

            case WIFI_EVENT_AP_STADISCONNECTED:

            case WIFI_EVENT_AP_PROBEREQRECVED:

            case WIFI_EVENT_STA_BEACON_TIMEOUT:
            break;
        }
    }else if(event_base == IP_EVENT) {
        switch(event_id)
        {
            case IP_EVENT_STA_GOT_IP:
                ip_event = (ip_event_got_ip_t*) event_data;
                // ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
                break;

            case IP_EVENT_GOT_IP6:

            case IP_EVENT_STA_LOST_IP:
            break;
        }
    }
}

void task_network(void* pvParameter)
{
    esp_err_t ret;
    int retry_error = 0;
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    s_wifi_event_group = xEventGroupCreate();

    while(1)
    {   
        vTaskDelay(10);
        if(wifi_state_id != next_wifi_state_id)
        {
            old_wifi_state_id = wifi_state_id;
            wifi_state_id = next_wifi_state_id;
        }

        switch(wifi_state_id)
        {
            case NETIF_STATE_NOT_INITED:
                ESP_LOGI(TAG,"Netif initialisation ...");
                ret = esp_netif_init();
                if(ret != ESP_OK){
                    ESP_LOGE(TAG,"Netif initialisation failed, error : %s", esp_err_to_name(ret));
                    netif_init_b = false;
                    next_wifi_state_id = NETIF_STATE_ERROR;
                } else {
                    retry_error = 0;
                    next_wifi_state_id = NETIF_STATE_LOOP_CREATE;
                }
                break;

            case NETIF_STATE_INIT:   
                vTaskDelay(100);
                ESP_LOGI(TAG, "Netif initialise !");
                netif_init_b = true;
                next_wifi_state_id = WIFI_STATE_CREATE_DEFAULTS;
                break;

            case NETIF_STATE_LOOP_CREATE:
                ret = esp_event_loop_create_default();
                if(ret !=  ESP_OK)
                {
                    ESP_LOGE(TAG,"Netif creation of event loop failed, error : %s", esp_err_to_name(ret));
                    old_wifi_state_id = NETIF_STATE_LOOP_CREATE;
                    next_wifi_state_id = NETIF_STATE_ERROR;
                } else {
                    retry_error = 0;
                    netif_loop_init = true;
                    next_wifi_state_id = NETIF_STATE_INIT;
                }
                break;

            case NETIF_STATE_FAILED:    
                ESP_LOGE(TAG, "Netif initialisation stop, it's failed!");

                next_wifi_state_id = NETIF_STATE_NOT_INITED;
                break;

            case NETIF_STATE_ERROR: 
                vTaskDelay(100);
                if(old_wifi_state_id == NETIF_STATE_NOT_INITED)
                {
                    if(retry_error < RETRY_NUMBER)
                    {
                    retry_error ++;
                    next_wifi_state_id = NETIF_STATE_NOT_INITED;
                    }
                } else if(old_wifi_state_id == NETIF_STATE_LOOP_CREATE){
                    if(retry_error < RETRY_NUMBER)
                    {
                    retry_error ++;
                    next_wifi_state_id = NETIF_STATE_NOT_INITED;
                    }
                } else {
                    netif_init_b = false;
                    netif_loop_init = false;
                    next_wifi_state_id = NETIF_STATE_FAILED;
                }
                break;

/******************** Wifi state  */

            case WIFI_STATE_INIT:
                ESP_LOGI(TAG, "Wifi initialisation ...");
                wifi_event_inited = false;
                next_wifi_state_id = WIFI_STATE_CREATE_DEFAULTS; 
                break;
                
            case WIFI_STATE_CREATE_DEFAULTS:
                if(wifi_default_sta_inited == false)
                {
                    pvWifi_sta = esp_netif_create_default_wifi_sta();
                    wifi_default_sta_inited = true;
                    ESP_LOGI(TAG, "Wifi creacte with defaults parameters!");
                }
                    next_wifi_state_id = WIFI_STATE_SET_HANDLERS;
                break;

            case WIFI_STATE_SET_HANDLERS:         

                ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                                    ESP_EVENT_ANY_ID,
                                                                    &wifi_ip_event_handler,
                                                                    NULL,
                                                                    &instance_any_id));
                ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                                    IP_EVENT_STA_GOT_IP,
                                                                    &wifi_ip_event_handler,
                                                                    NULL,
                                                                    &instance_got_ip));
                ESP_LOGI(TAG, "Wifi instance handler set !");
                next_wifi_state_id = WIFI_STATE_INIT_SYSTEM;
                break;  

            case WIFI_STATE_INIT_SYSTEM:
                if(wifi_event_inited == false)
                {
                    ret = esp_wifi_init(&cfg);
                    if(ret != ESP_OK)
                    {
                        ESP_LOGE(TAG, "Wifi init failed ! Error: %s", esp_err_to_name(ret));
                        abort();
                    }
                    ESP_LOGI(TAG, "Wifi inited with success !");
                    wifi_event_inited = true;
                }
                next_wifi_state_id = WIFI_STATE_SCAN_BOOT;
                break;

            case WIFI_STATE_SCAN_BOOT:
                ret = esp_wifi_clear_ap_list();
                if(ret)
                {
                    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
                    ret = esp_wifi_scan_start(&config_wifi_scan, true);
                    if(ret)
                    {
                        uint16_t scan_wifi_number = WIFI_SCAN_NUMBER;
                        ESP_LOGI(TAG, "Wifi scan finished!");
                        esp_wifi_scan_get_ap_records(&scan_wifi_number, wifi_scan_list);
                        int know_wifi = wifi_search_list(wifi_scan_list, wifi);
                        if(know_wifi < 0)
                        {
                            ESP_LOGW(TAG, "No wifi is known in the search list!");
                            next_wifi_state_id = 0;
                        } else {
                        ESP_LOGI(TAG, "Wifi find, SSID: %s", wifi[know_wifi].ssid);
                        }
                    }
                }
                break;

            case WIFI_STATE_CONNECT_TRY:   
                    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
                    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
                    // ESP_LOGI(TAG, "Wifi configurate with success ! SSID: %s, Password: %s", ); //ajouter les config wifi

            case WIFI_STATE_CONNECT_FAILED:  

            case WIFI_STATE_DISCONNECT:         

            case WIFI_STATE_SWITCH:     
                break;
        }
    }
}


// esp_err_t wifi_init(wifi_id_t wifi)
// {
//     esp_err_t ret;

//     memset(&wifi_config, 0, sizeof(wifi_config));

//     esp_netif_t * my_sta = esp_netif_create_default_wifi_sta();
//     esp_netif_destroy(my_sta);
//     esp_netif_init();

//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
//     strncpy(&wifi_config.sta.ssid, wifi.ssid, 32);
//     strncpy(&wifi_config.sta.password, wifi.password, 64);
    
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
//     ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

//     ESP_LOGI(TAG, "Wi-Fi configurate in station mode succefully");
//     ESP_ERROR_CHECK(esp_wifi_start());


//     for(int i_connect = 1; (wifi_connect < 1) && (i_connect < 3); i_connect ++)
//     {
//         ESP_LOGI(TAG, "Trying to etablish connection with SSID : %s and PASSWORD : %s", wifi.ssid, wifi.password);
//         ret = esp_wifi_connect();
//         if(ret != ESP_OK)
//         {
//             ESP_LOGE(TAG, "Connection failed, error : %s", esp_err_to_name(ret));
//             esp_wifi_stop();
//         }else{
//             ESP_LOGI(TAG, "Error message : %s", esp_err_to_name(ret));
//             wifi_connect = 1;
//             break;
//         }
//     }

//     if(ret == ESP_OK)
//     {
//         ESP_LOGI(TAG, "Wi-Fi connect with success, to SSID %s and PASSWORD: %s", wifi.ssid, wifi.password);
//         return ESP_OK;
//     } else {
//         ESP_LOGE(TAG, "Wi-Fi connection failed, to SSID: %s and PASSSWORD: %s, error message : %s",wifi.ssid, wifi.password, esp_err_to_name(ret));
//         esp_wifi_disconnect();
//         esp_wifi_deinit();
//         wifi_connect = 0;
//         return ret;
//     }
// }

// esp_err_t wifi_disconnect(void)
// {
//     esp_err_t ret =  esp_wifi_disconnect();
//     if(ret == ESP_OK)
//     ESP_LOGI(TAG, "Wi-Fi disconnect with success");
//     else
//     ESP_LOGE(TAG, "Wi-Fi disconnection failed");

//     esp_wifi_stop();
//     esp_wifi_deinit();
//     wifi_connect = 0;
//     // vEventGroupDelete(s_wifi_event_group);
//     return ret;
// }

// // /************************************************* */
// // static void event_handler(void* arg, esp_event_base_t event_base,
// //                                 int32_t event_id, void* event_data)
// // {
// //     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
// //         esp_wifi_connect();
// //     } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
// //         if (retry_num < RETRY_NUMBER) {
// //             esp_wifi_connect();
// //             retry_num++;
// //             ESP_LOGI(TAG, "retry to connect to the AP");
// //         } else {
// //             xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
// //         }
// //         ESP_LOGI(TAG,"connect to the AP fail");
// //     } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
// //         ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
// //         ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
// //         retry_num = 0;
// //         xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
// //     }
// // }

// esp_err_t wifi_init_sta(wifi_id_t wifi)
// {
//     s_wifi_event_group = xEventGroupCreate();

//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());
//     vTaskDelay(100);
//     esp_netif_create_default_wifi_sta();

//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));

//     esp_event_handler_instance_t instance_any_id;
//     esp_event_handler_instance_t instance_got_ip;
//     ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
//                                                         ESP_EVENT_ANY_ID,
//                                                         &event_handler,
//                                                         NULL,
//                                                         &instance_any_id));
//     ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
//                                                         IP_EVENT_STA_GOT_IP,
//                                                         &event_handler,
//                                                         NULL,
//                                                         &instance_got_ip));

//     wifi_config_t wifi_config;

//     strncpy(&wifi_config.sta.ssid, &wifi.ssid, 32);
//     strncpy(&wifi_config.sta.password, &wifi.password, 64);

//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
//     ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
//     ESP_ERROR_CHECK(esp_wifi_start() );

//     ESP_LOGI(TAG, "wifi_init_sta finished.");

//     /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
//      * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
//     EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
//             WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
//             pdFALSE,
//             pdFALSE,
//             portMAX_DELAY);

//     /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
//      * happened. */
//     if (bits & WIFI_CONNECTED_BIT) {
//         ESP_LOGI(TAG, "connected to ap SSID: %s password: %s",
//                  wifi.ssid, wifi.password);
//                  return ESP_OK;
//     } else if (bits & WIFI_FAIL_BIT) {
//         ESP_LOGI(TAG, "Failed to connect to SSID: %s, password: %s",
//                  wifi.ssid, wifi.password);
//                 esp_wifi_stop();
//                 esp_wifi_deinit();
//                 return ESP_FAIL;
//     } else {
//         ESP_LOGE(TAG, "UNEXPECTED EVENT");
//         return ESP_FAIL;
//     }

//     /* The event will not be processed after unregister */
//     ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
//     ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
//     vEventGroupDelete(s_wifi_event_group);
//     esp_event_loop_delete_default();

// }