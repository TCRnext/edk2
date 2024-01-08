#include "lv_conf.h"
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "lvgl.h"
#include "dev/lv_uefi_driver.h"
#include "dev/display.h"
#include <Library/TimerLib.h>

#define EMULATOR

extern lv_color32_t * buf_1_1;
EFI_SYSTEM_TABLE  *SystemTable1 =NULL;

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINT64 Start = 0;
  UINT64 End = 0;
  UINT64 time0 = 1;

  SystemTable1=SystemTable;
  SystemTable->ConOut->OutputString(SystemTable->ConOut,L"Test SystemTable...\n\r");
  lv_init();
  lv_uefi_driver_init();

  //lv_obj_t * m = NULL;
  //m=lv_obj_create(m);
  //lv_obj_set_size(m,MY_DISP_HOR_RES,MY_DISP_VER_RES);
    
  //lv_disp_load_scr(m);
  //lv_obj_set_style_bg_color(lv_screen_active(),lv_color_hex3(0xFFFFFF),LV_PART_MAIN);
  //lv_obj_set_style_bg_opa(lv_screen_active(),LV_OPA_0,LV_PART_MAIN);

  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_radius(&style, 5);

    /*Make a gradient*/
  lv_style_set_width(&style, 400);
  lv_style_set_height(&style, LV_SIZE_CONTENT);

  lv_style_set_pad_ver(&style, 20);
  lv_style_set_pad_left(&style, 5);

  lv_style_set_x(&style, 0);
  lv_style_set_y(&style, 0);
  lv_style_set_align(&style,LV_ALIGN_CENTER);

    /*Create an object with the new style*/
  lv_obj_t * obj = lv_obj_create(lv_screen_active());
  lv_obj_add_style(obj, &style, 0);
  lv_obj_set_style_opa(obj,5,0);

  lv_obj_t * label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello,JGINYUE!----LVGL for UEFI lfsx");
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);


  while (1)
  {

#ifndef EMULATOR

    End = GetPerformanceCounter();
    time0 = GetTimeInNanoSecond(End -Start);
    if (time0%1000000UL>500000UL)
    {
      time0 = time0/1000000UL+1;
    }
    else
    {
      time0 = time0/1000000UL;
    }
    lv_tick_inc(time0);
    lv_task_handler();
    Start = End;
    SystemTable->BootServices->Stall(1*100);

#else 
  lv_tick_inc(1);
  lv_task_handler();
  SystemTable->BootServices->Stall(9*100);
#endif
  }
  return EFI_SUCCESS;
}
