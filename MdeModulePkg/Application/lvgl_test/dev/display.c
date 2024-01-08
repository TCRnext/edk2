#include <Uefi.h>
#include "display.h"
#include "../lvgl.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
EFI_GRAPHICS_OUTPUT_PROTOCOL          	*Gop = NULL;
static lv_color32_t 					*buf_1_1=NULL;
void lv_port_disp_init(void)
{
  	lv_port_GOP_init();
  	lv_display_t * disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
  	lv_display_set_flush_cb(disp, disp_flush);
  	buf_1_1=(lv_color32_t*)AllocatePool(sizeof(lv_color32_t)*(MY_DISP_HOR_RES * MY_DISP_VER_RES));  
  	lv_display_set_draw_buffers(disp,buf_1_1,NULL,sizeof(lv_color32_t)*(MY_DISP_HOR_RES * MY_DISP_VER_RES),LV_DISPLAY_RENDER_MODE_PARTIAL);
  	lv_display_set_color_format(disp,LV_COLOR_FORMAT_NATIVE_WITH_ALPHA);
}

void disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
  	EFI_GRAPHICS_OUTPUT_PROTOCOL       *GraphicsOutput;
  	EFI_STATUS  Status = 0;
  	UINTN Width, Height;
  	GraphicsOutput = Gop;


  	if (area->x2 - area->x1 >= 0)
	{
      	Width = area->x2 - area->x1;
  	}
  	else 
	{
      	Width = area->x1 - area->x2;
  	}

  	if (area->y2 - area->y1 >= 0) 
	{
      Height = area->y2 - area->y1;
  	}
 	else 
	{
      Height = area->y1 - area->y2;
  	}
  	Width  ++;
  	Height ++;
  	Status = GraphicsOutput->Blt (
    GraphicsOutput,
    (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) px_map,
    EfiBltBufferToVideo,
    0,
    0,
    area->x1,
    area->y1,
    Width,
    Height,
    0
  	);
  	lv_display_flush_ready(disp);         /* Indicate you are ready with the flushing*/
  	return;
}

int lv_port_GOP_init(void)
{
  	EFI_STATUS                            Status;
  	UINT32                                Index = 0;
  	UINTN                                 SizeOfInfo = 0;
  	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *ModeInfo = NULL;

  	Status=SystemTable1->BootServices->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,NULL,(VOID **)&Gop);
  	for (Index = 0; Index < Gop->Mode->MaxMode; Index++) 
  	{
    	Status = Gop->QueryMode (Gop, Index, &SizeOfInfo, &ModeInfo);
    	if((ModeInfo->HorizontalResolution==MY_DISP_HOR_RES)&&(ModeInfo->VerticalResolution==MY_DISP_VER_RES))
    	{
      		Gop->SetMode(Gop,Index);
      		FreePool(ModeInfo);
      		return 0;
    	}
    	FreePool (ModeInfo);
  	}

  	for (Index = 0; Index < Gop->Mode->MaxMode; Index++) 
  	{
    	Status = Gop->QueryMode (Gop, Index, &SizeOfInfo, &ModeInfo);
    	if((ModeInfo->HorizontalResolution>=MY_DISP_HOR_RES)&&(ModeInfo->VerticalResolution>=MY_DISP_VER_RES))
    	{
      		Gop->SetMode(Gop,Index);
      		FreePool(ModeInfo);
      		return 0;
    	}
    	FreePool (ModeInfo);
  	}
  	return 1;
}

void * jy_get_buf_addr(void)
{
	return (void *)buf_1_1;
}