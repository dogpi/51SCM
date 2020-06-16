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

// �������ʾ������        0     1    2     3    4   5    6    7    8   9
unsigned char num[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

// ���ڴ��д������ݵ�ֵ ����Ϊ4λ
unsigned char show[4];

// ��ʱ10us
void delay10us(void)   //��� 0us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=2;a>0;a--);
}

void delay(unsigned int i)
{
	while(i--);	
}

// ��ʼ�ź� SCLΪ�ߵ�ƽ SDA�ɸߵ�ƽתΪ�͵�ƽ
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

// ��ֹ�ź� SCLΪ�ߵ�ƽ SDA�ɵ͵�ƽתΪ�ߵ�ƽ
void End()
{
	SDA=0;
	delay10us();
	SCL=1;
	delay10us();
	SDA=1;
	delay10us();
}


// �ӻ�Ӧ�������������ݳɹ�  SCLΪ�ߵ�ƽ SDA�ᱻ�ӻ��ɸߵ�ƽ��Ϊ�͵�ƽ
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
			// �������2m��û�����ͣ�����ʧ�ܻ�Ϊ��Ӧ��
			SCL=0;
			delay10us();
			return 0;
		}
	}
	SCL=0;
	delay10us();
	return 1;
}


// ������ӻ�����ACK


// ��������ʱ��SDA������SCL�ߵ�ƽʱ�ı��ƽ
unsigned char WriteByte(unsigned char value)
{
	SCL=0;
	delay10us();
	unsigned char i=0,j=0;
	for(i=0;i<8;i++){
		SDA=value>>7;
		value=value<<1;
		delay10us();
		SCL=1;		// �޸�SDA��������SCL SCL������ʱд������
		delay10us();	// ��ʱ ����д���ݵ�iic
		SCL=0;		// �´�ѭ���޸�������Ч
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
//			// �������2m��û�����ͣ�����ʧ�ܻ�Ϊ��Ӧ��
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
		value<<=1;	// ��ȡ��һλʱ data=0 data<<1=0
		value|=SDA;// ��һλ��ֵ
		delay10us();
		SCL=0;		// SCL�½���ʱ��ȡ����
		delay10us();
	}
	return value;
}

void Write24C02(unsigned char addr,unsigned char value)
{
	Start();
	WriteByte(0xa0);	// A0=0 A1=0 A2=0 W=0���ӵأ�����EEPROM�ĵ�ַΪ0xa0+0+0
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
		// д����
		delay(1000);
		if(0==k1){
			Write24C02(0x01,show_value);	// ���ַ��д������
		}
		while(!k1);	// ����̧�� 
	}
	if(0==k2){
		// ������
		delay(1000);
		if(0==k2){
			show_value=Read24C02(0x01);
		}
		while(!k2);	// ����̧�� 
	}
	if(0==k3){
		// ��ʾ���ݼ�
		delay(1000);
		if(0==k3){
			show_value++;
			if(show_value>255){
				show_value=0;
			}
		}
		while(!k3);	// ����̧�� 
	}
	if(0==k4){
		// ��ʾ������0
		delay(1000);
		if(0==k4){
			show_value=0;
		}
		while(!k4);	// ����̧�� 
	}
}

void ShowValue(unsigned char value)
{
	show[0] = num[value/1000];	//ǧλ
	show[1] = num[value%1000/100]; //��λ
	show[2] = num[value%1000%100/10]; //ʮλ
	show[3] = num[value%1000%100%10]; //��λ
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
		delay(100);
		P0=0x00;	// ������ ����
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