#include "jy_lvgl.h"
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "dev/display.h"
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>

#include "jginyuepage1_demo.h"
#include "jginyuepage1_mainpart.h"
#define EMULATOR
//build -p edk2-my\testpkg\test.dsc -a X64 -b RELEASE



int _fltused = 0;//?
int is_exit = 0;

EFI_SYSTEM_TABLE  *SystemTable1 = NULL;
void my_log_cb(lv_log_level_t level, const char * buf);
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
    jy_page_t * page1 = NULL;

    SystemTable1=SystemTable;
    SystemTable->ConOut->OutputString(SystemTable->ConOut,L"Test SystemTable...\n\r");
    
    lv_init();
    lv_uefi_driver_init();
    lv_log_register_print_cb(my_log_cb);

    page1 = test_page1();
    mainpart_init(page1);
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
        if(is_exit)
        {
            FreePool(jy_get_buf_addr());
            lv_deinit();
            FreePool(jy_get_mem_pool_addr());
            break;
        }
    }
    return EFI_SUCCESS;
}

void my_log_cb(lv_log_level_t level, const char * buf)
{
    DEBUG ((DEBUG_ERROR, "%s\n",buf));
}
