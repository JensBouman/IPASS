// ==========================================================================
//
// File      : main.cpp
// Part of   : C++ max7219 ledMatrix - FallBall demo
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
#include "hwlib.hpp"
#include "../../max7219.hpp"
#include <stdlib.h>
#include "MPU6050.hpp"
#include "lightMenu.hpp"
#include "oledmenu.hpp"
#include "menuController.hpp"

class gameControl : public lightMenu::menuItem{
	private:
		hwlib::target::pin_in calibrationButton;
		hwlib::window_ostream screen;
		MPU6050 mpu;
		int gameWait;
		int gapWidth;
	public:
		gameControl(hwlib::string<0> name, hwlib::target::pin_in & calibrationButton, hwlib::window_ostream & screen, MPU6050 & mpu, const int & tmpGameWait, const int & tmpGapWidth):
			lightMenu::menuItem(name),
			calibrationButton(calibrationButton),
			screen(screen),
			mpu(mpu),
			gameWait(tmpGameWait),
			gapWidth(tmpGapWidth)
		{}
		
		uint8_t calculateCoordData(const int & posX){
			uint8_t coordData = posX%8;
			// if its 0, that means its the furthest bit; 8
			if(coordData == 0){
				coordData = 8;
			}
			return coordData;
		}
		
		unsigned int calculateScreenN(const int & posX){
			//Calculating coords for led
			unsigned int screenN = (posX/8);
			//If posX mod 8 = 0, it means it is at the end of a screen (+1)
			if(posX%8 != 0){
				screenN +=1;
			}
			return screenN;
		}

		void setGameWait(const int & tmpGameWait){
			gameWait = tmpGameWait;
		}

		void setGapWidth(const int & tmpGapWidth){
			gapWidth = tmpGapWidth;
		}

		void run(){
			calibrate();
			int score = startGame();
			hwlib::cout << score << '\n';
			
			screen 
				<< "\f" << "Je hebt: "   
				<< "\t0302" << score
				<< "\n" << "punten behaald."
				<< hwlib::flush;
			
			hwlib::wait_ms(2500);
			
			screen << hwlib::flush;
		}
		
		void calibrate(){
//			setting random seed
			srand(mpu.getAccelX());
		}
		
		int startGame(){
			// max7219 led matrices
			const unsigned int n = 4;
			auto din     = hwlib::target::pin_out(hwlib::target::pins::d51);
			auto cs      = hwlib::target::pin_out(hwlib::target::pins::d47);
			auto clk     = hwlib::target::pin_out(hwlib::target::pins::d45);
			auto csInv   = max7219::pin_out_invert(cs);
			auto spi_bus = max7219::spiBusLed(clk, din, csInv, hwlib::pin_in_dummy);
			max7219::ledMatrixSet<n> matrices(spi_bus);
			matrices.resetRegisters();
			matrices.setCycle(false);
			
			int posX = n*8;
			int posY = 4;
			int previousPosX = posX;
			int previousPosY = posY;
			
			hwlib::wait_ms(1500);
			hwlib::cout << "gametime\n";
			hwlib::cout << "GameWait: " << gameWait << "\n";
			hwlib::cout << "GapWidth: " << gapWidth << "\n";
			bool game = true;
			int i = 0;
			while(game){
				int j = 0;
				bool player = true;
				while(player){
					previousPosX = posX;
					previousPosY = posY;
					mpu.setAccelGyro();

					int posXMod = 0;
					if((((matrices.getLedMatrix(calculateScreenN(posX-1)).getLatchedValue(posY)&255) >> (calculateCoordData(posX-1)-1)) & 1) == 1){
						posXMod = 0;
					}else{
						posXMod = -1; // gravity pulling it down once
					}
					posX += posXMod;
					if(posX <1){
						posX = 1;
					}
					
					int16_t turn = mpu.getAccelZ();
//					hwlib::cout << int(turn) << '\n';
					int posYMod = 0;
					if(turn > 5000){
						posYMod -= 1;
					}
					if(turn < -5000){
						posYMod += 1;
					}
					
					// boundaries
					posY += posYMod;
					if(posY > 8){
						posY = 8;
					}else if(posY < 1){
						posY = 1;
					}
					
					//falling 
					unsigned int screenN = calculateScreenN(posX);
					uint8_t coordData = calculateCoordData(posX);
					
					matrices.resetLedAt(calculateScreenN(previousPosX), previousPosY, calculateCoordData(previousPosX));
					matrices.setLed(screenN, posY, coordData, true);
					
					j++;
					if(j == 4){
						player = false;
					}else{
						hwlib::wait_ms(gameWait);
					}
				}

				uint8_t row = 0;
				if(i%4 == 0){
					row = (255 ^ gapWidth << rand()%8);
				}
				unsigned int screenN = calculateScreenN(posX);
				max7219::ledMatrix currentLed = matrices.getLedMatrix(screenN);
				uint16_t collumn = currentLed.getLatchedValue(posY);
				uint8_t coordData = calculateCoordData(posX);
				//If coords are same as row, game over
				if((((collumn & 255) >> (coordData-1))&1) == 1 && posX == n*8){
					game = false;
				}

					
				if(posX >= int(n*8)){
					posX = n*8;
				}else{
					posX += 1;
				}
				matrices.cycleSteps(1);
				matrices.setRow(1, row);
				hwlib::wait_ms(gameWait);
				i++;
			}
			matrices.resetRegisters();
			
			hwlib::wait_ms(500);
			matrices.setCycleDelay(150*60000);
			matrices.setLetter(4, 'G');
			matrices.setLetter(3, 'a');
			matrices.setLetter(2, 'm');
			matrices.setLetter(1, 'e');
			hwlib::wait_ms(1500);
			matrices.setLetter(4, 'O');
			matrices.setLetter(3, 'v');
			matrices.setLetter(2, 'e');
			matrices.setLetter(1, 'r');
			hwlib::wait_ms(1500);
			
			matrices.resetRegisters();
			
			return i;
		}
	
};

class difficultyControl : public lightMenu::menuItem{
	private:
		gameControl *game;
		int gameWait;
		int gapWidth;
	public:
		difficultyControl(hwlib::string<0> name, gameControl * game, const int & tmpGameWait, const int & tmpGapwidth):
			lightMenu::menuItem(name),
			game(game),
			gameWait(tmpGameWait),
			gapWidth(tmpGapwidth)
		{}
	
		void run(){
			game->setGameWait(gameWait);
			game->setGapWidth(gapWidth);
			setName(getName() << "!");
		}
	
};

int main(){
	// kill the watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;
	hwlib::wait_ms(1000);
	
	auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl1 );
	auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda1 );
	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
	auto displayOled = hwlib::glcd_oled( i2c_bus, 0x3c ); 
	
	
   auto font    = hwlib::font_default_8x8();
   auto display = hwlib::window_ostream( displayOled, font );
	
	auto buttonSelect = hwlib::target::pin_in(hwlib::target::pins::d35);
	auto buttonUp = hwlib::target::pin_in(hwlib::target::pins::d39);
	auto buttonDown = hwlib::target::pin_in(hwlib::target::pins::d31);
	
	// MPU6050 (gyroscope/accelerator)
	auto sclGyro = hwlib::target::pin_oc(hwlib::target::pins::scl);
	auto sdaGyro = hwlib::target::pin_oc(hwlib::target::pins::sda);
	auto i2c_bus_Mpu6050 = hwlib::i2c_bus_bit_banged_scl_sda(sclGyro, sdaGyro);
	auto mpu = MPU6050(i2c_bus_Mpu6050); 
	mpu.wakeUp();
	mpu.setAccelGyro();

	
	hwlib::string<9> gameEasy;
	gameEasy.clear() << "Beginner";
	
	hwlib::string<10> gameMedium;
	gameMedium.clear() << "Gevorderd";
	
	hwlib::string<7> gameHard;
	gameHard.clear() << "Expert";
	
	hwlib::string<9> settings;
	settings.clear() << "Settings";
	
	hwlib::string<11> startGame;
	startGame.clear() << "Start game";
	
	hwlib::string<6> back;
	back.clear() << "Terug";
	
	
	gameControl game(startGame, buttonSelect, display, mpu, 50, 3);
	difficultyControl easy(gameEasy, &game, 200, 7);
	difficultyControl medium(gameMedium, &game, 50, 3);
	difficultyControl hard(gameHard, &game, 10, 1);
	
	std::array<lightMenu::menuItem*, 4> settingsMenuArray = {&easy, &medium, &hard};
	std::array<lightMenu::menuItem*, 4> mainMenuArray = {};
	
	lightMenu::menu<4> mainMenu(mainMenuArray);
	lightMenu::menu<4> settingsMenu(settingsMenuArray, &mainMenu);
	
	std::array<lightMenu::menu<4>*, 2>menuArrayTest = {&mainMenu, &settingsMenu};
	lightMenu::baseMenu<2, 4> basisMenu(menuArrayTest);
	
	auto oled = oledMenu<2, 4>(basisMenu, displayOled, 5);
	
	auto menuControl = menuController<2, 4>(basisMenu, buttonUp, buttonSelect, buttonDown);
	
	lightMenu::menuItemMenuLink<2,4> settingsMenuController(settings, settingsMenu, basisMenu);
	
	lightMenu::previousMenuMenuItem<2, 4> backButton(back, basisMenu);
	
	mainMenu.addMenuItem(&settingsMenuController);
	mainMenu.addMenuItem(&game);
	settingsMenu.addMenuItem(&backButton);
	
	oled.draw();
	for(;;){
		if(menuControl.read()){
			oled.draw();
			hwlib::wait_ms(550);
		}

	}
}