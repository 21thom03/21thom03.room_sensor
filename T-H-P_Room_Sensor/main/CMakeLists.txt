idf_component_register(SRCS "wifi.c" "T-H-P_Room_Sensor.c"
                    INCLUDE_DIRS "."
                    REQUIRES i2c_sensor BME280)
