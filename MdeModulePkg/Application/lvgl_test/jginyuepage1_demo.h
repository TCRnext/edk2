#pragma once
#include "jy_lvgl.h"

jy_page_t * test_page1();
lv_obj_t * init_background();
lv_obj_t * init_head_bar(lv_obj_t * page,lv_style_t * style);
lv_obj_t * init_left_bar(lv_obj_t * page,lv_style_t * style);
lv_obj_t * init_bottom_bar(lv_obj_t * page,lv_style_t * style);
lv_obj_t * init_right_bar1(lv_obj_t * page,lv_style_t * style);
lv_obj_t * init_right_bar2(lv_obj_t * page,lv_style_t * style);
lv_obj_t * init_main_bar(lv_obj_t * page,lv_style_t * style);
void set_pos_size_style(lv_obj_t * window,int32_t x,int32_t y,int32_t w,int32_t h,lv_style_t * style);
void set_pos_size(lv_obj_t * window,int32_t x,int32_t y,int32_t w,int32_t h);

