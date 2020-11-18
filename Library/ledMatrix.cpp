// ==========================================================================
//
// File      : ledMatrix.cpp
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
#include "ledMatrix.hpp"

namespace max7219{
	

max7219::ledMatrix::ledMatrix(){
	initializeLatchedValue();
}

uint16_t max7219::ledMatrix::getLatchedValue(const unsigned int & i){
	if(i > 15){
		return 0x00;
	}
	return latchedValue[i];
}

void max7219::ledMatrix::latchRegister(){
	if((temporaryValue>>8) >= 0 && (temporaryValue>>8) <= 16){
		latchedValue[temporaryValue>>8] = temporaryValue;
	}
}

void max7219::ledMatrix::printRegisters(){
	for(int i = 0; i < REGISTER_COUNT; i++){
		hwlib::cout << "register " << i << " : ";
		for(int j = 15; j >= 0; --j){
			hwlib::cout << int((latchedValue[i] & (1<<j))>>j);
		}
		hwlib::cout << "\n";
	}
}

uint16_t max7219::ledMatrix::getTempValue(){
	return temporaryValue;
}

void max7219::ledMatrix::setTempValue(const uint16_t & data){
	temporaryValue = data;
}

void max7219::ledMatrix::initializeLatchedValue(){
	for(int i = 0; i < REGISTER_COUNT; i++){
		latchedValue[i] = 0x00;
	}
}

}