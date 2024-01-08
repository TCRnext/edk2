#pragma once
#include "jy_lvgl.h"

#define JGINYUE_USB_GENERAL_COMMAND_HEADER              0x01
#define JGINYUE_USB_LED_STRIPE_SET_COMMAND_HEADER       0x05
#define JGINYUE_USB_MODE_SET_COMMAND_HEADER             0x06
#define JGINYUE_USB_PER_LED_SET_COMMAND_HEADER          0x04    //NOT USED

#define JGINYUE_USB_GET_FW_VERSION                      0xA0
#define JGINYUE_USB_GET_FW_REPLY                        0x5A
#define JGINYUE_RG_DEFAULT                              0x01
#define JGINYUE_RG_SWAP                                 0x00

enum
{
    JGINYUE_USB_MODE_OFF                = 0x10,
    JGINYUE_USB_MODE_STATIC             = 0x11,
    JGINYUE_USB_MODE_BREATHING          = 0x12,
    JGINYUE_USB_MODE_STROBE             = 0x13,
    JGINYUE_USB_MODE_CYCLING            = 0x14,
    JGINYUE_USB_MODE_RANDOM             = 0x15,
    JGINYUE_USB_MODE_MUSIC              = 0x16, /* music mode,not support yet                                   */
    JGINYUE_USB_MODE_WAVE               = 0x17,
    JGINYUE_USB_MODE_SPRING             = 0x18, /* spring mode,not support yet                                  */
    JGINYUE_USB_MODE_WATER              = 0x19,
    JGINYUE_USB_MODE_RAINBOW            = 0x1A, /* rainbow mode,not support yet                                 */
    JGINYUE_USB_MODE_DIRECT             = 0x20, /* Not the exact USB protcol  - but need a way to differentiate */
};

enum
{
    JGINYUE_USB_SPEED_MAX               = 0xFF,
    JGINYUE_USB_SPEED_MIN               = 0x00,
    JGINYUE_USB_SPEED_DEFAULT           = 0x80
};

enum
{
    JGINYUE_DIRECTION_RIGHT             = 0x00,
    JGINYUE_DIRECTION_LEFT              = 0x01
};

enum
{
    JGINYUE_USB_BRIGHTNESS_MAX          = 0xFF,
    JGINYUE_USB_BRIGHTNESS_MIN          = 0x00,
    JGINYUE_USB_BRIGHTNESS_DEFAULT      = 0x80
};

enum 
{
    DATATYPE_NONE = 0x0000,
    DATATYPE_UINT8 = 0x0001,
    DATATYPE_UINT16 = 0x0002,
    DATATYPE_UINT32 = 0x0004,
    DATATYPE_UINT64 = 0x0008,
    DATATYPE_INT8 = 0x0010,
    DATATYPE_INT16 = 0x0020,
    DATATYPE_INT32 = 0x0040,
    DATATYPE_INT64 = 0x0080,
    DATATYPE_CHAR8 = 0x0100,
    DATATYPE_CHAR16 = 0x0200,
    DATATYPE_BOOL_OPTION = 0x0400,
    DATATYPE_OPTION = 0x0800,
};
void argbmenu_init(jy_page_t * page_element_1);