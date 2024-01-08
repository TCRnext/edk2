#pragma once
#include <Uefi.h>
#include "..\lvgl.h"

void mouse_init();

void uefi_mouse_read_cb(lv_indev_t * indev, lv_indev_data_t*data);

void jy_set_mouse_factor(UINT32 factor);