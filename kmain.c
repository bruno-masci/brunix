#include "monitor.h"

#include <asm/segment.h>

#include "descriptor_tables.h"


int kmain() {
	monitor_write("Starting Brunix...\n\n");

	init_descriptor_tables();


	monitor_write_dec((uint32_t)12345);
	monitor_write_hex((uint32_t)1024);

	asm volatile ("int $0x1");

	return 0;
}
