#include "MPU6050.hpp"

int MPU6050::whoAmI(){
	byte data[] = {MPU6050_WHO_AM_I};
	i2c_bus.write(deviceAddress, data, 1);
	i2c_bus.read(deviceAddress, data, 1);
	int whoHeIs = data[0];
	return whoHeIs;
}

void MPU6050::wakeUp(){
	byte data[] = {MPU6050_PWR_MGMT_1, 0};
	i2c_bus.write(deviceAddress, data, 2);
}

byte MPU6050::read8bit(const byte & address){
	byte data[] = {address};
	i2c_bus.write(deviceAddress, data, 1);
	i2c_bus.read(deviceAddress, data, 1);
	return *data;
}

int16_t MPU6050::read16bit(const byte & addressHigh, const byte & addressLow){
	byte dataHigh = read8bit(addressHigh);
	byte dataLow = read8bit(addressLow);
	int16_t conversion = (dataHigh << 8) + dataLow;
	return conversion;
}

void MPU6050::setFullScaleGyroRange(byte gyroFullScaleSelect){
	gyroFullScaleSelect = gyroFullScaleSelect << 3;
	byte data[] = {MPU6050_GYRO_CONFIG, gyroFullScaleSelect};
	i2c_bus.write(deviceAddress, data, 2);
}
double MPU6050::getFullScaleGyroRange(){
	byte data[] = {MPU6050_GYRO_CONFIG};
	i2c_bus.write(deviceAddress, data, 1);
	i2c_bus.read(deviceAddress, data, 1);
	fullScaleGyroRange = (data[0] >> 3) & 0x03;
	if (fullScaleGyroRange == 0) {
		gyroSensitivity = 131.0;
	} else if (fullScaleGyroRange == 1) {
		gyroSensitivity =  65.5;
	} else if (fullScaleGyroRange == 2) {
		gyroSensitivity =  32.8;
	} else if (fullScaleGyroRange == 3) {
		gyroSensitivity =  16.4;
	} else {
		gyroSensitivity = 131.0;
	}
	return gyroSensitivity;
}

void MPU6050::setFullScaleAccelRange(byte accelFullScaleSelect){
	accelFullScaleSelect = accelFullScaleSelect << 3;
	byte data[] = {MPU6050_ACCEL_CONFIG, accelFullScaleSelect};
	i2c_bus.write(deviceAddress, data, 2);
}
double MPU6050::getFullScaleAccelRange(){
	byte data[] = {MPU6050_ACCEL_CONFIG};
	i2c_bus.write(deviceAddress, data, 1);
	i2c_bus.read(deviceAddress, data, 1);
	fullScaleAccelRange = (data[0] >> 3) & 0x03;
	if (fullScaleAccelRange == 0) {
		accelSensitivity = 16384.0;
	} else if (fullScaleAccelRange == 1) {
		accelSensitivity =  8192.0;
	} else if (fullScaleAccelRange == 2) {
		accelSensitivity =  4096.0;
	} else if (fullScaleAccelRange == 3) {
		accelSensitivity =  2048.0;
	} else {
		accelSensitivity = 16384.0;
	}
	return accelSensitivity;
}

void MPU6050::setAccelGyro(){
	setAccel();
	setGyro();
}
void MPU6050::setAccel(){
	setAccelX();
	setAccelY(); 
	setAccelZ();
}
void MPU6050::setAccelX(){
	accelX = read16bit(MPU6050_ACCEL_XOUT_H, MPU6050_ACCEL_XOUT_L);
}
void MPU6050::setAccelY(){
	accelY = read16bit(MPU6050_ACCEL_YOUT_H, MPU6050_ACCEL_YOUT_L);
}
void MPU6050::setAccelZ(){
	accelZ = read16bit(MPU6050_ACCEL_ZOUT_H, MPU6050_ACCEL_ZOUT_L);
}
int16_t MPU6050::getAccelX(){
	return accelX;
}
int16_t MPU6050::getAccelY(){
	return accelY;
}
int16_t MPU6050::getAccelZ(){
	return accelZ;
}

void MPU6050::setGyro(){
	setGyroX();
	setGyroY();
	setGyroZ();
}
void MPU6050::setGyroX(){
	gyroX = read16bit(MPU6050_GYRO_XOUT_H, MPU6050_GYRO_XOUT_L);
}
void MPU6050::setGyroY(){
	gyroY = read16bit(MPU6050_GYRO_YOUT_H, MPU6050_GYRO_YOUT_L);
}
void MPU6050::setGyroZ(){
	gyroZ = read16bit(MPU6050_GYRO_ZOUT_H, MPU6050_GYRO_ZOUT_L);
}
int16_t MPU6050::getGyroX(){
	return gyroX;
}
int16_t MPU6050::getGyroY(){
	return gyroY;
}
int16_t MPU6050::getGyroZ(){
	return gyroZ;
}

void MPU6050::calibrate(){
	hwlib::cout << "Calibration started, keep device still.\n";
	hwlib::wait_ms(250);
	uint32_t deltaAccelX = 0, deltaAccelY = 0, deltaAccelZ = 0;
	uint32_t deltaGyroX = 0, deltaGyroY = 0, deltaGyroZ = 0;
	for(unsigned int i = 0; i < 50; i++){
		setAccelGyro();
		deltaAccelX += getAccelX();
		deltaAccelY += getAccelY();
		deltaAccelZ += getAccelZ();
		
		deltaGyroX += getGyroX();
		deltaGyroY += getGyroY();
		deltaGyroZ += getGyroZ();
		hwlib::wait_ms(50);
	}
	offsetAccelX = deltaAccelX / 50;
	offsetAccelY = deltaAccelY / 50; 
	offsetAccelZ = deltaAccelZ / 50;
	offsetGyroX = deltaGyroX / 50; 
	offsetGyroY = deltaGyroY / 50; 
	offsetGyroZ = deltaGyroZ / 50;
	hwlib::cout << "Calibration complete.\n";
}

uint16_t MPU6050::getOffsetAccelX(){
	return offsetAccelX;
}
uint16_t MPU6050::getOffsetAccelY(){
	return offsetAccelY;
}
uint16_t MPU6050::getOffsetAccelZ(){
	return offsetAccelZ;
}

uint16_t MPU6050::getOffsetGyroX(){
	return offsetGyroX;
}
uint16_t MPU6050::getOffsetGyroY(){
	return offsetGyroY;
}
uint16_t MPU6050::getOffsetGyroZ(){
	return offsetGyroZ;
}

void MPU6050::setTemp() {
	temperature = read16bit(MPU6050_TEMP_OUT_H, MPU6050_TEMP_OUT_L);
	temperatureInCelcius = temperature/340 + 36.53;
}

int16_t MPU6050::getTempInCelcius() {
	return temperatureInCelcius;
}

int16_t MPU6050::getTemp() {
	return temperatureInCelcius;
}