/*
 * msp.c
 *
 * Created: 26.02.2013 12:55:12
 *  Author: OliverS
 *
 * $Id$
 */ 

// implements MultiWii Serial Protocol.
#include "global.h"
#include "serial.h"
#include "sensors.h"
#include "mixer.h"
#include "rx.h"
#include "imu.h"

/*
#define INBUF_SIZE 64
static uint8_t inBuf[INBUF_SIZE];
*/

// Multiwii Serial Protocol 0
#define MSP_VERSION				 0

#define MSP_IDENT                100   //out message         multitype + multiwii version + protocol version + capability variable
#define MSP_STATUS               101   //out message         cycletime & errors_count & sensor present & box activation
#define MSP_RAW_IMU              102   //out message         9 DOF
#define MSP_SERVO                103   //out message         8 servos
#define MSP_MOTOR                104   //out message         8 motors
#define MSP_RC                   105   //out message         8 rc chan
#define MSP_RAW_GPS              106   //out message         fix, numsat, lat, lon, alt, speed
#define MSP_COMP_GPS             107   //out message         distance home, direction home
#define MSP_ATTITUDE             108   //out message         2 angles 1 heading
#define MSP_ALTITUDE             109   //out message         1 altitude
#define MSP_BAT                  110   //out message         vbat, powermetersum
#define MSP_RC_TUNING            111   //out message         rc rate, rc expo, rollpitch rate, yaw rate, dyn throttle PID
#define MSP_PID                  112   //out message         up to 16 P I D (8 are used)
#define MSP_BOX                  113   //out message         up to 16 checkbox (11 are used)
#define MSP_MISC                 114   //out message         powermeter trig + 8 free for future use
#define MSP_MOTOR_PINS           115   //out message         which pins are in use for motors & servos, for GUI
#define MSP_BOXNAMES             116   //out message         the aux switch names
#define MSP_PIDNAMES             117   //out message         the PID names
#define MSP_WP                   118   //out message         get a WP, WP# is in the payload, returns (WP#, lat, lon, alt, flags) WP#0-home, WP#16-poshold

#define MSP_SET_RAW_RC           200   //in message          8 rc chan
#define MSP_SET_RAW_GPS          201   //in message          fix, numsat, lat, lon, alt, speed
#define MSP_SET_PID              202   //in message          up to 16 P I D (8 are used)
#define MSP_SET_BOX              203   //in message          up to 16 checkbox (11 are used)
#define MSP_SET_RC_TUNING        204   //in message          rc rate, rc expo, rollpitch rate, yaw rate, dyn throttle PID
#define MSP_ACC_CALIBRATION      205   //in message          no param
#define MSP_MAG_CALIBRATION      206   //in message          no param
#define MSP_SET_MISC             207   //in message          powermeter trig + 8 free for future use
#define MSP_RESET_CONF           208   //in message          no param
#define MSP_WP_SET               209   //in message          sets a given WP (WP#,lat, lon, alt, flags)

#define MSP_EEPROM_WRITE         250   //in message          no param

#define MSP_DEBUG                254   //out message         debug1,debug2,debug3,debug4

static uint8_t checksum;
static uint8_t cmdMSP;


static void write8(uint8_t a)
{
	serialWriteChar(a);
	checksum ^= a;
}

static void write16(int16_t a)
{
	write8((a   ) & 0xFF);
	write8((a>>8) & 0xFF);
}

static void write32(uint32_t a)
{
	write8((a    ) & 0xFF);
	write8((a>> 8) & 0xFF);
	write8((a>>16) & 0xFF);
	write8((a>>24) & 0xFF);
}

static void writeHeader(uint8_t error, uint8_t dataLen)
{
	serialWriteChar('$');
	serialWriteChar('M');
	serialWriteChar(error ? '!' : '>');
	checksum = 0;
	write8(dataLen);
	write8(cmdMSP);
}

static void writeResponseHeader(uint8_t dataLen)
{
	writeHeader(0, dataLen);
}
static void writeErrorHeader()
{
	writeHeader(1, 0);
}

static void writeTail()
{
	serialWriteChar(checksum);
}

static void evalutateCmd()
{
	switch(cmdMSP)
	{
		case MSP_IDENT:
			writeResponseHeader(7);
			write8(210);		// version 210
			write8(2);			// 2 = QUADP
			write8(MSP_VERSION);
			write32(0);
			break;
		case MSP_STATUS:
			writeResponseHeader(10);
			write16(State.CycleTime);		// cycle time
			write16(0);			// i2c error count
			write16(1);			// sensors bit field. 1 = ACC, 2 = BARO, 4 = MAG, 8 = GPS, 16 = SONAR
			write32(0);			// don't know yet
			break;
		case MSP_RAW_IMU:
			writeResponseHeader(18);
			for(uint8_t i = 0; i < 3; i++) write16(ACC[i]);
			for(uint8_t i = 0; i < 3; i++) write16(GYRO[i]);
			for(uint8_t i = 0; i < 3; i++) write16(0);		// MAG
			break;
	   case MSP_SERVO:
			writeResponseHeader(16);
			for(uint8_t i = 0; i < 8; i++)
				write16(0);
			break;
		case MSP_MOTOR:
			writeResponseHeader(16);
			for(uint8_t i = 0; i < 8; i++) write16(MIXER[i]);
			break;
		case MSP_RC:
			writeResponseHeader(16);
			for(uint8_t i = 0; i < 8; i++) 
				write16(i < RX_CHANNELS ? RX_raw[i] : 0);
			break;
		case MSP_ATTITUDE:
		   writeResponseHeader(8);
		   write16((ANGLE[ROL] >> 8) * 10);
		   write16((ANGLE[PIT] >> 8) * 10);
		   write16(0);		// heading
		   write16(0);		// headFreeModeHold
		   break;
		case MSP_ACC_CALIBRATION:
			writeResponseHeader(0);
			sensorsCalibrate();
			break;
		default:
			writeErrorHeader();
	}
	writeTail();
}

void mspLoop()
{
	uint8_t c;
	static enum _msp_state
	{
		IDLE,
		HEADER_START,
		HEADER_M,
		HEADER_ARROW,
		HEADER_SIZE,
		HEADER_CMD,
	} state = IDLE;
	
	static uint8_t dataLen;
	
	while(serialAvail())
	{
		c = serialReadChar();
		
		if (state == IDLE)
		{
			if (c == '$')
				state = HEADER_START;
		}
		else if (state == HEADER_START)
		{
			state = c == 'M' ? HEADER_M : IDLE;
		}
		else if (state == HEADER_M)
		{
			state = c == '<' ? HEADER_ARROW : IDLE;
		}
		else if (state == HEADER_ARROW)
		{
			dataLen = c;
			checksum = 0;
			state = HEADER_SIZE;	
		}
		else if (state == HEADER_SIZE)
		{
			cmdMSP = c;
			checksum ^= c;
			state = HEADER_CMD;
		}
		else
		{
			if (dataLen)
			{
				dataLen--;
				checksum ^= c;
			}
			else
			{
				if (checksum == c)
					evalutateCmd();
				state = IDLE;
			}
		}
	}
}