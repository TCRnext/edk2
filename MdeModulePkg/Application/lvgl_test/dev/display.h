#pragma once
#include "../lvgl.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>


void lv_port_disp_init(void);

int lv_port_GOP_init(void);

static void disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);

void * jy_get_buf_addr(void);