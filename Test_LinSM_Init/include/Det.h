#ifndef DET_H
#define DET_H

#include "StandardTypes.h"

void Det_Start(void);
Std_ReturnType Det_ReportError (uint16 ModuleId, uint8 Instance_Id, uint8 ApiId, uint8 ErrorId);
Std_ReturnType Det_ReportRuntimeError (uint16 ModuleId, uint8 Instance_Id, uint8 ApiId, uint8 ErrorId);
Std_ReturnType Det_ReportTransientFault (uint16 ModuleId, uint8 Instance_Id, uint8 ApiId, uint8 FaultId);
void Det_GetVersionInfo (Std_VersionInfoType* versioninfo);

#endif
