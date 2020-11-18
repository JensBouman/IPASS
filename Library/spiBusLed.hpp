// ==========================================================================
//
// File      : spiBusLed.hpp
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
#ifndef SPIBUSLED_HPP
#define SPIBUSLED_HPP
#include "hwlib.hpp"

namespace max7219 {
			
	/// \brief
	/// An adaption off the hwlib::spi_bus supporting 16bit write_reads
	/// \details
	/// The max7219 chip uses a 16-bit spi protocol (in which the first 4 bits
	/// are not used). This class is a child of the 
	/// hwlib::spi_bus_bit_banged_sclk_mosi_miso class. Instead of overriding their
	/// read_write method this class implements a different method (writeReadCommand)
	class spiBusLed : public hwlib::spi_bus_bit_banged_sclk_mosi_miso {
		private:
			hwlib::pin_out & sclk;
			hwlib::pin_out & mosi;
			hwlib::pin_out & cs;
			hwlib::pin_in & miso;
			
		public:
			/// \brief
			/// Initializes parent and pins
			spiBusLed( hwlib::pin_out & sclk, hwlib::pin_out & mosi, hwlib::pin_out & cs, hwlib::pin_in & miso):
				hwlib::spi_bus_bit_banged_sclk_mosi_miso(sclk, mosi, miso),
				sclk(sclk),
				mosi(mosi),
				cs(cs),
				miso(miso)
			{};
			
			
			/// \brief
			/// Waits 50 nanoseconds
			/// \details
			/// The clock period of the max7219 is 100 nanoseconds, so half-
			/// period is 50ns
			void waitHalfPeriod();
			
			/// \brief
			/// Sets the chipselect pin low
			/// \details
			/// Chipselect on max7219 works inverted, low pin means data can be
			/// communicated.
			void openComms();
			
			/// \brief
			/// Sets the chipselect pin high
			/// \details
			/// Chipselect on max7219 works inverted, high pin means data can
			/// not be communicated.
			void closeComms();
			
			/// \brief
			/// Communicates the given data to the chip, and receives any 
			/// overflow in data_in
			/// \details
			/// Pushes data_out over the mosi line, least significant bit first,
			/// and gathers data_in over the miso line.
			void writeReadCommand(const uint16_t data_out[], uint16_t data_in[]);
			
			/// \brief
			/// Counts how many matrices are connected
			/// \details
			/// Pushes a dummy value over register 0x00, and keeps track of how
			/// many iterations have occured until it receives its output back
			/// on the input.
			/// 
			/// Does not alter the cs line. It goes through the temporary values
			/// of the chip, the value will not be latched.
			unsigned int countLeds();
			
			/// \brief
			/// Static function to build a 16 bit packet from a register and a
			/// address
			/// \details
			/// Pushes the register address 8 bits to the left, and |s the data
			/// resulting in the first 8 bits address, and the last 8 bits as
			/// data
			static uint16_t makeDataArray(const uint8_t & addr, const uint8_t & data);
	};
}

#endif // SPIBUSLED_HPP
