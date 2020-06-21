#include "ds18b20.h"



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
