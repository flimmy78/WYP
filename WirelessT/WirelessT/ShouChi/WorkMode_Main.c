#include "HardwareProfile.h"
#include <stdio.h>
void WorkMode_MainProc(void)
{
	unsigned char keynum;
	Si4463_SDN_W=1;
	if(KeyPollCheck()==1)
	{	
		keynum=GetKeyNum();
		if(keynum!=0xff)
		{
			switch(keynum)
			{
			case 1://�������
				WorkMode_AnalyzeInit();
				break;
			case 2://ʵʱ�ɼ�
				WorkMode_RealTimeInit();
				break;
			case 3://��ʷ����
				WorkMode_HistoryInit(0xff);
				break;
			case 4://ϵͳ����
				WorkMode_SettingInit();
				break;
			}
		}
		else
		{
			CommonKeyFun();
		}
// 		if(KeyValue.u16==0x010b)
// 		{
// 			//gTest=0;
// 			//Display_ShiShiCaiJi();				
// 		}
// 		else
// 		{
// 			gFlags.bReKey=1;
// 			x=x+12;
// 			Display_ShowTemp(x);
// 		}
	}
}
