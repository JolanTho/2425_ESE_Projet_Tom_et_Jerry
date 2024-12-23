/*
 * LP5812.c
 *
 *  Created on: Nov 13, 2024
 *      Author: romingo
 *
 *      Datasheet 		: https://www.ti.com/lit/ds/symlink/lp5812.pdf?ts=1731565478746&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLP5812
 *      Register Map 	: https://www.ti.com/lit/ug/snvu861/snvu861.pdf?ts=1731598783415&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLP5812%253Fdcmp%253Ddsproject%2526hqs%253D
 *
 */

#include "components/LP5812.h"

// Adresse I2C du LP5812 (vérifiez la documentation pour la bonne adresse)
#define LP5812_I2C_BASE_ADDR 0x1B
#define LP5812_I2C_INDEP_ADDR 0x1C
#define I2C_MAIN hi2c3

LED_t LED;

// Fonctions de configuration du LP5812
void LP5812_Init(void) {
	debug(INFORMATION,"LP5812 - INIT");
	// Activer les LEDs en écrivant dans le registre ENABLE
	/*
	 * Set chip_en = 1 to enable the device
	 */
	LP5812_WriteRegister(0x000,0x01)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	/*
	 * Set led_mode = 4h to configure the LED drive mode as direct drive mode.
	 */
	LP5812_WriteRegister(0x002,0x40)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	/*
	 * Send update command to complete configuration settings
	 */
	LP5812_WriteRegister(0x010,0x55)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;

	//Lire le registre 300h pou être certain que la configuration est bonne

	/*
	 * Enable all 12 LEDs (Write F0h to register 020h and FFh to register 021h)
	 */
	LP5812_WriteRegister(0x020,0xF0)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	LP5812_WriteRegister(0x021,0xFF)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	/*
	 * 	Set 51mA peak current for red LEDs (Write FFh to registers 035h, 038h, 03Bh, 03Eh) , and 40mA peak
	 *	current for green and blue LEDs (Write CCh to registers 034h, 036h, 037h, 039h, 03Ah, 03Ch, 03Dh,
		03Fh)
	 */
	LP5812_WriteRegister(0x035,0xFF)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	LP5812_WriteRegister(0x038,0xFF)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	LP5812_WriteRegister(0x03B,0xFF)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	LP5812_WriteRegister(0x03E,0xFF)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	/*	RED - BLUE And GREEN	*/
	LP5812_WriteRegister(0x034,0xCC)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	LP5812_WriteRegister(0x036,0xCC)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	LP5812_WriteRegister(0x037,0xCC)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	LP5812_WriteRegister(0x039,0xCC)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	LP5812_WriteRegister(0x03A,0xCC)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	LP5812_WriteRegister(0x03C,0xCC)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	LP5812_WriteRegister(0x03D,0xCC)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;
	LP5812_WriteRegister(0x03F,0xCC)!=HAL_OK ? debug(D_ERROR,"I2C MAIN"):	(void)0;


	LED.L1.R=0x47;

	LED.L1.R=0x44;
	LED.L1.G=0x45;
	LED.L1.B=0x44; // et 0x45

	LED.L2.R=0x47;
	LED.L2.G=0x48;
	LED.L2.B=0x46; //

	LED.L3.R=0x4A;
	LED.L3.G=0x4B;
	LED.L3.B=0x4C;

	LED.L4.R=0x4D;
	LED.L4.G=0x4E;
	LED.L4.B=0x4F;

	LED_update(LED.L1.R,0);
	LED_update(LED.L2.R,0);
	LED_update(LED.L3.R,0);
	LED_update(LED.L4.R,0);
	LED_update(LED.L1.B,0);
	LED_update(LED.L2.B,0);
	LED_update(LED.L3.B,0);
	LED_update(LED.L4.B,0);
	LED_update(LED.L1.G,0);
	LED_update(LED.L2.G,0);
	LED_update(LED.L3.G,0);
	LED_update(LED.L4.G,0);
// Changer les registres carRGB pas dans le bon sens.



	/*//!\\Attention : L'adresse du composant est sur 2 bytes :
	 *				1er bytes est composé de R/W +  2dernier bytes du registres ciblé + Broadcast/Speci
	 *				2eme byte composé du reste du registre cible
	 */
}

// Fonction pour écrire dans un registre 16 bits du LP5812
HAL_StatusTypeDef LP5812_WriteRegister(uint16_t reg, uint8_t data) {
	uint8_t addr_byte1, addr_byte2;
	uint8_t buffer[3];

	// Configuration des Address Byte 1 et Address Byte 2
	addr_byte1 = (LP5812_I2C_BASE_ADDR << 3) | ((reg >> 8) & 0x03);  // 5 bits d'adresse et 2 bits supérieurs du registre
	addr_byte2 = (uint8_t)(reg & 0xFF);                               // 8 bits inférieurs du registre

	// Préparer les données à envoyer : Address Byte 2 et Data
	buffer[0] = addr_byte2;
	buffer[1] = data;

	// Envoyer Address Byte 1, puis buffer
	return HAL_I2C_Master_Transmit(&hi2c3, addr_byte1, buffer, 2, HAL_MAX_DELAY);
}

HAL_StatusTypeDef LP5812_ReadRegister(uint16_t reg_addr, uint8_t *pData) {
	uint8_t addr_byte1_w,addr_byte1_r,addr_byte2;

	// Configuration des Address Byte 1 et Address Byte 2
	addr_byte1_w = (LP5812_I2C_BASE_ADDR << 3) | ((reg_addr >> 8) & 0x03);  // 5 bits d'adresse et 2 bits supérieurs du registre
	addr_byte1_r = (LP5812_I2C_BASE_ADDR << 3) | ((reg_addr >> 8) & 0x03);
	addr_byte2 = (uint8_t)(reg_addr & 0xFF);                          // 8 bits inférieurs du registre

	HAL_I2C_Master_Transmit(&I2C_MAIN, addr_byte1_w, &addr_byte2, 1, HAL_MAX_DELAY) !=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in ReadREGISTER"):(void)0;
	return HAL_I2C_Master_Receive(&I2C_MAIN, (addr_byte1_r << 1) | 0x01, pData, 1, HAL_MAX_DELAY);
}


void LED_update(uint16_t regRGB, uint8_t RGB){
	LP5812_WriteRegister(regRGB,RGB);
}
