#include "keyboard.h"
#include <Uefi.h>
#include "SDL\SDL.h"
#include "..\lvgl.h"

lv_obj_t * label3 = NULL;

void keypad_init(void)
{

    lv_indev_t * indev_keyboard = lv_indev_create();
    lv_indev_set_type(indev_keyboard,LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(indev_keyboard,keypad_read);
    label3 = lv_label_create(lv_screen_active());
    lv_obj_align(label3,LV_ALIGN_TOP_RIGHT,0,0);
    lv_indev_set_group(indev_keyboard,lv_group_get_default());
}

void keypad_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;
    Status =  SystemTable1->ConIn->ReadKeyStroke(SystemTable1->ConIn, &Key);
    if (Status == EFI_SUCCESS)
    {
        data->key = keycode_to_ascii(Key);
        data->state = LV_INDEV_STATE_PR;
        if (lv_obj_get_parent(label3)!=lv_screen_active())
        {
            lv_obj_set_parent(label3,lv_screen_active());   
        }
        lv_label_set_text_fmt(label3,"Key:%d %d", Key.ScanCode, Key.UnicodeChar);
        lv_obj_move_foreground(label3);
    }
    else
    {
    data->state = LV_INDEV_STATE_REL;
    }
    

}

uint32_t keycode_to_ascii(EFI_INPUT_KEY sdl_key)
{
    /*Remap some key to LV_KEY_... to manage groups*/
    switch(sdl_key.ScanCode) {
        case SCAN_UP:
            return LV_KEY_UP;
        case SCAN_DOWN:
            return LV_KEY_DOWN;
        case SCAN_RIGHT:
            return LV_KEY_RIGHT;
        case SCAN_LEFT:
            return LV_KEY_LEFT;
        case SCAN_HOME:
            return LV_KEY_HOME;
        case SCAN_END:
            return LV_KEY_END;
        case SCAN_DELETE:
            return LV_KEY_DEL;
        case SCAN_PAGE_UP:
            return LV_KEY_PREV;
        case SCAN_PAGE_DOWN:
            return LV_KEY_NEXT;
        case SCAN_ESC:
            return LV_KEY_ESC;
        case SCAN_NULL:
            switch(sdl_key.UnicodeChar) {
                case 13:
                    return LV_KEY_ENTER;
                default:
                    return sdl_key.UnicodeChar;
            }
        default:
            return sdl_key.UnicodeChar;
    }
}