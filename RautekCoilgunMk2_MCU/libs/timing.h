/*
 * timing.h
 *
 * Contains all the functions and
 *
 *  Created on: Sep 15, 2015
 *      Author: scott
 */

#ifndef TIMING_H_
#define TIMING_H_

typedef struct {
	char gateNum;
	uint32_t time;
} gate_time_t;

typedef struct {
	char coilNum;
	uint32_t timingOverride;
	char gateOverride;
	gate_time_t idealTime;
} timing_data_t;

#endif /* TIMING_H_ */
