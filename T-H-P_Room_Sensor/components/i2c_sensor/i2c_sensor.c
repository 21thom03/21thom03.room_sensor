#include <stdio.h>
#include "i2c_sensor.h"

static const char *TAG = "I2C";

/******************************
 * TOOLS
 ******************************/

void i2c_begin(i2c_sensor_t sensor)
{
}

/*---------------------------------------------------------------------------------------------------*/

void i2c_disconnected(i2c_sensor_t sensor)
{
}

/******************************
 * Reading functions
 ******************************/

esp_err_t i2c_read_bytes(i2c_sensor_t sensor, uint8_t data_register, uint8_t *data, size_t data_len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((sensor.slave_addr << 1) | I2C_MASTER_READ), ACK);
    if (data_register != 0)
        i2c_master_write_byte(cmd, (data_register), ACK);
    i2c_master_read(cmd, &data[0], data_len - 1, ACK);
    i2c_master_read(cmd, &data[data_len - 1], 1, NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(sensor.port, cmd, pdMS_TO_TICKS(TIMEOUT));
    i2c_cmd_link_delete(cmd);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "[port : %X, slave: 0x%X] Read %d bytes\n", sensor.port, sensor.slave_addr, data_len);
    }
    else
    {
        ESP_LOGE(TAG, "[port : %X, slave: 0x%X] Failed to read %d bytes, error : %d\n", sensor.port, sensor.slave_addr, data_len, ret);
        ESP_LOGE(TAG, "Error : %d Message: %s\n", ret, esp_err_to_name(ret));
    }
    return ret;
}

/******************************
 * Writting functions
 ******************************/

esp_err_t i2c_write_bytes(i2c_sensor_t sensor, uint8_t write_register, uint8_t *data, size_t data_len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((sensor.slave_addr << 1) | I2C_MASTER_WRITE), ACK);
    for(uint8_t rgt_nbr = 0; rgt_nbr < write_register || write_register == 0; rgt_nbr++)
    {
        i2c_master_write_byte(cmd, write_register, ACK);
        i2c_master_write(cmd, data, data_len, ACK);
    }

    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(sensor.port, cmd, pdMS_TO_TICKS(TIMEOUT));
    i2c_cmd_link_delete(cmd);

    if (!ret)
    {
        ESP_LOGI(TAG, "[port : %X, slave : 0x%X] Writing succesfully completed", sensor.port, sensor.slave_addr);
    }
    else
    {
        ESP_LOGE(TAG, "[port : %X, slave : 0x%X] Writing failed, error %d", sensor.port, sensor.slave_addr, ret);
        ESP_LOGE(TAG, "Message : %s", esp_err_to_name(ret));
    }
    return ret;
}

/*---------------------------------------------------------------------------------------------------*/