#include "Application.h"

Application* Application::ptr = NULL;

Application* Application::instance(){
	if(!ptr)
		ptr = new Application();
	return ptr;
}

void Application::destroy(){
	delete ptr;
	ptr = NULL;
}

Application::Application()
{
	//ctor
}

Application::~Application()
{
	//dtor
}

void Application::setup(){
	if(wiringPiSetup() == -1)
		return;
	for(unsigned char i=0;i<8;i++){
	  	digitalWrite(PINS[i], OUTPUT);
   	}

	//pinMode(PIN_FLASH, OUTPUT);
	pinMode(PIN_RESET, OUTPUT);
	pinMode(PIN_A0, OUTPUT);
	pinMode(PIN_A1, OUTPUT);
	pinMode(PIN_A2, OUTPUT);
	pinMode(PIN_A3, OUTPUT);
	pinMode(PIN_A4, OUTPUT);
	pinMode(PIN_CE, OUTPUT);
}

void Application::init(){
	digitalWrite(PIN_RESET, HIGH);
	wait();
	digitalWrite(PIN_RESET, LOW);

	std::vector<std::vector<bool>> heart = {
	{0,1,0,1,0},
	{1,0,1,0,1},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{0,1,0,1,0},
	{0,1,0,1,0},
	{0,0,1,0,0}
	};

	write_special_char(1, heart);
}

void Application::run(){
	setup();
	init();

	refresh = false;
	running = true;

	boost::thread* display = new boost::thread(Display(lines, &running, &refresh));
	std::string input;
	while(running){
		std::cin >> input;
		if(!input.compare("exit"))
			running = false;
	}
	display->join()
	delete display;
}

void Application::push_data(char* pin_config, int data){
	for(unsigned char i=0;i<8;i++){
		digitalWrite(pin_config[i], (data&(0x80>>i))>>(7-i));
	}
}

void Application::wait(){
	delayMicroseconds(1);
}

void Application::write_char(unsigned char pos, int ch){
	//digitalWrite(PIN_FLASH, LOW);
	digitalWrite(PIN_A0, pos&0x1);
	digitalWrite(PIN_A1, (pos&0x2)>>1);
	digitalWrite(PIN_A2, (pos&0x4)>>2);
	digitalWrite(PIN_A3, HIGH);
	digitalWrite(PIN_A4, HIGH);
	wait();//rpobably not needed
	digitalWrite(PIN_CE, (pos&0x8)>>3);
	wait();
	//wiringPiI2CWrite(EXPANDER_ADDR, ch);
	push_data(PINS, ch);
	wait();
}

void Application::write_special_char(unsigned char addr, std::vector<std::vector<bool>>& points){
	//digitalWrite(PIN_FLASH, LOW);
	digitalWrite(PIN_A4, LOW);
	digitalWrite(PIN_A3, LOW);
	wait();
	for(unsigned int ce=0;ce<2;ce++){
		digitalWrite(PIN_CE, ce);
		wait();
		//wiringPiI2CWrite(EXPANDER_ADDR, addr);
     	push_data(PINS, addr);
		wait();
		for(unsigned char row=0;row<7;++row){
			//digitalWrite(PIN_FLASH, LOW);
			digitalWrite(PIN_A4, LOW);
			digitalWrite(PIN_A3, HIGH);
			digitalWrite(PIN_A2, (row&0x4)>>2);
			digitalWrite(PIN_A1, (row&0x2)>>1);
			digitalWrite(PIN_A0, row&0x1);
			wait();
			unsigned char data = 0;
			for(unsigned int i=0;i<5;i++)
				data |= points[row][i]<<(4-i);
			//wiringPiI2CWrite(EXPANDER_ADDR, data);
    	   	push_data(PINS, data);
			wait();
		}
	}
}

void Application::write_command(bool ce, unsigned char cmd){
	//digitalWrite(PIN_FLASH, HIGH);
	digitalWrite(PIN_A4, HIGH);
	digitalWrite(PIN_A3, LOW);
	wait();
	digitalWrite(PIN_CE, ce);
	wait()
   	write_data(PINS, cmd);
	wait();
}

void Application::push_message(std::string str){
	std::vector<std::string> tlines;
	for(int i=0;i<str.length();i++){
		int temp = i+8;
		while(str[i] > 0x20 && str[i] < 0x7F && temp)
			temp--;
		if(temp){
			tlines.push_back(str.substr(i,temp-i));
			i = temp+1;
		}
		else{
			tlines.push_back(str.substr(i,8));
			i += 8;
		}
	}
	refresh = true;
	lines.copy(tlines);
	refresh = false;
}
