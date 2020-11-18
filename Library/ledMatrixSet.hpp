// ==========================================================================
//
// File      : ledMatrixSet.hpp
// Part of   : C++ max7219 ledMatrix library
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

#ifndef LEDMATRIXSET_HPP
#define LEDMATRIXSET_HPP
#include "hwlib.hpp"
#include "ledMatrix.hpp"
#include "spiBusLed.hpp"

namespace max7219{
	/// \brief
	/// Container class for daisy-chained max7219 chips
	/// \details
	/// This class manages a given set of ledMatrix classes and adds 
	/// functionality for the user. 
	/// 
	/// Due to it being a templated class, (and no access to heap for vectors)
	/// the number of ledMatrices must be known at compile time. N in the context
	/// of this class means the number of screens attached (and the size of the
	/// internal array in which they are stored).
	template<unsigned int n>
	class ledMatrixSet {
		private:
			ledMatrix ledMatrices[n];
			spiBusLed & spiBus; //Custom made spi controller for 16bit 
			int_fast32_t cycleDelayNs = 50; // in ns
			bool cycle = true; //If true, cycle functions will overflow back
			hwlib::font_default_8x8 f = hwlib::font_default_8x8(); //font
			
			/// \brief
			/// Inserts tempvalue at first ledMatrix, and propogates the
			/// previous value to next ledMatrices.
			/// \details
			/// Sets the temporary value of the last ledMatrix to the temporary
			/// value of the one before it.
			/// 
			/// The first ledMatrix gets the newly inserted value.
			///
			/// The temporary value of the last ledMatrix will be discarded.
			void insertTempValue(const uint16_t & tmpValue){
				for(int i = n-1; i > 0; --i){
					ledMatrices[i].setTempValue(ledMatrices[i-1].getTempValue());
				}
				ledMatrices[0].setTempValue(tmpValue);
			}
			
			/// \brief
			/// Calls the latchRegister() method on each ledMatrix starting at 
			/// n-1.
			void latchAllRegisters(){
				for(int i = n-1; i >= 0; --i){
					ledMatrices[i].latchRegister();
				}
			}
			
			/// \brief
			/// Makes a 16bit packet, sets it as temporary value and sends it
			/// to the spiBus.
			/// \details
			/// Adds the registerAddr and dataOut together with the spiBusLed
			/// makeDataArray() method, to build a correct packet.
			///
			/// It inserts that packet as temporary value, and sends it to the
			/// spiBus writeReadCommand() function.
			///
			/// If the given buffer parameter is set to false, it triggers a
			/// falling edge before sending the packet, and triggers a rising
			/// edge after sending the packet. This also latches temporary
			/// values.
			/// Leave buffer = true to set your own rising and falling edges.
			void sendCommand(const uint8_t & registerAddr, const uint8_t & dataOut, uint16_t & dataIn, const bool & buffer = true){
				uint16_t packet16bit = spiBusLed::makeDataArray(registerAddr, dataOut);
				if(buffer){
					insertTempValue(packet16bit);
					spiBus.writeReadCommand(&packet16bit, &dataIn);
				}else{
					openComms();
					insertTempValue(packet16bit);
					spiBus.writeReadCommand(&packet16bit, &dataIn);
					closeComms();
				}
			}
			
			/// \brief
			/// Sends a command to given screen number, and 0x00 to all others at no-nop register.
			/// \details
			/// This function loops through all ledMatrices and calls 
			/// sendCommand. when the iterator equals the given screen number,
			/// it gives the actual data and register address, otherwise it 
			/// sends 0x00 as both address and data.
			/// 
			/// It loops starting from i = n, towards i == 0.
			void sendCommandToScreen(const uint8_t & registerAddr, const uint8_t & dataOut, const unsigned int & screenN, uint16_t & dataIn, const bool & buffer = true){
				for(unsigned int i = n; i > 0; --i){
					if(i == screenN){
						sendCommand(registerAddr, dataOut, dataIn, buffer);
					}else{
						sendCommand(0x00, 0x00, dataIn, buffer);
					}
				}
			}
			
			/// \brief
			/// Triggers both a rising and falling edge and latches temporary
			/// values.
			void pushLastSignal(){
				openComms();
				closeComms();
			}
			
			/// \brief
			/// Pushes the all leds one collumn to the far end (towards n), and 
			/// if cycle is true the overflow is added back on the first 
			/// ledMatrix.
			/// \details
			/// Loops through each digit register. It keeps the overflow bit
			/// and then loops through the screens from the farthest (n) to 0.
			/// Then the last shifting bit of the earlier screen gets added to
			/// the bits from the currently iterated screen.
			/// 
			/// If cycle is true, then it sets the 0th screen with the overflowing bit.
			/// If cycle is false, the 0th screen will be pushed a 0.
			/// 
			/// Once all bits are send to the correct screen, comms will be closed, to ensure smooth transition.
			///
			/// Afterwards, the push last signa will be sent to ensure latching to be done.
			void cycleStep(){
				uint16_t dataIn = 0;
				for(int collumn = 1; collumn <= 8; collumn++){
					openComms();
					
					if(cycle){
						uint8_t lastBit = ledMatrices[n-1].getLatchedValue(collumn) & 128;
						for(int screen = n-1; screen > 0; --screen){
							uint8_t dataOut = (ledMatrices[screen].getLatchedValue(collumn) << 1) | ((ledMatrices[screen-1].getLatchedValue(collumn) & 128) >> 7);
							sendCommand(collumn, dataOut, dataIn);
						}
						uint8_t dataOut = (ledMatrices[0].getLatchedValue(collumn) << 1) | (lastBit >> 7);
						sendCommand(collumn, dataOut, dataIn);
					}else{
						for(int screen = n-1; screen > 0; --screen){
							uint8_t dataOut = (ledMatrices[screen].getLatchedValue(collumn) << 1) | ((ledMatrices[screen-1].getLatchedValue(collumn) & 128) >> 7);
							sendCommand(collumn, dataOut, dataIn);
						}
						uint8_t dataOut = (ledMatrices[0].getLatchedValue(collumn) << 1 | 0);
						sendCommand(collumn, dataOut, dataIn);
					}
					closeComms();
				}
				pushLastSignal();
			}
			
		public:			
			/// \brief
			/// Constructs the ledmatrixset with given reference to spiBus
			/// \details 
			/// Initializes n ledMatrices in the ledMatrices array, this will 
			/// initialize all registers as 0x00. resetRegisters() will have to 
			/// be called to ensure correct values.
			ledMatrixSet(spiBusLed & spiBus):
				spiBus(spiBus)
			{
				for(unsigned int i = 0; i < n; ++i){
					ledMatrices[i] = ledMatrix();
				}
			}
			
			/// \brief
			/// Returns the current cycleDelay in ns
			int_fast32_t getCycleDelay(){
				return cycleDelayNs;
			}
			
			/// \brief
			/// Sets the cycle delay in ns to given parameter. If the number is
			/// negative, it is set to 0. 
			void setCycleDelay(const int_fast32_t & tempDelay){
				if(tempDelay < 0){
					cycleDelayNs = 0;
				}else{
					cycleDelayNs = tempDelay;
				}
			}
			
			/// \brief
			/// Sets the cycle overflow. If true, will add overflow from the 
			/// nth screen to 0th screen.
			void setCycle(const bool & tempCycle){
				cycle = tempCycle;
			}
			
			/// \brief
			/// Gets the cycle overflow.
			bool getCycle(){
				return cycle;
			}
			
			/// \brief
			/// Returns the ledmatrix at given screen number. Screen number is 
			/// 1-based, ledmatrices is 0-based, so minus 1.
			ledMatrix getLedMatrix(unsigned int screenN){
				return (screenN <= n || screenN > 0) ? ledMatrices[screenN-1] : ledMatrices[0];
			}
			
			/// \brief
			/// Triggers rising edge and latches register values on all 
			/// ledMatrices
			/// \details
			/// Triggers rising edge on select pin through spiBus, after 
			/// waiting a half period.
			/// 
			/// Also latches temporary register values on all ledMatrices
			void closeComms(){
				spiBus.waitHalfPeriod();
				spiBus.closeComms();
				spiBus.waitHalfPeriod();
				latchAllRegisters();
			}
			
			/// \brief
			/// Sets a falling edge through spiBus openComms().
			void openComms(){
				spiBus.openComms();
			}
			
			/// \brief
			/// Sets a register on given screen to given data
			/// \details
			/// Opens communication, uses sendCommandToScreen() method to send
			/// the data to the correct screen and register and closes 
			/// communication
			void setRegister(const unsigned int & screenN, const uint8_t & registerAddr, const uint8_t & data){
				uint16_t dataIn = 0;
				openComms();
				sendCommandToScreen(registerAddr, data, screenN, dataIn);
				closeComms();
			}
			
			/// \brief
			/// Sets registers to their corresponding default values
			/// \details
			/// Loops through all 16 registers (0x14 and 0x15 aren't used, but 
			/// will be set) and sets the correct value. This differs for the
			/// shutdown, intensity, scan limit and test display registers.
			/// Those registers get their special default value. Others get set
			/// to 0x00.
			///
			/// After the registers are set, a 0x00 command is pushed n times,
			/// to make sure that the temporary registers are all empty
			void resetRegisters(){
				uint16_t dataIn = 0;
				for(uint8_t i = 0; i < 16; ++i){
					uint8_t registerData = 0x00;
					if(i == max7219::ledMatrix::ADDR_SHUTDOWN){
						registerData = max7219::ledMatrix::SHUTDOWN_OFF;
					}else if(i == max7219::ledMatrix::ADDR_INTENSITY){
						registerData = max7219::ledMatrix::INTENSITY_MAX;
					}else if(i == max7219::ledMatrix::ADDR_SCAN_LIMIT){
						registerData = max7219::ledMatrix::SCAN_LIMIT_ALL;
					}else if(i == max7219::ledMatrix::ADDR_DISPLAY_TEST){
						registerData = max7219::ledMatrix::DISPLAY_TEST_OFF;
					}
					openComms();
					sendCommand(i, registerData, dataIn);
					closeComms();
				}
				//make sure the data is pushed to the last ledMatrix
				for(unsigned int i = 0; i < n; ++i){
					openComms();
					sendCommand(0x00, 0x00, dataIn);
					closeComms();
				}
			}
			
			/// \brief
			/// Outputs all register contents to cout
			/// \details
			/// Calls the printRegister() function for all ledMatrices
			void printAllRegisters(){
				for(unsigned int i = 0; i < n; ++i){
					hwlib::cout << "Scherm " << i+1 << "\n" ;
					ledMatrices[i].printRegisters();
					hwlib::cout << "\n\n";
				}
			}
			
			/// \brief
			/// Sets the collumn to data at given screen.
			/// \details
			/// If given collumn is a digit register, the sendCommandToScreen()
			/// method will be called. If not, nothing happens.
			/// 
			/// If coordinates is set to true, the led on (collumn, data) will 
			/// be set, (1,1) being the corner.
			/// If coordinates is set to false, the data value as bits will be 
			/// set.
			void setLed(const unsigned int & screenN, const uint8_t & collumn, const uint8_t & data, const bool & coordinates = false){
				uint16_t dataIn = 0;
				uint8_t dataOut = data;
				if(coordinates == true){
					if(data >= 1 && data <= 8){
						dataOut = 1 << (data-1);
					}else{
						dataOut = 0x00;
					}
					dataOut = dataOut | uint8_t(ledMatrices[screenN-1].getLatchedValue(collumn) & 255);
				}
				if(collumn >= 1 && collumn <= 8){
					openComms();
					sendCommandToScreen(collumn, dataOut, screenN, dataIn);
					closeComms();
					pushLastSignal();
				}
			}
			
			/// \brief
			/// Sets the led at given screen, collumn and row to off.
			/// \details 
			/// The led on (collumn, data) will be set off, (1,1) being the
			/// corner.
			void resetLedAt(const unsigned int & screenN, const int & collumn, const int & row){
				uint16_t dataIn = 0;
				uint8_t dataOut = 255;
				if(row >= 1 && row <= 8){
					dataOut &=  ~(1 << (row-1));
				}else{
					dataOut = 0x00;
				}
				dataOut = dataOut & uint8_t(ledMatrices[screenN-1].getLatchedValue(collumn) & 255);
				if(collumn >= 1 && collumn <= 8){
					openComms();
					sendCommandToScreen(collumn, dataOut, screenN, dataIn);
					closeComms();
					pushLastSignal();
				}
			}
			
			/// \brief
			/// Sets given letter from 8x8 font to screen
			/// \details
			/// Gets the image from the font at index [letter]. This image
			/// contains each row of data. The colors will be evaluated against
			/// hwlib::black.
			void setLetter(const unsigned int & screenN, const char & letter){
				uint16_t dataIn = 0;
				auto & test = f[letter];
				for(int i = 0; i < 8; i++){
					uint8_t dataOut = 0;
					for(int j = 0; j < 8; j++){
						auto c = test[hwlib::location(j,i)];
						dataOut = (dataOut << 1) | (c == hwlib::black);
					}
					openComms();
					sendCommandToScreen(i+1, dataOut, screenN, dataIn);
					closeComms();
				}
			}
			
			/// \brief
			/// Sets a word spread over ledMatrices
			/// \details
			/// This function will trim the word to n, and display the letters
			/// from n to 0. It uses the setletter function with a char from word.
			void setWord(const char word[], const unsigned int & size){
				unsigned int x = size > n ? n : size; //auto trim the word to fit the matrix set
				for(unsigned int i = 0; i < x ; ++i){
					setLetter(i+1, word[x-i-1]);
				}
			}
			
			/// \brief 
			/// Sets the first row of given screen to the corresponding bit
			/// \details
			/// It iterates from 0 to 7, building its data from the latchedvalue
			/// of the matrix on screenN-1. The data from latchedvalue is |'ed
			/// with the last bit of row >> i.
			/// This means that the least significant bit will be sent to collumn 1,
			/// while the most significant bit will be sent to collumn 8.
			void setRow(const unsigned int screenN, const uint8_t & row){
				uint16_t dataIn = 0;
				for(int i = 0; i < 8; ++i){
					uint16_t dataOut = (ledMatrices[screenN-1].getLatchedValue(i+1) & 255) | ((row >> i) & 1);
					openComms();
					sendCommandToScreen(i+1, dataOut, screenN, dataIn);
					closeComms();
				}
				pushLastSignal();
			}
			
			
			/// \brief
			/// Cycles the leds stepsN times, with a delay of cycleDelayNs in ns
			/// \details
			/// the cycleStep() method will be called for each step, then wait
			/// cycleDelayNs
			void cycleSteps(const int & stepsN){
				for(int i = stepsN; i > 0; --i){
					cycleStep();
					hwlib::wait_us(cycleDelayNs/1000);
				}
			}
			
			/// \brief
			/// Cycles the whole screen nce
			/// \details
			/// Cycles over n*8 steps using cycleStep() method, waiting cycleDelayNs every step
			void cycleSet(){
				for(int i = n*8; i > 0; --i){
					cycleStep();
					hwlib::wait_us(cycleDelayNs/1000);
				}
			}
	};
	
}

#endif // LEDMATRIXSET_HPP