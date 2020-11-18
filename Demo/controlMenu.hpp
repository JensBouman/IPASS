// ==========================================================================
//
// File      : controlMenu.hpp
// Part of   : C++ light menu, easy to use menu library
// Author    : Jasper Oosterbroek
// Github    : https://github.com/JasperOosterbroek/light-menu
// Copyright : jasper.oosterbroek@student.hu.nl 2018
//
//
//Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


#ifndef CONTROLMENU_HPP
#define CONTROLMENU_HPP

#include "hwlib.hpp"
#include "baseMenu.hpp"

namespace lightMenu{
	/// @file

	/// \brief
	/// The controlMenu class provides a link between the by the user chosen controller and the menu.
	/// \details
	/// The controlMenu class provides a link between the by the user chosen controller and the menu, it displays all the possible functions a controller can do.
template<int t, int u>
class controlMenu
{

protected:
	baseMenu<t, u> & baseMenuRef;

public:
	/// \brief
	/// default constructor
	controlMenu<t, u>(baseMenu<t, u> & baseMenuRef):
	baseMenuRef(baseMenuRef)
	{}

	/// \brief
	/// Executes the cursorUp function in the baseMenu class.
	void cursorUp(){
		baseMenuRef.cursorUp();
	}
	
	/// \brief
	/// Executes the cursorDown function in the baseMenu class.
	void cursorDown(){
		baseMenuRef.cursorDown();
	}
	
	/// \brief
	/// Executes the select function in the baseMenu class.
	void select(){
		baseMenuRef.select();
	}
	
	/// \brief
	/// Executes the previousMenu function in the baseMenu class.
	void previousMenu(){
		baseMenuRef.previousMenu();
	}
	
	/// \brief
	/// executes the previousMenu function in the basemenu class with unsigned integer.
	void previousMenu(unsigned int i){
		baseMenuRef.previousMenu(i);
	}
};
}
#endif // CONTROLMENU_HPP