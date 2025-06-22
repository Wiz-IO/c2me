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
	delay(500);
	digitalWrite(0, 0);
	delayMicroseconds(500000);
	digitalWrite(0, 1);
	sec = seconds();
	mil = millis();
	mic = micros();
	return strlen("Hello Word");
}