/*
 * host_comms.h
 *
 *  Created on: Sep 14, 2015
 *      Author: scott
 */

#ifndef HOST_COMMS_H_
#define HOST_COMMS_H_

#include <mcu_api.h>
#include <mcu_errno.h>

#include <stdint.h>

#include "pins.h"
#include "timing.h"


#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// 3 digits so that the string length is the same no matter how many versions we have
// I hope to god i don't have to update the protocol more than 1000 times
#define PROTOCOL_VERSION 					001
#define PROTOCOL_VERSION_LEN				3

#define PROTOCOL_ERROR_UNKNOWN 				-1
#define PROTOCOL_ERROR_VERSION_MISMATCH		-2
#define PROTOCOL_ERROR_DATA_CORRUPT			-4


/**
 * Initialize communications with the process running on the host
 * processor. Blocks until communications have been established or
 * timeout milliseconds have elapsed
 * @param timeout the maximum number of ms to wait for the. If timeout is 0,
 * 			don't time out
 * @return Returns 0 on success, 1 on timeout, or a negative number on error
 * @note
 * 		Error usually means host program and MCU program are out of date with each other
 */
int host_init_comms(unsigned long timeout, char versionCheck);

/**
 * Wake up the host CPU by using the host_send() function to trigger an interrupt
 * @note
 * 		No error checking for fanciness is done. It *should* always work though.
 * 		Host can be put to sleep with 'echo -n "mem" > /sys/power/state'
 */
void host_wake_host();

/**
 * Send timing data from a photogate to the host process
 */
int host_sendTimingData(int stage, uint64_t data);

/**
 * Send the timing data for all the gates at once.
 * Used to offload data after a shot
 */
int host_sendBulkTimingData(uint64_t* data);


#endif /* HOST_COMMS_H_ */
