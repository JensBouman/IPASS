// ==========================================================================
//
// File      : menu.hpp
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

#ifndef MENU_HPP
#define MENU_HPP

#include <array>
#include "menuItem.hpp"

namespace lightMenu{
	/// @file

	/// \brief
	/// Menu is the class that contains a menu and the items it contains.
	/// \details
	/// Menu tracks it menu items which in turn have there own unique values.
	/// Menu also keeps track of it's so called parent menu, the parent menu is the menu that is higher in the menu hierarchy.
	/// The last thing menu keeps track of is it's own index in the basemenu which makes it easier to get and set menu's.
template<int t> class menu
{
	
private:
	std::array<menuItem*, t> menuItemArray;
	unsigned int parentMenuPosition = 0;
	unsigned int baseMenuPosition = 0;
	
public:
	/// \brief
	/// default empty constructor
	menu<t>():
		menuItemArray(){
	}
	
	/// \brief
	/// constructor with an by reference array of menuItems
	menu<t>(const std::array<menuItem*, t> & menuItems):
		menuItemArray( menuItems)
	{}
	
	/// \brief
	/// construct with a pointer to the parentMenu
	menu<t>(menu<t> *parentMenu){
		parentMenuPosition = parentMenu->getBaseMenuPosition();
	}
	
	/// \brief
	///constructor with an by reference array of menuItems and a pointer to the parentMenu
	menu<t>(const std::array<menuItem*, t> & menuItems, menu<t> *parentMenu):
		menuItemArray( menuItems )
		{
		parentMenuPosition = parentMenu->getBaseMenuPosition();
		}
	
	/// \brief
	/// Get menuItem by reference
	/// \details
	/// Returns the pointer of a menu item that is selected by the given integer index.
	menuItem* getMenuItemByIndex(int index){
		return menuItemArray[index];
	}
	
	/// \brief
	/// Get parentMenu position
	/// \details
	/// Returns the position of the parentMenuPosition as an unsigned integer.
	unsigned int getParentMenuPosition(){
		return parentMenuPosition;
	}
	
	/// \brief
	/// Set baseMenuPosition using given unsigned integer.
	void setBaseMenuPosition(unsigned int i){
		baseMenuPosition = i;
	}
	
	/// \brief
	/// returns the baseMenuPosition as an unsigned integer.
	unsigned int getBaseMenuPosition(){
		return baseMenuPosition;
	}
	
	/// \brief
	/// Returns the used array size as unsigned integer.
	/// \details
	/// Returns the size of the used positions in the array by checking if a position is not null.
	unsigned int getMenuItemArraySize(){
		unsigned int count = 0;
		for(unsigned int i = 0; i <= menuItemArray.max_size() -1; i++ ){
			if(menuItemArray[i] != NULL){
				count++;
			}
		}
		return count;
	}
	
	/// \brief
	/// Add a menuItem by pointer.
	/// \details
	/// Add a menuItem to the menuItemArray on the first position that is null in the array.
	/// If the array is empty no item is added.
	void addMenuItem(menuItem* item){
		for(unsigned int i = 0; i <= menuItemArray.max_size()-1; i++ ){
			if(menuItemArray[i] == NULL){
				menuItemArray[i] = item;
				break;
			}
		}
	}
	
};
}
#endif // MENU_HPP
