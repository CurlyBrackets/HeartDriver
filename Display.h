#ifndef DISPLAY_H
#define DISPLAY_H
#include <vector>
#include <string>
#include <cctype>
#include <wiringPi.h>
#include <boost/thread.hpp>


//#define PIN_FLASH 	0
#define PIN_RESET 	14
#define PIN_A0		13
#define PIN_A1		12
#define PIN_A2		3
#define PIN_A3		2
#define PIN_A4		0
#define PIN_CE		7

#define FADE_DELAY 150

const char* PINS = {11,10,6,5,4,1,16,15};

class Application;

class Display{
	public:
		Display(std::vector<std::string>& plines, bool* prunning, bool* prefresh);
		void operator()();
	private:
		std::vector<std::string> lines;
		bool* running, *refresh;
		int currentLine;
};

#include "Application.h"

#endif // DISPLAY_H
