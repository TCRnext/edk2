#pragma once
#include "jy_lvgl.h"
struct jy_three_obj_t
{
    UINT8 num_option;
    lv_obj_t * obj1;
    lv_obj_t * obj2;
    lv_obj_t * obj3;
    struct jy_three_obj_t * src;
};
typedef struct jy_three_obj_t jy_three_obj_t;

void mainpart_init(jy_page_t * page_element_1);
void option_style_init(lv_style_t * style ,UINT32 color);
void option_text_add(lv_obj_t * option,const CHAR8 *text);
void option_style_add(lv_obj_t * option);
void esc_key_callback_exit_msg(lv_event_t *e);

void jy_focus_cb(lv_event_t *e);