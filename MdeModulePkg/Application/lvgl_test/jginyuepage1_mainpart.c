#include "jginyuepage1_mainpart.h"
lv_style_t part_style_focus;
lv_style_t part_style_normal;
lv_style_t text_style;
extern int is_exit;



void jy_exit_cb(lv_event_t *e);
void jy_exit_option_cb(lv_event_t *e);

void jy_argb_cb(lv_event_t *e);
void jy_argb_cb_async(void *e);

void jy_submenu_cb(lv_event_t *e);
void jy_submenu_cb_async(void *e);

void jy_mouse_factor_cb(lv_event_t *e);
void jy_mouse_factor_option_cb(lv_event_t *e);

jy_three_obj_t * jy_num_option_gen(jy_three_obj_t * jy_three_obj_t_1,UINT8 jy_num_option);

void esc_key_callback_mainpage(lv_event_t *e);
void esc_key_callback_mainpage_async(void  *e);
void mainpart_init(jy_page_t * page_element_1)
{
    jy_three_obj_t * obj_mainbar_plus_obj_others = (jy_three_obj_t*)lv_malloc(sizeof(jy_three_obj_t));
    obj_mainbar_plus_obj_others->obj1 = NULL;
    obj_mainbar_plus_obj_others->obj2 = NULL;
    obj_mainbar_plus_obj_others->obj3 = NULL;
    obj_mainbar_plus_obj_others->src = obj_mainbar_plus_obj_others;


    lv_obj_clean(page_element_1->main_bar);
    lv_obj_t * jy_option_layer = lv_obj_create(page_element_1->main_bar);
    obj_mainbar_plus_obj_others->obj1 = jy_option_layer;
    lv_obj_set_size(jy_option_layer,880,620);
    lv_obj_set_pos(jy_option_layer,10,10);
    lv_obj_set_style_pad_all(jy_option_layer,0,0);
    lv_obj_set_style_border_width(jy_option_layer,0,0);
    lv_obj_set_style_bg_color(jy_option_layer,lv_color_hex(0xFFFFFF),0);
    lv_obj_set_style_bg_opa(jy_option_layer,LV_OPA_10,0);

    lv_group_remove_all_objs(lv_group_get_default());
    lv_group_add_obj(lv_group_get_default(),jy_option_layer);
    lv_gridnav_add(jy_option_layer,LV_GRIDNAV_CTRL_ROLLOVER);
   
    option_style_init(&part_style_normal,0xFFFFFF);
    option_style_init(&part_style_focus,0xa5fffc);  
    lv_style_init(&text_style);
    lv_style_set_text_color(&text_style,lv_color_black());
    
    lv_obj_t * option1_argb = lv_obj_create(jy_option_layer);
    lv_obj_t * option2_mouse_factor = lv_obj_create(jy_option_layer);
    lv_obj_t * option3_submenu = lv_obj_create(jy_option_layer);
    lv_obj_t * option4_exit = lv_obj_create(jy_option_layer);
    
    lv_obj_set_pos(option1_argb ,0,0);
    lv_obj_set_pos(option2_mouse_factor,0,30);
    lv_obj_set_pos(option3_submenu,0,60);
    lv_obj_set_pos(option4_exit,0,90);
    lv_obj_set_size(option1_argb,880,25);
    lv_obj_set_size(option2_mouse_factor,880,25);
    lv_obj_set_size(option3_submenu,880,25);
    lv_obj_set_size(option4_exit,880,25);

    option_style_add(option1_argb);
    option_style_add(option2_mouse_factor);
    option_style_add(option3_submenu);
    option_style_add(option4_exit);

    
    option_text_add(option1_argb,"MCU ARGB Settings");
    option_text_add(option2_mouse_factor,"Mouse Factor Select");
    option_text_add(option3_submenu,"Submenu Example");
    option_text_add(option4_exit,"exit");
    lv_obj_add_state(option1_argb,LV_STATE_FOCUSED);
    

    lv_obj_add_event_cb(option4_exit,jy_exit_cb,LV_EVENT_CLICKED,obj_mainbar_plus_obj_others);
    lv_obj_add_event_cb(option2_mouse_factor,jy_mouse_factor_cb,LV_EVENT_CLICKED,obj_mainbar_plus_obj_others);
    lv_obj_add_event_cb(option3_submenu,jy_submenu_cb,LV_EVENT_CLICKED,page_element_1);
    lv_obj_add_event_cb(option1_argb,jy_argb_cb,LV_EVENT_CLICKED,page_element_1);
    
    lv_obj_add_event_cb(option1_argb,jy_focus_cb,LV_EVENT_FOCUSED,NULL);
    lv_obj_add_event_cb(option2_mouse_factor,jy_focus_cb,LV_EVENT_FOCUSED,NULL);
    lv_obj_add_event_cb(option3_submenu,jy_focus_cb,LV_EVENT_FOCUSED,NULL);
    lv_obj_add_event_cb(option4_exit,jy_focus_cb,LV_EVENT_FOCUSED,NULL);
    //lv_obj_add_event_cb(jy_option_layer,esc_key_callback_mainpage,LV_EVENT_KEY,option4_exit);
}

void option_style_init(lv_style_t * style ,UINT32 color)
{
    lv_style_init(style);
    //lv_style_set_size(style,880,25); 
    lv_style_set_bg_color(style,lv_color_hex(color));
    lv_style_set_bg_opa(style,LV_OPA_70);
    lv_style_set_border_width(style,1);
    lv_style_set_border_opa(style,LV_OPA_100);
    //lv_style_set_border_color(style,lv_color_black());
    lv_style_set_border_side(style,LV_BORDER_SIDE_NONE);
    lv_style_set_radius(style,0);
    lv_style_set_pad_all(style,0);

}

void option_style_add(lv_obj_t * option)
{
    lv_obj_remove_flag(option,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(option,&part_style_normal,0);
    lv_obj_add_style(option,&part_style_focus,LV_STATE_FOCUSED);
    lv_obj_add_style(option,&part_style_focus,LV_STATE_FOCUS_KEY);
    lv_obj_add_flag(option,LV_OBJ_FLAG_CLICK_FOCUSABLE);
}

void option_text_add(lv_obj_t * option,const CHAR8 *text)
{
    lv_obj_t * option_label = lv_label_create(option);
    lv_obj_align(option_label,LV_ALIGN_LEFT_MID,5,0);
    lv_label_set_text(option_label,text);
    lv_obj_add_style(option_label,&text_style,0);
}

void jy_exit_cb(lv_event_t *e)
{
    jy_three_obj_t * mainbar_plus_mask = e->user_data;
    mainbar_plus_mask->obj3 = e->current_target;
    lv_gridnav_remove(mainbar_plus_mask->obj1);
    //lv_group_remove_obj(mainbar_plus_mask->obj1);

    lv_obj_t * mask = lv_obj_create(lv_screen_active());
    lv_obj_set_size(mask,MY_DISP_HOR_RES,MY_DISP_VER_RES);
    lv_obj_set_pos(mask,0,0);
    lv_obj_set_style_bg_color(mask,lv_color_hex(0x7F7F7F),0);
    lv_obj_set_style_bg_opa(mask,LV_OPA_90,0);
    lv_obj_set_style_radius(mask,0,0);
    mainbar_plus_mask->obj2 = mask;

    lv_obj_t * msgbox = lv_obj_create(mask);
    lv_obj_set_align(msgbox,LV_ALIGN_CENTER);
    lv_obj_set_size(msgbox,360,260);
    lv_obj_set_style_bg_color(msgbox,lv_color_hex(0xFFFFFF),0);
    lv_obj_set_style_bg_opa(msgbox,LV_OPA_COVER,0);

    lv_obj_t * title_label = lv_label_create(msgbox);
    lv_obj_set_align(title_label,LV_ALIGN_TOP_MID);
    lv_label_set_text(title_label,"Exit");
    lv_obj_set_style_text_color(title_label,lv_color_hex(0x000000),0);

    lv_obj_t * text_label = lv_label_create(msgbox);
    lv_obj_set_pos(text_label,10,80);
    lv_obj_set_size(text_label,300,80);
    lv_label_set_text(text_label,"Do you really want to exit? Example:ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789");

    jy_three_obj_t * option_list_data = jy_num_option_gen(mainbar_plus_mask,2);

    lv_obj_t * button_1 = lv_button_create(msgbox);
    lv_obj_set_pos(button_1,50,180);
    lv_obj_set_size(button_1,60,30);
    lv_obj_t * btn_label_1 = lv_label_create(button_1);
    lv_label_set_text(btn_label_1,"Yes");
    lv_obj_add_event_cb(button_1,jy_exit_option_cb,LV_EVENT_CLICKED,&option_list_data[0]);

    lv_obj_t * button_2 = lv_button_create(msgbox);
    lv_obj_set_pos(button_2,210,180);
    lv_obj_set_size(button_2,60,30);
    lv_obj_t * btn_label_2 = lv_label_create(button_2);
    lv_label_set_text(btn_label_2,"No");
    lv_obj_add_event_cb(button_2,jy_exit_option_cb,LV_EVENT_CLICKED,&option_list_data[1]);

    lv_group_remove_all_objs(lv_group_get_default());
    lv_group_add_obj(lv_group_get_default(),msgbox);
    lv_gridnav_add(msgbox,LV_GRIDNAV_CTRL_ROLLOVER);
    lv_obj_add_state(button_1,LV_STATE_FOCUS_KEY);

    //lv_obj_remove_state(button_2,LV_STATE_FOCUS_KEY);
    //lv_group_focus_obj(button_1);
    lv_gridnav_set_focused(msgbox,button_1,LV_ANIM_OFF);
    lv_obj_add_event_cb(msgbox,esc_key_callback_exit_msg,LV_EVENT_KEY,&option_list_data[0]);
    //lv_group_remove_obj(button_1);
    //lv_group_remove_obj(button_2);
}

void jy_exit_option_cb(lv_event_t *e)
{
    jy_three_obj_t * mainbar_plus_mask = e->user_data;  
    lv_obj_t * mask = mainbar_plus_mask->obj2;
    lv_obj_del_async(mask);
    switch (mainbar_plus_mask->num_option)
    {
    case 0:
        is_exit = 1;
        break;
    default:
        break;
    }
    lv_gridnav_add(mainbar_plus_mask->obj1,LV_GRIDNAV_CTRL_ROLLOVER);
    lv_group_add_obj(lv_group_get_default(),mainbar_plus_mask->obj1);
    lv_gridnav_set_focused(mainbar_plus_mask->obj1,mainbar_plus_mask->obj3,LV_ANIM_OFF);
    lv_free(mainbar_plus_mask->src);   
}

void jy_argb_cb(lv_event_t *e)
{
    lv_async_call(jy_argb_cb_async,e->user_data);
}

void jy_argb_cb_async(void *e)
{
    argbmenu_init(e);
}

void jy_submenu_cb(lv_event_t *e)
{
    lv_async_call(jy_submenu_cb_async,e->user_data);
}

void jy_submenu_cb_async(void *e)
{
    submenu_init(e);
}

void jy_mouse_factor_cb(lv_event_t *e)
{
    const CHAR8 * option_string_map[] = {"Auto","1x","2x","5x","10x",""};
    const UINT8 num_option = 5;
    const UINT32 option_x = 400;
    const UINT32 option_y = 25;

    lv_obj_t ** obj_option = (lv_obj_t **)lv_malloc(sizeof(lv_obj_t *)*num_option);//leaking,waiting to be fix
    lv_memzero(obj_option,sizeof(lv_obj_t *)*num_option);

    jy_three_obj_t * mainbar_plus_mask = e->user_data;
    mainbar_plus_mask->obj3 = e->current_target;
    //lv_gridnav_remove(mainbar_plus_mask->obj1);
    //lv_group_remove_obj(mainbar_plus_mask->obj1);

    lv_obj_t * mask = lv_obj_create(lv_screen_active());
    lv_obj_set_size(mask,MY_DISP_HOR_RES,MY_DISP_VER_RES);
    lv_obj_set_pos(mask,0,0);
    lv_obj_set_style_bg_color(mask,lv_color_hex(0x3F3F3F),0);
    lv_obj_set_style_bg_opa(mask,LV_OPA_70,0);
    lv_obj_set_style_radius(mask,0,0);
    mainbar_plus_mask->obj2 = mask;

    lv_obj_t * msgbox = lv_obj_create(mask);
    lv_obj_set_align(msgbox,LV_ALIGN_CENTER);
    lv_obj_set_size(msgbox,option_x+2*(5+10),option_y*(num_option+3)+2*(5+10));
    lv_obj_set_style_bg_color(msgbox,lv_color_hex(0xFFFFFF),0);
    lv_obj_set_style_bg_opa(msgbox,LV_OPA_50,0);
    lv_obj_set_style_pad_all(msgbox,10,0);
    lv_obj_set_style_border_color(msgbox,lv_color_hex(0xFFFFFF),0);
    lv_obj_set_style_border_side(msgbox,LV_BORDER_SIDE_BOTTOM|LV_BORDER_SIDE_RIGHT,0);
    lv_obj_set_style_border_opa(msgbox,LV_OPA_100,0);
    lv_obj_set_style_border_width(msgbox,5,0);
    

    lv_obj_t * title_label = lv_label_create(msgbox);
    lv_obj_set_align(title_label,LV_ALIGN_TOP_MID);
    lv_label_set_text(title_label,"mouse factor");
    lv_obj_set_style_text_color(title_label,lv_color_hex(0x000000),0);
    lv_obj_t * option_label =NULL;
    jy_three_obj_t * option_list_data = jy_num_option_gen(mainbar_plus_mask,num_option);

    for (UINT8 i = 0; i < num_option; i++)
    {
        obj_option[i] = lv_obj_create(msgbox);
        lv_obj_set_size(obj_option[i],option_x,option_y);
        lv_obj_set_pos(obj_option[i],0,30+i*(option_y+5));
        option_style_add(obj_option[i]);

        title_label = lv_label_create(obj_option[i]);
        lv_obj_set_align(title_label,LV_ALIGN_LEFT_MID);
        lv_label_set_text(title_label,option_string_map[i]);
        lv_obj_set_style_text_color(title_label,lv_color_hex(0x000000),0);
        lv_obj_add_event_cb(obj_option[i],jy_mouse_factor_option_cb,LV_EVENT_CLICKED,&option_list_data[i]);       
    }

    lv_gridnav_remove(mainbar_plus_mask->obj1);
    lv_group_remove_all_objs(lv_group_get_default());
    lv_group_add_obj(lv_group_get_default(),msgbox);
    lv_gridnav_add(msgbox,LV_GRIDNAV_CTRL_ROLLOVER);
    lv_obj_add_event_cb(msgbox,esc_key_callback_exit_msg,LV_EVENT_KEY,&option_list_data[0]);
    lv_gridnav_set_focused(msgbox,lv_obj_get_child(msgbox,0),LV_ANIM_OFF);

}

void jy_mouse_factor_option_cb(lv_event_t *e)
{
    UINT32 factor_1 = 1;
    jy_three_obj_t * mainbar_plus_mask = e->user_data;  
    lv_obj_t * mask = mainbar_plus_mask->obj2;
    lv_obj_del_async(mask);
    switch (mainbar_plus_mask->num_option)
    {
    case 0:
        factor_1 = factor_1;
        break;
    case 1:
        factor_1 = 1;
        break;
    case 2:
        factor_1 = 2;
        break;
    case 3:
        factor_1 = 5;
        break;
    case 4:
        factor_1 = 10;
        break;
    default:
        factor_1 = factor_1;
        break;
    }
    jy_set_mouse_factor(factor_1);
    lv_gridnav_add(mainbar_plus_mask->obj1,LV_GRIDNAV_CTRL_ROLLOVER);
    lv_group_add_obj(lv_group_get_default(),mainbar_plus_mask->obj1);
    lv_gridnav_set_focused(mainbar_plus_mask->obj1,mainbar_plus_mask->obj3,LV_ANIM_OFF);    
    lv_free(mainbar_plus_mask->src);
}

jy_three_obj_t * jy_num_option_gen(jy_three_obj_t * jy_three_obj_t_1,UINT8 jy_num_option)
{
    jy_three_obj_t * jy_num_option_list = (jy_three_obj_t *)lv_malloc(sizeof(jy_three_obj_t)*jy_num_option);
    for (UINT8 i = 0; i < jy_num_option; i++)
    {
        jy_num_option_list[i].obj1 = jy_three_obj_t_1->obj1;
        jy_num_option_list[i].obj2 = jy_three_obj_t_1->obj2;
        jy_num_option_list[i].obj3 = jy_three_obj_t_1->obj3;
        jy_num_option_list[i].num_option = i;
        jy_num_option_list[i].src = jy_num_option_list;
    }
    return jy_num_option_list;
}

void esc_key_callback_mainpage(lv_event_t *e)
{
    uint32_t key = lv_indev_get_key(lv_indev_active());
    if(key == LV_KEY_ESC)
    {
        lv_gridnav_set_focused(e->current_target,e->user_data,LV_ANIM_OFF);
        lv_obj_set_state(e->user_data,LV_STATE_PRESSED,1);
        lv_async_call(esc_key_callback_mainpage_async,e->user_data);
    }
}
void esc_key_callback_mainpage_async(void *e)
{
    lv_obj_set_state(e,LV_STATE_PRESSED,0);
}

void esc_key_callback_exit_msg(lv_event_t *e)
{
    uint32_t key = lv_indev_get_key(lv_indev_active());
    if(key != LV_KEY_ESC)
    {
        return;     
    }
    jy_three_obj_t * mainbar_plus_mask = e->user_data;  
    lv_obj_t * mask = mainbar_plus_mask->obj2;
    lv_obj_del_async(mask);
    lv_gridnav_add(mainbar_plus_mask->obj1,LV_GRIDNAV_CTRL_ROLLOVER);
    lv_group_remove_all_objs(lv_group_get_default());
    lv_group_add_obj(lv_group_get_default(),mainbar_plus_mask->obj1);
    lv_gridnav_set_focused(mainbar_plus_mask->obj1,mainbar_plus_mask->obj3,LV_ANIM_OFF);
    lv_free(mainbar_plus_mask->src); 
}

void jy_focus_cb(lv_event_t *e)
{
    if(lv_obj_get_state(e->current_target)&LV_STATE_FOCUS_KEY)
    {
        return;
    }
    lv_gridnav_set_focused(lv_obj_get_parent(e->current_target),e->current_target,LV_ANIM_OFF);
}

