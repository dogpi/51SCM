#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "reg51.h"
#include "intrins.h"
#include "delay.h"

sbit DSPORT = P3^7;

unsigned char DS18B20Init();
void DS18B20WriteByte(unsigned char value);
unsigned char DS18B20ReadByte();
void DS18B20ChangeTemp();
void DS18B20SendReadComm();
float DS18B20ReadTemp();

#endif