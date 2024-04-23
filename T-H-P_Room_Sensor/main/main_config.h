#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_

/**********************************
 * Project include
***********************************/
#include "driver/i2c.h"
#include "driver/gpio.h"


/**********************************
 * Constante definition
***********************************/
#define I2C_MASTER_SDA_IO
#define GPIO_PULLUP_ENABLE
#define I2C_MASTER_SCL_IO
#define I2C_MASTER_FREQ_HZ

#define I2C_PORT

/**********************************
 * Class and structure declaration
***********************************/
i2c_port_t i2c_sensor_port = ;
i2c_config_t master_config
{
    .mode = I2C_MODE_MASTER,
    .sda_io_num = I2C_MASTER_SDA_IO, // select GPIO specific to your project
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_io_num = I2C_MASTER_SCL_IO, // select GPIO specific to your project
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = I2C_MASTER_FREQ_HZ, // select frequency specific to your project
};



#endif /* end of MAIN_CONFIG_H_ */