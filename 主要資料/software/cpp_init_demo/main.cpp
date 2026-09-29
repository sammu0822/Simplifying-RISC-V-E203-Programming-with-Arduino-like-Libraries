#include <stdint.h>

//Global object with constructor
class LED{

public :
	LED() { initialized = 1234;}
	uint32_t initialized;

}

LED led;

extern "C" void puthex (uint32_t val);

int main()
{
	//印出是否contructor有生效
	puthex(led.initialized);  // 預期輸出 0x000004D2 (1234)
	while(1);
}
