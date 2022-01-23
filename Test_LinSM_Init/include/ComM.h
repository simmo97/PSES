#ifndef COMM_H
#define COMM_H

#include "StandardTypes.h"
#include "Rte_ComM_Type.h"
#include "ComStackTypes.h"

/*Indication of the actual bus mode by the corresponding Bus State Manager.
ComM shall propagate the indicated state to the users with means of the RTE and BswM.*/
void ComM_BusSM_ModeIndication(NetworkHandleType Channel, ComM_ModeType ComMode);

#endif
