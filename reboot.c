#include "reboot.h"
#include "hardware/resets.h"
#include "pico/bootrom.h"

void reboot_device(void) {
    reset_usb_boot(0, 0);
}
