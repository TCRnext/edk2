#pragma once

#include "lvgl.h"
#include "lv_conf.h"

typedef struct jy_page_t
{
    lv_obj_t * background;
    lv_obj_t * head_bar;
    lv_obj_t * left_bar;
    lv_obj_t * bottom_bar;
    lv_obj_t * right_bar1;
    lv_obj_t * right_bar2;
    lv_obj_t * main_bar;
}jy_page_t;

#include <Uefi.h>
#include "dev/lv_uefi_driver.h"
#include "dev/mouse.h"
#include "jginyuepage1_mainpart.h"
#include "jginyuepage1_submenu.h"
#include "jginyuepage1_argb.h"
