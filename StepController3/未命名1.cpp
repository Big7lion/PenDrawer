#include <stdio.h>
int main()
{
	long int a = 0xf3a4;
	int x=0xee,y=0x33;
	a=((long int)x<<8)|y;
	printf("%x\t",a);
	while(1);
}
