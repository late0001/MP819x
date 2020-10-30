
#include "stdafx.h"
//#include "mp8723xBT.h"
#include "mp8723xBT_DllHeader.h"

#if 0

//define dll function variable
hRTKBT_OpenAdapter		_RTKBT_OpenAdapter;
hRTKBT_CloseAdapter		_RTKBT_CloseAdapter;
hRTKBT_GetLastError   		_RTKBT_GetLastError;
hRTKBT_ClearCounter		_RTKBT_ClearCounter;
hRTKBT_GetStatistics		_RTKBT_GetStatistics;
hRTKBT_RadioOff			_RTKBT_RadioOff;
hRTKBT_SetTRxParameters	_RTKBT_SetTRxParameters;
hRTKBT_ShowUI			_RTKBT_ShowUI;
hRTKBT_WriteReg			_RTKBT_WriteReg;
hRTKBT_ReadReg			_RTKBT_ReadReg;
hRTKBT_HCIReset			_RTKBT_HCIReset;
hRTKBT_BTTestMode		_RTKBT_BTTestMode;
hRTKBT_StartPacketTx		_RTKBT_StartPacketTx;
hRTKBT_StartContinueTx		_RTKBT_StartContinueTx;
hRTKBT_StartRx			_RTKBT_StartRx;
hRTKBT_StopTest			_RTKBT_StopTest;
hRTKBT_QueryTRxStatus		_RTKBT_QueryTRxStatus;
hRTKBT_ReadThermalMeter	_RTKBT_ReadThermalMeter;
hRTKBT_TxPowerAdjust		_RTKBT_TxPowerAdjust;
hRTKBT_SendHCICmd		_RTKBT_SendHCICmd;

HMODULE hRTKBT;

int LoadRTKBTDLL()
{

  //specify your file route for this dll
  hRTKBT = LoadLibrary("mp8723xBT.dll");

	  if (hRTKBT == NULL) 
	   return 0;

	_RTKBT_OpenAdapter		=         (hRTKBT_OpenAdapter) 		GetProcAddress(hRTKBT,"@RTKBT_OpenAdapter$qucucui");
	_RTKBT_CloseAdapter		=         (hRTKBT_CloseAdapter)		GetProcAddress(hRTKBT,"@RTKBT_CloseAdapter$qv");
	
      	_RTKBT_GetLastError			=         (hRTKBT_GetLastError)   		GetProcAddress(hRTKBT,"@RTKBT_GetLastError$qv");
	_RTKBT_ClearCounter              =         (hRTKBT_ClearCounter)		GetProcAddress(hRTKBT,"@RTKBT_ClearCounter$qv");
	_RTKBT_GetStatistics          	=         (hRTKBT_GetStatistics)		GetProcAddress(hRTKBT,"@RTKBT_GetStatistics$qp20_RTKBT_TRXSTATISTICS");
	_RTKBT_RadioOff                    =         (hRTKBT_RadioOff)			GetProcAddress(hRTKBT,"@RTKBT_RadioOff$qv");
	_RTKBT_SetTRxParameters    =         (hRTKBT_SetTRxParameters)	GetProcAddress(hRTKBT,"@RTKBT_SetTRxParameters$q15_RTKBT_TRXPARAM");
	_RTKBT_ShowUI                   	=         (hRTKBT_ShowUI)			GetProcAddress(hRTKBT,"@RTKBT_ShowUI$qo");
	_RTKBT_WriteReg                 	=         (hRTKBT_WriteReg)			GetProcAddress(hRTKBT,"@RTKBT_WriteReg$quiuiuiui");
	_RTKBT_ReadReg                  	=         (hRTKBT_ReadReg)			GetProcAddress(hRTKBT,"@RTKBT_ReadReg$quiuiui");
	_RTKBT_HCIReset                 	=         (hRTKBT_HCIReset)			GetProcAddress(hRTKBT,"@RTKBT_HCIReset$qv");
	_RTKBT_BTTestMode               =         (hRTKBT_BTTestMode)		GetProcAddress(hRTKBT,"@RTKBT_BTTestMode$quc");
	_RTKBT_StartPacketTx            =         (hRTKBT_StartPacketTx)		GetProcAddress(hRTKBT,"@RTKBT_StartPacketTx$qv");
	_RTKBT_StartContinueTx        =         (hRTKBT_StartContinueTx)	GetProcAddress(hRTKBT,"@RTKBT_StartContinueTx$qv");
	_RTKBT_StartRx                  	=         (hRTKBT_StartRx)			GetProcAddress(hRTKBT,"@RTKBT_StartRx$qv");
	_RTKBT_StopTest                 	=         (hRTKBT_StopTest)			GetProcAddress(hRTKBT,"@RTKBT_StopTest$qv");
	_RTKBT_QueryTRxStatus        =         (hRTKBT_QueryTRxStatus)	GetProcAddress(hRTKBT,"@RTKBT_QueryTRxStatus$qv");
	_RTKBT_ReadThermalMeter   =         (hRTKBT_ReadThermalMeter)	GetProcAddress(hRTKBT,"@RTKBT_ReadThermalMeter$qv");
	_RTKBT_TxPowerAdjust          =         (hRTKBT_TxPowerAdjust)		GetProcAddress(hRTKBT,"@RTKBT_TxPowerAdjust$quc");
	_RTKBT_SendHCICmd              =         (hRTKBT_SendHCICmd)		GetProcAddress(hRTKBT,"@RTKBT_SendHCICmd$qusucpucuc");

  atexit(UnLoadRTKBTDLL);

  return 1;
}

void UnLoadRTKBTDLL()
{
   FreeLibrary(hRTKBT);
}
#endif
