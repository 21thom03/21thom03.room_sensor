#include <stdio.h>
#include "i2c_sensor.h"

static const char *TAG = "I2C";

/******************************
 * TOOLS
 ******************************/

void i2c_begin(i2c_sensor_t sensor)
{
    esp_err_t err;

    err = i2c_param_config(sensor.port, &sensor.i2c_config);
    err = i2c_driver_install(sensor.port, sensor.i2c_config.mode, 0, 0, 0);
    if (!err)
    {
        ESP_LOGW(TAG, "/-------Configuration finished---------/");
        ESP_LOGW(TAG, "I2C port : %d\r", sensor.port);
        ESP_LOGW(TAG, "SDA pin : %d, SCL pin : %d\r", sensor.i2c_config.sda_io_num, sensor.i2c_config.scl_io_num);
        ESP_LOGW(TAG, "Clock speed : %d Hertz\r", sensor.i2c_config.master.clk_speed);
        ESP_LOGW(TAG, "/--------------------------------------/");
    }
    else
        ESP_LOGE(TAG, "[port : %X] Configuration failed, ERROR : %d\n Error messge : %s", sensor.port, err, esp_err_to_name(err));
}

/*---------------------------------------------------------------------------------------------------*/

void i2c_disconnected(i2c_sensor_t sensor)
{
    esp_err_t err = i2c_driver_delete(sensor.port);
    printf("\n\n");
    if (err == ESP_OK)
        ESP_LOGI(TAG, "[port : %X] End of communication", sensor.port);
    else
        ESP_LOGW(TAG, "[port : %X] End of communication failed. Message : %s", sensor.port, esp_err_to_name(err));
}

/******************************
 * Reading functions
 ******************************/

esp_err_t i2c_read_bits(i2c_sensor_t sensor, uint8_t data_register, uint8_t *data, size_t data_len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((sensor.slave_addr << 1) | I2C_MASTER_WRITE), ACK);
    i2c_master_write_byte(cmd, (data_register), ACK);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((sensor.slave_addr << 1) | I2C_MASTER_READ), ACK);
    i2c_master_read(cmd, &data[0], data_len - 1, ACK);
    i2c_master_read(cmd, &data[data_len - 1], 1, NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(sensor.port, cmd, pdMS_TO_TICKS(TIMEOUT));
    i2c_cmd_link_delete(cmd);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "[port : %X, slave: 0x%X] Read %d bytes from 0x%X register\n", sensor.port, sensor.slave_addr, data_len, data_register);
    }
    else
    {
        ESP_LOGE(TAG, "[port : %X, slave: 0x%X] Failed to read %d bytes, error : %d\n", sensor.port, sensor.slave_addr, data_len, ret);
        ESP_LOGE(TAG, "Error : %d Message: %s\n", ret, esp_err_to_name(ret));
    }
    return ret;
}

esp_err_t i2c_read_bit(i2c_sensor_t sensor, uint8_t read_register, uint8_t *data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((sensor.slave_addr << 1) | I2C_MASTER_WRITE), ACK);
    i2c_master_write_byte(cmd, read_register, ACK);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((sensor.slave_addr << 1) | I2C_MASTER_READ), ACK);
    i2c_master_read_byte(cmd, data, NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(sensor.port, cmd, pdMS_TO_TICKS(TIMEOUT));
    i2c_cmd_link_delete(cmd);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "[port : %X, slave: 0x%X] Read the 0x%X register\n", sensor.port, sensor.slave_addr, read_register);
    }
    else
    {
        ESP_LOGE(TAG, "[port : %X, slave: 0x%X] Failed to read %X register, error : %d\n", sensor.port, sensor.slave_addr, read_register, ret);
        ESP_LOGE(TAG, "Error : %d Message: %s\n", ret, esp_err_to_name(ret));
    }
    return ret;
}

/******************************
 * Writting functions
 ******************************/

esp_err_t i2c_write_bits(i2c_sensor_t sensor, uint8_t write_register, uint8_t *data, size_t data_len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((sensor.slave_addr << 1) | I2C_MASTER_WRITE), ACK);
    for (uint8_t reg_nbr = 0; reg_nbr <= write_register && write_register != 0; reg_nbr++)
    {
        i2c_master_write_byte(cmd, (write_register + reg_nbr), ACK);
        i2c_master_write_byte(cmd, data[reg_nbr], ACK);
    }
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(sensor.port, cmd, pdMS_TO_TICKS(TIMEOUT));
    i2c_cmd_link_delete(cmd);

    if (!ret)
    {
        ESP_LOGI(TAG, "[port : %X, slave : 0x%X] Writing bits succesfully completed", sensor.port, sensor.slave_addr);
    }
    else
    {
        ESP_LOGE(TAG, "[port : %X, slave : 0x%X] Writing bits failed, error : %d, message : %s", sensor.port, sensor.slave_addr, ret, esp_err_to_name(ret));
    }
    return ret;
}

esp_err_t i2c_write_bit(i2c_sensor_t sensor, uint8_t write_register, uint8_t *data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((sensor.slave_addr << 1) | I2C_MASTER_WRITE), ACK);
    i2c_master_write_byte(cmd, write_register, ACK);
    i2c_master_write_byte(cmd, *data, ACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(sensor.port, cmd, pdMS_TO_TICKS(TIMEOUT));
    i2c_cmd_link_delete(cmd);

    if (!ret)
    {
        ESP_LOGI(TAG, "[port : %X, slave : 0x%X] Writing bit succesfully completed", sensor.port, sensor.slave_addr);
    }
    else
    {
        ESP_LOGE(TAG, "[port : %X, slave : 0x%X] Writing bit failed, error : %d, message : %s", sensor.port, sensor.slave_addr, ret, esp_err_to_name(ret));
    }
    return ret;
}