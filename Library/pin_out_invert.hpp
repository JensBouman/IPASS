// ==========================================================================
//
// File      : pin_out_invert.hpp
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

#ifndef PIN_OUT_INVERT_HPP
#define PIN_OUT_INVERT_HPP
#include "hwlib.hpp"

namespace max7219{
	
	/// \brief
	/// Decorator class to invert values for pin_out
	/// \details
	/// The max7219 requires the chipselect to be active-low. This class inverts
	/// the set functionaliy so that it can be used normally.
	class pin_out_invert : public hwlib::pin_out{
		private:
			hwlib::pin_out & slave;
		public:
			/// \brief
			/// Constructs the class with its own pin_out slave
			pin_out_invert(hwlib::pin_out & slave):
				slave(slave)
			{};
			
			/// \brief
			/// Inverts the set functionality of the base set method of pin_out.
			void set(bool x, hwlib::buffering buf = hwlib::buffering::unbuffered) override {
				slave.set(!x, buf);
			}
	};
}

#endif // PIN_OUT_INVERT_HPP
