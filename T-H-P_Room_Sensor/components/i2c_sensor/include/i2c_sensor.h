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
#define CONFIG_REGISTER     0xF5
#define TEMP_MSB_REG        0xFA
#define TEMP_LSB_REG        0xFB
#define TEMP_XLSB_REG       0xFC        // only 4 bits (7,6,5,4)

#define PRESS_MSB_REG       0xF7    
#define PRESS_LSB_REG       0xF8
#define PRESS_XLSB_REG      0xF9        // only 4 bits (7,6,5,4)

#define HUMIDITY_MSB_REG    0xFD
#define HUMIDITY_LSB_REG    0XFE


#ifdef __cplusplus
extern "C"
{
    class I2C
    {
    private:
        i2c_port_t port;
        i2c_config_t config;
        uint8_t slave_addr;

    public:
        explicit I2C(i2c_port_t port, i2c_config_t config, uint8_t slave_addr);

        //     .mode = I2C_MODE_MASTER,
        //     .sda_io_num = I2C_MASTER_SDA_IO, // select GPIO specific to your project
        //         .sda_pullup_en = GPIO_PULLUP_ENABLE,
        //     .scl_io_num = I2C_MASTER_SCL_IO, // select GPIO specific to your project
        //         .scl_pullup_en = GPIO_PULLUP_ENABLE,
        //     .master.clk_speed = I2C_MASTER_FREQ_HZ, // select frequency specific to your project
        //     //.clk_flags = 0,                         /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
        // }
        // }
        // ;
    };
}
#endif

#endif /* End of _I2C_SENSOR_HPP_ */