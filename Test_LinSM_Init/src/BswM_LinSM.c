#include "BswM_LinSM.h"

static NetworkHandleType Network_mock;
static LinIf_SchHandleType CurrentSchedule_mock;

void BswM_LinSM_CurrentSchedule(NetworkHandleType Network, LinIf_SchHandleType CurrentSchedule)
{
    Network_mock = Network;
    CurrentSchedule_mock = CurrentSchedule;
}

void BswM_LinSM_CurrentState(NetworkHandleType Network, LinSM_ModeType CurrentState)
{
    (void)Network;
    (void)CurrentState;
}


#ifdef TEST_FUNCTIONS

NetworkHandleType GetBswM_LinSM_CurrentScheduleNetwork(void)
{
    return Network_mock;
}

LinSM_ModeType GetBswM_LinSM_CurrentSchedule(void)
{
    return CurrentSchedule_mock;
}

#endif
