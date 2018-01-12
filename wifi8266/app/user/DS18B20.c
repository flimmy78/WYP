/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2014/1/1, v1.0 create this file.
*******************************************************************************/

#include "ets_sys.h"
#include "osapi.h"

#include "..\..\include\os_type.h"
#include "..\..\include\eagle_soc.h"
#include "..\..\include\gpio.h"
#include "DS18B20.h"
#include "TypeDefine.h"

LOCAL os_timer_t DS18B20_timer;
#define DS12B20_PinNum 2
#define  DS18B20_Pin GPIO_ID_PIN(DS12B20_PinNum)
#define  ONEWIRE_GETIN         GPIO_INPUT_GET(DS18B20_Pin)
#define  ONEWIRE_SETLOW        GPIO_OUTPUT_SET(DS18B20_Pin, 0)
#define  ONEWIRE_SETHIGH        GPIO_OUTPUT_SET(DS18B20_Pin, 1)
//---------------------------------------
//���ƣ���λDS18B20����
//---------------------------------------
#define TIMESFORRETRY_OneWire 5

unsigned char ICACHE_FLASH_ATTR DS18B20_Reset(void)
{
	unsigned char r,Retry,r1;
	r=1;
	Retry=TIMESFORRETRY_OneWire;
	while(Retry--)
	{
		ONEWIRE_SETLOW;
		os_delay_us(600);     //����DQ���߿�ʼ��λ
		ONEWIRE_SETHIGH;//�ȵ����������߲�����15��60us
		//di();
		os_delay_us(65);      //�ͷ�DQλ
		r=ONEWIRE_GETIN;//����Ӧ���ź�
		//ei();		
		os_delay_us(500);//��ʱ60��240us
		r1=ONEWIRE_GETIN;
		if(r1==0)
			r=1;
		if(r==0)
			break;
	}	
	return r;
}
 
//---------------------------------------
//���ƣ���һ�ֽں���
//---------------------------------------
unsigned char ICACHE_FLASH_ATTR DS18B20_ReadByte()
{
	unsigned char i;
	unsigned char b;
	unsigned char value=0;
	for(i=0;i<8;i++)
	{		
		ONEWIRE_SETLOW;
		os_delay_us(1);		
		ONEWIRE_SETHIGH;
		os_delay_us(12);
		b=ONEWIRE_GETIN;		
		os_delay_us(60);
		//b=OneWire_ReadBit();     //����λ����
		if(b!=0)               //����� 1 ��1
		{
			value|=(0x01<<i);  //�ȶ���λ���ٶ���λ
			//__delay_us(1);
		}
	}                         //������ 0
	return(value); 
}

void ICACHE_FLASH_ATTR DS18B20_WriteByte(unsigned char val)
{
	unsigned char i;
	unsigned char temp;
	for (i=0;i<8;i++)
	{
		temp=val>>i;
		temp&=0x01;
		ONEWIRE_SETLOW;
		os_delay_us(2);
		if (temp==1)  
		{
			ONEWIRE_SETHIGH;
		}
		os_delay_us(75);
		ONEWIRE_SETHIGH;
	}
}
 
//---------------------------------------
//���ƣ�DS18B20�¶�ת������
//---------------------------------------
unsigned char ICACHE_FLASH_ATTR DS18B20_Convert()
{
	if(DS18B20_Reset()) //**
		return DS18X20_ERROR;
	DS18B20_WriteByte(0xcc);			// to all devices
	DS18B20_WriteByte(0x44);
	return DS18X20_OK;
}
#define CRC8INIT	0x00
#define CRC8POLY	0x18              //0X18 = X^8+X^5+X^4+X^0
unsigned char ICACHE_FLASH_ATTR GetVerify_CRC8( unsigned char* pBuff, unsigned int Count )
{
	unsigned char crc;
	unsigned int i;
	unsigned char bit_counter;
	unsigned char data;
	unsigned char feedback_bit;

	crc = CRC8INIT;

	for(i = 0; i != Count; i++)
	{
		data = pBuff[i];
		bit_counter = 8;
		do 
		{ 
			feedback_bit = (crc ^ data) & 0x01;
			if ( feedback_bit == 0x01 ) 
				crc = crc ^ CRC8POLY;
			crc = (crc >> 1) & 0x7F;
			if ( feedback_bit == 0x01 ) 
				crc = crc | 0x80;
			data = data >> 1;
			bit_counter--;
		}while (bit_counter > 0);
	}
	return crc;
}
//---------------------------------------
//���ƣ�ת������������
//---------------------------------------
unsigned char DS18X20_Scratchpad[9];
#define DS18X20_SP_SIZE 9
unsigned char ICACHE_FLASH_ATTR DS18B20_ReadFlash()
{
	unsigned char i;
	if(DS18B20_Reset()) //**
		return DS18X20_ERROR;
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0xbe);
	for ( i=0 ; i< DS18X20_SP_SIZE; i++ ) 
		DS18X20_Scratchpad[i]=DS18B20_ReadByte();
	if ( GetVerify_CRC8( DS18X20_Scratchpad, DS18X20_SP_SIZE-1 )!=DS18X20_Scratchpad[DS18X20_SP_SIZE-1] ) 
		return DS18X20_ERROR_CRC;	
	sensor_data.DST_L=DS18X20_Scratchpad[0];//LSB
	sensor_data.DST_H=DS18X20_Scratchpad[1];//MSB	
	return DS18X20_OK;
// 
// 
// 	sensor_data.DST_L=DS18B20_ReadByte();//LSB
// 	sensor_data.DST_H=DS18B20_ReadByte();//MSB	
// 	return DS18X20_OK;
}


void ICACHE_FLASH_ATTR DS18B20_init(void)
{
	ONEWIRE_GETIN;
}


