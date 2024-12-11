/*
 * lidar.c
 *
 *  Created on: Dec 4, 2024
 *      Author: romingo
 */


#include "lidar.h"

#define DUREE 5000
#define DUREEP 30000

h_LIDAR_t lidar;
extern SemaphoreHandle_t semb_halfCllbck;
extern SemaphoreHandle_t semb_cpltCllbck;

void lidar_init(void){

	HAL_GPIO_WritePin(GPIOA,LIDAR_M_CTR_Pin , SET);

	  lidar.serial_drv.transmit = uart_transmit;
	  lidar.serial_drv.poll_receive = uart_poll_receive;

	  lidar.serial_drv.it_receive=uart_it_receive;
	  lidar.serial_drv.it_transmit=uart_it_transmit;

	  lidar.serial_drv.dma_receive=uart_dma_receive;
	  lidar.serial_drv.dma_transmit=uart_dma_transmit;
}

void lidarTake (void * pvParameters){
	uint8_t* buff;

	  if (LIDAR_start_scan_dma(&lidar) == 0) {
	      printf("LIDAR scanning started successfully.\r\n");
	  } else {
	      printf("Failed to start LIDAR scanning.\r\n");
	  }

	for(;;){
		// Attente du Semahore de half buffer
		xSemaphoreTake(semb_halfCllbck, portMAX_DELAY);

		// Pointeur sur le début des données
		buff = lidar.processing.receive_buff;
		LIDAR_process_frame(&lidar, buff);


		// Attente du semaphore du complete buffer
		xSemaphoreTake(semb_cpltCllbck, portMAX_DELAY);

		// Pointeur sur la moitié des données
		buff = &lidar.processing.receive_buff[FRAME_BUFF_SIZE];
		LIDAR_process_frame(&lidar, buff);
		}
}

void lidarprocess (void * pvParameters){

	for(;;){

	    medianFilter(&lidar);
	    find_clusters(&lidar);
	    printf("Clusters Maison trouvees : %d \r\n", lidar.processing.cluster_cnt);
	    vTaskDelay(DUREE);
	}

}
