#ifndef __WirelessProc_h__
#define __WirelessProc_h__

unsigned char WirelessRxList[64];
unsigned char WirelessRxCount;
#define RSSI_SAMPLES 10
unsigned char Si4432_RSSI (void);
void Wireless_RxInit(void);
//void Wireless_WithShouChi(void);
unsigned char Wireless_RxProc(void);
unsigned char Wireless_TxProc(unsigned char Command,const unsigned char* pTx,unsigned char TxCount);
#endif // __WirelessProc_h__
