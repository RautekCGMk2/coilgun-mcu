#include "mcu_api.h"
#include "mcu_errno.h"

#include "host_comms.h"


void mcu_main() {
	/* your configuration code starts here */
	// init pins
	// establish communications with host
	int result = 0;
	do {
		result = host_init_comms(10 * 1000, 1);
		if (result < 0) { // only need to print if it's an error. API prints on timeout
			debug_print(DBG_ERROR,
					"Host communications failed to initialize. Error code:%d\n",
					result);
		}
	} while (result);
	debug_print(DBG_INFO, "Successfully established communications with host processor\n");

	while (1) /* your loop code starts here */
	{
	}
}
