#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "data.h"
QueueHandle_t xQueueData;

// não mexer! Alimenta a fila com os dados do sinal
void data_task(void *p) {
    vTaskDelay(pdMS_TO_TICKS(400));

    int data_len = sizeof(sine_wave_four_cycles) / sizeof(sine_wave_four_cycles[0]);
    for (int i = 0; i < data_len; i++) {
        xQueueSend(xQueueData, &sine_wave_four_cycles[i], 1000000);
    }

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void process_task(void *p) {
    int data = 0;
    int list_data[1000];
    int contador = 0;
    while (true) {
        if (xQueueReceive(xQueueData, &data, 100)) {
            list_data[contador] = data;
            int y;
            if(contador >3){
                y = (list_data[contador]+list_data[contador - 1]+list_data[contador - 2]+list_data[contador - 3] +list_data[contador - 4]) /5 ;
                printf("%d\n",y);
            }
            else{
                if(contador == 3){                
                    y = (list_data[contador]+list_data[contador - 1]+list_data[contador - 2]+list_data[contador - 3])/5 ;
                }
                else if(contador == 2 ){
                    y = (list_data[contador]+list_data[contador - 1]+list_data[contador - 2])/5 ;
                }
                else if(contador == 1 ){
                y = (list_data[contador]+list_data[contador - 1])/5 ;
                }
                else{
                y = list_data[contador]/5;
                }

                printf("%d\n",y);
                
            }
            contador ++;
            // implementar filtro aqui!
            // deixar esse delay!
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

int main() {
    stdio_init_all();

    xQueueData = xQueueCreate(64, sizeof(int));

    xTaskCreate(data_task, "Data task ", 4096, NULL, 1, NULL);
    xTaskCreate(process_task, "Process task", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}
