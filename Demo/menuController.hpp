#ifndef MENUCONTROLLER_HPP
#define MENUCONTROLLER_HPP
#include "hwlib.hpp"
#include "lightMenu.hpp"

template<int t, int u>
class menuController : public lightMenu::controlMenu<t, u>
{
	hwlib::target::pin_in pinUp;
	hwlib::target::pin_in pinRight;
	hwlib::target::pin_in pinDown;

public:
	menuController(lightMenu::baseMenu<t, u> & baseMenu, hwlib::target::pin_in & pinUp, hwlib::target::pin_in & pinRight, hwlib::target::pin_in & pinDown):
		lightMenu::controlMenu<t, u> (baseMenu),
		pinUp( pinUp ),
		pinRight( pinRight ),
		pinDown( pinDown )
	{};
	bool read(){
//		hwlib::cout << "pinUp : " << pinUp.get() << "\n";
//		hwlib::cout << "pinDown : " << pinDown.get() << "\n";
//		hwlib::cout << "pinRight : " << pinRight.get() << "\n";
		if(pinUp.get() == 1){
			lightMenu::controlMenu<t, u>::cursorUp();
			return true;
		}else if(pinDown.get() == 1){
			lightMenu::controlMenu<t, u>::cursorDown();
			return true;
		}else if(pinRight.get() == 1){
			lightMenu::controlMenu<t, u>::select();
			return true;
		}else{
			return false;
		}
	}
};

#endif // MENUCONTROLLER_HPP