/*
 * menu_screen_t.h
 *
 * Created: 02.08.2012 08:12:11
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef MENU_SCREEN_H_
#define MENU_SCREEN_H_

#ifndef P_STR
#define P_STR static const char PROGMEM
#endif

P_STR scrPIEditor[] = 
	"Axis:\n"
	"\n"
	"P Gain:\n"
	"P Limit:\n"
	"I Gain:\n"
	"I Limit:";

P_STR scrReceiverTest[] = 
	"Aileron:\n"
	"Elevator:\n"
	"Rudder:\n"
	"Throttle:\n"
	"Auxiliary:";

P_STR scrSensorTest[] = 
	"Gyro X:\n"
	"Gyro Y:\n"
	"Gyro Z:\n"
	"Acc  X:\n"
	"Acc  Y:\n"
	"Acc  Z:\n"
	"Batt:";

P_STR scrModeSettings[] = 
	"Self-Level:\n"
	"Arming:\n"
	"Link Roll Pitch:\n"
	"Auto Disarm:\n"
	"CPPM Enabled:";

P_STR scrStickScaling[] = 
	"Stick Scaling\n"
	"\n"
	"Roll (Ail):\n"
	"Pitch (Ele):\n"
	"Yaw (Rud):\n"
	"Throttle:";

P_STR scrMiscSettings[] = 
	"Minimum throttle:\n"
	"Height Dampening:\n"
	"Height D. Limit:\n"
	"Alarm 1/10 volts:\n"
	"Servo filter:";

P_STR scrSelflevelSettings[] =
	"P Gain:\n"
	"P Limit:\n"
	"\n"
	"Acc Trim Roll:\n"
	"Acc Trim Pitch:";

P_STR scrCPPMSettings[] = 
	"Roll (Ail):\n"
	"Pitch (Ele):\n"
	"Yaw (Rud):\n"
	"Throttle:\n"
	"Aux:";
	
P_STR scrSensorCal0[] = 
	"Place the aircraft on\n"
	"a level surface and\n"
	"press CONTINUE.\n"
	"The FC will then wait\n"
	"5 sec to let the\n"
	"aircraft settle down.";
P_STR scrSensorCal1[] = 
	"Calibration failed.\n"
	"\n"
	"Make sure the air-\n"
	"craft is level and\n"
	"stationary, and try\n"
	"again.";

P_STR scrESCCal0[] = 
	"1 TAKE OFF THE\n"
	"  PROPELLERS!!\n"
	"2 Check the throttle\n"
	"  direction in the\n"
	"  receiver test menu.\n"
	"  Reverse if\n"
	"  necessary.";
P_STR scrESCCal1[] = 
	"3 Memorize the rest\n"
	"  of the instructions\n"
	"  because the next\n"
	"  step is to turn off\n"
	"  the power.\n"
	"4 Turn off the power.\n"
	"5 Turn on the radio.";
P_STR scrESCCal2[] = 
	"6 Set the throttle to\n"
	"  full.\n"
	"7 Press down buttons\n"
	"  1 and 4 and keep\n"
	"  them down until the\n"
	"  last step.\n"
	"8 Turn on power.";	
P_STR scrESCCal3[] = 
	"9 Wait for the ESCs\n"
	"  full-throttle cali-\n"
	"  bration sound.\n"
	"10 Set the throttle\n"
	"  to idle.\n"
	"11 Wait for the idle";		
P_STR scrESCCal4[] = 
	"  calibration sound.\n"
	"12 Release the\n"
	"  buttons.\n"
	"13 Check if the mot-\n"
	"  ors start at the\n"
	"  same time. If not,\n"
	"  adjust the";	
P_STR scrESCCal5[] = 
	"  'Minimum throttle'\n"
	"  in the 'Misc. Set-\n"
	"  tings menu.\n"
	"\n"
	"  You are now done!";

static const char* const scrESCCal[] PROGMEM = 
	{
		scrESCCal0,
		scrESCCal1,
		scrESCCal2,
		scrESCCal3,
		scrESCCal4,
		scrESCCal5,
	};

P_STR scrRadioCal0[] = 
	"Release sticks on the\n"
	"transmitter and set \n"
	"throttle to idle.\n"
	"\n"
	"Press CONTINUE to\n"
	"calibrate.";

P_STR scrRadioCal1[] = 
	"\n"
	"Check the correct\n"
	"signals from\n"
	"receiver.\n"
	"One or more signals\n"
	"are missing.";

P_STR scrRadioCal2[] = 
	"Calibration failed.\n"
	"\n"
	"Make sure the sticks\n"
	"are released and\n"
	"throttle is idle.";

P_STR scrMixerEditor[] = 
	"Throttle:       Ch:\n"
	"Aileron:\n"
	"Elevator:\n"
	"Rudder:\n"
	"Offset:\n"
	"Type:       Rate:";

P_STR scrDebug[] =
	"MixerIndex:\n"
	"Channel0:\n"
	"Channel1:\n"
	"Channel2:\n"
	"Channel3:";


#endif /* MENU_SCREEN_H_ */