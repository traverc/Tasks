#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN  GPIO_NUM_10        //Choose a good LED pin to use
#define DELAY_T1 1000
#define DELAY_T2 1500


void task1()
{
    int counter = 0;

    while (true)
    {
        printf("Task1: counter = %d\n", counter);
        counter++;
        vTaskDelay(DELAY_T1 / portTICK_PERIOD_MS);
    }
}
void task2()
{
    int counter = 0;

    while (true)
    {
        printf("Task1: counter = %d\n", counter);
        counter--;
        vTaskDelay(DELAY_T2 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{

    xTaskCreatePinnedToCore(task1, "First_Task", 2048, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(task2, "Second_Task", 2048, NULL, 3, NULL, 0);
}
