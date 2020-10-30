#include "stdafx.h"
#include "RtlFunc.h"
#include "CustomOid.h"

#include "MP_DLLInc.h"
#include <commctrl.h>
#include <windows.h>
#include <windowsx.h>
#include <setupapi.h>
#include "MP819xVCDlg.h"

// ***** Global Variable *****//
CONTROL_BLOCK 		Adapter;
CFont				EfuseFont;
CFont				EfuseFont_Small;
CFont				NormalFont;
CFont				SmallFont;
CString				BBDev_Str;
TCHAR   			szDbgMsg[256];
TCHAR   			szPopMsg[512];
UINT                RxPktNumNoChgCnt;
UINT               	AutoRxWriteFileIndex;
UINT                AutoTxWriteFileIndex;
TEST_MODE			TestItem;
AUTO_TEST_TYPE      AutoTestType;      
HANDLE				G_xhThrd;
BOOL				bOneByteAlign=0;
BOOL				bInfoDlgExist=0;
BOOL				CustomPowerTableStatus=0;
BOOL				TestStop;
BOOL				bInReset, bNeedReset, bInQueryRx, bInInitialGain;
BOOL				bEEByteMode;
BOOL				bEEWiFiMode;

BOOL				TxPwrTrackSwitch;
BOOL				PwrIndexFromEfuse;
BOOL				FixIdxBaseWhenPwrTrack;
BOOL				LED1Switch;
BOOL				LED2Switch;
BOOL                TxThreadInProgress;
BOOL                AutoTestFlag;
BOOL                AutoTestRxOn;
BOOL   	  	   		TxTrackFlagStop;
BOOL               	Done_Flag;
BOOL				bMain;
BOOL				bBTTest;
RT_RF_TYPE_DEF_E	RfMode=RF_2T4R;
ULONG				EfuseUtilize = 0;
BOOL				AutoloadOK;
ULONG				RfPathSwitchMap = BOTH_AVAILABLE;
UINT				RxCombine=0;
UINT				BoardType=0;
ULONG				ValidRfPath = 0;
BOOL				bVHTEnabled = 1;
BOOL				bAltDefAnt = 0;
BOOL				bHPConfig = 0;
RT_FLASH_MODE		FlashMode=FLASH_DISABLE_MODE;

UI_COUNTER_STATISTICS	uistatistics;

UINT            	Channels[14];
UINT                Channels_2_4G_20M[CHANNEL_NUM_MAX];
UINT                ChannelNum_2_4G_20M;
UINT                Channels_2_4G_40M[CHANNEL_NUM_MAX];
UINT                ChannelNum_2_4G_40M;
UINT                Channels_5G_20M[CHANNEL_NUM_MAX];
UINT                ChannelNum_5G_20M;
UINT                Channels_5G_40M[CHANNEL_NUM_MAX];
UINT                ChannelNum_5G_40M;
UINT                Channels_5G_80M[CHANNEL_NUM_MAX];
UINT                ChannelNum_5G_80M;
UINT                Channels_5G_160M[CHANNEL_NUM_MAX];
UINT                ChannelNum_5G_160M;

customizeBW			customBWsetting[2][MAX_SUPPORT_BW_NUM];	


TxSCTuple TxSC_List_20M[TxSC_20M_NUM] = {
	{0, 0, "0"}
};
TxSCTuple TxSC_List_40M[TxSC_40M_NUM] = {
	{0, 0, "0"}, {1, 1, "1"}, {2, 2, "2"}
};
TxSCTuple TxSC_List_80M[TxSC_80M_NUM] = {
	{0, 0, "0"}, {1, 1, "1"}, {2, 2, "2"},
	{3, 3, "3"}, {4, 4, "4"}, {5, 9, "9"},
	{6, 10, "10"}
};


RatePair RatesAll[RATES_ALL_NUM] = {
	{RATE_CCK_1M     , "CCK 1M"   }, {RATE_CCK_2M     , "CCK 2M"   }, {RATE_CCK_55M    , "CCK 5.5M" },
	{RATE_CCK_11M    , "CCK 11M"  }, 
	{RATE_OFDM_6M    , "OFDM 6M"  }, {RATE_OFDM_9M    , "OFDM 9M"  }, {RATE_OFDM_12M   , "OFDM 12M" }, 
	{RATE_OFDM_18M   , "OFDM 18M" }, {RATE_OFDM_24M   , "OFDM 24M" }, {RATE_OFDM_36M   , "OFDM 36M" }, 
	{RATE_OFDM_48M   , "OFDM 48M" }, {RATE_OFDM_54M   , "OFDM 54M" },
	{RATE_MCS0       , "MCS0"     }, {RATE_MCS1       , "MCS1"     }, {RATE_MCS2       , "MCS2"     },
	{RATE_MCS3       , "MCS3"     }, {RATE_MCS4       , "MCS4"     }, {RATE_MCS5       , "MCS5"     },
	{RATE_MCS6       , "MCS6"     }, {RATE_MCS7       , "MCS7"     }, {RATE_MCS8       , "MCS8"     },
	{RATE_MCS9       , "MCS9"     }, {RATE_MCS10      , "MCS10"    }, {RATE_MCS11      , "MCS11"    },
	{RATE_MCS12      , "MCS12"    }, {RATE_MCS13      , "MCS13"    }, {RATE_MCS14      , "MCS14"    },
	{RATE_MCS15      , "MCS15"    }, {RATE_MCS16      , "MCS16"    }, {RATE_MCS17      , "MCS17"    },
	{RATE_MCS18      , "MCS18"    }, {RATE_MCS19      , "MCS19"    }, {RATE_MCS20      , "MCS20"    },
	{RATE_MCS21      , "MCS21"    }, {RATE_MCS22      , "MCS22"    }, {RATE_MCS23      , "MCS23"    },
	{RATE_MCS24      , "MCS24"    }, {RATE_MCS25      , "MCS25"    }, {RATE_MCS26      , "MCS26"    },
	{RATE_MCS27      , "MCS27"    }, {RATE_MCS28      , "MCS28"    }, {RATE_MCS29      , "MCS29"    },
	{RATE_MCS30      , "MCS30"    }, {RATE_MCS31      , "MCS31"    }, {RATE_VHT1SS_MCS0, "Nss1 MCS0"}, 
	{RATE_VHT1SS_MCS1, "Nss1 MCS1"}, {RATE_VHT1SS_MCS2, "Nss1 MCS2"}, {RATE_VHT1SS_MCS3, "Nss1 MCS3"}, 
	{RATE_VHT1SS_MCS4, "Nss1 MCS4"}, {RATE_VHT1SS_MCS5, "Nss1 MCS5"}, {RATE_VHT1SS_MCS6, "Nss1 MCS6"}, 
	{RATE_VHT1SS_MCS7, "Nss1 MCS7"}, {RATE_VHT1SS_MCS8, "Nss1 MCS8"}, {RATE_VHT1SS_MCS9, "Nss1 MCS9"},
	{RATE_VHT2SS_MCS0, "Nss2 MCS0"}, {RATE_VHT2SS_MCS1, "Nss2 MCS1"}, {RATE_VHT2SS_MCS2, "Nss2 MCS2"}, 
	{RATE_VHT2SS_MCS3, "Nss2 MCS3"}, {RATE_VHT2SS_MCS4, "Nss2 MCS4"}, {RATE_VHT2SS_MCS5, "Nss2 MCS5"}, 
	{RATE_VHT2SS_MCS6, "Nss2 MCS6"}, {RATE_VHT2SS_MCS7, "Nss2 MCS7"}, {RATE_VHT2SS_MCS8, "Nss2 MCS8"}, 
	{RATE_VHT2SS_MCS9, "Nss2 MCS9"}, {RATE_VHT3SS_MCS0, "Nss3 MCS0"}, {RATE_VHT3SS_MCS1, "Nss3 MCS1"}, 
	{RATE_VHT3SS_MCS2, "Nss3 MCS2"}, {RATE_VHT3SS_MCS3, "Nss3 MCS3"}, {RATE_VHT3SS_MCS4, "Nss3 MCS4"}, 
	{RATE_VHT3SS_MCS5, "Nss3 MCS5"}, {RATE_VHT3SS_MCS6, "Nss3 MCS6"}, {RATE_VHT3SS_MCS7, "Nss3 MCS7"}, 
	{RATE_VHT3SS_MCS8, "Nss3 MCS8"}, {RATE_VHT3SS_MCS9, "Nss3 MCS9"}, {RATE_VHT4SS_MCS0, "Nss4 MCS0"},
	{RATE_VHT4SS_MCS1, "Nss4 MCS1"}, {RATE_VHT4SS_MCS2, "Nss4 MCS2"}, {RATE_VHT4SS_MCS3, "Nss4 MCS3"}, 
	{RATE_VHT4SS_MCS4, "Nss4 MCS4"}, {RATE_VHT4SS_MCS5, "Nss4 MCS5"}, {RATE_VHT4SS_MCS6, "Nss4 MCS6"}, 
	{RATE_VHT4SS_MCS7, "Nss4 MCS7"}, {RATE_VHT4SS_MCS8, "Nss4 MCS8"}, {RATE_VHT4SS_MCS9, "Nss4 MCS9"} 
};

RatePair Rates_2_4G_20M[Rates_2_4G_20M_NUM] = {
	{RATE_CCK_1M     , "CCK 1M"   }, {RATE_CCK_2M     , "CCK 2M"   }, {RATE_CCK_55M    , "CCK 5.5M" },
	{RATE_CCK_11M    , "CCK 11M"  }, 
	{RATE_OFDM_6M    , "OFDM 6M"  }, {RATE_OFDM_9M    , "OFDM 9M"  }, {RATE_OFDM_12M   , "OFDM 12M" }, 
	{RATE_OFDM_18M   , "OFDM 18M" }, {RATE_OFDM_24M   , "OFDM 24M" }, {RATE_OFDM_36M   , "OFDM 36M" }, 
	{RATE_OFDM_48M   , "OFDM 48M" }, {RATE_OFDM_54M   , "OFDM 54M" },
	{RATE_MCS0       , "MCS0"     }, {RATE_MCS1       , "MCS1"     }, {RATE_MCS2       , "MCS2"     },
	{RATE_MCS3       , "MCS3"     }, {RATE_MCS4       , "MCS4"     }, {RATE_MCS5       , "MCS5"     },
	{RATE_MCS6       , "MCS6"     }, {RATE_MCS7       , "MCS7"     }, {RATE_MCS8       , "MCS8"     },
	{RATE_MCS9       , "MCS9"     }, {RATE_MCS10      , "MCS10"    }, {RATE_MCS11      , "MCS11"    },
	{RATE_MCS12      , "MCS12"    }, {RATE_MCS13      , "MCS13"    }, {RATE_MCS14      , "MCS14"    },
	{RATE_MCS15      , "MCS15"    }, {RATE_MCS16      , "MCS16"    }, {RATE_MCS17      , "MCS17"    },
	{RATE_MCS18      , "MCS18"    }, {RATE_MCS19      , "MCS19"    }, {RATE_MCS20      , "MCS20"    },
	{RATE_MCS21      , "MCS21"    }, {RATE_MCS22      , "MCS22"    }, {RATE_MCS23      , "MCS23"    },
	{RATE_MCS24      , "MCS24"    }, {RATE_MCS25      , "MCS25"    }, {RATE_MCS26      , "MCS26"    },
	{RATE_MCS27      , "MCS27"    }, {RATE_MCS28      , "MCS28"    }, {RATE_MCS29      , "MCS29"    },
	{RATE_MCS30      , "MCS30"    }, {RATE_MCS31      , "MCS31"    }, 
	{RATE_VHT1SS_MCS0, "Nss1 MCS0"}, {RATE_VHT1SS_MCS1, "Nss1 MCS1"}, {RATE_VHT1SS_MCS2, "Nss1 MCS2"}, 
	{RATE_VHT1SS_MCS3, "Nss1 MCS3"}, {RATE_VHT1SS_MCS4, "Nss1 MCS4"}, {RATE_VHT1SS_MCS5, "Nss1 MCS5"}, 
	{RATE_VHT1SS_MCS6, "Nss1 MCS6"}, {RATE_VHT1SS_MCS7, "Nss1 MCS7"}, {RATE_VHT1SS_MCS8, "Nss1 MCS8"}, 
	{RATE_VHT2SS_MCS0, "Nss2 MCS0"}, {RATE_VHT2SS_MCS1, "Nss2 MCS1"}, {RATE_VHT2SS_MCS2, "Nss2 MCS2"}, 
	{RATE_VHT2SS_MCS3, "Nss2 MCS3"}, {RATE_VHT2SS_MCS4, "Nss2 MCS4"}, {RATE_VHT2SS_MCS5, "Nss2 MCS5"}, 
	{RATE_VHT2SS_MCS6, "Nss2 MCS6"}, {RATE_VHT2SS_MCS7, "Nss2 MCS7"}, {RATE_VHT2SS_MCS8, "Nss2 MCS8"}, 
	{RATE_VHT3SS_MCS0, "Nss3 MCS0"}, {RATE_VHT3SS_MCS1, "Nss3 MCS1"}, {RATE_VHT3SS_MCS2, "Nss3 MCS2"}, 
	{RATE_VHT3SS_MCS3, "Nss3 MCS3"}, {RATE_VHT3SS_MCS4, "Nss3 MCS4"}, {RATE_VHT3SS_MCS5, "Nss3 MCS5"}, 
	{RATE_VHT3SS_MCS6, "Nss3 MCS6"}, {RATE_VHT3SS_MCS7, "Nss3 MCS7"}, {RATE_VHT3SS_MCS8, "Nss3 MCS8"}, 
	{RATE_VHT3SS_MCS9, "Nss3 MCS9"}, 
	{RATE_VHT4SS_MCS0, "Nss4 MCS0"}, {RATE_VHT4SS_MCS1, "Nss4 MCS1"}, {RATE_VHT4SS_MCS2, "Nss4 MCS2"}, 
	{RATE_VHT4SS_MCS3, "Nss4 MCS3"}, {RATE_VHT4SS_MCS4, "Nss4 MCS4"}, {RATE_VHT4SS_MCS5, "Nss4 MCS5"}, 
	{RATE_VHT4SS_MCS6, "Nss4 MCS6"}, {RATE_VHT4SS_MCS7, "Nss4 MCS7"}, {RATE_VHT4SS_MCS8, "Nss4 MCS8"}
};

RatePair Rates_2_4G_40M[Rates_2_4G_40M_NUM] = {
	{RATE_MCS0       , "MCS0"     }, {RATE_MCS1       , "MCS1"     }, {RATE_MCS2       , "MCS2"     },
	{RATE_MCS3       , "MCS3"     }, {RATE_MCS4       , "MCS4"     }, {RATE_MCS5       , "MCS5"     },
	{RATE_MCS6       , "MCS6"     }, {RATE_MCS7       , "MCS7"     }, {RATE_MCS8       , "MCS8"     },
	{RATE_MCS9       , "MCS9"     }, {RATE_MCS10      , "MCS10"    }, {RATE_MCS11      , "MCS11"    },
	{RATE_MCS12      , "MCS12"    }, {RATE_MCS13      , "MCS13"    }, {RATE_MCS14      , "MCS14"    },
	{RATE_MCS15      , "MCS15"    }, {RATE_MCS16      , "MCS16"    }, {RATE_MCS17      , "MCS17"    },
	{RATE_MCS18      , "MCS18"    }, {RATE_MCS19      , "MCS19"    }, {RATE_MCS20      , "MCS20"    },
	{RATE_MCS21      , "MCS21"    }, {RATE_MCS22      , "MCS22"    }, {RATE_MCS23      , "MCS23"    },
	{RATE_MCS24      , "MCS24"    }, {RATE_MCS25      , "MCS25"    }, {RATE_MCS26      , "MCS26"    },
	{RATE_MCS27      , "MCS27"    }, {RATE_MCS28      , "MCS28"    }, {RATE_MCS29      , "MCS29"    },
	{RATE_MCS30      , "MCS30"    }, {RATE_MCS31      , "MCS31"    }, {RATE_VHT1SS_MCS0, "Nss1 MCS0"}, 
	{RATE_VHT1SS_MCS1, "Nss1 MCS1"}, {RATE_VHT1SS_MCS2, "Nss1 MCS2"}, {RATE_VHT1SS_MCS3, "Nss1 MCS3"}, 
	{RATE_VHT1SS_MCS4, "Nss1 MCS4"}, {RATE_VHT1SS_MCS5, "Nss1 MCS5"}, {RATE_VHT1SS_MCS6, "Nss1 MCS6"}, 
	{RATE_VHT1SS_MCS7, "Nss1 MCS7"}, {RATE_VHT1SS_MCS8, "Nss1 MCS8"}, {RATE_VHT1SS_MCS9, "Nss1 MCS9"}, 
	{RATE_VHT2SS_MCS0, "Nss2 MCS0"}, {RATE_VHT2SS_MCS1, "Nss2 MCS1"}, {RATE_VHT2SS_MCS2, "Nss2 MCS2"}, 
	{RATE_VHT2SS_MCS3, "Nss2 MCS3"}, {RATE_VHT2SS_MCS4, "Nss2 MCS4"}, {RATE_VHT2SS_MCS5, "Nss2 MCS5"}, 
	{RATE_VHT2SS_MCS6, "Nss2 MCS6"}, {RATE_VHT2SS_MCS7, "Nss2 MCS7"}, {RATE_VHT2SS_MCS8, "Nss2 MCS8"}, 
	{RATE_VHT2SS_MCS9, "Nss2 MCS9"}, {RATE_VHT3SS_MCS0, "Nss3 MCS0"}, {RATE_VHT3SS_MCS1, "Nss3 MCS1"}, 
	{RATE_VHT3SS_MCS2, "Nss3 MCS2"}, {RATE_VHT3SS_MCS3, "Nss3 MCS3"}, {RATE_VHT3SS_MCS4, "Nss3 MCS4"}, 
	{RATE_VHT3SS_MCS5, "Nss3 MCS5"}, {RATE_VHT3SS_MCS6, "Nss3 MCS6"}, {RATE_VHT3SS_MCS7, "Nss3 MCS7"}, 
	{RATE_VHT3SS_MCS8, "Nss3 MCS8"}, {RATE_VHT3SS_MCS9, "Nss3 MCS9"}, {RATE_VHT4SS_MCS0, "Nss4 MCS0"},
	{RATE_VHT4SS_MCS1, "Nss4 MCS1"}, {RATE_VHT4SS_MCS2, "Nss4 MCS2"}, {RATE_VHT4SS_MCS3, "Nss4 MCS3"}, 
	{RATE_VHT4SS_MCS4, "Nss4 MCS4"}, {RATE_VHT4SS_MCS5, "Nss4 MCS5"}, {RATE_VHT4SS_MCS6, "Nss4 MCS6"}, 
	{RATE_VHT4SS_MCS7, "Nss4 MCS7"}, {RATE_VHT4SS_MCS8, "Nss4 MCS8"}, {RATE_VHT4SS_MCS9, "Nss4 MCS9"} 
};

RatePair Rates_5G_20M[Rates_5G_20M_NUM] = {
	{RATE_OFDM_6M    , "OFDM 6M"  }, {RATE_OFDM_9M    , "OFDM 9M"  }, {RATE_OFDM_12M   , "OFDM 12M" }, 
	{RATE_OFDM_18M   , "OFDM 18M" }, {RATE_OFDM_24M   , "OFDM 24M" }, {RATE_OFDM_36M   , "OFDM 36M" }, 
	{RATE_OFDM_48M   , "OFDM 48M" }, {RATE_OFDM_54M   , "OFDM 54M" },
	{RATE_MCS0       , "MCS0"     }, {RATE_MCS1       , "MCS1"     }, {RATE_MCS2       , "MCS2"     },
	{RATE_MCS3       , "MCS3"     }, {RATE_MCS4       , "MCS4"     }, {RATE_MCS5       , "MCS5"     },
	{RATE_MCS6       , "MCS6"     }, {RATE_MCS7       , "MCS7"     }, {RATE_MCS8       , "MCS8"     },
	{RATE_MCS9       , "MCS9"     }, {RATE_MCS10      , "MCS10"    }, {RATE_MCS11      , "MCS11"    },
	{RATE_MCS12      , "MCS12"    }, {RATE_MCS13      , "MCS13"    }, {RATE_MCS14      , "MCS14"    },
	{RATE_MCS15      , "MCS15"    }, {RATE_MCS16      , "MCS16"    }, {RATE_MCS17      , "MCS17"    },
	{RATE_MCS18      , "MCS18"    }, {RATE_MCS19      , "MCS19"    }, {RATE_MCS20      , "MCS20"    },
	{RATE_MCS21      , "MCS21"    }, {RATE_MCS22      , "MCS22"    }, {RATE_MCS23      , "MCS23"    },
	{RATE_MCS24      , "MCS24"    }, {RATE_MCS25      , "MCS25"    }, {RATE_MCS26      , "MCS26"    },
	{RATE_MCS27      , "MCS27"    }, {RATE_MCS28      , "MCS28"    }, {RATE_MCS29      , "MCS29"    },
	{RATE_MCS30      , "MCS30"    }, {RATE_MCS31      , "MCS31"    }, 
	{RATE_VHT1SS_MCS0, "Nss1 MCS0"}, {RATE_VHT1SS_MCS1, "Nss1 MCS1"}, {RATE_VHT1SS_MCS2, "Nss1 MCS2"}, 
	{RATE_VHT1SS_MCS3, "Nss1 MCS3"}, {RATE_VHT1SS_MCS4, "Nss1 MCS4"}, {RATE_VHT1SS_MCS5, "Nss1 MCS5"}, 
	{RATE_VHT1SS_MCS6, "Nss1 MCS6"}, {RATE_VHT1SS_MCS7, "Nss1 MCS7"}, {RATE_VHT1SS_MCS8, "Nss1 MCS8"}, 
	{RATE_VHT2SS_MCS0, "Nss2 MCS0"}, {RATE_VHT2SS_MCS1, "Nss2 MCS1"}, {RATE_VHT2SS_MCS2, "Nss2 MCS2"}, 
	{RATE_VHT2SS_MCS3, "Nss2 MCS3"}, {RATE_VHT2SS_MCS4, "Nss2 MCS4"}, {RATE_VHT2SS_MCS5, "Nss2 MCS5"}, 
	{RATE_VHT2SS_MCS6, "Nss2 MCS6"}, {RATE_VHT2SS_MCS7, "Nss2 MCS7"}, {RATE_VHT2SS_MCS8, "Nss2 MCS8"}, 
	{RATE_VHT3SS_MCS0, "Nss3 MCS0"}, {RATE_VHT3SS_MCS1, "Nss3 MCS1"}, {RATE_VHT3SS_MCS2, "Nss3 MCS2"}, 
	{RATE_VHT3SS_MCS3, "Nss3 MCS3"}, {RATE_VHT3SS_MCS4, "Nss3 MCS4"}, {RATE_VHT3SS_MCS5, "Nss3 MCS5"}, 
	{RATE_VHT3SS_MCS6, "Nss3 MCS6"}, {RATE_VHT3SS_MCS7, "Nss3 MCS7"}, {RATE_VHT3SS_MCS8, "Nss3 MCS8"}, 
	{RATE_VHT3SS_MCS9, "Nss3 MCS9"}, 
	{RATE_VHT4SS_MCS0, "Nss4 MCS0"}, {RATE_VHT4SS_MCS1, "Nss4 MCS1"}, {RATE_VHT4SS_MCS2, "Nss4 MCS2"}, 
	{RATE_VHT4SS_MCS3, "Nss4 MCS3"}, {RATE_VHT4SS_MCS4, "Nss4 MCS4"}, {RATE_VHT4SS_MCS5, "Nss4 MCS5"}, 
	{RATE_VHT4SS_MCS6, "Nss4 MCS6"}, {RATE_VHT4SS_MCS7, "Nss4 MCS7"}, {RATE_VHT4SS_MCS8, "Nss4 MCS8"}
};

RatePair Rates_5G_40M[Rates_5G_40M_NUM] = {
	{RATE_MCS0       , "MCS0"     }, {RATE_MCS1       , "MCS1"     }, {RATE_MCS2       , "MCS2"     },
	{RATE_MCS3       , "MCS3"     }, {RATE_MCS4       , "MCS4"     }, {RATE_MCS5       , "MCS5"     },
	{RATE_MCS6       , "MCS6"     }, {RATE_MCS7       , "MCS7"     }, {RATE_MCS8       , "MCS8"     },
	{RATE_MCS9       , "MCS9"     }, {RATE_MCS10      , "MCS10"    }, {RATE_MCS11      , "MCS11"    },
	{RATE_MCS12      , "MCS12"    }, {RATE_MCS13      , "MCS13"    }, {RATE_MCS14      , "MCS14"    },
	{RATE_MCS15      , "MCS15"    }, {RATE_MCS16      , "MCS16"    }, {RATE_MCS17      , "MCS17"    },
	{RATE_MCS18      , "MCS18"    }, {RATE_MCS19      , "MCS19"    }, {RATE_MCS20      , "MCS20"    },
	{RATE_MCS21      , "MCS21"    }, {RATE_MCS22      , "MCS22"    }, {RATE_MCS23      , "MCS23"    },
	{RATE_MCS24      , "MCS24"    }, {RATE_MCS25      , "MCS25"    }, {RATE_MCS26      , "MCS26"    },
	{RATE_MCS27      , "MCS27"    }, {RATE_MCS28      , "MCS28"    }, {RATE_MCS29      , "MCS29"    },
	{RATE_MCS30      , "MCS30"    }, {RATE_MCS31      , "MCS31"    }, {RATE_VHT1SS_MCS0, "Nss1 MCS0"}, 
	{RATE_VHT1SS_MCS1, "Nss1 MCS1"}, {RATE_VHT1SS_MCS2, "Nss1 MCS2"}, {RATE_VHT1SS_MCS3, "Nss1 MCS3"}, 
	{RATE_VHT1SS_MCS4, "Nss1 MCS4"}, {RATE_VHT1SS_MCS5, "Nss1 MCS5"}, {RATE_VHT1SS_MCS6, "Nss1 MCS6"}, 
	{RATE_VHT1SS_MCS7, "Nss1 MCS7"}, {RATE_VHT1SS_MCS8, "Nss1 MCS8"}, {RATE_VHT1SS_MCS9, "Nss1 MCS9"}, 
	{RATE_VHT2SS_MCS0, "Nss2 MCS0"}, {RATE_VHT2SS_MCS1, "Nss2 MCS1"}, {RATE_VHT2SS_MCS2, "Nss2 MCS2"}, 
	{RATE_VHT2SS_MCS3, "Nss2 MCS3"}, {RATE_VHT2SS_MCS4, "Nss2 MCS4"}, {RATE_VHT2SS_MCS5, "Nss2 MCS5"}, 
	{RATE_VHT2SS_MCS6, "Nss2 MCS6"}, {RATE_VHT2SS_MCS7, "Nss2 MCS7"}, {RATE_VHT2SS_MCS8, "Nss2 MCS8"}, 
	{RATE_VHT2SS_MCS9, "Nss2 MCS9"}, {RATE_VHT3SS_MCS0, "Nss3 MCS0"}, {RATE_VHT3SS_MCS1, "Nss3 MCS1"}, 
	{RATE_VHT3SS_MCS2, "Nss3 MCS2"}, {RATE_VHT3SS_MCS3, "Nss3 MCS3"}, {RATE_VHT3SS_MCS4, "Nss3 MCS4"}, 
	{RATE_VHT3SS_MCS5, "Nss3 MCS5"}, {RATE_VHT3SS_MCS6, "Nss3 MCS6"}, {RATE_VHT3SS_MCS7, "Nss3 MCS7"}, 
	{RATE_VHT3SS_MCS8, "Nss3 MCS8"}, {RATE_VHT3SS_MCS9, "Nss3 MCS9"}, {RATE_VHT4SS_MCS0, "Nss4 MCS0"},
	{RATE_VHT4SS_MCS1, "Nss4 MCS1"}, {RATE_VHT4SS_MCS2, "Nss4 MCS2"}, {RATE_VHT4SS_MCS3, "Nss4 MCS3"}, 
	{RATE_VHT4SS_MCS4, "Nss4 MCS4"}, {RATE_VHT4SS_MCS5, "Nss4 MCS5"}, {RATE_VHT4SS_MCS6, "Nss4 MCS6"}, 
	{RATE_VHT4SS_MCS7, "Nss4 MCS7"}, {RATE_VHT4SS_MCS8, "Nss4 MCS8"}, {RATE_VHT4SS_MCS9, "Nss4 MCS9"} 
};

RatePair Rates_5G_80M[Rates_5G_80M_NUM] = {
	{RATE_VHT1SS_MCS0, "Nss1 MCS0"}, {RATE_VHT1SS_MCS1, "Nss1 MCS1"}, {RATE_VHT1SS_MCS2, "Nss1 MCS2"}, 
	{RATE_VHT1SS_MCS3, "Nss1 MCS3"}, {RATE_VHT1SS_MCS4, "Nss1 MCS4"}, {RATE_VHT1SS_MCS5, "Nss1 MCS5"}, 
	{RATE_VHT1SS_MCS6, "Nss1 MCS6"}, {RATE_VHT1SS_MCS7, "Nss1 MCS7"}, {RATE_VHT1SS_MCS8, "Nss1 MCS8"}, 
	{RATE_VHT1SS_MCS9, "Nss1 MCS9"}, 
	{RATE_VHT2SS_MCS0, "Nss2 MCS0"}, {RATE_VHT2SS_MCS1, "Nss2 MCS1"}, {RATE_VHT2SS_MCS2, "Nss2 MCS2"}, 
	{RATE_VHT2SS_MCS3, "Nss2 MCS3"}, {RATE_VHT2SS_MCS4, "Nss2 MCS4"}, {RATE_VHT2SS_MCS5, "Nss2 MCS5"}, 
	{RATE_VHT2SS_MCS6, "Nss2 MCS6"}, {RATE_VHT2SS_MCS7, "Nss2 MCS7"}, {RATE_VHT2SS_MCS8, "Nss2 MCS8"}, 
	{RATE_VHT2SS_MCS9, "Nss2 MCS9"}, 
	{RATE_VHT3SS_MCS0, "Nss3 MCS0"}, {RATE_VHT3SS_MCS1, "Nss3 MCS1"}, {RATE_VHT3SS_MCS2, "Nss3 MCS2"}, 
	{RATE_VHT3SS_MCS3, "Nss3 MCS3"}, {RATE_VHT3SS_MCS4, "Nss3 MCS4"}, {RATE_VHT3SS_MCS5, "Nss3 MCS5"}, 
	{RATE_VHT3SS_MCS7, "Nss3 MCS7"}, {RATE_VHT3SS_MCS8, "Nss3 MCS8"}, {RATE_VHT3SS_MCS9, "Nss3 MCS9"}, 
	{RATE_VHT4SS_MCS0, "Nss4 MCS0"}, {RATE_VHT4SS_MCS1, "Nss4 MCS1"}, {RATE_VHT4SS_MCS2, "Nss4 MCS2"}, 
	{RATE_VHT4SS_MCS3, "Nss4 MCS3"}, {RATE_VHT4SS_MCS4, "Nss4 MCS4"}, {RATE_VHT4SS_MCS5, "Nss4 MCS5"}, 
	{RATE_VHT4SS_MCS6, "Nss4 MCS6"}, {RATE_VHT4SS_MCS7, "Nss4 MCS7"}, {RATE_VHT4SS_MCS8, "Nss4 MCS8"}, 
	{RATE_VHT4SS_MCS9, "Nss4 MCS9"} 
};


UCHAR	Patterns[17] = {
	0x00, 0x05, 0x0a, 0x0f,
	0x50, 0x55, 0x5a, 0x5f,
	0xa0, 0xa5, 0xaa, 0xaf,
	0xf0, 0xf5, 0xfa, 0xff,
	0x12
	};

BYTE	UnicastDID[6] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};


BOOL 
DeInitAdapter2k(
	)
/*++

Routine Description:

	This routine free the found adapter resource.

Arguments:
	
Return Value:

	Result - TRUE: Success.

--*/
{
	GlobalUnlock(Adapter.hMem2);
	GlobalFree(Adapter.hMem2);		
	GlobalUnlock(Adapter.hMem);
	GlobalFree(Adapter.hMem);

	return TRUE;
}

UCHAR 
Asc2Hex(
	IN	CHAR	ASCCode
	)
/*++

Routine Description:

	This routine translate ASCII code to Hex number.

Arguments:

	ASCCode - ASCII code
	
Return Value:

	UCHAR - Hex number.

--*/
{
    if ( (ASCCode <= '9') && (ASCCode >= '0') ) {
       return  (ASCCode-'0');
    } else {
       if ( (ASCCode <= 'f') && (ASCCode >= 'a') ) {
          return  (ASCCode-'a'+10);
       } else {
          if ( (ASCCode <= 'F') && (ASCCode >= 'A') ) {
             return  (ASCCode-'A'+10);
          } else {
			 if (ASCCode == ' ')
				 return 0xEE;
			 else
				return  0xFF;
          }
       }
    }
}

char *
HexToString(
	IN	UCHAR		*in,
	IN	int			inlen,
	IN	char			*out,
	IN	int			outlen,
	IN	bool			IsNum
	)
/*++

Routine Description:

	This routine translate hex to string.

Arguments:
	in - Input hex.
	inlen - Hex length.
	out - Output string pointer.
	outlen - Translated charaters number.
	IsNum - The input hex is number.	
	
Return Value:

	Result - char *: Pointer to the string.

--*/
{
	int i,j=outlen;
	char Hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	memset(out,'0',outlen);
	out[outlen]=0;

	if(IsNum)
	{
		for(i=0;i<inlen && j>0;i++)
		{
			out[--j]=Hex[in[i]&0x0f];
			out[--j]=Hex[(in[i]>>4)&0x0f];
		}
	}
	else
	{
		for(i=inlen-1;i>=0 && j>0;i--)
		{
			out[--j]=Hex[in[i]&0x0f];
			out[--j]=Hex[(in[i]>>4)&0x0f];
		}
	}

	return out;
}


BOOL
CheckAdd(
	IN	TCHAR	*TempBuffer,
	IN	UINT		BuffLen
	)
/*++

Routine Description:

	This routine check if the input string is valid.

Arguments:
	TempBuffer - Input string.
	BuffLen - String buffer length.	
	
Return Value:

	Result - TRUE: This address is valid.

--*/
{ 
	int	i;

	for (i=0; i < (int)BuffLen;i++)
	{
		if(Asc2Hex((CHAR)TempBuffer[i]) == 0xff)
			return FALSE;
	}
	return TRUE;
}




