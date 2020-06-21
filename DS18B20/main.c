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

void delay500us(void)   //��� 0us
{
    unsigned char a,b;
    for(b=71;b>0;b--)
        for(a=2;a>0;a--);
}


void delay80us(void)   //��� 0us
{
    unsigned char a,b;
    for(b=11;b>0;b--)
        for(a=2;a>0;a--);
}

void delay60us(void)   //��� 0us
{
    unsigned char a,b;
    for(b=3;b>0;b--)
        for(a=8;a>0;a--);
}
void delay45us(void)   //��� 0us
{
    unsigned char a;
    for(a=21;a>0;a--);
}

void delay15us(void)   //��� 0us
{
    unsigned char a;
    for(a=6;a>0;a--);
}

void delay10us(void)   //��� 0us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=2;a>0;a--);
}


unsigned char DS18B20Init()
{
	unsigned char i;
	// ����������������
	DSPORT=0;
	delay500us();	// 480-960us
	DSPORT=1;
	delay80us();	// ����̫��������������͵�ʱ�䣬�ᱻ�����������ߣ���ô���������жϿ϶�ʧ�ܡ�
	// �ж�DS18B20�Ƿ���������������Ӧ
	// �����ʼ���ɹ� ����15-60us�����ͣ�������60-240us
	while(DSPORT)
	{
		delay10us();
		i++;
		if(i>500){
			return 0;	// 5msδ��Ӧ
		}
	}
	return 1;
}

void DS18B20WriteByte(unsigned char value)
{
	unsigned char i=0;

	for(i=0;i<8;i++){
		// д����ǰ��������15us
		DSPORT=0;
		_nop_();
		DSPORT = value&0x01;		// С��д

		// д��һλ ��ʱ����60us ����
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
		// �������������� ��ʱ>1us
		DSPORT=0;
		_nop_();
		_nop_();
		_nop_();
		// �������������� ��ʱ10us
		DSPORT=1;
		delay10us();
		
		bi= DSPORT;
		byte = (byte>>1)|(bi<<7);	// С�˶�  
		/*��EEPROM���෴��EEPROM�Ǵ�˶�д 
		EEPROM��дҪô�ö��ô�ˣ�Ҫô����С�ˣ���Ϊ�������Լ�д��,
		ֻҪ���ֶ�д��һ�¼��ɡ�
		��DS18B20������������DS18B20�����ģ����������ߴ���ʱҲ��С�ˣ����������ȡҪ��С�ˡ�
		*/
		delay60us();
	}

	return byte;
}

// ת���¶�
void DS18B20ChangeTemp()
{
	DSPORT=1;
	DS18B20Init();
	delayXms(2);
	DS18B20WriteByte(0xcc);
	DS18B20WriteByte(0x44);
}

// ��ȡ�¶� ����
void DS18B20SendReadComm()
{
	DSPORT=1;
	DS18B20Init();
	delayXms(2);
	DS18B20WriteByte(0xcc);
	DS18B20WriteByte(0xbe);
}

// ��ȡ�¶�ֵ
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

// �������ʾ������        0     1    2     3    4   5    6    7    8   9
unsigned char num[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char NG = 0x40;
// ���Ҫ���С�����ֱ���ڸ�λ�ϼ�0x80
// ���ڴ��д������ݵ�ֵ ����Ϊ4λ
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
	show[1] = num[Z%100/10]; //ʮλ
	show[2] = num[Z%100%10]+0x80; //��λ
	show[3] = num[F]; //ʮ��λλ
}
void ShowOnDevice()
{
	unsigned char i=0;
	for(i=0;i<4;i++){
		// ѭ�����ĸ���������Ҫ��ʾ����
		switch(i){
			case 0:
				LSA=0;LSB=0;LSC=0;break;	// ��ʾ��0λ
			case 1:
				LSA=1;LSB=0;LSC=0;break;	// ��ʾ��1λ
			case 2:
				LSA=0;LSB=1;LSC=0;break;	// ��ʾ��2λ
			case 3:
				LSA=1;LSB=1;LSC=0;break;	// ��ʾ��3λ
		}
		P0=show[i];
		delay500us();
		P0=0x00;	// ������ ����
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