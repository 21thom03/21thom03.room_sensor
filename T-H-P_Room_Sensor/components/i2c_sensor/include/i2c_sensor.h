#ifndef _I2C_SENSOR_H_
#define _I2C_SENSOR_H_

#include <stdint.h>

/* ERROR include*/
#include "esp_err.h"
#include "esp_log.h"

#include "driver/i2c.h"
#include "driver/gpio.h"

/**********************************
 * Functions utilities
**********************************/
#define ACK                     0x01
#define NACK                    0x00
#define I2C_MASTER_READ         0x01
#define I2C_MASTER_WRITE        0x00
#define TIMEOUT                 pdMS_TO_TICKS(40) 

/**
 * @brief   I2C sensor config
 * @tparam      port [i2c port connect to the sensor]
 * @tparam      i2c_config [configuration of i2c port]
 * @tparam      slave_addr
*/
typedef struct
{
        i2c_port_t port;
        i2c_config_t i2c_config;
        uint8_t slave_addr;
} i2c_sensor_t;

/**********************************
 * I2C functions
**********************************/

void i2c_begin(i2c_sensor_t sensor);

void i2c_disconnected(i2c_sensor_t sensor);

/**
 * @brief Read I2C several values
 * @param sensor            [I2C sensor configuration]
 * @param write_register    [Register number to write]
 * @param data              [data read on I2C bus]
 * @param data_len          [data lenght write on the bus]
 * @param timeout           [timeout maximum of the bus]
 */
esp_err_t i2c_read_bits(i2c_sensor_t sensor, uint8_t data_register, uint8_t *data, size_t data_len);

/**
 * @brief Read one I2C value
 * @param sensor            [I2C sensor configuration]
 * @param write_register    [Register number to write]
 * @param data              [data read on I2C bus]
 */
esp_err_t i2c_read_bit(i2c_sensor_t sensor, uint8_t data_register, uint8_t *data);

/**
 * @brief Write several data bits in I2C bus
 * @param sensor            [I2C sensor configuration]
 * @param write_register    [Register number to write]
 * @param data              [data write on I2C bus]
 * @param data_len          [data lenght write on the bus]
 */
esp_err_t i2c_write_bits(i2c_sensor_t sensor, uint8_t write_register, uint8_t *data, size_t data_len);

/**
 * @brief Write one data bit in I2C bus
 * @param sensor            [I2C sensor configuration]
 * @param write_register    [Register number to write]
 * @param data              [data write on I2C bus]
 * @param data_len          [data lenght write on the bus]
 */
esp_err_t i2c_write_bit(i2c_sensor_t sensor, uint8_t write_register, uint8_t *data);

#endif /* End of _I2C_SENSOR_H_ */