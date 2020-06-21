#include "reg51.h"
#include "intrins.h"

/*
	LCD1602管脚介绍
	1、VSS 电源地
	2、VDD 电源正极
	3、VL	 液晶显示偏压信号，调整对比度
	4、RS	 数据/命令 选择端 （数据H 命令L）
	5、R/W 读/写     选择端 （读H   写L）   直接接地，一直写
	6、E	 使能信号
	7、D0
	8、D1
	9、D2
	10、D3
	11、D4
	12、D5
	13、D6
	14、D7
	15、BLA 背光正极
	16、BLK 背光负极
	
	命令：
		RS=0;
		R/W=0
		data:
			0x01: 清屏
			
			0x04: 写入新数据后光标左移  显示屏不移动
			0x05: 写入新数据后光标左移  显示屏整体右移1个字符
			0x06: 写入新数据后光标右移  显示屏不移动
			0x07: 写入新数据后光标右移  显示屏整体右移1个字符
		
			0x08: 显示器功能关 无光标 光标闪烁
			0x09: 显示器功能关 无光标 光标不闪烁
			0x0a: 显示器功能关 有光标 光标闪烁
			0x0b: 显示器功能关 有光标 光标不闪烁
			0x0c: 显示器功能开 无光标 光标闪烁
			0x0d: 显示器功能开 无光标 光标不闪烁
			0x0e: 显示器功能开 有光标 光标闪烁
			0x0f: 显示器功能开 有光标 光标不闪烁
		
		DDRAM:
			第一行：00H 01H 02H 03H 04H 05H 06H ...... 27H  // 00-0f为显示位  后面为缓冲区
			第二行：40H 41H 42H 43H 44H 45H 46H ...... 67H  // 40-4f为显示位  后面为缓冲区
			// 在实现滚动时可以使用缓冲区实现
			// 在使用时 这些地址要 叫0x80
	
*/

#define DATAPORT P0

sbit E = P2^7;
sbit RS = P2^6;
sbit RW = P2^5;

void delay1ms(void)   //误差 0us
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=142;b>0;b--)
            for(a=2;a>0;a--);
}


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

//////busy返回1，否则返回0//////
char IsBusy()
{
	
	return 0;
}



void LCD1602Init()
{
	WriteCommand(0x38);	// 8位数据总线 2行显示 5*7点阵/每字符
	WriteCommand(0x06); // 写后光标右移，不移动屏幕
	WriteCommand(0x0c);	// 开显示 不显示光标
	WriteCommand(0x01);	// 清屏
	WriteCommand(0x80); // 起始显示点，第一个行第一个位置
}

char str1[] = "hello lcd1602";
char str2[] = "Hello LCD1602";

void main()
{
	unsigned char i=0;
	LCD1602Init();
	for (i=0;i<16;i++){
		if(str1[i]=='\0'){	// 可以有效防止显示乱码
			break;
		}
		WriteData(str1[i]);
	}
	WriteCommand(0x80+0x40);	// 第二行
	for (i=0;i<16;i++){
		if(str2[i]=='\0'){
			break;
		}
		WriteData(str2[i]);
	}
	while(1);
}