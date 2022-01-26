/* -------------------------------- Arctic Core ------------------------------
 * Arctic Core - the open source AUTOSAR platform http://arccore.com
 *
 * Copyright (C) 2009  ArcCore AB <contact@arccore.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * -------------------------------- Arctic Core ------------------------------*/

#include "LinIf.h"

static int callCounter = 0;

Std_ReturnType LinIf_ScheduleRequest(NetworkHandleType network, LinIf_SchHandleType schedule){
    (void)network;
    (void)schedule;
    callCounter++; 
    return E_OK;
}

Std_ReturnType LinIf_WakeUp(NetworkHandleType Channel){
    (void)Channel;
    return E_OK;
}

Std_ReturnType LinIf_GotoSleep(NetworkHandleType Channel){
    (void)Channel;
    return E_OK;
}

void resetLinIf_ScheduleRequest(void){
    callCounter = 0;
}

int getLinIfScheduleRequestCallCounter(void){
    return callCounter;
}
