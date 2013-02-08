#include <iostream>
#include <boost/thread.hpp>
#include <vector>
#include "ClockedInput.h"

int main(int argc, char** argv){
	std::vector<Input::Callback> funcs = {
		[](){std::cout << "First button pressed" << std::endl;},
		[](){std::cout << "Second button pressed" << std::endl;},
		[](){std::cout << "Third button pressed" << std::endl;},
		[](){std::cout << "Fourth button pressed" << std::endl;}
	};
	bool running = true;

	boost::thread *trd = new boost::thread(Input(&running, funcs, 8, 9, 4));
	std::string input;
	while(running){
		std::cin >> input;
		if(!input.compare("exit"))
			running = false;
		else
			std::cout << input  << "rawr1" << std::endl;
	}
	trd->join();
	delete trd;
	return 0;
}
