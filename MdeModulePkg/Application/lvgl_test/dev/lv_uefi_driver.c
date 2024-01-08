#include "..\lvgl.h"
#include "lv_uefi_driver.h"
#include "display.h"
#include "keyboard.h"
#include "mouse.h"

void lv_uefi_driver_init(void)
{
    lv_port_disp_init();
    lv_group_t * group = lv_group_create();
    lv_group_set_default(group);
    mouse_init();
    keypad_init();
}