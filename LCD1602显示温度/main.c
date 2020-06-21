#include "reg51.h"
#include "ds18b20.h"
#include "lcd1602.h"
#include "delay.h"

char s[] = "current temp is:";
char str[7];
void ShowValue(float value)
{
	int Z=0;
	int F=0;
	if(value<0){
		str[0] = '-';
		value = 0-value;
		Z=value;
		F=value*10-Z*10;
	}
	else{
		str[0] = '+';
		Z=value;
		F=value*10-Z*10;
	}
	str[1] = Z%100/10 + 48;
	str[2] = Z%100%10 + 48;
	str[3] = '.';
	str[4] = F + 48;
	str[5] = 'C';
	str[6] = '\0';
}

void main()
{
	float temp = 0;

	unsigned char i=0;

	LCD1602Init();
	
	while(1){
		WriteCommand(0x01);
		WriteCommand(0x80);
		
		for(i=0;i<16;i++){
			if(s[i]=='\0'){
				break;
			}
			WriteData(s[i]);
		}
		
		
		WriteCommand(0x80+0x40);	// µÚ¶þÐÐ
		
		temp = DS18B20ReadTemp();
		ShowValue(temp);
		
		for(i=0;i<16;i++){
			if(str[i]=='\0'){
				break;
			}
			WriteData(str[i]);
		}

		delayXms(1000);
	}

}