/*
 * driverLidar.c
 *
 *  Created on: Dec 11, 2024
 *      Author: romingo
 */



#include "lidar.h"
#include "shell.h"
#define MIN_DISTANCE 40 //mm

//Stop and stop scanning
int LIDAR_stop(h_LIDAR_t *h_LIDAR) {
	uint8_t cmd_buff[CMD_BUFF_SIZE] = {CMD_BEGIN, CMD_STOP}; // Commande pour arrêter le LIDAR

	// Envoi de la commande pour arrêter le LIDAR
	if (h_LIDAR->serial_drv.it_transmit(cmd_buff, CMD_BUFF_SIZE) != 0) {
		printf("Erreur lors de l'envoi de la commande.\r\n");
		return -1;
	}

	// Attente que la transmission soit terminée
	uint32_t start_time = HAL_GetTick();
	while (!h_LIDAR->rx_flag_uart) {
		if (HAL_GetTick() - start_time > 1000) { // Timeout après 1000 ms
			printf("Timeout lors de la transmission UART.\r\n");
			return -2;
		}
	}

	return 0; // Commande envoyée avec succès
}

//Soft restart
//No response
int LIDAR_restart(h_LIDAR_t * h_LIDAR){
	debug(START,"RESTART LIDAR");
	uint8_t cmd_buff[CMD_BUFF_SIZE]={CMD_BEGIN,CMD_RESTART};
	// Envoi de la commande pour restart le LIDAR
	if (h_LIDAR->serial_drv.it_transmit(cmd_buff, CMD_BUFF_SIZE) != 0) {
		printf("Erreur lors de l'envoi de la commande.\r\n");
		return -1;
	}

	return 0;
}

int LIDAR_get_info(h_LIDAR_t *h_LIDAR) {
	uint8_t cmd_buff[CMD_BUFF_SIZE] = {CMD_BEGIN, CMD_INFO};

	// Envoi de la commande pour obtenir les informations
	if (h_LIDAR->serial_drv.it_transmit(cmd_buff, CMD_BUFF_SIZE) != 0) {
		printf("Erreur lors de l'envoi de la commande.\r\n");
		return -1;
	}

	// Préparation à la réception
	h_LIDAR->rx_flag_uart = 0; // Réinitialise le drapeau
	if (h_LIDAR->serial_drv.it_receive(h_LIDAR->info_buff, INFO_BUFF_SIZE) != 0) {
		printf("Erreur lors de la preparation de la réception.\r\n");
		return -2;
	}

	// Attente de la réception
	uint32_t start_time = HAL_GetTick();
	while (!h_LIDAR->rx_flag_uart) {
		if (HAL_GetTick() - start_time > 10000) { // Timeout de 1000 ms
			printf("Timeout lors de la reception.\r\n");
			return -3;
		}
	}

	// Analyse des données
	h_LIDAR->device_info.start_sign = (h_LIDAR->info_buff[0] << 8) | h_LIDAR->info_buff[1];
	h_LIDAR->device_info.length = (h_LIDAR->info_buff[2]) |
			(h_LIDAR->info_buff[3] << 8) |
			(h_LIDAR->info_buff[4] << 16);
	h_LIDAR->device_info.mode = h_LIDAR->info_buff[5] >> 6;
	h_LIDAR->device_info.type_code = h_LIDAR->info_buff[6];

	if (h_LIDAR->device_info.start_sign != 0xA55A) {
		printf("Signature de debut invalide : %04X\r\n", h_LIDAR->device_info.start_sign);
		return -4;
	}

	if (h_LIDAR->device_info.type_code != 0x04) {
		printf("Type code invalide : %X\r\n", h_LIDAR->device_info.type_code);
		return -5;
	}

	// Récupération des autres informations
	h_LIDAR->device_info.model = h_LIDAR->info_buff[7];
	snprintf(h_LIDAR->device_info.firmware, sizeof(h_LIDAR->device_info.firmware),
			"%d.%d", h_LIDAR->info_buff[8], h_LIDAR->info_buff[9]);
	h_LIDAR->device_info.hardware = h_LIDAR->info_buff[10];
	snprintf(h_LIDAR->device_info.serial, sizeof(h_LIDAR->device_info.serial),
			"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
			h_LIDAR->info_buff[11], h_LIDAR->info_buff[12], h_LIDAR->info_buff[13], h_LIDAR->info_buff[14],
			h_LIDAR->info_buff[15], h_LIDAR->info_buff[16], h_LIDAR->info_buff[17], h_LIDAR->info_buff[18],
			h_LIDAR->info_buff[19], h_LIDAR->info_buff[20], h_LIDAR->info_buff[21], h_LIDAR->info_buff[22],
			h_LIDAR->info_buff[23], h_LIDAR->info_buff[24], h_LIDAR->info_buff[25], h_LIDAR->info_buff[26]);


	// Affichage des informations
	printf("Start sign : %04X\r\n", h_LIDAR->device_info.start_sign);
	printf("Length : %lu\r\n", h_LIDAR->device_info.length);
	printf("Mode : %X\r\n", h_LIDAR->device_info.mode);
	printf("Type code : %X\r\n", h_LIDAR->device_info.type_code);
	printf("Model : %X\r\n", h_LIDAR->device_info.model);
	printf("Firmware version : %s\r\n", h_LIDAR->device_info.firmware);
	printf("Hardware version : %u\r\n", h_LIDAR->device_info.hardware);
	printf("Serial number : %s\r\n", h_LIDAR->device_info.serial);

	return 0;
}


int LIDAR_get_health_stat(h_LIDAR_t *h_LIDAR) {
	uint8_t cmd_buff[CMD_BUFF_SIZE] = {CMD_BEGIN, CMD_HEALTH};

	// Envoi de la commande pour obtenir l'état de santé
	if (h_LIDAR->serial_drv.it_transmit(cmd_buff, CMD_BUFF_SIZE) != 0) {
		printf("Erreur lors de l'envoi de la commande.\r\n");
		return -1;
	}

	// Préparation à la réception
	h_LIDAR->rx_flag_uart = 0; // Réinitialise le drapeau
	if (h_LIDAR->serial_drv.it_receive(h_LIDAR->health_buff, HEALTH_BUFF_SIZE) != 0) {
		printf("Erreur lors de la préparation de la réception.\r\n");
		return -2;
	}

	// Attente de la réception
	uint32_t start_time = HAL_GetTick();
	while (!h_LIDAR->rx_flag_uart) {
		if (HAL_GetTick() - start_time > 1000) { // Timeout de 1000 ms
			printf("Timeout lors de la reception.\r\n");
			return -3;
		}
	}

	// Analyse des données
	h_LIDAR->health_stat.start_sign = (h_LIDAR->health_buff[0] << 8) | h_LIDAR->health_buff[1];
	h_LIDAR->health_stat.length = (h_LIDAR->health_buff[2]) |
			(h_LIDAR->health_buff[3] << 8) |
			(h_LIDAR->health_buff[4] << 16);
	h_LIDAR->health_stat.mode = h_LIDAR->health_buff[5] >> 6;
	h_LIDAR->health_stat.type_code = h_LIDAR->health_buff[6];
	h_LIDAR->health_stat.status_code = h_LIDAR->health_buff[7];
	h_LIDAR->health_stat.error_code = h_LIDAR->health_buff[8] | (h_LIDAR->health_buff[9] << 8);

	if (h_LIDAR->health_stat.start_sign != 0xA55A) {
		printf("Signature de debut invalide : %04X\r\n", h_LIDAR->health_stat.start_sign);
		return -4;
	}

	if (h_LIDAR->health_stat.type_code != 0x06) {
		printf("Type code invalide : %X\r\n", h_LIDAR->health_stat.type_code);
		return -5;
	}

	// Affichage de l'état de santé
	printf("Start sign : %04X\r\n", h_LIDAR->health_stat.start_sign);
	printf("Length : %lu\r\n", h_LIDAR->health_stat.length);
	printf("Mode : %X\r\n", h_LIDAR->health_stat.mode);
	printf("Type code : %X\r\n", h_LIDAR->health_stat.type_code);
	printf("Status code : %02X\r\n", h_LIDAR->health_stat.status_code);
	printf("Error code : %04X\r\n", h_LIDAR->health_stat.error_code);

	return 0;
}

int LIDAR_start_scan_dma(h_LIDAR_t *h_LIDAR) {
	uint8_t cmd_buff[CMD_BUFF_SIZE] = {CMD_BEGIN, CMD_START}; // Commande pour démarrer le scan

	// Envoi de la commande
	if (h_LIDAR->serial_drv.it_transmit(cmd_buff, CMD_BUFF_SIZE) != 0) {
		printf("Erreur lors de l'envoi de la commande de démarrage du scan.\r\n");
		return -1;
	}

	// Configurer la réception DMA en mode circulaire
	if (h_LIDAR->serial_drv.dma_receive(h_LIDAR->processing.receive_buff, DATA_BUFF_SIZE) != 0) {
		printf("Erreur lors de la configuration de la réception DMA.\r\n");
		return -2;
	}

	printf("Scan DMA demarre avec succes en mode circulaire.\r\n");
	return 0;
}

void LIDAR_process_frame(h_LIDAR_t *LIDAR, uint8_t *buff) {
	// uint8_t *buff = LIDAR->processing.frame_buff; // Buffer circulaire DMA
	int buffer_size = FRAME_BUFF_SIZE;              // Taille totale du buffer
	int start_idx = 0;                             // Indice de départ pour parcourir le buffer

	while (start_idx < buffer_size) {

		if (buff[start_idx] == 0xAA && buff[(start_idx + 1) % buffer_size] == 0x55) {

			//printf("Trame trouvée \r\n");

			// L'entête est trouvé, extraire les métadonnées
			int header_idx = start_idx; // Index actuel pour début de trame

			// Start Angle
			uint16_t FSA = (buff[(header_idx + 4) % buffer_size] |
					(buff[(header_idx + 5) % buffer_size] << 8)) >>
							7; // Shift de 7 pour diviser de 64 + Shift à 1 pour avoir l'angle en degré

			// End Angle
			uint16_t LSA = (buff[(header_idx + 6) % buffer_size] |
					(buff[(header_idx + 7) % buffer_size] << 8)) >>
							7; // Shift de 7 pour diviser de 64 + Shift à 1 pour avoir l'angle en degré

			// Nombre de points
			uint8_t LSN = buff[(header_idx + 3) % buffer_size];

			// Calcul de la taille totale attendue de la trame
			int frame_size = 10 + LSN * 2; // 10 octets d'entête + 2 octets par point
			if (frame_size > buffer_size) {
				//                printf("Erreur : Taille de la trame (%d) dépasse la taille du buffer (%d).\r\n", frame_size, buffer_size);
				break;
			}

			// Vérifier si toute la trame est contenue dans le buffer
			if ((start_idx + frame_size) % buffer_size < start_idx) {
				//                printf("Trame partielle detectee en fin de buffer. Ignoree.\r\n");
				break;
			}

			//            // Vérification du checksum qui ne fonctionne jamais car mal fait
			//            uint16_t checksum_calculated = 0;
			//            for (int i = 0; i < frame_size - 2; i++) {
			//                checksum_calculated ^= buff[(header_idx + i) % buffer_size];
			//            }
			//            uint16_t checksum_received = (buff[(header_idx + 8) % buffer_size] |
			//                                          (buff[(header_idx + 9) % buffer_size] << 8)) >>
			//                                        		  1;
			//
			//            if (checksum_calculated != checksum_received) {
			//                printf("Erreur : Checksum invalide. Calcule: 0x%04X, Recu: 0x%04X\r\n", checksum_calculated, checksum_received);
			//                start_idx += 1; // Avancer pour trouver la prochaine trame valide
			//                continue;
			//            }

			// Traiter les données de la trame

			for (int i = 0; i < LSN; i++) {
				// Lecture de la distance brute
				uint16_t Si = buff[(header_idx + 10 + i * 2) % buffer_size] |
						(buff[(header_idx + 11 + i * 2) % buffer_size] << 8);


				int Di = Si / 4; // Distance réelle en mm

				// Calcul de l'angle
				int Ai = (i+1)*abs(LSA-FSA)/(LSN-1) + FSA;

				if (Ai < 0 || Ai >= 360) {
					continue;
				}

				// Stockage dans le buffer des points
				if (Di < 50 || Di > 4000) {
					LIDAR->processing.point_buff[Ai] = 0; // Distance hors plage
				} else {
					LIDAR->processing.point_buff[Ai] = Di; // Distance valide
				}
			}

			//            printf("Trame traitee : FSA=%d, LSA=%d, Points=%d\r\n", FSA, LSA, LSN);

			// Avancer dans le buffer jusqu'à la fin de la trame traitée
			start_idx += frame_size;
		} else {
			// Avancer d'un octet si aucune entête valide n'est trouvée
			start_idx++;
		}
	}

}

// Permet de calculer la distance moyenen sur un tableau entre deux indices
int calculer_distance_moyenne(const int *distances, int debut, int fin) {
	int somme = 0;
	int n = fin - debut + 1;
	for (int i = debut; i <= fin; i++) {
		somme += distances[i];
	}
	return somme / n;
}

/**
 * @brief Trouve les clusters dans les données du LIDAR.
 * @param LIDAR Pointeur vers la structure h_LIDAR_t.
 */
void find_clusters(h_LIDAR_t *LIDAR) {
	int *distances = LIDAR->processing.filtred_buff;
	int cluster_count = 0;
	int debut_cluster = 0;
	LIDAR->processing.cluster_DistanceMIN = (Cluster_t){.angle_moyen=0,.distance_moyenne=4000,.count=0};
	// Parcours des distances pour identifier les clusters
	for (int i = 1; i < NB_DEGRES; i++) {
		// Si la différence dépasse le seuil, un cluster est terminé
		if (fabs(distances[i] - distances[i - 1]) > CLUSTER_SEUIL) {
			// Calcul et stockage des informations du cluster
			int distance_moyenne= calculer_distance_moyenne(distances, debut_cluster, i - 1);
			if (cluster_count < MAX_CLUSTERS && distance_moyenne > MIN_DISTANCE) {
				LIDAR->processing.clusters[cluster_count].distance_moyenne = distance_moyenne;
				LIDAR->processing.clusters[cluster_count].angle_moyen = (debut_cluster + i - 1) / 2;
				LIDAR->processing.clusters[cluster_count].count = i - debut_cluster;
				cluster_count++;
				if (distance_moyenne <  LIDAR->processing.cluster_DistanceMIN.distance_moyenne){
					LIDAR->processing.cluster_DistanceMIN.distance_moyenne = distance_moyenne;
					LIDAR->processing.cluster_DistanceMIN.angle_moyen = (debut_cluster + i - 1) / 2;
					LIDAR->processing.cluster_DistanceMIN.count = i - debut_cluster;
				}
			}
			debut_cluster = i; // Nouveau cluster
		}
	}

	// Traitement du dernier cluster
	int distance_last = calculer_distance_moyenne(distances, debut_cluster, NB_DEGRES - 1);
	if (cluster_count < MAX_CLUSTERS && distance_last > MIN_DISTANCE ) {
		LIDAR->processing.clusters[cluster_count].distance_moyenne = distance_last;
		LIDAR->processing.clusters[cluster_count].angle_moyen = (debut_cluster + NB_DEGRES - 1) / 2;
		LIDAR->processing.clusters[cluster_count].count = NB_DEGRES - debut_cluster;
		cluster_count++;
		if (distance_last <  LIDAR->processing.cluster_DistanceMIN.distance_moyenne){
			LIDAR->processing.cluster_DistanceMIN = LIDAR->processing.clusters[cluster_count];
		}
	}

	// Mise à jour du compteur de clusters
	LIDAR->processing.cluster_cnt = cluster_count;

}

//Applique un filtre médian aux données du LIDAR.
void medianFilter(h_LIDAR_t *LIDAR) {
	int *signal = LIDAR->processing.point_buff;     // Signal brut
	int *filtred = LIDAR->processing.filtred_buff; // Signal filtré
	int signal_length = NB_DEGRES;
	int window[5];
	int middle = 2; // La médiane est au centre d'une fenêtre de taille 5

	for (int i = 0; i < signal_length; i++) {
		// Construire la fenêtre de voisinage
		for (int j = 0; j < 5; j++) {
			int index = i - middle + j;
			// Gérer les bords du signal
			if (index < 0) index = 0;
			if (index >= signal_length) index = signal_length - 1;
			window[j] = signal[index];
		}

		// Trier la fenêtre pour extraire la médiane
		for (int j = 0; j < 5; j++) {
			for (int k = j + 1; k < 5; k++) {
				if (window[j] > window[k]) {
					int temp = window[j];
					window[j] = window[k];
					window[k] = temp;
				}
			}
		}

		// Stocker la médiane dans le tableau filtré
		filtred[i] = window[middle];
	}
}



// Définir les fonctions UART pour le mode polling
int uart_transmit(uint8_t *p_data, uint16_t size) {
	return HAL_UART_Transmit(&huart2, p_data, size, HAL_MAX_DELAY);
}

int uart_poll_receive(uint8_t *p_data, uint16_t size) {
	return HAL_UART_Receive(&huart2, p_data, size, 100);
}

int uart_it_receive(uint8_t *p_data, uint16_t size){
	HAL_UART_Receive_IT(&huart2, p_data, size);
	return 0;
}

int uart_it_transmit(uint8_t *p_data, uint16_t size){
	HAL_UART_Transmit_IT(&huart2, p_data, size);
	return 0;
}

int uart_dma_receive(uint8_t *p_data, uint16_t size){
	HAL_UART_Receive_DMA(&huart2, p_data, size);
	return 0;
}

int uart_dma_transmit(uint8_t *p_data, uint16_t size){
	HAL_UART_Transmit_DMA(&huart2, p_data, size);
	return 0;
}
