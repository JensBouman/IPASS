// ==========================================================================
//
// File      : ledMatrix.hpp
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

#ifndef LEDMATRIX_hpp
#define LEDMATRIX_hpp
#include "hwlib.hpp"

namespace max7219
{
	/// \brief
	/// max7219 8x8 led matrix chip interface
	/// \details
	/// This class implements the max7219 chip.
	/// The chip is a serial input/output driver. It containts an 8x8 static 
	/// RAM module in which the registers and contents are stored.
	/// All communication will be executed through SPI. This allows the chip
	/// to be daisy-chained multiple times without adding any wires.
	class ledMatrix {
		public:
			const static uint8_t DATA_BLANK = 0x00;
			const static uint8_t REGISTER_COUNT = 0x10;
			
			/* 		PUBLIC REGISTERS 	*/
			/// \brief
			/// 0x00 = No-op register, used for dumping data when daisy-chaining multiple max7219's
			const static uint8_t ADDR_NO_OP = 0x00;
			/// \brief
			/// 0x01 - 0x08 = collumn registers, these will display the actual leds
			const static uint8_t ADDR_COL_1 = 0x01;
			/// \brief
			/// 0x01 - 0x08 = collumn registers, these will display the actual leds
			const static uint8_t ADDR_COL_2 = 0X02;
			/// \brief
			/// 0x01 - 0x08 = collumn registers, these will display the actual leds
			const static uint8_t ADDR_COL_3 = 0x03;
			/// \brief
			/// 0x01 - 0x08 = collumn registers, these will display the actual leds
			const static uint8_t ADDR_COL_4 = 0x04;
			/// \brief
			/// 0x01 - 0x08 = collumn registers, these will display the actual leds
			const static uint8_t ADDR_COL_5 = 0x05;
			/// \brief
			/// 0x01 - 0x08 = collumn registers, these will display the actual leds
			const static uint8_t ADDR_COL_6 = 0x06;
			/// \brief
			/// 0x01 - 0x08 = collumn registers, these will display the actual leds
			const static uint8_t ADDR_COL_7 = 0x07;
			/// \brief
			/// 0x01 - 0x08 = collumn registers, these will display the actual leds
			const static uint8_t ADDR_COL_8 = 0x08;
			/// \brief
			/// 0x09 = decode register, used for number displays (not implemented)
			const static uint8_t ADDR_DECODE = 0x09;
			/// \brief
			/// 0x0A = intensity register, setting the brightness of the screen
			const static uint8_t ADDR_INTENSITY = 0x0A;
			/// \brief
			/// 0x0B = scan limit reigster, sets the display of collumns
			const static uint8_t ADDR_SCAN_LIMIT = 0x0B;
			/// \brief
			/// 0x0C = shutdown register
			const static uint8_t ADDR_SHUTDOWN = 0x0C;
			/// \brief
			/// 0x0F = display test register, overrides control, but does not alter it
			const static uint8_t ADDR_DISPLAY_TEST = 0x0F;
			
			const static uint8_t INTENSITY_MIN = 0x00;
			const static uint8_t INTENSITY_MAX = 0x0F;
			const static uint8_t SCAN_LIMIT_ALL = 0x07;
			const static uint8_t SHUTDOWN_ON = 0x00;
			const static uint8_t SHUTDOWN_OFF = 0x01;
			const static uint8_t DISPLAY_TEST_ON = 0x01;
			const static uint8_t DISPLAY_TEST_OFF = 0x00;
			
			/// \brief 
			/// Construct a ledMatrix with initialized register values
			/// \details
			/// This constructor sets every internal register value to 0
			ledMatrix();

			/// \brief
			/// Returns register value at register i
			/// \details
			/// if given a value higher than 15, 0x00 will be returned because
			/// no register higher than 15 exists.
			///
			/// The value is obtained from its internal array, addressed at i
			uint16_t getLatchedValue(const unsigned int & i);

			/// \brief
			/// Sets the temporary value into the permanent register
			/// \details
			/// The temporary values are stored according to the datasheet,
			/// meaning that the first 8 bytes (of which 4 are used) are used to
			/// store the address. That address is obtained by shifting the temporary value 8 to the right.
			/// The remaining 8 bytes are the value.
			/// The whole value will be stored to prevent giving meaning to 
			/// array indexes.
			///
			/// This function does not reset current temporary value.
			void latchRegister();

			/// \brief
			/// Prints all latched register values to the console
			/// \details
			/// Loops through the latchedvalue array and prints the individual
			/// values in binary to the console.
			void printRegisters();

			/// \brief
			/// Returns the currently set temporary value.
			uint16_t getTempValue();

			/// \brief
			/// Sets the current temporary value to given data.
			/// Does not latch previous set temporary value.
			void setTempValue(const uint16_t & data);
			
		private:
			uint16_t latchedValue[REGISTER_COUNT];
			uint16_t temporaryValue = 0x00;
			

			/// \brief
			/// Sets all contents of the latchedValue array to 0
			/// \details
			/// The internal values are all set to 0. When the ledMatrixSet
			/// resetregister method is called, this will set the latchedvalues
			/// to the correct values
			void initializeLatchedValue();
	};
}

#endif // LEDMATRIX_hpp
