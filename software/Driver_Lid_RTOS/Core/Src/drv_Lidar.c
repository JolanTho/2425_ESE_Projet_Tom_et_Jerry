#include "drv_LIDAR.h"
#include <stdio.h>

//Stop and stop scanning
//No answer
int LIDAR_stop(h_LIDAR_t * h_LIDAR){
	uint8_t cmd_buff[CMD_BUFF_SIZE]={CMD_BEGIN,CMD_STOP};
	h_LIDAR->serial_drv.transmit(cmd_buff,CMD_BUFF_SIZE);
	return 0;
}

// Récupérer les informations de l'appareil
int LIDAR_get_info(h_LIDAR_t *h_LIDAR) {
    uint8_t cmd_buff[CMD_BUFF_SIZE] = {CMD_BEGIN,CMD_INFO};

    // Envoi de la commande pour obtenir les informations
    if (h_LIDAR->serial_drv.transmit(cmd_buff, CMD_BUFF_SIZE) != 0) {
        printf("Erreur lors de l'envoi de la commande.\r\n");
        return -1;
    }

    // Réception des informations dans le buffer
    if (h_LIDAR->serial_drv.poll_receive(h_LIDAR->info_buff, INFO_BUFF_SIZE) != 0) {
        printf("Erreur lors de la reception des donnees.\r\n");
        return -2;
    }

    // Analyse des données
    h_LIDAR->device_info.start_sign = (h_LIDAR->info_buff[0] << 8) | h_LIDAR->info_buff[1];
    h_LIDAR->device_info.length = (h_LIDAR->info_buff[2]) |
                                  (h_LIDAR->info_buff[3] << 8) |
                                  (h_LIDAR->info_buff[4] << 16);
    h_LIDAR->device_info.mode = h_LIDAR->info_buff[5] >> 6;
    h_LIDAR->device_info.type_code = h_LIDAR->info_buff[6];

    // Vérification de la validité de la réponse
    if (h_LIDAR->device_info.start_sign != 0xA55A) {
        printf("Signature de début invalide : %04X\r\n", h_LIDAR->device_info.start_sign);
        return -3;
    }

    if (h_LIDAR->device_info.type_code != 0x04) {
        printf("Type code invalide : %X\r\n", h_LIDAR->device_info.type_code);
        return -4;
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
    if (h_LIDAR->serial_drv.transmit(cmd_buff, CMD_BUFF_SIZE) != 0) {
        printf("Erreur lors de l'envoi de la commande.\r\n");
        return -1;
    }

    // Réception de l'état de santé
    if (h_LIDAR->serial_drv.poll_receive(h_LIDAR->health_buff, HEALTH_BUFF_SIZE) != 0) {
        printf("Erreur lors de la réception des données.\r\n");
        return -2;
    }

    // Analyse des données reçues
    h_LIDAR->health_stat.start_sign = (h_LIDAR->health_buff[0] << 8) | h_LIDAR->health_buff[1];
    h_LIDAR->health_stat.length = (h_LIDAR->health_buff[2]) |
                                  (h_LIDAR->health_buff[3] << 8) |
                                  (h_LIDAR->health_buff[4] << 16);
    h_LIDAR->health_stat.mode = h_LIDAR->health_buff[5] >> 6;
    h_LIDAR->health_stat.type_code = h_LIDAR->health_buff[6];
    h_LIDAR->health_stat.status_code = h_LIDAR->health_buff[7];
    h_LIDAR->health_stat.error_code = h_LIDAR->health_buff[8] | (h_LIDAR->health_buff[9] << 8);

    // Vérifications des données
    if (h_LIDAR->health_stat.start_sign != 0xA55A) {
        printf("Signature de début invalide : %04X\r\n", h_LIDAR->health_stat.start_sign);
        return -3;
    }

    if (h_LIDAR->health_stat.type_code != 0x06) {
        printf("Type code invalide : %X\r\n", h_LIDAR->health_stat.type_code);
        return -4;
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

