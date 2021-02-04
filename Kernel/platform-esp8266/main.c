#include <stdint.h>
#include <eagle_soc.h>
#include "esp8266_peri.h"
#include "kernel.h"
#include "kdata.h"
#include "printf.h"
#include "globals.h"
#include "rom.h"
#include "kernel-lx106.def"

volatile uint32_t* const esp8266_gpioToFn[16] = { &GPF0, &GPF1, &GPF2, &GPF3, &GPF4, &GPF5, &GPF6, &GPF7, &GPF8, &GPF9, &GPF10, &GPF11, &GPF12, &GPF13, &GPF14, &GPF15 };

uaddr_t ramtop = DATATOP;
uint8_t need_resched;

void map_init(void) {}
void platform_discard(void) {}
void platform_monitor(void) {}
void platform_reboot(void) {}

uint_fast8_t platform_param(char* p)
{
	return 0;
}

int main(void)
{
	/* Check offsets */

	if ((U_DATA__U_SP_OFFSET != offsetof(struct u_data, u_sp)) ||
	    (U_DATA__U_PTAB_OFFSET != offsetof(struct u_data, u_ptab)) ||
	    (P_TAB__P_PID_OFFSET != offsetof(struct p_tab, p_pid)) ||
		(P_TAB__P_STATUS_OFFSET != offsetof(struct p_tab, p_status)))
	{
		kprintf("U_DATA__U_SP = %d\n", offsetof(struct u_data, u_sp));
		kprintf("U_DATA__U_PTAB = %d\n", offsetof(struct u_data, u_ptab));
		kprintf("P_TAB__P_PID_OFFSET = %d\n", offsetof(struct p_tab, p_pid));
		kprintf("P_TAB__P_STATUS_OFFSET = %d\n", offsetof(struct p_tab, p_status));
		panic("bad offsets");
	}
	
	ramsize = 80;
	procmem = 64;
    sys_cpu_feat = AF_LX106_ESP8266;

	di();
	fuzix_main();
}

