#include "main_config.h"

/***********************************
 * Variables GPIO
***********************************/
static char* TAG = "GPIO";

void task_gpio(void* pvParameter)
{
    int gpio_val;
    gpio_event_group = xEventGroupCreate();
    EventBits_t ret_bits;

    ESP_ERROR_CHECK(gpio_reset_pin(GPIO_PIN));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_PIN, GPIO_MODE_OUTPUT));
    while(1)
    {
        ret_bits = xEventGroupWaitBits(gpio_event_group,GPIO_ON | GPIO_OFF | GPIO_BLINK,pdFALSE,pdFALSE, portMAX_DELAY);
        switch(ret_bits)
        {
            case GPIO_OFF:
                ESP_LOGI(TAG, "GPIO %d, OFF", GPIO_PIN);
                gpio_set_level(GPIO_PIN, 0);
                vTaskDelay(10);
                xEventGroupClearBits(gpio_event_group, ret_bits);
                break;

            case GPIO_BLINK:
                ESP_LOGI(TAG, "GPIO %d, blink", GPIO_PIN);
                gpio_val = gpio_get_level(GPIO_PIN);
                for(int i =0; i == 10; i++)
                {
                    gpio_set_level(GPIO_PIN, gpio_val);
                    gpio_val = !gpio_val;
                    vTaskDelay(GPIO_BLINK_FREQ_HZ/portTICK_PERIOD_MS);
                }
                xEventGroupClearBits(gpio_event_group, ret_bits);
                break;

            case GPIO_ON:
                ESP_LOGI(TAG, "GPIO %d, ON", GPIO_PIN);
                gpio_set_level(GPIO_PIN, 1);
                xEventGroupClearBits(gpio_event_group, ret_bits);
                break;

            default :
            break;
        }
    }
}