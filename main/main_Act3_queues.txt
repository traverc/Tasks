//Activity 3
#include <stdio.h>
#include "freertos/FreeRTOS.h"
//#include "esp_log.h"  
#define TASK_PRIO_3         3
#define COMP_LOOP_PERIOD    5000
#define QUEUE_CREATE_ERR_STR    "queue creation failed"
// For the Queue example
static QueueHandle_t msg_queue; //Define a handle for Queue
static const uint8_t msg_queue_len = 40; //Set a max length for Queue
static volatile bool timed_out; 

static void print_q_msg(void *arg)
{
    int data;  // data type should be same as queue item type
    int to_wait_ms = 1000;  // the maximal blocking waiting time in milliseconds
    const TickType_t xTicksToWait = pdMS_TO_TICKS(to_wait_ms);

    while (!timed_out) {
        if (xQueueReceive(msg_queue, &data, xTicksToWait) == pdTRUE) {
            printf("received data = %d \n", data);
        } else {
            printf("Did not received data in the past %d ms- \n", to_wait_ms);
        }
    }
    vTaskDelete(NULL);
}

static void send_q_msg(void *arg)
{
    int sent_num  = 0;
    while (!timed_out) {
    // Try to add item to queue, fail immediately if queue is full
        if (xQueueSend(msg_queue, &sent_num, portMAX_DELAY) != pdTRUE) {
        printf("Queue full\n");
        }
        printf("sent data = %d \n", sent_num);
        sent_num++;

        // send an item for every 250ms
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
        vTaskDelete(NULL);
}

void app_main(void)
{
    timed_out = false;
    //Create a message queue
      msg_queue = xQueueCreate(msg_queue_len, sizeof(int));
    if (msg_queue == NULL) {  
        printf(QUEUE_CREATE_ERR_STR);
        }
    xTaskCreatePinnedToCore(print_q_msg, "print_q_msg", 4096, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(send_q_msg, "send_q_msg", 4096, NULL, 3, NULL, 0);
    // time out and stop running after 5 seconds
    vTaskDelay(pdMS_TO_TICKS(COMP_LOOP_PERIOD));
    timed_out = true;
    // delay to let tasks finish the last loop
    vTaskDelay(500 / portTICK_PERIOD_MS);
}