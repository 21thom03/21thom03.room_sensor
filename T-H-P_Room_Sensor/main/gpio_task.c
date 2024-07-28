#include "main_config.h"

/***********************************
 * Variables GPIO
***********************************/
static char* TAG = "GPIO";

void task_gpio(void* pvParameter)
{
    bool blink_var = false;
    int8_t old_gpio_state = -1;

    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_PIN));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_PIN, GPIO_MODE_OUTPUT));
    while(1)
    {
        switch(GPIO_var)
        {
            case GPIO_OFF:
                if(GPIO_var != old_gpio_state)
                {
                    old_gpio_state = GPIO_var;
                    ESP_LOGI(TAG, "GPIO %d, OFF", GPIO_PIN);
                }
                gpio_set_level(GPIO_PIN, 0);
                break;

            case GPIO_BLINK:
                if(GPIO_var != old_gpio_state)
                {
                    old_gpio_state = GPIO_var;
                    ESP_LOGI(TAG, "GPIO %d, blink", GPIO_PIN);
                }
                gpio_set_level(GPIO_PIN, 1);
                vTaskDelay(GPIO_BLINK_FREQ_HZ/portTICK_PERIOD_MS);
                gpio_set_level(GPIO_PIN, 0);
                vTaskDelay(GPIO_BLINK_FREQ_HZ/portTICK_PERIOD_MS);
                gpio_set_level(GPIO_PIN, 1);
                break;

            case GPIO_ON:
                if(GPIO_var != old_gpio_state)
                {
                    old_gpio_state = GPIO_var;
                    ESP_LOGI(TAG, "GPIO %d, ON", GPIO_PIN);
                }
                esp_err_t ret = gpio_set_level(GPIO_PIN, 1);
                printf("Ret = %s", esp_err_to_name(ret));
                break;
        }
    }
}