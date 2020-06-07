/**************************************************************************************
*		              LED点阵--显示汉字实验												  *
实现现象：下载程序后，按照光盘内操作视频接线，LED点阵显示汉字

			
注意事项：必须将74HC595模块的JP595短接片短接，否则不会出现现象																				  
***************************************************************************************/

#include "reg51.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include "intrins.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

//--定义使用的IO口--//
sbit SRCLK=P3^6;
sbit RCLK=P3^5;
sbit SER=P3^4;

//定义LED点阵的位选，也就是类似于数码管的位选，因为要对其动态扫描操作
//数组前16位和后16位数据正好是相反的，也就是说先让第3个595输出低电平，然后再让第4个595输出低电平

u8 code ledwei[]=
{
0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
};



u8 code ledduan1[]=
{
/*--  文字:  普  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x40,0x44,0x54,0x64,0x45,0x7E,0x44,0x44,0x44,0x7E,0x45,0x64,0x54,0x44,0x40,0x00,
0x00,0x00,0x00,0xFF,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,0x00,0x00,
};

u8 code ledduan2[]=
{
/*--  文字:  中  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0xF0,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00,
0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x0F,0x00,0x00,0x00,
};



/*******************************************************************************
* 函 数 名         : delay
* 函数功能		   : 延时函数，i=1时，大约延时10us
*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}

/*******************************************************************************
* 函 数 名         : Hc595SendByte(u8 dat1,u8 dat2,u8 dat3,u8 dat4)
* 函数功能		   : 通过595发送四个字节的数据
* 输    入         : dat1：第4个595输出数值
*                  * dat2: 第3个595输出数值
*                  * dat3：第2个595输出数值
*                  * dat4：第1个595输出数值
* 输    出         : 无
*******************************************************************************/
void Hc595SendByte(u8 dat1,u8 dat2,u8 dat3,u8 dat4)
{
	u8 a;

	SRCLK = 1;
	RCLK = 1;

	for(a=0;a<8;a++)		 //发送8位数
	{
		SER = dat1 >> 7;		 //从最高位开始发送
		dat1 <<= 1;

		SRCLK = 0;			 //发送时序
		_nop_();
		_nop_();
		SRCLK = 1;	
	}

	for(a=0;a<8;a++)		 //发送8位数
	{
		SER = dat2 >> 7;		 //从最高位开始发送
		dat2 <<= 1;

		SRCLK = 0;			 //发送时序
		_nop_();
		_nop_();
		SRCLK = 1;	
	}

	for(a=0;a<8;a++)		 //发送8位数
	{
		SER = dat3 >> 7;		 //从最高位开始发送
		dat3 <<= 1;

		SRCLK = 0;			 //发送时序
		_nop_();
		_nop_();
		SRCLK = 1;	
	}

	for(a=0;a<8;a++)		 //发送8位数
	{
		SER = dat4 >> 7;		 //从最高位开始发送
		dat4 <<= 1;

		SRCLK = 0;			 //发送时序
		_nop_();
		_nop_();
		SRCLK = 1;	
	}

	RCLK = 0;
	_nop_();
	_nop_();
	RCLK = 1;
}


/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	u8 i;
	
	while(1)
	{	 
		for(i=0;i<16;i++)
		{
			Hc595SendByte(~ledwei[i+16],~ledwei[i],ledduan1[16+i],ledduan1[i]);
			delay(10);
		}		
	}		
}
