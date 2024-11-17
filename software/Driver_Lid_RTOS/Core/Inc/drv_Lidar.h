#ifndef DRV_LIDAR_H_
#define DRV_LIDAR_H_

#include <stdint.h>
#include <stdio.h>

// Constantes
#define INFO_BUFF_SIZE 27
#define HEALTH_BUFF_SIZE 10
#define CMD_BUFF_SIZE 2

// Commandes pour le LiDAR
typedef enum LIDAR_command_enum
{
    CMD_BEGIN   = 0xA5, // Premier octet de la commande
	CMD_STOP    = 0x65, //Stop and stop scanning
    CMD_INFO    = 0x90, // Récupérer les informations de l'appareil
    CMD_HEALTH  = 0x91  // Récupérer l'état de santé de l'appareil
} LIDAR_command_t;

// Prototypes des fonctions de transmission et réception
typedef int (*LIDAR_transmit_drv_t)(uint8_t *p_data, uint16_t size);
typedef int (*LIDAR_poll_receive_drv_t)(uint8_t *p_data, uint16_t size);

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

// Structure pour les fonctions de communication
typedef struct {
    LIDAR_transmit_drv_t transmit;
    LIDAR_poll_receive_drv_t poll_receive;
} LIDAR_serial_drv_t;

// Structure principale du LiDAR
typedef struct {
    LIDAR_serial_drv_t serial_drv;
    LIDAR_device_info_t device_info;
    LIDAR_health_stat_t health_stat;
    uint8_t info_buff[INFO_BUFF_SIZE];
    uint8_t health_buff[HEALTH_BUFF_SIZE];
} h_LIDAR_t;

// Prototypes des fonctions
int LIDAR_stop(h_LIDAR_t * h_LIDAR);
int LIDAR_get_info(h_LIDAR_t *h_LIDAR);
int LIDAR_get_health_stat(h_LIDAR_t *h_LIDAR);

#endif /* DRV_LIDAR_H_ */
