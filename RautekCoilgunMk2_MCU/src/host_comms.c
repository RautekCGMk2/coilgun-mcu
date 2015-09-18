/*
 * host_comms.c
 *
 *  Created on: Sep 15, 2015
 *      Author: scott
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-sign"

#include "host_comms.h"

#include <string.h>
#include <stdlib.h>

#define READ_BUFFER_LEN 64

#define SEND_PROTOCOL_CONFIRM() \
	if(host_send("confirmed", 9)) { \
		debug_print(DBG_INFO, \
				"Communications with host successfully established with protocol version %d\n", \
				PROTOCOL_VERSION); \
		return 1; \
	} else {\
		\
	}

int host_init_comms(unsigned long timeout, char versionCheck) {
	// Wait for the host to send some data
	unsigned char* buf;
	buf = malloc(sizeof(unsigned char) * READ_BUFFER_LEN);
	int readLen;
	unsigned long startTime = time_ms();
	unsigned long currTime = startTime;
	debug_print(DBG_DEBUG, "Initializing communications with host...\n");
	if (timeout)
		debug_print(DBG_DEBUG, "Will wait up to %d ms for a response\n",
				timeout);

	do {
		readLen = host_receive(buf, READ_BUFFER_LEN);
		currTime = time_ms();
		mcu_sleep(10);
	} while ((readLen <= 0)
			|| ((timeout > 0) && (currTime - startTime > timeout)));

	// check to see if we read data or we timed out
	if (readLen > 0) { // we didn't time out :)
		// check to see if it's the data we expected
		if (strncmp(buf, "api_ver:", 7) == 0) { // host started the reply properly
			buf += 7;
			if (!versionCheck) {
				debug_print(DBG_INFO,
						"Ignoring version check. Sending confirm...\n");
				SEND_PROTOCOL_CONFIRM();
			}
			if (strncmp(buf, STR(PROTOCOL_VERSION), PROTOCOL_VERSION_LEN)
					== 0) { // protocol version match
				debug_print(DBG_INFO,
						"Host protocol version matched. Sending confirm...\n");
				SEND_PROTOCOL_CONFIRM();
			} else { // protocol version mismatch
				debug_print(DBG_ERROR,
						"Host protocol version mismatch. Host running version %.3s, expected %s\n",
						buf, STR(PROTOCOL_VERSION));
				return PROTOCOL_ERROR_VERSION_MISMATCH;
			}
		} else { // host didn't reply the way we expected, return an error
			debug_print(DBG_FATAL,
					"Communications with host failed to initialize\n");
			return PROTOCOL_ERROR_DATA_CORRUPT;
		}
	} else { // We timed out
		debug_print(DBG_WARNING,
				"Timed out while waiting for comms with host. Waited %d ms\n",
				timeout);
		return 0;
	}

	return PROTOCOL_ERROR_UNKNOWN; // if we got here, something went horribly wrong
}

#pragma GCC diagnostic pop
