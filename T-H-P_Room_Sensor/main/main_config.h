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
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_wifi.h"

#include "i2c_sensor.h"
#include "BME280.h"

/**********************************
 * Wi-Fi configuration
***********************************/
#define WIFI_SSID               ""
#define WIFI_PASSWORD           ""

/**********************************
 * Constante definition
***********************************/
#define I2C_MASTER_SDA_IO       21      
#define GPIO_PULLUP_ENABLE      true
#define I2C_MASTER_SCL_IO       22
#define I2C_MASTER_FREQ_HZ      100000      //100Kh

#define I2C_PORT                I2C_NUM_0

#endif /* end of MAIN_CONFIG_H_ */