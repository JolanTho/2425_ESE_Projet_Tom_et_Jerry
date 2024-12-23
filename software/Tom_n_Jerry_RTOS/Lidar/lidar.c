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
extern MDriver_t MDriver1;
extern MDriver_t MDriver2;
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
		ZXB5210_angle(lidar.processing.cluster_DistanceMIN.angle_moyen);

		if(lidarDebugShell){
			printf("Clusters Maison trouvees : %d \r\n", lidar.processing.cluster_cnt);
			printf("Clusters Distance Minimum : %i | %i° \r\n", lidar.processing.cluster_DistanceMIN.distance_moyenne,lidar.processing.cluster_DistanceMIN.angle_moyen);
			printf("FWD1 | pulseGoal: %lu | pulse: %lu\r\n", MDriver1.FWD->pulseGoal, *(MDriver1.FWD->CCR_Channel));
			printf("REV1 | pulseGoal: %lu | pulse: %lu\r\n", MDriver1.REV->pulseGoal, *(MDriver1.REV->CCR_Channel));
			printf("FWD2 | pulseGoal: %lu | pulse: %lu\r\n", MDriver2.FWD->pulseGoal, *(MDriver2.FWD->CCR_Channel));
			printf("REV2 | pulseGoal: %lu | pulse: %lu\r\n", MDriver2.REV->pulseGoal, *(MDriver2.REV->CCR_Channel));
		}
		vTaskDelay(DUREE);
	}

}
