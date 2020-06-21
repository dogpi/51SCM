#include "lcd1602.h"

#include "reg51.h"
#include "intrins.h"



void WriteCommand(unsigned char comm)
{
	E = 0;	// EΪ�ߵ�ƽʱ�Ὣ�����ϵ�����д��Ĵ��� 
	RS = 0;	// ��������
	RW = 0;	// д
	DATAPORT=comm;
	delay1ms();
		
	E = 1;	// д��
	delay1ms();
	E = 0;
}


void WriteData(unsigned char value)
{
	E = 0;
	RS = 1;	// ��������
	RW = 0; // д
	
	DATAPORT = value;
	delay1ms();
	E = 1;
	delay1ms();
	E = 0;
}


void LCD1602Init()
{
	WriteCommand(0x38);	// 8λ�������� 2����ʾ 5*7����/ÿ�ַ�
	WriteCommand(0x06); // д�������ƣ����ƶ���Ļ
	WriteCommand(0x0c);	// ����ʾ ����ʾ���
	WriteCommand(0x01);	// ����
	WriteCommand(0x80); // ��ʼ��ʾ�㣬��һ���е�һ��λ��
}
