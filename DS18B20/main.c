#include "reg51.h"
#include  "intrins.h"

sbit DSPORT = P3^7;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;


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

void delay500us(void)   //误差 0us
{
    unsigned char a,b;
    for(b=71;b>0;b--)
        for(a=2;a>0;a--);
}


void delay80us(void)   //误差 0us
{
    unsigned char a,b;
    for(b=11;b>0;b--)
        for(a=2;a>0;a--);
}

void delay60us(void)   //误差 0us
{
    unsigned char a,b;
    for(b=3;b>0;b--)
        for(a=8;a>0;a--);
}
void delay45us(void)   //误差 0us
{
    unsigned char a;
    for(a=21;a>0;a--);
}

void delay15us(void)   //误差 0us
{
    unsigned char a;
    for(a=6;a>0;a--);
}

void delay10us(void)   //误差 0us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=2;a>0;a--);
}


unsigned char DS18B20Init()
{
	unsigned char i;
	// 首先拉低数据总线
	DSPORT=0;
	delay500us();	// 480-960us
	DSPORT=1;
	delay80us();	// 不能太大，如果超出了拉低的时间，会被上拉电阻拉高，那么接下来的判断肯定失败。
	// 判断DS18B20是否拉低数据总线响应
	// 如果初始化成功 会在15-60us内拉低，并持续60-240us
	while(DSPORT)
	{
		delay10us();
		i++;
		if(i>500){
			return 0;	// 5ms未响应
		}
	}
	return 1;
}

void DS18B20WriteByte(unsigned char value)
{
	unsigned char i=0;

	for(i=0;i<8;i++){
		// 写数据前总线拉低15us
		DSPORT=0;
		_nop_();
		DSPORT = value&0x01;		// 小端写

		// 写完一位 延时最少60us 拉高
		delay80us();
		DSPORT=1;
		//value>>=1;
		value = value>>1;
	}
}

unsigned char DS18B20ReadByte()
{
	unsigned char i=0,j=0,bi=0,byte=0;
	for(i=0;i<8;i++){
		// 将数据总线拉低 延时>1us
		DSPORT=0;
		_nop_();
		_nop_();
		_nop_();
		// 将数据总线拉高 延时10us
		DSPORT=1;
		delay10us();
		
		bi= DSPORT;
		byte = (byte>>1)|(bi<<7);	// 小端读  
		/*与EEPROM的相反，EEPROM是大端读写 
		EEPROM读写要么用都用大端，要么都用小端，因为数据是自己写的,
		只要保持读写端一致即可。
		而DS18B20读出的数据是DS18B20产生的，从数据总线传出时也是小端，所以这里读取要用小端。
		*/
		delay60us();
	}

	return byte;
}

// 转换温度
void DS18B20ChangeTemp()
{
	DSPORT=1;
	DS18B20Init();
	delayXms(2);
	DS18B20WriteByte(0xcc);
	DS18B20WriteByte(0x44);
}

// 读取温度 命令
void DS18B20SendReadComm()
{
	DSPORT=1;
	DS18B20Init();
	delayXms(2);
	DS18B20WriteByte(0xcc);
	DS18B20WriteByte(0xbe);
}

// 读取温度值
float DS18B20ReadTemp()
{
	char H,L;
	float tmp=0;
	DS18B20ChangeTemp();
	DS18B20SendReadComm();
	L=DS18B20ReadByte();
	H=DS18B20ReadByte();
//	H = 0x0a;
//	L = 0xff;
	tmp=tmp+(H&0x01)*16;
	H=H>>1;
	tmp=tmp+(H&0x01)*32;
	H=H>>1;
	tmp=tmp+(H&0x01)*64;
	
	tmp=tmp+(L&0x01)*0.0625;
	L = L>>1;
	tmp=tmp+(L&0x01)*0.125;
	L = L>>1;
	tmp=tmp+(L&0x01)*0.25;
	L = L>>1;
	tmp=tmp+(L&0x01)*0.5;
	L = L>>1;
	tmp=tmp+(L&0x01)*1;
	L = L>>1;
	tmp=tmp+(L&0x01)*2;
	L = L>>1;
	tmp=tmp+(L&0x01)*4;
	L = L>>1;
	tmp=tmp+(L&0x01)*8;
	
	
	if((H&0xf8)==0xf8){
		tmp = 0-tmp;
	}
	
	return tmp;
}

// 数码管显示的数字        0     1    2     3    4   5    6    7    8   9
unsigned char num[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char NG = 0x40;
// 如果要添加小数点就直接在个位上加0x80
// 用于存放写入的数据的值 最大可为4位
unsigned char show[4];
void ShowValue(float value)
{
	int Z=0;
	int F=0;
	if(value<0){
		show[0] = NG;
		value = 0-value;
		Z=value;
		F=value*10-Z*10;
	}
	else{
		show[0] = 0x00;
		Z=value;
		F=value*10-Z*10;
	}
	show[1] = num[Z%100/10]; //十位
	show[2] = num[Z%100%10]+0x80; //个位
	show[3] = num[F]; //十分位位
}
void ShowOnDevice()
{
	unsigned char i=0;
	for(i=0;i<4;i++){
		// 循环给四个数码管输出要显示的数
		switch(i){
			case 0:
				LSA=0;LSB=0;LSC=0;break;	// 显示第0位
			case 1:
				LSA=1;LSB=0;LSC=0;break;	// 显示第1位
			case 2:
				LSA=0;LSB=1;LSC=0;break;	// 显示第2位
			case 3:
				LSA=1;LSB=1;LSC=0;break;	// 显示第3位
		}
		P0=show[i];
		delay500us();
		P0=0x00;	// 共阴极 消隐
	}

}

float Peras(int value)
{
	float tmp;
	char H=value&0xff00;
	char L=value&0x00ff;
	tmp=tmp+(H&0x01)*16;
	H=H>>1;
	tmp=tmp+(H&0x01)*32;
	H=H>>1;
	tmp=tmp+(H&0x01)*64;
	
	tmp=tmp+(L&0x01)*0.0625;
	L = L>>1;
	tmp=tmp+(L&0x01)*0.125;
	L = L>>1;
	tmp=tmp+(L&0x01)*0.25;
	L = L>>1;
	tmp=tmp+(L&0x01)*0.5;
	L = L>>1;
	tmp=tmp+(L&0x01)*1;
	L = L>>1;
	tmp=tmp+(L&0x01)*2;
	L = L>>1;
	tmp=tmp+(L&0x01)*4;
	L = L>>1;
	tmp=tmp+(L&0x01)*8;
	if(H<0){
		tmp = 0-tmp;
	}
	return tmp;
}

void main()
{
	unsigned char ret=11;
//	int value = 0;
	float tmp=0;
	
	while(1){
		//ret = DS18B20Init();
		tmp = DS18B20ReadTemp();
//		tmp = Peras(value);
		ShowValue(tmp);
		ShowOnDevice();
	}

}