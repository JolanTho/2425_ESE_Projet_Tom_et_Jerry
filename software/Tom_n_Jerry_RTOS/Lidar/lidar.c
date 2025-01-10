/*
 * lidar.c
 *
 *  Created on: Dec 4, 2024
 *      Author: romingo
 */


#include "lidar.h"
#include "components/ZXB5210.h"
#include "shell.h"

#define DUREE 10
#define DUREEP 30000

h_LIDAR_t lidar;

extern SemaphoreHandle_t semb_halfCllbck;
extern SemaphoreHandle_t semb_cpltCllbck;
extern int lidarDebugShell;

SemaphoreHandle_t semb_process_authorized;
void lidar_init(void){

	HAL_GPIO_WritePin(GPIOA,LIDAR_M_CTR_Pin , SET);

	lidar.serial_drv.transmit = uart_transmit;
	lidar.serial_drv.poll_receive = uart_poll_receive;

	lidar.serial_drv.it_receive=uart_it_receive;
	lidar.serial_drv.it_transmit=uart_it_transmit;

	lidar.serial_drv.dma_receive=uart_dma_receive;
	lidar.serial_drv.dma_transmit=uart_dma_transmit;

	semb_process_authorized = xSemaphoreCreateBinary();
}

void lidarTake (void * pvParameters){
	uint8_t* buff;
	//Remove in the shell
	//LIDAR_start_scan_dma(&lidar) == 0 ? debug(START,"LIDAR") : debug(D_ERROR,"LIDAR");
	SemaphoreHandle_t semMutex_Process=xSemaphoreCreateMutex();
	for(;;){
		// Attente du Semahore de half buffer
		xSemaphoreTake(semb_halfCllbck, portMAX_DELAY);
		// Pointeur sur le début des données
		buff = lidar.processing.receive_buff;
		xSemaphoreTake(semMutex_Process,portMAX_DELAY);
		LIDAR_process_frame(&lidar, buff); //Peut provoquer des HardFault sans les mutexs
		xSemaphoreGive(semMutex_Process);

		xSemaphoreGive(semb_process_authorized);
		// Attente du semaphore du complete buffer
		xSemaphoreTake(semb_cpltCllbck, portMAX_DELAY);
		// Pointeur sur la moitié des données
		buff = &lidar.processing.receive_buff[FRAME_BUFF_SIZE];

		xSemaphoreTake(semMutex_Process,portMAX_DELAY);
		LIDAR_process_frame(&lidar, buff);
		xSemaphoreGive(semMutex_Process);

		xSemaphoreGive(semb_process_authorized);
	}
}

void lidarprocess (void * pvParameters){
	for(;;){
		medianFilter(&lidar);
		find_clusters(&lidar);
		// Changer la distance minimale si workMode idee tab_distance[distance_souris, distance_chat]
		if (lidar.processing.cluster_DistanceMIN.distance_moyenne < 4000){
			ZXB5210_angle(lidar.processing.cluster_DistanceMIN.angle_moyen);
		}

		if(lidarDebugShell){
			printf("Clusters Maison trouvees : %d \r\n", lidar.processing.cluster_cnt);
			printf("Clusters Distance Minimum : %i | %i° \r\n", lidar.processing.cluster_DistanceMIN.distance_moyenne,lidar.processing.cluster_DistanceMIN.angle_moyen);

		}
		vTaskDelay(DUREE);
	}

}
