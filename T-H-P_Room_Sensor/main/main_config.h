#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_

/**********************************
 * Project include
***********************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_err.h"
#include "esp_log.h"

#include "esp_sntp.h"
#include "driver/i2c.h"
#include "driver/gpio.h"


#include "i2c_sensor.h" 
#include "BME280.h"

/**********************************
 * Wi-Fi configuration
***********************************/
#define WIFI_NUMBER             3

typedef struct {
    char ssid[32];
    char bssid[6];
    char password[64];
} wifi_id_t;

wifi_id_t wifi[WIFI_NUMBER];

/***********************************
 * I2C constantes
***********************************/
#define I2C_MASTER_SDA_IO       21      
#define GPIO_PULLUP_ENABLE      true
#define I2C_MASTER_SCL_IO       22
#define I2C_MASTER_FREQ_HZ      100000      //100KHz

#define I2C_PORT                I2C_NUM_0

/***********************************
 * GPIO constante
***********************************/
#define GPIO_PIN                32
#define GPIO_BLINK_FREQ_HZ      50  //in Hz

/*Event BIT*/
#define GPIO_OFF                BIT0
#define GPIO_ON                 BIT1
#define GPIO_BLINK              BIT2

EventGroupHandle_t gpio_event_group;

#endif /* end of MAIN_CONFIG_H_ */