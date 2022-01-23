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

#define USE_DET
#define USE_COM

 #include "LinSM.h"
 #include "LinIf.h"          /* SWS_LinSM_00001 */
 #include "ComM.h"           /* SWS_LinSM_00013, SWS_LinSM_00105 */
 #include "BswM.h"           /* SWS_LinSM_00196 */
 #include "BswM_LinSM.h"     /* SWS_LinSM_00201 */
 #include "ComM_BusSM.h"     /* SWS_LinSM_00305 */
 #include "Det.h"
 
 /* SWS_LinSM_00219 */
 #include "Rte_ComM_Type.h"
 #include "ComStackTypes.h"
 #include "LinTrcv.h"
//#include "STATIC.h"


#define LINIF_CONTROLLER_CNT 1
#define LINIF_SCH_CNT 1
#define INVALID_COM_GROUP 0xFFFF

STATIC uint8 ScheduleRequestTimer[LINIF_CONTROLLER_CNT];
STATIC uint8 GoToSleepTimer[LINIF_CONTROLLER_CNT];
STATIC uint8 WakeUpTimer[LINIF_CONTROLLER_CNT];
STATIC uint32 comRxPduIdGrp[LINIF_CONTROLLER_CNT], comTxPduIdGrp[LINIF_CONTROLLER_CNT];

/* SWS_LinSM_00020*/
/* SWS_LinSM_00161*/
STATIC LinSM_StatusType LinSMStatus = LINSM_UNINIT;
/* SWS_LinSM_00173*/
/* SWS_LinSM_00021*/
STATIC LinSM_StatusType LinSMNetworkStatus[LINIF_CONTROLLER_CNT];
//-----------------------------------------------
///* Development error macros. */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
#define VALIDATE(_exp,_api,_err )                       \
        if( !(_exp) ) {                                 \
          Det_ReportError(MODULE_ID_LINSM,0,_api,_err); \
          return;                                       \
        }

#define VALIDATE_W_RV(_exp,_api,_err,_rv )              \
        if( !(_exp) ) {                                 \
          Det_ReportError(MODULE_ID_LINSM,0,_api,_err); \
          return (_rv);                                 \
        }
#else
#define VALIDATE(_exp,_api,_err )
#define VALIDATE_W_RV(_exp,_api,_err,_rv )
#endif

/*
This function initializes the SM
---
SWS_LinSM_00155
*/
void LinSM_Init(const LinSM_ConfigType* ConfigPtr)
{
	/* SWS_LinSM_00151 */

	(void)ConfigPtr;
	uint8 i;
	for (i = 0; i < LINIF_CONTROLLER_CNT; i++)
	{
		/* SWS_LinSM_00152*/
		/* SWS_LinSM_00043*/
		/* SWS_LinSM_00152*/
		/* SWS_LinSM_00166 */
		LinSMNetworkStatus[i] = LINSM_NO_COM;
		/* SWS_LinSM_00216*/
		ScheduleRequestTimer[i] = 0;
		GoToSleepTimer[i] = 0;
		WakeUpTimer[i] = 0;

		comRxPduIdGrp[i] = INVALID_COM_GROUP;
		comTxPduIdGrp[i] = INVALID_COM_GROUP;
	}
	/* SWS_LinSM_00025*/
	/* SWS_LinSM_00160*/
	LinSMStatus = LINSM_INIT;
}

/*
The upper layer requests a schedule table to be changed on the one LIN network
---
SWS_LinSM_00113
*/
Std_ReturnType LinSM_ScheduleRequest(NetworkHandleType network, LinIf_SchHandleType schedule)
{
	/* SWS_LinSM_00241 */
	/* SWS_LinSM_00114 */
	if (!(network < LINIF_CONTROLLER_CNT))
	{
		Det_ReportError(MODULE_ID_LINSM, 0, LINSM_SCHEDULE_REQUEST_SERVICE_ID, LINSM_E_NONEXISTENT_NETWORK);
		return E_NOT_OK;
	}

	/* SWS_LinSM_00115 */
	if (!(schedule < LINIF_SCH_CNT))
	{
		Det_ReportError(MODULE_ID_LINSM, 0, LINSM_SCHEDULE_REQUEST_SERVICE_ID, LINSM_E_PARAMETER);
		return E_NOT_OK;
	}

	/* SWS_LinSM_00116 */
	if (LinSMStatus == LINSM_UNINIT)
	{
		Det_ReportError(MODULE_ID_LINSM, 0, LINSM_SCHEDULE_REQUEST_SERVICE_ID, LINSM_E_UNINIT);
		return E_NOT_OK;
	}

	/* SWS_LinSM_10211 */
	if (LinSMNetworkStatus[network] != LINSM_FULL_COM) 
	{
		return E_NOT_OK;
	}

	ScheduleRequestTimer[network] = LINSM_SCHEDULE_REQUEST_TIMEOUT;

	/* SWS_LinSM_00079 */
	/* SWS_LinSM_00168 */
	return LinIf_ScheduleRequest(network, schedule);
}

/*
---
SWS_LinSM_00117
*/
void LinSM_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
	/* SWS_LinSM_00119 */	
	if (NULL != versioninfo) 
	{
		versioninfo->vendorID = LIN_VENDOR_ID;
		versioninfo->moduleID = LIN_MODULE_ID;
		versioninfo->sw_major_version = LIN_SW_MAJOR_VERSION;
		versioninfo->sw_minor_version = LIN_SW_MINOR_VERSION;
		versioninfo->sw_patch_version = LIN_SW_PATCH_VERSION;
	}
	else if (!(versioninfo != NULL))
	{
		/* SWS_LinSM_00119 */
		Det_ReportError(MODULE_ID_LINSM, 0, LINSM_SCHEDULE_REQUEST_SERVICE_ID, LINSM_E_PARAMETER_POINTER);
	}
	else
	{
		/* Do nothing */
	}
}

/* SWS_LinSM_00122 */
Std_ReturnType LinSM_GetCurrentComMode(NetworkHandleType network, ComM_ModeType* mode)
{
	/* SWS_LinSM_00125 */
	VALIDATE_W_RV((LinSMStatus != LINSM_UNINIT), LINSM_GET_CURRENT_COM_MODE_SERVICE_ID, LINSM_E_UNINIT, E_NOT_OK);
	/* SWS_LinSM_00123 */
	VALIDATE_W_RV((network < LINIF_CONTROLLER_CNT), LINSM_GET_CURRENT_COM_MODE_SERVICE_ID, LINSM_E_NONEXISTENT_NETWORK, E_NOT_OK);
	/* SWS_LinSM_00124 */
	VALIDATE_W_RV((mode != NULL), LINSM_GET_CURRENT_COM_MODE_SERVICE_ID, LINSM_E_PARAMETER_POINTER, E_NOT_OK);

	switch (LinSMStatus)
	{
		/* SWS_LinSM_00182 */
	case LINSM_UNINIT:
		return COMM_NO_COMMUNICATION;
		/* SWS_LinSM_00181 */
	case LINSM_FULL_COM:
		return COMM_FULL_COMMUNICATION;
		/* SWS_LinSM_00180 */
	case LINSM_NO_COM:
		return COMM_NO_COMMUNICATION;
	}
	return E_OK;
}

/*
Requesting of a communication mode
The mode switch will not be made instant. The LinSM will notify the caller when mode transition is made
---
SWS_LinSM_00126
*/
Std_ReturnType LinSM_RequestComMode(NetworkHandleType network, ComM_ModeType mode)
{
	Std_ReturnType  res = E_NOT_OK;
	ComM_ModeType   mode_int;
	/* SWS_LinSM_00128 */
	VALIDATE_W_RV((LinSMStatus != LINSM_UNINIT), LINSM_REQUEST_COM_MODE_SERVICE_ID, LINSM_E_UNINIT, E_NOT_OK);
	/* SWS_LinSM_00127 */
	VALIDATE_W_RV((network < LINIF_CONTROLLER_CNT), LINSM_REQUEST_COM_MODE_SERVICE_ID, LINSM_E_NONEXISTENT_NETWORK, E_NOT_OK);
	/* SWS_LinSM_00191 */
	VALIDATE_W_RV((mode >= COMM_FULL_COMMUNICATION), LINSM_REQUEST_COM_MODE_SERVICE_ID, LINSM_E_PARAMETER, E_NOT_OK);

	switch (mode)
	{
	case COMM_NO_COMMUNICATION:
		/* SWS_LinSM_00178 */
		if (E_OK == LinIf_GotoSleep(network)) 
		{
			LinSMNetworkStatus[network] = LINSM_GOTO_SLEEP;
			GoToSleepTimer[network] = LINSM_GOTO_SLEEP_TIMEOUT;
			res = E_OK;
			/* SWS_LinSM_00223 */
			mode_int = mode;
		}
		break;
		/* SWS_LinSM_00183 */
	case COMM_SILENT_COMMUNICATION:
		/* SWS_LinSM_00178 */
		res = E_NOT_OK;
		break;
	case COMM_FULL_COMMUNICATION:
		/* SWS_LinSM_00047 */
		WakeUpTimer[network] = LINSM_WAKEUP_TIMEOUT; //should be done here since some implementations will confirm immediatly
		if (E_OK == LinIf_WakeUp(network)) 
		{
			res = E_OK;
			/* SWS_LinSM_00223 */
			mode_int = mode;
		}
		else
		{
			WakeUpTimer[network] = 0;
		}
		break;
	default:
		/* SWS_LinSM_00178 */
		break;
	}
	/* SWS_LinSM_00167 */
	return res;
}

/*
Periodic function that runs the timers of different request timeouts
---
SWS_LinSM_00156
*/
void LinSM_MainFunction(void)
{
	/* SWS_LinSM_00157 */
	// TIMERS HANDLING
	/* SWS_LinSM_00162 */
}

/*
The LinIf module will call this callback when the nwe requested schedule table is active
---
SWS_LinSM_00129
*/
void LinSM_ScheduleRequestConfirmation(NetworkHandleType network, LinIf_SchHandleType schedule)
{
	/* SWS_LinSM_00242 */

	/* SWS_LinSM_00131 */
	VALIDATE((LinSMStatus != LINSM_UNINIT), LINSM_SCHEDULE_REQUEST_CONF_SERVICE_ID, LINSM_E_UNINIT);
	/* SWS_LinSM_00130 */
	VALIDATE((network < LINIF_CONTROLLER_CNT), LINSM_SCHEDULE_REQUEST_CONF_SERVICE_ID, LINSM_E_NONEXISTENT_NETWORK);

	if (ScheduleRequestTimer[network] != 0) {
		BswM_LinSM_CurrentSchedule(network, schedule);
		ScheduleRequestTimer[network] = 0;
	}
}

/*
The LinIF will call this callback when the go to sleep command is received on the network or a bus idle timeout occurs
Only applicable for LIN slave nodes
---
SWS_LinSM_91000
*/
void LinSM_GotoSleepIndication(NetworkHandleType channel)
{
	//This function is not implemented as we are working only in mastrer mode
}

/*
The LinIf will call this callback when go to sleep command is sent succesfully or not sent succesfully on the network
---
SWS_LinSM_00135
*/
void LinSM_GotoSleepConfirmation(NetworkHandleType newtork, boolean success)
{
	/* SWS_LinSM_00137 */
	VALIDATE((LinSMStatus != LINSM_UNINIT), LINSM_GOTO_SLEEP_CONF_SERVICE_ID, LINSM_E_UNINIT);
	/* SWS_LinSM_00136 */
	VALIDATE((newtork < LINIF_CONTROLLER_CNT), LINSM_GOTO_SLEEP_CONF_SERVICE_ID, LINSM_E_NONEXISTENT_NETWORK);

	if (TRUE == success)
	{
		if (GoToSleepTimer[newtork] != 0) {
			GoToSleepTimer[newtork] = 0;
			/* SWS_LinSM_00035 */
			LinSMNetworkStatus[newtork] = LINSM_NO_COM;
			/* SWS_LinSM_00027 */
			/* SWS_LinSM_00193 */
			ComM_BusSM_ModeIndication(newtork, COMM_NO_COMMUNICATION);
		}
	}
}

/*
The LinIf will call this callback when the wake up signal command is sent not succesfully/succesfully on the network
---
SWS_LinSM_00132
*/
void LinSM_WakeUpConfirmation(NetworkHandleType network, boolean success)
{
	/* SWS_LinSM_00134 */
	VALIDATE((LinSMStatus != LINSM_UNINIT), LINSM_WAKEUP_CONF_SERVICE_ID, LINSM_E_UNINIT);
	/* SWS_LinSM_00133 */
	VALIDATE((network < LINIF_CONTROLLER_CNT), LINSM_WAKEUP_CONF_SERVICE_ID, LINSM_E_NONEXISTENT_NETWORK);

	if (TRUE == success)
	{
		/* SWS_LinSM_00049 */
		LinSMStatus = LINSM_FULL_COM;

		//if (WakeUpTimer[network] != 0) {
			WakeUpTimer[network] = 0;
			/* SWS_LinSM_00033 */
			ComM_BusSM_ModeIndication(network, COMM_FULL_COMMUNICATION);
			/* SWS_LinSM_00301 */
			LinSMNetworkStatus[network] = LINSM_RUN_COMMUNICATION;
		//}
	}
	/* SWS_LinSM_00202 */
}

/* SWS_LinSM_00175 */
void LinSM_TimerTick(void)
{

}

#ifdef UNIT_TEST

void setLinSMStatus(LinSM_StatusType status)
{
    LinSMStatus = status;
}

void setLinSMNetworkStatus(LinSM_StatusType status)
{
    LinSMNetworkStatus[0] = status;
}

#endif // UNIT_TEST

