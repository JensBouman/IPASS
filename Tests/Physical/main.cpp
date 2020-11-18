// ==========================================================================
//
// File      : main.cpp
// Part of   : C++ max7219 ledMatrix library
// Author    : Jens Bouman
// Github    : https://github.com/JensBouman/max7219
// Copyright : jens.bouman@student.hu.nl 2018
//
//
//Distributed under the Boost Software License, Version 1.0.
// (See accompanying file Boost V1.0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#include "hwlib.hpp"
#include "library/max7219.hpp"

int main(){

	// kill the watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;

	//This program will test the phyiscal capabilities of the screen. Read each
	//of these tests before running the program to check what is supposed to 
	//appear. This test will assume a 1x4 led matrix module. Change the template
	//size if your module is not 1x4.

	namespace target = hwlib::target;
	
	auto din     = hwlib::target::pin_out(hwlib::target::pins::d51);
	auto cs      = hwlib::target::pin_out(hwlib::target::pins::d47);
	auto clk     = hwlib::target::pin_out(hwlib::target::pins::d45);
	auto csInv   = max7219::pin_out_invert(cs);
	auto spi_bus = max7219::spiBusLed(clk, din, csInv, hwlib::pin_in_dummy);
	max7219::ledMatrixSet<4> matrices(spi_bus);

	matrices.resetRegisters();
	//At this point any previous leds that were on should be off.
	hwlib::wait_ms(1000);
	
	//Leds start simple. (get it?)
	matrices.setLed(1,2,3);
	matrices.setLed(2,3,4);
	matrices.setLed(3,4,5);
	// Now should the following leds should be turned on:
	// Screen 1, collumn 2 the first and second led (bitcode 3)
	// Screen 2, collumn 3 the third led (bitcode 4)
	// Screen 3, collumn 4, the first and the third led (bitcode 5)
	hwlib::wait_ms(2000);
	
	matrices.resetRegisters();
	
	//Now it is time to set leds at coordinates instead of bitcodes
	matrices.setLed(1,2,3, true);
	matrices.setLed(2,3,4, true);
	matrices.setLed(3,8,8, true);
	// Now should the following leds should be turned on:
	// Screen 1, collumn 2 the third led (2,3)
	// Screen 2, collumn 3 the fourth led (3,4)
	// Screen 3, collumn 8,the eighth led (8,8)
	hwlib::wait_ms(2000);
	
	matrices.resetLedAt(1,2,3);
	matrices.resetLedAt(2,3,4);
	matrices.resetLedAt(3,8,8);
	hwlib::wait_ms(500);
	//Now all screens should be blank, as we individually reset each led
	
	//Time to write some characters
	matrices.setLetter(4, '8');
	matrices.setLetter(3, '+');
	//Now there should be the character '8' on the fourth screen (counting from the
	//first screen connected) and a '+' on the third screen (next to the '8')
	hwlib::wait_ms(2000);
	
	matrices.resetRegisters();
	
	char s[] = "Goed";
	matrices.setWord(s, 4);
	//At this point the word Goed should be seen on the leds, spread over the 4
	//matrices.
	hwlib::wait_ms(2000);
	
	//That screen is very bright, lets turn it down a notch
	matrices.setRegister(4, 0x0A, 0x01);
	matrices.setRegister(3, 0x0A, 0x01);
	matrices.setRegister(2, 0x0A, 0x01);
	matrices.setRegister(1, 0x0A, 0x01);
	//Lets also make sure that the registers are latched properly by setting a 
	//falling and rising edge
	matrices.openComms();
	matrices.closeComms();
	//The word "Goed" should be less bright now.
	hwlib::wait_ms(2000);
	
	
	//Time to get dizzy, lets spin the word around
	matrices.cycleSet();
	hwlib::wait_ms(500);
	//Okay, too fast, lets slow it down a bit
	matrices.setCycleDelay(45*1000000);
	matrices.cycleSet();
	hwlib::wait_ms(500);
	//How about just a couple of steps?
	matrices.cycleSteps(12);
	hwlib::wait_ms(500);
	//Okay, i am done with it, time to shift it out of the screen
	matrices.setCycle(false);
	matrices.cycleSteps(32);
	hwlib::wait_ms(500);
	
	//Alright, time for a recap. After we changed the brightness, the following
	//should've happened:
	//1. The word "goed" cycled very fast across the screen.
	//2. The word "goed" cycled much more slowly across the screen.
	//3. The word "goed" cycled for 12 steps (rows) across the screen.
	//4. The text on the screen cycled 32 steps across the screen, without
	//   the overflow being added to the front, eliminating the word from the screen.
	
	//And reset the registers for the next program
	matrices.resetRegisters();
	matrices.setCycle(true);
}