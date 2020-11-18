// ==========================================================================
//
// File      : previousMenuMenuItem.hpp
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
#ifndef PREVIOUSMENUMENUITEM_HPP
#define PREVIOUSMENUMENUITEM_HPP
#include "menuItem.hpp"
#include "baseMenu.hpp"

namespace lightMenu{
	/// @file

	/// \brief
	/// previousMenuMenuItem class is a menuItem child which creates a menuItem that points to a parent menu.
	/// \details
	/// Using the guidelines of the menuItem class this class creates an easy way to go to the parent menu
template<int t, int u>
class previousMenuMenuItem : public menuItem
{
	
private:
	baseMenu<t,u> & baseMenuRef;
public:
	/// \brief
	/// Default constructor
	previousMenuMenuItem(hwlib::string<0> & name, baseMenu<t, u> & baseMenuRef):
	menuItem(name),
	baseMenuRef(baseMenuRef)
	{};

	/// \brief
	/// The run function in this instance calls the previous menu function
	void run(){
		baseMenuRef.previousMenu();
	}
};
}
#endif // PREVIOUSMENUMENUITEM_HPP
