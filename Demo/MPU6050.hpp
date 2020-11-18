#ifndef MPU6050_HPP
#define MPU6050_HPP
#include "hwlib.hpp"

#define byte uint8_t
using byte_array = byte[8];

class MPU6050{
private:
	hwlib::i2c_bus_bit_banged_scl_sda i2c_bus;
	using byte_array = byte[8];
	
	const byte arraySize8 = 8;
	
	byte deviceAddress;
	
	//Raw values
	int16_t accelX, accelY, accelZ;
	int16_t gyroX, gyroY, gyroZ;
	int16_t temperature;
	int16_t temperatureInCelcius;
	
	//Offset
	uint16_t offsetAccelX, offsetAccelY, offsetAccelZ;
	uint16_t offsetGyroX, offsetGyroY, offsetGyroZ;
	
	//Readable values
	double gForceX, gForceY, gForceZ;
	double rotX, rotY, rotZ;
	
	byte fullScaleAccelRange;
	byte fullScaleGyroRange;
	
	double gyroSensitivity;
	double accelSensitivity;
	
	//MPU6050 Registers
	//MPU addresses
	const byte MPU6050_ADDRESS_LOW 	= 0x68;
	const byte MPU6050_ADDRESS_HIGH	= 0x69;
	
	//Gyro scale
	const byte MPU6050_GYRO_FS_250	= 0;
	const byte MPU6050_GYRO_FS_500	= 1;
	const byte MPU6050_GYRO_FS_1000	= 2;
	const byte MPU6050_GYRO_FS_2000	= 3;
	
	//Accelerometer scale
	const byte MPU6050_ACCEL_FS_2	= 0;
	const byte MPU6050_ACCEL_FS_4	= 1;
	const byte MPU6050_ACCEL_FS_8	= 2;
	const byte MPU6050_ACCEL_FS_16	= 3;
	
	//Gyroscope and accelerometer tests
	const byte MPU6050_SELF_TEST_X	= 0x0D;
	const byte MPU6050_SELF_TEST_Y	= 0x0E;
	const byte MPU6050_SELF_TEST_Z	= 0x0F;
	const byte MPU6050_SELF_TEST_A	= 0x10;
	
	//Sample rate
	const byte MPU6050_SMPLRT_DIV	= 0x19;
	
	//Configurations
	const byte MPU6050_CONFIG		= 0x1A;
	const byte MPU6050_GYRO_CONFIG 	= 0x1B;
	const byte MPU6050_ACCEL_CONFIG	= 0x1C;
	
	//FIFO buffer
	const byte MPU6050_FIFO_EN		= 0x23;
	
	//Master and slaves
	const byte MPU6050_I2C_MST_CTRL		= 0x24;
	const byte MPU6050_I2C_SLV0_ADDR	= 0x25;
	const byte MPU6050_I2C_SLV0_REG		= 0x26;
	const byte MPU6050_I2C_SLV0_CTRL	= 0x27;
	const byte MPU6050_I2C_SLV1_ADDR	= 0x28;
	const byte MPU6050_I2C_SLV1_REG		= 0x29;
	const byte MPU6050_I2C_SLV1_CTRL	= 0x2A;
	const byte MPU6050_I2C_SLV2_ADDR	= 0x2B;
	const byte MPU6050_I2C_SLV2_REG		= 0x2C;
	const byte MPU6050_I2C_SLV2_CTRL	= 0x2D;
	const byte MPU6050_I2C_SLV3_ADDR	= 0x2E;
	const byte MPU6050_I2C_SLV3_REG		= 0x2F;
	const byte MPU6050_I2C_SLV3_CTRL	= 0x30;
	const byte MPU6050_I2C_SLV4_ADDR	= 0x31;
	const byte MPU6050_I2C_SLV4_REG		= 0x32;
	const byte MPU6050_I2C_SLV4_DO		= 0x33;
	const byte MPU6050_I2C_SLV4_CTRL	= 0x34;
	const byte MPU6050_I2C_SLV4_DI		= 0x35;
	const byte MPU6050_I2C_MST_STATUS	= 0x36;
	
	//Interrupt signals
	const byte MPU6050_INT_PIN_CFG	= 0x37;
	const byte MPU6050_INT_ENABLE	= 0x38;
	const byte MPU6050_INT_STATUS	= 0x3A;
	
	//Accelerometer outputs
	const byte MPU6050_ACCEL_XOUT_H	= 0x3B;
	const byte MPU6050_ACCEL_XOUT_L	= 0x3C;
	const byte MPU6050_ACCEL_YOUT_H	= 0x3D;
	const byte MPU6050_ACCEL_YOUT_L	= 0x3E;
	const byte MPU6050_ACCEL_ZOUT_H	= 0x3F;
	const byte MPU6050_ACCEL_ZOUT_L	= 0x40;
	
	//Temperature outputs
	const byte MPU6050_TEMP_OUT_H	= 0x41;
	const byte MPU6050_TEMP_OUT_L	= 0x42;
	
	//Gyroscope outputs
	const byte MPU6050_GYRO_XOUT_H	= 0x43;
	const byte MPU6050_GYRO_XOUT_L	= 0x44;
	const byte MPU6050_GYRO_YOUT_H	= 0x45;
	const byte MPU6050_GYRO_YOUT_L	= 0x46;
	const byte MPU6050_GYRO_ZOUT_H	= 0x47;
	const byte MPU6050_GYRO_ZOUT_L	= 0x48;
	
	//Clock
	const byte MPU6050_CLOCK_INTERNAL	= 0x00;
	const byte MPU6050_CLOCK_PLL_XGYRO	= 0x01;
	const byte MPU6050_CLOCK_PLL_YGYRO	= 0x02;
	const byte MPU6050_CLOCK_PLL_ZGYRO	= 0x03;
	const byte MPU6050_CLOCK_PLL_EXT32K	= 0x04;
	const byte MPU6050_CLOCK_PLL_EXT19M	= 0x05;
	const byte MPU6050_CLOCK_KEEP_RESET	= 0x07;
	
	//Data external sensors
	const byte MPU6050_EXT_SENS_DATA_00	= 0x49;
	const byte MPU6050_EXT_SENS_DATA_01	= 0x4A;
	const byte MPU6050_EXT_SENS_DATA_02	= 0x4B;
	const byte MPU6050_EXT_SENS_DATA_03	= 0x4C;
	const byte MPU6050_EXT_SENS_DATA_04	= 0x4D;
	const byte MPU6050_EXT_SENS_DATA_05	= 0x4E;
	const byte MPU6050_EXT_SENS_DATA_06	= 0x4F;
	const byte MPU6050_EXT_SENS_DATA_07	= 0x50;
	const byte MPU6050_EXT_SENS_DATA_08	= 0x51;
	const byte MPU6050_EXT_SENS_DATA_09	= 0x52;
	const byte MPU6050_EXT_SENS_DATA_10	= 0x53;
	const byte MPU6050_EXT_SENS_DATA_11	= 0x54;
	const byte MPU6050_EXT_SENS_DATA_12	= 0x55;
	const byte MPU6050_EXT_SENS_DATA_13	= 0x56;
	const byte MPU6050_EXT_SENS_DATA_14	= 0x57;
	const byte MPU6050_EXT_SENS_DATA_15	= 0x58;
	const byte MPU6050_EXT_SENS_DATA_16	= 0x59;
	const byte MPU6050_EXT_SENS_DATA_17	= 0x5A;
	const byte MPU6050_EXT_SENS_DATA_18	= 0x5B;
	const byte MPU6050_EXT_SENS_DATA_19	= 0x5C;
	const byte MPU6050_EXT_SENS_DATA_20	= 0x5D;
	const byte MPU6050_EXT_SENS_DATA_21	= 0x5E;
	const byte MPU6050_EXT_SENS_DATA_22	= 0x5F;
	const byte MPU6050_EXT_SENS_DATA_23	= 0x60;
	
	//Slave outputs
	const byte MPU6050_I2C_SLV0_DO	= 0x63;
	const byte MPU6050_I2C_SLV1_DO	= 0x64;
	const byte MPU6050_I2C_SLV2_DO	= 0x65;
	const byte MPU6050_I2C_SLV3_DO	= 0x66;
	
	//Master delay control
	const byte MPU6050_I2C_MST_DELAY_CTRL	= 0x67;
	
	//Reset the analog and ditital path
	const byte MPU6050_SIGNAL_PATH_RESET	= 0x68;
	
	//Enable and disable various settings
	const byte MPU6050_USER_CTRL = 0x6A;
	
	//Power management options
	const byte MPU6050_PWR_MGMT_1 = 0x6B;
	const byte MPU6050_PWR_MGMT_2 = 0x6C;
	
	//Sample counter
	const byte MPU6050_FIFO_COUNT_H	= 0x72;
	const byte MPU6050_FIFO_COUNT_L	= 0x73;
	const byte MPU6050_FIFO_R_W		= 0x74;
	
	//Identity
	const byte MPU6050_WHO_AM_I		= 0x75; 
public:
	MPU6050(hwlib::i2c_bus_bit_banged_scl_sda & i2c_bus, const byte address = 0x68):
		i2c_bus(i2c_bus),
		deviceAddress(address)
	{};
	
	int whoAmI();
	void wakeUp();
	byte read8bit(const byte & address);
	int16_t read16bit(const byte & addressHigh, const byte & addressLow);
	
	void calibrate();
	
	//Temperature
	void setTemp();
	int16_t getTempInCelcius();
	int16_t getTemp();
	
	// Gyro ranges
	void setFullScaleGyroRange(byte range);
	double getFullScaleGyroRange();
	
	// Accel ranges
	void setFullScaleAccelRange(byte range);
	double getFullScaleAccelRange();
	
	void setAccelGyro();
	void setAccel();
	void setAccelX();
	void setAccelY();
	void setAccelZ();
	int16_t getAccelX();
	int16_t getAccelY();
	int16_t getAccelZ();
	
	void setGyro();
	void setGyroX();
	void setGyroY();
	void setGyroZ();
	int16_t getGyroX();
	int16_t getGyroY();
	int16_t getGyroZ();
	
	uint16_t getOffsetAccelX();
	uint16_t getOffsetAccelY();
	uint16_t getOffsetAccelZ();
	
	uint16_t getOffsetGyroX();
	uint16_t getOffsetGyroY();
	uint16_t getOffsetGyroZ();
};

#endif // MPU6050_HPP
