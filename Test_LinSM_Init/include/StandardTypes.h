#ifndef STANDARD_TYPES_H
#define STANDARD_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//typedef _Bool boolean;
#define bool                        int
#define TRUE                            1
#define FALSE                           0
#define boolean bool
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

typedef uint8 Std_ReturnType;

#define E_OK        (Std_ReturnType)0
#define E_NOT_OK    (Std_ReturnType)1

#define STD_ON      0x01
#define STD_OFF     0x00

typedef struct {
    uint16 vendorID;
    uint16 moduleID;
    uint8 sw_major_version;    
    uint8 sw_minor_version;    
    uint8 sw_patch_version;    
} Std_VersionInfoType;

#endif
