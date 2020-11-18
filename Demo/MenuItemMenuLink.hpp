// ==========================================================================
//
// File      : menuItemMenuLink.hpp
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


#ifndef MENUITEMMENULINK_HPP
#define MENUITEMMENULINK_HPP
#include "hwlib.hpp"
#include "baseMenu.hpp" 
#include "menu.hpp"
#include "menuItem.hpp"

namespace lightMenu{
	/// @file

	/// \brief
	/// menuItemMenuLink class is a menuItem child which creates a menuItem that points to other menu's.
	/// \details
	/// Using the guidelines of the menuItem class this class creates an easy way to make a link to other menus using the menu refrence.
template <int t, int u>
class menuItemMenuLink : public menuItem
{
private: 
	menu<u> & menulink;
	baseMenu<t,u> & baseMenuRef;
	public:
	/// \brief
	/// Default constructor
	menuItemMenuLink(hwlib::string<0> & name, menu<u> & menulink, baseMenu<t, u> & baseMenuRef):
	menuItem(name),
	menulink(menulink),
	baseMenuRef(baseMenuRef)
	{};
	/// \brief
	/// The run function in this instance sets the menulink as the currentmenu and resets the cursor position.
	void run(){
		baseMenuRef.setMenu(&menulink);
		baseMenuRef.setCursorPos(0);
	}

};
}
#endif // MENUITEMMENULINK_HPP
