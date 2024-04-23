#include <stdio.h>
#include "i2c_sensor.h"

extern "C"
{
    I2C::I2C(i2c_port_t port, i2c_config_t config, uint8_t slave_addr) : port{port}, config{config}, slave_addr{slave_addr}
    {
    }
}
