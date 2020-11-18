// ==========================================================================
//
// File      : oledMenu.hpp
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
#ifndef OLEDMENU_HPP
#define OLEDMENU_HPP
#include "hwlib.hpp"
#include "lightMenu.hpp"
template<int t, int u>
class oledMenu : public lightMenu::displayMenu<t, u>
{
	
	hwlib::glcd_oled & display;
	int lines;
public:
	oledMenu(lightMenu::baseMenu<t, u> & baseMenu, hwlib::glcd_oled & display, int lines):
	lightMenu::displayMenu<t, u>(baseMenu),
	display(display),
	lines(lines)
	{}

	void draw() override{
		auto font    = hwlib::font_default_8x8();
		auto writeableDisplay = hwlib::window_ostream(display, font );
		writeableDisplay << '\f';
		for(unsigned int i = lightMenu::displayMenu<t,u>::baseMenuRef.getCurrentCursorPos(); i < lines + lightMenu::displayMenu<t,u>::baseMenuRef.getCurrentCursorPos(); i++){
			
			if( i == lightMenu::displayMenu<t,u>::baseMenuRef.getCurrentCursorPos()){
				writeableDisplay << char(62);
			}else{
				writeableDisplay << " ";
			}
			if(i < lightMenu::displayMenu<t,u>::baseMenuRef.getCurrentMenuSize()){
				writeableDisplay << lightMenu::displayMenu<t,u>::baseMenuRef.getCurrentMenu()->getMenuItemByIndex(i)->getName();
			}
			writeableDisplay << '\n';
		}
		writeableDisplay << hwlib::flush;
	}
	
	void clear() override{
		display.clear();
	}

};

#endif // OLEDMENU_HPP
