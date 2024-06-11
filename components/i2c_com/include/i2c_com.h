#ifndef _I2C_COM_H_
#define _I2C_COM_H_

#include <stdint.h>

/******************************
 * External include
******************************/
#include "esp_err.h"
#include "esp_log.h"

/******************************
 * Components
******************************/
#include "binary_convert.h"

#include "driver/i2c.h"
#include "driver/gpio.h"

/******************************
 * Constant
******************************/

#define I2C_MASTER_WRITE        0x00
#define I2C_MASTER_READ         0x01

#define ACK                     FALSE
#define NACK                    TRUE

/******************************
 * Structure
******************************/

typedef struct
{
    char name[20];
    uint8_t slave_addr;
    timer_t timeout;
} i2c_slave_t;

 /*---------------------------------------------------------------------------------------------------*/

typedef struct
{
    i2c_port_t I2C_port_number;
    i2c_config_t config;
} i2c_master_t;

/**
 * @brief Configure the I2C connection and create first connection (install I2C driver)
 * @param i2c_master [master config with master port number and master config]
 */
void I2C_begin(i2c_master_t i2c_master);

void i2c_disconnected(i2c_master_t i2c_master);

/******************************
 * Reading functions
******************************/

/**
 * @brief Read I2C value(s)
 * @param i2c_master        [I2C bus master configuration]
 * @param i2c_slave         [I2C bus slave configuration]
 * @param write_register    [Register number to write]
 * @param data              [data read on I2C bus]
 * @param data_len          [data lenght write on the bus]
 * @param timeout           [timeout maximum of the bus]
 */
esp_err_t i2c_read_bytes(i2c_master_t i2c_master, i2c_slave_t i2c_slave, uint8_t data_register, uint8_t *data, size_t data_len);

/******************************
 * Writting functions
******************************/

/**
 * @brief Write data in I2C bus
 * @param i2c_master        [I2C bus master configuration]
 * @param i2c_slave         [I2C bus slave configuration]
 * @param write_register    [Register number to write]
 * @param data              [data write on I2C bus]
 * @param data_len          [data lenght write on the bus]
 * @param timeout           [timeout maximum of the bus]
 */
esp_err_t i2c_write_bytes(i2c_master_t i2c_master, i2c_slave_t i2c_slave, uint8_t write_register, uint8_t *data, size_t data_len);

/******************************
 * TOOLS
******************************/

/**
 * @brief Test the I2C connection
 * @param i2c_master        [I2C bus master configuration]
 * @param i2c_slave         [I2C bus slave configuration]
 */
esp_err_t test_connection(i2c_master_t i2c_master, i2c_slave_t i2c_slave);

/**
 * @brief scanne all the I2C adresse to search if an I2C device is connect
 * @return
 *          void
 */
void scanner(i2c_master_t i2c_master);
#endif