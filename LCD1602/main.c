#include "reg51.h"
#include "intrins.h"

/*
	LCD1602�ܽŽ���
	1��VSS ��Դ��
	2��VDD ��Դ����
	3��VL	 Һ����ʾƫѹ�źţ������Աȶ�
	4��RS	 ����/���� ѡ��� ������H ����L��
	5��R/W ��/д     ѡ��� ����H   дL��   ֱ�ӽӵأ�һֱд
	6��E	 ʹ���ź�
	7��D0
	8��D1
	9��D2
	10��D3
	11��D4
	12��D5
	13��D6
	14��D7
	15��BLA ��������
	16��BLK ���⸺��
	
	���
		RS=0;
		R/W=0
		data:
			0x01: ����
			
			0x04: д�������ݺ�������  ��ʾ�����ƶ�
			0x05: д�������ݺ�������  ��ʾ����������1���ַ�
			0x06: д�������ݺ�������  ��ʾ�����ƶ�
			0x07: д�������ݺ�������  ��ʾ����������1���ַ�
		
			0x08: ��ʾ�����ܹ� �޹�� �����˸
			0x09: ��ʾ�����ܹ� �޹�� ��겻��˸
			0x0a: ��ʾ�����ܹ� �й�� �����˸
			0x0b: ��ʾ�����ܹ� �й�� ��겻��˸
			0x0c: ��ʾ�����ܿ� �޹�� �����˸
			0x0d: ��ʾ�����ܿ� �޹�� ��겻��˸
			0x0e: ��ʾ�����ܿ� �й�� �����˸
			0x0f: ��ʾ�����ܿ� �й�� ��겻��˸
		
		DDRAM:
			��һ�У�00H 01H 02H 03H 04H 05H 06H ...... 27H  // 00-0fΪ��ʾλ  ����Ϊ������
			�ڶ��У�40H 41H 42H 43H 44H 45H 46H ...... 67H  // 40-4fΪ��ʾλ  ����Ϊ������
			// ��ʵ�ֹ���ʱ����ʹ�û�����ʵ��
			// ��ʹ��ʱ ��Щ��ַҪ ��0x80
	
*/

#define DATAPORT P0

sbit E = P2^7;
sbit RS = P2^6;
sbit RW = P2^5;

void delay1ms(void)   //��� 0us
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=142;b>0;b--)
            for(a=2;a>0;a--);
}


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

//////busy����1�����򷵻�0//////
char IsBusy()
{
	
	return 0;
}



void LCD1602Init()
{
	WriteCommand(0x38);	// 8λ�������� 2����ʾ 5*7����/ÿ�ַ�
	WriteCommand(0x06); // д�������ƣ����ƶ���Ļ
	WriteCommand(0x0c);	// ����ʾ ����ʾ���
	WriteCommand(0x01);	// ����
	WriteCommand(0x80); // ��ʼ��ʾ�㣬��һ���е�һ��λ��
}

char str1[] = "hello lcd1602";
char str2[] = "Hello LCD1602";

void main()
{
	unsigned char i=0;
	LCD1602Init();
	for (i=0;i<16;i++){
		if(str1[i]=='\0'){	// ������Ч��ֹ��ʾ����
			break;
		}
		WriteData(str1[i]);
	}
	WriteCommand(0x80+0x40);	// �ڶ���
	for (i=0;i<16;i++){
		if(str2[i]=='\0'){
			break;
		}
		WriteData(str2[i]);
	}
	while(1);
}