#include "monitor.h"

int kmain() {
	monitor_write("Starting Brunix...\n\n");
	monitor_write("...Ready!\n\n");
//	monitor_write_dec((u32int)12345);
	return 0;
}