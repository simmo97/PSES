#include "Det.h"
#include <assert.h>
#include <stdio.h>

void Det_Start(void){

}

Std_ReturnType Det_ReportError (uint16 ModuleId, uint8 Instance_Id, uint8 ApiId, uint8 ErrorId){
    (void)ModuleId;
    (void)Instance_Id;
    (void)ApiId;
    printf("Error Id: 0x%02X\n", ErrorId);
    return E_OK;
}

Std_ReturnType Det_ReportRuntimeError (uint16 ModuleId, uint8 Instance_Id, uint8 ApiId, uint8 ErrorId){
    (void)ModuleId;
    (void)Instance_Id;
    (void)ApiId;
    printf("Runtime Error Id: 0x%02X\n", ErrorId);
    return E_OK;
}

Std_ReturnType Det_ReportTransientFault (uint16 ModuleId, uint8 Instance_Id, uint8 ApiId, uint8 FaultId){
    (void)ModuleId;
    (void)Instance_Id;
    (void)ApiId;
    printf("TransientFault Id: 0x%02X\n", FaultId);
    return E_OK;
}


void Det_GetVersionInfo (Std_VersionInfoType* versioninfo){
    assert(versioninfo != NULL);
}
