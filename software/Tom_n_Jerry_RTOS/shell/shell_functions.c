/*
 * shell_functions.c
 *
 *  Created on: Oct 23, 2024
 *      Author: romingo
 */
#include "shell.h"
#include "stdio.h"
#include "stdlib.h"

int fonction(int argc, char ** argv)
{
	for (int i = 1; i<argc; i++){
		printf("argv[%d]: %s\r\n",i, argv[i]);
	}
	return 0;
}

int addition(int argc, char ** argv){
	uint32_t sum = 0;
	if (argc<=1){
		return 0;
	}
	for (int i = 1; i<argc; i++){
		sum = sum+atoi(argv[i]);
	}
	printf("++ >%lu\r\n",sum);
	return 0;
}
