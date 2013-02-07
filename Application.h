#ifndef APPLICATION_H
#define APPLICATION_H
#include "Display.h"
#include <iostream>

class Application
{
	public:
		virtual ~Application();
		static Application* instance();
		static void destroy();
		void setup();
		void init();
		void run();

		void push_message(std::string str);

	private:
		std::vector<std::string> lines;
		bool running, refresh;
		static Application* ptr;

		Application();
		void push_data(char* pin_config, int data);
		void wait();
		void write_char(unsigned char pos, int ch);
		void write_special_char(unsigned char addr, std::vector<std::vector<bool>>& points);
		void write_command(bool ce, unsigned char cmd);
};

#endif // APPLICATION_H
