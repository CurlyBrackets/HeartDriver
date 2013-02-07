#include "Display.h"

Display::Display(std::vector<std::string>& plines, bool* prunning, bool* prefresh):
running(prunning),refresh(prefresh){
	lines = plines;
	currentLine = 0;
	for(char brightness=0;brightness<8;brightness++){
		Application::instance()->write_command(false, brightness&0x7);
		Application::instance()->write_command(true,  brightness&0x7);
		Application::instance()->wait();
	}
}

void Display::operator()(){
	while(*running){
		for(char brightness=0;brightness<8;brightness++){
			Application::instance()->write_command(false, brightness&0x7);
			Application::instance()->write_command(true,  brightness&0x7);
			delay(FADE_DELAY/7);
		}
		if(*refresh){
			currentLine = 0;
			while(*refresh);
		}
		for(char char_addr=0;char_addr<8;char_addr++){
			if(lines[currentLine].empty() || char_addr > lines[currentLine].size()-1)
				Application::instance()->write_char(char_addr, ' ');
			else
				Application::instance()->write_char(char_addr,lines[currentLine][char_addr]);

			if(currentLine+1 > lines.size()-1 || lines[currentLine+1].empty() || char_addr > lines[currentLine+1].size()-1)
				Application::instance()->write_char(0x8|char_addr, ' ');
			else
				Application::instance()->write_char(0x8|char_addr, lines[currentLine+1].size())
		}
		for(char brightness = 7;brightness > -1;brightness--){
			Application::instance()->write_command(false, brightness&0x7);
			Application::instance()->write_command(true, brightness&0x7);
			delay(FADE_DELAY/7);
		}
		currentLine += 2;
		if(currentLine > lines.size()-1){
			currentLine = 0;
			delay(2000);
		}
		else{
			delay(1000);
		}
	}
}
