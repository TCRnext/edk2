#pragma once
#include <Uefi.h>
#include "..\lvgl.h"
void keypad_init(void);
void keypad_read(lv_indev_t * indev, lv_indev_data_t * data);
uint32_t keycode_to_ascii(EFI_INPUT_KEY sdl_key);