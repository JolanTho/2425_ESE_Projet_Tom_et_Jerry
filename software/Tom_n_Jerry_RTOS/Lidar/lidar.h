/*
 * lidar.h
 *
 *  Created on: Dec 4, 2024
 *      Author: romingo
 */

#ifndef LIDAR_H_
#define LIDAR_H_


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "usart.h"
#include "gpio.h"
#include "cmsis_os.h"



// Super Define
#define INFO_BUFF_SIZE 27 // Taille de résultat INFO
#define HEALTH_BUFF_SIZE 10 // Taille de résultat HEALTH
#define CMD_BUFF_SIZE 2 // Taille de CMD


#define DATA_BUFF_SIZE 4000 // Taille du buffer de données recus
#define FRAME_BUFF_SIZE 2000	// Taille du demi buffer de données recus
#define POINT_BUFF_SIZE 360 // Taille des données LIDAR
#define NB_DEGRES 360 // Les degres de traitement

// Methode de cluster maison
#define CLUSTER_SEUIL 30   // Seuil pour la différence de distance entre deux points pour définir un nouveau cluster
// Erreur systematique de 2cm on prend 30mm pour etre large.
#define MAX_CLUSTERS 100    // Nombre maximum de clusters gérés

// Methode de cluster jolie
#define MAX_K 20
#define MAX_ITERATIONS 100


// Commandes pour le LiDAR
typedef enum LIDAR_command_enum
{
	CMD_BEGIN   = 0xA5, // Octet de départ pour toutes les commandes
	CMD_START   = 0x60, // Démarrer le scan
	CMD_STOP    = 0x65, // Arrêter le scan
	CMD_INFO    = 0x90, // Obtenir les informations de l'appareil
	CMD_HEALTH  = 0x91, // Obtenir l'état de santé
	CMD_RESTART = 0x80  // Redémarrer l'appareil
} LIDAR_command_t;

// Prototypes des fonctions de transmission et réception
typedef int (*LIDAR_transmit_drv_t)(uint8_t *p_data, uint16_t size);
typedef int (*LIDAR_poll_receive_drv_t)(uint8_t *p_data, uint16_t size);

typedef int (* LIDAR_it_receive_drv_t)(uint8_t *p_data, uint16_t size);
typedef int (* LIDAR_it_transmit_drv_t)(uint8_t *p_data, uint16_t size);

typedef int (* LIDAR_DMA_receive_drv_t)(uint8_t *p_data, uint16_t size);
typedef int (* LIDAR_dma_transmit_drv_t)(uint8_t *p_data, uint16_t size);

// Structure pour stocker les informations de l'appareil
typedef struct {
	uint16_t start_sign;
	uint32_t length;
	uint8_t mode;
	uint8_t type_code;
	uint8_t model;
	char firmware[6];
	uint8_t hardware;
	char serial[17];
} LIDAR_device_info_t;

// Structure pour stocker l'état de santé de l'appareil
typedef struct {
	uint16_t start_sign;
	uint32_t length;
	uint8_t mode;
	uint8_t type_code;
	uint8_t status_code;
	uint16_t error_code;
} LIDAR_health_stat_t;

// Structure d'un cluster de points
typedef struct {
	int angle_moyen;       // Angle moyen du cluster
	int distance_moyenne;  // Distance moyenne du cluster
	int count;             // Nombre de points dans le cluster
} Cluster_t;


typedef struct LIDAR_processing_struct {
	//Header
	uint16_t PH;

	//CT Package Type / fréquence
	uint8_t CT;

	//LSN nombre de points de la trame
	uint8_t LSN;

	//FSA Start Angle
	uint16_t FSA;

	//LSA End Angle
	uint16_t LSA;

	//CS Check Code
	uint16_t CS;

	//Frame index
	uint8_t idx;

	// Definition pour récuperer les data
	uint8_t frame_buff[FRAME_BUFF_SIZE]; // Buffer pour stocker une frame
	int point_buff[POINT_BUFF_SIZE];     // Tableau pour les 360 angles
	uint8_t receive_buff[DATA_BUFF_SIZE]; // Buffer pour la réception DMA

	// Definition pour traiter les data
	int filtred_buff[NB_DEGRES];      // Points filtrés
	Cluster_t clusters[MAX_CLUSTERS]; // Liste des clusters
	int cluster_cnt;
	Cluster_t cluster_DistanceMIN;


} LIDAR_processing_t;

// Structure pour les fonctions de communication
typedef struct {
	LIDAR_transmit_drv_t transmit;
	LIDAR_poll_receive_drv_t poll_receive;

	LIDAR_it_receive_drv_t it_receive;
	LIDAR_transmit_drv_t it_transmit;

	LIDAR_DMA_receive_drv_t dma_receive;
	LIDAR_transmit_drv_t dma_transmit;

} LIDAR_serial_drv_t;

// Structure principale du LiDAR
typedef struct {
	LIDAR_serial_drv_t serial_drv;
	LIDAR_device_info_t device_info;
	LIDAR_health_stat_t health_stat;

	uint8_t info_buff[INFO_BUFF_SIZE];
	uint8_t health_buff[HEALTH_BUFF_SIZE];

	//Traitement des trames
	LIDAR_processing_t processing;

	volatile uint8_t rx_flag_uart;  // Flag pour UART en mode IT
	volatile uint8_t rx_flag_dma;   // Flag pour UART en mode DMA
} h_LIDAR_t;

void lidar_init(void);
void lidar_run(void*);

void lidarprocess (void * pvParameters);
void lidarTake (void * pvParameters);
void counter (void * pvParameters);

// Prototypes des fonctions
int LIDAR_stop(h_LIDAR_t * h_LIDAR);
int LIDAR_restart(h_LIDAR_t * h_LIDAR);
int LIDAR_get_info(h_LIDAR_t *h_LIDAR);
int LIDAR_get_health_stat(h_LIDAR_t *h_LIDAR);

// Fonctions pour récuperer les valeurs du lidar
int LIDAR_start_scan_dma(h_LIDAR_t *h_LIDAR);
void LIDAR_process_frame(h_LIDAR_t *h_LIDAR, uint8_t *buff);

// Méthode cluster maison
void medianFilter(h_LIDAR_t *LIDAR);
void find_clusters(h_LIDAR_t *LIDAR);

// Méthode du K_Means
void kMeansClustering(h_LIDAR_t *LIDAR);

// Définir les fonctions UART pour le mode polling
int uart_transmit(uint8_t *p_data, uint16_t size);
int uart_poll_receive(uint8_t *p_data, uint16_t size);
int uart_it_receive(uint8_t *p_data, uint16_t size);
int uart_it_transmit(uint8_t *p_data, uint16_t size);
int uart_dma_receive(uint8_t *p_data, uint16_t size);
int uart_dma_transmit(uint8_t *p_data, uint16_t size);

#endif /* LIDAR_H_ */
