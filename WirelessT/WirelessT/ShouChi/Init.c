#include "HardwareProfile.h"
#include <stdio.h>
void InitMCU(void)             //ϵͳ��ʼ������
{
	PLLEN=1;
	__delay_ms(40);
	//AD��ʼ��	
	ADCON1=0b00001101;  //ȫ��Ϊ���ݿ�
	ADCON0=0b00000100;
	ADCON2=0b10111110;
	CMCON=0x07;
	Si4463_SDN_DIR=0;
	Si4463_NIRQ_DIR=1;
	Si4463_SDN_W=1;//si4432��λ

	SPIROM_HOLD_W=1;
	SPIROM_HOLD_DIR=0;

	TRISD=0x00;
	TRISE |= 0x0f;
	TRISC2=1;
	
	NewON_W=1;
	NewON_DIR=0;
	NewOFF_DIR=1;
	IPEN=1;
	/************************************************************************/
	/* ������ʼ��                                                           */
	/************************************************************************/
	//RDPU=0;//PORTD�ر�������

	/************************************************************************/
	/* TMR0 ��ʾ��ʱ��90ms                                                  */
	/************************************************************************/
	T0CON=0b00000010;
	WRITETIMER0(20536);
	TMR0IP=0;
	TMR0IF=0;
	TMR0IE=1;	
	/************************************************************************/
	/* TMR1 ϵͳ���ģ�100ms                                                 */
	/************************************************************************/
	T1CON=0b10110000;//1:8,2us
	WRITETIMER1(15536);//100ms
	TMR1ON=1;
	TMR1IP=0;//�����ȼ�
	TMR1IE=1;//ʹ���ж�
	TMR1IF=0;//���жϱ�־
	/************************************************************************/
	/* �ⲿ�ж�1                                                            */
	/************************************************************************/
	TRISB0=1;
	INTEDG0=0;
	//INT0IP=1;
	INT0IF=0;
	INT0IE=1;
	/************************************************************************/
	/* ����1																*/
	/************************************************************************/
	TRISC7=1;
	TRISC6=1;
	TXSTA1=0b00100110;//�첽��ʽ��ʹ�ܷ���(bit6=1��9λ����)��λ�ֳ����߲�����(bit2����0)
	RCSTA1=0b10010000; //ʹ�ܴ��ڡ�ʹ�ܽ��ա��첽��ʽ��(bit6=1��9λ����)��λ�ֳ�	
	BAUDCON1=0x08;//�����Զ��������Զ����
	SPBRG1=34;	//Baud Rate = Fosc/(4(SPBRG+1)) = 115200
	RCIP=0;//�����ȼ�
	RCIE=0;//ʹ���ж�
	RCIF=0;//���жϱ�־
	/************************************************************************/
	/* ����2                                                                */
	/* ���ͣ�����װ��TXREG��ʼ����                                        */
	/* ���գ����յ����RCREG�ж�����                                        */
	/************************************************************************/
	TRISG2=1;
	TRISG1=0;
	TXSTA2=0b00100110;//�첽��ʽ��ʹ�ܷ��͡���λ�ֳ����߲�����
	RCSTA2=0b10010000; //ʹ�ܴ��ڡ�ʹ�ܽ��ա��첽��ʽ����λ�ֳ�
	SPBRG2=0x67;	//Baud Rate = Fosc/(16(SPBRG+1)) = 9600
	RC2IP=0;//�����ȼ�
	RC2IE=1;//ʹ���ж�
	RC2IF=0;//���жϱ�־

	PEIE=1;
	GIE=1;
}
void InitVariable(void)
{
	unsigned char i;
	
	SystemTick=0;	
	for(i=0;i<MaxWithFloorRx;i++)
		WithFloorRxList[i]=0;
	ROMParams.WirelessParamIndex=1;
	WorkMode=WorkMode_Main;
}
void InitSystem( void )
{
	unsigned char x,j,k;
	unsigned long addr1;
	_SensorData SensorData;
	InitMCU();
	InitVariable();		
	DisplayInit();
 	SPIROM_Init();
 	FRAM_GetROMParams();
	DS1302_Init();
	DS1302_Switch(1);
	DS1302_SetHour24(1);
	DS1302_GetTime();
	Si4463_Init();
	CP1616_Client_Init();
	CP1616_Client_Addr.u16=0x0001;
	Display_BK(0xff);
	gFlags.bSleep=0;
	gFlags.bSleep1=0;
	Display_BK(80);
// 	SensorDataInfo.bLoop=1;
// 	SensorDataInfo.curIndex=0;
// 	sprintf(SensorDataInfo.InstallPosition,"1234567890");
// 	for(x=0;x<100;x++)
// 	{
// 		if(addr1>0x0001f614)
// 			addr1=x;
// 		addr1=(unsigned long)x*SensorDataListCount;
// 		addr1=addr1+Sensor1DataAddr;
// 		SPIROM_WriteArray(addr1,SensorDataInfo.All,SensorDataInfoCount);
// 		addr1+=SensorDataInfoCount;
// 		for(k=0;k<210;k++)
// 		{
// 			for(j=0;j<6;j++)
// 				SensorData.All[j]=k;
// 			SPIROM_WriteArray(addr1,SensorData.All,6);
// 			addr1+=6;
// 		}		
// 	}
// #ifndef Proteus		
// 	Wireless_RxInit();
// 	
// 	Si4463_FIFO_INFO(rx);
// 	Si4463_INT_STATUS(rx);
// 	Si4463_PART_INFO(rx);
// 	Si4463_START_RX(1,0x00,3,0x08,0x08,0x08);
// #endif
}
