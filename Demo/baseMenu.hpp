// ==========================================================================
//
// File      : baseMenu.hpp
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

#ifndef BASEMENU_HPP
#define BASEMENU_HPP

#include <array>
#include "menu.hpp"

namespace lightMenu{
	
	/// @file

	/// \brief
	/// BaseMenu is the reference point to all the menu data.
	/// \details
	/// BaseMenu is the brains behind the lightMenu library, baseMenu holds all the important functions and data to fully take control of the menus.
	/// Since baseMenu saves Menu's it makes multidimensional menu's possible.
	///
	/// Basemenu is a templated class because the heap is not usable and the used array's are of variable size wich is set at compile time.
template<int t, int u> class baseMenu {

	private:
		std::array<menu<u>*, t> & menus;
		unsigned int currentMenuIndex = 0;
		unsigned int cursorPosition = 0;

	public:
		
		/// \brief
		/// Construct a baseMenu filled with an array of menu's
		baseMenu<t,u>(std::array<menu<u>*, t> & menus):
			menus (menus)
		{
			for(unsigned int i = 0; i < menus.size(); i++){
				if(menus[i] != NULL){
					menus[i]->setBaseMenuPosition(i);
				}
			}
		}
		
		/// \brief
		/// returns a menu from an index.
		/// \details
		/// returns a menu by reference when given an integer as index.
		
		menu<u>* getMenuByIndex(int index){
			return menus[index];
		}
		
		/// \brief
		/// returns a pointer of menu to the currently selected menu
		/// \details
		/// returns a pointer of menu selecte by the currentMenuIndex variable
		menu<u>* getCurrentMenu(){
			return menus[currentMenuIndex];
		}
		
		/// \brief
		/// returns the current value cursorPosition
		/// \details
		/// returns the current value of cursorPosition as an unsigned integer
		unsigned int getCurrentCursorPos(){
			return cursorPosition;
		}
		
		/// \brief
		/// set cursorPosition to given value
		/// \details
		/// set cursorPosition to the given unsigned integer while taking the menu size limit into account.
		void setCursorPos(unsigned int i){
			if(getCurrentMenuSize() >= i){
				cursorPosition = i;
			}
		}
		
		/// \brief
		/// returns the currently selected menu size
		/// \brief
		/// returns the currenctly selected menu size using the currentMenuIndex and a pointer to the currently selected menu.
		unsigned int getCurrentMenuSize(){
			return menus[currentMenuIndex]->getMenuItemArraySize();
		}
		
		/// \brief
		/// Decreases the value of cursor position.
		/// \details
		/// Decreases the value of cursor position but not below 0.
		/// Since the menu items will probably be displayed from 0 untill the end of the array, the cursor needs to decrease to move the cursor up visually.
		void cursorUp(){
			if(cursorPosition > 0){
				cursorPosition--;
			}
		}
		
		/// \brief
		/// Increases the value of cursor position.
		/// \details
		/// Increases the value of cursor position but not above the current menu size.
		/// Since the menu items will probably be displayed from 0 untill the end of the array, the cursor needs to Increases to move the cursor down visually.
		void cursorDown(){
			if(cursorPosition < getCurrentMenuSize() -1){
				cursorPosition++;
			}
		}
		
		/// \brief
		/// Executes the currently selected menuitem.
		/// \details
		/// Executes the currently selected menuItem, using the menu pointer of the current menu and the cursor position variable.
		void select(){
			getCurrentMenu()->getMenuItemByIndex(cursorPosition)->run();
		}
		
		/// \brief
		/// Set the menu to the given menu
		/// \details
		/// Set the menu to the by refrence given menu using the getBaseMenuPosition function which returns the index of the given function.
		void setMenu(menu<u>* menu){
			currentMenuIndex = menu->getBaseMenuPosition(); 
		}
		
		/// \brief
		/// Set menu to given integer
		void setMenu(unsigned int i){
			currentMenuIndex = i;
		}
		
		/// \brief
		/// Set the new menu to the parent menu of the current menu.
		/// \brief
		/// Set the new menu to the parent menu of the current menu and resetting the cursor.
		void previousMenu(){
			currentMenuIndex = getCurrentMenu()->getParentMenuPosition();
			cursorPosition = 0;
		}
		
		/// \brief
		/// Set the new menu to the parent menu of the current menu.
		/// \brief
		/// Set the new menu to the parent menu of the current menu and resetting the cursor, while also going to a selected menu item.
		void previousMenu(unsigned int newCursorPos){
			currentMenuIndex = getCurrentMenu()->getParentMenuPosition();
			cursorPosition = newCursorPos;
		}
		
		/// \brief
		/// Adds a new menu to the menus array.
		/// \details
		/// Adds a new menu to the menus array while also making sure it does not overload the array and setting a new baseMenuPosition to the newly added menu.
		void addMenu(menu<u>* menu){
			for(unsigned int i = 0; i < menus.size(); i++ ){
				if(menus[i] == NULL){
					menus[i] = menu;
					menus[i]->setBaseMenuPosition(i);
				}
			}
		}
};
}
	
#endif // BASEMENU_HPP
