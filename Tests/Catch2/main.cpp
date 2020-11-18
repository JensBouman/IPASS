#include "hwlib.hpp"
#include "library/max7219.hpp"

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
//This file will be used for the functional tests of the system, not the leds themself.
//This will only guarantee that the system is functioning, not necessarily that 
//the leds are working as intended. For that purpose, refere to the physical tests.


/* ------------- ledMatrixSet tests ------- */
TEST_CASE( "constructor, no parameters; registers" ){
	//testing after initialization that the registers are still 0. they will be set after resetregisters
	auto spi_bus = max7219::spiBusLed(hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_in_dummy);
	max7219::ledMatrixSet<4> matrices(spi_bus);
	
	REQUIRE(matrices.getLedMatrix(1).getLatchedValue(max7219::ledMatrix::ADDR_INTENSITY) == 0x00);   
	REQUIRE(matrices.getLedMatrix(2).getLatchedValue(max7219::ledMatrix::ADDR_INTENSITY) == 0x00);   
	REQUIRE(matrices.getLedMatrix(3).getLatchedValue(max7219::ledMatrix::ADDR_INTENSITY) == 0x00);   
	REQUIRE(matrices.getLedMatrix(4).getLatchedValue(max7219::ledMatrix::ADDR_INTENSITY) == 0x00);   
}

TEST_CASE("cycle delay, get"){
	auto spi_bus = max7219::spiBusLed(hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_in_dummy);
	max7219::ledMatrixSet<4> matrices(spi_bus);
	int_fast32_t delay = matrices.getCycleDelay();
	REQUIRE(delay == 50);
}

TEST_CASE("cycle delay, set"){
	auto spi_bus = max7219::spiBusLed(hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_in_dummy);
	max7219::ledMatrixSet<4> matrices(spi_bus);
	matrices.setCycleDelay(150);
	REQUIRE(matrices.getCycleDelay() == 150);
}

TEST_CASE("cycle delay, set lower limit"){
	auto spi_bus = max7219::spiBusLed(hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_in_dummy);
	max7219::ledMatrixSet<4> matrices(spi_bus);
	matrices.setCycleDelay(-150);
	REQUIRE(matrices.getCycleDelay() == 0);
}

TEST_CASE("cycle delay, set and get"){
	auto spi_bus = max7219::spiBusLed(hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_in_dummy);
	max7219::ledMatrixSet<4> matrices(spi_bus);
	matrices.setCycleDelay(150);
	int_fast32_t delay = matrices.getCycleDelay();
	REQUIRE(delay == 150);
}

TEST_CASE("cycle, get"){
	auto spi_bus = max7219::spiBusLed(hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_in_dummy);
	max7219::ledMatrixSet<4> matrices(spi_bus);
	bool cycle = matrices.getCycle();
	REQUIRE(cycle == true);
}

TEST_CASE("cycle, set"){
	auto spi_bus = max7219::spiBusLed(hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_in_dummy);
	max7219::ledMatrixSet<4> matrices(spi_bus);
	matrices.setCycle(true);
	REQUIRE(matrices.getCycle() == true);
}

TEST_CASE("cycle, get and set"){
	auto spi_bus = max7219::spiBusLed(hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_in_dummy);
	max7219::ledMatrixSet<4> matrices(spi_bus);
	matrices.setCycle(false);
	bool cycle = matrices.getCycle();
	REQUIRE(cycle == false);
}

TEST_CASE("getLedMatrix, temporary value check"){
	auto spi_bus = max7219::spiBusLed(hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_in_dummy);
	max7219::ledMatrixSet<4> matrices(spi_bus);
	max7219::ledMatrix led = matrices.getLedMatrix(2);
	led.setTempValue(0x0112);
	REQUIRE(led.getTempValue() == 0x0112);
}

TEST_CASE("getLedMatrix, latched value check"){
	auto spi_bus = max7219::spiBusLed(hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_in_dummy);
	max7219::ledMatrixSet<4> matrices(spi_bus);
	max7219::ledMatrix led = matrices.getLedMatrix(2);
	led.setTempValue(0x0112); //register 0x01 data 0x12
	led.latchRegister();
	REQUIRE(led.getLatchedValue(0x01) == 0x0112);
}

TEST_CASE("resetRegisters, different registers"){
	auto spi_bus = max7219::spiBusLed(hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_out_dummy, hwlib::pin_in_dummy);
	max7219::ledMatrixSet<4> matrices(spi_bus);
	REQUIRE(matrices.getLedMatrix(1).getLatchedValue(0x0C) == 0x0000);
	matrices.resetRegisters();
	REQUIRE(matrices.getLedMatrix(1).getLatchedValue(0x0C) == 0x0C01);
}


/* ------------- ledMatrix tests ------- */
TEST_CASE("ledMatrix, constructor"){
	max7219::ledMatrix led;
	
	REQUIRE(led.getLatchedValue(0x01) == 0x0000);
	REQUIRE(led.getLatchedValue(0x08) == 0x0000);
}

TEST_CASE("ledMatrix tempvalue, set and get"){
	max7219::ledMatrix led;
	led.setTempValue(0x0112);
	REQUIRE(led.getTempValue() == 0x0112);
}

TEST_CASE("ledMatrix latch register, set and get"){
	max7219::ledMatrix led;
	led.setTempValue(0x0112);
	REQUIRE(led.getLatchedValue(0x01) == 0x0000);
	led.latchRegister();
	REQUIRE(led.getLatchedValue(0x01) == 0x0112);
}

TEST_CASE("ledMatrix latch register, get out of bounds"){
	max7219::ledMatrix led;
	led.setTempValue(0x0112);
	led.latchRegister();
	REQUIRE(led.getLatchedValue(0x88) == 0x00);
}


/* ------------- spiBusLed tests ------- */
TEST_CASE("makeDataArray, general testing"){
	REQUIRE(max7219::spiBusLed::makeDataArray(15, 8) == 0x0F08);
	REQUIRE(max7219::spiBusLed::makeDataArray(32, 0) == 0x2000);
	REQUIRE(max7219::spiBusLed::makeDataArray(34, 99) == 0x2263);
	REQUIRE(max7219::spiBusLed::makeDataArray(0, 1) == 0x0001);
}
