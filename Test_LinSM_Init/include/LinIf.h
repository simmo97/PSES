#ifndef LINIF_H
#define LINIF_H

#include <stdint.h>
#include "StandardTypes.h"
#include "ComStackTypes.h"

typedef uint8_t LinIf_SchHandleType;

Std_ReturnType LinIf_ScheduleRequest(NetworkHandleType network, LinIf_SchHandleType schedule);
void resetLinIf_ScheduleRequest(void);
int getLinIfScheduleRequestCallCounter(void);

Std_ReturnType LinIf_WakeUp(NetworkHandleType Channel);
Std_ReturnType LinIf_GotoSleep(NetworkHandleType Channel);

// from LinIf_cfg.h
#define LINIF_CONTROLLER_CNT    1
#define LINIF_SCH_CNT           1

#endif
