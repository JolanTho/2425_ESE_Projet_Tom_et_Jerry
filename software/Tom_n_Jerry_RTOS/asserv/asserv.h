/*
 * asserv.h
 *
 *  Created on: Nov 20, 2024
 *      Author: romingo
 */

#ifndef ASSERV_H_
#define ASSERV_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "cmsis_os.h"
#include "string.h"
#include "callBack.h"

void asserv_init(void);
void asserv_courant_run(void*);
void asserv_position_run(void*);

#endif /* ASSERV_H_ */
