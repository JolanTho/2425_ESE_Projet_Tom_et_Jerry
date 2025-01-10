/*
 * init.c
 *
 *  Created on: Jan 9, 2025
 *      Author: romingo
 */

#include "init.h"
#include"components/LP5812.h"

void init_run(void*){
	shell_init();
	lidar_init();
	changement_mode_init();
	//chenillard_RGB();
}
