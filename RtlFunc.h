#ifndef __RTLFUNC
#define __RTLFUNC

#include <tchar.h>
#include "MP819xVC.h"
#include "Feature.h"


#define IN
#define OUT
#define PROT_DRV_PREFIX TEXT("\\Device\\mp8185s_")
#define RTL8187B_CHIP_ID       		0x8187
#define RTL8187B_CHIP_VER       	0x2
#define START                  		1
#define STOP                    	0
#define INPUTLEN                	26
#define TX_PWR_NUM              	42	//28

#define MaxIdlePollingTimes     	5   // 500ms
#define AutoTestProcessTx_TIMER		0x0ade0001
#define UPDATE_RX_STAT_TIMER		0x0bda0001
#define UPDATE_WPSButtonPush_TIMER	0x0bda0002
#define UPDATE_HWRfOnOff_TIMER		0x0bda0003
#define DUT_MONITOR					0x0bda0004
#define IO_MONITOR					0x0bda0005
#define DIALOG_MONITOR				0x0bda0006
#define DEV_BB_ADCSMP_POLLING		0x0bda0007
#define UPDATE_TX_STAT_TIMER		0x0bda0008
#define UPDATE_ACK_COUNTER_TIMER	0x0bda0009
#define RSSI_MONITOR				0x0bda000a

//for EEPROM_93C56
#define TxPowerNumAC				12             //modified by vivi, for eeprom version 1.0, 20080219



#define IS_RF_2G_20M(_band, _bandwidth)	((_band == WIRELESS_MODE_N_24G)\
										&& ((_bandwidth == MPT_BW_20MHZ)|| (_bandwidth == MPT_BW_10MHZ)|| (_bandwidth == MPT_BW_5MHZ))\
										)
#define IS_RF_2G_40M(_band, _bandwidth)	((_band == WIRELESS_MODE_N_24G)\
										&& (_bandwidth == MPT_BW_40MHZ))
#define IS_RF_5G_20M(_band, _bandwidth)	(((_band == WIRELESS_MODE_N_5G)||(_band == WIRELESS_MODE_AC_5G))\
										&& ((_bandwidth == MPT_BW_20MHZ)|| (_bandwidth == MPT_BW_10MHZ)|| (_bandwidth == MPT_BW_5MHZ))\
										)
#define IS_RF_5G_40M(_band, _bandwidth)	(((_band == WIRELESS_MODE_N_5G)||(_band == WIRELESS_MODE_AC_5G))\
										&& (_bandwidth == MPT_BW_40MHZ))
#define IS_RF_5G_80M(_band, _bandwidth)	(((_band == WIRELESS_MODE_N_5G)||(_band == WIRELESS_MODE_AC_5G))\
										&& (_bandwidth == MPT_BW_80MHZ))


#define NONE_TEST               0
#define TX_TEST                 1
#define RX_TEST                 2
#define CONT_TEST               3
#define SINGLETONE_TEST         4
#define CARRIERSUPPRESSION_TEST 5

//
// Internal developement flags.
//
#define PACKET_TX_LONGER_IFG	0 // 1 for longer inter-frame gap.

typedef enum    _UI_EFUSE_COMBO{
    TARGET_FILE,
	TARGET_EFUSE,
	TARGET_BYTE
}UI_EFUSE_COMBO;

typedef struct _UI_COUNTER_STATISTICS {
    UINT		ctr_G_xhThrd_PacketsTx;
    UINT        ctr_threadFreeOk_PacketsTx;
    UINT		ctr_G_xhThrd_ContinuousTx;
    UINT		ctr_threadFreeOk_ContinuousTx;
    UINT		ctr_G_xhThrd_CarrierSupTx;
    UINT		ctr_threadFreeOk_CarrierSupTx;
    UINT		ctr_PacketAllocatePacket_Ok;
    UINT		ctr_PacketAllocatePacket_Fail;
    UINT		ctr_PacketInitPacket_Ok;
    UINT		ctr_PacketInitPacket_Fail;
    UINT		ctr_PacketSendPacket_Ok;
    UINT		ctr_PacketSendPacket_Fail;
    UINT		ctr_PacketFreePacket_Ok;
    UINT		ctr_PacketFreePacket_Fail;
}UI_COUNTER_STATISTICS , *PUI_COUNTER_STATISTICS;


//======================================
typedef enum    _RF_TYPE{
    RF_TYPE_MIN,    //0
    RF_INTERSIL,    //1
    RF_RFMD2948,    //2
    RF_PHILIPS,     //3
    RF_MAXIM2820,   //4
    RF_GCT,         //5
    RF_RFMD2958,    //6
    RF_MAXIM2822,   //7
    RF_MAXIM2825,   //8
    RF_ZEBRA,       //9
    RF_OMC8255,     //10
    RF_8256,        //11
    RF_8258,        //12
    RF_6052, 		//13
    RF_PSEUDO_11N,  //14
}RF_TYPE_E;
// 2008/07/22 MH Copy from MP UI and redefine to compatabile with nornal drv
// We must use the definiton to replace above enum later
typedef enum    _MP_UI_RF_TYPE_DEFINITION_{
    MP_RF_TYPE_MIN,    //0
    MP_RF_INTERSIL,    //1
    MP_RF_RFMD2948,    //2
    MP_RF_PHILIPS,     //3
    MP_RF_MAXIM2820,   //4
    MP_RF_GCT,         //5
    MP_RF_RFMD2958,    //6
    MP_RF_MAXIM2822,   //7
    MP_RF_MAXIM2825,   //8
    // For 9X series.
    MP_RF_ZEBRA,       //9
    MP_RF_OMC8255,     //10
    MP_RF_8256,        //11
    MP_RF_8258,        //12
    MP_RF_PSEUDO_11N,  //13
}MP_RF_TYPE_E;

//======================================
typedef enum
{
	HID_8185     ,
	HID_8185B    ,
	HID_8187     ,
	HID_8187B    ,
	HID_8187SU	 ,
	HID_8187SE	 ,
	HID_8190     ,
	HID_8192E 	 ,
	HID_8192U 	 ,
	HID_8192SE   ,
	HID_8192SU   ,
	HID_8192CE   ,
	HID_8192CU   ,
	HID_8192DE   ,
	HID_8192DU   ,
	HID_8723AE   ,
	HID_8723AU   ,
	HID_8723AS   ,
	HID_8188EE   ,
	HID_8188EU   ,
	HID_8188ES   ,
	HID_8812AE   ,
	HID_8812AU   ,
	HID_8812AS   ,    
	HID_8821AE   ,
	HID_8821AU   ,
	HID_8821AS   ,    	
	HID_8192EE   ,
	HID_8192EU   ,
	HID_8192ES   ,    	
	HID_8723BE   ,
	HID_8723BU   ,
	HID_8723BS   ,  
	HID_8814AE   ,
	HID_8814AU   ,
	HID_8814AS   ,
	HID_8821BE   ,
	HID_8821BU   ,
	HID_8821BS   , 
	HID_8822BE   ,
	HID_8822BU   ,
	HID_8822BS   , 
	HID_8703BE   ,
	HID_8703BU   ,
	HID_8703BS   , 
	HID_8188FE   ,
	HID_8188FU   ,
	HID_8188FS   , 
	HID_8723DE   ,
	HID_8723DU   ,
	HID_8723DS   , 
	HID_8821CE   ,
	HID_8821CU   ,
	HID_8821CS   , 
	HID_8710BE   ,
	HID_8710BU   ,
	HID_8710BS   ,
	HID_8192FE   ,
	HID_8192FU   ,
	HID_8192FS   ,
	HID_8725AU   ,
	HID_8725AS   ,
	HID_8725AE   ,
	HID_8731AU   ,
	HID_8733AS   ,
}HardwareID;

typedef enum
{
    ON_CHANNEL_CHANGED,
    ON_BANDWIDTH_CHANGED,
    ON_RATE_CHANGED,
	ON_MODULATION_CHANGED,
} ON_RF_CHANGE;


#define IS_HARDWARE_SERIES(_series) ((Adapter.HardwareId == HID_##_series##E \
                                   || Adapter.HardwareId == HID_##_series##U \
                                   || Adapter.HardwareId == HID_##_series##S) ? TRUE : FALSE)
#define IS_HARDWARE_8731AU ( Adapter.HardwareId == HID_8731AU ? TRUE : FALSE)

#define IS_11AC_SERIES() (IS_HARDWARE_SERIES(8812A) \
							|| IS_HARDWARE_SERIES(8821A) \
							|| IS_HARDWARE_SERIES(8821C) \
							|| IS_HARDWARE_SERIES(8822B) \
							|| IS_HARDWARE_SERIES(8814A) \
							|| IS_HARDWARE_8731AU) 

#define IS_11N_SERIES()  (!(IS_11AC_SERIES()))

                                   
//======================================
typedef enum
{
	EEPROM_93C46 = 0x1,
	EEPROM_93C56,
	EEPROM_BOOT_EFUSE, // Using this enum number temporally to backward compactable DLL. Added by Roger, 2008.11.10.
}EEPROM_TYPE;

typedef enum
{
	INTF_SEL0_USB 			  	= 0,		// USB
	INTF_SEL1_USB_High_Power  		= 1,		// USB with high power PA
	INTF_SEL2_MINICARD		  	= 2,		// Minicard
	INTF_SEL3_USB_Solo 		 	= 3,		// USB solo-Slim module
	INTF_SEL4_USB_Combo			= 4,		// USB Combo-Slim module
}USB_BOARD_TYPE;

#define EEPROM_To_TxPowers		0
#define TxPowers_To_EEPROM		1
//======================================
typedef enum
{
	Band_24G = 0x0,
	Band_5G,
}BandType;
//======================================
typedef enum
{
	RfPath_A = 0x0,
	RfPath_B,
	RfPath_C,
	RfPath_D
}RF_Path;
//======================================
typedef enum    _HOST_TYPE{
    HOST_USB,    //0
    HOST_UART,    //1
    HOST_PCIE,    //2
}HOST_TYPE_E;

typedef enum
{
	RfOn = 0,		// Rf on 
	RfOff = 1,		// Rf off
}RF_POWER_STATE;


//======================================
/* Tx power index range */
#define MIN_TX_PWR_INDEX_B_MODE 0 
#define MAX_TX_PWR_INDEX_B_MODE 15
#define MIN_TX_PWR_INDEX_G_MODE 0 
#define MAX_TX_PWR_INDEX_G_MODE 15
#define MIN_TX_PWR_INDEX_A_MODE 0 
#define MAX_TX_PWR_INDEX_A_MODE 35
#define MIN_TX_PWR_INDEX_N_MODE 0 
// For 90P/92 series
#define MAX_TXPWR_IDX_NMODE_90P 36
// For 92S
#define MAX_TXPWR_IDX_NMODE_92S 63
//======================================
/* Adapter.CurrentModulation */

//2 Defined in Typedef.h
typedef enum _WIRELESS_MODE {
        WIRELESS_MODE_A     = 0x01,
        WIRELESS_MODE_B     = 0x02,
        WIRELESS_MODE_G     = 0x04,
        WIRELESS_MODE_AUTO  = 0x08,
        WIRELESS_MODE_N_24G = 0x10,
        WIRELESS_MODE_N_5G  = 0x20,
        WIRELESS_MODE_AC_5G		= 0x40,
		WIRELESS_MODE_AC_24G	= 0x80,
		WIRELESS_MODE_AC_ONLY	= 0x100,
		WIRELESS_MODE_MAX		= 0x800
} WIRELESS_MODE;

//2 Modulation Index
//#define WIRELESS_INDEX_A		3	//not supported now
//#define WIRELESS_INDEX_B		0   //not supported now
#define WIRELESS_INDEX_24G	    0
#define WIRELESS_INDEX_5G	    1
#define WIRELESS_INDEX_UNKNOWN  2

typedef enum _RFPATH_SETTING {
        RFPATH_2G_BTG		=	0,
        RFPATH_2G_WLG		=	1,
        RFPATH_5G_WLA		=	2,
		RFPATH_BLUETOOTH	=	3
} RFPATH_SETTING;

typedef enum _ANT_NUM_MAP {
		BOTH_AVAILABLE 		= 0x1,
		ONLY_ANT1			= 0x2,
		ONLY_ANT2			= 0x3,
		DONT_CARE			= 0x4
} ANT_NUM_MAP;


//4 <Note> The following enumeration order must be matched with _MPT_RATE_INDEX.
typedef enum _MPT_RATE{
	/* CCK rate. Total: 4*/
	RATE_CCK_1M = 1, 
	RATE_CCK_2M,
	RATE_CCK_55M,
	RATE_CCK_11M,
	
	/* OFDM rate. Total: 8*/
	RATE_OFDM_6M,       //#5
	RATE_OFDM_9M,
	RATE_OFDM_12M,
	RATE_OFDM_18M,
	RATE_OFDM_24M,
	RATE_OFDM_36M,      //#10
	RATE_OFDM_48M,
	RATE_OFDM_54M,
	
	/* HT rate.  Total: 16*/
	RATE_MCS0,
	RATE_MCS1,
	RATE_MCS2,          //#15
	RATE_MCS3,
	RATE_MCS4,
	RATE_MCS5,
	RATE_MCS6,
	RATE_MCS7,          //#20
	RATE_MCS8,
	RATE_MCS9,
	RATE_MCS10,
	RATE_MCS11,
	RATE_MCS12,         //#25
	RATE_MCS13,
	RATE_MCS14,
	RATE_MCS15,
	RATE_MCS16,
	RATE_MCS17,         //#30
	RATE_MCS18,
	RATE_MCS19,
	RATE_MCS20,
	RATE_MCS21,
	RATE_MCS22,         //#35
	RATE_MCS23,
	RATE_MCS24,
	RATE_MCS25,
	RATE_MCS26,
	RATE_MCS27,         //#40
	RATE_MCS28,
	RATE_MCS29,
	RATE_MCS30,
	RATE_MCS31,

	
	/* VHT rate. Total: 20*/
	RATE_VHT1SS_MCS0 = 100, // To reserve MCS16~MCS31, the index starts from #100.
	RATE_VHT1SS_MCS1,   //#101
	RATE_VHT1SS_MCS2,
	RATE_VHT1SS_MCS3,
	RATE_VHT1SS_MCS4,
	RATE_VHT1SS_MCS5,
	RATE_VHT1SS_MCS6,   //#106
	RATE_VHT1SS_MCS7,
	RATE_VHT1SS_MCS8,
	RATE_VHT1SS_MCS9,
	RATE_VHT2SS_MCS0,
	RATE_VHT2SS_MCS1,   //#111
	RATE_VHT2SS_MCS2,
	RATE_VHT2SS_MCS3,
	RATE_VHT2SS_MCS4,
	RATE_VHT2SS_MCS5,
	RATE_VHT2SS_MCS6,   //#116
	RATE_VHT2SS_MCS7,
	RATE_VHT2SS_MCS8,
	RATE_VHT2SS_MCS9,
	RATE_VHT3SS_MCS0,
	RATE_VHT3SS_MCS1,	// #121
	RATE_VHT3SS_MCS2,
	RATE_VHT3SS_MCS3,
	RATE_VHT3SS_MCS4,
	RATE_VHT3SS_MCS5,
	RATE_VHT3SS_MCS6,	// #126
	RATE_VHT3SS_MCS7,
	RATE_VHT3SS_MCS8,
	RATE_VHT3SS_MCS9,
	RATE_VHT4SS_MCS0,
	RATE_VHT4SS_MCS1,	// #131
	RATE_VHT4SS_MCS2,
	RATE_VHT4SS_MCS3,
	RATE_VHT4SS_MCS4,
	RATE_VHT4SS_MCS5,
	RATE_VHT4SS_MCS6,	// #136
	RATE_VHT4SS_MCS7,
	RATE_VHT4SS_MCS8,
	RATE_VHT4SS_MCS9,
	RATE_LAST
	 
}MPT_RATE_E, *PMPT_RATE_E;

//2 MACRO used by SetRegByChannelForTest
#define IS_CCK_RATE(_value)     (RATE_CCK_1M <= _value && _value <= RATE_CCK_11M)
#define IS_OFDM_RATE(_value)    (RATE_OFDM_6M <= _value && _value <= RATE_OFDM_54M)
#define IS_HT_RATE(_value)     (RATE_MCS0 <= _value && _value <= RATE_MCS31)
#define IS_HT_1S_RATE(_value)  (RATE_MCS0 <= _value && _value <= RATE_MCS7)
#define IS_HT_2S_RATE(_value)  (RATE_MCS8 <= _value && _value <= RATE_MCS15)
#define IS_HT_3S_RATE(_value)  (RATE_MCS16 <= _value && _value <= RATE_MCS23)
#define IS_HT_4S_RATE(_value)  (RATE_MCS24 <= _value && _value <= RATE_MCS31)

#define IS_VHT_RATE(_value)     (RATE_VHT1SS_MCS0 <= _value && _value <= RATE_VHT4SS_MCS9)
#define IS_VHT_1S_RATE(_value)  (RATE_VHT1SS_MCS0 <= _value && _value <= RATE_VHT1SS_MCS9)
#define IS_VHT_1S_ES_RATE(_value)  (RATE_VHT1SS_MCS8 <= _value && _value <= RATE_VHT1SS_MCS9)
#define IS_VHT_2S_RATE(_value)  (RATE_VHT2SS_MCS0 <= _value && _value <= RATE_VHT2SS_MCS9)
#define IS_VHT_3S_RATE(_value)  (RATE_VHT3SS_MCS0 <= _value && _value <= RATE_VHT3SS_MCS9)
#define IS_VHT_4S_RATE(_value)  (RATE_VHT4SS_MCS0 <= _value && _value <= RATE_VHT4SS_MCS9)

#define IS_2SS_RATE(_rate) ((RATE_MCS8 <= _rate && _rate <= RATE_MCS15) ||\
							(RATE_VHT2SS_MCS0<= _rate && _rate <= RATE_VHT2SS_MCS9))
#define IS_3SS_RATE(_rate) ((RATE_MCS16 <= _rate && _rate <= RATE_MCS23) ||\
							(RATE_VHT3SS_MCS0<= _rate && _rate <= RATE_VHT3SS_MCS9))
#define IS_4SS_RATE(_rate) ((RATE_MCS24 <= _rate && _rate <= RATE_MCS31) ||\
							(RATE_VHT4SS_MCS0<= _rate && _rate <= RATE_VHT4SS_MCS9))


//======================================
/* Adapter.PacketType */
typedef enum _PKT_TYPE {
        Normal_PKT,
        NDP_PKT	  
} PKT_TYPE;
//======================================
/* Adapter.PacketPreamble */
typedef enum _PREAMBLE {
        Long_Preamble	= 0x01,
        Short_Preamble  	  ,
        Long_GI     		  ,
        Short_GI  		
} PREAMBLE;


//======================================
/* Adapter.CurrentBandWidth */
typedef enum _MPT_BANDWIDTH{
	MPT_BW_20MHZ = 0,				// 20MHz
	MPT_BW_40MHZ_DUPLICATE = 1,	// 40MHz
	MPT_BW_40MHZ_ABOVE = 2,		// Primary above
	MPT_BW_40MHZ_BELOW = 3,		// Primary below
	MPT_BW_40MHZ = 4,				// 40MHz
	MPT_BW_80MHZ = 5,				// 80 MHz
	MPT_BW_80MHZ_20_ABOVE = 6,	// Primary upper lower
	MPT_BW_80MHZ_20_BELOW = 7,	// Primary lower upper
	MPT_BW_80MHZ_20_BOTTOM = 8,	// Primary lower lower
	MPT_BW_80MHZ_20_TOP = 9,		// Primary upper upper
	MPT_BW_80MHZ_40_ABOVE = 10,
	MPT_BW_80MHZ_40_BELOW = 11,
	MPT_BW_5MHZ = 12,
	MPT_BW_10MHZ = 13,
}MPT_BANDWIDTHE, *PMPT_BANDWIDTH;

#define MAX_SUPPORT_BW_NUM	8
//======================================
/* Adapter.CurrentTxSC */
typedef enum _DATA_SC{
	DATA_SC_DONOT_CARE = 0,
	DATA_SC_20_ABOVE_OF_80MHZ = 1,
	DATA_SC_20_BELOW_OF_80MHZ = 2,
	DATA_SC_20_TOP_OF_80MHZ = 3,
	DATA_SC_20_BOTTOM_OF_80MHZ = 4,
	DATA_SC_20_RECV1 = 5,
	DATA_SC_20_RECV2 = 6,
	DATA_SC_20_RECV3 = 7,
	DATA_SC_20_RECV4 = 8,
	DATA_SC_40_ABOVE_OF_80MHZ = 9,
	DATA_SC_40_BELOW_OF_80MHZ = 10,
}DATA_SC, *PDATA_SC_E;
//======================================

/* Adapter.CurrentAntenna */
typedef enum _ANTENNA_PATH{
        ANTENNA_NONE 	= 0x00,
        ANTENNA_D		,
        ANTENNA_C		,
        ANTENNA_CD		,
        ANTENNA_B		,
        ANTENNA_BD		,
        ANTENNA_BC		,
        ANTENNA_BCD		,
        ANTENNA_A		,
        ANTENNA_AD		,
        ANTENNA_AC		,
        ANTENNA_ACD		,
        ANTENNA_AB		,
        ANTENNA_ABD		,
        ANTENNA_ABC		,
        ANTENNA_ABCD	
} ANTENNA_PATH;
/* AntennaTx 2T4R index */
#define AntennaTx_A_Index_2T4R		0
#define AntennaTx_C_Index_2T4R		1
#define AntennaTx_AC_Index_2T4R		2

/* AntennaTx 3T3R index for 8814A */
#define AntennaTx_1_Index_3T3R_Nsts1	0
#define AntennaTx_2_Index_3T3R_Nsts1	1
#define AntennaTx_3_Index_3T3R_Nsts1	2
//#define AntennaTx_Aux_Index_3T3R_Nsts1	3
#define AntennaTx_12_Index_3T3R_Nsts1	3
#define AntennaTx_123_Index_3T3R_Nsts1	4
#define AntennaTx_12_Index_3T3R_Nsts2	0
#define AntennaTx_23_Index_3T3R_Nsts2	1
#define AntennaTx_123_Index_3T3R_Nsts3	0
#define AntennaTx_12_Index_STBC		0
/* AntennaTx 2T2R index for 8814A */
#define AntennaTx_1_Index_2T2R		0
#define AntennaTx_2_Index_2T2R		1
#define AntennaTx_12_Index_2T2R	2
/* AntennaTx 1T2R index */
#define AntennaTx_A_Index_1T2R		0	//for 8192e 1T2R
#define AntennaTx_B_Index_1T2R		1	//for 8192e 1T2R

/* AntennaTx 2T2R index for 8192S */
#define AntennaTx_A_Index_2T2R		0
#define AntennaTx_B_Index_2T2R		1
#define AntennaTx_AB_Index_2T2R		2

/* AntennaTx 1T1R WL/BT index for 8723B*/
#define AntennaTx_S1_Index_2T2R		0
#define AntennaTx_S0_Index_2T2R		1
#define AntennaRx_S1_Index_2T2R		0
#define AntennaRx_S0_Index_2T2R		1

/* AntennaTx 1T1R index for 8192S */
#define AntennaTx_A_Index_1T1R		0	//for 8192S 1T2R

/* AntennaRx 2T4R index for 8190P */
#define AntennaRx_A_Index_2T4R		0
#define AntennaRx_B_Index_2T4R		1
#define AntennaRx_C_Index_2T4R		2
#define AntennaRx_D_Index_2T4R		3
#define AntennaRx_AC_Index_2T4R		4
#define AntennaRx_BD_Index_2T4R		5
#define AntennaRx_ABCD_Index_2T4R	6

/* AntennaRx 3T3R index for 8814A */
#define AntennaRx_Aux_Index_3T3R	0
#define AntennaRx_1_Index_3T3R		1
#define AntennaRx_2_Index_3T3R		2
#define AntennaRx_3_Index_3T3R		3
#define AntennaRx_12_Index_3T3R		4
#define AntennaRx_23_Index_3T3R		5
#define AntennaRx_123_Index_3T3R	6
#define AntennaRx_Aux123_Index_3T3R	7

/* AntennaRx 1T2R index for 8192 E/U*/
#define AntennaRx_A_Index_1T2R		0	
#define AntennaRx_B_Index_1T2R		1	
#define AntennaRx_AB_Index_1T2R		2

/* AntennaRx 2T2R index for 8192S */
#define AntennaRx_A_Index_2T2R		0
#define AntennaRx_B_Index_2T2R		1
#define AntennaRx_AB_Index_2T2R		2
/* AntennaRx 2T2R index for 88814A */
#define AntennaRx_1_Index_2T2R		0
#define AntennaRx_2_Index_2T2R		1
#define AntennaRx_12_Index_2T2R		2
/* AntennaRx 2T3R index for 88814A */
#define AntennaRx_1_Index_2T3R		0
#define AntennaRx_2_Index_2T3R		1
#define AntennaRx_3_Index_2T3R		2
#define AntennaRx_12_Index_2T3R		3
#define AntennaRx_23_Index_2T3R		4
#define AntennaRx_123_Index_2T3R		5

/* AntennaRx 1T1R index for 8192S */
#define AntennaRx_A_Index_1T1R		0

//======================================
/* Adapter.CurrentChannel */
#define ChannelMin40M			3
#define ChannelMax40M			11
#define	Channels_2_4G_NUM		14
#define	Channels_2_4G_20M_NUM		14
#define	Channels_2_4G_40M_NUM		9

#define ChannelMin_5G_20M		36
#define ChannelMax_5G_20M		177
#define ChannelMin_5G_40M		38
#define ChannelMax_5G_40M		175
#define ChannelMin_5G_80M		42
#define ChannelMax_5G_80M		171
#define ChannelMin_5G_160M		50
#define ChannelMax_5G_160M		163

#define	Channels_5G_20M_NUM	    28
#define	Channels_5G_40M_NUM	    14
#define	Channels_5G_80M_NUM	    7
#define	Channels_5G_160M_NUM	3

#define CHANNEL_NUM_MAX		104

extern  UINT            Channels_2_4G_20M[CHANNEL_NUM_MAX];
extern  UINT            ChannelNum_2_4G_20M;
extern  UINT            Channels_2_4G_40M[CHANNEL_NUM_MAX];
extern  UINT            ChannelNum_2_4G_40M;
extern  UINT            Channels_5G_20M[CHANNEL_NUM_MAX];
extern  UINT            ChannelNum_5G_20M;
extern  UINT            Channels_5G_40M[CHANNEL_NUM_MAX];
extern  UINT            ChannelNum_5G_40M;
extern  UINT            Channels_5G_80M[CHANNEL_NUM_MAX];
extern  UINT            ChannelNum_5G_80M;
extern  UINT            Channels_5G_160M[CHANNEL_NUM_MAX];
extern  UINT            ChannelNum_5G_160M;
extern	UINT			BW_2G_20M_Enable;
extern	UINT			BW_2G_40M_Enable;
extern	UINT			BW_5G_20M_Enable;
extern	UINT			BW_5G_40M_Enable;
extern	UINT			BW_5G_80M_Enable;
extern	UINT			BW_5G_160M_Enable;
extern  BOOL			bVHTEnabled;
extern  BOOL			bAltDefAnt;
extern  BOOL			bHPConfig;

typedef	enum _RT_FLASH_MODE
{
	FLASH_DISABLE_MODE,
	FLASH_USER_MODE,
	FLASH_DEBUG_MODE = 0xD,
	FLASH_UNKNOWN_MODE = 0xF
}RT_FLASH_MODE;

typedef	enum _RT_FLASH_DEVICE_TYPE
{
	FLASH_DEVICE_DISK,
	FLASH_DEVICE_CDROM,
	FLASH_DEVICE_UNKNOWN
}RT_FLASH_DEVICE_TYPE;

typedef struct _customizeBW {
	char bw_Str[10];
	UINT bw_en;
	UINT comboxOrder;
	MPT_BANDWIDTHE comboxBW;
}customizeBW;

extern	customizeBW		customBWsetting[2][MAX_SUPPORT_BW_NUM];	

typedef struct _TxSCTuple {
	UINT Index;
    UINT TxSC_Val;
    char TxSC_Str[15];
} TxSCTuple;

#define TxSC_20M_NUM		1
#define TxSC_40M_NUM		3
#define TxSC_80M_NUM        7

extern TxSCTuple TxSC_List_20M[TxSC_20M_NUM];
extern TxSCTuple TxSC_List_40M[TxSC_40M_NUM];
extern TxSCTuple TxSC_List_80M[TxSC_80M_NUM];

typedef struct _RatePair {
    UINT rate;
    char rateStr[15];
} RatePair;

#define RATES_ALL_NUM			84
#define Rates_2_4G_20M_NUM		81
#define Rates_2_4G_40M_NUM      72
#define Rates_5G_20M_NUM		77
#define Rates_5G_40M_NUM		72
#define Rates_5G_80M_NUM        39
extern RatePair RatesAll[RATES_ALL_NUM];
extern RatePair Rates_2_4G_20M[Rates_2_4G_20M_NUM];
extern RatePair Rates_2_4G_40M[Rates_2_4G_40M_NUM];
extern RatePair Rates_5G_20M[Rates_5G_20M_NUM];
extern RatePair Rates_5G_40M[Rates_5G_40M_NUM];
extern RatePair Rates_5G_80M[Rates_5G_80M_NUM];

//======================================
/* Adapter.CurrentTxAGCOffset */
typedef enum _TxAGC_Offset{
        TxAGC_Offset_0 	= 0x00,
        TxAGC_Offset_1		,
        TxAGC_Offset_2		,
        TxAGC_Offset_3		,
        TxAGC_Offset_4		,
        TxAGC_Offset_5		,
        TxAGC_Offset_6		,
        TxAGC_Offset_7		,
        TxAGC_Offset_neg8	,
        TxAGC_Offset_neg7	,
        TxAGC_Offset_neg6	,
        TxAGC_Offset_neg5	,
        TxAGC_Offset_neg4	,
        TxAGC_Offset_neg3	,
        TxAGC_Offset_neg2	,
        TxAGC_Offset_neg1
} TxAGC_Offset;
//======================================
/* Test Item define */
typedef enum _TEST_MODE{
    NONE                  =0x0,
    PACKETS_TX                ,
    PACKETS_RX                ,
    CONTINUOUS_TX             ,
	OFDM_Single_Tone_TX       ,
	CCK_Carrier_Suppression_TX,
	WPS_BUTTON_PUSH,
	HW_RF_ON_OFF,
	BT_TEST
}TEST_MODE;
//======================================
/* Auto test type */
typedef enum _AUTO_TEST_TYPE{
    AUTO_TEST_NONE        =0x0,
    AUTO_TEST_TX_SITE         ,
    AUTO_TEST_RX_SITE         ,
    AUTO_TEST_STOP            ,
}AUTO_TEST_TYPE;
//======================================
typedef struct _MIMO_RSSI{
	UINT	EnableAntenna;
	UINT	AntennaA;
	UINT 	AntennaB;
	UINT 	AntennaC;
	UINT 	AntennaD;
}MIMO_RSSI, *PMIMO_RSSI;
//======================================
/* Index and value convertion */
#define ComboDataRate			0
#define ComboPreamble			1
//#define ComboBandwidth		2
#define ComboModulation			3
#define ComboAntennaTx			4
#define ComboAntennaRx			5
#define ComboChannel			6

#define Index2Value		0
#define Value2Index		1
//======================================
/* RF mode */
// Copy from driver to be RF type def
typedef	enum _RT_RF_TYPE_DEFINITION
{
	RF_1T2R = 0,
	RF_2T4R,
	RF_2T2R,
	RF_1T1R,
	RF_2T2R_GREEN,
	RF_3T3R,
	RF_4T4R,
	RF_2T3R,
	RF_3T4R,
	RF_819X_MAX_TYPE = 0xF
}RT_RF_TYPE_DEF_E;
//======================================
typedef	enum _BUTTON_TYPE_DEFINITION
{
	ButtonWPS = 0,
}BUTTON_TYPE_DEF_E;

// RF state.
typedef	enum _RT_RF_POWER_STATE
{
	eRfOn,		// RF is on after RFSleep or RFOff
	eRfSleep,	// 802.11 Power Save mode
	eRfOff,		// HW/SW Radio OFF or Inactive Power Save
	//=====Add the new RF state above this line=====//
	eRFMax
}RT_RF_POWER_STATE;

//The definition MUST be the same with Driver.
typedef enum _RT_FW_TYPE
{
	RT_FW_NORMAL,
	RT_FW_WOWLAN,
	RT_FW_XBOX,
	RT_FW_FLASH,
}RT_FW_TYPE;

typedef enum _RT_STATUS{
	RT_STATUS_SUCCESS = 0,
	RT_STATUS_FAILURE,
	RT_STATUS_PENDING,
	RT_STATUS_RESOURCE,
	RT_STATUS_INVALID_CONTEXT,
	RT_STATUS_INVALID_PARAMETER,
	RT_STATUS_NOT_SUPPORT,
	RT_STATUS_OS_API_FAILED,
	RT_STATUS_BUFFER_TOO_SHORT,
	RT_STATUS_INVALID_LENGTH,
	RT_STATUS_NOT_RECOGNIZED,
	RT_STATUS_MALFORMED_PKT,
	RT_STATUS_PKT_DROP,
	RT_STATUS_MEDIA_BUSY,
	RT_STATUS_INVALID_DATA,
	RT_STATUS_INVALID_STATE,
	RT_STATUS_IO_EXCEPTION,
	RT_STATUS_RUNNING_LEVEL_TOO_HIGH,
}RT_STATUS,*PRT_STATUS;

typedef struct _CONTROL_BLOCK {
    PVOID       hFile;
    HANDLE      hEvent;
    TCHAR       AdapterName[128]; // For memory leak in _MBCS. 2005.01.28, by rcnjko.

	TCHAR		NetCfgInstanceId[128];	// NIC guid
	UINT		enumId;					// NIC enumid

    HANDLE      hMem;
    LPBYTE      lpMem;

    HGLOBAL     hMem2;
    LPBYTE      lpMem2;
    
    ULONG       LastReadSize;
    UINT        BufferSize;

    UINT        EEType;
    UINT        RFType;
    UINT        HardwareId;
    UCHAR			CurrentMAC[6];
    UINT			CurrentInitialGainValue;

	/*	TX POWER */
    UINT			CurrentTxPower;
    UINT			CurrentTxAGCOffset;	/* D:bit16~23, C:bit8~15, B:bit0~7 */	
    UINT			CurrentCrystalCap;	
	
	UINT			CurrentAntenna;	/* Lower for Rx, upper 16bits for Tx. */

	WIRELESS_MODE	CurrentModulation;
	RFPATH_SETTING	CurrentRFPathSetting;
	RFPATH_SETTING	DefaultRFPathSetting;


	/* TX Setting */
	UCHAR			bSGI:1;
	UCHAR			bLDPC:1;
	UCHAR			bSTBC:1;
    MPT_RATE_E		CurrentRate;
	PKT_TYPE		PacketType;
    UCHAR			PacketPreamble;
	UINT			PacketCount;
	UINT			PacketLength;
	USHORT			PacketPeriod;
	UCHAR			PacketPattern;

	MPT_BANDWIDTHE	CurrentBandWidth;

	DATA_SC			TxSC;

	UINT			CurrentChannel;
    UINT			*Channels;		
    UINT			nTotalChnlCnt;
    UINT			nTotalRateCnt;

	BOOL			bIsMonitorOn;
	UINT			MonitorAddr1;
	UINT			EfuseMapSize;
	BOOL			EnableAllRates;

	RT_PMAC_TX_INFO	PMacTxInfo;
	
	RT_FLASH_MODE		flash_mode;
	ULONG				flash_size;
	RT_FLASH_DEVICE_TYPE	flash_device_type;
	
	UINT				fw_list;
}CONTROL_BLOCK, *PCONTROL_BLOCK;

//for tx power tracking
//
// ***** Extern variables *****//
//
extern  CONTROL_BLOCK   Adapter;
extern  CFont			EfuseFont;
extern  CFont			EfuseFont_Small;
extern	CFont			NormalFont;
extern	CFont			SmallFont;
extern	CString			BBDev_Str;
extern 	TCHAR   		szDbgMsg[256];
extern 	TCHAR   		szPopMsg[512];
extern  UINT            RxPktNumNoChgCnt;
extern  UINT            AutoRxWriteFileIndex;
extern  UINT            AutoTxWriteFileIndex;
extern  TEST_MODE      	TestItem;
extern  AUTO_TEST_TYPE  AutoTestType;       
extern  HANDLE          G_xhThrd;
extern	BOOL			bOneByteAlign;
extern	BOOL			bInfoDlgExist;
extern	BOOL			CustomPowerTableStatus;
extern  BOOL            TestStop;
extern  BOOL            bInReset, bNeedReset, bInQueryRx, bInInitialGain;
extern  BOOL            bEEByteMode;
extern  BOOL			bEEWiFiMode;
extern  BOOL            TxThreadInProgress; 
extern  BOOL            AutoTestFlag;   
extern  BOOL            AutoTestRxOn;   
extern  BOOL			TxPwrTrackSwitch;
extern  BOOL			PwrIndexFromEfuse;
extern	BOOL			FixIdxBaseWhenPwrTrack;
extern  BOOL			LED1Switch;
extern  BOOL			LED2Switch;
extern  BOOL   	   		TxTrackFlagStop;       //added by vivi
extern  BOOL           	bMain;
extern  BOOL            bBTTest;
extern  UINT			RxCombine;
extern  RT_RF_TYPE_DEF_E RfMode;
extern  UINT			BoardType;
extern  ULONG			EfuseUtilize;
extern  BOOL			AutoloadOK;
extern  ULONG			RfPathSwitchMap;
extern  ULONG			ValidRfPath;

extern  UINT            Channels[14];
extern  UCHAR           Patterns[17];
extern  BYTE            UnicastDID[6];

extern	UI_COUNTER_STATISTICS	uistatistics;

/**
 * PrintMsg & PrintMsgErr.
 * PrintMsg displays messages only in debug mode,
 * while PrintMsgErr always displays messages.
 */

#ifdef _DEBUG
	//#if ((_ATL_VER >= 0x1000))
		#define Win32Print(_msg, ...)				\
		do {										\
  			sprintf(szDbgMsg, "[MP UI]: "); OutputDebugString(szDbgMsg);	\
			sprintf(szDbgMsg, _msg, __VA_ARGS__); OutputDebugString(szDbgMsg);\
  			sprintf(szDbgMsg, "                     [%s, line #(%d)]\n", TEXT(__FUNCTION__), __LINE__);OutputDebugString(szDbgMsg);\
		} while(0)
	//#else
	//	#define Win32Print                      (void)sizeof
	//#endif	

	#define PrintMsg(_msg)						\
	do {										\
		OutputDebugString(_T("[MP UI]: "));	\
		OutputDebugString(_msg);				\
	} while(0)
#else
	#define Win32Print                          (void)sizeof
	#define PrintMsg(_msg)
#endif


//
// ***** Rtl Functions *****//
//
BOOL
RtlWlanCard(
    IN  ULONG       ChipID,
    IN  ULONG       ChipVer
    );

BOOL 
DeInitAdapter2k(
    );

UCHAR 
Asc2Hex(
    IN  CHAR        ASCCode
    );

char*
HexToString(
    IN  UCHAR       *in,
    IN  int         inlen,
    OUT char            *out,
    OUT int         outlen,
    IN  bool            IsNum
    );

BOOL
CheckAdd(
    IN  TCHAR   *TempBuffer,
    IN  UINT        BuffLen
    );



#endif  // End #ifndef  __RTLFUNC
