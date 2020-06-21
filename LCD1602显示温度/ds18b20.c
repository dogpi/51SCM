#include "ds18b20.h"



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
