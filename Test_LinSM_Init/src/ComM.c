#include "ComM.h"
#include "STATIC.h"

STATIC NetworkHandleType Channel_mock;
STATIC ComM_ModeType ComMode_mock;

void ComM_BusSM_ModeIndication(NetworkHandleType Channel,ComM_ModeType ComMode)
{
    Channel_mock = Channel;
    ComMode_mock = ComMode;
}


#ifdef TEST_FUNCTIONS

NetworkHandleType GetComM_BusSM_ModeIndicationChannel(void)
{
    return Channel_mock;
}

ComM_ModeType GetComM_BusSM_ModeIndicationComMode(void)
{
    return ComMode_mock;
}

void SetComM_BusSM_ModeIndicationChannel(NetworkHandleType Channel)
{
     Channel_mock = Channel;
}

void SetComM_BusSM_ModeIndicationComMode(ComM_ModeType mode)
{
    ComMode_mock = mode;
}

#endif
