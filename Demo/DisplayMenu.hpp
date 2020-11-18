// ==========================================================================
//
// File      : displayMenu.hpp
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


#ifndef DISPLAYMENU_HPP
#define DISPLAYMENU_HPP

#include "baseMenu.hpp"

namespace lightMenu{
	/// @file

	/// \brief
	/// The displayMenu is the abstract class that the user can extend to display the menu on the preffered display
	/// \details
	/// The displayMenu is the abstract class that sets some guidelines for the making of a display specific display driver.
template<int t, int u>
class displayMenu
{
protected:
	baseMenu<t, u> & baseMenuRef;
	public:
	/// \brief
	/// default constructor
	displayMenu<t, u>(baseMenu<t, u> & baseMenuRef):
	baseMenuRef(baseMenuRef)
	{}

	/// \brief
	/// A virtual class to profide guidelines for the child class
	virtual void draw() = 0;
	
	/// \brief
	/// A virtual class to profide guidelines for the child class
	virtual void clear() = 0;
};
}
#endif // DISPLAYMENU_HPP
