#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "reg51.h"
#include "intrins.h"
#include "delay.h"

#define DATAPORT P2
sbit E = P1^0;
sbit RS = P1^2;
sbit RW = P1^1;

//#define DATAPORT P0
//sbit E = P2^7;
//sbit RS = P2^6;
//sbit RW = P2^5;

void WriteCommand(unsigned char com);
void WriteData(unsigned char value);
void LCD1602Init();

#endif