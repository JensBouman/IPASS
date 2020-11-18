// ==========================================================================
//
// File      : menuItem.hpp
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

#ifndef MENUITEM_HPP
#define MENUITEM_HPP
#include "hwlib.hpp"
#include <functional>

namespace lightMenu{
	/// @file

	/// \brief
	/// MenuItem contains the name and a virtual function for the child class.
	/// \details
	/// The menuitem class is the bridge between the menu and a child class that has to run when selected.
class menuItem{
	
private:
	hwlib::string<0> name;
public:

	/// \brief
	/// Default constructor
	menuItem(hwlib::string<0> name):
	name( name )
	{}
	
	/// \brief
	/// Returns the name
	/// \details
	/// Returns the name as an hwlib string.
	hwlib::string<0> getName(){
		return name;
	}

	/// \brief
	/// Set name
	/// \details
	/// Set name to given string by clearing the previous and setting the new name.
	void setName(hwlib::string<0> newName){
		hwlib::cout << "string change" << '\n';
		name.clear() << newName;
	}

	/// \brief
	/// The virtual void function the child class needs to have.
	/// \details
	/// This virtual function is called by the baseMenu when an item is selected and executes the function in the child class.
	virtual void run() = 0;

};
}
#endif // MENUITEM_HPP
