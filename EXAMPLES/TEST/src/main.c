#include "api.h"

int aaa = 0x1234000;
int bbb = 0x0005678;
int sec, mil, mic;

int test(int x)
{
	for (int i = 0; i < 10; i++)
		x += bbb;
	return x;
}

int main(void)
{
	aaa = test(aaa);
	delay(500);					 // динамична - API
	digitalWrite(0, 0);			 // динамична - API
	delayMicroseconds(500000);	 // динамична - API
	digitalWrite(0, 1);			 // динамична - API
	sec = seconds();			 // динамична - API
	mil = millis();				 // динамична - API
	mic = micros();				 // динамична - API
	return strlen("Hello Word"); // динамична - API
}