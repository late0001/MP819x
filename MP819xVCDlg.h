#ifndef __MP819XVCDLG
#define __MP819XVCDLG

#include "tester.h"
#include "RtlFunc.h"
#include "IniIO.h"
#include "ColorEdit.h"
#include "ColorStatic.h"
#include "afxwin.h"
#include "wt208_vsa.h"
#include "afxcmn.h"

#if !defined(AFX_MP819xVCDLG_H__ADE5B272_8AF7_4936_805A_352B13850918__INCLUDED_)
#define AFX_MP819xVCDLG_H__ADE5B272_8AF7_4936_805A_352B13850918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma region [MACROS]
#define CONVERT(_trans, _index, _value, _in, _out, _matched)\
            do {\
                if (_trans == Index2Value) {\
                    if (_in == _index) {\
                        _out = _value;\
                        _matched = TRUE;\
                    }\
                } else if (_trans == Value2Index) {\
                    if (_in == _value) {\
                        _out = _index;\
                        _matched = TRUE;\
                    }\
                } else {\
                    _matched = FALSE;\
                }\
            } while(0)
            
#define NEW_CONVERT(_trans, _combobox, _in, _out, _matched)\
        if (_trans == Index2Value) {\
        	_out = _combobox.GetItemData(_in);\
        	_matched = TRUE;\
        } else if (_trans == Value2Index) {\
        	for (int _i = 0; _i < _combobox.GetCount(); ++_i) {\
        		if (_in == _combobox.GetItemData(_i)) {\
        			_out = _i;\
        			_matched = TRUE;\
        		}\
        	}\
        } else {\
        	_matched = TRUE;\
        } 
        
#define BATCH_CONVERT(_trans, _in, _out, _matched, _size, _value)\
            do {\
                if (_trans == Index2Value) {\
                    if (_in < _size) {\
                        _out = _value[_in];\
                        _matched = TRUE;\
                    }\
                } else if (_trans == Value2Index){\
                    for (int i = 0; i < _size; ++i) {\
                        if (_in == _value[i]) {\
                            _out = i;\
                            _matched = TRUE;\
                        }\
                    }\
                } else {\
                    _matched = TRUE;\
                }\
            } while(0)
            
#define ERR_INDEX_OR_VALUE_UNKNOWN(_trans, _errstr)\
            do {\
				CString errorString;\
                if(_trans == Index2Value)\
                    errorString = "Index2Value ";\
                else\
                    errorString = "Value2Index ";\
        	    errorString += "conversion failed!";\
				errorString += _errstr;\
        	    ErrMsgBox(errorString);\
        	    PostMessage(WM_QUIT);\
            } while(0)
#pragma endregion MACROS
/////////////////////////////////////////////////////////////////////////////
// CMP819xVCDlg dialog

typedef struct _ChipStruct {
	UINT 		ChipID;
	UINT 		ChipVer;
	HardwareID 	ChipType;
    TCHAR		ChipName[20];
} ChipStruct;

static ChipStruct namingTable[] = {
		{0x8723, 0x01, HID_8723AE,  "RTL8723AE   "},
		{0x8723, 0x02, HID_8723AU,  "RTL8723AU   "},
		{0x8723, 0x03, HID_8723AS,  "RTL8723AS   "},
		{0x8188, 0x01, HID_8188EE,  "RTL8188EE   "},
		{0x8188, 0x02, HID_8188EU,  "RTL8188EU   "},
		{0x8188, 0x03, HID_8188ES,  "RTL8188ES   "},
		{0x8812, 0x01, HID_8812AE,  "RTL8812AE   "},
		{0x8812, 0x02, HID_8812AU,  "RTL8812AU   "},
		{0x8812, 0x03, HID_8812AS,  "RTL8812AS   "},
		{0x8821, 0x01, HID_8821AE,  "RTL8821AE   "},
		{0x8821, 0x02, HID_8821AU,  "RTL8821AU   "},
		{0x8821, 0x03, HID_8821AS,  "RTL8821AS   "},   
		{0x8192E, 0x01, HID_8192EE,  "RTL8192EE   "},
		{0x8192E, 0x02, HID_8192EU,  "RTL8192EU   "},
		{0x8192E, 0x03, HID_8192ES,  "RTL8192ES   "},   
		{0x8723B, 0x01, HID_8723BE,  "RTL8723BE   "},
		{0x8723B, 0x02, HID_8723BU,  "RTL8723BU   "},
		{0x8723B, 0x03, HID_8723BS,  "RTL8723BS   "}, 
		{0x8814, 0x01, HID_8814AE,  "RTL8814AE   "},
		{0x8814, 0x02, HID_8814AU,  "RTL8814AU   "},
		{0x8814, 0x03, HID_8814AS,  "RTL8814AS   "},
		{0x8821B, 0x01, HID_8821BE,  "RTL8821BE   "},
		{0x8821B, 0x02, HID_8821BU,  "RTL8821BU   "},
		{0x8821B, 0x03, HID_8821BS,  "RTL8821BS   "},
		{0x8822B, 0x01, HID_8822BE,  "RTL8822BE   "},
		{0x8822B, 0x02, HID_8822BU,  "RTL8822BU   "},
		{0x8822B, 0x03, HID_8822BS,  "RTL8822BS   "},
		{0x8703B, 0x01, HID_8703BE,  "RTL8703BE   "},
		{0x8703B, 0x02, HID_8703BU,  "RTL8703BU   "},
		{0x8703B, 0x03, HID_8703BS,  "RTL8703BS   "},
		{0x8188F, 0x01, HID_8188FE,  "RTL8188FE   "},
		{0x8188F, 0x02, HID_8188FU,  "RTL8188FU   "},
		{0x8188F, 0x03, HID_8188FS,  "RTL8188FS   "},
		{0x8723D, 0x01, HID_8723DE,  "RTL8723DE   "},
		{0x8723D, 0x02, HID_8723DU,  "RTL8723DU   "},
		{0x8723D, 0x03, HID_8723DS,  "RTL8723DS   "},
		{0x8821C, 0x01, HID_8821CE,  "RTL8821CE   "},
		{0x8821C, 0x02, HID_8821CU,  "RTL8821CU   "},
		{0x8731, 0x02, HID_8731AU,  "RTL8731AU   "},
		{0x8821C, 0x03, HID_8821CS,  "RTL8821CS   "},
		{0x8733, 0x03,  HID_8733AS, "RTL8733AS    "},
		{0x8710B, 0x01, HID_8710BE,  "RTL8710BE   "},
		{0x8710B, 0x02, HID_8710BU,  "RTL8710BU   "},
		{0x8710B, 0x03, HID_8710BS,  "RTL8710BS   "},		
		{0x8192F, 0x01, HID_8192FE,  "RTL8192FE   "},
		{0x8192F, 0x02, HID_8192FU,  "RTL8192FU   "},
		{0x8192F, 0x03, HID_8192FS,  "RTL8192FS   "},
		{0x8725A, 0x02, HID_8725AU,  "RTL8725AU   "},
		{0x8725A, 0x03, HID_8725AS,  "RTL8725AS   "},
}; 

typedef struct _FW_TYPE {
	RT_FW_TYPE	fw_type;
	TCHAR	fw_name[20];
} FW_TYPE;

static FW_TYPE FW_Table[] = {
		{RT_FW_NORMAL, "Normal FW"},
		{RT_FW_WOWLAN, "WoWlan FW"},
		{RT_FW_XBOX, "XBOX FW"},
		{RT_FW_FLASH, "SPI Flash FW"},
}; 

static
ULONG
__stdcall
ThreadXmit(
	PVOID
	);

#define   WM_UPDATECAPDATA   (WM_USER+1001)
#define   WM_STARTTHREAD     (WM_USER+1002)// ???¡¥
#define   WM_STOPTHREAD      (WM_USER+1003)// ¨ª¡ê?1
#define   WM_AUTORANGE       (WM_USER+1004)

struct VSATHREADDATA
{
	VsaParameter vsaParam;
	char buf[2048];
};
extern VSATHREADDATA vsaThData;

class CMP819xVCDlg : public CDialog
{
// Construction
public:
	CMP819xVCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMP819xVCDlg)
	enum { IDD = IDD_MP819xVC_DIALOG };
	CEdit	m_BT_Rate_text;
	CStatic	m_BT_Rate;
	CStatic	m_BT_Port_No;
	CEdit	m_BT_Port_No_text;
	CButton	m_BT_WiFi;
	CComboBox	m_Xtal_XinXout;
	CComboBox	m_XinXout;
	CButton	m_BT;
	CButton m_Button_DevBB;
	CComboBox	m_TxPowerB;
	CComboBox	m_TxPowerC;	
	CComboBox	m_TxPowerD;

	CStatic		m_TxPowerB_Label;
	CStatic		m_TxPowerC_Label;
	CStatic		m_TxPowerD_Label;

	CStatic	m_EfuseUsed;
	CButton	m_UpdateEfuse;
	CButton	m_Led1;
	CButton	m_TxPowerTrack;
	CComboBox	m_AntennaTx;
	CComboBox	m_AntennaRx;
	CComboBox	m_RFPathSetting;
	CComboBox	m_RFPathSetting_Duplicate;
	CComboBox	m_RfRegRfPath;
	CComboBox	m_FileIndex;
	CEdit	m_PacketLength;
	CStatic	m_Channel_Width_Static;
	CComboBox	m_Channel_Width;
	CComboBox	m_TxSC_Selection;
	CStatic	m_Modulation_Static;
	CStatic	m_Preamble_Static;
	CButton	m_Start;
	CEdit	m_SelfMacAddr;
	CComboBox	m_TxPower;
	CStatic	m_RxAckPktCounter;
	CStatic m_RxAckPktCounter_Label;
	CButton m_RxAckPktCounter_Enable_Btn;
	CButton m_RxFilter_Enable_Btn;
	CStatic	m_XmitOK;
	CStatic	m_RcvOK;
	CStatic	m_RcvPhyOK;
	CStatic	m_RcvPhyErr;
	CButton m_RxPhyOkCheckBox;
	CButton m_RxPhyErrCheckBox;
	CStatic	m_RcvMacOK;
	CStatic	m_RcvMacErr;
	CButton m_RxMacOkCheckBox;
	CButton m_RxMacErrCheckBox;
	CButton m_CheckBox_LDPC;
	CButton m_CheckBox_STBC;
	CButton m_CheckBox_EnableTxPowerLimit;
	CButton m_CheckBox_PwrIndexFromEfuse;

	CEdit	m_TxCount;
	CComboBox	m_TestList;
	CStatic	m_RfChip;
	CStatic	m_Efuse_Utilize;
	CStatic	m_RfAutoload;
	CStatic m_RFEType;
	CColorStatic	m_Result;
	CEdit	m_RegOffset;
	CEdit	m_RegData;
	CStatic	m_RcvErr;
	CComboBox	m_Rate;
	CComboBox	m_Preamble;
	CComboBox	m_Pattern;
	CComboBox	m_Modulation;
	CStatic	m_EEpromType;
	CEdit	m_EEByteValue;
	CEdit	m_EEByteOffset;
	CEdit	m_DestMacAddr;
	CEdit	m_CisData;
	CComboBox	m_Channel;
	CButton	m_ReadMacAddr;
	CButton	m_WriteMac;
	CButton	m_WriteEEprom;
	CButton	m_SetDest_Mac;
	CButton	m_RfRegWrite;
	CButton	m_RfRegRead;
	CButton	m_Reset;
	CButton	m_RegWrite;
	CButton	m_RegRead;
	CButton	m_ReadEEprom;
	CButton	m_Quit;
	CButton	m_Help;
	CButton m_RfPathSwitch;
	CButton m_Button_IQK;
	CButton m_Button_LCK;
	CButton m_Button_DPK;	
	CButton m_Button_DIG;	
	CButton m_Button_U2U3;
	CStatic	m_static_fw;	
	CComboBox	m_fw_type;
	CComboBox m_EfuseTargetSel;

	CComboBox m_CustomerSelPwr;
	CComboBox m_CustomerSelPwrLmt;
	CButton m_Button_CSC;
	CButton m_TxSC_Help_Btn;
	CStatic m_TxSC_Label;
	CStatic m_RFPathSet_Label;
	CStatic m_RFPathSet_Label_Duplicate;
	CButton m_ButtonMonitor;
	CStatic m_MonitorText1;
	CStatic m_MonitorValue1;
	CComboBox m_TxPowerLimitSel;

	CButton m_TxPMAC_Switch;
	CComboBox m_TxPMAC_TypeSel;
	CStatic m_TxPMAC_Type_Label;
	CEdit	m_TxPMAC_Period;
	CStatic m_TxPMAC_Period_Label;

	CStatic m_TxPacketSetting;
	
	CButton m_SysOnRegRead;
	CButton m_SysOnRegWrite;
	CButton m_ButtonKfree;
	CStatic m_KfreeValue;
	
	CButton m_CheckBox_EnhanceAntiInterference;	

	CStatic m_MonitorRSSIText;
	CStatic m_MonitorRSSIValue1;

	CEdit m_BtPinValue;
	CButton m_BtPinGet;
	CButton m_BtPinSet;

	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMP819xVCDlg)
	protected:		
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMP819xVCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnStartTesting();
	afx_msg void OnSelchangeTestList();
	afx_msg void OnSelchangeChannel();
	afx_msg void OnSelchangeRate();
	afx_msg void OnSelchangeAntennaTx();
	afx_msg void OnSelchangeAntennaRx();
	afx_msg void OnSelchangeTxPower();
	afx_msg void OnSelchangePattern();
	afx_msg void OnSelchangePreamble();
	afx_msg void OnSelchangeBand();
	afx_msg void OnSelchangeEfuseTarget();
	afx_msg void OnSelchangeCustomerSelPwr();
	afx_msg void OnSelchangeCustomerSelPwrLmt();
	afx_msg void OnCustomerConfirm();
	afx_msg void OnWriteEeprom();
	afx_msg void OnReadEeprom();
	afx_msg void OnReadMacAddr();
	afx_msg void OnWriteMac();
	afx_msg void OnSetDestMac();
	afx_msg void OnRegRead();
	afx_msg void OnRegWrite();
	afx_msg void OnRfRegRead();
	afx_msg void OnRfRegWrite();
	afx_msg void OnReset();
	afx_msg void OnHlp();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnQuit();
	afx_msg void OnChangeFileIndex();
	afx_msg void OnSelChangeBandwidth();
	afx_msg void OnSelchangeAntennaTxAGCOffset();
	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTxPowerTrack();
	afx_msg void OnEditchangeTxPower();
	afx_msg void OnEditchangeChannel();
	afx_msg void OnUpdateEFUSE();
	afx_msg void OnThermalREAD();
	afx_msg void OnTssiRead();
	afx_msg void OnMonitorClicked();
	afx_msg void OnLed1Control();
	afx_msg void OnLed1Control2();
	afx_msg void OnRFPathSwitch();
	afx_msg void OnBt();
	afx_msg void OnSelchangeINITIALXinXout();
	afx_msg void OnSelchangeXtalXinXout();
	afx_msg void OnBtWifiEfuse();
	afx_msg void OnIqk();
	afx_msg void OnLck();
	afx_msg void OnDIG();
	afx_msg void OnBnClickedCheckEnableTxPwrLmt();
	afx_msg void OnBnClickedCheckPwrIndexFromEfuse();
	afx_msg void OnBnClickedStbc();
	afx_msg void OnBnClickedLdpc();
	afx_msg void OnBnClickedDevBB();
	afx_msg void OnBnClickedTxPmacMode();
	afx_msg void OnSelchangeTxSC_Selection();
	afx_msg void OnBnClickedTxscHelpBtn();
	afx_msg void OnSelchangeAntennaSetting();
	afx_msg void OnBnClickedRxfilterBtn();
	afx_msg void OnSysonRegRead();
	afx_msg void OnSysonRegWrite();
	
	afx_msg void OnSelchangeAntennaSettingDuplicate();
	afx_msg void OnBnClickedKfree();
	afx_msg void OnSelchangeFW_Type();

	afx_msg void OnBnClickedEnhanceAntiInterference();
	
	afx_msg void OnBnClickedBtPinGet();
	afx_msg void OnBnClickedBtPinSet();
	int GetDemod();
	afx_msg LRESULT OnUpdateCapData(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	LPADAPTER 
	StartDutByMode(
		IN MODE _mode
		);
	
	VOID	
	SetTxPowerControlRange(
		IN	UCHAR		btMin,
		IN	UCHAR		btMax
		);

	VOID
	SelectTxPowerControl( 
		IN	UINT btValue
		);
	
	VOID
	ErrMsgBox(
		IN	LPCTSTR		msg
		);

	UINT
	GetSelectedTxPowerControl(
		);
	
	void 
	StartTesting(
		IN	BOOL		bStart
		) ;	

    void
    SetRegByChannelForTest(
        IN  HardwareID  HID
    );
    
	ULONG
	XmitOnePacket(
		);

	BOOL
	GetValueFromControl(
		IN	PVOID 		pCEditlToRead,
		IN	UINT 		unValueByteCnt,
		OUT	ULONG 		*punValueRead
		);

	BOOL
	RegReadByExpression(
		IN	PVOID 		pCEditlToRead
		);

	BOOL
	RegWriteByExpression();	
	
	void
	StartTesting_PacketsTx(
		IN	BOOL		bStart
		);

	void
	StartTesting_PacketsRx(
		IN	BOOL		bStart
		);

	void
	StartTesting_PMacPacketTx(
		IN  BOOL    bStart
		);

	void
	StartTesting_ContinuousTx(
		IN	BOOL		bStart
		);

	void
	StartTesting_SingleCarrierTx(
		IN	BOOL		bStart
		);

	void
	StartTesting_SingleToneTx(
		IN	BOOL		bStart
		);

	void
	StartTesting_CarrierSuppressionTx(
		IN	BOOL		bStart
		);
	
	void
	StartTesting_WPSButtonPush(
		IN	BOOL		bStart
		);
		
	void
	StartTesting_HWRfOnOff(
		IN	BOOL		bStart
		);

	void
	UpdateRxStat(
		);
	
	void
	UpdateTxStat(
		);

	void
	UpdateAckCounter(
		);

	void IoMonitor();

	void RSSIMonitor();
	
	void
	WPSButtonPushPolling(
		);

	
	void
	HWRfStatusPolling(
		);


	void
	SetComponent(
		);
	
	void
	GetTXParameter(
		);
	
	void
	StartAutoTestTx(
	    );

	void
	AutoTestProcessTx(
		);
	
	void
	TxPowerTrackTest(
		);
	
	void
	StartAutoTestRx(
	    );
	    
	void
	AutoConfigRegister(
	    );
    
    void
    WriteToRxStatisticsFile(
        IN	PVOID 		pRxStaStr
        );
    
    void
    StopAutoTest(
        );
        
	void UpdateModulation(
		IN UINT newModulation,
		IN UINT disp
		);
	
	void UpdateRFPathSetting(
		IN UINT newAntSetting,
		IN UINT disp
		);

	void UpdateBandwidth(
		IN	UINT newBandwidth,
		IN UINT disp
		);
	
	void UpdateDataRate(
		IN UINT	newRate,
		IN UINT disp
		);
	
	void OnSetTSSIControlRegister(
		IN UINT value,
		IN UINT disp	
	);
	
	void UpdatePreamble(
		IN UINT newPreamble,
		IN UINT disp
		);
	
	void UpdateAntennaTxRx(
		IN UINT newAntennaTx,
		IN UINT	newAntennaRx,
		IN UINT disp
		);
		
	void UpdateChannel(
		IN UINT newChannel,
		IN UINT disp
		);
	
	void UpdateMimoTxPowers(
		);
		
	void UpdateTxPower(
		IN UINT newTxPower,
		IN UINT disp
		);

	void UpdateCrystalCap(
		IN UINT newCrystalCap,
		IN UINT disp
		);
	
	
	UINT IndexValueConvert(
		IN UINT ComboType,
		IN UINT TransType,
		IN UINT Invalue
		);
	
	void ChangeAntenna(
		);
	
	void SetTxPowerComboBox(
		IN UINT AntennaTx,
		IN BOOL Enable
		);

	UINT GetCurrChannelTxPower(
		);
	
	void ItemEnDisable(
		IN BOOL enable,
		IN UINT	Force
		);
	
	void OnLedControl(
		IN UINT	LED_Num,
		IN UINT	LED_Mode
		);
	UINT GetEfuseSize(
		UINT hardwareID
		);
	void UpdateCustomPwrTableReg(
		CHAR*	PwrByRateFilename,
		CHAR*	PwrLmtFilename
		);
	void OnInitSetTitle();
	BOOL OnInitGlobals();
	void OnInitHWType();
	void OnInitItemTest();
	void OnInitCustomerPwr();
	void OnInitCustomerPwrLmt();
	UINT OnInitCheckCustomPowerTables();
	void OnInitItemBandWidth();
	void OnInitWirelessMode();
	void OnInitItemChannel();
	void OnInitItemRate();
	void OnInitItemRfRegPath();
	void OnInitItemAntTxRx();
	void OnInitItemRFPathSetting();
	void OnInitItemPreambleGuardInterval();
	void OnInitItemTxPktSetting();
	void OnInitTestResult();
	void OnInitItemAutoTest();
	void OnInitRfPathSwitch();
	void OnInitTxPwrTrack();
	void OnInitPwrIndexBase();
	void OnInitXtalCal();
	void OnInitPmacMode();
	void OnBnClickedButtonDPK();
	void OnBnClickedButtonCFOTrack();
	void OnBnClickedButtonU2U3Switch();
	void OnBnClickedButtonAckCounterEn();
	BOOL OpenBtUI();
	CString GetHwTypeName();
    
	void ShowTxPMAC(
		bool bEnable
		);

	BOOL QueryPmacMode();
	void ReInitFWOption();

public:

	void CMP819xVCDlg::initVSA();
	void CMP819xVCDlg::demo_multi_stopVSA();
	bool CMP819xVCDlg::demo_multi_Vsa();
	void CMP819xVCDlg::setAutoRange();
	void CMP819xVCDlg::GetVsaParameter(VsaParameter *vsaParameter);
	bool CMP819xVCDlg::demo_vsg();
	int CMP819xVCDlg::PrintToMemo(char *fmt, ...);

	int XXInit();
	void ModlessDlgReturn(void);
	int NeedDisabled11ACMode(void);
	void initBW_Disable11AC(void);
private:
	BOOLEAN bDigON;
	BOOLEAN bAckCounterEnable;
	BOOLEAN bRxFilterEnable;
	FORCED_USB_MODE usbMode;
	BOOL GetHardwareID();
	TCHAR CS_PWR_FILE[260];// for custom config file
	TCHAR CS_PWR_LMT_FILE[260];// for custom config file
    void SetupUI();
	void GetEnumID();
	void SwitchRateSet(WIRELESS_MODE WirelessMode, MPT_BANDWIDTHE Bandwidth); 
    void ResetUIByRFSetting(ON_RF_CHANGE ChangedItem);
	void ReloadTxSC_List();
	void checkPowerTableStatus();
	void OnTooltipInit(HWND hwnd);
	void ShowValidTxPath();
	void ShowValidRxPath();
	afx_msg void OnFlash();
	afx_msg void OnEnableAutoinstall();
	afx_msg BOOL AutoInstallSwitch();
	afx_msg BOOL FlashMediaType();
	afx_msg BOOL FlashAccessCap();
	CButton m_AutoInstall;
	CButton m_Flash;

	
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnStartvsa();
	CEdit m_edit_ip;
	afx_msg void OnBnClickedBtnCon();
	afx_msg void OnDestroy();
	CRichEditCtrl m_redit1;
	afx_msg void OnBnClickedBtnVsg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MP819xVCDLG_H__ADE5B272_8AF7_4936_805A_352B13850918__INCLUDED_)

#endif
