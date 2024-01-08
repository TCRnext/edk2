#include <Uefi.h>
#include "lvgl.h"
#include "lv_conf.h"
#include "jginyuepage1_demo.h"

jy_page_t * test_page1()
{
  jy_page_t * test_page1 = (jy_page_t*)lv_malloc(sizeof(jy_page_t));
  lv_obj_t * page = NULL;

  page = init_background();
  test_page1->background = page;

  static lv_style_t windows_style;
  lv_style_init(&windows_style);
  lv_style_set_bg_color(&windows_style,lv_color_make(255,255,255));
  lv_style_set_bg_opa(&windows_style,LV_OPA_50);
  lv_style_set_border_width(&windows_style,3);
  lv_style_set_border_opa(&windows_style,LV_OPA_100);
  lv_style_set_border_color(&windows_style,lv_color_make(255,255,255));
  lv_style_set_border_side(&windows_style,LV_BORDER_SIDE_BOTTOM|LV_BORDER_SIDE_RIGHT);
  lv_style_set_radius(&windows_style,10);

  test_page1->head_bar = init_head_bar(page,&windows_style);
  test_page1->left_bar = init_left_bar(page,&windows_style);
  test_page1->bottom_bar = init_bottom_bar(page,&windows_style);
  test_page1->right_bar1 = init_right_bar1(page,&windows_style);
  test_page1->right_bar2 = init_right_bar2(page,&windows_style);
  test_page1->main_bar = init_main_bar(page,&windows_style);
  
  return test_page1;
}

lv_obj_t * init_background()
{
  LV_IMG_DECLARE(bgtest1);
  lv_obj_t * background = lv_image_create(NULL);
  set_pos_size(background,0,0,1920,1080);
  lv_img_set_src(background,&bgtest1);
  lv_display_load_scr(background);
  return background;
}

lv_obj_t * init_head_bar(lv_obj_t * page,lv_style_t * style)
{
  lv_obj_t * head_bar = lv_obj_create(page);
  set_pos_size_style(head_bar,5,5,1910,195,style);//1910*195 (5,5)->(1915,200)
  return head_bar;
}

lv_obj_t * init_left_bar(lv_obj_t * page,lv_style_t * style)
{
  lv_obj_t * left_bar = lv_obj_create(page);
  set_pos_size_style(left_bar,5,240,455,700,style);//455*700 (5,240)->(460,940)
  return left_bar;
}

lv_obj_t * init_bottom_bar(lv_obj_t * page,lv_style_t * style)
{
  lv_obj_t * bottom_bar = lv_obj_create(page);
  set_pos_size_style(bottom_bar,5,960,1910,115,style);//1910*115 (5,960)->(1915,1075)
  return bottom_bar;
}

lv_obj_t * init_right_bar1(lv_obj_t * page,lv_style_t * style)
{
  lv_obj_t * right_bar1 = lv_obj_create(page);
  set_pos_size_style(right_bar1,1460,240,455,450,style);//455*450 (1460,240)->(1915,690)
  return right_bar1;
}

lv_obj_t * init_right_bar2(lv_obj_t * page,lv_style_t * style)
{
  lv_obj_t * right_bar2 = lv_obj_create(page);
  set_pos_size_style(right_bar2,1460,700,455,240,style);//455*240 (1460,700)->(1915,940)
  return right_bar2;
}

lv_obj_t * init_main_bar(lv_obj_t * page,lv_style_t * style)
{
  lv_obj_t * main_bar = lv_obj_create(page);
  set_pos_size_style(main_bar,480,240,960,700,style);//960*700 (480,240)->(1440,940)
  return main_bar;
}

void set_pos_size_style(lv_obj_t * window,int32_t x,int32_t y,int32_t w,int32_t h,lv_style_t * style)
{
  lv_obj_add_style(window,style,0);
  lv_obj_set_size(window,w,h);
  lv_obj_set_pos(window,x,y);
  lv_obj_clear_flag(window,LV_OBJ_FLAG_CLICKABLE);
}

void set_pos_size(lv_obj_t * window,int32_t x,int32_t y,int32_t w,int32_t h)
{
  lv_obj_set_size(window,w,h);
  lv_obj_set_pos(window,x,y);
  lv_obj_clear_flag(window,LV_OBJ_FLAG_CLICKABLE);
}
