#include "Application.cpp"
int main(){
	Application::instance()->run();
	Application::destroy();
	return 0;
}
