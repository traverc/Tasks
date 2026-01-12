//Activit 8 Challenge - Mini Clock
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "driver/gpio.h"

#define LED_PIN            10
#define TIMER_PERIOD_MS    1000
#define STOP_TIME_MS       60000
#define RESTART_TIME_MS    2000

TimerHandle_t clock_timer;
TimerHandle_t control_timer;

int seconds = 0;

// LED timer callback
void clock_timer_callback(TimerHandle_t xTimer)
{
    static int led_state = 0;
    led_state = !led_state;
    gpio_set_level(LED_PIN, led_state);

    seconds++;

    printf("Operating time: %d", seconds);
    if (seconds == 1){
        printf(" second\n");
    } else {
        printf(" seconds\n");
    }

}

// Control timer callback
void control_timer_callback(TimerHandle_t xTimer)
{
    static int state = 0;

    if (state == 0) {
        printf("Stopping clock timer...\n");
        xTimerStop(clock_timer, 0);

        seconds = 0;

        // Change control timer to restart delay
        xTimerChangePeriod(control_timer, pdMS_TO_TICKS(RESTART_TIME_MS), 0);
        state = 1;
    } else {
        printf("Restarting clock timer...\n");
        xTimerStart(clock_timer, 0);

        xTimerChangePeriod(control_timer, pdMS_TO_TICKS(STOP_TIME_MS), 0);

        state = 0;
    }
}

void app_main(void)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Create LED timer (periodic)
    clock_timer = xTimerCreate("Clock_Timer", pdMS_TO_TICKS(TIMER_PERIOD_MS), pdTRUE, NULL, clock_timer_callback);

    // Create control timer (periodic)
    control_timer = xTimerCreate("Control_Timer", pdMS_TO_TICKS(STOP_TIME_MS), pdTRUE, NULL, control_timer_callback);

    if (clock_timer && control_timer){
        printf("Starting clock timer...\n");
        xTimerStart(clock_timer, 0);

        printf("Starting control timer...\n");
        xTimerStart(control_timer, 0);
    } else {
        printf("Timer creation failed!\n");
    }
}
