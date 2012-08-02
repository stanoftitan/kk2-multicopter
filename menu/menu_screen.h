/*
 * menu_screen.h
 *
 * Created: 02.08.2012 08:12:11
 *  Author: OliverS
 */ 


#ifndef MENU_SCREEN_H_
#define MENU_SCREEN_H_

screen scrSensorCal[] = {
	{
	"Place the aircraft on",
	"a level surface and",
	"press CONTINUE.",
	"The FC will then wait",
	"5 sec to let the",
	"aircraft settle down."
	},
	{
	"Calibration failed.",
	"",
	"Make sure the air-",
	"craft is level and",
	"stationary, and try",
	"again."
	}	
};

screen scrESCCal[] = {
	{
	"1 TAKE OFF THE",
	"  PROPELLERS!!",
	"2 Check the throttle",
	"  direction in the",
	"  receiver test menu.",
	"  Reverse if",
	"  necessary."
	},
	{
	"3 Memorize the rest",
	"  of the instructions",
	"  because the next",
	"  step is to turn off",
	"  the power.",
	"4 Turn off the power.",
	"5 Turn on the radio."
	},
	{
	"6 Set the throttle to",
	"  full.",
	"7 Press down button",
	"  1 and 4 and keep",
	"  them down until the",
	"  last step.",
	"8 Turn on power."		
	},
	{
	"9 Wait for the ESCs",
	"  full-throttle cali-",
	"  bration sound.",
	"10 Set the throttle",
	"  to idle.",
	"11 Wait for the idle"		
	},
	{
	"  calibration sound.",
	"12 Release the",
	"  buttons.",
	"13 Check if the mot-",
	"  ors start at the",
	"  same time. If not,",
	"  adjust the"		
	},
	{
	"  'Minimum throttle'",
	"  in the 'Misc. Set-",
	"  tings menu.",
	"",
	"  You are now done!",
	},
};

screen scrRadioCal[] = {
	{
	"Release sticks on the",
	"radio and set ",
	"throttle to idle.",
	"",
	"Press CONTINUE to",
	"calibrate."
	}	
};


#endif /* MENU_SCREEN_H_ */