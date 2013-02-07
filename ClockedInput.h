#ifndef CLOCKED_INPUT
#define CLOCKED_INPUT
#include <vector>
#include <wiringPi.h>

#define INPUT_DELAY 5

class Input{
	public:
		typedef void(*Callback)();
		Input(bool *prunning, std::vector<Callback>& calls, int readPin, int clockPin, int numInput){
			callbacks = calls;
			r = readPin;
			c = clockPin;
			pinMode(c, OUTPUT);
			pinMode(r, INPUT);
			pullUpDnCntrol(r, PUD_DOWN);
			current = 0;
			this->numInput = numInput;
		}
		void operator()(){
			while(*running){
				if(digitalRead(r) == HIGH && current < numInput && callbacks[current])
					callbacks[current]();
				digitalWrite(c, HIGH);
				delayMicroseconds(INPUT_DELAY);
				digitalWrite(c, LOW);
				current++;
				if(current > numInput-1)
					current = 0;
			}
		}
	private:
		std::vector<Callback> callbacks;
		int r, c, current, numInput;
		bool *running;
};

#endif // CLOCKED_INPUT

