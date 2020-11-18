// ==========================================================================
//
// File      : spiBusLed.cpp
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
#include "spiBusLed.hpp"

namespace max7219 {
	
	void spiBusLed::waitHalfPeriod(){
		hwlib::wait_us( 50/1000 );
	}

	void spiBusLed::openComms(){
		cs.set(0);
	}

	void spiBusLed::closeComms(){
		cs.set(1);
	}

	uint16_t spiBusLed::makeDataArray(const uint8_t & addr, const uint8_t & data){
		uint16_t endData = 0x00;
		endData |= (addr << 8);
		endData |= data;
		return endData;
	}

	unsigned int spiBusLed::countLeds(){
		uint16_t command = spiBusLed::makeDataArray(0x00, 0xFF);
		uint16_t input = 0x00;
		writeReadCommand(&command, &input);
		unsigned int counter = 0;
		while(input != 0x00FF){
			command = spiBusLed::makeDataArray(0x00,0x00);
			++counter;
			writeReadCommand(&command, &input);
			hwlib::wait_ns(200);
		}
		return counter;
	}

	void spiBusLed::writeReadCommand(const uint16_t data_out[], uint16_t data_in[]){
		uint_fast16_t d = 
			( data_out == nullptr )
				? 0 
				: *data_out++;

		for( uint_fast16_t j = 0; j < 16; ++j ){
			mosi.set( ( d & 0x8000 ) != 0 );
			waitHalfPeriod();
			sclk.set( 1 );
			waitHalfPeriod();
			d = d << 1;
			
			if( miso.get() ){
				d |= 0x01;
			}
			sclk.set( 0 );
		}

		if( data_in != nullptr ){
			*data_in++ = d;
		}
	}
}
