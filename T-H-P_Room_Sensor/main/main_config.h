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
#include "esp_err.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_wifi.h"

#include "i2c_sensor.h" 
#include "BME280.h"

/**********************************
 * Wi-Fi configuration
***********************************/
#define WIFI_SSID               "Livebox-FA70"  //"Ouiii"
#define WIFI_PASSWORD           "C1409D1206M1408T2103N2702" //"Zbeubzbeub2103"      

/**********************************
 * Constante definition
***********************************/
#define I2C_MASTER_SDA_IO       21      
#define GPIO_PULLUP_ENABLE      true
#define I2C_MASTER_SCL_IO       22
#define I2C_MASTER_FREQ_HZ      100000      //100Kh

#define I2C_PORT                I2C_NUM_0

#endif /* end of MAIN_CONFIG_H_ */