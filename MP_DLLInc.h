#ifndef __MP_DLL_H__
#define __MP_DLL_H__


#include <Windows.h>
#include <cstddef>
#include <basetsd.h>

#ifdef MPDLL_EXPORTS
	#define RTLMP_API __declspec(dllexport) __stdcall
#else
	#define RTLMP_API __declspec(dllimport) __stdcall
#endif

#define PROT_DRV_PREFIX TEXT("\\Device\\mp8185s_")
#define DOSNAMEPREFIX   TEXT("Packet_")

#define	MAX_LINK_NAME_LENGTH		64		///< Define the max adapter name length.
#define	MAX_DESCRIPTION_LENGTH		128		///< Define the max adapter description length.
#define	MAX_ADAPTER_NUM				8
#define MAX_TXGAIN_TABLE_SIZE   7
#define MAX_TXDAC_TABLE_SIZE	5

#define RT_DEVICE_ID_PCI								0x00000000
#define RT_DEVICE_ID_USB								0x00000001
#define RT_DEVICE_ID_SDIO								0x00000002

typedef struct _ADAPTER {
	HANDLE		hFile;									///< The handle that application can use to access the lower actual adapter.
	TCHAR		SymbolicLink[MAX_LINK_NAME_LENGTH];		///< The adapter name.
	TCHAR		Description[MAX_DESCRIPTION_LENGTH];	///< The adapter description.
	GUID		nicGuid;
	UINT		ChipID;
	UINT		ChipVer;
	UINT		busType;
} ADAPTER, *LPADAPTER;

typedef struct _ADAPTER_LIST{
	int			Cnt;
	ADAPTER		adapter[MAX_ADAPTER_NUM];
} ADAPTER_LIST, *LPADAPTER_LIST;

typedef struct _CONTROL_INFO{
	int				bindIndex;
	LPADAPTER		pCtrlAdapter;
	ADAPTER_LIST	AdapterList;
} CONTROL_INFO, *PCONTROL_INFO;

//+++++++++++++++++++++++++  Additional Structures  +++++++++++++++++++++++++
typedef struct _RT8187WIPARAM
{
    ULONG       bMptActCompleted;
    ULONG       eMptActType;
    ULONG       IoOffset;
    ULONG       IoValue;
    ULONG	     outLen;
    UCHAR	     outBuf[200];
}RT8187WIPARAM, *PRT8187WIPARAM;


typedef struct _ADAPTER_EX{
	wchar_t	SymbolicLink[MAX_LINK_NAME_LENGTH];  ///< The adapter name.
	wchar_t	Description[MAX_DESCRIPTION_LENGTH];  ///< The adapter description.
} ADAPTER_EX, *LPADAPTER_EX;



typedef struct _BT_REQ_CMD{
    UCHAR       opCodeVer;
    UCHAR       opCode;
    USHORT      paraLength;
    UCHAR       pParamStart[1];
} BT_REQ_CMD, *PBT_REQ_CMD;

typedef struct _BT_RSP_CMD{
    USHORT	status;
    USHORT      paraLength;
    UCHAR       pParamStart[1];
} BT_RSP_CMD, *PBT_RSP_CMD;

typedef struct _HAL_REQ_CMD{
    UCHAR       opCodeVer;
    UCHAR       opCode;
    USHORT      paraLength;
    UCHAR       pParamStart[1];
} HAL_REQ_CMD, *PHAL_REQ_CMD;

typedef struct _HAL_RSP_CMD{
    USHORT		status;
    USHORT      paraLength;
    UCHAR       pParamStart[1];
} HAL_RSP_CMD, *PHAL_RSP_CMD;

/**
 * The PACKET data structrue.
 *
 */
typedef struct _PACKET {
    HANDLE       hEvent;  ///< The event handle.
    OVERLAPPED   OverLapped;  ///< The overlapped structure.
    PVOID        Buffer;  ///< Pointer to buffer.
    UINT         Length;  ///< Length of this packet.
} PACKET, *LPPACKET;

// For DLL Version tracking.
typedef enum _DLL_VERSION {
	VERSION_N5_v0 = 0x00000000,
	VERSION_N5_v1 = 0x00000001,
	VERSION_N6_v0 = 0x80000000,
	VERSION_N6_v1 = 0x80000001,
	VERSION_MAX = 0x8000FFFF
} DLL_VERSION;

// For BT DLL Operation returned status.
typedef enum _DLL_STATUS {
	DLL_STATUS_SUCCESS,
	DLL_STATUS_INVALID_PARAMETER,
	DLL_STATUS_ALLOCATE_MEM_FAIL,
	DLL_STATUS_TIMEOUT,
	DLL_STATUS_NOT_SUPPORT,
	DLL_STATUS_MAX
} DLL_STATUS;

// For 8723AS BT EFUSE
typedef enum _UART_Function {
    H4_NO_FLOWCTL,
    H4_FLOWCTL,
    H5_NO_FLOWCTL,
    H5_FLOWCTL
} UART_Function;

// For U2/U3 switch.
typedef enum
{
	NOT_DEFINED,
	USB2,
	USB3
}FORCED_USB_MODE;

typedef enum
{
	ERR_SUCCESS = 0,
	ERR_DRIVER_FAILURE,
	ERR_IO_FAILURE,
	ERR_WI_TIMEOUT,
	ERR_WI_BUSY,
	ERR_BAD_FORMAT,
	ERR_INVALID_DATA,
	ERR_NOT_ENOUGH_SPACE,
	ERR_WRITE_PROTECT,
	ERR_READ_BACK_FAIL,	
} MP_ERROR_CODE;

#ifdef __cplusplus
	extern "C" {
#endif // #ifdef __cplusplus

//=========================================================================//
//							BT Solo API                                    //
//=========================================================================//
#ifndef _BTPGEfuseModlue_H
#define _BTPGEfuseModlue_H

#pragma once

#define REALTEKDLL_EXPORTS

	#ifdef REALTEKDLL_EXPORTS
		#define RTKBTPGEFUSEDLL_API __declspec(dllexport)
	#else
		#define RTKBTPGEFUSEDLL_API __declspec(dllimport)
	#endif

//---------------------------------------------------------------------------
//Define
#define STATUS_SUCCRESS  0
#define STATUS_FAIL      -1

enum UARTModeTag{
     UART_H4=0,
     UART_H5=1
};
enum IntfaceTag{
      USE_USB=0,
      USE_UART=1
};
//API Defune

//Device open/close
RTKBTPGEFUSEDLL_API int API_BTPGEfuse_Open(int Useinterface,int portNumber,unsigned long Baudtart,int UARTMode);
RTKBTPGEFUSEDLL_API int API_BTPGEfuse_Close(void);

//efuse
RTKBTPGEFUSEDLL_API int API_BT_ReadEfuseByte(int efuseMap,unsigned int Log_Address,unsigned char *DataByte);
RTKBTPGEFUSEDLL_API int API_BT_WriteEfuseByte(int efuseMap,unsigned int Log_Address,unsigned char DataByte);
RTKBTPGEFUSEDLL_API int API_BT_UpdataEfuse(void);
RTKBTPGEFUSEDLL_API int API_BT_RefreshEfuseMap(void);

//Get function
enum API_Command_TAG {

    GET_BT_SYSTEM_CONTENT=0,            //read efuse content from  system map
    GET_BT_BTEFUSE_CONTENT=1,           //read efuse content from  BT map
    GET_BT_BD_ADDR=2,                   //read bd_addr from  BT map
    //////////////////
    NumOfAPI_Command
};
RTKBTPGEFUSEDLL_API int API_BT_GetEfuseInfo(int Command,unsigned int efuseLogAddress,unsigned char *pData,int length);

//Module   create and release
RTKBTPGEFUSEDLL_API int API_BTPGEfuse_BuildModule(void);       //Build  BTPGEfuse  Module
RTKBTPGEFUSEDLL_API int API_BTPGEfuse_ReleaseModule(void);     //Relase  BTPGEfuse  Module

//debug
RTKBTPGEFUSEDLL_API int API_BTPGEfuse_ModuleFrameVisible(int bFlag);
RTKBTPGEFUSEDLL_API int API_BT_GetEfuseMapBurnLen(int efuseMap,int *BurnLen);

typedef int (*pfAPI_BTPGEfuse_Open)(int Useinterface,int portNumber,unsigned long Baudtart,int UARTMode);
typedef int (*pfAPI_BTPGEfuse_Close)(void);
typedef int (*pfAPI_BT_ReadEfuseByte)(int efuseMap,unsigned int Log_Address,unsigned char *DataByte);
typedef int (*pfAPI_BT_WriteEfuseByte)(int efuseMap,unsigned int Log_Address,unsigned char DataByte);
typedef int (*pfAPI_BT_UpdataEfuse)(void);
typedef int (*pfAPI_BT_RefreshEfuseMap)(void);
typedef int (*pfAPI_BT_GetEfuseInfo)(int Command,unsigned int efuseLogAddress,unsigned char *pData,int length);
//add for new BT PG dll by chengjin
typedef struct BASE_INTERFACE_MODULE_TAG BASE_INTERFACE_MODULE;

typedef int
(*BASE_FP_OPEN)(
	BASE_INTERFACE_MODULE *pBaseInterface
	);



typedef int
(*BASE_FP_SEND)(
	BASE_INTERFACE_MODULE *pBaseInterface,
	unsigned char *pWritingBuf,
	unsigned long Len
	);



typedef int
(*BASE_FP_RECV )(
	BASE_INTERFACE_MODULE *pBaseInterface,
	unsigned char *pReadingBuf,
	unsigned long  Len,
	unsigned long *pRetLen
	);



typedef int
(*BASE_FP_CLOSE)(
	BASE_INTERFACE_MODULE *pBaseInterface
	);



typedef void
(*BASE_FP_WAIT_MS)(
	BASE_INTERFACE_MODULE *pBaseInterface,
	unsigned long WaitTimeMs
	);





typedef void
(*BASE_FP_SET_USER_DEFINED_DATA_POINTER)(
	BASE_INTERFACE_MODULE *pBaseInterface,
	int UserDefinedData
	);



typedef void
(*BASE_FP_GET_USER_DEFINED_DATA_POINTER)(
	BASE_INTERFACE_MODULE *pBaseInterface,
	int *pUserDefinedData
	);


#define MAX_DATA_LEN	20

// Base interface module structure
struct BASE_INTERFACE_MODULE_TAG
{
	BASE_FP_OPEN Open;
	BASE_FP_SEND Send;
	BASE_FP_RECV Recv;
	BASE_FP_CLOSE Close;
	BASE_FP_WAIT_MS WaitMs;
	
	BASE_FP_SET_USER_DEFINED_DATA_POINTER   SetUserDefinedDataPointer;
	BASE_FP_GET_USER_DEFINED_DATA_POINTER   GetUserDefinedDataPointer;

	unsigned char InterfaceType;

	// User defined data
	unsigned long UserDefinedData;

	//for usb , uart
	unsigned char PortNo;		

	//for uart
	unsigned long Baudrate;
	unsigned char bUartProtocol;

	unsigned char pData[MAX_DATA_LEN];

};
typedef struct BASE_BTMPDLL_MODULE_TAG   BASE_BTMPDLL_MODULE; 

#define MAX_USERAWDATA_SIZE       1050
typedef struct BT_PARAMETER_TAG   BT_PARAMETER;
typedef struct BT_DEVICE_REPORT_TAG BT_DEVICE_REPORT;
typedef struct BT_CHIPINFO_TAG   BT_CHIPINFO;

struct  BT_PARAMETER_TAG
{
	unsigned long ParameterIndex;

	unsigned char mPGRawData[MAX_USERAWDATA_SIZE];
	unsigned char mParamData[MAX_USERAWDATA_SIZE];	
	unsigned char mChannelNumber;
	unsigned long mPacketType;
	unsigned char mTxGainIndex;
	unsigned char mTxGainValue;
	unsigned long mTxPacketCount;
	unsigned long	mPayloadType;
	unsigned long  mPacketHeader;
	unsigned char mWhiteningCoeffValue;
	unsigned char mTxDAC;
	ULONG64 mHitTarget;

	unsigned char TXGainTable[MAX_TXGAIN_TABLE_SIZE];
	unsigned char TXDACTable[MAX_TXDAC_TABLE_SIZE];

	unsigned char bHoppingFixChannel;
	unsigned long Rtl8761Xtal;
	unsigned char ExeMode; 
	unsigned char PHY;				//for Le Enhanced bt 5.0
	unsigned char ModulationIndex;	//for Le Enhanced bt 5.0
};

struct BT_CHIPINFO_TAG 
{ 
	unsigned int HCI_Version;
	unsigned int HCI_SubVersion;
	unsigned int LMP_Version;
	unsigned int LMP_SubVersion;

	unsigned int ChipType;
	unsigned int Version;
	int Is_After_PatchCode;

};
struct BT_DEVICE_REPORT_TAG { 

	unsigned long	TotalTXBits;
	unsigned long	TotalTxCounts;

	unsigned long	RXRecvPktCnts;
	unsigned long	TotalRXBits;
	unsigned long	TotalRxCounts;
	unsigned long	TotalRxErrorBits;
	int RxRssi;
	float ber;
	float Cfo;

	unsigned char CurrTXGainTable[MAX_TXGAIN_TABLE_SIZE];
	unsigned char CurrTXDACTable[MAX_TXDAC_TABLE_SIZE];

	unsigned char CurrThermalValue;
	unsigned long CurrRtl8761Xtal;
	unsigned char CurrStage;
	
	BT_CHIPINFO	*pBTInfo;
	BT_CHIPINFO	 BTInfoMemory;
	unsigned char ReportData[MAX_USERAWDATA_SIZE];	
};
typedef enum{
	REPORT_ALL = 0,
	REPORT_PKT_TX,
	REPORT_CON_TX,
	REPORT_RKT_RX,
	REPORT_TX_GAIN_TABLE,
	REPORT_TX_DAC_TABLE,
	REPORT_XTAL,
	REPORT_THERMAL,
	REPORT_BT_STAGE,
	REPORT_CHIP,
	REPORT_LOGICAL_EFUSE,
	REPORT_LE_RX,
	REPORT_LE_CONTINUE_TX,
	REPORT_FW_PACKET_TX,
	REPORT_FW_CONTINUE_TX,
	REPORT_FW_PACKET_RX,
	REPORT_FW_LE_CONTINUE_TX,	
	REPORT_TX_POWER_INFO,
	REPORT_GPIO3_0,
	REPORT_MP_DEBUG_MESSAGE,
	REPORT_MP_FT_VALUE,	
	REPORT_POWER_TRACKING,
    REPORT_MP_TXCAL_INFO,
	REPORT_FLASH_CONFIG,
	REPORT_LERX_PACKET_INFORMATION,
	REPORT_SNR
	
}BT_REPORT_TAG;

typedef enum {
	HCI_RESET = 0,						// 0

	// test mode
	TEST_MODE_ENABLE,					// 1

	// efuse setting
	WRITE_EFUSE_DATA,					// 2  

	// set table
	SET_TX_GAIN_TABLE,				// 3  
	SET_TX_DAC_TABLE,					// 4
	SET_DEFAULT_TX_GAIN_TABLE,		//5  
	SET_DEFAULT_TX_DAC_TABLE,		//6 
	
	//set power
	SET_POWER_GAIN_INDEX,			//7
	SET_POWER_GAIN,					//8
	SET_POWER_DAC,					//9

	// set xtal
	SET_XTAL,							//10  

	// report clear
	REPORT_CLEAR,						//11

	// pkt tx
	PACKET_TX_START,					//12 
	PACKET_TX_UPDATE,					//13 
	PACKET_TX_STOP,					//14 

	// continue tx
	CONTINUE_TX_START,				//15 
	CONTINUE_TX_UPDATE,				//16 
	CONTINUE_TX_STOP,					//17 

	// pkt rx
	PACKET_RX_START,					//18 
	PACKET_RX_UPDATE,					//19 
	PACKET_RX_STOP,					//20 

	// hopping mode
	HOPPING_DWELL_TIME,				//21 

	// LE
	LE_TX_DUT_TEST_CMD,				//22 
	LE_RX_DUT_TEST_CMD,				//23 
	LE_DUT_TEST_END_CMD,				//24 

	READ_EFUSE_DATA,					//25

	SET_CONFIG_FILE_DATA,				//26
	CLEAR_CONFIG_FILE_DATA,			//27

	//LE Continue TX
	LE_CONTINUE_TX_START,				//28
	LE_CONTINUE_TX_STOP,				//29

	// FW pkt tx
	FW_PACKET_TX_START,				//30  
	FW_PACKET_TX_STOP,				//31 

	// FW pkt rx
	FW_PACKET_RX_START,				//32 
	FW_PACKET_RX_STOP,				//33 	

	// FW continue tx
	FW_CONTINUE_TX_START,			//34 
	FW_CONTINUE_TX_STOP,				//35 

	//FW LE Continue TX
	FW_LE_CONTINUE_TX_START,			//36
	FW_LE_CONTINUE_TX_STOP, 			//37
	
	FW_READ_TX_POWER_INFO,			//38

	SET_GPIO3_0,						//39
	
	SET_ANT_INFO,						//40	
	SET_ANT_DIFF_S0S1,				//41

	TX_POWER_TRACKING,			//42
	SET_K_TX_CH_PWR,					//43	

	WRITE_FLASH_CONFIG,				//44 fro BBPro flash config
	// new flow function 	
	TX_POWER_GAIN_K,
	TX_POWER_FLATNESS,
	TX_PATH_LOSS_MODULE,

	CONFIG_EXTEND,	
	READ_FLASH_CONFIG,				//fro BBPro flash config
	UNLOCK_8822C,
	SetTxPower_8822C,	

	
	FW_SINGLETONE_TX_START,
	FW_SINGLETONE_TX_STOP, 
	
	SET_BQB_UART_MODE,
	GET_SNR_SETTING,
	SET_HW_RESET,
	BT_ACTION_NUM
} BT_ACTIONCONTROL_TAG;
typedef int
(*BT_DLL_MODULE_FP_ACTION_REPORT)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	int ActiceItem,
	BT_DEVICE_REPORT *pReport
	);

typedef int
(*BT_DLL_MODULE_FP_UPDATA_PARAMETER)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	BT_PARAMETER 	*pParam
	);

typedef int
(*BT_DLL_MODULE_FP_ACTION_CONTROLEXCUTE)(													
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule
	);

typedef int
(*BT_DLL_MODULE_FP_ACTION_DLFW)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	char *pPatchcode,
	int patchLength,
	int Mode
	);

typedef int
  (*BT_DLL_MODULE_FP_SET_MD_REG_MASK_BITS)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	unsigned char Addr,
	unsigned char Msb,
	unsigned char Lsb,
	const unsigned long UserValue
	);

typedef int
(*BT_DLL_MODULE_FP_GET_MD_REG_MASK_BITS)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	unsigned char RegStartAddr,
	unsigned char Msb,
	unsigned char Lsb,
	unsigned long *pReadingValue
	);

typedef int
(*BT_DLL_MODULE_FP_SET_RF_REG_MASK_BITS)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	unsigned char RegStartAddr,
	unsigned char Msb,
	unsigned char Lsb,
	const unsigned long WritingValue
	);

typedef int
(*BT_DLL_MODULE_FP_GET_RF_REG_MASK_BITS)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	unsigned char RegStartAddr,
	unsigned char Msb,
	unsigned char Lsb,
	unsigned long *pReadingValue
	);

typedef int
(*BT_DLL_MODULE_FP_SEND_HCICOMMANDWITHEVENT)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	unsigned int  OpCode,
	unsigned char PayLoadLength,
	unsigned char *pPayLoad,
	unsigned char  EventType,
	unsigned char  *pEvent
	);

typedef int
(*BT_DLL_MODULE_FP_RECV_ANYEVENT)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	unsigned char  *pEvent
	);

typedef int
(*BT_DLL_MODULE_FP_SET_SYS_REG_MASK_BITS)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	unsigned long Addr,
	unsigned char Msb,
	unsigned char Lsb,
	const unsigned long UserValue
	);

typedef int
(*BT_DLL_MODULE_FP_GET_SYS_REG_MASK_BITS)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	unsigned long Addr,
	unsigned char Msb,
	unsigned char Lsb,
	unsigned long *pUserValue
	);

typedef int
(*BT_DLL_MODULE_FP_SET_BB_REG_MASK_BITS)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	int Page,
	unsigned long RegStartAddr,
	unsigned char Msb,
	unsigned char Lsb,
	const unsigned long WritingValue
	);

typedef int
(*BT_DLL_MODULE_FP_GET_BB_REG_MASK_BITS)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	int Page,
	unsigned long Addr,
	unsigned char Msb,
	unsigned char Lsb,
	unsigned long *pUserValue
	);

typedef int
(*BT_DLL_MODULE_FP_SET_REG_MASK_BITS)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	int Type,
	int Page,
	unsigned long RegStartAddr,
	unsigned char Msb,
	unsigned char Lsb,
	const unsigned long WritingValue
	);

typedef int
(*BT_DLL_MODULE_FP_GET_REG_MASK_BITS)(
	BASE_BTMPDLL_MODULE  *pDLLBtBaseModule,
	int Type,
	int Page,
	unsigned long Addr,
	unsigned char Msb,
	unsigned char Lsb,
	unsigned long *pUserValue
	);


struct BASE_BTMPDLL_MODULE_TAG
{

	BT_DLL_MODULE_FP_UPDATA_PARAMETER UpDataParameter;
	BT_DLL_MODULE_FP_ACTION_CONTROLEXCUTE ActionControlExcute;
	BT_DLL_MODULE_FP_ACTION_REPORT ActionReport;
	BT_DLL_MODULE_FP_ACTION_DLFW DownloadPatchCode;
	BT_DLL_MODULE_FP_SET_MD_REG_MASK_BITS SetMdRegMaskBits;
	BT_DLL_MODULE_FP_GET_MD_REG_MASK_BITS GetMdRegMaskBits;	
	BT_DLL_MODULE_FP_SET_RF_REG_MASK_BITS SetRfRegMaskBits;
	BT_DLL_MODULE_FP_GET_RF_REG_MASK_BITS GetRfRegMaskBits;

	BT_DLL_MODULE_FP_SET_SYS_REG_MASK_BITS SetSysRegMaskBits;
	BT_DLL_MODULE_FP_GET_SYS_REG_MASK_BITS GetSysRegMaskBits;	
	BT_DLL_MODULE_FP_SET_BB_REG_MASK_BITS SetBBRegMaskBits;
	BT_DLL_MODULE_FP_GET_BB_REG_MASK_BITS GetBBRegMaskBits;

	BT_DLL_MODULE_FP_SET_REG_MASK_BITS SetRegMaskBits;
	BT_DLL_MODULE_FP_GET_REG_MASK_BITS GetRegMaskBits;
	
	BT_DLL_MODULE_FP_SEND_HCICOMMANDWITHEVENT SendHciCommandWithEvent;
	BT_DLL_MODULE_FP_RECV_ANYEVENT RecvAnyHciEvent;
	BASE_INTERFACE_MODULE *pBaseInterface;

	int BuildStatus;
};
typedef int  (*pfBTMPAPI_BuildInterfaceVendor)(
	BASE_INTERFACE_MODULE **ppBaseInterface,
	BASE_INTERFACE_MODULE *pBaseInterfaceModuleMemory,
	//Parmater
	unsigned int InterfaceType,
	unsigned char	PortNo,
	unsigned long Baudrate,
	unsigned char *pIp,	
	//basic fuction
	BASE_FP_OPEN Open,
	BASE_FP_SEND Send,
	BASE_FP_RECV Recv,
	BASE_FP_CLOSE Close,
	BASE_FP_WAIT_MS WaitMs
	);

typedef int  (*pfBTMPAPI_BuildInterfaceRTK)(
	BASE_INTERFACE_MODULE **ppBaseInterface,
	BASE_INTERFACE_MODULE *pBaseInterfaceModuleMemory,
	unsigned int InterfaceType,
	unsigned char	PortNo,
	unsigned long Baudrate,
	unsigned char *pIp	
	);

typedef int  (*pfBTMPAPI_ExternFunctionRTK)(
	int Chip_type,
	int Enter1_Leave2,
	BASE_INTERFACE_MODULE **ppBaseInterface,
	BASE_INTERFACE_MODULE *pBaseInterfaceModuleMemory,
	unsigned int   InterfaceType,
	unsigned char  PortNo,
	unsigned long  Baudrate,
	unsigned char *pWriting,
	unsigned char *pReading,
	unsigned int   return_check,
	unsigned char  *pData,
	unsigned int   Ending_close,
	unsigned int   Delay
	);


typedef int (*pfBTMPAPI_BuildBluetoothModule)(
	BASE_BTMPDLL_MODULE *pBaseBTMPDLLModule,
	BASE_INTERFACE_MODULE *pBaseInterface,
	void *pExtra,
	unsigned char *pTxGainTable,
	unsigned char *pTxDACTable
	);

typedef struct  BASE_BTMPDLLObject{
	pfBTMPAPI_BuildBluetoothModule BTMPAPI_BuildBluetoothModule;
	pfBTMPAPI_BuildInterfaceRTK BTMPAPI_BuildInterfaceRTK;
	pfBTMPAPI_BuildInterfaceVendor BTMPAPI_BuildInterfaceVendor;
    //pfBTMPAPI_BluetoothVerify  BTMPAPI_BluetoothVerify;
	pfBTMPAPI_ExternFunctionRTK   ExternFunctionRTK;


}BASE_BTMPDLLObject,*pBASE_BTMPDLLObject;


//Module
typedef int (*pfAPI_BTPGEfuse_BuildModule)(void);
typedef int (*pfAPI_BTPGEfuse_ReleaseModule)(void);
typedef int (*pfAPI_BTPGEfuse_ModuleFrameVisible)(int bFlag);
typedef int  (*pfAPI_BT_GetEfuseMapBurnLen)(int efuseMap,int *BurnLen);

typedef struct mBTPGEfuseModlue{
	pfAPI_BTPGEfuse_Open				 BTPGEfuse_Open;
        pfAPI_BTPGEfuse_Close                BTPGEfuse_Close;
        pfAPI_BT_ReadEfuseByte				 BT_ReadEfuseByte;
        pfAPI_BT_WriteEfuseByte				 BT_WriteEfuseByte;
        pfAPI_BT_UpdataEfuse				 BT_UpdataEfuse;
        pfAPI_BT_RefreshEfuseMap			 BT_RefreshEfuseMap;
        pfAPI_BT_GetEfuseMapBurnLen          BT_GetEfuseMapBurnLen;
        pfAPI_BTPGEfuse_BuildModule			 BTPGEfuse_BuildModule;
        pfAPI_BTPGEfuse_ReleaseModule        BTPGEfuse_ReleaseModule;
		pfAPI_BTPGEfuse_ModuleFrameVisible   BTPGEfuse_ModuleFrameVisible;
	
		pfAPI_BT_GetEfuseInfo                BT_GetEfuseInfo;
	
	    
	    int CreateModlue;
}mBTPGEfuseModlue,pmBTPGEfuseModlue;



#ifdef __cplusplus  
 extern "C" {                                      
#endif 
// Load dll and frame
void ReleaseBTPGEfuseModlueDLL(BASE_BTMPDLLObject *Modlue);
// release dll and frame
int BuildBTPGEfuseModlueDLL(BASE_BTMPDLLObject *Modlue);
#ifdef __cplusplus    
  }                                                    
#endif 
#endif

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                            MP Export APIs
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 * This function can find Realtek Adapter list.
 *
 * @param in/out ADAPTER_LIST
 *			Input the structure pointer and the result
 *			will be written to by the pointer.
 * @return TRUE if adapter found.
 *		   FALSE if no adapter found.
 */
BOOL RTLMP_API
FindRealtekAdapter(
	ADAPTER_LIST	*pAdapterList
	);

/**
 * This function reset the device under test.
 *
 * @param AdapterObject
 *			Pointer to ADAPTER obtained from OpenDUT.
 * @return TRUE if success.
 *		   FALSE if failed.
 */
INT RTLMP_API
ResetDUT(
    LPADAPTER	AdapterObject
	);

/**
 * This function open the device which to be test.
 * It must be called first to setup basic resources before calling to StartTest.
 * 
 * @param AdapterName
 *			The adapter name which to be opened. Format of this name is
 *			\\Device\\mp8185s_{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}.
 *			{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx} is instance GUID of your adapter.
 * @return Pointer to ADAPTER structure which you must supply to call other 
 *			functions. It returns NULL if open failed.
 */
PVOID RTLMP_API
OpenDUT(
    LPTSTR		AdapterName
    );



/**
 * This function open the device which to be test.
 * It must be called first to setup basic resources before calling to StartTest.
 *
 * @param ChipID
 *          The adapter's ChipID which to be opened. 
 * @param ChipVer
 *          The adapter's ChipVer which to be opened. 
 * @return Pointer to ADAPTER structure which you must supply to call other
 *          functions. It returns NULL if open failed.
 */
PVOID RTLMP_API
OpenSpecificDUT(
    ULONG      ChipID,
    ULONG      ChipVer
    );

PVOID RTLMP_API
OpenSpecificDutSilently(
    ULONG      ChipID,
    ULONG      ChipVer
    );

PVOID RTLMP_API
OpenSpecificDUTwithNID(
    ULONG      ChipID,
    ULONG      ChipVer,
	CHAR*		InstanceId
    );

PVOID RTLMP_API
OpenSpecificDutSilentlywithNID(
    ULONG      ChipID,
    ULONG      ChipVer,
	CHAR*		InstanceId
    );
/**
 * This function close the device under test.
 *
 * @param lpAdapter
 *			Pointer to ADAPTER obtained from OpenDUT.
 * @return TRUE if success.
 *		   FALSE if failed.
 */
INT RTLMP_API
CloseDUT(
    LPADAPTER	lpAdapter
    );

/**
 * Step into test state for opened adapter. This function must be called after
 * OpenDUT and before other operational functions such as SetDataRate ...
 * Or the call to operational functions will get unpredictable errors.
 *
 * @param AdapterObject
 *			Pointer to ADAPTER obtained from OpenDUT.
 * @return TRUE if success.
 *		   FALSE if failed.
 */
INT RTLMP_API
StartTest(
	LPADAPTER	AdapterObject
	);

/**
 * Stop and step out from test state. This function must be called before calling
 * to CloseDUT. Or some allocated resources will not be released.
 *
 * @param AdapterObject
 *			Pointer to ADAPTER obtained from OpenDUT.
 * @return TRUE if success.
 *		   FALSE if failed.
 */
INT RTLMP_API
StopTest(
	LPADAPTER	AdapterObject
	);

/**
 * This routine sets the transmit data rate of the DUT.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Rate
 *			1       : CCK 1M  
 *          2       : CCK 2M  
 *          3       : CCK 5.5M
 *          4       : CCK 11M 
 *          5       : OFDM 6M 
 *          6       : OFDM 9M 
 *          7       : OFDM 12M
 *          8       : OFDM 18M
 *          9       : OFDM 24M
 *          10      : OFDM 36M
 *          11      : OFDM 48M
 *          12      : OFDM 54M
 *          13~28   : MCS0~MCS15
 * @return If success, return TRUE.
 *		   If failed, return FALSE.
 */
INT RTLMP_API
SetDataRate(
	LPADAPTER	AdapterObject,
	int			Rate
    );





/**
 * This routine sets the DUT to use Long(1) or Short(2) preamble for the 
 * transmission.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Mode
 *			1:  Long Preamble\n
 *			2:  Short Preamble\n
 *          3:  Long GI\n
 *          4:  Short GI\n
 * @return TRUE: Success.
 */
INT RTLMP_API
SetPreamble(
	LPADAPTER	AdapterObject,
	int			Mode
    );


INT RTLMP_API
SetManualDiversityBB(
    LPADAPTER	AdapterObject,
    int			Mode
    );

/**
 * This routine sets the DUT to transmit and receive on a specific channel.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Channel
 *			Channel number.
 * @return TRUE: Success
 */
INT RTLMP_API
SetChannelDirectCall(
    LPADAPTER	AdapterObject,
    int			Channel
    );


INT RTLMP_API
SetPMacPacketTx(
    LPADAPTER		AdapterObject,
    PVOID			pPMacTxInfo
    );

INT RTLMP_API
SetTMacPacketTx(
    LPADAPTER		AdapterObject
    );

/**
 * This routine sets the DUT to start(1) or stop(0) to perform continuous
 * transmission.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Mode
 *			0: stop\n
 *			1: start\n
 * @return TRUE: Success
 */
INT RTLMP_API
SetTxContinuousDirectCall(
    LPADAPTER	AdapterObject,
    int			Mode
    );

/**
 * This routine sets the DUT to start(1) or stop(0) to perform continuous
 * transmission for "Carrier Suppression" test.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Mode
 *			0: stop\n
 *			1: start\n
 * @return TRUE: Success
 */
INT RTLMP_API
SetCarrierSuppressionTxContinuous(
    LPADAPTER	AdapterObject,
    int			Mode
    );

/**
 * This routine sets the DUT to stop(Mode:0) or start(Mode:1) the
 * single carrier continus tx.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Mode
 *			0:  Disable\n
 *			1:  Enable\n
 * @return TRUE: Success
 */
INT RTLMP_API
SetSingleCarrierTxContinuous(
    LPADAPTER	AdapterObject,
    int			Mode
    );

/**
 * This routine sets the DUT to stop(Mode:0) or start(Mode:1) the
 * single tone tx.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Mode
 *			0:  Disable\n
 *			1:  Enable\n
 * @return TRUE: Success
 */
INT RTLMP_API
SetSingleToneTx(
    LPADAPTER	AdapterObject,
    int			Mode
    );

/**
 * This routine sets the DUT to Packet Rx Test Mode if bStartTest==TRUE,
 * and restore to normal state otherwise.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param bStartTest
 *			TRUE: Enter Packet Rx Test mode.\n
 *			FALSE: Restore to normal state.\n
 * @return TRUE if success.
 *		   FALSE if failed.
 */
INT RTLMP_API
SetPacketRxTest(
	LPADAPTER	AdapterObject,
	BOOL		bStartTest
	);

/**
 * This routine sets the DUT to use antenna 1 or 2 for transmission. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Ant
 *			1: Antenna 1
 *			2: Antenna 2 
 * @return TRUE: Success
 */
INT RTLMP_API
SetTxAntennaBB(
    LPADAPTER	AdapterObject,
    int Ant
    );

INT RTLMP_API
ReadRSSIValue(
    LPADAPTER	AdapterObject,
    ULONG*		RegDataValue
    );

/**
 * This routine sets the DUT to transmit and receive on a specific antenna.
 *
 * @param AdapterObject
 *          AdapterObject return by OpenDUT.
 * @param Ant
 *          ANTENNA_NONE	= 0x00  
 *          ANTENNA_D		= 0x01
 *          ANTENNA_C		= 0x02
 *          ANTENNA_CD		= 0x03
 *          ANTENNA_B		= 0x04
 *          ANTENNA_BD		= 0x05
 *          ANTENNA_BC		= 0x06
 *          ANTENNA_BCD		= 0x07
 *          ANTENNA_A		= 0x08
 *          ANTENNA_AD		= 0x09
 *          ANTENNA_AC		= 0x0A
 *          ANTENNA_ACD		= 0x0B
 *          ANTENNA_AB		= 0x0C
 *          ANTENNA_ABD		= 0x0D
 *          ANTENNA_ABC		= 0x0E
 *          ANTENNA_ABCD	= 0x0F
 * @return TRUE: Success
 */
INT RTLMP_API
SetAntennaBB(
    LPADAPTER	AdapterObject,
    int Ant
    );

int RTLMP_API
SwitchRFPathSetting(
    LPADAPTER  AdapterObject,
    ULONG 	   Mode
	);

int RTLMP_API
QueryRFPathSetting(
    LPADAPTER  AdapterObject,
    ULONG 	   *Mode
	);

int RTLMP_API
QueryRFPathSettingEx(
        LPADAPTER  AdapterObject
);

int RTLMP_API
QueryRFEType(
        LPADAPTER  AdapterObject,
        ULONG 	   *Type
);

int RTLMP_API
QueryRFPathSwitchMap(
        LPADAPTER  AdapterObject,
        ULONG 	   *MapType
);

/**
 * This routine sets the DUT Control Register corresponding to the MANUAL
 * POWER_CONTROL to the desired value. (in 1dB scale)
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Value
 *			Index of Tx Power Level.
 * @return TRUE: Success.
 */
INT RTLMP_API
SetTxPowerControl(
    LPADAPTER	AdapterObject,
    unsigned int Value
    );
    
/**
 * This routine sets the Tx AGC Offset of Antenna B,C, and D
 *   to the desired value. (in 1dB scale)
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Value
 *			Value of Tx AGC Offset B, C, and D
 * @return TRUE: Success.
 */
INT RTLMP_API
SetTxAGCOffset(
    LPADAPTER       AdapterObject,
    unsigned int	Value
    );
    

/**
 * This routine sets the modulation to CCK(2) or OFDM(3).
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Modulation
 *			1:  Auto\n
 *			2:  CCK\n
 *			3:  OFDM\n
 * @return TRUE: Success
 */
INT RTLMP_API
SetModulation(
    LPADAPTER	AdapterObject,
    int			Modulation
    );

/**
 * This routine sets the Path to Main or Aux.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param bMain
 *			TRUE:  Main\n
 *			FALSE: Aux\n
 * @return TRUE: Success
 */

INT RTLMP_API
SetRfPathSwitch(
        LPADAPTER  AdapterObject,
        BOOL 	   bMain
        );

/**
 * This routine query the Path setting.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param bMain
 *			TRUE:  Main\n
 *			FALSE: Aux\n
 * @return TRUE: Success
 */

INT RTLMP_API
GetRfPathSwitch(
        LPADAPTER  AdapterObject,
        INT	   	   *bMain
        );


/**
 * This routine queries the EEPROM type in DUT.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Type
 *			The buffer for the returned value.\n
 *			Type 1: 93c46, Type 2: 93c56
 * @return TRUE: Success.
 */
INT RTLMP_API
QueryEEpromType(
    LPADAPTER	AdapterObject, 
    int*		Type
    );



/**
 * This routine queries the Board type in DUT.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Type
 *			The buffer for the returned value.\n
 * @return TRUE: Success.
 */
INT RTLMP_API
QueryBoardType(
    LPADAPTER	AdapterObject, 
    int*		Type
    );


/**
 * This routine queries the channel switch status in DUT.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param index
 *			The buffer for the returned value.\n
 * @return TRUE: Success.
 */
INT RTLMP_API
QueryChannelSwitchStatus(
    LPADAPTER	AdapterObject, 
    int*		Status
    );

INT RTLMP_API
QueryIsVS(
        LPADAPTER AdapterObject,
        int* bIsVS
        );


INT RTLMP_API
QueryIsVL(
        LPADAPTER AdapterObject,
        int* bIsVL
        );


/**
 * This routine queries the modulation/wirelessmode in DUT.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Modulation
 *			The buffer for the returned value.\n
 *			Type 1: 93c46, Type 2: 93c56
 * @return TRUE: Success.
 */
INT RTLMP_API
GetModulation(
    LPADAPTER	AdapterObject, 
    int*		Modulation
    );

/**
 * This routine write the MAC address to the DUT EEPROM.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param MACAddress
 *			The Mac Address.
 * @return TRUE: Success
 */
INT RTLMP_API
WriteMACAddress(
    LPADAPTER	AdapterObject,
    UCHAR*		MACAddress
    );

/**
 * This routine write the MAC address to the DUT EEPROM.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param MACAddress
 *			The Mac Address.
 * @return TRUE: Success
 */
INT RTLMP_API
ReadMACAddress(
    LPADAPTER	AdapterObject,
    char*		MACAddress
    );

int RTLMP_API
ReadCustomerString(
    LPADAPTER   AdapterObject,
    char*		CusStr
    );

/**
 * This routine resets the number of Tx packets sent.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @return TRUE: Success.
 */
INT RTLMP_API
ResetTxPacketSent(
    LPADAPTER	AdapterObject
    );

/**
 * This routine gets the totoal number of Tx packets sent.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param TxPacketCount
 *			Returned the count of Tx packet sent.
 * @return TRUE: Success.
 */
INT RTLMP_API
QueryTxPacketSent(
    LPADAPTER	AdapterObject,
    int			*TxPacketCount
    );


INT RTLMP_API
QueryPHYTxPacketSent(
	LPADAPTER  AdapterObject,
	UINT 	   *TxPacketCount
    );


/**
 * This routine resets the number of Rx packets received.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @return TRUE: Success.
 */
INT RTLMP_API
ResetRxPacketReceived(
    LPADAPTER	AdapterObject
    );

/**
 * This routine gets the total number of Rx packets received.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param RxPacketCount
 *			Returned count of Rx packet received.
 * @return TRUE: Success.
 */
INT RTLMP_API
QueryRxPacketReceived(
    LPADAPTER	AdapterObject,
    int			*RxPacketCount
    );
INT RTLMP_API
QueryPHYLayerRxPacketReceived (
        LPADAPTER  	AdapterObject,
        int 		*RxPacketCount
        );
INT RTLMP_API
QueryMacRxPacketReceived (
        LPADAPTER  	AdapterObject,
        int			*RxPacketCount
        );
/**
 * This routine gets the total number of Rx packets with CRC error.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param RxPacketCount
 *			Returned count of Rx packet with CRC error.
 * @return TRUE: Success.
 */
INT RTLMP_API
QueryRxPacketCRC32Error(
    LPADAPTER		AdapterObject,
    int				*RxPacketCRC32Error
    );
INT RTLMP_API
QueryPHYLayerRxPacketCRC32Error (
        LPADAPTER		AdapterObject,
        int				*RxPacketCRC32Error
        );
INT RTLMP_API
QueryMacRxPacketCRC32Error(
        LPADAPTER  	AdapterObject,
        int 		*RxPacketCRC32Error
        );
/**
 * This routine query the permanent address of the DUT. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param MACAddress
 *			The Mac Address.
 * @return TRUE: Success
 */
INT RTLMP_API
QueryPermanentAddress(
    LPADAPTER	AdapterObject,
    char*		MACAddress
    );


/**
 * This routine Write flash.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param FlashAddressOffset
 *			The address offset for Flash Write.  
 * @param FlashValue
 *			Write Length of Flash.
  * @return TRUE: Success.
 */
int RTLMP_API
WriteFlash(
   LPADAPTER  AdapterObject,
   UINT	FlashAddressOffset,
   UINT   FlashValue
    );

/**
 * This routine Read flash.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param FlashAddressOffset
 *			The address offset for Flash Read.  
 * @param FlashValue
 *			Read Length of Flash.
  * @return TRUE: Success.
 */
int RTLMP_API
ReadFlash(
   LPADAPTER  AdapterObject,
   UINT	FlashAddressOffset,
   UINT    *FlashValue
    );

/**
 * This routine get flash  progress.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param DownloadProgress
 *			Query Flash  Progress.
  * @return TRUE: Success.
 */
int RTLMP_API
QueryFlashProgress(
    LPADAPTER  AdapterObject,
    UINT 	*DownloadProgress
    );

/**
 * This routine Erase flash.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param FlashAddressOffset
 *			The address offset for Flash Erase.  
 * @param EraseLength
 *			Erase Length of Flash.
  * @return TRUE: Success.
 */
int RTLMP_API
EraseFlash(
    LPADAPTER  AdapterObject,
    UINT	FlashAddressOffset,
   UINT 	EraseLength
    );


/**
 * This routine download Image to flash or verify Image in flash.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Flag
 *			Download or Verify.
  * @param FlashAddressOffset
 *			The address offset for Flash Download or Verify. 
 * @param chImageFilePath
 *			The Image File Path.
  * @return TRUE: Success.
 */
int RTLMP_API
UpdateFlashImage(
    LPADAPTER  AdapterObject,
    UCHAR	Flag,
    ULONG       FlashAddressOffset,
    CHAR * 	chImageFilePath
    );

/**
 * This routine get flash size.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
  * @param FlashSize
 *			Flash Size. 
  * @return TRUE: Success.
 */
int RTLMP_API
QueryFlashSize(
   LPADAPTER  AdapterObject,
   ULONG 	*FlashSize
    );

/**
 * This routine write value to specified MAC register.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param RegOffsetValue
 *			The offset of MAC register to write.
 * @param RegDataWidth
 *			The data width of MAC register to write.
 * @param RegDataValue
 *			The value to write. 
 * @return TRUE: Success.
 */
INT RTLMP_API
WriteRegData(
    LPADAPTER	AdapterObject,
    UINT		RegOffsetValue,
    UINT		RegDataWidth,
    ULONG		RegDataValue
    );
INT RTLMP_API
SICWriteRegData(
    LPADAPTER	AdapterObject,
    UINT		RegOffsetValue,
    UINT		RegDataWidth,
    ULONG		RegDataValue
    );

/**
 * This routine read value the from specified MAC register.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param RegOffsetValue
 *			The offset of MAC register to read.
 * @param RegDataWidth
 *			The data width of MAC register to read.
 * @param RegDataValue
 *			The value read.
 * @return TRUE: Success.
 */
INT RTLMP_API
ReadRegData(
    LPADAPTER	AdapterObject,
    UINT		RegOffsetValue,
    UINT		RegDataWidth,
    ULONG*		RegDataValue
    );
INT RTLMP_API
SICReadRegData(
    LPADAPTER	AdapterObject,
    UINT		RegOffsetValue,
    UINT		RegDataWidth,
    ULONG*		RegDataValue
    );

/**
 * This routine write value to specified RF register.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param RegOffsetValue
 *			The offset of RF register to write.
 * @param RegDataWidth
 *			The data width of RF register to write.
 * @param RegDataValue
 *			The value to write.
 * @return TRUE: Success.
 */
INT RTLMP_API
RFWriteRegData(
    LPADAPTER	AdapterObject,
    UINT		RegOffsetValue,
    UINT		RegDataWidth,
    ULONG		RegDataValue
    );

/**
 * This routine read value of specified RF register. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param RegOffsetValue
 *			The offset of RF register to read.
 * @param RegDataWidth
 *			The data width of RF register to read.
 * @param RegDataValue
 *			The value read. 
 * @return TRUE: Success.
 */
INT RTLMP_API
RFReadRegData(
    LPADAPTER	AdapterObject,
    UINT		RegOffsetValue,
    UINT		RegDataWidth,
    ULONG*		RegDataValue
    );

/**
 * This routine write 1-byte value to specified BB register. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param RegOffsetValue
 *			The offset of BB register to write.
 * @param bOFDM
 *			TRUE if the regsiter to access is for OFDM.
 * @param RegDataValue
 *			The value to write. 
 * @return TRUE: Success.
 */
INT RTLMP_API
WriteBBData(
    LPADAPTER	AdapterObject,
    UINT		RegOffsetValue,
	BOOLEAN		bOFDM,
    ULONG		RegDataValue
    );

/**
 * This routine read value the from specified BB register.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param RegOffsetValue
 *			The offset of BB register to read.
 * @param bOFDM
 *			TRUE if the regsiter to access is for OFDM.
 * @param RegDataValue
 *			The value read. 
 * @return TRUE: Success.
 */
INT RTLMP_API
ReadBBData(
    LPADAPTER	AdapterObject,
    UINT		RegOffsetValue,
	BOOLEAN		bOFDM,
    ULONG*		RegDataValue
    );

/**
 * This routine sets the Rx Initial Gain Value
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param InitialGainValue
 *			InitialGainValue : 1..7
 * @return TRUE: Success.
 */
INT RTLMP_API
SetInitialGain(
    LPADAPTER	AdapterObject,
    int			InitialGainValue
    );

/**
 * This routine queries the Chip ID and Version in DUT.  
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param ChipID:
 *			The value returns Chip ID.
 * @param ChipVer:
 *			The value returns Chip Reversion.
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
QueryChipIDandVer(
	LPADAPTER   AdapterObject,
	ULONG*		ChipID,
	ULONG*		ChipVer
	);


/**
 * This routine queries the VID and PID in DUT.  
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param VID:
 *			The value returns VID.
 * @param PID:
 *			The value returns PID.
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
QueryVIDandPID(
	LPADAPTER   AdapterObject,
	ULONG*		VID,
	ULONG*		PID
	);


/**
 * This routine queries the RF type in DUT.  
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Type
 *			The returned type.
 * @return TRUE: Success.
 */
INT RTLMP_API
QueryRFType(
    LPADAPTER	AdapterObject, 
    int*		Type
    );



/**
 * This routine queries the RF type in DUT.  
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Type
 *			The returned type.
 * @return TRUE: Success.
 */
INT RTLMP_API
QueryRFMode(
    LPADAPTER	AdapterObject, 
    int*		Mode
    );


/**
 * This routine is used to enable/disable ACK counter.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param bEnableAckCounter
 *			FALSE: Disable Ack counter.\n
 *			TRUE: Enable Ack counter.
 * @return TRUE: Success.
 */
INT RTLMP_API
EnableAckCounter(
	LPADAPTER	AdapterObject, 
	int			bEnableAckCounter
	);

/**
 * This routine resets ACK counter.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @return TRUE: Success.
 */
INT RTLMP_API
ResetAckCounter(
	LPADAPTER	AdapterObject
	);

/**
 * This routine queries the current ACK counter.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Value
 *			The buffer for the returned value.
 * @return TRUE: Success.
 */
INT RTLMP_API
GetAckCounter(
	LPADAPTER	AdapterObject, 
	ULONG*		Value
	);


/**
 * This routine queries the Signal Location
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param SignalLocation
 *			Set to the returned signal location after called.
 *          1. 20M
 *          2. 40M-Dup
 *          3. 40M-LSC
 *          4. 40M-USC
 *          5. 40M-DSC
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
QuerySignalLocationType(
	LPADAPTER   AdapterObject,
	int*		SignalLocation
	);
/**
 * This routine set the Signal Location
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param SignalLocation
 *			New signal location you want to set to the adapter.
 *          1. 20M
 *          2. 40M-Dup
 *          3. 40M-LSC
 *          4. 40M-USC
 *          5. 40M-DSC
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
SetSignalLocationType(
	LPADAPTER   AdapterObject,
	int 		SignalLocation
	);

/**
 * This routine sends the Debug command to firmware/driver
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param DbgCmdLength
 *			The total length of InData
 * @param InData
 *          The input buffer
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
DebugCmdSend(
	LPADAPTER   AdapterObject,
	int 		DbgCmdLength,
	UCHAR       *InData
	);
/**
 * This routine checks the Debug command result ready or not
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param ready
 *			result 1:ready, 0:not ready.
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
DebugCmdCheck(
	LPADAPTER   AdapterObject,
	int 		*ready
	);
/**
 * This routine get the Debug command result returned from firmware/driver
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param OutData
 *          The returned data
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
DebuggCmdReturn(
	LPADAPTER   AdapterObject,
	UCHAR       *OutData
	);
/**
 * This routine query the signal strength
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param OutData
 *          The returned data
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
Query_MIMPO_RSSI(
    LPADAPTER   AdapterObject,
    UCHAR       *OutData,
    int         *BytesReceived
    );

/**
 * This routine set the CrystalCap
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param CrystalCap
 *          The setting value.
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
SetCrystalCap(
    LPADAPTER   AdapterObject,
    int         CrystalCap
    );

/**
 * This routine get the current CrystalCap
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param CrystalCap
 *          The returned value.
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
GetCrystalCap(
    LPADAPTER   AdapterObject,
    int         *CrystalCap
    );


/**
 * This routine read the valid RF path from Driver..
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param RfPath
 *          The returned value
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
ReadValidRFPath(
    LPADAPTER   AdapterObject,
    ULONG		*RfPath
    );

/**
 * This routine read the RF mode(1T2R or 2T4R) from EEPROM.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param RfMode
 *          The returned value
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
ReadRFMode(
    LPADAPTER   AdapterObject,
    int			*RfMode
    );

/**
 * This routine read the ThermalMeter value.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param ThermalValue
 *          The returned value
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
ReadRFThermalMeter(
    LPADAPTER   AdapterObject,
    int			*ThermalValue
    );

/**
 * This routine get the autoload status
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param AutoloadStatus
 *          The returned value.
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
GetAutoloadStatus(
    LPADAPTER   AdapterObject,
    int         *AutoloadStatus
    );

/**
 * This routine notify the driver start/stop the testing with testing type
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param bStart
 *          1=Start, 0=Stop.
 * @param TestItem
 *			The testing type.
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
MPStartTest(
    LPADAPTER   AdapterObject,
    ULONG		bStart, 
    ULONG		TestItem
    );


INT RTLMP_API
ReadEFuseByteEx(
    LPADAPTER   AdapterObject,
    ULONG       Offset
    );

/**
 * This routine read 1-byte value from EFUSE.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Offset
 *         		The byte offset of EFUSE to read.
 * @param Value
 *	    		The value read.
 * @return TRUE: Success.
 */
INT RTLMP_API
ReadEFuseByte(
    LPADAPTER   AdapterObject,
    ULONG       Offset,
    UCHAR*      Value
    );

/**
 * This routine read 2-byte value from EFUSE.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Offset
 *         		The byte offset of EFUSE to read.
 * @param Value
 *	    		The value read.
 * @return TRUE: Success.
 */
INT RTLMP_API
ReadEFuse2Byte(
    LPADAPTER   AdapterObject,
    ULONG       	Offset,
    USHORT      *Value
    );

/**
 * This routine read 4-byte value from EFUSE.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Offset
 *         		The byte offset of EFUSE to read.
 * @param Value
 *	    		The value read.
 * @return TRUE: Success.
 */
INT RTLMP_API
ReadEFuse4Byte(
    LPADAPTER   AdapterObject,
    ULONG       Offset,
    ULONG     *Value
    );


/**
 * This routine write 1-byte value into EFUSE.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Offset
 *         		The byte offset of EFUSE to write.
 * @param Value
 *	    		The value to write.
 * @return TRUE: Success.
 */
INT RTLMP_API
WriteEFuseByte(
    LPADAPTER   AdapterObject,
    ULONG       Offset,
    UCHAR       Value
    );

/**
 * This routine write value into EFUSE. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param EFuse
 *         		The buffer for EFUSE value to write.   
 * @return TRUE: Success.
 */
INT RTLMP_API
WriteEFuse(
    LPADAPTER   AdapterObject,
    UCHAR       *EFuse
    );
/**
 * This routine update value into EFUSE. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param EFuse
 *         		The buffer for EFUSE value to write.   
 * @return TRUE: Success.
 */
INT RTLMP_API
UpdateEFuse(
    LPADAPTER   AdapterObject
    );

/**
 * This routine update MAP form EFUSE. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param EFuse
 *         		The buffer for EFUSE value to write.   
 * @return TRUE: Success.
 */
INT RTLMP_API
UpdateEFuseMAP(
    LPADAPTER   AdapterObject
    );


/**
 * This routine read value from EFUSE. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param EFuse
 *         		The buffer for EFUSE value to read.   
 * @return TRUE: Success.
 */
INT RTLMP_API
ReadEFuse(
    LPADAPTER   AdapterObject,
    UCHAR       *EFuse
    );



/**
 * This routine update MAP and read value from EFUSE . 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param EFuse
 *         		The buffer for EFUSE value to read.   
 * @return TRUE: Success.
 */
INT RTLMP_API
ReadEFuseMAP(
    LPADAPTER   AdapterObject,
    UCHAR       *EFuse
    );



INT RTLMP_API
SelectBTSoloMap(
    ULONG Map
    );

BOOL RTLMP_API
ReadBTSoloMacAddress(
	UCHAR *pMacAddress
	);

INT RTLMP_API
ReadBTEFuseByteEx(
    LPADAPTER   AdapterObject,
    ULONG       Offset
    );
INT RTLMP_API
BT_ReadWriteEfuseMap(
UCHAR *sysEfuse,
UCHAR *btEfuse,
ULONG mode,
ULONG Type
);
/**
 * This routine read 1-byte value from BT EFUSE.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Offset
 *         		The byte offset of EFUSE to read.
 * @param Value
 *	    		The value read.
 * @return TRUE: Success.
 */
INT RTLMP_API
ReadBTEFuseByte(
    LPADAPTER   AdapterObject,
    ULONG       Offset,
    UCHAR*      Value
    );

/**
 * This routine read 2-byte value from BT EFUSE.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Offset
 *         		The byte offset of EFUSE to read.
 * @param Value
 *	    		The value read.
 * @return TRUE: Success.
 */
INT RTLMP_API
ReadBTEFuse2Byte(
    LPADAPTER   AdapterObject,
    ULONG       Offset,
    USHORT      *Value
    );

/**
 * This routine read 4-byte value from BT EFUSE.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Offset
 *         		The byte offset of EFUSE to read.
 * @param Value
 *	    		The value read.
 * @return TRUE: Success.
 */
INT RTLMP_API
ReadBTEFuse4Byte(
    LPADAPTER   AdapterObject,
    ULONG       Offset,
    ULONG     *Value
    );

/**
 * This routine write 1-byte value into BT EFUSE.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param Offset
 *         		The byte offset of EFUSE to write.
 * @param Value
 *	    		The value to write.
 * @return TRUE: Success.
 */
INT RTLMP_API
WriteBTEFuseByte(
    LPADAPTER   AdapterObject,
    ULONG       Offset,
    UCHAR       Value
    );

/**
 * This routine write value into BT EFUSE. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param EFuse
 *         		The buffer for EFUSE value to write.   
 * @return TRUE: Success.
 */
INT RTLMP_API
WriteBTEFuse(
    LPADAPTER   AdapterObject,
    UCHAR       *EFuse
    );

INT RTLMP_API
PGBDAddressToBTEFuse(  // PG BD Address
    	LPADAPTER   AdapterObject,
	UCHAR* BDAddr
    );

INT RTLMP_API
PGTxTIndexToBTEFuse(	 //PG Tx Index
    	LPADAPTER   AdapterObject,
	UCHAR	 TxIndex
    );

INT RTLMP_API
PGXtalTrimToBTEFuse(	 //PG XTal Trim Value for CFO-K
    	LPADAPTER   AdapterObject,
	UCHAR	 XtalTrimValue,
	bool		CFOTrackingenable
    );

INT RTLMP_API
PGBaudRateToBTEFuse(	             //PG BT baud rate
	LPADAPTER   AdapterObject,
	ULONG	 	BaudRate,
	UART_Function   UartFunc
    );

/**
 * This routine update value into BT EFUSE. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param EFuse
 *         		The buffer for EFUSE value to write.   
 * @return TRUE: Success.
 */
INT RTLMP_API
UpdateBTEFuse(
    LPADAPTER   AdapterObject
    );

/**
 * This routine update MAP form BT EFUSE. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param EFuse
 *         		The buffer for EFUSE value to write.   
 * @return TRUE: Success.
 */
INT RTLMP_API
UpdateBTEFuseMAP(
    LPADAPTER   AdapterObject
    );


/**
 * This routine reload MAP form BT EFUSE. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @return TRUE: Success.
 */
INT RTLMP_API
ReloadBTEFuse(
    LPADAPTER   AdapterObject
    );



/**
 * This routine read value from BT EFUSE. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param EFuse
 *         		The buffer for EFUSE value to read.   
 * @return TRUE: Success.
 */
INT RTLMP_API
ReadBTEFuse(
    LPADAPTER   AdapterObject,
    UCHAR       *EFuse
    );



/**
 * This routine update MAP and read value from BT EFUSE . 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param EFuse
 *         		The buffer for EFUSE value to read.   
 * @return TRUE: Success.
 */
INT RTLMP_API
ReadBTEFuseMAP(
    LPADAPTER   AdapterObject,
    UCHAR       *EFuse
    );


INT RTLMP_API
DLL_ReadBTEFuseUtilization(
    LPADAPTER   AdapterObject,
    ULONG       *EFuseUtilize
    );

/**
 * This routine reads the PG Format Version. 
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param PGFmtVer
 *         	The read value.   
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
GetPGFmtVer(
    LPADAPTER   AdapterObject,
    int         *PGFmtVer
    );

/**
 * This routine sets the led_num to light as the led_mode
 * transmission.
 *
 * @param AdapterObject
 *          AdapterObject return by OpenDUT.
 * @param led_num
 *			The led# of the NIC
 * @param led_mode
 *			The mode you want the led to bright
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
LEDControl(
    LPADAPTER  AdapterObject,
    int led_num,
    int led_mode
    );

BOOLEAN RTLMP_API
CheckLED(
    LPADAPTER   AdapterObject,
    int			LedNum,
    int			*bTurnedON
    );

/**
 * This routine returns the result for WPS button pushed or not
 * transmission.
 *
 * @param AdapterObject
 *          AdapterObject return by OpenDUT.
 * @param pushed
 *			result
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
ButtonPushed(
    LPADAPTER   AdapterObject,
    int			buttonType,
    int			*pushed
    );

/**
 * This routine returns the result for HW RF on or off
 * transmission.
 *
 * @param AdapterObject
 *          AdapterObject return by OpenDUT.
 * @param rfstatus
 *			result
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
QueryRfStatus(
    LPADAPTER   AdapterObject,
    int			*rfstatus
    );

int RTLMP_API
TxPwrTracking_Info_Trigger(
    LPADAPTER   AdapterObject,
	ULONG		bStart
    );

int RTLMP_API
TxPwrTracking_Info_GetResult(
    LPADAPTER   AdapterObject,
	UCHAR	 	*OutBuf
    );

int RTLMP_API
PSD_GetResult(
    LPADAPTER   AdapterObject,
	UINT	 	*OutBuf,
	UINT		point,
	UINT		start_point,
	UINT		stop_point,
	UINT		average,
	UINT		buf_size
    );

int RTLMP_API
PSD_GetResultEx(
    LPADAPTER   AdapterObject,
	UINT	 	*OutBuf,
	UINT		buf_size
    );


int RTLMP_API
PSD_Initialize(
	LPADAPTER   AdapterObject,
	INT		point,
	INT		start_point,
	INT		stop_point,
	INT		average
        );

INT RTLMP_API
ADC_GetResult(
		LPADAPTER   AdapterObject,
		UINT 		*OutBuf
		);

INT RTLMP_API
ADC_Trigger(
        LPADAPTER   AdapterObject,
		PULONG		Param
        );

INT RTLMP_API
ADC_Stop(
        LPADAPTER   AdapterObject
        );

INT RTLMP_API
TriggerIQK(
        LPADAPTER   AdapterObject
        );

INT RTLMP_API
TriggerLCK(
        LPADAPTER   AdapterObject
        );

INT RTLMP_API
TriggerDPK(
        LPADAPTER   AdapterObject
        );

INT RTLMP_API
EnableDPK(
        LPADAPTER  AdapterObject,
        BOOL 	   bEnable
        );




BOOLEAN RTLMP_API
SetRFOnOff(
    LPADAPTER       AdapterObject,
    int			   RfOnOff			//0: on, 1: off	
    );


//
// 2008/12/12 MH Check Efuse Utilization and autoload condition.
//
INT RTLMP_API
DLL_ReadEFuseUtilization(
    LPADAPTER   AdapterObject,
    ULONG       *EFuseUtilize
    );
INT RTLMP_API
DLL_CheckAutloadStatus(
    LPADAPTER   AdapterObject,
    UCHAR       *AutoloadFail
    );

//
// 2009/01/09 MH 92S will switch tx power tracking here. UI will send True to enable
// power tack and false to stop it.
//
INT RTLMP_API
DLL_SwitchTxPwrTrack(
    LPADAPTER   AdapterObject,
    UCHAR         TxPwrTrackState
    );


/**
 * This routine query the TxPwrTracking setting.
 *
 * @param AdapterObject
 *			AdapterObject return by OpenDUT.
 * @param TxPwrTrackState
 *			TRUE:  On\n
 *			FALSE: Off\n
 * @return TRUE: Success
 */

INT RTLMP_API
DLL_QueryTxPwrTrack(
        LPADAPTER  AdapterObject,
        INT	   	   *TxPwrTrackState
        );

    
/**
 * This routine returns the calculated tx power index for current bandwidth/channel/rate
 * value.
 *
 * @param AdapterObject
 *          AdapterObject return by OpenDUT.
 * @param RfPath
 *			RF-path (0=A, 1=B)
 * @param powerIndex
 *			result
 * @return TRUE: Success.
 */
BOOLEAN RTLMP_API
GetCalTxPwrIndex(
    LPADAPTER   AdapterObject,
    ULONG		RfPath,
    int			*powerIndex
    );

/**
 * This routine returns the kfree value in physical efuse.
 * value.
 *
 * @param AdapterObject
 *          AdapterObject return by OpenDUT.
 * @param KfreeValue
 *			result
 * @return TRUE: Success.
 */
int RTLMP_API
GetKfreeFromEfuse(
    LPADAPTER   AdapterObject,
    ULONG       *KfreeValue
    );

INT RTLMP_API
SetForcedUsbMode(
    LPADAPTER       AdapterObject,
    FORCED_USB_MODE Value
    );

BOOLEAN RTLMP_API
QueryBandwidthCap(
    LPADAPTER   AdapterObject,
    int*        BWCap
    );

BOOLEAN RTLMP_API
	QueryChipIf8731A(
	LPADAPTER   AdapterObject,
	int*        flag
	);

BOOLEAN RTLMP_API
SetRxFilter(
	LPADAPTER   AdapterObject,
	BOOLEAN		bFilter
	);

INT RTLMP_API
SetDigOnOff(
    LPADAPTER   AdapterObject,
    BOOLEAN 	bTurnOn
	);


DLL_STATUS RTLMP_API
BT_OperationEntry(
		LPADAPTER   AdapterObject,
        PVOID 		InputBuffer,
        PVOID 		OutputBuffer
        );

DLL_STATUS RTLMP_API
HAL_OperationEntry(
		LPADAPTER   AdapterObject,
        PVOID 		InputBuffer,
        PVOID 		OutputBuffer
        );

BOOLEAN RTLMP_API
ToggleRxPhyCounter(
        LPADAPTER AdapterObject
        );


INT RTLMP_API
FillTxDesc_LDPC(
    LPADAPTER       AdapterObject,
    unsigned int    Value
);

INT RTLMP_API
FillTxDesc_STBC(
    LPADAPTER       AdapterObject,
    unsigned int    Value
);

INT RTLMP_API
SelectTxPowerLimitTable(
        LPADAPTER  AdapterObject,
        int 	   Table
);

BOOLEAN RTLMP_API
LoadPwrLmtTableStatus(
		int			*Status
);

BOOLEAN RTLMP_API
LoadPwrTableStatus(
		int			*Status
);

BOOLEAN RTLMP_API
ResetPwrLmtTableStatus(
);

BOOLEAN RTLMP_API
ResetPwrTableStatus(
);

INT RTLMP_API
GetMaskArray(
    LPADAPTER   AdapterObject,
    CHAR 		*OutArray
);

INT RTLMP_API
GetMaskByIndex(
    LPADAPTER   AdapterObject,
    ULONG       Index
);

VOID RTLMP_API
SetGPIO(
	LPADAPTER	AdapterObject,
	UCHAR		PinNumber,
	BOOLEAN		bHigh
);


MP_ERROR_CODE RTLMP_API
GetLastErrorMP();

INT RTLMP_API
SwitchCFOTracking(
    LPADAPTER   AdapterObject,
    BOOLEAN     bTurnOnCFO
);

BOOLEAN RTLMP_API
GeneralSendOID(
    	LPADAPTER   AdapterObject,
        ULONG		Oid,
		BOOLEAN		bSetOid,
        ULONG		Parameter,		
		BOOLEAN 	bWaitUntilCompleted,
		ULONG 		Timout,
		ULONG 		WorkItemToWait,
    	PULONG      pOutValue
);

INT RTLMP_API
QueryCVersion(
        LPADAPTER AdapterObject,
        int*      CVersion
);

INT RTLMP_API
QueryUSBSpeed(
        LPADAPTER  AdapterObject,
        int 	   *USBSpeed
);

INT RTLMP_API
WritePseudoEFuseByte(
    LPADAPTER   AdapterObject,
    UCHAR 		Type,
    ULONG       Offset,
    UCHAR       Value
);

BOOLEAN RTLMP_API
DLL_SendSpecificPacket(
    LPADAPTER   AdapterObject,
	PVOID		PktBuffer,
	USHORT		PktLength,
    PVOID		Reserved
    );

int RTLMP_API
StartRxDescDataParsing(
        LPADAPTER  AdapterObject,
        UCHAR 	   bStart
);

int RTLMP_API
QueryRxDescData(
        LPADAPTER  AdapterObject,
        UCHAR 	   *Buf
);

int RTLMP_API
DownloadFW(
	LPADAPTER	AdapterObject,
	UCHAR		fw_type
);

BOOLEAN RTLMP_API
SetEnhanceAntiInterference(
    LPADAPTER   AdapterObject,
    BOOLEAN	bEnable
    );

/**
 * This API(WriteFTPattern) Writes FT pattern 
 *
 * @param AdapterObject
 *          AdapterObject return by OpenDUT.
 * @return TRUE: Success, FALSE: Fail.
 */
BOOLEAN  RTLMP_API
WriteFTPattern(
	LPADAPTER   AdapterObject
);

/**
 * This API(CheckFTPattern) Checks FT pattern 
 *
 * @param AdapterObject
 *          AdapterObject return by OpenDUT.
 * @return INT: 
 * |Byte3|Byte2|Byte1  |Byte0 |
 * |Reseverd   |Version|Status|
 * Status:  0x0 = Match, 0x1 = MisMatch, 0x2 = Unknown version
 * Version: Value might be 0x00~0x0F
 * if return value is : 0xFFFFFFFF, it means the API fail to query FT pattern.
 */
INT  RTLMP_API
CheckFTPattern(
	LPADAPTER   AdapterObject
);

BOOLEAN RTLMP_API
GetBtPinValue(
    LPADAPTER   AdapterObject,
    ULONG	*Value
);

void RTLMP_API
SetBtPinValue(
    LPADAPTER   AdapterObject,
    ULONG	Value
);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                            End of MP Export APIs
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//=========================================================================//
//							Integrated API                                 								//
//=========================================================================//

extern BASE_BTMPDLLObject *gBTEfusepGToolModlue;
extern LPADAPTER	gAdapterObject;
extern CONTROL_INFO g_pCtrlInfo;
extern HANDLE		gContTxThreadHandle;

INT32	RTLMP_API	QueryDLLVersion();
PVOID	RTLMP_API	StartDUTwithNID(UINT32 ChipID, UINT32 ChipVersion, CHAR* InstanceId);
PVOID	RTLMP_API	StartDUT(UINT32 ChipID, UINT32 ChipVersion);
BOOL	RTLMP_API	StopDUT();
INT32	RTLMP_API	RegisterRead(UINT32 Offset);
BOOL	RTLMP_API	RegisterWrite(UINT32 Offset, UINT32 Value);
INT32	RTLMP_API	RegisterRead_RF(UINT32 RFPath, UINT32 Offset);
BOOL	RTLMP_API	RegisterWrite_RF(UINT32 RFPath, UINT32 Offset, UINT32 Value);
INT32	RTLMP_API 	GetEFuseUtilization();
INT32	RTLMP_API 	GetEFuseUtilization_BT();
BOOL	RTLMP_API	IsAutoloadSuccessful();

BOOL	RTLMP_API	OpenBTSoloDUT();
BOOL	RTLMP_API	OpenBTSoloDUT_UART(ULONG PortNum, ULONG audRate);
BOOL	RTLMP_API	ConfigureRF(UINT32 Band, UINT32 Bandwidth, UINT32 Channel, UINT32 Rate); 
BOOL	RTLMP_API	SetRFPath(UINT32 RFPath);
BOOL	RTLMP_API	SetAntenna(UINT32 Antenna);
BOOL	RTLMP_API	CloseBTSoloDUT();
BOOL	RTLMP_API	AccelerateTest(BOOLEAN	Enable);

#ifdef __cplusplus    
  }                                                    
#endif 
#endif