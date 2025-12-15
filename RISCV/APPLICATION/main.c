#include <string.h>
#include "api.h"

char test[10];

int main(void)
{
	putchar('H');
	putchar('E');
	putchar('L');
	putchar('L');
	putchar('O');
	delay(100);
	millis();
	delayMicroseconds(1000000);
	memset(test, 0, sizeof(test));
	return strlen("Hello World"); // 11
}