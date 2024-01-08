#include "jginyuepage1_submenu.h"
#include <Library\UefiLib.h>
#include <Library\MemoryAllocationLib.h>
lv_style_t part_style_focus;
lv_style_t part_style_normal;
lv_style_t text_style;
extern EFI_SYSTEM_TABLE  *SystemTable1;


struct obj_displaymode_t
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL            *Gop;
    UINT32                                  index;
    jy_page_t                               *page_element;
};
typedef struct obj_displaymode_t obj_displaymode_t;

CHAR8 *gPixelFormat[] = {
"PixelRedGreenBlueReserved8BitPerColor",
"PixelBlueGreenRedReserved8BitPerColor",
"PixelBitMask",
"PixelBltOnly",
"PixelFormatMax",
};

static lv_obj_t ** obj_option = NULL;
static obj_displaymode_t * obj_displaymode_list = NULL;

void obj_displaymode_make(obj_displaymode_t * obj_displaymode_list_1,EFI_GRAPHICS_OUTPUT_PROTOCOL * Gop,UINT32 index,jy_page_t * page_element);
void jy_mouse_factor_option_cb(lv_event_t *e);
void jy_focus_cb_1(lv_event_t *e);
void jy_focus_cb_2(lv_event_t *e);

void jy_exit_submenu_async(void * data);


void submenu_init(jy_page_t * page_element_1)
{
    EFI_STATUS                              Status;  
    EFI_GRAPHICS_OUTPUT_PROTOCOL            *Gop =NULL;
    UINT32                                  Index = 0;
    UINT32                                  max_mode = 0;
    UINT32                                  current_mode = 0;
                          

    lv_obj_clean(page_element_1->main_bar);

    lv_obj_t * jy_option_layer = lv_obj_create(page_element_1->main_bar);
    lv_obj_set_size(jy_option_layer,880,620);
    lv_obj_set_pos(jy_option_layer,10,10);
    lv_obj_set_style_pad_all(jy_option_layer,0,0);
    lv_obj_set_style_border_width(jy_option_layer,0,0);
    lv_obj_set_style_bg_color(jy_option_layer,lv_color_hex(0xFFFFFF),0);
    lv_obj_set_style_bg_opa(jy_option_layer,LV_OPA_10,0);

    lv_group_add_obj(lv_group_get_default(),jy_option_layer);
    lv_gridnav_add(jy_option_layer,LV_GRIDNAV_CTRL_ROLLOVER);
   
    option_style_init(&part_style_normal,0xFFFFFF);
    option_style_init(&part_style_focus,0xa5fffc);  
    lv_style_init(&text_style);
    lv_style_set_text_color(&text_style,lv_color_black());
  
    Status=SystemTable1->BootServices->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,NULL,(VOID **)&Gop);
    max_mode = Gop->Mode->MaxMode;
    current_mode = Gop->Mode->Mode;

    lv_obj_set_flex_flow(jy_option_layer,LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_column(jy_option_layer,15,0);
    lv_obj_set_flex_align(jy_option_layer,LV_FLEX_ALIGN_START,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_START);

    obj_option = (lv_obj_t **)lv_malloc(sizeof(lv_obj_t *)*max_mode);
    lv_memzero(obj_option,sizeof(lv_obj_t *)*max_mode);

    obj_displaymode_list = (obj_displaymode_t * )lv_malloc(sizeof(obj_displaymode_t)*max_mode);
    obj_displaymode_make(obj_displaymode_list,Gop,max_mode,page_element_1);

    lv_obj_t * option_label = NULL;
    for (UINT32 i = 0; i < max_mode; i++)
    {
        obj_option[i] = lv_obj_create(jy_option_layer);
        lv_obj_set_size(obj_option[i],880,25);
        option_style_add(obj_option[i]);
        option_label = lv_label_create(obj_option[i]);
        lv_obj_align(option_label,LV_ALIGN_LEFT_MID,5,0);
        lv_label_set_text_fmt(option_label,"Mode#%d",i);
        lv_obj_add_style(option_label,&text_style,0);
        lv_obj_add_event_cb(obj_option[i],jy_focus_cb_1,LV_EVENT_FOCUSED,NULL);
        lv_obj_add_event_cb(obj_option[i],jy_focus_cb_2,LV_EVENT_STYLE_CHANGED,&obj_displaymode_list[i]);
        if(i == current_mode) lv_label_set_text(option_label,"Current_mode");
    }
    lv_obj_add_event_cb(jy_option_layer,esc_key_callback_exit_submenu,LV_EVENT_KEY,page_element_1);

    if(lv_obj_get_child_cnt(jy_option_layer)>0)
    {
        lv_gridnav_set_focused(jy_option_layer,obj_option[0],LV_ANIM_OFF);
    }

}

void obj_displaymode_make(obj_displaymode_t * obj_displaymode_list_1,EFI_GRAPHICS_OUTPUT_PROTOCOL * Gop,UINT32 index,jy_page_t * page_element)
{
    for (UINT32 i = 0; i < index; i++)
    {
        obj_displaymode_list_1[i].Gop = Gop;
        obj_displaymode_list_1[i].index = i;
        obj_displaymode_list_1[i].page_element = page_element;
    }
}

void jy_focus_cb_1(lv_event_t *e)
{
    if(lv_obj_get_state(e->current_target)&LV_STATE_FOCUS_KEY)
    {
        return;
    }
    lv_gridnav_set_focused(lv_obj_get_parent(e->current_target),e->current_target,LV_ANIM_OFF);
}

void jy_focus_cb_2(lv_event_t *e)
{
    if(!lv_obj_get_state(e->current_target)&LV_STATE_FOCUS_KEY)
    {
        return;
    }
    UINTN                                   SizeOfInfo = 0;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION    *ModeInfo = NULL;
    EFI_STATUS                              Status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL            *Gop = NULL;  

    obj_displaymode_t * obj_displaymode_list_1 = e->user_data;
    Gop = obj_displaymode_list_1->Gop;

    lv_obj_t * right_bar_1 = ((jy_page_t *)(obj_displaymode_list_1->page_element))->right_bar1;//455*450
    lv_obj_clean(right_bar_1);
    lv_obj_t * help_label = lv_label_create(right_bar_1);
    lv_obj_set_align(help_label,LV_ALIGN_CENTER);
    lv_obj_set_size(help_label,405,400);
    lv_obj_set_style_text_color(help_label,lv_color_hex(0x000000),0);

    Gop->QueryMode(Gop,obj_displaymode_list_1->index,&SizeOfInfo,&ModeInfo);

    lv_label_set_text_fmt(help_label,
    "Mode: %d\n\nVersion: 0x%04x\n\nHorizontalResolution:%d\n\nVerticalResolution:%d\n\n.PixelFormat:\n%s\n\n",
    obj_displaymode_list_1->index,
    ModeInfo->Version,
    ModeInfo->HorizontalResolution,
    ModeInfo->VerticalResolution,
    gPixelFormat[ModeInfo->PixelFormat]
    );
    FreePool(ModeInfo);
}

void esc_key_callback_exit_submenu(lv_event_t *e)
{
    uint32_t key = lv_indev_get_key(lv_indev_active());
    if(key != LV_KEY_ESC)
    {
        return;     
    }
    lv_async_call(jy_exit_submenu_async,e->user_data); 
}

void jy_exit_submenu_async(void * data)
{
    lv_obj_clean(((jy_page_t *)data)->right_bar1);
    mainpart_init(data);
    lv_free(obj_option);
    lv_free(obj_displaymode_list);
}