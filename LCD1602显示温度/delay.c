#include "delay.h"

void delay1ms()
{
	unsigned char a,b,c;
	for(c=1;c>0;c--)
			for(b=142;b>0;b--)
					for(a=2;a>0;a--);
}

void delayXms(unsigned int x)
{
	int i=0;
	for(i=0;i<x;i++){
		delay1ms();
	}
}

void delay500us(void)   //Îó²î 0us
{
    unsigned char a,b;
    for(b=71;b>0;b--)
        for(a=2;a>0;a--);
}


void delay80us(void)   //Îó²î 0us
{
    unsigned char a,b;
    for(b=11;b>0;b--)
        for(a=2;a>0;a--);
}

void delay60us(void)   //Îó²î 0us
{
    unsigned char a,b;
    for(b=3;b>0;b--)
        for(a=8;a>0;a--);
}
void delay45us(void)   //Îó²î 0us
{
    unsigned char a;
    for(a=21;a>0;a--);
}

void delay15us(void)   //Îó²î 0us
{
    unsigned char a;
    for(a=6;a>0;a--);
}

void delay10us(void)   //Îó²î 0us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=2;a>0;a--);
}
