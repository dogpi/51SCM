/**************************************************************************************
*		              继电器实验												  *
实现现象：下载程序后继电器模块指示灯DK亮，会听到继电器吸合的声音
注意事项：无																				  
***************************************************************************************/

#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

sbit relay=P1^4;	   


/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	relay=0;
	while(1);
}
