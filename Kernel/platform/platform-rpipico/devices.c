#include <kernel.h>
#include <version.h>
#include <kdata.h>
#include <devsys.h>
#include <blkdev.h>
#include <tty.h>
#include <devtty.h>
#include <dev/devsd.h>
#include <printf.h>
#include "globals.h"
#include "picosdk.h"
#include <hardware/irq.h>
#include <pico/multicore.h>
#include "core1.h"

int simple_open(uint_fast8_t minor, uint16_t flag);
int simple_close(uint_fast8_t minor);
int simple_rdwr(uint_fast8_t minor, uint_fast8_t rawflag, uint_fast8_t flag);
int simple_ioctl(uint_fast8_t minor, uarg_t a, char *b);

struct devsw dev_tab[] = /* The device driver switch table */
    {
        // minor    open         close        read      write           ioctl
        // ---------------------------------------------------------------------
        /* 0: /dev/hd - block device interface */
        {blkdev_open, no_close, blkdev_read, blkdev_write, blkdev_ioctl},
        /* 1: /dev/fd - Floppy disk block devices */
        {no_open, no_close, no_rdwr, no_rdwr, no_ioctl},
        /* 2: /dev/tty	TTY devices */
        {tty_open, tty_close, tty_read, tty_write, tty_ioctl},
        /* 3: /dev/lpr	Printer devices */
        {no_open, no_close, no_rdwr, no_rdwr, no_ioctl},
        /* 4: /dev/mem etc	System devices (one offs) */
        {no_open, no_close, sys_read, sys_write, sys_ioctl},
        /* Pack to 7 with nxio if adding private devices and start at 8 */
        {nxio_open, no_close, no_rdwr, no_rdwr, no_ioctl},
        {nxio_open, no_close, no_rdwr, no_rdwr, no_ioctl},
        {nxio_open, no_close, no_rdwr, no_rdwr, no_ioctl},
        /* 8: */
        // you need to create a device node from the shell, before using it:
        // mknod /dev/test c 8 1
        // 
        // Could the OS take care of that? We invoke mknod when creating
        // the FS image, in update-flash.sh
        // {simple_open, simple_close, simple_rdwr, simple_rdwr, simple_ioctl}
};

static absolute_time_t now;

bool validdev(uint16_t dev) {
  /* This is a bit uglier than needed but the right hand side is
     a constant this way */
  if (dev > ((sizeof(dev_tab) / sizeof(struct devsw)) << 8) - 1)
    return false;
  else
    return true;
}

// int simple_open(uint_fast8_t minor, uint16_t flag) {
//     kprintf("hello\n");
//     return 0;
// }

// int simple_close(uint_fast8_t minor) {
// 	kprintf("hello\n");
// 	return 0;
// }

// int simple_rdwr(uint_fast8_t minor, uint_fast8_t rawflag, uint_fast8_t flag) {
//     kprintf("hello\n");
// 	return 0;
// }

// int simple_ioctl(uint_fast8_t minor, uarg_t a, char *b) {
//     kprintf("hello\n");
// 	return 0;
// }


static void timer_tick_cb(unsigned alarm) {
  absolute_time_t next;
  update_us_since_boot(&next, to_us_since_boot(now) + (1000000 / TICKSPERSEC));
  if (hardware_alarm_set_target(0, next)) {
    update_us_since_boot(&next, time_us_64() + (1000000 / TICKSPERSEC));
    hardware_alarm_set_target(0, next);
  }
  irqflags_t irq = di();
  udata.u_ininterrupt = 1;
  tty_interrupt();
  timer_interrupt();
  udata.u_ininterrupt = 0;
  irqrestore(irq);
}

void device_init(void) {
  /* Timer interrup must be initialized before blcok devices.
     set_boot_line uses pause syscall which will not be operational otherwise.
   */
  hardware_alarm_claim(0);
  update_us_since_boot(&now, time_us_64());
  hardware_alarm_set_callback(0, timer_tick_cb);
  hardware_alarm_force_irq(0);

  /* The flash device is too small to be useful, and a corrupt flash will
   * cause a crash on startup... oddly. */
#ifdef CONFIG_PICO_FLASH
  flash_dev_init();
#endif
  sd_rawinit();
  devsd_init();
}

/* vim: sw=4 ts=4 et: */
