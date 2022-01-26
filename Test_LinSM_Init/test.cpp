#define UNIT_TEST 1
#define TEST_FUNCTIONS 
#include "gtest/gtest.h"
#include "STATIC.h"
#include "LinSM.c"
#include "Det.c"
#include "LinIf.c"
#include "BswM_LinSM.c"
#include "ComM.c"
/////////////////////////////////////////////////////////////////
// Included Files
/////////////////////////////////////////////////////////////////
#include <iostream>
#include "gtest/gtest.h"

using ::testing::UnitTest;
using namespace std;

/////////////////////////////////////////////////////////////////
//LinSM_Init_Tests
/////////////////////////////////////////////////////////////////

TEST(LinSM_Init_Test, InitializeVariables) {
	LinSM_ConfigType config_type = DEFAULT_CONFIG;
	LinSM_Init(&config_type);
	EXPECT_EQ(LinSMNetworkStatus[LINIF_CONTROLLER_CNT - 1], LINSM_NO_COM);

	EXPECT_EQ(ScheduleRequestTimer[LINIF_CONTROLLER_CNT - 1], 0);
	EXPECT_EQ(GoToSleepTimer[LINIF_CONTROLLER_CNT - 1], 0);
	EXPECT_EQ(WakeUpTimer[LINIF_CONTROLLER_CNT - 1], 0);

	EXPECT_EQ(comRxPduIdGrp[LINIF_CONTROLLER_CNT - 1], INVALID_COM_GROUP);
	EXPECT_EQ(comTxPduIdGrp[LINIF_CONTROLLER_CNT - 1], INVALID_COM_GROUP);
	
	EXPECT_EQ(LinSMStatus, LINSM_INIT);
}

/////////////////////////////////////////////////////////////////
//LinSM_ScheduleRequest_Tests
/////////////////////////////////////////////////////////////////

TEST(LinSM_ScheduleRequest_Test, NetworkStatus_NOCOM_SMStatus_INIT) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	LinIf_SchHandleType schedule = LINIF_SCH_CNT - 1;
	setLinSMStatus(LINSM_INIT);	

	setLinSMNetworkStatus(LINSM_NO_COM);
	EXPECT_EQ(E_NOT_OK, LinSM_ScheduleRequest(network, schedule));
}

TEST(LinSM_ScheduleRequest_Test, NetworkStatus_FULCOM_SMStatus_INIT) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	LinIf_SchHandleType schedule = LINIF_SCH_CNT - 1;
	setLinSMStatus(LINSM_INIT);

	setLinSMNetworkStatus(LINSM_FULL_COM);
	EXPECT_EQ(E_OK, LinSM_ScheduleRequest(network, schedule));
}

TEST(LinSM_ScheduleRequest_Test, NetworkStatus_FULCOM_SMStatus_UNINIT) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	LinIf_SchHandleType schedule = LINIF_SCH_CNT - 1;
	setLinSMNetworkStatus(LINSM_FULL_COM);

	setLinSMStatus(LINSM_UNINIT);
	EXPECT_EQ(E_NOT_OK, LinSM_ScheduleRequest(network, schedule));
}

TEST(LinSM_ScheduleRequest_Test, NetworkStatus_NOCOM_SMStatus_UNINIT) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	LinIf_SchHandleType schedule = LINIF_SCH_CNT - 1;
	setLinSMNetworkStatus(LINSM_NO_COM);

	setLinSMStatus(LINSM_UNINIT);
	EXPECT_EQ(E_NOT_OK, LinSM_ScheduleRequest(network, schedule));
}

TEST(LinSM_ScheduleRequest_Test, Test_Network_NOTOK) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT;
	LinIf_SchHandleType schedule = LINIF_SCH_CNT - 1;
	setLinSMStatus(LINSM_INIT);
	setLinSMNetworkStatus(LINSM_FULL_COM);
	EXPECT_EQ(E_NOT_OK, LinSM_ScheduleRequest(network, schedule));
}

TEST(LinSM_ScheduleRequest_Test, Test_Schedule_NOTOK) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	LinIf_SchHandleType schedule = LINIF_SCH_CNT ;
	setLinSMStatus(LINSM_INIT);
	setLinSMNetworkStatus(LINSM_FULL_COM);
	EXPECT_EQ(E_NOT_OK, LinSM_ScheduleRequest(network, schedule));
}

/////////////////////////////////////////////////////////////////
//LinSM_GetCurrentComMode_Tests
/////////////////////////////////////////////////////////////////

TEST(LinSM_GetCurrentComMode_Test, Test_SMStatus_OK) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	ComM_ModeType mode_v;
	ComM_ModeType* mode = &mode_v;
	setLinSMStatus(LINSM_INIT);

	EXPECT_EQ(E_OK, LinSM_GetCurrentComMode(network, mode));
}

TEST(LinSM_GetCurrentComMode_Test, Test_SMStatus_NOTOK) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	ComM_ModeType mode_v;
	ComM_ModeType* mode = &mode_v;
	setLinSMStatus(LINSM_UNINIT);

	EXPECT_EQ(E_NOT_OK, LinSM_GetCurrentComMode(network, mode));
}

TEST(LinSM_GetCurrentComMode_Test, Test_Network_OK) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	ComM_ModeType mode_v;
	ComM_ModeType* mode = &mode_v;
	setLinSMStatus(LINSM_INIT);

	EXPECT_EQ(E_OK, LinSM_GetCurrentComMode(network, mode));
}

TEST(LinSM_GetCurrentComMode_Test, Test_Network_NOTOK) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT;
	ComM_ModeType mode_v;
	ComM_ModeType* mode = &mode_v;
	setLinSMStatus(LINSM_INIT);

	EXPECT_EQ(E_NOT_OK, LinSM_GetCurrentComMode(network, mode));
}

TEST(LinSM_GetCurrentComMode_Test, Test_Mode_OK) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	ComM_ModeType mode_v = COMM_NO_COMMUNICATION;
	ComM_ModeType* mode = &mode_v;
	setLinSMStatus(LINSM_INIT);

	EXPECT_EQ(E_OK, LinSM_GetCurrentComMode(network, mode));
}

TEST(LinSM_GetCurrentComMode_Test, Test_Mode_NOTOK) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	ComM_ModeType* mode = NULL;
	setLinSMStatus(LINSM_INIT);

	EXPECT_EQ(E_NOT_OK, LinSM_GetCurrentComMode(network, mode));
}

/////////////////////////////////////////////////////////////////
//LinSM_RequestComMode_Tests
/////////////////////////////////////////////////////////////////

TEST(LinSM_RequestComMode_Test, Test_Status_UNINIT) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	ComM_ModeType mode = COMM_FULL_COMMUNICATION;
	setLinSMStatus(LINSM_UNINIT);

	EXPECT_EQ(E_NOT_OK, LinSM_RequestComMode(network, mode));
}

TEST(LinSM_RequestComMode_Test, Test_Status_INIT) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	ComM_ModeType mode = COMM_FULL_COMMUNICATION;
	setLinSMStatus(LINSM_INIT);

	EXPECT_EQ(E_OK, LinSM_RequestComMode(network, mode));
}

TEST(LinSM_RequestComMode_Test, Test_Network_OK) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	ComM_ModeType mode = COMM_FULL_COMMUNICATION;
	setLinSMStatus(LINSM_INIT);

	EXPECT_EQ(E_OK, LinSM_RequestComMode(network, mode));
}

TEST(LinSM_RequestComMode_Test, Test_Network_NOTOK) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT;
	ComM_ModeType mode = COMM_FULL_COMMUNICATION;
	setLinSMStatus(LINSM_INIT);

	EXPECT_EQ(E_NOT_OK, LinSM_RequestComMode(network, mode));
}

TEST(LinSM_RequestComMode_Test, Test_Mode_NOTOK) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	ComM_ModeType mode = COMM_SILENT_COMMUNICATION;
	setLinSMStatus(LINSM_INIT);

	EXPECT_EQ(E_NOT_OK, LinSM_RequestComMode(network, mode));
}

TEST(LinSM_RequestComMode_Test, Test_Mode_OK) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	ComM_ModeType mode = COMM_FULL_COMMUNICATION;
	setLinSMStatus(LINSM_INIT);

	EXPECT_EQ(E_OK, LinSM_RequestComMode(network, mode));
}

/////////////////////////////////////////////////////////////////
//LinSM_GetVersionInfo_Test
/////////////////////////////////////////////////////////////////

TEST(LinSM_GetVersionInfo_Test, FillValidPointerWithVersionInforamtion)
{
	Std_VersionInfoType versioninfo = { 0, 0, 0, 0, 0 };

	LinSM_GetVersionInfo(&versioninfo);

	// expected {1, 11, 21, 31, 41}
	EXPECT_EQ(versioninfo.vendorID, 1);
	EXPECT_EQ(versioninfo.moduleID, 11);
	EXPECT_EQ(versioninfo.sw_major_version, 21);
	EXPECT_EQ(versioninfo.sw_minor_version, 31);
	EXPECT_EQ(versioninfo.sw_patch_version, 41);
}

/////////////////////////////////////////////////////////////////
//LinSM_WakeUpConfirmation_Tests
/////////////////////////////////////////////////////////////////

TEST(LinSM_WakeUpConfirmation_Test, Set_LinSMStatus_LINSM_FULL_COM_AfterWakeup) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	LinIf_SchHandleType schedule = LINIF_SCH_CNT - 1;
	setLinSMStatus(LINSM_INIT);
	setLinSMNetworkStatus(LINSM_NO_COM);

	LinSM_WakeUpConfirmation(network, TRUE);
	EXPECT_EQ(LINSM_FULL_COM, LinSMStatus);
}

TEST(LinSM_WakeUpConfirmation_Test, ComM_BusSM_ModeIndication_AfterWakeup) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	LinIf_SchHandleType schedule = LINIF_SCH_CNT - 1;
	setLinSMStatus(LINSM_INIT);
	setLinSMNetworkStatus(LINSM_NO_COM);

	LinSM_WakeUpConfirmation(network, TRUE);
	EXPECT_EQ(network, Channel_mock);
	EXPECT_EQ(COMM_FULL_COMMUNICATION, ComMode_mock);
}


TEST(LinSM_WakeUpConfirmation_Test, Set_LinSMNetworkStatus_LINSM_RUN_COMMUNICATION_AfterWakeup) {
	NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	LinIf_SchHandleType schedule = LINIF_SCH_CNT - 1;
	setLinSMStatus(LINSM_INIT);
	setLinSMNetworkStatus(LINSM_NO_COM);

	LinSM_WakeUpConfirmation(network, TRUE);
	EXPECT_EQ(LINSM_RUN_COMMUNICATION, LinSMNetworkStatus[network]);
}

/////////////////////////////////////////////////////////////////
//Main
/////////////////////////////////////////////////////////////////

GTEST_API_ int main (int argc, char **argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
