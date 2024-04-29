#ifndef _I2C_SENSOR_H_
#define _I2C_SENSOR_H_

#include <stdint.h>

/* ERROR include*/
#include "esp_err.h"
#include "esp_log.h"

#include "driver/i2c.h"
#include "driver/gpio.h"

/**********************************
 * I2C Registers
***********************************/
#define HUMIDITY_LSB_REG        0XFE
#define HUMIDITY_MSB_REG        0xFD
#define TEMP_XLSB_REG           0xFC        // only 4 bits (7,6,5,4)
#define TEMP_LSB_REG            0xFB
#define TEMP_MSB_REG            0xFA
#define PRESS_XLSB_REG          0xF9        // only 4 bits (7,6,5,4)
#define PRESS_LSB_REG           0xF8
#define PRESS_MSB_REG           0xF7
#define CONFIG_REGISTER         0xF5        // t_sb (7,6,5) - filter (4,3,2) - spi3w_en (0)
#define CTRL_MEAS               0xF4        // osrs_t (7,6,5) - osrs_p (4,3,2) - mode (1,0)
#define STATUS                  0xF3        // measuring (3) - im_update (0)
#define CTRL_HUM                0xF2        // osrs_h (3,2,1,0)
#define CALIB_26_41             0xE1
#define RESET                   0xE0
#define CHIP_ID                 0xD0        //READ ONLY
#define CALIB_00_25             0x88           

/**********************************
 * Functions utilities
**********************************/
#define ACK                     0x01
#define NACK                    0x00
#define I2C_MASTER_READ         0x01
#define I2C_MASTER_WRITE        0x00
#define TIMEOUT                 pdMS_TO_TICKS(20) 

/**********************************
 * Structure et Enumeration
**********************************/

typedef struct
{
        i2c_port_t port;
        i2c_config_t config;
        uint8_t slave_addr;
} i2c_sensor_t;

typedef enum
{
    NORMAL_CONFIG   =   0x03,
    FORCED_CONFIG   =   0x01,
    SLEEP_CONFIG    =   0x00
} BME280_config_t;

typedef enum
{
    Over_0          =   0x00,
    Over_1          =   0x01,
    Over_2          =   0x02,
    Over_4          =   0x03,
    Over_8          =   0x04,
    Over_16         =   0x05
} BME280_oversampling_t;

typedef enum
{
    no_filter       =   0x00,
    filter_2        =   0x01,
    filter_4        =   0x02,
    filter_8        =   0x03,
    filter_16       =   0x04
} BME280_filter_t;

typedef enum
{
    t_sb_0p5        =   0x00,
    t_sb_62p5       =   0x01,
    t_sb_125        =   0x02,
    t_sb_250        =   0x03,
    t_sb_500        =   0x04,
    t_sb_1000       =   0x05,
    t_sb_10         =   0x06,
    t_sb_20         =   0x07
} standby_time_t;

/**********************************
 * I2C functions
**********************************/

void i2c_begin(i2c_sensor_t sensor);

void i2c_disconnected(i2c_sensor_t sensor);

/**
 * @brief Read I2C value(s)
 * @param i2c_master        [I2C bus master configuration]
 * @param i2c_slave         [I2C bus slave configuration]
 * @param write_register    [Register number to write]
 * @param data              [data read on I2C bus]
 * @param data_len          [data lenght write on the bus]
 * @param timeout           [timeout maximum of the bus]
 */
esp_err_t i2c_read_bytes(i2c_sensor_t sensor, uint8_t data_register, uint8_t *data, size_t data_len);
        //     .mode = I2C_MODE_MASTER,
        //     .sda_io_num = I2C_MASTER_SDA_IO, // select GPIO specific to your project
        //         .sda_pullup_en = GPIO_PULLUP_ENABLE,
        //     .scl_io_num = I2C_MASTER_SCL_IO, // select GPIO specific to your project
        //         .scl_pullup_en = GPIO_PULLUP_ENABLE,
        //     .master.clk_speed = I2C_MASTER_FREQ_HZ, // select frequency specific to your project
        //     //.clk_flags = 0,                         /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */

/**
 * @brief Write data in I2C bus
 * @param sensor            [I2C bus configuration]
 * @param write_register    [Register number to write]
 * @param data              [data write on I2C bus]
 * @param data_len          [data lenght write on the bus]
 * @param timeout           [timeout maximum of the bus]
 */
esp_err_t i2c_write_bytes(i2c_sensor_t sensor, uint8_t write_register, uint8_t *data, size_t data_len);

/**
 * @brief Configure the sensor status between Sleep, Forced and Normal
*/
esp_err_t i2c_BME280_config(i2c_sensor_t sensor, BME280_config_t config);

#endif /* End of _I2C_SENSOR_H_ */