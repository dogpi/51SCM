#include "reg51.h"

sbit SCL=P2^1; 
sbit SDA=P2^0;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

sbit k1=P3^0;
sbit k2=P3^1;
sbit k3=P3^2;
sbit k4=P3^3;

unsigned char show_value=0;

// 数码管显示的数字        0     1    2     3    4   5    6    7    8   9
unsigned char num[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

// 用于存放写入的数据的值 最大可为4位
unsigned char show[4];

// 延时10us
void delay10us(void)   //误差 0us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=2;a>0;a--);
}

void delay(unsigned int i)
{
	while(i--);	
}

// 起始信号 SCL为高电平 SDA由高电平转为低电平
void Start()
{
	SDA=1;
	delay10us();
	SCL=1;
	delay10us();
	SDA=0;
	delay10us();
	SCL=0;
	delay10us();
}

// 终止信号 SCL为高电平 SDA由低电平转为高电平
void End()
{
	SDA=0;
	delay10us();
	SCL=1;
	delay10us();
	SDA=1;
	delay10us();
}


// 从机应答主机接受数据成功  SCL为高电平 SDA会被从机由高电平拉为低电平
unsigned char Response()
{
	unsigned char i=0;
	SDA=1;
	delay10us();
	SCL=1;
	delay10us();
	
	while(SDA){
		i++;
		if(i>200){
			// 如果超多2m还没被拉低，则发送失败或为费应答
			SCL=0;
			delay10us();
			return 0;
		}
	}
	SCL=0;
	delay10us();
	return 1;
}


// 主机向从机发送ACK


// 发送数据时，SDA不能在SCL高电平时改变电平
unsigned char WriteByte(unsigned char value)
{
	SCL=0;
	delay10us();
	unsigned char i=0,j=0;
	for(i=0;i<8;i++){
		SDA=value>>7;
		value=value<<1;
		delay10us();
		SCL=1;		// 修改SDA后再拉高SCL SCL上升沿时写入数据
		delay10us();	// 延时 用于写数据到iic
		SCL=0;		// 下次循环修改数据有效
		delay10us();
	}
	if(!Response()){
		return 0;
	}
//	SDA=1;
//	delay10us();
//	SCL=1;
//	delay10us();
//	
//	while(SDA){
//		j++;
//		if(j>200){
//			// 如果超多2m还没被拉低，则发送失败或为费应答
//			SCL=0;
//			delay10us();
//			return 0;
//		}
//	}
//	SCL=0;
//	delay10us();
	return 1;
}

unsigned char ReadByte()
{
	unsigned char value=0,i=0;
	SDA=1;
	delay10us();
	for(i=0;i<8;i++){
		SCL=1;
		delay10us();
		value<<=1;	// 获取第一位时 data=0 data<<1=0
		value|=SDA;// 第一位的值
		delay10us();
		SCL=0;		// SCL下降沿时读取数据
		delay10us();
	}
	return value;
}

void Write24C02(unsigned char addr,unsigned char value)
{
	Start();
	WriteByte(0xa0);	// A0=0 A1=0 A2=0 W=0都接地，所以EEPROM的地址为0xa0+0+0
	WriteByte(addr);
	WriteByte(value);
	End();
}

unsigned char Read24C02(unsigned char addr)
{
	unsigned char value;
	Start();
	WriteByte(0xa0);
	WriteByte(addr);
	Start();
	WriteByte(0xa1);		// 0xa0 + (A0A1A2)0+(W)1=0xa1
	value=ReadByte();
	End();
	return value;
}



void KeyBoard()
{
	if(0==k1){
		// 写数据
		delay(1000);
		if(0==k1){
			Write24C02(0x01,show_value);	// 向地址中写入数据
		}
		while(!k1);	// 按键抬起 
	}
	if(0==k2){
		// 读数据
		delay(1000);
		if(0==k2){
			show_value=Read24C02(0x01);
		}
		while(!k2);	// 按键抬起 
	}
	if(0==k3){
		// 显示数据加
		delay(1000);
		if(0==k3){
			show_value++;
			if(show_value>255){
				show_value=0;
			}
		}
		while(!k3);	// 按键抬起 
	}
	if(0==k4){
		// 显示数据清0
		delay(1000);
		if(0==k4){
			show_value=0;
		}
		while(!k4);	// 按键抬起 
	}
}

void ShowValue(unsigned char value)
{
	show[0] = num[value/1000];	//千位
	show[1] = num[value%1000/100]; //百位
	show[2] = num[value%1000%100/10]; //十位
	show[3] = num[value%1000%100%10]; //个位
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
		delay(100);
		P0=0x00;	// 共阴极 消隐
	}

}

void main()
{
	while(1){
		KeyBoard();
		ShowValue(show_value);
		ShowOnDevice();
	}
}