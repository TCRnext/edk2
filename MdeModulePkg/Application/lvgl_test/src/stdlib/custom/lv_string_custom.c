/**
 * @file lv_string.c
 */

/*********************
 *      INCLUDES
 *********************/
#pragma once
#include "../../lv_conf_internal.h"
#if LV_USE_STDLIB_STRING == LV_STDLIB_CUSTOM
#include "../lv_string.h"
#include "../lv_mem.h" /*Need lv_malloc*/
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

LV_ATTRIBUTE_FAST_MEM void * lv_memcpy(void * dst, const void * src, size_t len)
{
    return CopyMem(dst, src, len);
}

LV_ATTRIBUTE_FAST_MEM void lv_memset(void * dst, uint8_t v, size_t len)
{
    SetMem(dst, len, v);
}

int lv_memcmp(const void * dst, const void * src, size_t len)
{
    return CompareMem(dst ,src ,len);
}


size_t lv_strlen(const char * str)
{
    return AsciiStrLen(str);
}

char * lv_strncpy(char * dst, const char * src, size_t dest_size)
{
    
    AsciiStrnCpyS(dst, AsciiStrLen(src)+1, src, dest_size);

    return dst;
}

char * lv_strcpy(char * dst, const char * src)
{
    AsciiStrCpyS (dst, AsciiStrLen(src)+1, src);
    return dst;
}

int32_t lv_strcmp(const char * s1, const char * s2)
{
    return AsciiStrCmp(s1, s2);
}

char * lv_strdup(const char * src)
{
    /*strdup uses malloc, so use the lv_malloc when LV_USE_STDLIB_MALLOC is not LV_STDLIB_CLIB */
    size_t len = lv_strlen(src) + 1;
    char * dst = lv_malloc(len);
    if(dst == NULL) return NULL;

    lv_memcpy(dst, src, len); /*do memcpy is faster than strncpy when length is known*/
    return dst;
}

char * lv_strcat(char * dst, const char * src)
{
    AsciiStrCatS(dst ,AsciiStrLen(dst)+AsciiStrLen(src) ,src);
    return dst;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /*LV_STDLIB_CLIB*/
