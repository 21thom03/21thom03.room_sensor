#include <stdio.h>
#include <stdint.h>

#include "i2c_com.h"

static const char *TAG = "I2C";

/******************************
 * SETUP
******************************/

void I2C_begin(i2c_master_t i2c_master)
{
    esp_err_t err;

    err = i2c_param_config(i2c_master.I2C_port_number, &i2c_master.config);
    err = i2c_driver_install(i2c_master.I2C_port_number, i2c_master.config.mode, 0, 0, 0);
    if (!err)
        ESP_LOGI(TAG, "Configuration OK\n");
    else
        ESP_LOGE(TAG, "Configuration failed, ERROR : %d\n Error messge : %s", err, esp_err_to_name(err));
}

 /*---------------------------------------------------------------------------------------------------*/

void i2c_disconnected(i2c_master_t i2c_master)
{
    esp_err_t err = i2c_driver_delete(i2c_master.I2C_port_number);
    printf("\n\n");
    if(err == ESP_OK) ESP_LOGI(TAG, "I2C port number : %d end of communication", i2c_master.I2C_port_number);
    else ESP_LOGW(TAG, "I2C port number : %d end of communication failed. Message : %s", i2c_master.I2C_port_number, esp_err_to_name(err));
}

/******************************
 * Reading functions
******************************/

esp_err_t i2c_read_bytes(i2c_master_t i2c_master, i2c_slave_t i2c_slave, uint8_t data_register, uint8_t *data, size_t data_len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((i2c_slave.slave_addr << 1) | I2C_MASTER_READ), ACK);
    if(data_register != 0) i2c_master_write_byte(cmd, (data_register), ACK);
    i2c_master_read(cmd, &data[0], data_len-1, ACK);
    i2c_master_read(cmd, &data[data_len-1], 1, NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_master.I2C_port_number, cmd, pdMS_TO_TICKS(i2c_slave.timeout));
    i2c_cmd_link_delete(cmd);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "[port : %X, slave: 0x%X, Slave name : %s] Read %d bytes\n", i2c_master.I2C_port_number, i2c_slave.slave_addr, i2c_slave.name, data_len);
    }
    else
    {
        ESP_LOGE(TAG, "[port : %X, slave: 0x%X, Slave name : %s] Failed to read %d bytes, error : %d\n", i2c_master.I2C_port_number, i2c_slave.slave_addr, i2c_slave.name, data_len, ret);
        ESP_LOGE(TAG, "Error : %d Message: %s\n", ret, esp_err_to_name(ret));
    }
    return ret;
}

/******************************
 * Writting functions
******************************/

esp_err_t i2c_write_bytes(i2c_master_t i2c_master, i2c_slave_t i2c_slave, uint8_t write_register, uint8_t *data, size_t data_len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((i2c_slave.slave_addr << 1) | I2C_MASTER_WRITE), ACK);
    if(write_register != 0)i2c_master_write_byte(cmd, write_register, ACK);
    if(data_len != 0)i2c_master_write(cmd, data, data_len, ACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_master.I2C_port_number, cmd, pdMS_TO_TICKS(i2c_slave.timeout));
    i2c_cmd_link_delete(cmd);

    if (!ret)
    {
        ESP_LOGI(TAG, "[port : %X, slave : 0x%X] Writing succesfully completed", i2c_master.I2C_port_number, i2c_slave.slave_addr);
    }
    else
    {
        ESP_LOGE(TAG, "[port : %X, slave : 0x%X] Writing failed, error %d", i2c_master.I2C_port_number, i2c_slave.slave_addr, ret);
        ESP_LOGE(TAG, "Message : %s", esp_err_to_name(ret));
    }
    return ret;
}

/******************************
 * TOOLS
******************************/

esp_err_t test_connection(i2c_master_t i2c_master, i2c_slave_t i2c_slave)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((i2c_slave.slave_addr << 1) | I2C_MASTER_WRITE), ACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_master.I2C_port_number, cmd, pdMS_TO_TICKS(i2c_slave.timeout));
    i2c_cmd_link_delete(cmd);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "[port : %X, slave : 0x%X, Slave name : %s] Communication initialize", i2c_master.I2C_port_number, i2c_slave.slave_addr, i2c_slave.name);
    }
    else
    {
        ESP_LOGE(TAG, "[port : %X, slave : 0x%X, Slave name : %s] Communication failed, error : %d, Message : %s", i2c_master.I2C_port_number, i2c_slave.slave_addr, i2c_slave.name, ret, esp_err_to_name(ret));
    }
    return ret;
}

 /*---------------------------------------------------------------------------------------------------*/

void scanner(i2c_master_t i2c_master)
{
    i2c_slave_t i_slave;
    i_slave.timeout = 20;

    ESP_LOGW(TAG, "\n>> I2C scanning ...\n");
    uint8_t count = 0;
    for (size_t i = 0x3; i < 0x78; i++)
    {
        i_slave.slave_addr = i;
        if (test_connection(i2c_master, i_slave) == ESP_OK)
        {
            ESP_LOGW(TAG, "- Device found at address 0x%X", i);
            count++;
        }
    }
    if (count == 0)
        ESP_LOGE(TAG, "- No I2C devices found!\n");
}

 /*---------------------------------------------------------------------------------------------------*/