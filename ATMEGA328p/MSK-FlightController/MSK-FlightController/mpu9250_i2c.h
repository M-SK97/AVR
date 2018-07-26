#ifndef mpu9250_i2c
#define mpu9250_i2c

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "USART.h"
#include "I2C.h"
#include "micros.h"

//Magnetometer Registers
#define AK8963_ADDRESS   0x0C
#define WHO_AM_I_AK8963  0x00 // should return 0x48
#define INFO             0x01
#define AK8963_ST1       0x02  // data ready status bit 0
#define AK8963_XOUT_L    0x03  // data
#define AK8963_XOUT_H    0x04
#define AK8963_YOUT_L    0x05
#define AK8963_YOUT_H    0x06
#define AK8963_ZOUT_L    0x07
#define AK8963_ZOUT_H    0x08
#define AK8963_ST2       0x09  // Data overflow bit 3 and data read error status bit 2
#define AK8963_CNTL      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_ASTC      0x0C  // Self test control
#define AK8963_I2CDIS    0x0F  // I2C disable
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value

#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02

/*#define X_FINE_GAIN      0x03 // [7:0] fine gain
#define Y_FINE_GAIN      0x04
#define Z_FINE_GAIN      0x05
#define XA_OFFSET_H      0x06 // User-defined trim values for accelerometer
#define XA_OFFSET_L_TC   0x07
#define YA_OFFSET_H      0x08
#define YA_OFFSET_L_TC   0x09
#define ZA_OFFSET_H      0x0A
#define ZA_OFFSET_L_TC   0x0B */

#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F

#define SELF_TEST_A       0x10

#define XG_OFFSET_H       0x13  // User-defined trim values for gyroscope
#define XG_OFFSET_L       0x14
#define YG_OFFSET_H       0x15
#define YG_OFFSET_L       0x16
#define ZG_OFFSET_H       0x17
#define ZG_OFFSET_L       0x18
#define SMPLRT_DIV        0x19
#define CONFIG            0x1A
#define GYRO_CONFIG       0x1B
#define ACCEL_CONFIG      0x1C
#define ACCEL_CONFIG2     0x1D
#define LP_ACCEL_ODR      0x1E
#define WOM_THR           0x1F

// Duration counter threshold for motion interrupt generation, 1 kHz rate,
// LSB = 1 ms
#define MOT_DUR           0x20
// Zero-motion detection threshold bits [7:0]
#define ZMOT_THR          0x21
// Duration counter threshold for zero motion interrupt generation, 16 Hz rate,
// LSB = 64 ms
#define ZRMOT_DUR         0x22

#define FIFO_EN            0x23
#define I2C_MST_CTRL       0x24
#define I2C_SLV0_ADDR      0x25
#define I2C_SLV0_REG       0x26
#define I2C_SLV0_CTRL      0x27
#define I2C_SLV1_ADDR      0x28
#define I2C_SLV1_REG       0x29
#define I2C_SLV1_CTRL      0x2A
#define I2C_SLV2_ADDR      0x2B
#define I2C_SLV2_REG       0x2C
#define I2C_SLV2_CTRL      0x2D
#define I2C_SLV3_ADDR      0x2E
#define I2C_SLV3_REG       0x2F
#define I2C_SLV3_CTRL      0x30
#define I2C_SLV4_ADDR      0x31
#define I2C_SLV4_REG       0x32
#define I2C_SLV4_DO        0x33
#define I2C_SLV4_CTRL      0x34
#define I2C_SLV4_DI        0x35
#define I2C_MST_STATUS     0x36
#define INT_PIN_CFG        0x37
#define INT_ENABLE         0x38
#define DMP_INT_STATUS     0x39  // Check DMP interrupt
#define INT_STATUS         0x3A
#define ACCEL_XOUT_H       0x3B
#define ACCEL_XOUT_L       0x3C
#define ACCEL_YOUT_H       0x3D
#define ACCEL_YOUT_L       0x3E
#define ACCEL_ZOUT_H       0x3F
#define ACCEL_ZOUT_L       0x40
#define TEMP_OUT_H         0x41
#define TEMP_OUT_L         0x42
#define GYRO_XOUT_H        0x43
#define GYRO_XOUT_L        0x44
#define GYRO_YOUT_H        0x45
#define GYRO_YOUT_L        0x46
#define GYRO_ZOUT_H        0x47
#define GYRO_ZOUT_L        0x48
#define EXT_SENS_DATA_00   0x49
#define EXT_SENS_DATA_01   0x4A
#define EXT_SENS_DATA_02   0x4B
#define EXT_SENS_DATA_03   0x4C
#define EXT_SENS_DATA_04   0x4D
#define EXT_SENS_DATA_05   0x4E
#define EXT_SENS_DATA_06   0x4F
#define EXT_SENS_DATA_07   0x50
#define EXT_SENS_DATA_08   0x51
#define EXT_SENS_DATA_09   0x52
#define EXT_SENS_DATA_10   0x53
#define EXT_SENS_DATA_11   0x54
#define EXT_SENS_DATA_12   0x55
#define EXT_SENS_DATA_13   0x56
#define EXT_SENS_DATA_14   0x57
#define EXT_SENS_DATA_15   0x58
#define EXT_SENS_DATA_16   0x59
#define EXT_SENS_DATA_17   0x5A
#define EXT_SENS_DATA_18   0x5B
#define EXT_SENS_DATA_19   0x5C
#define EXT_SENS_DATA_20   0x5D
#define EXT_SENS_DATA_21   0x5E
#define EXT_SENS_DATA_22   0x5F
#define EXT_SENS_DATA_23   0x60
#define MOT_DETECT_STATUS  0x61
#define I2C_SLV0_DO        0x63
#define I2C_SLV1_DO        0x64
#define I2C_SLV2_DO        0x65
#define I2C_SLV3_DO        0x66
#define I2C_MST_DELAY_CTRL 0x67
#define SIGNAL_PATH_RESET  0x68
#define MOT_DETECT_CTRL    0x69
#define USER_CTRL          0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define PWR_MGMT_1         0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2         0x6C
#define DMP_BANK           0x6D  // Activates a specific bank in the DMP
#define DMP_RW_PNT         0x6E  // Set read/write pointer to a specific start address in specified DMP bank
#define DMP_REG            0x6F  // Register in DMP from which to read or to which to write
#define DMP_REG_1          0x70
#define DMP_REG_2          0x71
#define FIFO_COUNTH        0x72
#define FIFO_COUNTL        0x73
#define FIFO_R_W           0x74
#define WHO_AM_I_MPU9250   0x75 // Should return 0x71
#define XA_OFFSET_H        0x77
#define XA_OFFSET_L        0x78
#define YA_OFFSET_H        0x7A
#define YA_OFFSET_L        0x7B
#define ZA_OFFSET_H        0x7D
#define ZA_OFFSET_L        0x7E

char H,L;

char OnTimeRunCounterMPU = 1;

long AccX;
long AccY;
long AccZ;

long GyroX;
long GyroY;
long GyroZ;

int aTrimX;
int aTrimY;
int aTrimZ;

/*double AngleX = 0;
double AngleY = 0;*/

long milliseconds = 0;
long  PrevTime;
double dt;
long Time = 0;
double Freq;


long HLbytesConvert16(uint8_t H , uint8_t L)
{
	return (H<<8) | (L);
}

long mpuRead16bit(uint8_t RegA)
{
	i2cStart();
	i2cSend(0b11010000);
	i2cSend(RegA);
	i2cStart();
	i2cSend(0b11010001);
	H = i2cReadAck();
	L = i2cReadNoAck();
	i2cStop();
	
	return HLbytesConvert16(H,L);
}

uint8_t mpuRead8bit(uint8_t RegA)
{
	i2cStart();
	i2cSend(0b11010000);
	i2cSend(RegA);
	i2cStart();
	i2cSend(0b11010001);
	L = i2cReadNoAck();
	i2cStop();
	
	return L;
}

void mpuWrite(uint8_t RegA , uint8_t Data)
{
	i2cStart();
	i2cSend(0b11010001);
	i2cSend(RegA);
	i2cSend(Data);
	i2cStop();
}

double *ComplementraryFilterMPU()
{
	static double Angle[4];
	
	//Time interval
		PrevTime = Time;
		Time = micros();
		dt = (Time - PrevTime)/1000000.0;
		//Freq = 1/dt;
		
		AccX = mpuRead16bit(ACCEL_XOUT_H)+aTrimX;
		AccY = mpuRead16bit(ACCEL_YOUT_H)+aTrimY;
		AccZ = mpuRead16bit(ACCEL_ZOUT_H)+aTrimZ;
		
		GyroX = mpuRead16bit(GYRO_XOUT_H);
		GyroY = mpuRead16bit(GYRO_YOUT_H);
		GyroZ = mpuRead16bit(GYRO_ZOUT_H);
		//Angle x and y form accelerometer.
		Angle[2] = atan(AccY/sqrt(square(AccX) + square(AccZ)))*180/3.14;
		Angle[3] = -atan(AccX/sqrt(square(AccY) + square(AccZ)))*180/3.14;
		
		//Setting initial angle from the accelerometer.
		if(OnTimeRunCounterMPU == 1)
		{
			Angle[0] = Angle[2];
			Angle[1] = Angle[3];
			OnTimeRunCounterMPU = 0;
		}
		
		//Filter
		Angle[0] = 0.98*(Angle[0] + GyroX/131*dt) + 0.02*(Angle[2]);
		Angle[1] = 0.98*(Angle[1] + GyroY/131*dt) + 0.02*(Angle[3]);
		
		
		return Angle;
}

double *ComplementraryFilterAllAnglesMPU()
{
	static double Angle[6];
	
	//Time interval
		PrevTime = Time;
		Time = micros();
		dt = (Time - PrevTime)/1000000.0;
		Freq = 1/dt;
		
		AccX = mpuRead16bit(ACCEL_XOUT_H)+aTrimX;
		AccY = mpuRead16bit(ACCEL_YOUT_H)+aTrimY;
		AccZ = mpuRead16bit(ACCEL_ZOUT_H)+aTrimZ;
		
		GyroX = mpuRead16bit(GYRO_XOUT_H);
		GyroY = mpuRead16bit(GYRO_YOUT_H);
		GyroZ = mpuRead16bit(GYRO_ZOUT_H);
		//Angle x and y form accelerometer.
		Angle[2] = atan(AccY/sqrt(square(AccX) + square(AccZ)))*180/3.14;
		Angle[3] = -atan(AccX/sqrt(square(AccY) + square(AccZ)))*180/3.14;
		
		//Setting initial angle from the accelerometer.
		if(OnTimeRunCounterMPU == 1)
		{
			Angle[0] = Angle[2];
			Angle[1] = Angle[3];
			OnTimeRunCounterMPU = 0;
		}
		/*else
		{
			AngleXG = AngleX + (GyroX/131*dt);
			AngleYG = AngleY + (GyroY/131*dt);
		}*/
		
		//Angle x and y form gyroscope.
		Angle[4] = Angle[4] + (GyroX/131*dt);
		Angle[5] = Angle[5] + (GyroY/131*dt);
		
		//Filter
		Angle[0] = 0.98*(Angle[0] + GyroX/131*dt) + 0.02*(Angle[2]);
		Angle[1] = 0.98*(Angle[1] + GyroY/131*dt) + 0.02*(Angle[3]);
		
		char cAngleX[6];
		dtostrf(Angle[0],3,1,cAngleX);
		
		char cAngleY[6];
		dtostrf(Angle[1],3,1,cAngleY);
		
		char cAngleXA[6];
		dtostrf(Angle[2],3,1,cAngleXA);
		
		char cAngleYA[6];
		dtostrf(Angle[3],3,1,cAngleYA);
		
		char cAngleXG[6];
		dtostrf(Angle[4],3,1,cAngleXG);
		
		char cAngleYG[6];
		dtostrf(Angle[5],3,1,cAngleYG);
		
		char cFreq[6];
		dtostrf(Freq,3,1,cFreq);
		
		char Time[3];
		dtostrf(milliseconds/1000.0,2,2,Time);
		
		printString(cAngleX);
		printString("\t");
		printString(cAngleY);
		printString("\t");
		printString(cAngleXA);
		printString("\t");
		printString(cAngleYA);
		printString("\t");
		printString(cAngleXG);
		printString("\t");
		printString(cAngleYG);
		printString("\t");
		printString(cFreq);
		printString("\t");
		printString(Time);
		printString("\n");
		
		
		return Angle;
}

void MPUtimerInit() //Uses Timer1 so add ISR(TIMER1_COMPA_vect)
										/*{
											milliseconds += 1;
										}*/
{
	//Setup the timer
	TCCR1B |= 1<<CS10 | 1<<WGM12;
	TIMSK1 |= 1<<OCIE1A;
	OCR1A = 16000;
}

void MPUInit()
{
	// Raw Readings
	AccX = mpuRead16bit(ACCEL_XOUT_H);
	AccY = mpuRead16bit(ACCEL_YOUT_H);
	AccZ = mpuRead16bit(ACCEL_ZOUT_H);
	
	GyroX = mpuRead16bit(GYRO_XOUT_H);
	GyroY = mpuRead16bit(GYRO_YOUT_H);
	GyroZ = mpuRead16bit(GYRO_ZOUT_H);
	//Trim
	
	aTrimX = -AccX;
	aTrimY = -AccY;
	aTrimZ = -(AccZ-16384);
}
#endif