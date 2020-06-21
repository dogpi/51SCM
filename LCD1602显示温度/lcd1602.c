#include "lcd1602.h"

#include "reg51.h"
#include "intrins.h"



void WriteCommand(unsigned char comm)
{
	E = 0;	// E为高电平时会将引脚上的数据写入寄存器 
	RS = 0;	// 发送命令
	RW = 0;	// 写
	DATAPORT=comm;
	delay1ms();
		
	E = 1;	// 写入
	delay1ms();
	E = 0;
}


void WriteData(unsigned char value)
{
	E = 0;
	RS = 1;	// 发送数据
	RW = 0; // 写
	
	DATAPORT = value;
	delay1ms();
	E = 1;
	delay1ms();
	E = 0;
}


void LCD1602Init()
{
	WriteCommand(0x38);	// 8位数据总线 2行显示 5*7点阵/每字符
	WriteCommand(0x06); // 写后光标右移，不移动屏幕
	WriteCommand(0x0c);	// 开显示 不显示光标
	WriteCommand(0x01);	// 清屏
	WriteCommand(0x80); // 起始显示点，第一个行第一个位置
}
