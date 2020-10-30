#include "mp8723xBT.h"

//define file pointer
typedef bool 		(*hRTKBT_OpenAdapter)(UINT8 HostType, UINT8 PortNo, UINT32 Rate); 
typedef bool 		(*hRTKBT_CloseAdapter)();
typedef UINT32 	(*hRTKBT_GetLastError)();
typedef bool    	(*hRTKBT_ClearCounter)();
typedef bool    	(*hRTKBT_GetStatistics)(RTKBT_TRXSTATISTICS* statistics);
typedef bool     	(*hRTKBT_RadioOff)();
typedef bool		(*hRTKBT_SetTRxParameters)(RTKBT_TRXPARAM params);
typedef bool		(*hRTKBT_ShowUI)(bool IsShow);
typedef bool		(*hRTKBT_WriteReg)(UINT32 type, UINT32 RegAddr, UINT32 RegValue, UINT32 bitmask);
typedef UINT32 	(*hRTKBT_ReadReg)(UINT32 type, UINT32 RegAddr, UINT32 bitmask);
typedef bool		(*hRTKBT_HCIReset)();
typedef bool		(*hRTKBT_BTTestMode)(UINT8 Type);
typedef bool		(*hRTKBT_StartPacketTx)();
typedef bool		(*hRTKBT_StartContinueTx)();
typedef bool		(*hRTKBT_StartRx)();
typedef bool		(*hRTKBT_StopTest)();
typedef bool		(*hRTKBT_QueryTRxStatus)();
typedef UINT8	(*hRTKBT_ReadThermalMeter)();
typedef bool		(*hRTKBT_TxPowerAdjust)(UINT8 FineTuneIndex);
typedef UINT8*	(*hRTKBT_SendHCICmd)(UINT16 nOpcode, UINT8 nLength, UINT8* lpParam, UINT8 nEventcode);

int LoadRTKBTDLL();
void UnLoadRTKBTDLL();











