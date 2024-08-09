idf_component_register(SRCS "gpio_task.c" "wifi_task.c" "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES i2c_sensor BME280 nvs_flash)
