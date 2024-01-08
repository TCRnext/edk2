#include "jginyuepage1_argb.h"
#include <Library/UefiUsbLib.h>
#include <Library/UefiLib.h>


struct jy_argb_zones_cfg_t
{
    UINT8 zones;            //0x01 zone1,0x02 zone2,0xFF all zones,0x03 = zone1+zone2
    UINT8 zones_reserve;
    UINT8 LED_numbers;
    UINT8 RG_swap;
    UINT8 modes;
    UINT8 speed;
    UINT8 brightness;
    UINT8 direction;
    UINT8 color_r;
    UINT8 color_g;
    UINT8 color_b;
};
typedef struct jy_argb_zones_cfg_t jy_argb_zones_cfg_t;

struct jy_argb_cfg_t
{
    jy_argb_zones_cfg_t cfg[2]; //use malloc in later version
    UINT8 is_init ;
    UINT8 zones_number;
    UINT8 major_version;
    UINT8 minor_version;
    UINT8 is_connect;
    UINT8 zone_choice;
    UINTN errorcode;
    UINTN errorcode1;
    UINT8 interface_number;
};
typedef struct jy_argb_cfg_t jy_argb_cfg_t;

struct jy_sub_option_list_t
{
    UINTN number_option;
    UINTN* key_list;
    CHAR8** sub_option_string_list;
    void * data_ptr;
    UINT16 data_type;
    lv_obj_t * obj_option;
};
typedef struct jy_sub_option_list_t jy_sub_option_list_t;

void jy_argb_init(void);
void jy_get_argb_FW_version(void);
UINT8 jy_get_MCU_USBIO(void);

void esc_key_callback_exit_argbmenu(lv_event_t *e);
void jy_exit_argbmenu_async(void *data);

void esc_key_callback_exit_msg_1(lv_event_t *e);
void esc_key_callback_exit_msg_1_async(void *e);

void jy_focus_argbmenu_cb_2(lv_event_t *e);

void init_option(lv_obj_t * option,void * data_ptr,UINT16 data_type,jy_sub_option_list_t * list);
void init_option_all(lv_obj_t * option_layer);

void jy_option_list_cb(lv_event_t *e);
void jy_option_list_cb_suboption_cb(lv_event_t *e);
const CHAR8 * option_string_map_argb[] = 
{
    "Connect Status:",
    "Zones:",
    "Modes:",
    "Red:",
    "Green:",
    "Blue:",
    "Color_select:",
    "Speed:",
    "Brightness:",
    "Direction:",
    "Apply to one",
    "Apply to all",
    ""
};

const CHAR8 * help_string_map_argb[] = 
{
    "Show Connect Status",
    "Choose ARGB zones:\nSupport Zone1 or Zone2",
    "Modes: \nChoose the hardware ARGB effects mode",
    "Red: 0-255",
    "Green: 0-255",
    "Blue: 0-255",
    "Color_select: Click to choose a color by a colormix board",
    "Speed: Change the speed of hardware ARGB effects(useless in some mode)",
    "Brightness: Change the Brightness of hardware ARGB effects(useless in some mode)",
    "Direction: Change the Direction of hardware ARGB effects(useless in some mode)",
    "Apply to one :apply the setting to one zone",
    "Apply to all :apply the setting to all zone",
    ""
};

const UINTN zones_list[] = {0x01,0x02,-1};
const CHAR8 * zones_string_list[] ={"Zone1","Zone2",""};
jy_sub_option_list_t zones =
{
    .number_option = 2,
    .key_list = zones_list,
    .sub_option_string_list = zones_string_list,
};

const UINTN modes_list[] = 
{
    JGINYUE_USB_MODE_OFF,
    JGINYUE_USB_MODE_STATIC,
    JGINYUE_USB_MODE_BREATHING,
    JGINYUE_USB_MODE_STROBE,
    JGINYUE_USB_MODE_CYCLING,
    JGINYUE_USB_MODE_RANDOM,
    JGINYUE_USB_MODE_WAVE,
    JGINYUE_USB_MODE_WATER,
    JGINYUE_USB_MODE_DIRECT,
    -1
};
const CHAR8 * modes_string_list[] =
{
    "OFF",
    "STATIC",
    "BREATHING",
    "STROBE",
    "CYCLING",
    "RANDOM",
    "WAVE",
    "WATER",
    "DIRECT",
    ""
};
jy_sub_option_list_t modes =
{
    .number_option = 9,
    .key_list = modes_list,
    .sub_option_string_list = modes_string_list,
};

const UINTN direction_list[] = {JGINYUE_DIRECTION_LEFT,JGINYUE_DIRECTION_RIGHT,-1};
const CHAR8 * direction_string_list[] ={"Left","Right",""};
jy_sub_option_list_t direction =
{
    .number_option = 2,
    .key_list = direction_list,
    .sub_option_string_list = direction_string_list,
};

lv_style_t part_style_focus;
lv_style_t part_style_normal;
lv_style_t text_style;
extern EFI_SYSTEM_TABLE             *SystemTable1;
static EFI_USB_IO_PROTOCOL          *MCU_USB_IO = NULL;
EFI_HANDLE                          *PointerHandleBuffer1 = NULL;
static jy_argb_cfg_t jy_argb_cfg =
{
    .is_init = 0,
    .zones_number = 2,
    .cfg = {0},
    .major_version = 0,
    .minor_version = 0,
    .is_connect = 0,
    .zone_choice = 0x01,
    .errorcode = 0,

};


void argbmenu_init(jy_page_t * page_element_1)
{
    jy_three_obj_t * obj_mainbar_plus_obj_others = (jy_three_obj_t*)lv_malloc(sizeof(jy_three_obj_t));
    obj_mainbar_plus_obj_others->obj1 = NULL;
    obj_mainbar_plus_obj_others->obj2 = NULL;
    obj_mainbar_plus_obj_others->obj3 = NULL;
    obj_mainbar_plus_obj_others->src = obj_mainbar_plus_obj_others;

    if(jy_argb_cfg.is_init == 0)
    {
        jy_argb_init();
    }
                          
    lv_obj_clean(page_element_1->main_bar);

    
    lv_obj_t * jy_option_layer = lv_obj_create(page_element_1->main_bar);
    obj_mainbar_plus_obj_others->obj1 = jy_option_layer;
    lv_obj_set_size(jy_option_layer,880,620);
    lv_obj_set_pos(jy_option_layer,10,10);
    lv_obj_set_style_pad_all(jy_option_layer,0,0);
    lv_obj_set_style_border_width(jy_option_layer,0,0);
    lv_obj_set_style_bg_color(jy_option_layer,lv_color_hex(0xFFFFFF),0);
    lv_obj_set_style_bg_opa(jy_option_layer,LV_OPA_10,0);
    

    lv_group_add_obj(lv_group_get_default(),jy_option_layer);
    lv_gridnav_add(jy_option_layer,LV_GRIDNAV_CTRL_ROLLOVER);
    lv_gridnav_add(jy_option_layer,LV_GRIDNAV_CTRL_ROLLOVER);
   
    
    option_style_init(&part_style_normal,0xFFFFFF);
    option_style_init(&part_style_focus,0xa5fffc);  
    lv_style_init(&text_style);
    lv_style_set_text_color(&text_style,lv_color_black());
    

    
    lv_obj_set_flex_flow(jy_option_layer,LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_column(jy_option_layer,5,0);
    lv_obj_set_style_pad_row(jy_option_layer,5,0);
    lv_obj_set_flex_align(jy_option_layer,LV_FLEX_ALIGN_START,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_START);
    

    
    lv_obj_t * option1_MCU_connect_status = lv_obj_create(jy_option_layer);
    lv_obj_t * option2_zones_select = lv_obj_create(jy_option_layer);
    lv_obj_t * option3_mode_select = lv_obj_create(jy_option_layer);
    lv_obj_t * option4_red_option = lv_obj_create(jy_option_layer);
    lv_obj_t * option5_green_option = lv_obj_create(jy_option_layer);
    lv_obj_t * option6_blue_option = lv_obj_create(jy_option_layer);
    lv_obj_t * option7_color_select = lv_obj_create(jy_option_layer);
    lv_obj_t * option8_speed = lv_obj_create(jy_option_layer);
    lv_obj_t * option9_brightness = lv_obj_create(jy_option_layer);
    lv_obj_t * option10_direction = lv_obj_create(jy_option_layer);
    lv_obj_t * option11_apply_to_one = lv_obj_create(jy_option_layer);
    lv_obj_t * option12_apply_to_all = lv_obj_create(jy_option_layer);
    

    lv_obj_t * option_temp = NULL;
    lv_obj_t * option_label = NULL;
    for (UINT8 i = 0; i < lv_obj_get_child_cnt(jy_option_layer); i++)
    {
        option_temp = lv_obj_get_child(jy_option_layer,i);
        if((i>=3)&&(i<=5))
        {
            lv_obj_set_size(option_temp,290,25);
        }
        else if(i == 6)
        {
            lv_obj_set_size(option_temp,880,50);   
        }
        else
        {
            lv_obj_set_size(option_temp,880,25);
        }
        option_style_add(option_temp);
        option_label = lv_label_create(option_temp);
        lv_obj_align(option_label,LV_ALIGN_LEFT_MID,5,0);
        lv_label_set_text(option_label,option_string_map_argb[i]);
        lv_obj_add_style(option_label,&text_style,0);
        //lv_obj_add_event_cb(option_temp,jy_focus_cb,LV_EVENT_FOCUSED,NULL);
        //lv_obj_add_event_cb(option_temp,jy_focus_argbmenu_cb_2,LV_EVENT_STYLE_CHANGED,page_element_1);
    }

    if(jy_argb_cfg.is_connect == 1)
    {
        option_label = lv_label_create(option1_MCU_connect_status);
        lv_obj_align(option_label,LV_ALIGN_CENTER,5,0);
        lv_label_set_text(option_label,"Connected");
        lv_obj_add_style(option_label,&text_style,0);
        lv_obj_set_style_text_color(option_label,lv_color_make(0,255,0),0);
        option_label = lv_label_create(option1_MCU_connect_status);
        lv_obj_align(option_label,LV_ALIGN_RIGHT_MID,-5,0);
        lv_label_set_text_fmt(option_label,"Ver%x.%xError:%d.%d",jy_argb_cfg.major_version,jy_argb_cfg.minor_version,jy_argb_cfg.errorcode,jy_argb_cfg.errorcode1);
        lv_obj_add_style(option_label,&text_style,0);
        lv_obj_set_style_text_color(option_label,lv_color_make(0,255,0),0);        
    }
    else
    {
        option_label = lv_label_create(option1_MCU_connect_status);
        lv_obj_align(option_label,LV_ALIGN_CENTER,5,0);
        lv_label_set_text(option_label,"Disconnected");
        lv_obj_add_style(option_label,&text_style,0);
        lv_obj_set_style_text_color(option_label,lv_color_make(255,0,0),0);
        option_label = lv_label_create(option1_MCU_connect_status);
        lv_obj_align(option_label,LV_ALIGN_RIGHT_MID,-5,0);
        lv_label_set_text_fmt(option_label,"Ver%x.%xError:%d.%d",jy_argb_cfg.major_version,jy_argb_cfg.minor_version,jy_argb_cfg.errorcode,jy_argb_cfg.errorcode1);
        lv_obj_add_style(option_label,&text_style,0);
        lv_obj_set_style_text_color(option_label,lv_color_make(255,0,0),0); 
    }

    init_option_all(jy_option_layer);
    lv_obj_add_event_cb(jy_option_layer,esc_key_callback_exit_argbmenu,LV_EVENT_KEY,page_element_1);
    lv_gridnav_set_focused(jy_option_layer,option1_MCU_connect_status,LV_ANIM_OFF);
    lv_obj_add_event_cb(option2_zones_select,jy_option_list_cb,LV_EVENT_CLICKED,&zones);
    lv_obj_add_event_cb(option3_mode_select,jy_option_list_cb,LV_EVENT_CLICKED,&modes);
    lv_obj_add_event_cb(option10_direction,jy_option_list_cb,LV_EVENT_CLICKED,&direction);
    
}

void jy_argb_init(void)
{
    jy_argb_cfg.is_init = 1;
    if(!jy_get_MCU_USBIO())
    {
        jy_get_argb_FW_version();
    }
    jy_argb_cfg.is_connect = 0;
    for (UINT8 i = 0; i < jy_argb_cfg.zones_number; i++)
    {
        lv_memset(&(jy_argb_cfg.cfg[i]),0x00,sizeof(jy_argb_zones_cfg_t));
        jy_argb_cfg.cfg[i].zones = i+1;
        jy_argb_cfg.cfg[i].color_r = 0x7F;
        jy_argb_cfg.cfg[i].color_g = 0x7F;
        jy_argb_cfg.cfg[i].color_b = 0x7F;
        jy_argb_cfg.cfg[i].brightness = JGINYUE_USB_BRIGHTNESS_DEFAULT;
        jy_argb_cfg.cfg[i].modes = JGINYUE_USB_MODE_STATIC;
        jy_argb_cfg.cfg[i].LED_numbers = 100;
        jy_argb_cfg.cfg[i].speed = JGINYUE_USB_SPEED_DEFAULT;
        jy_argb_cfg.cfg[i].RG_swap = JGINYUE_RG_DEFAULT;
        jy_argb_cfg.cfg[i].direction = JGINYUE_DIRECTION_LEFT;
    }
    
}

void jy_get_argb_FW_version(void)
{
    EFI_STATUS                          Status;
    UINT32                              status1=0;
    UINT8                               usb_buf[20];
    UINTN                               length = 16;
    lv_memset(usb_buf,0x00,16);

    usb_buf[0x00]  = JGINYUE_USB_GENERAL_COMMAND_HEADER;
    usb_buf[0x01]  = JGINYUE_USB_GET_FW_VERSION;

    Status = MCU_USB_IO->UsbSyncInterruptTransfer(MCU_USB_IO,0x02,usb_buf,&length,3000,&status1);
    jy_argb_cfg.errorcode1 = status1;
    Status = MCU_USB_IO->UsbSyncInterruptTransfer(MCU_USB_IO,0x81,usb_buf,&length,3000,&status1);
    jy_argb_cfg.errorcode = status1;
    jy_argb_cfg.major_version = usb_buf[0x02];
    jy_argb_cfg.minor_version = usb_buf[0x03];
    if(!EFI_ERROR(Status))
    {
        jy_argb_cfg.is_connect = 1;//return;
    } 
    else
    {
        jy_argb_cfg.is_connect = 1;
    }
    return;    
    /*
    Status = UsbSetReportRequest(MCU_USB_IO,
    jy_argb_cfg.interface_number,
    JGINYUE_USB_GENERAL_COMMAND_HEADER,
    HID_OUTPUT_REPORT,
    8,
    usb_buf);

    if(EFI_ERROR(Status))
    {
        //return;
    }
    jy_argb_cfg.errorcode = Status;
    SystemTable1->BootServices->Stall(50*100);
    lv_memset(usb_buf,0x00,16);
    Status = UsbGetReportRequest(MCU_USB_IO,
    jy_argb_cfg.interface_number,
    JGINYUE_USB_GENERAL_COMMAND_HEADER,
    HID_INPUT_REPORT,
    8,
    usb_buf);

    //jy_argb_cfg.errorcode = Status;
    if(!EFI_ERROR(Status))
    {
        jy_argb_cfg.is_connect = 1;//return;
    } 
    return;
    */
}

UINT8 jy_get_MCU_USBIO(void)
{
    EFI_STATUS                          Status;
	UINTN                               Index = 0;
    UINTN                               HandleCount = 0;
    EFI_USB_DEVICE_DESCRIPTOR           device_descriptor;
    EFI_USB_INTERFACE_DESCRIPTOR        interface_descriptor;
    UINT8                               ret = 1;


    lv_memzero(&device_descriptor,sizeof(EFI_USB_DEVICE_DESCRIPTOR));       

    Status = SystemTable1->BootServices->LocateHandleBuffer(
    ByProtocol,
    &gEfiUsbIoProtocolGuid,
    NULL,
    &HandleCount,
    &PointerHandleBuffer1
    );

    if(EFI_ERROR (Status))
    {
    lv_obj_t * label1 = lv_label_create(lv_screen_active());
    lv_label_set_text_fmt(label1, "Error%d",Status);
    return 2;
    }

    for ( Index  = 0; Index <  HandleCount; Index++)
    {
        Status = SystemTable1->BootServices->HandleProtocol(
        PointerHandleBuffer1[Index],
        &gEfiUsbIoProtocolGuid,
        (VOID **)&MCU_USB_IO
        );
        MCU_USB_IO->UsbGetDeviceDescriptor(MCU_USB_IO,&device_descriptor);
        MCU_USB_IO->UsbGetInterfaceDescriptor(MCU_USB_IO,&interface_descriptor);
        if((device_descriptor.IdVendor == 0x0416)&&(device_descriptor.IdProduct == 0xA125)&&(interface_descriptor.InterfaceClass == 0x03))
        {
            jy_argb_cfg.interface_number = interface_descriptor.InterfaceNumber;
            ret = 0;
            break;
        }
        else
        {
            lv_memzero(&device_descriptor,sizeof(EFI_USB_DEVICE_DESCRIPTOR));
        }
    }
    return ret;
}

void esc_key_callback_exit_argbmenu(lv_event_t *e)
{
    uint32_t key = lv_indev_get_key(lv_indev_active());
    if(key != LV_KEY_ESC)
    {
        return;     
    }
    lv_async_call(jy_exit_argbmenu_async,e->user_data); 
}

void jy_exit_argbmenu_async(void *data)
{
    lv_obj_clean(((jy_page_t *)data)->right_bar1);
    mainpart_init(data);
}

void esc_key_callback_exit_msg_1(lv_event_t *e)
{
    jy_sub_option_list_t * sublist = e->user_data;
    uint32_t key = lv_indev_get_key(lv_indev_active());
    if(key != LV_KEY_ESC)
    {
        return;     
    }
    lv_obj_t * mask = lv_obj_get_parent(e->current_target);
    
    lv_gridnav_add(lv_obj_get_parent(sublist->obj_option),LV_GRIDNAV_CTRL_ROLLOVER);
    lv_group_remove_all_objs(lv_group_get_default());
    lv_group_add_obj(lv_group_get_default(),lv_obj_get_parent(sublist->obj_option));
    lv_gridnav_set_focused(lv_obj_get_parent(sublist->obj_option),sublist->obj_option,LV_ANIM_OFF);
    lv_async_call(esc_key_callback_exit_msg_1_async,e->current_target);

}
void esc_key_callback_exit_msg_1_async(void *e)
{
    lv_obj_t * mask = lv_obj_get_parent((lv_obj_t *)e);
    lv_obj_remove_event_cb((lv_obj_t *)e,esc_key_callback_exit_msg_1);
    lv_obj_del_async(mask);
}

void jy_focus_argbmenu_cb_2(lv_event_t *e)
{
    if(!lv_obj_get_state(e->current_target)&LV_STATE_FOCUS_KEY)
    {
        return;
    }
    UINT32 index = lv_obj_get_index(e->current_target);
    jy_page_t * page_element_2 = e->user_data;
    lv_obj_t * right_bar_1 = page_element_2->right_bar1;
    lv_obj_clean(right_bar_1);
    lv_obj_t * help_label = lv_label_create(right_bar_1);
    lv_obj_set_align(help_label,LV_ALIGN_CENTER);
    lv_obj_set_size(help_label,405,400);
    lv_obj_set_style_text_color(help_label,lv_color_hex(0x000000),0);
    lv_label_set_text(help_label,help_string_map_argb[index]);
}

void init_option(lv_obj_t * option,void * data_ptr,UINT16 data_type,jy_sub_option_list_t * list)
{
    lv_obj_t * option_label_2 = NULL;
    INTN data=0;
    if(data_type == DATATYPE_NONE) return;

    if(lv_obj_get_child_cnt(option)==1)
    {
        option_label_2 = lv_label_create(option);
        lv_obj_align(option_label_2,LV_ALIGN_RIGHT_MID,-5,0);
        lv_obj_add_style(option_label_2,&text_style,0);
        lv_obj_set_style_text_color(option_label_2,lv_color_make(0,0,0),0); 
    }
    else  option_label_2 = lv_obj_get_child(option,1);

    switch (data_type&0x00FF)
    {
    case DATATYPE_UINT8:
        data = *((UINT8 *)data_ptr);
        break;
    case DATATYPE_UINT16:
        data = *((UINT16 *)data_ptr);
        break;
    case DATATYPE_UINT32:
        data = *((UINT32 *)data_ptr);
        break;
    case DATATYPE_UINT64:
        data = *((UINT64 *)data_ptr);
        break;
    case DATATYPE_INT8:
        data = *((INT8 *)data_ptr);
        break;
    case DATATYPE_INT16:
        data = *((INT16 *)data_ptr);
        break;
    case DATATYPE_INT32:
        data = *((INT32 *)data_ptr);
        break;
    case DATATYPE_INT64:
        data = *((INT64 *)data_ptr);
        break;    
    default:
        break;
    }

    if (data_type&DATATYPE_BOOL_OPTION)
    {
        if (!data)
        {
            lv_label_set_text(option_label_2,"Disable");
            return;
        }
        else
        {
            lv_label_set_text(option_label_2,"Enabled");
            return;
        }
        
    }
    else if (data_type&DATATYPE_OPTION)
    {
        if(list == NULL) lv_label_set_text(option_label_2,"Error : no list!");
        for (UINTN i = 0; i < list->number_option; i++)
        {
            if (list->key_list[i] == data)
            {
                lv_label_set_text_fmt(option_label_2,"%s",list->sub_option_string_list[i]);
                list->data_ptr = data_ptr;
                list->data_type = data_type;
                list->obj_option = option;
                return;
            }
        }
        lv_label_set_text_fmt(option_label_2,"Error : list have no these value!0x%x",data);
    }
    else
    {
        lv_label_set_text_fmt(option_label_2,"%d",data);
    }
}

void init_option_all(lv_obj_t * option_layer)
{
    lv_obj_t * option_temp = NULL;
    UINT8 zone_index;
    switch (jy_argb_cfg.zone_choice)
    {
    case 0x01:
        zone_index=0;/* code */
        break;
    case 0x02:
        zone_index=1;
        break;
    default:
        break;
    }
    for (UINT8 i = 0; i < lv_obj_get_child_cnt(option_layer); i++)
    {
        option_temp = lv_obj_get_child(option_layer,i);

        switch (i)
        {
        case 1:
            init_option(option_temp,&(jy_argb_cfg.zone_choice),DATATYPE_UINT8|DATATYPE_OPTION,&zones);
            break;
        case 2:
            init_option(option_temp,&(jy_argb_cfg.cfg[zone_index].modes),DATATYPE_UINT8|DATATYPE_OPTION,&modes);
            break;
            
        case 3:
            init_option(option_temp,&(jy_argb_cfg.cfg[zone_index].color_r),DATATYPE_UINT8,NULL);
            break;
        case 4:
            init_option(option_temp,&(jy_argb_cfg.cfg[zone_index].color_g),DATATYPE_UINT8,NULL);
            break;
        case 5:
            init_option(option_temp,&(jy_argb_cfg.cfg[zone_index].color_b),DATATYPE_UINT8,NULL);  
            break;
        case 7:
            init_option(option_temp,&(jy_argb_cfg.cfg[zone_index].speed),DATATYPE_UINT8,NULL);  
            break;
        case 8:
            init_option(option_temp,&(jy_argb_cfg.cfg[zone_index].brightness),DATATYPE_UINT8,NULL);
            break;
            
        case 9:
            init_option(option_temp,&(jy_argb_cfg.cfg[zone_index].direction),DATATYPE_UINT8|DATATYPE_OPTION,&direction);
            break;  
        default:
            init_option(option_temp,NULL,DATATYPE_NONE,NULL);
            break;
        }
    }
}

void jy_option_list_cb(lv_event_t *e)
{
    jy_sub_option_list_t * sublist = (jy_sub_option_list_t *)lv_malloc(sizeof(jy_sub_option_list_t));
    lv_memcpy(sublist,e->user_data,sizeof(jy_sub_option_list_t));

    const UINT32 option_x = 400;
    const UINT32 option_y = 25;
    UINTN num_option = sublist->number_option;

    lv_obj_t * mask = lv_obj_create(lv_screen_active());
    lv_obj_set_size(mask,MY_DISP_HOR_RES,MY_DISP_VER_RES);
    lv_obj_set_pos(mask,0,0);
    lv_obj_set_style_bg_color(mask,lv_color_hex(0x3F3F3F),0);
    lv_obj_set_style_bg_opa(mask,LV_OPA_70,0);
    lv_obj_set_style_radius(mask,0,0);

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
    lv_label_set_text(title_label,"test example");
    lv_obj_set_style_text_color(title_label,lv_color_hex(0x000000),0);
    lv_obj_t * option_label = NULL;
    lv_obj_t * obj_option = NULL;

    for (UINT8 i = 0; i < num_option; i++)
    {
        obj_option = lv_obj_create(msgbox);
        lv_obj_set_size(obj_option,option_x,option_y);
        lv_obj_set_pos(obj_option,0,30+i*(option_y+5));
        option_style_add(obj_option);

        title_label = lv_label_create(obj_option);
        lv_obj_set_align(title_label,LV_ALIGN_LEFT_MID);
        lv_label_set_text(title_label,sublist->sub_option_string_list[i]);
        lv_obj_set_style_text_color(title_label,lv_color_hex(0x000000),0);
        lv_obj_add_event_cb(obj_option,jy_option_list_cb_suboption_cb,LV_EVENT_CLICKED,sublist);       
    }

    lv_gridnav_remove(lv_obj_get_parent(e->current_target));
    lv_group_remove_all_objs(lv_group_get_default());
    lv_group_add_obj(lv_group_get_default(),msgbox);
    lv_gridnav_add(msgbox,LV_GRIDNAV_CTRL_ROLLOVER);
    lv_obj_add_event_cb(msgbox,esc_key_callback_exit_msg_1,LV_EVENT_KEY,sublist);
    lv_gridnav_set_focused(msgbox,lv_obj_get_child(msgbox,0),LV_ANIM_OFF);

}

void jy_option_list_cb_suboption_cb(lv_event_t *e)
{
    jy_sub_option_list_t * sublist = e->user_data;
    UINTN num_option = sublist->number_option;
    UINTN target_index = lv_obj_get_index(e->current_target)-1;
    void * data_ptr = sublist->data_ptr;
    UINT16 data_type = sublist->data_type;
    lv_obj_t * mask = lv_obj_get_parent(lv_obj_get_parent(e->current_target));
    lv_obj_del_async(mask);
    UINTN data = sublist->key_list[target_index];
    
    /*
    switch (lv_obj_get_index(sublist->obj_option))
    {
    case 1:
        jy_argb_cfg.zone_choice = data; 
        break;
    case 2:
        jy_argb_cfg.cfg[jy_argb_cfg.zone_choice-1].modes = data;
        break;
    case 9:
        jy_argb_cfg.cfg[jy_argb_cfg.zone_choice-1].direction = data;
        break;
    default:
        break;
    } 
    init_option_all(lv_obj_get_parent(sublist->obj_option));
    */
    //lv_gridnav_add(lv_obj_get_parent(sublist->obj_option),LV_GRIDNAV_CTRL_ROLLOVER);
    lv_group_add_obj(lv_group_get_default(),lv_obj_get_parent(sublist->obj_option));
    //lv_gridnav_set_focused(lv_obj_get_parent(sublist->obj_option),sublist->obj_option,LV_ANIM_OFF);
}
