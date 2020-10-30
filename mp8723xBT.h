
#ifndef RTKBT_DYNAMIC_EXPORTS
#define RTKBT_DYNAMIC_EXPORTS
#endif

#ifdef RTKBT_DYNAMIC_EXPORTS
#define RtkBTAPI __declspec(dllexport)  //dynamic import
#else
#define RtkBTAPI __declspec(dllimport)  //static import
#endif


typedef struct _RTKBT_TRXSTATISTICS
{
	double   RxBits;            
	double   ErrorBits;
	double   RxBER;			// in %
	double   RSSI;			//in dBm
	double   CFO;				//in KHz
	double   SignalQuality;
	double   TxBits;			
} RTKBT_TRXSTATISTICS;


typedef struct _RTKBT_TRXPARAM
{
	UINT8      Rate;                 		// 1~3       
	UINT8      Channel;           		// 0~78
	UINT32    TxPktCnt;        		// 0: continue packet Tx 
	UINT8      TxPKtInterval;		//0~15-> 100ns~1600ns
	UINT8      PayloadType;      	 //0: ("All 0's"), 1:("All 1's"), 2:("0101"), 3:("1010"), 4:("0x0~0xf")  
	UINT16    PayloadLength;  		 // in bits, 1 ~ 8191
	UINT32    PacketHeader;            //0x0 ~ 0x3ffff
	UINT8      WhitenCoeff;		 // 0x0~ 0x7f, Whitening disable if out of this range
	char        BDAddress[12];           //12 hex digits
} RTKBT_TRXPARAM;


enum _ErrorCode
{
	eNoError,
	eAdapterInitialFail,	
	eInvalidHostType,	
	eInvalidUSBPortNumber,
	eInvalidUARTPortNumber,
	eInvalidPCIePortNumber,
	eInvalidUARTBaudRate,
	eNotRTKBTChip,
	eInvalidDataRate,
	eInvalidTxPktInterval,
	eInvalidChannelNumber,
	eInvalidPayloadType,
	eInvalidPayloadLength,
	eInvalidPacketHeader,
	eInvalidBDAddress,
	eInvalidRegsiterType,
	eInvalidTxIndex,
	eHCIResetFail,
	eEnableDUTModeError,
	eRedundancyTestAction,
	ePacketTxFail,
	eContinueTxFail,
	ePacketRxFail,
	eStopFail,
	eCFOAdjustFail,
	eUnknowError	
};


#ifdef __cplusplus  
 extern "C" {                                      
#endif

#ifndef RTKBT_DYNAMIC_EXPORTS   //for static import

bool RtkBTAPI RTKBT_OpenAdapter(UINT8 HostType, UINT8 PortNo, UINT32 Rate);
bool RtkBTAPI RTKBT_CloseAdapter();
UINT32 RtkBTAPI  RTKBT_GetLastError(); 
bool RtkBTAPI RTKBT_ClearCounter();
bool RtkBTAPI RTKBT_GetStatistics(RTKBT_TRXSTATISTICS* statistics);
bool RtkBTAPI RTKBT_RadioOff();
bool RtkBTAPI RTKBT_SetTRxParameters(RTKBT_TRXPARAM params);
bool RtkBTAPI RTKBT_ShowUI(bool IsShow);
bool RtkBTAPI RTKBT_WriteReg(UINT32 type, UINT32 RegAddr, UINT32 RegValue, UINT32 bitmask);
UINT32 RtkBTAPI RTKBT_ReadReg(UINT32 type, UINT32 RegAddr, UINT32 bitmask);
bool RtkBTAPI RTKBT_HCIReset();
bool RtkBTAPI RTKBT_BTTestMode();
bool RtkBTAPI RTKBT_StartPacketTx();
bool RtkBTAPI RTKBT_StartContinueTx();
bool RtkBTAPI RTKBT_StartRx();
bool RtkBTAPI RTKBT_StopTest();
bool RtkBTAPI RTKBT_QueryTRxStatus();
UINT8 RtkBTAPI RTKBT_ReadThermalMeter();
bool RtkBTAPI RTKBT_TxPowerAdjust(UINT8  CorseTuneIndex, UINT8 FineTuneIndex) ;  //CorseTuneIndex: TBD, FineTuneIndex:1~9
bool RtkBTAPI RTKBT_CFOAdjust(UINT8  index);
UINT8*	RtkBTAPI RTKBT_SendHCICmd(UINT16 nOpcode, UINT8 nLength, UINT8* lpParam, UINT8 nEventcode);

#endif


#ifdef __cplusplus  
	}                                     
#endif


