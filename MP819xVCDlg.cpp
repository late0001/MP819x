#include "stdafx.h"
#include <Commctrl.h>   // Tooltip
#include <Setupapi.h>   // Registry
#include <fcntl.h>      //*** For opening/creating files ***//
#include <sys/stat.h>   //*** Get file information ***//
#include <io.h>
#include <math.h>


#include "MP819xVC.h"
#include "MP819xVCDlg.h"
#include "DevDlg.h"
#include "IniIO.h"
#include "InfoDlg.h"

#include "mp8723xBT_DllHeader.h"
#include "FlashDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MP_QUIT() do { PostMessage(WM_QUIT); return FALSE; } while(0)

// For lengthy font setting parameters
#define _PARAS_ 0,0,0,FW_BOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS

#pragma region [BT_DLL]
//define dll function variable
hRTKBT_OpenAdapter		_RTKBT_OpenAdapter;
hRTKBT_CloseAdapter		_RTKBT_CloseAdapter;
hRTKBT_GetLastError   	_RTKBT_GetLastError;
hRTKBT_ClearCounter		_RTKBT_ClearCounter;
hRTKBT_GetStatistics	_RTKBT_GetStatistics;
hRTKBT_RadioOff			_RTKBT_RadioOff;
hRTKBT_SetTRxParameters	_RTKBT_SetTRxParameters;
hRTKBT_ShowUI			_RTKBT_ShowUI;
hRTKBT_WriteReg			_RTKBT_WriteReg;
hRTKBT_ReadReg			_RTKBT_ReadReg;
hRTKBT_HCIReset			_RTKBT_HCIReset;
hRTKBT_BTTestMode		_RTKBT_BTTestMode;
hRTKBT_StartPacketTx	_RTKBT_StartPacketTx;
hRTKBT_StartContinueTx	_RTKBT_StartContinueTx;
hRTKBT_StartRx			_RTKBT_StartRx;
hRTKBT_StopTest			_RTKBT_StopTest;
hRTKBT_QueryTRxStatus	_RTKBT_QueryTRxStatus;
hRTKBT_ReadThermalMeter	_RTKBT_ReadThermalMeter;
hRTKBT_TxPowerAdjust	_RTKBT_TxPowerAdjust;
hRTKBT_SendHCICmd		_RTKBT_SendHCICmd;

HMODULE hRTKBT;

int LoadRTKBTDLL()
{

	  //specify your file route for this dll
	  hRTKBT = LoadLibrary("mp8723xBT.dll");

	  if (hRTKBT == NULL) 
	   return 0;

	_RTKBT_OpenAdapter		=		(hRTKBT_OpenAdapter)		GetProcAddress(hRTKBT,"@RTKBT_OpenAdapter$qucucui");
	_RTKBT_CloseAdapter 		=		(hRTKBT_CloseAdapter) 		GetProcAddress(hRTKBT,"@RTKBT_CloseAdapter$qv");
	
	_RTKBT_GetLastError 		=		(hRTKBT_GetLastError) 		GetProcAddress(hRTKBT,"@RTKBT_GetLastError$qv");
	_RTKBT_ClearCounter 		=		(hRTKBT_ClearCounter)		GetProcAddress(hRTKBT,"@RTKBT_ClearCounter$qv");
	_RTKBT_GetStatistics		=		(hRTKBT_GetStatistics)		GetProcAddress(hRTKBT,"@RTKBT_GetStatistics$qp20_RTKBT_TRXSTATISTICS");
	_RTKBT_RadioOff 			=		(hRTKBT_RadioOff)			GetProcAddress(hRTKBT,"@RTKBT_RadioOff$qv");
	_RTKBT_SetTRxParameters    	=		(hRTKBT_SetTRxParameters)	GetProcAddress(hRTKBT,"@RTKBT_SetTRxParameters$q15_RTKBT_TRXPARAM");
	_RTKBT_ShowUI			=		(hRTKBT_ShowUI)			GetProcAddress(hRTKBT,"@RTKBT_ShowUI$qo");
	_RTKBT_WriteReg 			=		(hRTKBT_WriteReg) 			GetProcAddress(hRTKBT,"@RTKBT_WriteReg$quiuiuiui");
	_RTKBT_ReadReg			=		(hRTKBT_ReadReg)			GetProcAddress(hRTKBT,"@RTKBT_ReadReg$quiuiui");
	_RTKBT_HCIReset 			=		(hRTKBT_HCIReset) 			GetProcAddress(hRTKBT,"@RTKBT_HCIReset$qv");
	_RTKBT_BTTestMode		=		(hRTKBT_BTTestMode)		GetProcAddress(hRTKBT,"@RTKBT_BTTestMode$quc");
	_RTKBT_StartPacketTx		=		(hRTKBT_StartPacketTx)		GetProcAddress(hRTKBT,"@RTKBT_StartPacketTx$qv");
	_RTKBT_StartContinueTx		= 		(hRTKBT_StartContinueTx)	GetProcAddress(hRTKBT,"@RTKBT_StartContinueTx$qv");
	_RTKBT_StartRx			=		(hRTKBT_StartRx)			GetProcAddress(hRTKBT,"@RTKBT_StartRx$qv");
	_RTKBT_StopTest 			=		(hRTKBT_StopTest) 			GetProcAddress(hRTKBT,"@RTKBT_StopTest$qv");
	_RTKBT_QueryTRxStatus		=		(hRTKBT_QueryTRxStatus)	GetProcAddress(hRTKBT,"@RTKBT_QueryTRxStatus$qv");
	_RTKBT_ReadThermalMeter   	= 		(hRTKBT_ReadThermalMeter)	GetProcAddress(hRTKBT,"@RTKBT_ReadThermalMeter$qv");
	_RTKBT_TxPowerAdjust		= 		(hRTKBT_TxPowerAdjust)		GetProcAddress(hRTKBT,"@RTKBT_TxPowerAdjust$quc");
	_RTKBT_SendHCICmd		=		(hRTKBT_SendHCICmd)		GetProcAddress(hRTKBT,"@RTKBT_SendHCICmd$qusucpucuc");

  atexit(UnLoadRTKBTDLL);

  return 1;
}

void UnLoadRTKBTDLL()
{
   FreeLibrary(hRTKBT);
}
#pragma endregion BT_DLL

#pragma region [CAboutDlg]
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    CStatic m_AboutVersion;
    CEdit   m_AboutEdit;
	//}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    virtual BOOL OnInitDialog();
	//}}AFX_MSG
    HICON m_hIcon;
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDI_REALTEK_ICO);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    DDX_Control(pDX, IDC_ABOUT_VERSION, m_AboutVersion);
    DDX_Control(pDX, AboutEdit, m_AboutEdit);
    //}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetIcon(m_hIcon, FALSE);

    CString AboutRealtek;
    AboutRealtek.LoadString(IDS_ABOUT_REALTEK);
    m_AboutEdit.SetWindowText(AboutRealtek);

    CString AboutVersion;
    AboutVersion = regAdapter.Version;
    AboutVersion = "\nMP Version " + AboutVersion;
    AboutVersion.Insert(0, regAdapter.Name);

    m_AboutVersion.SetWindowText(AboutVersion);


    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
#pragma endregion CAboutDlg


// CMP819xVCDlg dialog

CMP819xVCDlg::CMP819xVCDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CMP819xVCDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMP819xVCDlg)
	//}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	AfxInitRichEdit2();
    m_hIcon = AfxGetApp()->LoadIcon(IDI_REALTEK_ICO);
	bDigON = FALSE;
	bAckCounterEnable = FALSE;
	bRxFilterEnable = FALSE;
	usbMode = USB2;	
}

#pragma region [CMP819xVCDlg]-Essential Components
void CMP819xVCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMP819xVCDlg)
	DDX_Control(pDX, IDC_BT_RATE_TEXT, m_BT_Rate_text);
	DDX_Control(pDX, IDC_BT_RATE, m_BT_Rate);
	DDX_Control(pDX, IDC_BT_PORT_NUMBER, m_BT_Port_No);
	DDX_Control(pDX, IDC_BT_PORT_NO_TEXT, m_BT_Port_No_text);
	DDX_Control(pDX, IDC_BT_WIFI, m_BT_WiFi);
	DDX_Control(pDX, IDC_Xtal_XinXout, m_Xtal_XinXout);
	DDX_Control(pDX, IDC_BT, m_BT);
	DDX_Control(pDX, IDC_TX_POWER2, m_TxPowerB);
	DDX_Control(pDX, IDC_TX_POWER3, m_TxPowerC);
	DDX_Control(pDX, IDC_TX_POWER4, m_TxPowerD);

	DDX_Control(pDX, IDC_TX_POWER_B_LABEL, m_TxPowerB_Label);
	DDX_Control(pDX, IDC_TX_POWER_C_LABEL, m_TxPowerC_Label);
	DDX_Control(pDX, IDC_TX_POWER_D_LABEL, m_TxPowerD_Label);

	DDX_Control(pDX, IDC_STATIC_EfuseUsed, m_EfuseUsed);
	DDX_Control(pDX, IDC_UPDATE_EFUSE, m_UpdateEfuse);
	DDX_Control(pDX, IDC_LED1_CONTROL, m_Led1);
	DDX_Control(pDX, IDC_RF_Thermal_READ, m_TxPowerTrack);
	DDX_Control(pDX, IDC_ANTENNA_TX, m_AntennaTx);
	DDX_Control(pDX, IDC_ANTENNA_RX, m_AntennaRx);
	DDX_Control(pDX, IDC_RF_PATH_SETTING, m_RFPathSetting);
	DDX_Control(pDX, IDC_RF_PATH_SETTING_DUPLICATE, m_RFPathSetting_Duplicate);
	DDX_Control(pDX, IDC_RFREG_RFPATH, m_RfRegRfPath);
	DDX_Control(pDX, IDC_File_Index, m_FileIndex);
	DDX_Control(pDX, IDC_PacketLength, m_PacketLength);
	DDX_Control(pDX, IDC_CHANNEL_WIDTH_STATIC, m_Channel_Width_Static);
	DDX_Control(pDX, IDC_CHANNEL_WIDTH, m_Channel_Width);
	DDX_Control(pDX, IDC_MODULATION_STATIC, m_Modulation_Static);
	DDX_Control(pDX, IDC_TxSC_Selection, m_TxSC_Selection);
	DDX_Control(pDX, IDC_TxSC_Help_Btn, m_TxSC_Help_Btn);
	DDX_Control(pDX, IDC_TxSubCarrier_STATIC, m_TxSC_Label);
	DDX_Control(pDX, IDC_RF_PATH_SETTING_STATIC, m_RFPathSet_Label);
	DDX_Control(pDX, IDC_RF_PATH_SETTING_STATIC_DUPLICATE, m_RFPathSet_Label_Duplicate);
	DDX_Control(pDX, IDC_PREAMBLE_STATIC, m_Preamble_Static);
	DDX_Control(pDX, IDC_START, m_Start);
	DDX_Control(pDX, IDC_SELF_MAC_ADDR, m_SelfMacAddr);
	DDX_Control(pDX, IDC_TX_POWER, m_TxPower);
	DDX_Control(pDX, IDC_ACK_PKT_COUNTER, m_RxAckPktCounter);
	DDX_Control(pDX, IDC_BUTTON_ACK_COUNTER_EN, m_RxAckPktCounter_Enable_Btn);
	DDX_Control(pDX, IDC_STATIC_ACK_PKT_COUNTER, m_RxAckPktCounter_Label);
	DDX_Control(pDX, IDC_RXFILTER_BTN, m_RxFilter_Enable_Btn);
	DDX_Control(pDX, IDC_XMIT_OK, m_XmitOK);
	DDX_Control(pDX, IDC_RCV_OK, m_RcvOK);
	DDX_Control(pDX, IDC_TX_COUNT, m_TxCount);
	DDX_Control(pDX, IDC_TEST_LIST, m_TestList);
	DDX_Control(pDX, IDC_RFCHIP, m_RfChip);
	DDX_Control(pDX, IDC_EFUSE_UTILIZE, m_Efuse_Utilize);
	DDX_Control(pDX, IDC_RF_TYPE_AUTOLOAD, m_RfAutoload);
	DDX_Control(pDX, IDC_RFE_TYPE_TEXT, m_RFEType);
	DDX_Control(pDX, IDC_RESULT, m_Result);
	DDX_Control(pDX, IDC_REG_OFFSET, m_RegOffset);
	DDX_Control(pDX, IDC_REG_DATA, m_RegData);
	DDX_Control(pDX, IDC_RCV_ERR, m_RcvErr);
	DDX_Control(pDX, IDC_PHY_RX_COUNTER_OK, m_RcvPhyOK);
	DDX_Control(pDX, IDC_PHY_RX_COUNTER_ERR, m_RcvPhyErr);
	DDX_Control(pDX, IDC_CHECK_PHY_OK, m_RxPhyOkCheckBox);
	DDX_Control(pDX, IDC_CHECK_PHY_ERR, m_RxPhyErrCheckBox);
	DDX_Control(pDX, IDC_MAC_RX_COUNTER_OK, m_RcvMacOK);
	DDX_Control(pDX, IDC_MAC_RX_COUNTER_ERR, m_RcvMacErr);
	DDX_Control(pDX, IDC_CHECK_MAC_OK, m_RxMacOkCheckBox);
	DDX_Control(pDX, IDC_CHECK_MAC_ERR, m_RxMacErrCheckBox);
	DDX_Control(pDX, IDC_RATE, m_Rate);
	DDX_Control(pDX, IDC_PREAMBLE, m_Preamble);
	DDX_Control(pDX, IDC_PATTERN, m_Pattern);
	DDX_Control(pDX, IDC_MODULATION, m_Modulation);
	DDX_Control(pDX, IDC_EEPROM_TYPE, m_EEpromType);
	DDX_Control(pDX, IDC_EE_BYTE_VALUE, m_EEByteValue);
	DDX_Control(pDX, IDC_EE_BYTE_OFFSET, m_EEByteOffset);
	DDX_Control(pDX, IDC_DEST_MAC_ADDR, m_DestMacAddr);
	DDX_Control(pDX, IDC_CIS_DATA, m_CisData);
	DDX_Control(pDX, IDC_CHANNEL, m_Channel);
	DDX_Control(pDX, IDC_READ_MAC_ADDR, m_ReadMacAddr);
	DDX_Control(pDX, IDC_WRITE_MAC, m_WriteMac);
	DDX_Control(pDX, IDC_WRITE_EEPROM, m_WriteEEprom);
	DDX_Control(pDX, IDC_SET_DEST_MAC, m_SetDest_Mac);
	DDX_Control(pDX, IDC_RF_REG_WRITE, m_RfRegWrite);
	DDX_Control(pDX, IDC_RF_REG_READ, m_RfRegRead);
	DDX_Control(pDX, IDC_RESET, m_Reset);
	DDX_Control(pDX, IDC_REG_WRITE, m_RegWrite);
	DDX_Control(pDX, IDC_REG_READ, m_RegRead);
	DDX_Control(pDX, IDC_READ_EEPROM, m_ReadEEprom);
	DDX_Control(pDX, IDC_QUIT, m_Quit);
	DDX_Control(pDX, IDC_HLP, m_Help);
	DDX_Control(pDX, IDC_RFPathSwitch, m_RfPathSwitch);	
	DDX_Control(pDX, IDC_STATIC_FW, m_static_fw);
	DDX_Control(pDX, IDC_FW_TYPE, m_fw_type);
	DDX_Control(pDX, IDC_BUTTON_DEVBB, m_Button_DevBB);
	DDX_Control(pDX, IDC_BUTTON_IQK, m_Button_IQK);	
	DDX_Control(pDX, IDC_BUTTON_LCK, m_Button_LCK);	
	DDX_Control(pDX, IDC_BUTTON_DPK, m_Button_DPK);
	DDX_Control(pDX, IDC_BUTTON_U2U3_SWITCH, m_Button_U2U3);		
	DDX_Control(pDX, IDC_BUTTON_DIG, m_Button_DIG);
	DDX_Control(pDX, IDC_EFUSE_TARGET_SEL, m_EfuseTargetSel);		
	DDX_Control(pDX, IDC_LDPC, m_CheckBox_LDPC);
	DDX_Control(pDX, IDC_STBC, m_CheckBox_STBC);
	DDX_Control(pDX, IDC_CHECK_ENABLE_TX_PWR_LMT, m_CheckBox_EnableTxPowerLimit);

	DDX_Control(pDX, IDC_CUSTOMER_SEL_PWR, m_CustomerSelPwr);
	DDX_Control(pDX, IDC_CUSTOMER_SEL_PWR_LMT, m_CustomerSelPwrLmt);
	DDX_Control(pDX, IDC_CUSTOMER_SEL_COM, m_Button_CSC);

	DDX_Control(pDX, IDC_MONITOR, m_ButtonMonitor);		
	DDX_Control(pDX, IDC_STATIC_REG_MONITOR_1, m_MonitorText1);		
	DDX_Control(pDX, IDC_REG_MONITOR_1, m_MonitorValue1);		

	DDX_Control(pDX, IDC_TX_PMAC_MODE_BTN, m_TxPMAC_Switch);
	DDX_Control(pDX, IDC_TX_PMAC_TYPE, m_TxPMAC_TypeSel);
	DDX_Control(pDX, IDC_TX_PMAC_TYPE_LABEL, m_TxPMAC_Type_Label);
	DDX_Control(pDX, IDC_TX_PMAC_PERIOD, m_TxPMAC_Period);
	DDX_Control(pDX, IDC_TX_PMAC_PERIOD_LABEL, m_TxPMAC_Period_Label);

	DDX_Control(pDX, IDC_TX_PKT_SETTING, m_TxPacketSetting);
	DDX_Control(pDX, IDC_PWR_INDEX_FROM_EFUSE, m_CheckBox_PwrIndexFromEfuse);

	DDX_Control(pDX, IDC_SYSON_REG_READ, m_SysOnRegRead);
	DDX_Control(pDX, IDC_SYSON_REG_WRITE, m_SysOnRegWrite);	
	DDX_Control(pDX, IDC_ENABLE_AUTOINSTALL, m_AutoInstall);
	DDX_Control(pDX, IDC_FLASH, m_Flash);
	DDX_Control(pDX, IDC_KFREE, m_ButtonKfree);
	DDX_Control(pDX, IDC_KFREE_VALUE, m_KfreeValue);
	DDX_Control(pDX, IDC_Enhance_AntiInterference, m_CheckBox_EnhanceAntiInterference);

	DDX_Control(pDX, IDC_BT_PIN_GET, m_BtPinGet);
	DDX_Control(pDX, IDC_BT_PIN_SET, m_BtPinSet);
	DDX_Control(pDX, IDC_BT_PIN_VALUE, m_BtPinValue);

	DDX_Control(pDX, IDC_STATIC_RSSI_MONITOR, m_MonitorRSSIText);		
	DDX_Control(pDX, IDC_STATIC_RSSI_VALUE, m_MonitorRSSIValue1);	

	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDT_IP, m_edit_ip);
	DDX_Control(pDX, IDC_RDT_MEMO1, m_redit1);
}

BEGIN_MESSAGE_MAP(CMP819xVCDlg, CDialog)
    //{{AFX_MSG_MAP(CMP819xVCDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
	ON_WM_TIMER()
    
    ON_CBN_SELCHANGE(IDC_TEST_LIST, OnSelchangeTestList)
    ON_CBN_SELCHANGE(IDC_CHANNEL, OnSelchangeChannel)
    ON_CBN_SELCHANGE(IDC_RATE, OnSelchangeRate)
    ON_CBN_SELCHANGE(IDC_ANTENNA_TX, OnSelchangeAntennaTx)
    ON_CBN_SELCHANGE(IDC_ANTENNA_RX, OnSelchangeAntennaRx)
    ON_CBN_SELCHANGE(IDC_TX_POWER, OnSelchangeTxPower)
	ON_CBN_SELCHANGE(IDC_TX_POWER2, OnSelchangeTxPower)
    ON_CBN_SELCHANGE(IDC_TX_POWER3, OnSelchangeTxPower)    
	ON_CBN_SELCHANGE(IDC_TX_POWER4, OnSelchangeTxPower)   
    ON_CBN_SELCHANGE(IDC_PATTERN, OnSelchangePattern)
    ON_CBN_SELCHANGE(IDC_PREAMBLE, OnSelchangePreamble)
    ON_CBN_SELCHANGE(IDC_MODULATION, OnSelchangeBand)
    ON_CBN_SELCHANGE(IDC_EFUSE_TARGET_SEL, OnSelchangeEfuseTarget)
	ON_CBN_SELCHANGE(IDC_CUSTOMER_SEL_PWR, OnSelchangeCustomerSelPwr)
	ON_CBN_SELCHANGE(IDC_CUSTOMER_SEL_PWR_LMT, OnSelchangeCustomerSelPwrLmt)
	ON_CBN_SELCHANGE(IDC_File_Index, OnChangeFileIndex)
	ON_CBN_SELCHANGE(IDC_CHANNEL_WIDTH, OnSelChangeBandwidth)
	ON_CBN_SELCHANGE(IDC_Xtal_XinXout, OnSelchangeXtalXinXout)
	ON_CBN_SELCHANGE(IDC_TxSC_Selection, OnSelchangeTxSC_Selection)
	ON_CBN_SELCHANGE(IDC_RF_PATH_SETTING, OnSelchangeAntennaSetting)
	ON_CBN_SELCHANGE(IDC_RF_PATH_SETTING_DUPLICATE, OnSelchangeAntennaSettingDuplicate)
	ON_CBN_SELCHANGE(IDC_FW_TYPE, OnSelchangeFW_Type)

	ON_BN_CLICKED(IDC_START, OnStartTesting)
	ON_BN_CLICKED(IDC_CUSTOMER_SEL_COM, OnCustomerConfirm)
    ON_BN_CLICKED(IDC_WRITE_EEPROM, OnWriteEeprom)
    ON_BN_CLICKED(IDC_READ_EEPROM, OnReadEeprom)
    ON_BN_CLICKED(IDC_READ_MAC_ADDR, OnReadMacAddr)
    ON_BN_CLICKED(IDC_WRITE_MAC, OnWriteMac)
    ON_BN_CLICKED(IDC_SET_DEST_MAC, OnSetDestMac)
    ON_BN_CLICKED(IDC_REG_READ, OnRegRead)
    ON_BN_CLICKED(IDC_REG_WRITE, OnRegWrite)
    ON_BN_CLICKED(IDC_RF_REG_READ, OnRfRegRead)
    ON_BN_CLICKED(IDC_RF_REG_WRITE, OnRfRegWrite)
    ON_BN_CLICKED(IDC_RESET, OnReset)
    ON_BN_CLICKED(IDC_HLP, OnHlp)
    ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_BN_CLICKED(IDC_RF_Thermal_READ, OnTxPowerTrack)
	ON_BN_CLICKED(IDC_UPDATE_EFUSE, OnUpdateEFUSE)
	ON_BN_CLICKED(IDC_Thermal_READ, OnThermalREAD)
	ON_BN_CLICKED(IDC_MONITOR, OnMonitorClicked)
	ON_BN_CLICKED(IDC_LED1_CONTROL, OnLed1Control)
	
	ON_BN_CLICKED(IDC_RFPathSwitch, OnRFPathSwitch)
	ON_BN_CLICKED(IDC_BT, OnBt)
	ON_BN_CLICKED(IDC_BT_WIFI, OnBtWifiEfuse)
	ON_BN_CLICKED(IDC_BUTTON_IQK, OnIqk)
	ON_BN_CLICKED(IDC_BUTTON_LCK, OnLck)
	ON_BN_CLICKED(IDC_BUTTON_DIG, OnDIG)	
	ON_BN_CLICKED(IDC_BUTTON_DPK, OnBnClickedButtonDPK)		
    ON_BN_CLICKED(IDC_BUTTON_U2U3_SWITCH, OnBnClickedButtonU2U3Switch)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_TX_PWR_LMT, OnBnClickedCheckEnableTxPwrLmt)
	ON_BN_CLICKED(IDC_PWR_INDEX_FROM_EFUSE, OnBnClickedCheckPwrIndexFromEfuse)
	ON_BN_CLICKED(IDC_STBC, OnBnClickedStbc)
	ON_BN_CLICKED(IDC_LDPC, OnBnClickedLdpc)
	ON_BN_CLICKED(IDC_BUTTON_DEVBB, OnBnClickedDevBB)
	ON_BN_CLICKED(IDC_TX_PMAC_MODE_BTN, OnBnClickedTxPmacMode)
	ON_BN_CLICKED(IDC_TxSC_Help_Btn, OnBnClickedTxscHelpBtn)
	ON_BN_CLICKED(IDC_BUTTON_ACK_COUNTER_EN, OnBnClickedButtonAckCounterEn)
	ON_BN_CLICKED(IDC_RXFILTER_BTN, OnBnClickedRxfilterBtn)
	ON_BN_CLICKED(IDC_SYSON_REG_READ, OnSysonRegRead)
	ON_BN_CLICKED(IDC_SYSON_REG_WRITE, OnSysonRegWrite)
	ON_BN_CLICKED(IDC_FLASH, OnFlash)
	ON_BN_CLICKED(IDC_ENABLE_AUTOINSTALL, OnEnableAutoinstall)
	ON_BN_CLICKED(IDC_KFREE, OnBnClickedKfree)	

	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)

	ON_BN_CLICKED(IDC_Enhance_AntiInterference, OnBnClickedEnhanceAntiInterference)

	ON_BN_CLICKED(IDC_BT_PIN_GET, OnBnClickedBtPinGet)
	ON_BN_CLICKED(IDC_BT_PIN_SET, OnBnClickedBtPinSet)


	ON_BN_CLICKED(IDC_BUTTON1, CMP819xVCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_STARTVSA, CMP819xVCDlg::OnBnClickedBtnStartvsa)
	ON_BN_CLICKED(IDC_BTN_CON, &CMP819xVCDlg::OnBnClickedBtnCon)
	ON_MESSAGE(WM_UPDATECAPDATA, &CMP819xVCDlg::OnUpdateCapData)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_VSG, &CMP819xVCDlg::OnBnClickedBtnVsg)
END_MESSAGE_MAP()

#pragma endregion Essential Components


// CMP819xVCDlg message handlers

#pragma region [Misc.] Global and Local Functions


ULONG 
SendSpecificPacket(
	BOOL        bRandom, 
	ULONG       TmpULong
	)
{
	BOOL        bResult;
	
	UINT        count, Xmited=0;
	UCHAR		pBuf[2048] = {0};
	UINT		transmitLen = Adapter.PacketLength + 32;

	PrintMsg(TEXT("===> SendSpecificPacket()\n"));

	uistatistics.ctr_PacketAllocatePacket_Ok++;
	
	Adapter.lpMem2[0]=0x08;
	Adapter.lpMem2[1]=0x00;
	Adapter.lpMem2[2]=0x00;
	Adapter.lpMem2[3]=0x00;

	memcpy(Adapter.lpMem2+4, UnicastDID, 6);
	memcpy(Adapter.lpMem2+10, Adapter.CurrentMAC, 6);

	for (UINT i = 16; i < 22; i++)
		Adapter.lpMem2[i] = i-16;

	if(bRandom)
	{
		srand( GetTickCount() );
		for(count = 31; count < transmitLen; count ++)
		{
			UINT tmpPacketPattern = rand();
			Adapter.lpMem2[count] = (UCHAR)tmpPacketPattern % 0xFF;
		}
	}
	else
	{
		for (count = 31; count < transmitLen; count++)
			Adapter.lpMem2[count] = Adapter.PacketPattern;
	}

	Adapter.lpMem2[22]=0x01;
	Adapter.lpMem2[23]=0x23;
	Adapter.lpMem2[24]=0x39;
	Adapter.lpMem2[25]=0x81;
	Adapter.lpMem2[26]=0xEC;
	Adapter.lpMem2[27]=0x10;
	Adapter.lpMem2[28]=0x01;

	memcpy(Adapter.lpMem2 + 29, &(Adapter.PacketLength), 2);

	Adapter.lpMem2[31] = Xmited++;

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	memcpy(pBuf, Adapter.lpMem2, transmitLen);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	bResult = DLL_SendSpecificPacket(
				(LPADAPTER)Adapter.hFile,
				pBuf,			//Buffer
				(UINT16)transmitLen,
				NULL			//Reserved
				);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	if(bResult)
	{
		TmpULong++;
		uistatistics.ctr_PacketSendPacket_Ok++;
		PrintMsg(TEXT("SendSpecificPacket() ok\n"));
	}
	else
	{
		uistatistics.ctr_PacketSendPacket_Fail++;
		PrintMsg(TEXT("SendSpecificPacket() fail\n"));
	}
	PrintMsg(TEXT("<=== SendSpecificPacket()\n"));
	return TmpULong;
}


ULONG __stdcall ThreadXmit(PVOID pArg)
{
    BOOL        bResult;
    UINT        PacketCount, TxOKFinal=0;
    BOOL        Endless, bRandom;
    TCHAR       strBuf[100];
    ULONG       TmpULong = 0, TxOK = 0;
	ULONG		TmpCount = 0;
    HWND        hDlg = (HWND)pArg;
    HWND		hStart = GetDlgItem(hDlg, IDC_START);

    TxThreadInProgress = TRUE;

	PrintMsg(TEXT("===> ThreadXmit()\n"));

	bResult = QueryTxPacketSent(
                    (LPADAPTER) Adapter.hFile,
                    (int*)&TxOK
                    );
    _stprintf(strBuf, TEXT("%d"), TxOK);
    SetDlgItemText(hDlg, IDC_XMIT_OK, strBuf);
        
	PacketCount = Adapter.PacketCount;
    if(PacketCount == 0)
        Endless = TRUE;
    else
        Endless = FALSE;

    if (Adapter.PacketPattern == 0x12)
        bRandom = TRUE;
    else
        bRandom = FALSE;

    do{
		TmpULong = SendSpecificPacket(bRandom, TmpULong);


		if(TxOK != TmpULong)
		{
			TxOK = TmpULong;
			Win32Print("SendOnePacket() ok number = %d\n", TxOK);
				
			_stprintf(strBuf, TEXT("%d"), TxOK);
			SetDlgItemText(hDlg, IDC_XMIT_OK, strBuf);
		}
        if((!Endless) && (PacketCount > 0))
        {
            if(TmpULong >= PacketCount)
                PacketCount = 0;
        }
		
        if (TestStop)
        {
            PacketCount = 0;
            Endless = FALSE;
        }
    } while((Endless) || (PacketCount != 0));

    if (!TestStop && !AutoTestFlag)
    {
        SendDlgItemMessage(hDlg, IDC_START, BM_CLICK, 0, 0);
    }

 	//wait for 8190 adapter's finishing sending packets in it's queue.
	int same_cnt=0, timeout=0, max_timeout_cnt=100;
	//prevent from "start" Tx Packets or other test item again before
    //the end of the thread ThreadXmit().
    EnableWindow(hStart, FALSE);
    if( Adapter.CurrentRate == RATE_CCK_1M ||
       	Adapter.CurrentRate == RATE_CCK_2M ||
       	Adapter.CurrentRate == RATE_CCK_55M )
	{
		//because of the slow rate, we must wait longer.
        max_timeout_cnt = 600;
	}
	while(TestStop == FALSE)
	{
		Sleep(5);
		bResult = QueryTxPacketSent(
				(LPADAPTER) Adapter.hFile,
				(int*)&TxOK
				);
		if(TxOKFinal == TxOK)
			same_cnt++;
		TxOKFinal = TxOK;
		_stprintf(strBuf, TEXT("%d"), TxOK);
		SetDlgItemText(hDlg, IDC_XMIT_OK, strBuf);
		if(TxOK >= TmpULong)
           		break;
		if(timeout++ >= max_timeout_cnt)
			break;
    }

    //Thread is going to be end, let "start" button can be pushed.
    EnableWindow(hStart, TRUE);
    
	TxThreadInProgress = FALSE;
	PrintMsg(TEXT("<=== ThreadXmit()\n"));
    return 0;
}


void _initChannelFromINI(char* CustomizedChannel, int size, unsigned int *gArray, unsigned int *gSize)
{
	const char *channel = reinterpret_cast < const char* >(CustomizedChannel);
	int i = 0;
	while (i < size) {
		int j = i;
		char chnl[3] = {0};
		while (channel[j] != ',' && j < size)
			j++;
		strncpy(chnl, channel+i, j-i);
		gArray[(*gSize)++] = atoi(chnl);
		i = j + 1;
	}
}

void initBW_40()
{
	int	 checkBW = 1;

	if (Adapter.HardwareId == HID_8188FS || Adapter.HardwareId == HID_8188FU)
		QueryBandwidthCap((LPADAPTER) Adapter.hFile, &checkBW);
	if(checkBW == 0)
	{
		strcpy(customBWsetting[0][1].bw_Str, "");
		customBWsetting[0][1].bw_en = 0;
	}
}
void CMP819xVCDlg::initBW_Disable11AC()
{
	if(NeedDisabled11ACMode())
		customBWsetting[1][2].bw_en = 0;
}
void initBW()
{
	UINT uOrder = 0;
	//
	// 2G setting
	strcpy(customBWsetting[0][0].bw_Str, "20M");
	customBWsetting[0][0].bw_en = 1;
	if(1 == customBWsetting[0][0].bw_en)
	{
		customBWsetting[0][0].comboxOrder = uOrder;
		customBWsetting[0][0].comboxBW = MPT_BW_20MHZ;
		uOrder++;
	}

	strcpy(customBWsetting[0][1].bw_Str, "40M");
	customBWsetting[0][1].bw_en = GetPrivateProfileInt("2G_40M", "BandWidthEnable", 1, CONFIG_CHIP_FILE);
	if(1 == customBWsetting[0][1].bw_en)
	{
		customBWsetting[0][1].comboxOrder = uOrder;
		customBWsetting[0][1].comboxBW = MPT_BW_40MHZ;
		uOrder++;
	}

	strcpy(customBWsetting[0][2].bw_Str, "");
	customBWsetting[0][2].bw_en = 0;

	strcpy(customBWsetting[0][3].bw_Str, "");
	customBWsetting[0][3].bw_en = 0;

	strcpy(customBWsetting[0][4].bw_Str, "5M");
	customBWsetting[0][4].bw_en = GetPrivateProfileInt("2G_5M", "BandWidthEnable", 0, CONFIG_CHIP_FILE);
	if(1 == customBWsetting[0][4].bw_en)
	{
		customBWsetting[0][4].comboxOrder = uOrder;
		customBWsetting[0][4].comboxBW = MPT_BW_5MHZ;
		uOrder++;
	}

	strcpy(customBWsetting[0][5].bw_Str, "10M");
	customBWsetting[0][5].bw_en = GetPrivateProfileInt("2G_10M", "BandWidthEnable", 0, CONFIG_CHIP_FILE);
	if(1 == customBWsetting[0][5].bw_en)
	{
		customBWsetting[0][5].comboxOrder = uOrder;
		customBWsetting[0][5].comboxBW = MPT_BW_10MHZ;
		uOrder++;
	}


	//reset to 0
	uOrder = 0;
	//
	// 5G setting
	strcpy(customBWsetting[1][0].bw_Str, "20M");
	customBWsetting[1][0].bw_en = 1;
	if(1 == customBWsetting[1][0].bw_en)
	{
		customBWsetting[1][0].comboxOrder = uOrder;
		customBWsetting[1][0].comboxBW = MPT_BW_20MHZ;
		uOrder++;
	}


	strcpy(customBWsetting[1][1].bw_Str, "40M");
	customBWsetting[1][1].bw_en = GetPrivateProfileInt("5G_40M", "BandWidthEnable", 1, CONFIG_CHIP_FILE);
	if(1 == customBWsetting[1][1].bw_en)
	{
		customBWsetting[1][1].comboxOrder = uOrder;
		customBWsetting[1][1].comboxBW = MPT_BW_40MHZ;
		uOrder++;
	}


	strcpy(customBWsetting[1][2].bw_Str, "80M");
	customBWsetting[1][2].bw_en = GetPrivateProfileInt("5G_80M", "BandWidthEnable", 1, CONFIG_CHIP_FILE);
	if(1 == customBWsetting[1][2].bw_en)
	{
		customBWsetting[1][2].comboxOrder = uOrder;
		customBWsetting[1][2].comboxBW = MPT_BW_80MHZ;
		uOrder++;
	}

	strcpy(customBWsetting[1][3].bw_Str, "160M");
	customBWsetting[1][3].bw_en = 0;
	if(1 == customBWsetting[1][3].bw_en)
	{
		customBWsetting[1][3].comboxOrder = uOrder;
		//customBWsetting[1][3].comboxBW = MPT_BW_160MHZ;
		uOrder++;
	}
	
	strcpy(customBWsetting[1][4].bw_Str, "5M");
	customBWsetting[1][4].bw_en = GetPrivateProfileInt("5G_5M", "BandWidthEnable", 0, CONFIG_CHIP_FILE);
	if(1 == customBWsetting[1][4].bw_en)
	{
		customBWsetting[1][4].comboxOrder = uOrder;
		customBWsetting[1][4].comboxBW = MPT_BW_5MHZ;
		uOrder++;
	}

	strcpy(customBWsetting[1][5].bw_Str, "10M");
	customBWsetting[1][5].bw_en = GetPrivateProfileInt("5G_10M", "BandWidthEnable", 0, CONFIG_CHIP_FILE);
	if(1 == customBWsetting[1][5].bw_en)
	{
		customBWsetting[1][5].comboxOrder = uOrder;
		customBWsetting[1][5].comboxBW = MPT_BW_10MHZ;
		uOrder++;
	}

}
void initRate()
{
	
	bVHTEnabled = GetPrivateProfileInt("DataRate", "VHTEnable", 1, CONFIG_CHIP_FILE);
	
}

BOOL initFlash()
{

	RT_FLASH_MODE		default_mode = FLASH_UNKNOWN_MODE;
	ULONG				default_size = 0xFF;
	RT_FLASH_DEVICE_TYPE	default_device_type = FLASH_DEVICE_UNKNOWN;
	BOOL				result = TRUE;

	Adapter.flash_mode = (RT_FLASH_MODE)GetPrivateProfileInt("Flash", "FlashMode", default_mode, CONFIG_CHIP_FILE);
	Adapter.flash_size = GetPrivateProfileInt("Flash", "FlashSize", default_size, CONFIG_CHIP_FILE);
	Adapter.flash_device_type = (RT_FLASH_DEVICE_TYPE)GetPrivateProfileInt("Flash", "DeviceType", default_device_type, CONFIG_CHIP_FILE);

	if( IS_HARDWARE_SERIES(8821C) || IS_HARDWARE_8731AU )
	{
		if( 0 == Adapter.flash_size || Adapter.flash_mode == FLASH_DISABLE_MODE )
			result = FALSE;
		else
		{
			if (Adapter.flash_mode == default_mode)
				Adapter.flash_mode = FLASH_USER_MODE;
			if (Adapter.flash_size == default_size)
				Adapter.flash_size = 0x800000;
			if (Adapter.flash_device_type == default_device_type)
				Adapter.flash_device_type = FLASH_DEVICE_CDROM;
		}

	}
	else if (Adapter.HardwareId == HID_8710BU || Adapter.HardwareId == HID_8192FU)
	{
		if (Adapter.flash_mode == FLASH_DISABLE_MODE || Adapter.flash_mode == default_mode)
			result = FALSE;
		else
		{
			if( Adapter.flash_size == default_size )
			{
				if(Adapter.HardwareId == HID_8710BU)
				{
					int ret = QueryFlashSize((LPADAPTER)Adapter.hFile, &(Adapter.flash_size));
					if(false == ret)
						Adapter.flash_size = 0x200000;
				}						
				else
					Adapter.flash_size = 0x200000;
			}
			if( Adapter.flash_device_type == default_device_type )
				Adapter.flash_device_type = FLASH_DEVICE_CDROM;
		}
	}
	else
		result = FALSE;
	
	Win32Print("flash_mode = 0x%x, flash_size = 0x%x, ,flash_device_type = 0x%x!!\n", Adapter.flash_mode,  Adapter.flash_size, Adapter.flash_device_type);
	
	return result;
}

void CMP819xVCDlg::ReInitFWOption()
{
	Adapter.fw_list = 0;
	m_fw_type.ResetContent();
	m_fw_type.AddString(FW_Table[RT_FW_NORMAL].fw_name);
	m_fw_type.SetItemData(Adapter.fw_list++, FW_Table[RT_FW_NORMAL].fw_type);
	 if (IS_HARDWARE_SERIES(8821C) || IS_HARDWARE_8731AU )
	{
		m_fw_type.AddString(FW_Table[RT_FW_FLASH].fw_name);
		m_fw_type.SetItemData(Adapter.fw_list++, FW_Table[RT_FW_FLASH].fw_type);
	 }
}

void initChannelLists()
{
	char iniChnl[CHANNEL_NUM_MAX] = {0};
	int copied = 0;
	LPCSTR defaultChannel = "";

	defaultChannel = "1,2,3,4,5,6,7,8,9,10,11,12,13,14";
	copied = GetPrivateProfileString("2G_20M", "Channel", defaultChannel, iniChnl, CHANNEL_NUM_MAX, CONFIG_CHIP_FILE);
	if (copied != 0) { _initChannelFromINI(iniChnl, copied, Channels_2_4G_20M, &ChannelNum_2_4G_20M); }
	
	defaultChannel = "3,4,5,6,7,8,9,10,11";
	copied = GetPrivateProfileString("2G_40M", "Channel", defaultChannel, iniChnl, CHANNEL_NUM_MAX, CONFIG_CHIP_FILE);
	if (copied != 0) { _initChannelFromINI(iniChnl, copied, Channels_2_4G_40M, &ChannelNum_2_4G_40M); }

	defaultChannel = "36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,144,149,153,157,161,165,169,173,177";
	copied = GetPrivateProfileString("5G_20M", "Channel", defaultChannel, iniChnl, CHANNEL_NUM_MAX, CONFIG_CHIP_FILE);
	if (copied != 0) { _initChannelFromINI(iniChnl, copied, Channels_5G_20M, &ChannelNum_5G_20M); }
	
	defaultChannel = "38,46,54,62,102,110,118,126,134,142,151,159,167,175";
	copied = GetPrivateProfileString("5G_40M", "Channel", defaultChannel, iniChnl, CHANNEL_NUM_MAX, CONFIG_CHIP_FILE);
	if (copied != 0) { _initChannelFromINI(iniChnl, copied, Channels_5G_40M, &ChannelNum_5G_40M); }
	
	defaultChannel = "42,58,106,122,138,155,171";
	copied = GetPrivateProfileString("5G_80M", "Channel", defaultChannel, iniChnl, CHANNEL_NUM_MAX, CONFIG_CHIP_FILE);
	if (copied != 0) { _initChannelFromINI(iniChnl, copied, Channels_5G_80M, &ChannelNum_5G_80M); }
	
	defaultChannel = "50,114,163";
	copied = GetPrivateProfileString("5G_160M", "Channel", defaultChannel, iniChnl, CHANNEL_NUM_MAX, CONFIG_CHIP_FILE);
	if (copied != 0) { _initChannelFromINI(iniChnl, copied, Channels_5G_160M, &ChannelNum_5G_160M); }
}


void initCustomizeSetting()
{
	
	bHPConfig = GetPrivateProfileInt("Customize", "HPConfig", 0, CONFIG_CHIP_FILE);
	bAltDefAnt = GetPrivateProfileInt("Customize", "AltDefAnt", 0, CONFIG_CHIP_FILE);
	
}

#pragma endregion Global and Local Functions

#pragma region [Misc.]

void CMP819xVCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}


#pragma endregion Others

#pragma region [Autotest]
/* the following string array is for the auto test file */
#define MaxTestItemIndex 14
const char *const AutoTestItem[]={
    "None",
    "PketTx",
    "PketRx",
    "ConsTx",
    "CrSnTX",
    "OSnTTx",
    "PMACLb",
    "OFDMLb",
    "CCK_Lb",
    "RfStMd",
    "RfDSMd",
    "RfSlMd",
    "BBRest",
    "ReInit",
};

#define MaxRateTypeIndex 28
const char *const AutoRateType[]={
    "C=01",
    "C=02",
    "C=55",
    "C=11",
    "O=06",
    "O=09",
    "O=12",
    "O=18",
    "O=24",
    "O=36",
    "O=48",
    "O=54",
    "M=00",
    "M=01",
    "M=02",
    "M=03",
    "M=04",
    "M=05",
    "M=06",
    "M=07",
    "M=08",
    "M=09",
    "M=10",
    "M=11",
    "M=12",
    "M=13",
    "M=14",
    "M=15",
};

#define MaxPreambleIndex 4
const char *const AutoPreamble[]={
    "LPr",
    "SPr",
    "LGI",
    "SGI",
};

//#define MaxModulationIndex 4
//const char *const AutoModulation[]={
//    "A",
//    "B",
//    "G",
//    "N",
//};

#define MaxPktPtnIndex 17
const char *const AutoPktPtn[]={
    "00",
    "05",
    "0A",
    "0F",
    "50",
    "55",
    "5A",
    "5F",
    "A0",
    "A5",
    "AA",
    "AF",
    "F0",
    "F5",
    "FA",
    "FF",
    "Rm",
};


#define TEST_ITEM_MAXBYTE               100
#define TEST_ITEM_MAXCASE               300
char AutoTestArray[TEST_ITEM_MAXCASE][TEST_ITEM_MAXBYTE];
int AutoTestNum;
int AutoTestDoneCnt;
int AutoTestTxWaitCnt;
void CMP819xVCDlg::StartAutoTestTx()
{
    char sline[256];
    FILE *PFile;
    char tempStr[20];

    AutoTestNum = 0;
    AutoTestDoneCnt = 0;
    AutoTestTxWaitCnt = 0;
    AutoTxWriteFileIndex = 1;       // start from 1

    if ((PFile=fopen("autotest.txt", "r")) == NULL)
    {
        ErrMsgBox(TEXT("Reading autotest File Failure !!"));
    }
    else
    {
        while(1)
        {
            fgets(sline, TEST_ITEM_MAXBYTE-1, PFile);   /* get one line */
            if ((sline[0] != '/') && (sline[0] != NULL) &&
                (sline[0] != ' ') && (sline[0] != 0x0A))    // 0x0A = NEW LINE
            {
                if (sline[0] == ':')    /* end of file */
                {
                    sscanf(sline, "%s", tempStr);
                    if(strcmp(tempStr, ":END") == 0)
                        break;
                }
                else
                {
                    strcpy(AutoTestArray[AutoTestNum++], sline);
                }
            }
        }
        fclose(PFile);

        AutoTestFlag = TRUE;
        //SetTimer(AutoTestProcessTx_TIMER, 100, NULL);
    }

}

#define AutoTestTxWaitTimes    10   //10*100ms
void CMP819xVCDlg::AutoTestProcessTx()
{
    int i;
    char TxRsltStr[100], tempTxRsltStr[100];
    FILE *PFile;
    char tempStr[20];
    char TItemStr[20], RateStr[20], PreambleStr[20];
    char ModulationStr[20], PktPtnStr[10], PktLenStr[10], PktCntStr[10];
    BOOL bResult;
    int TxOK;

    if(TxThreadInProgress)      //ThreadXmit is still under progress.
        return;

    if(TestStop)
    {
        if(AutoTestDoneCnt >= AutoTestNum)  //finish the testing.
        {
            StopAutoTest();
            m_FileIndex.SetCurSel(0);
            AutoTestType = AUTO_TEST_NONE;
            ItemEnDisable(TRUE, 1);
            return;
        }
        if(AutoTestTxWaitCnt++ < AutoTestTxWaitTimes)   //wait to start next test item
            return;

        sscanf(&AutoTestArray[AutoTestDoneCnt][0], "%s %s %s %s %s %s %s",
                TItemStr, RateStr, PreambleStr, ModulationStr,
                PktPtnStr, PktLenStr, PktCntStr);
        /* Test Item */
        for(i=0; i<MaxTestItemIndex; i++)
        {
            if(strcmp(TItemStr, AutoTestItem[i]) == 0)
            {
                m_TestList.SetCurSel(i);
                OnSelchangeTestList();
            }
        }
        /* Data Rate */
        for(i=0; i<MaxRateTypeIndex; i++)
        {
            if(strcmp(RateStr, AutoRateType[i]) == 0)
            {
                m_Rate.SetCurSel(i);
                OnSelchangeRate();
            }
        }
        if( IS_CCK_RATE(Adapter.CurrentRate) || IS_HT_RATE(Adapter.CurrentRate))
        {
        	/* Preamble, guard interval */
        	for(i=0; i<MaxPreambleIndex; i++)
        	{
        	    if(strcmp(PreambleStr, AutoPreamble[i]) == 0)
        	    {
        	        m_Preamble.SetCurSel(i);
        	        OnSelchangePreamble();
        	    }
        	}
    	}
        /* PktPtn */
        for(i=0; i<MaxPktPtnIndex; i++)
        {
            if(strcmp(PktPtnStr, AutoPktPtn[i]) == 0)
            {
                m_Pattern.SetCurSel(i);
                OnSelchangePattern();
            }
        }
        /* Packet Length */
        m_PacketLength.SetWindowText(PktLenStr);
        /* Packet Counts */
        m_TxCount.SetWindowText(PktCntStr);
        Adapter.PacketCount = atoi(PktCntStr);
        OnStartTesting();   //Start to test
        sprintf(tempStr, "Test item %d is under progress!", AutoTestDoneCnt+1);
        m_Result.SetWindowText(TEXT(tempStr));
    }
    else    //finish 1 test item
    {
        AutoTestTxWaitCnt = 0;
        bResult = QueryTxPacketSent(
            (LPADAPTER) Adapter.hFile,
            (int*)&TxOK
        );
        _stprintf(tempStr, TEXT("%d"), TxOK);
        m_XmitOK.SetWindowText(tempStr);
        OnStartTesting();   //Stop testing.

        if ((PFile=fopen("resultTx.txt", "at")) == NULL)
        {
            ErrMsgBox(TEXT("Write ResultTx File Failure !!"));
        }
        else
        {
            int len;
            sprintf(tempTxRsltStr, &AutoTestArray[AutoTestDoneCnt][0]);
            len = strlen(tempTxRsltStr);
            tempTxRsltStr[len-1] = 0;
            sprintf(TxRsltStr, "%d. %s, TxOk : %d", AutoTxWriteFileIndex++,
                    tempTxRsltStr,//&AutoTestArray[AutoTestDoneCnt][0],
                    TxOK
                    );
            fprintf(PFile, "%s\n", TxRsltStr);
            fclose(PFile);
        }

        AutoTestDoneCnt++;
        sprintf(tempStr, "Test item %d is done!", AutoTestDoneCnt);
        m_Result.SetWindowText(TEXT(tempStr));
    }
}


void CMP819xVCDlg::StartAutoTestRx()
{
    AutoTestRxOn = TRUE;
    AutoRxWriteFileIndex = 0;
    m_TestList.SetCurSel(PACKETS_RX);
    OnSelchangeTestList();
    OnStartTesting();   //Start to test
    m_Result.SetWindowText(TEXT("Auto Rx test started!"));
    m_Start.EnableWindow(FALSE);
}

void CMP819xVCDlg::AutoConfigRegister()
{
    char sline[256];
    FILE *PFile;
    char outStr[256];
    long addr, data;
    if ((PFile=fopen("register.txt", "r")) == NULL)
    {
        ErrMsgBox(TEXT("Reading register File Failure !!"));
    }
    else
    {
        do {
            fgets(sline, 100, PFile);
            if ((sline[0] != '/')&& (sline[0] != NULL))
            {
                sscanf(sline, "%lx %lx", &addr, &data);
                if ( addr!=0xff )
                {
                    sprintf(outStr, "0x%02x 0x%04lx\n", addr, data);
                    //ErrMsgBox(TEXT(outStr));
                    //PutRegSetting(addr, bMaskDWord, data);

                }
                else
                    break;
            }
        } while ( addr!=0xff );

        fclose(PFile);
    }
}

void CMP819xVCDlg::StopAutoTest()
{
    switch(AutoTestType)
    {
        case AUTO_TEST_TX_SITE:// Auto test (Tx site)
            if(AutoTestFlag)
            {
                KillTimer(AutoTestProcessTx_TIMER);
                AutoTestNum = 0;
                AutoTestDoneCnt = 0;
                AutoTestFlag = FALSE;
            }
            break;
        case AUTO_TEST_RX_SITE:// Auto test (Rx site)
            OnStartTesting();   //Stop testing.
            break;
        default:
            m_FileIndex.SetCurSel(0);
            AutoTestType = AUTO_TEST_NONE;
            break;
    }
    m_Start.EnableWindow(TRUE);
}
#pragma endregion Autotest

///////////////////////////////////////////jdjdjd
#include "common.h"
#include "wt208_vsa.h"
#pragma comment(lib,"WLAN.Tester.API.lib")

static int connect_id = -1;
CMP819xVCDlg *gDlg;

char TesterIpaddress[32] ={0}; //"192.168.10.254";
static VsaParameter vsaParam;
static VsgParameter vsgParam;
double g_wifi_freq = 2412 * 1e6;
double g_txpower = -10;
double g_external_gain = 0.0;
static bool is_enable_vsg = false;
char VSGWavePath[256] = {0};
static HANDLE handle_VSG = INVALID_HANDLE_VALUE;
DWORD vsgThreadId =NULL;
HANDLE threadEvent;
static volatile bool is_vsg_stop = false;
static volatile bool is_vsg_start = false;
const int MAX_DEMO_LOOP = 1000;
VSATHREADDATA vsaThData;

//wt208_vsa **vsa_demo;
wt208_vsa *vsa_demo=NULL;
int vsa_size = 0;
char memoBuf[1024];

void CMP819xVCDlg::initVSA()
{
	int ret;
	vsa_size = get_thread_config();
	//vsa_demo = new wt208_vsa *[vsa_size];
	//for(int i = 0; i < vsa_size; i++)
	//{
		vsa_demo = new wt208_vsa();
		ret = vsa_demo->init();
	//}

}
#define MAP_PREFIX          "Local\\"
#define MAP_NAME            "SampleMap"
#define FULL_MAP_NAME       MAP_PREFIX MAP_NAME
// Max size of the file mapping object.
#define MAP_SIZE            65536
// File offset where the view is to begin.
#define VIEW_OFFSET         0

// The number of bytes of a file mapping to map to the view. All bytes of the 
// view must be within the maximum size of the file mapping object (MAP_SIZE). 
// If VIEW_SIZE is 0, the mapping extends from the offset (VIEW_OFFSET) to  
// the end of the file mapping.
#define VIEW_SIZE           1024

HANDLE handlesynEvent;
PVOID pView = NULL;

int CMP819xVCDlg::XXInit()
{
	HANDLE hMapFile = NULL;
	

	// Create the file mapping object.
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,   // Use paging file - shared memory
		NULL,                   // Default security attributes
		PAGE_READWRITE,         // Allow read and write access
		0,                      // High-order DWORD of file mapping max size
		MAP_SIZE,               // Low-order DWORD of file mapping max size
		FULL_MAP_NAME           // Name of the file mapping object
		);
	if (hMapFile == NULL) 
	{
		myprintf("CreateFileMapping failed w/err 0x%08lx\n", GetLastError());
		goto Cleanup;
	}
	myprintf("The file mapping (%s) is created\n", FULL_MAP_NAME);
	pView = MapViewOfFile(
		hMapFile,               // Handle of the map object
		FILE_MAP_ALL_ACCESS,    // Read and write access
		0,                      // High-order DWORD of the file offset 
		VIEW_OFFSET,            // Low-order DWORD of the file offset 
		VIEW_SIZE               // The number of bytes to map to view
		);
	if (pView == NULL)
	{ 
		myprintf("MapViewOfFile failed w/err 0x%08lx\n", GetLastError()); 
		goto Cleanup;
	}
	myprintf("The file view is mapped\n");

	// Prepare a message to be written to the view.
	PSTR pszMessage = "nima";
	DWORD cbMessage = (strlen(pszMessage) + 1) * sizeof(*pszMessage);

	// Write the message to the view.
	memcpy(pView, pszMessage, cbMessage);

	myprintf("This message is written to the view:\n\"%s\"\n",
		pszMessage);
	handlesynEvent = CreateEventA(NULL, TRUE, FALSE, "Robots_Sync_Signal");
	//getchar();
	return 0;
Cleanup:

	if (hMapFile)
	{
		if (pView)
		{
			// Unmap the file view.
			UnmapViewOfFile(pView);
			pView = NULL;
		}
		// Close the file mapping object.
		CloseHandle(hMapFile);
		hMapFile = NULL;
	}
	return -1;

}
#pragma region [Initialization]
int CMP819xVCDlg::NeedDisabled11ACMode(void)
{
	int flag;
	QueryChipIf8731A((LPADAPTER)Adapter.hFile,&flag);
	return !!flag;
}
// Main Initialization
BOOL CMP819xVCDlg::OnInitDialog()
{
	int     stringBufferLen = 100;
	int     nTotalChnlCnt = 60;

	CDialog::OnInitDialog();
	OnInitGlobals();									// init global variables
	EnableToolTips();
	
	//if( ! StartDriverService() )						// Start protocol service.
    //    MP_QUIT();
	Adapter.hFile = StartDutByMode(regAdapter.iMode);	// Start DUT
	if( Adapter.hFile == NULL)
		MP_QUIT();
	if(StartTest((LPADAPTER)Adapter.hFile) != 1) {		// Start MassProduction Testing
		ErrMsgBox(TEXT("StartTest Fail!!"));
		CloseDUT((LPADAPTER)Adapter.hFile);
		MP_QUIT();
	}

	initBW_40();
	initBW_Disable11AC();
	SetupUI(); 											// Setup UI layout for different ICs.
	OnInitHWType(); 									// get eeprom type and RF mode
	OnInitItemTest(); 									// TestItem initialization
	SetTxPowerControlRange(0, MAX_TXPWR_IDX_NMODE_92S); // set tx power index range
    OnInitWirelessMode(); 								// wireless mode
	OnInitItemAntTxRx(); 								// Actually, RF Path (No RF would cause hang)
	OnInitItemRFPathSetting();							// BTG/WLG/WLA Switch
	OnInitItemBandWidth(); 								// 20M/40M/80M/TXSC control
    OnInitItemChannel(); 								// channel item
    OnInitItemRate(); 									// rate item
	OnInitItemRfRegPath(); 								// Path of RF register	
	OnInitTxPwrTrack();
	
	OnInitPwrIndexBase();								// Tx Power Index From UI or Efuse
	OnInitItemPreambleGuardInterval(); 					// Preamble, Guard Interval
	OnInitItemTxPktSetting(); 							// Tx packet setting
	OnInitTestResult(); 								// Test Result init
	
	OnInitRfPathSwitch();							// Actually, T/Rx Antenna switch


	OnInitItemAutoTest(); 								// Auto function
	OnInitXtalCal();

	//Only for 8822be, 8821ce
	if (Adapter.HardwareId == HID_8822BE || Adapter.HardwareId == HID_8821CE)
	{
		m_MonitorValue1.ShowWindow(TRUE);
		m_ButtonMonitor.SetWindowText(TEXT("- Monitor"));
		SetTimer(RSSI_MONITOR, 300, NULL);
	}	
	
	UpdateTxPower(GetCurrChannelTxPower(), 0); 			// Tx Power
	LoadRTKBTDLL();
	//SetTimer(DUT_MONITOR, 500, NULL); // Under experiment
	
	if(CustomPowerTableStatus > 0)
		SetTimer(DIALOG_MONITOR, 200, NULL);
	gDlg = this;
	m_edit_ip.SetWindowText("192.168.10.254");
	//XXInit();
	WT_DLLInitialize();
	initVSA();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// Core Functions & Related
BOOL CMP819xVCDlg::OnInitGlobals()
{

	InitRegAdapter(); // Init the global variable "regAdapter"
	
	initChannelLists();

	initBW();
	initRate();
	initCustomizeSetting();

	GetHardwareID();
	if (Adapter.HardwareId == -1) {
	    ErrMsgBox(TEXT("Not support this Chip ID or Version!!"));
	    MP_QUIT();
	}
	Adapter.BufferSize = 2400;
	Adapter.hMem = GlobalAlloc(GMEM_MOVEABLE,2400);
	Adapter.lpMem = (UCHAR*)GlobalLock(Adapter.hMem);
	Adapter.hMem2 = GlobalAlloc(GMEM_MOVEABLE,2400);
	Adapter.lpMem2 = (UCHAR*)GlobalLock(Adapter.hMem2);
	Adapter.bIsMonitorOn = FALSE;
	
	// Set Status
	TestStop = TRUE;
	TxTrackFlagStop= TRUE;   //added by vivi, for tx power tracking
	bInReset = FALSE;
	bEEByteMode = FALSE;
	bEEWiFiMode = TRUE;
	bBTTest = FALSE;


	// 2016/01/08 Ping_Yan Add for Tx Power Index Base From Efuse or UI
	PwrIndexFromEfuse = FALSE;

	//cosa add for LED control
	LED1Switch = FALSE;
	LED2Switch = FALSE;
	
	G_xhThrd = INVALID_HANDLE_VALUE;	//0
	// for auto test function
	TxThreadInProgress = FALSE;
	AutoTestFlag = FALSE;      
	Adapter.PacketCount = 0;          
	AutoTestRxOn = FALSE;      
	RxPktNumNoChgCnt = 0;      

	// Using One Byte Alignment
	bOneByteAlign = FALSE;

	// 2016/01/08 Ping_Yan Add, to Disable Ui TxPower Combo-box When Tx Power Tracking
	FixIdxBaseWhenPwrTrack = FALSE;

	EfuseFont.CreateFont(
					13,                        // nHeight
					0,                        // nWidth
					0,                        // nEscapement
					0,                        // nOrientation
					FW_BOLD ,                // nWeight
					FALSE,                    // bItalic
					FALSE,                    // bUnderline
					0,                        // cStrikeOut
					ANSI_CHARSET,              // nCharSet
					OUT_DEFAULT_PRECIS,        // nOutPrecision
					CLIP_DEFAULT_PRECIS,      // nClipPrecision
					DEFAULT_QUALITY,          // nQuality
					DEFAULT_PITCH | FF_SWISS,
					_T("Courier New")                    // nPitchAndFamily
			); 

	EfuseFont_Small.CreateFont(
					12,                        // nHeight
					_PARAS_,
					_T("Courier New")          // nPitchAndFamily
			);

	NormalFont.CreateFont(
					16,						// nHeight
					_PARAS_,
					_T("Courier New")		// nPitchAndFamily
			);

	SmallFont.CreateFont(
					14,						// nHeight
					_PARAS_,
					_T("Times New Roman")		// nPitchAndFamily
			);

	return TRUE;
}
void CMP819xVCDlg::SetupUI()
{

	//==========================================================
	//                                      Setup Default UI
	//==========================================================
	
	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
	    CString strAboutMenu;
	    strAboutMenu.LoadString(IDS_ABOUTBOX);
	    if (!strAboutMenu.IsEmpty())
	    {
	        pSysMenu->AppendMenu(MF_SEPARATOR);
	        pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	    }
	}
	// set dialog title
	if (regAdapter.iMode != CREATOR_MODE)
		OnInitSetTitle();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon

	m_Button_DIG.ShowWindow(FALSE);
	m_Button_DPK.ShowWindow(FALSE);
	m_Button_U2U3.ShowWindow(FALSE);
	m_Button_DevBB.ShowWindow(FALSE);
	m_Modulation_Static.ShowWindow(FALSE);

	// PowerLimit and PowerByRate Selection
	GetEnumID();
	OnInitCustomerPwr();
	OnInitCustomerPwrLmt();
	//OnInitCheckCustomPowerTables(); //init check registry for power table status
	m_Button_CSC.ShowWindow(TRUE);
	m_CustomerSelPwr.ShowWindow(TRUE);
	m_CustomerSelPwrLmt.ShowWindow(TRUE);
	m_CheckBox_EnableTxPowerLimit.ShowWindow(TRUE);
	m_CheckBox_PwrIndexFromEfuse.ShowWindow(FALSE);
	// These Customer Power Table Settings should be disabled On Open UI
	m_CustomerSelPwr.EnableWindow(FALSE);
	m_CustomerSelPwrLmt.EnableWindow(FALSE);
	m_CheckBox_EnableTxPowerLimit.EnableWindow(FALSE);
	m_ButtonKfree.EnableWindow(FALSE);
	m_KfreeValue.EnableWindow(FALSE);


	m_CheckBox_LDPC.ShowWindow(FALSE);
	m_CheckBox_STBC.ShowWindow(FALSE);

	m_TxSC_Selection.ShowWindow(FALSE);
	m_TxSC_Help_Btn.ShowWindow(FALSE);
	m_TxSC_Label.ShowWindow(FALSE);

	m_EEByteOffset.EnableWindow(TRUE);
	m_EEByteValue.EnableWindow(TRUE);
	m_Led1.ShowWindow(TRUE);
	m_EfuseTargetSel.SetCurSel(TARGET_BYTE);

	SetComponent(); 

	m_RcvPhyErr.SetWindowText("PHYERR");
	m_RcvPhyOK.SetWindowText("PHYOK");
	m_RcvMacErr.SetWindowText("MACERR");
	m_RcvMacOK.SetWindowText("MACOK");

	// Tx PMAC (Since 8814A)
	m_TxPMAC_Switch.ShowWindow(FALSE);
	m_TxPMAC_TypeSel.ShowWindow(FALSE);
	m_TxPMAC_Type_Label.ShowWindow(FALSE);
	m_TxPMAC_Period.ShowWindow(FALSE);
	m_TxPMAC_Period_Label.ShowWindow(FALSE);
	
	// Rx Ack Counter Related Component
	m_RxAckPktCounter_Label.ShowWindow(FALSE);
	m_RxAckPktCounter.ShowWindow(FALSE);
	m_RxAckPktCounter_Enable_Btn.ShowWindow(FALSE);

	// Rx Packet Filter Button
	m_RxFilter_Enable_Btn.ShowWindow(FALSE);

	// Antenna Setting (Since 8821C)
	m_RFPathSetting.ShowWindow(FALSE);
	m_RFPathSet_Label.ShowWindow(FALSE);

	// hide BT button, useless now
	m_BT.ShowWindow(FALSE);

	// hide button, except 8710BU
	m_SysOnRegRead.ShowWindow(FALSE);
	m_SysOnRegWrite.ShowWindow(FALSE);
	m_AutoInstall.ShowWindow(FALSE);
	m_Flash.ShowWindow(FALSE);
	
	// hide RFE Type Text
	m_RFEType.ShowWindow(FALSE);
	
	// hide the "Normal Mode" RFPathSetting Combobox
	m_RFPathSet_Label_Duplicate.ShowWindow(FALSE);
	m_RFPathSetting_Duplicate.ShowWindow(FALSE);

	m_fw_type.AddString(FW_Table[RT_FW_NORMAL].fw_name);
	m_fw_type.SetItemData(Adapter.fw_list++, FW_Table[RT_FW_NORMAL].fw_type);
	m_fw_type.EnableWindow(TRUE);
	m_fw_type.ShowWindow(FALSE);
	m_static_fw.ShowWindow(FALSE);

	m_CheckBox_EnhanceAntiInterference.ShowWindow(FALSE);

	m_BtPinGet.ShowWindow(FALSE);
	m_BtPinSet.ShowWindow(FALSE);
	m_BtPinValue.ShowWindow(FALSE);

	//==========================================================
	//                                      Setup  UI By Chip
	//==========================================================

	// Common
	if (regAdapter.iMode == CREATOR_MODE)
	{
		// Rx Packet Filter Button
		m_RxFilter_Enable_Btn.ShowWindow(TRUE);

		// Rx Ack Counter Related Component
		m_RxAckPktCounter_Label.ShowWindow(TRUE); 		//Rx Ack Pkt Counter Label
		m_RxAckPktCounter.ShowWindow(TRUE); 			//Rx Ack Pkt Counter
		m_RxAckPktCounter_Enable_Btn.ShowWindow(TRUE); 	//Rx Ack Pkt Counter Enable Button
		
		// RFE Type
		m_RFEType.ShowWindow(TRUE);
	}

	if (IS_HARDWARE_SERIES(8814A)) 
	{
		
		//------------- Disabled ---------------
		
		m_BT_WiFi.ShowWindow(FALSE);
		m_BT.ShowWindow(FALSE);

		//------------- Enabled ---------------
		if (regAdapter.iMode == CREATOR_MODE){
			m_Button_DevBB.ShowWindow(TRUE);
			m_TxSC_Selection.ShowWindow(TRUE);
			m_TxSC_Help_Btn.ShowWindow(TRUE);
			m_TxSC_Label.ShowWindow(TRUE);
		}
		
		m_TxPowerB_Label.SetWindowText("1");
		m_TxPowerC_Label.SetWindowText("2");
		m_TxPowerD_Label.SetWindowText("3");

		m_TxPMAC_Switch.ShowWindow(TRUE); // Tx PMAC
		m_RfPathSwitch.ShowWindow(TRUE);
        m_Modulation.ShowWindow(TRUE);
		m_Modulation_Static.ShowWindow(TRUE);
		m_CheckBox_LDPC.ShowWindow(TRUE);
		m_CheckBox_STBC.ShowWindow(TRUE);
		m_Button_DIG.ShowWindow(TRUE);
		
		if (Adapter.HardwareId == HID_8814AU)
			m_Button_U2U3.ShowWindow(TRUE);
		
		OnInitPmacMode();

	} 
	else if (IS_HARDWARE_SERIES(8812A))
	{
		
		//------------- Disabled ---------------
		
		m_BT_WiFi.ShowWindow(FALSE);
		m_BT.ShowWindow(FALSE);

		//------------- Enabled ---------------
		if (regAdapter.iMode == CREATOR_MODE){
			m_TxSC_Selection.ShowWindow(TRUE);
			m_TxSC_Help_Btn.ShowWindow(TRUE);
			m_TxSC_Label.ShowWindow(TRUE);

			m_TxPMAC_Switch.ShowWindow(TRUE); // Tx PMAC
			OnInitPmacMode();
		}

		m_RfPathSwitch.ShowWindow(TRUE);
        m_Modulation.ShowWindow(TRUE);
		m_Modulation_Static.ShowWindow(TRUE);
		m_CheckBox_LDPC.ShowWindow(TRUE);
		m_CheckBox_STBC.ShowWindow(TRUE);
		m_Button_DIG.ShowWindow(TRUE);
		m_Button_DPK.ShowWindow(TRUE);
		
		if (Adapter.HardwareId == HID_8812AU)
			m_Button_U2U3.ShowWindow(TRUE);
		
	} 
	else if (IS_HARDWARE_SERIES(8821A))
	{
		
		//------------- Disabled ---------------
		
		m_BT_WiFi.ShowWindow(FALSE);
		m_BT.ShowWindow(FALSE);
		
		//------------- Enabled ---------------
		if (regAdapter.iMode == CREATOR_MODE){
			m_TxSC_Selection.ShowWindow(TRUE);
			m_TxSC_Help_Btn.ShowWindow(TRUE);
			m_TxSC_Label.ShowWindow(TRUE);
		}

        m_RfPathSwitch.ShowWindow(TRUE);
		m_Modulation.ShowWindow(TRUE);
		m_Modulation_Static.ShowWindow(TRUE);
		m_CheckBox_LDPC.ShowWindow(TRUE);
		m_CheckBox_STBC.ShowWindow(TRUE);
		m_Button_DIG.ShowWindow(TRUE);
	
	}
	else if (IS_HARDWARE_SERIES(8821C) || IS_HARDWARE_8731AU)
	{
		//------------- Disabled ---------------
		
		m_BT_WiFi.ShowWindow(FALSE);
		m_BT.ShowWindow(FALSE);
		
		//------------- Enabled ---------------
		if (regAdapter.iMode == CREATOR_MODE){

			m_RFPathSetting.ShowWindow(TRUE); // BTG/WLG Switch
			m_RFPathSet_Label.ShowWindow(TRUE);

			m_RFPathSetting_Duplicate.ShowWindow(FALSE);
			m_RFPathSet_Label_Duplicate.ShowWindow(FALSE);

			m_Button_DevBB.ShowWindow(TRUE);
			m_TxSC_Selection.ShowWindow(TRUE);
			m_TxSC_Help_Btn.ShowWindow(TRUE);
			m_TxSC_Label.ShowWindow(TRUE);
		}
		else
		{
			m_RFPathSetting_Duplicate.ShowWindow(TRUE);
			m_RFPathSet_Label_Duplicate.ShowWindow(TRUE);
		}

		if(Adapter.HardwareId == HID_8821CE)
		{
			m_MonitorRSSIText.ShowWindow(TRUE);
			m_MonitorRSSIValue1.ShowWindow(TRUE);
		}
		else
		{
			m_MonitorRSSIText.ShowWindow(FALSE);
			m_MonitorRSSIValue1.ShowWindow(FALSE);
		}

		// Using One Byte Alignment
		bOneByteAlign = TRUE;

		m_TxSC_Label.SetWindowText("T/RxSC");

		m_RfPathSwitch.ShowWindow(TRUE);
		m_Modulation.ShowWindow(TRUE);
		m_Modulation_Static.ShowWindow(TRUE);
		m_CheckBox_LDPC.ShowWindow(TRUE);
		m_CheckBox_STBC.ShowWindow(FALSE); //set to hidden, suggested by RF fengyu
		m_Button_DIG.ShowWindow(FALSE); //set to hidden, suggested by RF fengyu
		
		 if( TRUE == initFlash() )
		 {
			m_AutoInstall.ShowWindow(TRUE);
			m_Flash.ShowWindow(TRUE);
			m_fw_type.AddString(FW_Table[RT_FW_FLASH].fw_name);
			m_fw_type.SetItemData(Adapter.fw_list++, FW_Table[RT_FW_FLASH].fw_type);
			m_fw_type.ShowWindow(TRUE);
			m_static_fw.ShowWindow(TRUE);
		 }
		 
		m_TxPMAC_Switch.ShowWindow(TRUE); // Tx PMAC
		OnInitPmacMode();
	}
	else if (IS_HARDWARE_SERIES(8822B))
	{
		
		//------------- Disabled ---------------
		
		m_BT_WiFi.ShowWindow(FALSE);
		m_BT.ShowWindow(FALSE);
		
		//------------- Enabled ---------------
		if (regAdapter.iMode == CREATOR_MODE){
			m_Button_DevBB.ShowWindow(TRUE);
			m_TxSC_Selection.ShowWindow(TRUE);
			m_TxSC_Help_Btn.ShowWindow(TRUE);
			m_TxSC_Label.ShowWindow(TRUE);

		}

		if(Adapter.HardwareId == HID_8822BE)
		{
			m_MonitorRSSIText.ShowWindow(TRUE);
			m_MonitorRSSIValue1.ShowWindow(TRUE);
		}
		else
		{
			m_MonitorRSSIText.ShowWindow(FALSE);
			m_MonitorRSSIValue1.ShowWindow(FALSE);
		}

		// Using One Byte Alignment
		bOneByteAlign = TRUE;
		
		m_TxSC_Label.SetWindowText("T/RxSC");

        m_RfPathSwitch.ShowWindow(TRUE);
		m_Modulation.ShowWindow(TRUE);
		m_Modulation_Static.ShowWindow(TRUE);
		m_CheckBox_LDPC.ShowWindow(TRUE);
		m_CheckBox_STBC.ShowWindow(TRUE);
		m_Button_DIG.ShowWindow(TRUE);
		FixIdxBaseWhenPwrTrack = TRUE; //When Tx Power Track is on, Disable TxPower Index Combo-box
		m_CheckBox_PwrIndexFromEfuse.ShowWindow(TRUE);

		//For MS
		if( GetPrivateProfileInt("USER", "EnhanceAntiInference", 1, CONFIG_CHIP_FILE) == 1)
		{
			ULONG	RFEType = 0;
			QueryRFEType((LPADAPTER)(Adapter.hFile), &RFEType);
			if((RFEType&0xFF)==15)
			{
				SetEnhanceAntiInterference( (LPADAPTER) Adapter.hFile,FALSE);
				m_CheckBox_EnhanceAntiInterference.ShowWindow(TRUE);
			}
		}


		m_TxPMAC_Switch.ShowWindow(TRUE); // Tx PMAC
		OnInitPmacMode();
		
	}
	else if (IS_HARDWARE_SERIES(8723A) || IS_HARDWARE_SERIES(8723B))
	{
		m_BT.ShowWindow(FALSE);

	}
	else if (IS_HARDWARE_SERIES(8703B))
	{
		m_BT.ShowWindow(FALSE);
		bOneByteAlign = TRUE; // Enable One Byte Aligned Since 8703B

	}
	else if (IS_HARDWARE_SERIES(8723D))
	{
		m_BT.ShowWindow(FALSE);
		bOneByteAlign = TRUE; // Enable One Byte Aligned Since 8703B
		FixIdxBaseWhenPwrTrack = TRUE; //When Tx Power Track is on, Disable TxPower Index Combo-box
		m_CheckBox_PwrIndexFromEfuse.ShowWindow(TRUE);
		m_ButtonKfree.EnableWindow(TRUE);
		m_KfreeValue.EnableWindow(TRUE);

		if(Adapter.HardwareId == HID_8723DS)
		{
			m_BtPinGet.ShowWindow(TRUE);
			m_BtPinSet.ShowWindow(TRUE);
			m_BtPinValue.ShowWindow(TRUE);
		}

	}
	else if (IS_HARDWARE_SERIES(8192E))
	{
		m_BT.ShowWindow(FALSE);
		m_RfPathSwitch.ShowWindow(TRUE);
		
	}
	else if (IS_HARDWARE_SERIES(8188E))
	{
		m_BT_WiFi.ShowWindow(FALSE);
		m_BT.ShowWindow(FALSE);
		
		//<20150213, Enable Custom PowerTables for 88E series>
		//m_Button_CSC.ShowWindow(FALSE);
		//m_CustomerSelPwr.ShowWindow(FALSE);
		//m_CustomerSelPwrLmt.ShowWindow(FALSE);
		//m_CheckBox_EnableTxPowerLimit.ShowWindow(FALSE);
	
	}
	else if (IS_HARDWARE_SERIES(8188F))
	{
		
		bOneByteAlign = TRUE;

		//<20150806, Enable Custom PowerTables for 88F series>
		//m_Button_CSC.ShowWindow(FALSE);
		//m_CustomerSelPwr.ShowWindow(FALSE);
		//m_CustomerSelPwrLmt.ShowWindow(FALSE);
		//m_CheckBox_EnableTxPowerLimit.ShowWindow(FALSE);

	}
	else if (IS_HARDWARE_SERIES(8710B))
	{
		bOneByteAlign = TRUE;
		m_BT_WiFi.ShowWindow(FALSE);
		m_BT.ShowWindow(FALSE);	
		FixIdxBaseWhenPwrTrack = TRUE; //When Tx Power Track is on, Disable TxPower Index Combo-box
		m_CheckBox_PwrIndexFromEfuse.ShowWindow(TRUE);
		m_SysOnRegRead.ShowWindow(TRUE);
		m_SysOnRegWrite.ShowWindow(TRUE);
		
		if( TRUE == initFlash() )
		{
			m_AutoInstall.ShowWindow(TRUE);
			m_Flash.ShowWindow(TRUE);
		}
	}
	else if (IS_HARDWARE_SERIES(8192F))
	{
		m_BT.ShowWindow(FALSE);
		bOneByteAlign = TRUE; // Enable One Byte Aligned Since 8703B
		FixIdxBaseWhenPwrTrack = TRUE; //When Tx Power Track is on, Disable TxPower Index Combo-box
		m_CheckBox_PwrIndexFromEfuse.ShowWindow(TRUE);
		m_ButtonKfree.EnableWindow(TRUE);
		m_KfreeValue.EnableWindow(TRUE);
		m_Button_DPK.ShowWindow(TRUE);
		if (regAdapter.iMode == CREATOR_MODE){
			m_Button_DevBB.ShowWindow(TRUE);
			m_TxSC_Selection.ShowWindow(TRUE);
			m_TxSC_Help_Btn.ShowWindow(TRUE);
			m_TxSC_Label.ShowWindow(TRUE);
		}

		if( TRUE == initFlash() )
		{
			m_AutoInstall.ShowWindow(TRUE);
			m_Flash.ShowWindow(TRUE);
		}
	}	
	else
	{
		m_BT.ShowWindow(FALSE);
		m_CheckBox_LDPC.ShowWindow(FALSE);
		m_CheckBox_STBC.ShowWindow(FALSE);
		m_MonitorRSSIText.ShowWindow(FALSE);
		m_MonitorRSSIValue1.ShowWindow(FALSE);
	}


	if(Adapter.HardwareId != HID_8723AS) {
		m_BT_Port_No.ShowWindow(FALSE);
		m_BT_Port_No_text.ShowWindow(FALSE);		
		m_BT_Rate.ShowWindow(FALSE);		
		m_BT_Rate_text.ShowWindow(FALSE);				
	} else {
		m_BT_Port_No_text.SetWindowText("1");
		m_BT_Rate_text.SetWindowText("115200");				
		m_BT_Rate_text.SetReadOnly(); // <20120703, Kordan> To support H4, so fixed at 115200.
		m_RxPhyOkCheckBox.SetCheck(BST_CHECKED);
		m_RxPhyErrCheckBox.SetCheck(BST_CHECKED);
		m_RxMacOkCheckBox.SetCheck(BST_CHECKED);
		m_RxMacErrCheckBox.SetCheck(BST_CHECKED);
	}
}
void CMP819xVCDlg::GetEnumID()
{
	TCHAR 			szBuf[MAX_PATH];
	CString			targetFile1;
	CString			targetFile2;
	TCHAR			Buf1[MAX_PATH];
	
	HKEY 			hKey;
	DWORD			keyStatus	= FALSE;
	UINT			initStatus	= FALSE;
	int				regStatus1	= FALSE;
	int				regStatus2	= FALSE;
	UINT			loadStatus	= FALSE;
	DWORD 			dwType, cbBuf;
	UINT			bFound		= NULL;
	const UINT 		MAX_NID_NUM = 10000;
	CString 		Str1, Str2, Str3, NID;

	NID.Format("%s",Adapter.NetCfgInstanceId);

	for(int i = 0;i < MAX_NID_NUM; i++){
		Str1.Format("\\%04d",i);
		Str2.Format("%s%s","SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}",
					Str1);
		strcpy(Buf1, (LPCTSTR)Str2);
		keyStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
								TEXT(Buf1),
								0,KEY_READ,&hKey);
		if (keyStatus == ERROR_SUCCESS) {
			cbBuf = (MAX_PATH - 1) * sizeof(TCHAR);
			Str3.Format("");
			if(RegQueryValueEx(hKey, TEXT("NetCfgInstanceId"), NULL, &dwType, (BYTE*)szBuf, &cbBuf) == ERROR_SUCCESS){
				
				Str3.Format("%s",szBuf);
				if(Str3 == NID){
					RegCloseKey(hKey);
					Adapter.enumId	= i;
					bFound			= TRUE;
					return;
				}
			}
			
		} else {
			continue;
		}
		
		RegCloseKey(hKey);
	}
	return;
}
void CMP819xVCDlg::OnInitSetTitle()
{
	CString Title = "";

    Title = Title + regAdapter.Name + " MP Diagnostic Program " + regAdapter.Version;
    SetWindowText(Title);
}
void CMP819xVCDlg::SetComponent()
{
    switch(regAdapter.iMode)
    {
    // DBG_MODE
    case 0:
        break;

    // INTERNAL_MODE
    case 1:
        m_Modulation_Static.ShowWindow(FALSE);
        m_Modulation.ShowWindow(FALSE);


        break;

    case 2:
    default:
        
        m_WriteMac.ShowWindow(FALSE);
        m_FileIndex.ShowWindow(FALSE);


        
        break;
    }
}

// Sub Functions
void CMP819xVCDlg::OnInitHWType()
{
	BOOL    bResult;
	TCHAR   strBuf[100];
	ULONG	RFEType = 0;
	
	
	Adapter.EEType = EEPROM_BOOT_EFUSE;
	
		
			m_EEpromType.SetWindowText(TEXT("E-FUSE"));
			m_UpdateEfuse.EnableWindow(true);
			m_EfuseUsed.EnableWindow(true);

			// Read Efuse Utilization
			DLL_ReadEFuseUtilization((LPADAPTER)(Adapter.hFile), (ULONG *)&EfuseUtilize);
			_stprintf(strBuf, TEXT("%d Bytes"), (EfuseUtilize&0xFFFF));        		
			m_Efuse_Utilize.SetWindowText(strBuf);
		
	
	// Read Rf mode to 1T1R to 4T4R
	bResult = ReadRFMode(
			(LPADAPTER)(Adapter.hFile),
			(int *)&RfMode
			);
	if(!bResult)
	    ErrMsgBox(TEXT("Fail to query RF mode !!" ));
	
	if (IS_HARDWARE_SERIES(8814A))
	{
		bResult = ReadValidRFPath(
			(LPADAPTER)(Adapter.hFile),
			&ValidRfPath
			);
		if(!bResult)
		{
	   	 	ErrMsgBox(TEXT("Fail to query valid RF path!!" ));
			ValidRfPath = 0;
		}
		if((ValidRfPath!=0x606&&
		     ValidRfPath!=0x60e&&
		     ValidRfPath!=0x60f&&
		     ValidRfPath!=0xe0e&&
		     ValidRfPath!=0xe0f) 
		     &&IS_HARDWARE_SERIES(8814A))
		{
			//20150128, KenSun
			//If RF path value is abnormal use default setting for 8814A
			ValidRfPath = 0x0e0f;
			RfMode = RF_3T3R;
		}
	}
	
	// Get RFE Type from driver
	QueryRFEType((LPADAPTER)(Adapter.hFile), &RFEType);
	if (RFEType < 64)
	{
		_stprintf(strBuf, TEXT("0x%x"), (RFEType&0xFF));
		m_RFEType.SetWindowText(strBuf);
	}
	
	//
	// Get autoload status from driver and Display on UI
	//
	GetAutoloadStatus((LPADAPTER)(Adapter.hFile), &AutoloadOK);

	if (RfMode == RF_1T1R && AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("1*1_PG"));	
	else if (RfMode == RF_1T2R && AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("1*2_PG"));	
	else if (RfMode == RF_2T2R && AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("2*2_PG"));	
	else if (RfMode == RF_2T2R_GREEN && AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("1*2T_PG"));	
	else if (RfMode == RF_3T3R && AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("3*3_PG"));	
	else if (RfMode == RF_2T3R && AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("2*3_PG"));	
	else if (RfMode == RF_2T4R && AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("2*4_PG"));	
	else if (RfMode == RF_2T2R && !AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("2*2_NPG"));	
	else if (RfMode == RF_2T2R_GREEN && !AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("1*2T_NPG"));
	else if (RfMode == RF_1T1R && !AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("1*1_NPG"));	
	else if (RfMode == RF_1T2R && !AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("1*2_NPG"));	
	else if (RfMode == RF_3T3R && !AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("3*3_NPG"));	
	else if (RfMode == RF_2T3R && !AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("2*3_NPG"));	
	else if (RfMode == RF_2T4R && !AutoloadOK)
		m_RfAutoload.SetWindowText(TEXT("2*4_NPG"));	

	UCHAR	tmpAddr[6];
	if (ReadMACAddress((LPADAPTER)Adapter.hFile, (char *)tmpAddr))
		memcpy(Adapter.CurrentMAC, tmpAddr, 6);
	m_SelfMacAddr.SetWindowText(HexToString(Adapter.CurrentMAC, 6, strBuf, 12, false));
	m_DestMacAddr.SetWindowText("FFFFFFFFFFFF");
}

void CMP819xVCDlg::OnInitItemTest()
{
	m_TestList.AddString("None");
	m_TestList.AddString("Packets Tx");
	m_TestList.AddString("Packets Rx");
	m_TestList.AddString("Continuous Tx");
	m_TestList.AddString("Single Tone Tx");
	m_TestList.AddString("Carrier Suppression Tx");
	
	m_TestList.SetCurSel(0);

	//<20121127> DIG is necessary in Jaguar. We turn ON as default and a button for switching.
	//<20140206> DIG is default OFF now, Asked by James.
	if (IS_11AC_SERIES())
	{
		bDigON = FALSE;
		SetDigOnOff((LPADAPTER)Adapter.hFile, bDigON);
		m_Button_DIG.SetWindowText((bDigON ? TEXT("OFF DIG") : TEXT("ON DIG")));
		m_Result.SetWindowText(TEXT("Packets Rx Start and DIG OFF"));
	}
}

void CMP819xVCDlg::OnInitItemBandWidth()
{
	if (IS_11AC_SERIES())
	{
		if(1 == customBWsetting[1][2].bw_en)
			UpdateBandwidth(MPT_BW_80MHZ, 0);
		else if(1 == customBWsetting[1][1].bw_en)
			UpdateBandwidth(MPT_BW_40MHZ, 0);
		else
			UpdateBandwidth(MPT_BW_20MHZ, 0);
	}
	else
		UpdateBandwidth(MPT_BW_20MHZ, 0);
}


void CMP819xVCDlg::OnInitWirelessMode()
{
	if ( ! IS_11AC_SERIES() ) 
		UpdateModulation(WIRELESS_MODE_N_24G, 0);
	else
		UpdateModulation(WIRELESS_MODE_AC_5G, 0);
	
	BOOL    bResult;
	int 	tempVal;

	m_Modulation.AddString("2.4G");
	m_Modulation.AddString("5G");	

	
	// Query Modulation/wirelessmode.
	bResult = GetModulation(
					(LPADAPTER)Adapter.hFile,
					(int *)&(Adapter.CurrentModulation)
				);
	UpdateModulation(Adapter.CurrentModulation, 0);
	tempVal = IndexValueConvert(ComboModulation, Value2Index, Adapter.CurrentModulation);
	m_Modulation.SetCurSel(tempVal);

}

void CMP819xVCDlg::OnInitItemChannel()
{
	UINT nTotalChnlCnt = 14;
	UINT i;
	TCHAR   strBuf[100];
	
	for(i=0; i<nTotalChnlCnt; i++)
	{
	    _stprintf((TCHAR*)(strBuf), TEXT("%d"), Adapter.Channels[i]);
	    m_Channel.AddString(strBuf);
		m_Channel.SetItemData(i, Adapter.Channels[i]);
	}

	if(Adapter.CurrentModulation == WIRELESS_MODE_N_24G)
	{
		if(Adapter.CurrentBandWidth == MPT_BW_20MHZ)
		{
			Adapter.Channels = Channels_2_4G_20M;
			Adapter.nTotalChnlCnt = ChannelNum_2_4G_20M;
		}
		else if (Adapter.CurrentBandWidth == MPT_BW_40MHZ)
		{
			Adapter.Channels = Channels_2_4G_40M;
			Adapter.nTotalChnlCnt = ChannelNum_2_4G_40M;
		}			
	}
	else if(Adapter.CurrentModulation == WIRELESS_MODE_N_5G || 
		    Adapter.CurrentModulation == WIRELESS_MODE_AC_5G)
	{
		if(Adapter.CurrentBandWidth == MPT_BW_20MHZ)
		{
			Adapter.Channels = Channels_5G_20M;
			Adapter.nTotalChnlCnt = ChannelNum_5G_20M;
		}
		else if (Adapter.CurrentBandWidth == MPT_BW_40MHZ)
		{
			Adapter.Channels = Channels_5G_40M;
			Adapter.nTotalChnlCnt = ChannelNum_5G_40M;		
		}
		else if (Adapter.CurrentBandWidth == MPT_BW_80MHZ)
		{
			Adapter.Channels = Channels_5G_80M;
			Adapter.nTotalChnlCnt = ChannelNum_5G_80M;		
		}
	}					
	else 
	{
		m_Channel_Width.AddString("Unknown Wireless mode");;
	}	
	
	m_Channel.ResetContent();
	for(i = 0; i < Adapter.nTotalChnlCnt; i++)
	{
		if(Adapter.CurrentModulation == WIRELESS_MODE_N_5G || 
			Adapter.CurrentModulation == WIRELESS_MODE_AC_5G)
		{
			if(Adapter.CurrentBandWidth == MPT_BW_20MHZ)
				_stprintf((TCHAR*)(strBuf), TEXT("%d"), Channels_5G_20M[i]);
			else if (Adapter.CurrentBandWidth == MPT_BW_40MHZ)
				_stprintf((TCHAR*)(strBuf), TEXT("%d"), Channels_5G_40M[i]);
			else if (Adapter.CurrentBandWidth == MPT_BW_80MHZ)
				_stprintf((TCHAR*)(strBuf), TEXT("%d"), Channels_5G_80M[i]);
		}		
		else
		{
			if(Adapter.CurrentBandWidth == MPT_BW_20MHZ)
				_stprintf((TCHAR*)(strBuf), TEXT("%d"), Channels_2_4G_20M[i]);
			else if (Adapter.CurrentBandWidth == MPT_BW_40MHZ)
				_stprintf((TCHAR*)(strBuf), TEXT("%d"), Channels_2_4G_40M[i]);
		}
		m_Channel.AddString(strBuf);
		m_Channel.SetItemData(i, Adapter.Channels[i]);
	}

	UpdateChannel(Adapter.Channels[0], 0);	
}

void CMP819xVCDlg::OnInitItemRate()
{
	Adapter.EnableAllRates = GetPrivateProfileInt("USER", "EnableAllRates", 0, CONFIG_CHIP_FILE);
	if(NeedDisabled11ACMode()) bVHTEnabled = FALSE;
	if (Adapter.EnableAllRates) 
	{
		Adapter.nTotalRateCnt = RATES_ALL_NUM;
		for (UINT i = 0; i < Adapter.nTotalRateCnt; ++i) {
			m_Rate.AddString(RatesAll[i].rateStr);
			m_Rate.SetItemData(i, RatesAll[i].rate);
		}
		m_Rate.SetCurSel(IndexValueConvert(ComboDataRate, Value2Index, RATE_MCS0));
		UpdateDataRate(RATE_MCS0, 0);
	} else {
		SwitchRateSet(Adapter.CurrentModulation, Adapter.CurrentBandWidth);
	}
}

void CMP819xVCDlg::OnInitItemRfRegPath()
{
	// RF Reg RfPath
	m_RfRegRfPath.ResetContent();
	if(ValidRfPath!=0)
	{
		BYTE valid_path = 0;
		char rf_path_str[50] = {0};
		for(int i = 0 ; i < 8; i ++)
		{
			valid_path = ((ValidRfPath>>i) & 0x01);//Normally, Rx paths are more than Tx
			if(valid_path)
			{
				if(i == 0 && (IS_HARDWARE_SERIES(8814A)))
					sprintf(rf_path_str,"RfPath_Aux");	
				else
					sprintf(rf_path_str,"RfPath_%d",i);
				m_RfRegRfPath.AddString(rf_path_str);
			}
		}
	}
	else if(RfMode == RF_3T3R && (IS_HARDWARE_SERIES(8814A)))
	{
		m_RfRegRfPath.AddString("RfPath_Aux");
		m_RfRegRfPath.AddString("RfPath_1");
		m_RfRegRfPath.AddString("RfPath_2");
		m_RfRegRfPath.AddString("RfPath_3");
	}
	else if(RfMode == RF_1T2R || RfMode == RF_2T2R || RfMode == RF_2T2R_GREEN)
	{
		m_RfRegRfPath.AddString("RfPath_A");
		m_RfRegRfPath.AddString("RfPath_B");
	}
	else if((RfMode == RF_2T4R||RfMode == RF_2T3R) && (IS_HARDWARE_SERIES(8814A)))
	{
		m_RfRegRfPath.AddString("RfPath_1");
		m_RfRegRfPath.AddString("RfPath_2");
	}
	else if (RfMode == RF_1T1R)
	{
		m_RfRegRfPath.AddString("RfPath_A");
	}
	m_RfRegRfPath.SetCurSel(0);
}
void CMP819xVCDlg::ShowValidTxPath()
{
	BYTE valid_tx = 0;
	char rf_path_str[50] = {0};
	for(int i = 0 ; i < 8; i ++)
	{
		valid_tx = ((ValidRfPath>>(8+i)) & 0x01);
		if(valid_tx)
		{
			if(IS_HARDWARE_SERIES(8814A))
			{
				if(i == 0 )
					sprintf(rf_path_str,"Aux");	
				else
					sprintf(rf_path_str,"%d",i);
			}
			m_AntennaTx.AddString(rf_path_str);
		}
	}
	if(RfMode == RF_3T3R && regAdapter.iMode == CREATOR_MODE)
		m_AntennaTx.AddString("Aux");
}
void CMP819xVCDlg::ShowValidRxPath()
{
	BYTE valid_rx = 0;
	valid_rx = (BYTE)ValidRfPath &0xff;
	 if(RfMode == RF_3T3R || RfMode == RF_2T4R|| RfMode == RF_2T3R)
	 {
	 	switch(valid_rx){
			case 0xf:
				if (IS_HARDWARE_SERIES(8814A))
				{
					m_AntennaRx.AddString("Aux");
					m_AntennaRx.AddString("1");
					m_AntennaRx.AddString("2");
					m_AntennaRx.AddString("3");
					m_AntennaRx.AddString("1,2");
					m_AntennaRx.AddString("2,3");
					m_AntennaRx.AddString("1,2,3");
					m_AntennaRx.AddString("Aux,1,2,3");
				}
				break;
			case 0xe:
				if (IS_HARDWARE_SERIES(8814A))
				{
					m_AntennaRx.AddString("1");
					m_AntennaRx.AddString("2");
					m_AntennaRx.AddString("3");
					m_AntennaRx.AddString("1,2");
					m_AntennaRx.AddString("2,3");
					m_AntennaRx.AddString("1,2,3");
				}
				break;
	 		}
	 }
	 else if(RfMode == RF_2T2R || RfMode == RF_2T2R_GREEN)
	 {
	 	switch(valid_rx){
			case 0x6:
				if (IS_HARDWARE_SERIES(8814A))
				{
					m_AntennaRx.AddString("1");
					m_AntennaRx.AddString("2");
					m_AntennaRx.AddString("1,2");
				}
				break;
	 		}
	 }
}
void CMP819xVCDlg::OnInitItemAntTxRx()
{
	UINT AntennaTxIndex, AntennaRxIndex, AntennaTx, AntennaRx;
	
	// AntennaTx
	m_AntennaTx.ResetContent();
	if(ValidRfPath!=0)
	{
		BYTE tx_path = (BYTE)(ValidRfPath>>8)&0xff;
		ShowValidTxPath();
		switch(tx_path){
			case 0xe:
				if(IS_HARDWARE_SERIES(8814A))
				{
					AntennaTxIndex = 0;
					AntennaTx = ANTENNA_B;
				}
				break;
			case 0x6:
				if(IS_HARDWARE_SERIES(8814A))
				{
					AntennaTxIndex = 0;
					AntennaTx = ANTENNA_B;
				}
				break;
			default:
				break;
			}			
	}
	else if(RfMode == RF_3T3R && (IS_HARDWARE_SERIES(8814A)))
	{
		m_AntennaTx.AddString("Aux");
		m_AntennaTx.AddString("1");
		m_AntennaTx.AddString("2");
		m_AntennaTx.AddString("3");

		AntennaTxIndex = AntennaTx_1_Index_3T3R_Nsts1;
		AntennaTx = ANTENNA_B;
	}
	else if((RfMode == RF_2T2R) || (RfMode == RF_2T2R_GREEN))
	{
		if (bHPConfig == TRUE)
		{
			m_AntennaTx.AddString("1");  // "B"
			m_AntennaTx.AddString("2");  // "A"
			m_AntennaTx.AddString("1,2"); //"AB"
		}
		else
		{
			m_AntennaTx.AddString("A");
			m_AntennaTx.AddString("B");
			m_AntennaTx.AddString("AB");
		}
		AntennaTxIndex = AntennaTx_A_Index_2T2R;
		AntennaTx = ANTENNA_A;
	}
	else if (IS_HARDWARE_SERIES(8723B) || IS_HARDWARE_SERIES(8723D))
	{
		if (bHPConfig == TRUE)
		{
			m_AntennaTx.AddString("1");  // "S0"
			m_AntennaTx.AddString("2");  // "S1"
		}
		else
		{
			m_AntennaTx.AddString("S1");  // Default as Wi-Fi: "S1"
			m_AntennaTx.AddString("S0");  // BT: "S0"
		}
		
		// 8723BU SingleAntenna: Default use "S0", Asked by Rock.
		// Add an registry "AltDefAnt" for alternative default antenna setting.
		if (Adapter.HardwareId == HID_8723BU || bAltDefAnt == TRUE)
		{
			AntennaTxIndex = AntennaTx_S0_Index_2T2R;
			AntennaTx = ANTENNA_B;
		}
		else
		{
			AntennaTxIndex = AntennaTx_S1_Index_2T2R;
			AntennaTx = ANTENNA_A;
		}
		
	}
	else if(RfMode == RF_1T2R)
	{
		m_AntennaTx.AddString("A");	
		m_AntennaTx.AddString("B");
		AntennaTxIndex = AntennaTx_A_Index_1T2R;
		AntennaTx = ANTENNA_A;
		
		m_AntennaTx.EnableWindow(FALSE);
	}
	else if(IS_HARDWARE_SERIES(8703B)) // Unlike 8723B, it only has one antenna actually. 
	{
		m_AntennaTx.AddString("A");
		AntennaTxIndex = AntennaTx_A_Index_1T1R;
		AntennaTx = ANTENNA_A;
		
		m_AntennaTx.EnableWindow(FALSE);
	}
	else if(RfMode == RF_1T1R)
	{
		if (bHPConfig == TRUE)
			m_AntennaTx.AddString("1");
		else
			m_AntennaTx.AddString("A");

		AntennaTxIndex = AntennaTx_A_Index_1T1R;
		AntennaTx = ANTENNA_A;
		
		m_AntennaTx.EnableWindow(FALSE);
	}
	
	m_AntennaTx.SetCurSel(AntennaTxIndex);
	
	// AntennaRx
	m_AntennaRx.ResetContent();
	if(ValidRfPath!=0)
	{
		BYTE rx_path = (BYTE)ValidRfPath &0xff;
		ShowValidRxPath();
		switch(rx_path){
			case 0xf:
				if(IS_HARDWARE_SERIES(8814A))
				{
					AntennaRxIndex = AntennaRx_Aux123_Index_3T3R;
					AntennaRx = ANTENNA_ABCD;;
				}
				break;
			case 0xe:
				if(IS_HARDWARE_SERIES(8814A))
				{
					AntennaRxIndex = AntennaRx_123_Index_2T3R;
					AntennaRx = ANTENNA_BCD;
				}
				break;
			case 0x6:
				if(IS_HARDWARE_SERIES(8814A))
				{
					AntennaRxIndex = AntennaRx_12_Index_2T2R;
					AntennaRx = ANTENNA_BC;
				}
				break;
			default:
				break;
			}		
	}
	else if(RfMode == RF_3T3R && (IS_HARDWARE_SERIES(8814A)))
	{
		m_AntennaRx.AddString("Aux");
		m_AntennaRx.AddString("1");
		m_AntennaRx.AddString("2");
		m_AntennaRx.AddString("3");
		m_AntennaRx.AddString("1,2");
		m_AntennaRx.AddString("2,3");
		m_AntennaRx.AddString("1,2,3");
		m_AntennaRx.AddString("Aux,1,2,3");
		AntennaRxIndex = AntennaRx_1_Index_3T3R;
		AntennaRx = ANTENNA_B;
	}
	else if(RfMode == RF_2T2R || RfMode == RF_2T2R_GREEN)
	{
		if (bHPConfig == TRUE)
		{
			m_AntennaRx.AddString("1");  // "B"
			m_AntennaRx.AddString("2");  // "A"
			m_AntennaRx.AddString("1,2"); //"AB"
		}
		else
		{
			m_AntennaRx.AddString("A");
			m_AntennaRx.AddString("B");
			m_AntennaRx.AddString("AB");
		}
		AntennaRxIndex = AntennaRx_AB_Index_2T2R;
		AntennaRx = ANTENNA_AB;
	}
	else if(RfMode == RF_1T2R)
	{
		m_AntennaRx.AddString("A");
		m_AntennaRx.AddString("B");
		m_AntennaRx.AddString("AB");
		AntennaRxIndex = AntennaRx_AB_Index_1T2R;
		AntennaRx = ANTENNA_AB;
	}
	else if(RfMode == RF_1T1R)
	{
		if (IS_HARDWARE_SERIES(8723B) || IS_HARDWARE_SERIES(8723D))
		{

			if (bHPConfig == TRUE)
			{
				m_AntennaRx.AddString("1");  // "S0"
				m_AntennaRx.AddString("2");  // "S1"
			}
			else
			{
				m_AntennaRx.AddString("S1");  // Default as Wi-Fi: "S1"
				m_AntennaRx.AddString("S0");  // BT: "S0"
			}

			// 8723BU SingleAntenna: Default use "S0", Asked by Rock.
			// Add an registry "AltDefAnt" for alternative default antenna setting.
			if (Adapter.HardwareId == HID_8723BU || bAltDefAnt == TRUE)
			{
				AntennaRxIndex = AntennaRx_S0_Index_2T2R;
				AntennaRx = ANTENNA_B;
			}
			else
			{
				AntennaRxIndex = AntennaRx_S1_Index_2T2R;
				AntennaRx = ANTENNA_A;
			}
		}
		else if(IS_HARDWARE_SERIES(8703B)) // Unlike 8723B, it only has one antenna actually. 
		{
			m_AntennaRx.AddString("A");
			AntennaRxIndex = AntennaRx_A_Index_1T1R;
			AntennaRx = ANTENNA_A;
		}
		else
		{
			if (bHPConfig == TRUE)
				m_AntennaRx.AddString("1");
			else
				m_AntennaRx.AddString("A");

			AntennaRxIndex = AntennaRx_A_Index_1T1R;
			AntennaRx = ANTENNA_A;
		}
		
		m_AntennaRx.EnableWindow(FALSE);
	}
	m_AntennaRx.SetCurSel(AntennaRxIndex);

	//Set Tx Power Combo-Box Window Enable/Disable
	SetTxPowerComboBox(AntennaTx, TRUE);
	//Really update Tx/Rx antenna path setting.
	UpdateAntennaTxRx(AntennaTx, AntennaRx, 0);
}

void CMP819xVCDlg::OnInitItemRFPathSetting()
{
	//Add Antenna Setting for New IC
	if(IS_HARDWARE_SERIES(8821C) || IS_HARDWARE_8731AU)
	{
		// Query Default RF Path Setting From Driver (For 2.4G Case)
		QueryRFPathSetting(
			(LPADAPTER)Adapter.hFile,
			(ULONG *)&(Adapter.DefaultRFPathSetting)
			);
		// Exception Handling
		if (Adapter.DefaultRFPathSetting >= RFPATH_5G_WLA)
			Adapter.DefaultRFPathSetting = RFPATH_2G_BTG;

		// Reset ComboBox Content
		m_RFPathSetting.ResetContent();
		m_RFPathSetting.AddString("BTG"); //index 0
		m_RFPathSetting.AddString("WLG"); //index 1
		m_RFPathSetting.AddString("WLA"); //index 2
		m_RFPathSetting.AddString("Bluetooth"); //index 3

		// Reset Duplicate ComboBox Content
		m_RFPathSetting_Duplicate.ResetContent();
		m_RFPathSetting_Duplicate.AddString("Default"); //index 0
		m_RFPathSetting_Duplicate.AddString("Bluetooth"); //index 1

		if (Adapter.CurrentModulation == WIRELESS_MODE_N_24G)
		{
			UpdateRFPathSetting(Adapter.DefaultRFPathSetting, 0);
			//m_RFPathSetting.EnableWindow(TRUE);
		}
		else
		{
			UpdateRFPathSetting(RFPATH_5G_WLA, 0);
			//m_RFPathSetting.EnableWindow(FALSE);
		}
	}
}

void CMP819xVCDlg::OnInitItemPreambleGuardInterval()
{
	// Preamble, Guard Interval
	m_Preamble.ResetContent();
	m_Preamble.AddString(TEXT("Long Preamble"));
	m_Preamble.AddString(TEXT("Short Preamble"));
	m_Preamble.AddString(TEXT("Long GI"));
	m_Preamble.AddString(TEXT("Short GI"));
	UpdatePreamble(Long_GI, 0);
}

void CMP819xVCDlg::OnInitItemTxPktSetting()
{
	int i;
	TCHAR   strBuf[100];
	
	// Packet Length
	m_PacketLength.SetWindowText("1000");
	Adapter.PacketLength = 1000;

	// Packet Pattern
	for (i=0; i<16; i++)
	{
	    _stprintf( strBuf, TEXT("0x%02hX"), Patterns[i] );
	    m_Pattern.AddString(strBuf);
	}
	m_Pattern.AddString(TEXT("Random"));
	Adapter.PacketPattern = Patterns[16];
	m_Pattern.SetCurSel(16);

	// PMAC Settings (since 8814A)
	m_TxPMAC_TypeSel.AddString(TEXT("Normal"));
	m_TxPMAC_TypeSel.AddString(TEXT("NDP"));
	m_TxPMAC_TypeSel.SetCurSel(0);
	if (IS_HARDWARE_SERIES(8821C) || IS_HARDWARE_8731AU)
	{
		m_TxPMAC_Period.SetWindowText("2000");
		Adapter.PacketPeriod = 2000;	
	}
	else
	{
		m_TxPMAC_Period.SetWindowText("100");
		Adapter.PacketPeriod = 100;
	}
}

void CMP819xVCDlg::OnInitTestResult()
{
	BOOL    bResult;
	TCHAR   strBuf[100];
	UINT    TxOK;
    int		RxOK, RxCRCErr;

	MPStartTest((LPADAPTER)(Adapter.hFile), FALSE, TestItem);
	// Tx OK
	bResult = ResetTxPacketSent(
	            (LPADAPTER) Adapter.hFile
	            );
	bResult = ResetRxPacketReceived(
	            (LPADAPTER) Adapter.hFile
	            );
	
	bResult = QueryTxPacketSent(
	            (LPADAPTER) Adapter.hFile,
	            (int*)&TxOK
	            );
	if(!bResult)
	{
	    ErrMsgBox(TEXT("Fail to Query Tx Packet Sent !!" ));
	}
	_stprintf(strBuf, TEXT("%d"), TxOK);
	m_XmitOK.SetWindowText(strBuf);
	
	
	// Rx Packet received
	bResult = QueryRxPacketReceived(
	            (LPADAPTER) Adapter.hFile,
	            &RxOK
	            );
	if(!bResult)
	{
	    ErrMsgBox(TEXT("Fail to Query Rx Packet Received !!" ));
	}
	_stprintf(strBuf, TEXT("%d"), RxOK);
	
	m_RcvOK.SetWindowText(strBuf);
	
	// Rx Packet CRC32 Error
	bResult = QueryRxPacketCRC32Error(
	            (LPADAPTER) Adapter.hFile,
	            &RxCRCErr
	            );
	if(!bResult)
	{
	    ErrMsgBox(TEXT("Fail to Query Rx Packet CRC32 Error !!" ));
	}
	
	_stprintf(strBuf, TEXT("%d"), RxCRCErr);
	m_RcvErr.SetWindowText(strBuf);
}

void CMP819xVCDlg::OnInitItemAutoTest()
{
	// Auto function
	m_FileIndex.ResetContent();
	m_FileIndex.AddString("None");
	m_FileIndex.AddString("Auto test (Tx site)");
	m_FileIndex.AddString("Auto test (Rx site)");
	//m_FileIndex.AddString("Batch config register");
	m_FileIndex.AddString("Stop testing.");
	m_FileIndex.SetCurSel(0);
}

void CMP819xVCDlg::OnInitRfPathSwitch()
{
	BOOL    bResult;
	CString Aux = "To Aux";
	CString Main = "To Main";
	
	if(IS_HARDWARE_SERIES(8192E) || IS_HARDWARE_SERIES(8812A) || IS_HARDWARE_SERIES(8703B) ||
		IS_HARDWARE_SERIES(8821A) || IS_HARDWARE_SERIES(8822B) || IS_HARDWARE_SERIES(8723D) ||
		IS_HARDWARE_SERIES(8192F)){
		Aux = "To BT";
		Main = "To WIFI";
	}
	else if (IS_HARDWARE_SERIES(8821C) || IS_HARDWARE_8731AU)
	{ // Show current Ant Num for 8821C
		Aux = "Ant_1";
		Main = "Ant_2";
	}
	
	if ( ! regAdapter.bIsComboCard) {
		Aux = "To Aux";
		Main = "To Main";
	}
	

	bResult = GetRfPathSwitch(
	            (LPADAPTER) Adapter.hFile,
	            &bMain
	            );
	if(!bResult)
	{
	    ErrMsgBox(TEXT("Fail to Query Rf Path setting!!" ));
		bMain = TRUE;
	}	

	if(bMain)
		m_RfPathSwitch.SetWindowText(Aux);
	else
		m_RfPathSwitch.SetWindowText(Main);

	// Get RF Path Switch Mapping from Driver
	if (IS_HARDWARE_SERIES(8821C) || IS_HARDWARE_8731AU)
	{
		bResult = QueryRFPathSwitchMap(
	            (LPADAPTER) Adapter.hFile,
	            &RfPathSwitchMap
	            );
		if(!bResult)
		{
			ErrMsgBox(TEXT("Fail to Query Rf Path Swtich Mapping!!" ));
			RfPathSwitchMap = BOTH_AVAILABLE;
		}

		if (RfPathSwitchMap == DONT_CARE)
		{
			m_RfPathSwitch.SetWindowText("N/A");
			return;
		}
		else if (RfPathSwitchMap == ONLY_ANT1)
		{
			m_RfPathSwitch.SetWindowText("Ant_1");
			return;
		}
		else if (RfPathSwitchMap == ONLY_ANT2)
		{
			m_RfPathSwitch.SetWindowText("Ant_2");
			return;
		}
	}
}

void CMP819xVCDlg::OnInitTxPwrTrack()
{
	BOOL    bResult;
	
	bResult = DLL_QueryTxPwrTrack(
	            (LPADAPTER) Adapter.hFile,
	            &TxPwrTrackSwitch
	            );
	if(!bResult)
	{
	    ErrMsgBox(TEXT("Fail to Query TxPwrTrack setting!!" ));
		TxPwrTrackSwitch = FALSE;
	}	

	//Stop TxPowerTrack When Start UI
	//Ohterwise PowerIndex Combo-box will be Disable as default, user may be confused.
	if(FixIdxBaseWhenPwrTrack && TxPwrTrackSwitch)
	{
		TxPwrTrackSwitch = FALSE;
		m_TxPowerTrack.SetWindowText(TEXT("TxPwrTrack Start"));
		DLL_SwitchTxPwrTrack((LPADAPTER)(Adapter.hFile), 0);
		return;
	}

	if(TxPwrTrackSwitch)
	{
		//ErrMsgBox(TEXT("TxPwrTrack Stop !!" ));
		m_TxPowerTrack.SetWindowText(TEXT("TxPwrTrack Stop"));
	}
	else
	{
		//ErrMsgBox(TEXT("TxPwrTrack Start !!" ));
		m_TxPowerTrack.SetWindowText(TEXT("TxPwrTrack Start"));
	}
}

void CMP819xVCDlg::OnInitPwrIndexBase()
{
	m_CheckBox_PwrIndexFromEfuse.EnableWindow(AutoloadOK && !TxPwrTrackSwitch);
}

void CMP819xVCDlg::OnInitXtalCal()
{
	// CrystalCap
	UINT i;
	TCHAR   strBuf[100];
	UINT		bound = 0x3F;
	
	m_Xtal_XinXout.ResetContent();
	for(i=0; i<=bound; i++)
	{
		 _stprintf( strBuf, TEXT("0x%02hX"), i );	
//		sprintf(strBuf, "%d", i);
		m_Xtal_XinXout.AddString(strBuf);
	}

	m_Xtal_XinXout.SetCurSel(32);
	
	if(GetCrystalCap((LPADAPTER) Adapter.hFile, (int *)&Adapter.CurrentCrystalCap))
		UpdateCrystalCap(Adapter.CurrentCrystalCap, 1);
	else
		UpdateCrystalCap(0x20, 1);

}

void CMP819xVCDlg::OnInitPmacMode()
{
	bool bPmacModeON = FALSE;
	if( GetPrivateProfileInt("USER", "isPmacMode", 0, CONFIG_CHIP_FILE))
		bPmacModeON = TRUE;

	if (bPmacModeON)
		m_TxPMAC_Switch.SetWindowText("HW Tx Packet Setting");
	else
		m_TxPMAC_Switch.SetWindowText("SW Tx Packet Setting");

	// Reset Length
	m_PacketLength.SetWindowText("1000");
	Adapter.PacketLength = 1000;

	ShowTxPMAC(bPmacModeON);
}

// CustomizedTables
void CMP819xVCDlg::OnInitCustomerPwr()
{
	CMP819xVCDlg::m_CustomerSelPwr.ResetContent();
	CMP819xVCDlg::m_CustomerSelPwr.AddString("Power by rate file: Default");
	CMP819xVCDlg::m_CustomerSelPwr.AddString("...Select other file...");
	CMP819xVCDlg::m_CustomerSelPwr.SetCurSel(0);
}
void CMP819xVCDlg::OnInitCustomerPwrLmt()
{
	CMP819xVCDlg::m_CustomerSelPwrLmt.ResetContent();
	CMP819xVCDlg::m_CustomerSelPwrLmt.AddString("Power limit file: Default");
	CMP819xVCDlg::m_CustomerSelPwrLmt.AddString("...Select other file...");
	CMP819xVCDlg::m_CustomerSelPwrLmt.SetCurSel(0);
}
UINT CMP819xVCDlg::OnInitCheckCustomPowerTables()
{
	TCHAR 			szBuf[MAX_PATH];
	TCHAR			SystemDir[MAX_PATH];
	CString			targetFile1;
	CString			targetFile2;
	TCHAR			Buf1[MAX_PATH];
	TCHAR			Buf2[MAX_PATH];
	
	HKEY 			hKey;
	DWORD			keyStatus	= FALSE;
	UINT			initStatus	= FALSE;
	int				regStatus1	= FALSE;
	int				regStatus2	= FALSE;
	UINT			loadStatus	= FALSE;
	UINT			defaultFail1	= FALSE;
	UINT			defaultFail2	= FALSE;
	DWORD 			dwType, cbBuf;
	BOOLEAN			valid		= FALSE;
	

	CString Str1, Str2, Str3;

	GetSystemDirectory(SystemDir,MAX_PATH);

	Str1.Format("\\%04d",Adapter.enumId);
	Str2.Format("%s%s","SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}",
				Str1);
	strcpy(Buf1, (LPCTSTR)Str2);
	keyStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
							TEXT(Buf1),
							0,KEY_READ|KEY_WRITE,&hKey);
	if (keyStatus == ERROR_SUCCESS) {
		cbBuf = (MAX_PATH - 1) * sizeof(TCHAR);
		if(RegQueryValueEx(hKey, TEXT("EnableTxPowerLimit"), NULL, &dwType, (BYTE*)szBuf, &cbBuf) == ERROR_SUCCESS){
			int bEnable = 0;
			sscanf(szBuf,"%d",&bEnable);
			if(bEnable)
				valid = TRUE;
		}
		strcpy(Buf1,"");
		cbBuf = (MAX_PATH - 1) * sizeof(TCHAR);
		if(RegQueryValueEx(hKey, TEXT("PwrByRateFile"), NULL, &dwType, (BYTE*)Buf1, &cbBuf) == ERROR_SUCCESS){
			Str1.Format("%s",Buf1);
			if(Str1 != ""){
				
				LoadPwrTableStatus(&regStatus1);
				if(regStatus1){
					CMP819xVCDlg::m_CustomerSelPwr.AddString(Buf1);
					CMP819xVCDlg::m_CustomerSelPwr.SetCurSel(2);
					targetFile1.Format("%s\\drivers\\%s",SystemDir,Buf1);
					strcpy(CS_PWR_FILE, (LPCTSTR)targetFile1);
					
				}else{
					;
				}
			 
			}else{
				LoadPwrTableStatus(&regStatus1);
				if(!regStatus1){
					defaultFail1 = TRUE;
				}
				regStatus1=TRUE;
				
			}
		}else{
			regStatus1=TRUE;
		}
		strcpy(Buf2,"");
		if(valid){
			cbBuf = (MAX_PATH - 1) * sizeof(TCHAR);
			if(RegQueryValueEx(hKey, TEXT("PwrLimitFile"), NULL, &dwType, (BYTE*)Buf2, &cbBuf) == ERROR_SUCCESS){
				Str2.Format("%s",Buf2);
				if(Str2 != ""){
					
					LoadPwrLmtTableStatus(&regStatus2);
					if(regStatus2){
						CMP819xVCDlg::m_CustomerSelPwrLmt.AddString(Buf2);
						CMP819xVCDlg::m_CustomerSelPwrLmt.SetCurSel(2);
						targetFile2.Format("%s\\drivers\\%s",SystemDir,Buf2);
						strcpy(CS_PWR_LMT_FILE, (LPCTSTR)targetFile2);
						
					}else{
						;
					}

				}else{
					LoadPwrLmtTableStatus(&regStatus2);
					if(!regStatus2){
						defaultFail2 = TRUE;
					}
					regStatus2=TRUE;

				}
			}else{
				regStatus2=TRUE;
			}
		
		}else{
			regStatus2=TRUE;
		}
	}
	RegCloseKey(hKey);
	
	CMP819xVCDlg::m_CheckBox_EnableTxPowerLimit.SetCheck(valid);
	
	if(!regStatus1||!regStatus2){
		CustomPowerTableStatus = 1;
		Str3.Format("Due to content format error or file not available,\n");
		Str1.Format("");
		if(!regStatus1)
			Str1.Format("[%s] not loaded, using default power by rate table,\n",Buf1);
		Str2.Format("%s%s",Str3,Str1);
		Str1.Format("");
		if(!regStatus2 && !valid)
			Str1.Format("**EnableTxPowerLimit is [OFF] so that\n [%s] not loaded, using default power limit table,\n",Buf2);
		else if(!regStatus2)
			Str1.Format("[%s] not loaded, using default power limit table,\n",Buf2);
		Str3.Format("%s%s\n**make sure you have already re-enable the NIC to load new settings.",Str2,Str1);
		strcpy(szPopMsg, (LPCTSTR)Str3);
	}
	if(defaultFail1||defaultFail2){
		CustomPowerTableStatus = 2;
		Str3.Format("[ Important ] Detect changes made.\n**Please re-enable the NIC to load new settings,\n");
		Str2.Format("%s otherwise settings may not be loaded properly. \n\n**Wish to continue using MP tool anyway, press [NO].",Str3);
		strcpy(szPopMsg, (LPCTSTR)Str2);
	}
	
	return initStatus;
}

#pragma endregion Initialization

#pragma region [TestCase]

void CMP819xVCDlg::
StartTesting_PacketsTx(
    IN  BOOL    bStart
    )
{
    BOOL    bResult, bEnable;
    DWORD   XthreadID, dwResult;

    if(bStart)
    {
        bResult = ResetTxPacketSent((LPADAPTER) Adapter.hFile);
        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Reset Tx Packet Sent FAIL"));
            ErrMsgBox(TEXT("Fail to Reset Tx Packet Sent !!" ));
        }

        bResult = SetChannelDirectCall(
                    (LPADAPTER) Adapter.hFile,
                    Adapter.CurrentChannel
                    );
        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Set Channel FAIL"));
            ErrMsgBox(TEXT("Fail to Set Channel !!" ));
        }

        bResult = SetDataRate(
                    (LPADAPTER) Adapter.hFile,
                    Adapter.CurrentRate
                    );
        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Set Data Rate FAIL"));
            ErrMsgBox(TEXT("Fail to Set Data Rate !!" ));
        }
	
        bResult = SetTxPowerControl(
                    (LPADAPTER) Adapter.hFile,
                    Adapter.CurrentTxPower
                    );
        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Set Tx Power FAIL"));
            ErrMsgBox(TEXT("Fail to Set Tx Power !!" ));
        }

		while(G_xhThrd != INVALID_HANDLE_VALUE)
		{
			uistatistics.ctr_G_xhThrd_PacketsTx++;
            dwResult = WaitForSingleObject(G_xhThrd, 100);

            if(dwResult == WAIT_OBJECT_0)
            {
            	G_xhThrd = INVALID_HANDLE_VALUE;
            	uistatistics.ctr_threadFreeOk_PacketsTx++;
            }	
		}
        G_xhThrd=CreateThread(
                    NULL,
                    0,
                    ThreadXmit,
                    m_hWnd,
                    0,
                    &XthreadID
                    );

        m_Result.SetWindowText(TEXT("Packets Tx Start"));
    }
    else
    {
        m_Result.SetWindowText(TEXT("Packets Tx Stop"));
    }
	
    bEnable = !bStart;
    
    ItemEnDisable(bEnable, 0);
}

void CMP819xVCDlg::
StartTesting_PacketsRx(
    IN  BOOL    bStart
    )
{
    BOOL    bResult, bEnable;
    int		RxOK, RxCRCErr;//, PER;
    TCHAR   strBuf[20];

    if(bStart)
    {
		if(IS_HARDWARE_SERIES(8703B) || IS_HARDWARE_SERIES(8723B))
		{
			bResult = SetChannelDirectCall(
						(LPADAPTER) Adapter.hFile,
						Adapter.CurrentChannel
						);
			if(!bResult)
			{
				m_Result.SetWindowText(TEXT("Set Channel FAIL"));
				ErrMsgBox(TEXT("Fail to Set Channel !!" ));
			}
		}

    	ResetRxPacketReceived((LPADAPTER)Adapter.hFile);
        SetTimer(UPDATE_RX_STAT_TIMER, 100, NULL);
		
		m_Result.SetWindowText(TEXT("Packets Rx Start"));
    }
    else
    {
        AutoTestRxOn = FALSE;
        //
        // Kill the timer, UPDATE_RX_STAT_TIMER.
        //
        KillTimer(UPDATE_RX_STAT_TIMER);

        bResult = QueryRxPacketReceived((LPADAPTER)Adapter.hFile, &RxOK);

        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Query Rx Packet Received FAIL"));
            ErrMsgBox(TEXT("Fail to Query Rx Packet Received !!" ));
        }

        _stprintf(strBuf, TEXT("%d"), RxOK);
        m_RcvOK.SetWindowText(strBuf);

        bResult = QueryRxPacketCRC32Error((LPADAPTER)Adapter.hFile, &RxCRCErr);

        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Query Rx Packet CRC32 Error FAIL"));
            ErrMsgBox(TEXT("Fail to Query Rx Packet CRC32 Error !!" ));
        }

        _stprintf(strBuf, TEXT("%d"), RxCRCErr);
        m_RcvErr.SetWindowText(strBuf);

        //if (RxOK > 0)
        //{
        //    PER = 10000*RxCRCErr/(RxOK+RxCRCErr);
        //    _stprintf(strBuf, TEXT("%d"), PER);
        //    m_CisData.SetWindowText(strBuf);
        //}

        m_Result.SetWindowText(TEXT("Packets Rx Stop"));
    }

    bEnable = !bStart;
    
    ItemEnDisable(bEnable, 0);
}


void CMP819xVCDlg::
StartTesting_PMacPacketTx(
    IN  BOOL		bStart
    )
{
    BOOL    bResult, bEnable;

	if(bStart)
	{
		ResetRxPacketReceived((LPADAPTER)Adapter.hFile);
		ResetTxPacketSent((LPADAPTER) Adapter.hFile);

		PMAC_Enter(&(Adapter.PMacTxInfo));	
		bResult = SetPMacPacketTx(
					(LPADAPTER) Adapter.hFile,
					(PVOID)(&(Adapter.PMacTxInfo))
					);
		if(!bResult)
		{
			m_Result.SetWindowText(TEXT("Set PMAC Packet Tx  FAIL"));
			ErrMsgBox(TEXT("Fail to Set PMAC Packet Tx!!" ));
		}

		m_Result.SetWindowText(TEXT("PMAC Packet TX Start"));
		SetTimer(UPDATE_TX_STAT_TIMER, 100, NULL);
	}
	else
	{
		PMAC_Leave(&(Adapter.PMacTxInfo));
		bResult = SetPMacPacketTx(
						(LPADAPTER) Adapter.hFile,
						&(Adapter.PMacTxInfo)
					);
		if(!bResult)
		{
			m_Result.SetWindowText(TEXT("Set PMAC Packet Tx FAIL"));
			ErrMsgBox(TEXT("Fail to Set PMAC Packet Tx!!" ));
		}

		m_Result.SetWindowText(TEXT("PMAC Packet TX Stop"));
		KillTimer(UPDATE_TX_STAT_TIMER);
	}

	bEnable = !bStart;
	ItemEnDisable(bEnable, 0);
}


void
CMP819xVCDlg::
StartTesting_WPSButtonPush(
    IN  BOOL    bStart
    )
{
    BOOL    bEnable;

    if(bStart)
    {
        //SetTimer(UPDATE_WPSButtonPush_TIMER, 500, NULL);
        m_Result.SetWindowText(TEXT("WPS Button Polling Start"));
    }
    else
    {
        //
        // Kill the timer, UPDATE_WPSButtonPush_TIMER.
        //
        KillTimer(UPDATE_WPSButtonPush_TIMER);
        m_Result.SetWindowText(TEXT("WPS Button Polling Stop"));
    }

    bEnable = !bStart;
    
    ItemEnDisable(bEnable, 0);
}


void CMP819xVCDlg::
StartTesting_HWRfOnOff(
    IN  BOOL    bStart
    )
{
    BOOL    bEnable;

    if(bStart)
    {
        //SetTimer(UPDATE_HWRfOnOff_TIMER, 500, NULL);
        m_Result.SetWindowText(TEXT("HW RF Status Polling Start"));
    }
    else
    {
        //
        // Kill the timer, UPDATE_HWRfOnOff_TIMER.
        //
        KillTimer(UPDATE_HWRfOnOff_TIMER);
        m_Result.SetWindowText(TEXT("HW RF Status Polling Stop"));
    }

    bEnable = !bStart;
    
    ItemEnDisable(bEnable, 0);
}


void CMP819xVCDlg::
StartTesting_ContinuousTx(
    IN  BOOL    bStart
    )
{
    BOOL    bResult, bEnable;

    if(bStart)
    {
        //
        // We transmit a packet FIRST whatever modulations (B or G).
        // Added by Roger. 2006.12.20.
        //
        bResult = ResetTxPacketSent((LPADAPTER) Adapter.hFile);
		
        bResult =SetTxContinuousDirectCall(
                    (LPADAPTER) Adapter.hFile,
                    START
                    );
        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Set Tx Continuous FAIL"));
            ErrMsgBox(TEXT("Fail to Set Tx Continuous !!" ));
        }
		
		XmitOnePacket();
        m_Result.SetWindowText(TEXT("ContinuousTx Start"));
    }
    else
    {
        bResult = SetTxContinuousDirectCall(
                    (LPADAPTER) Adapter.hFile,
                    STOP
                    );
        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Set Tx Continuous FAIL"));
            ErrMsgBox(TEXT("Fail to Set Tx Continuous !!" ));
        }

        m_Result.SetWindowText(TEXT("ContinuousTx Stop"));
    }
    
    bEnable = !bStart;
    
    ItemEnDisable(bEnable, 0);
}

void CMP819xVCDlg::
StartTesting_SingleCarrierTx(
    IN  BOOL    bStart
    )
{
    BOOL    bResult, bEnable;
    UINT	tempIndex;

    if(bStart)
    {
        bResult = ResetTxPacketSent((LPADAPTER) Adapter.hFile);
        
        if( ! IS_OFDM_RATE(Adapter.CurrentRate) )
        {	/* switch the rate to OFDM rate */
        	tempIndex = IndexValueConvert(ComboDataRate, Value2Index, RATE_OFDM_54M);
            m_Rate.SetCurSel(tempIndex);
            OnSelchangeRate();
        }

        bResult = SetSingleCarrierTxContinuous(
                        (LPADAPTER) Adapter.hFile,
                        START
                        );
        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Set Single Carrier Tx Continuous FAIL"));
            ErrMsgBox(TEXT("Fail to Set Single Carrier Tx Continuous !!" ));
        }
		
        XmitOnePacket();

        m_Result.SetWindowText(TEXT("Single Carrier Tx Start"));
    }
    else
    {
        bResult = SetSingleCarrierTxContinuous(
                    (LPADAPTER) Adapter.hFile,
                    STOP
                    );
        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Set Single Carrier Tx Continuous FAIL"));
            ErrMsgBox(TEXT("Fail to Set Single Carrier Tx Continuous !!" ));
        }

        m_Result.SetWindowText(TEXT("Single Carrier Tx Stop"));
    }

    bEnable = !bStart;
    
    ItemEnDisable(bEnable, 0);
}

void CMP819xVCDlg::
StartTesting_SingleToneTx(
    IN  BOOL    bStart
    )
{
    BOOL    bResult, bEnable;
    UINT	tempIndex;

	if(bStart)
	{
		bResult = ResetTxPacketSent((LPADAPTER) Adapter.hFile);

		// According to RF Ed,
		// All IC later than 8188E(included) do not have this limitation anymore
# if 0

		// <20121130, Kordan> Jaguar ICs should not be limited by OFDM rates to support 5G 80M.
		if( ! IS_OFDM_RATE(Adapter.CurrentRate) && 
			! ( IS_11AC_SERIES() || IS_HARDWARE_SERIES(8723B) || IS_HARDWARE_SERIES(8703B) || IS_HARDWARE_SERIES(8192E) || IS_HARDWARE_SERIES(8723D) || IS_HARDWARE_SERIES(8710B) || IS_HARDWARE_SERIES(8192F))
		)
		{	/* switch the rate to OFDM rate */
			tempIndex = IndexValueConvert(ComboDataRate, Value2Index, RATE_OFDM_54M);
			m_Rate.SetCurSel(tempIndex);
			OnSelchangeRate();
		}
# endif
		bResult = SetSingleToneTx(
			(LPADAPTER) Adapter.hFile,
			START
			);
		if(!bResult)
		{
			m_Result.SetWindowText(TEXT("Set Single Tone Tx FAIL"));
			ErrMsgBox(TEXT("Fail to Set Single Tone Tx !!" ));
		}

#if 0
		// <20121130, Kordan> Jaguar ICs use RF LO only, no packet needed.
		if (! (IS_11AC_SERIES() || IS_HARDWARE_SERIES(8723B) || IS_HARDWARE_SERIES(8703B) || IS_HARDWARE_SERIES(8192E) || IS_HARDWARE_SERIES(8723D) || IS_HARDWARE_SERIES(8710B) || IS_HARDWARE_SERIES(8192F)))
			XmitOnePacket();
#endif	
		m_Result.SetWindowText(TEXT("Single Tone Tx Start"));
	}
	else
	{
		bResult =	SetSingleToneTx(
					(LPADAPTER) Adapter.hFile,
					STOP
					);
		if(!bResult)
		{
			m_Result.SetWindowText(TEXT("Set Single Tone Tx FAIL"));
			ErrMsgBox(TEXT("Fail to Set Single Tone Tx !!" ));
		}

	}
	
	bEnable = !bStart;
	ItemEnDisable(bEnable, 0);
}


void CMP819xVCDlg::
StartTesting_CarrierSuppressionTx(
    IN  BOOL    bStart
    )
{

    BOOL    bResult, bEnable;
    UINT	tempIndex;

    if(bStart)
    {
		if(! IS_CCK_RATE(Adapter.CurrentRate))
        {	/* switch the rate to CCK rate */
        	tempIndex = IndexValueConvert(ComboDataRate, Value2Index, RATE_CCK_11M);
            m_Rate.SetCurSel(tempIndex);
            OnSelchangeRate();
        }

        bResult = ResetTxPacketSent((LPADAPTER) Adapter.hFile);
        bResult = SetCarrierSuppressionTxContinuous(
                    (LPADAPTER) Adapter.hFile,
                    START
                    );
        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Set Carrier Suppression Tx FAIL"));
            ErrMsgBox(TEXT("Fail to Set Carrier Suppression Tx !!" ));
        }
		
		XmitOnePacket();
        m_Result.SetWindowText(TEXT("Carrier Suppression Tx Start"));
    }
    else
    {
        bResult = SetCarrierSuppressionTxContinuous(
                    (LPADAPTER) Adapter.hFile,
                    STOP
                    );
        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Set Carrier Suppression Tx FAIL"));
            ErrMsgBox(TEXT("Fail to Set Carrier Suppression Tx !!" ));
        }
        m_Result.SetWindowText(TEXT("Carrier Suppression Tx Stop"));
    }

    bEnable = !bStart;
    
    ItemEnDisable(bEnable, 0);
}
#pragma endregion TestCase

#pragma region [Events]

#pragma region [Event]-Timer
void CMP819xVCDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
    {
	    case UPDATE_RX_STAT_TIMER:
	        UpdateRxStat();
	        break;
		case UPDATE_TX_STAT_TIMER:
			UpdateTxStat();
			break;
		case UPDATE_ACK_COUNTER_TIMER:
			UpdateAckCounter();
			break;
	    case AutoTestProcessTx_TIMER:
	        AutoTestProcessTx();
	        break;
	    case UPDATE_WPSButtonPush_TIMER:
			WPSButtonPushPolling();
			break;
	    case UPDATE_HWRfOnOff_TIMER:
			HWRfStatusPolling();
			break;		
		case DUT_MONITOR:
			//funcTest();
			break;
		case IO_MONITOR:
			IoMonitor();
			break;
		case DIALOG_MONITOR:
			checkPowerTableStatus();
			break;
		case RSSI_MONITOR:
			RSSIMonitor();
			break;	

	    default:
	        break;
    }

    CDialog::OnTimer(nIDEvent);
}

// Related Functions
void CMP819xVCDlg::WriteToRxStatisticsFile(IN PVOID pRxStaStr)
{
    FILE *PFile;

    if ((PFile=fopen("resultRx.txt", "at")) == NULL)
    {
        ErrMsgBox(TEXT("Write Result File Failure !!"));
    }
    else
    {
        fprintf(PFile, "%s\n", pRxStaStr);
        fclose(PFile);
    }
}

void CMP819xVCDlg::UpdateRxStat()
{
	//
	// Description: Update Rx statistics.
	// 2005.07.22, by rcnjko.
	//
    BOOL    bResult;
    int		RxOK, RxCRCErr;
	int		PhyRxOK, PhyRxCRCErr;
	int		MacRxOK, MacRxCRCErr;
    TCHAR   RxOKstr[100];
	TCHAR   PhyRxOKstr[100];
	TCHAR   MacRxOKstr[100];
    static  int    LastRxOK;
    TCHAR   RxStatisStr[100];
    TCHAR   RxTempStr[30], RxTempStr1[30];
    //MIMO_RSSI mimoRssi;
	//UCHAR	pBuf[20] = {0};
    //int len;
    //char signalStrength[40];

	PrintMsg(TEXT("===> UpdateRxStat()\n"));

    RxStatisStr[0] = 0;
    if((!bInReset) && (!bNeedReset) && (!bInInitialGain))
    {
        bInQueryRx = TRUE;

        bResult = QueryRxPacketReceived(
                    (LPADAPTER) Adapter.hFile,
                    &RxOK
                    );
        sprintf(RxOKstr, "%d", RxOK);
        sprintf(RxTempStr, "%d", RxOK);
        m_RcvOK.SetWindowText(RxOKstr);

        if(RxOK != 0)
        {
            if(AutoTestRxOn)
                m_Result.SetWindowText(TEXT("Auto Rx test is in progress!"));
            if(LastRxOK == RxOK)
            {
                if(AutoTestRxOn)
                    RxPktNumNoChgCnt++;
                else
                    m_Result.SetWindowText(TEXT("No more Inbound Packet"));
            }
            else
                RxPktNumNoChgCnt = 0;
        }
        else
            RxPktNumNoChgCnt = 0;

        LastRxOK = RxOK;

        bResult = QueryRxPacketCRC32Error(
                    (LPADAPTER) Adapter.hFile,
                    &RxCRCErr
                    );
        sprintf(RxOKstr, "%d", RxCRCErr);
        sprintf(RxTempStr1, "%d", RxCRCErr);
        m_RcvErr.SetWindowText(RxOKstr);
#if 0
		memcpy(pBuf, (PUCHAR)&mimoRssi, sizeof(mimoRssi));

        bResult = Query_MIMPO_RSSI(
                (LPADAPTER)Adapter.hFile,
                pBuf,//(UCHAR *)&mimoRssi,
                &len
                );
        if(!bResult)
        {
            //m_Result.SetWindowText(TEXT("Fail to Query MIMO RSSI !"));
        }
        else
        {
        	UINT AntennaSS_A=0, AntennaSS_B=0, AntennaSS_C=0, AntennaSS_D=0;

			memcpy((PUCHAR)&mimoRssi, pBuf, sizeof(mimoRssi));

        	if(Adapter.CurrentAntenna & ANTENNA_A)
        		AntennaSS_A = mimoRssi.AntennaA;
        	if(Adapter.CurrentAntenna & ANTENNA_B)
        		AntennaSS_B = mimoRssi.AntennaB;
        	if(Adapter.CurrentAntenna & ANTENNA_C)
        		AntennaSS_C = mimoRssi.AntennaC;
        	if(Adapter.CurrentAntenna & ANTENNA_D)
        		AntennaSS_D = mimoRssi.AntennaD;
            if(RfMode == RF_2T4R)
            {
            	sprintf(signalStrength, TEXT("%d%%,%d%%,%d%%,%d%%"), AntennaSS_A,
					AntennaSS_B, AntennaSS_C, AntennaSS_D);
			}
			else if(RfMode == RF_1T2R)
			{
				sprintf(signalStrength, TEXT("%d%%,%d%%"), AntennaSS_A, AntennaSS_B);
			}
			else if(RfMode == RF_2T2R|| RfMode == RF_2T2R_GREEN)
			{
				sprintf(signalStrength, TEXT("%d%%,%d%%"), AntennaSS_A, AntennaSS_B);
			}
			else if(RfMode == RF_1T1R)
			{
				sprintf(signalStrength, TEXT("%d%%"), AntennaSS_A);
			}
			else if(RfMode == RF_2T3R)
			{
				if(IS_HARDWARE_SERIES(8814A))
					sprintf(signalStrength, TEXT("%d%%,%d%%,%d%%"),AntennaSS_B, AntennaSS_C, AntennaSS_D);
			}
#endif

        if(RxPktNumNoChgCnt > MaxIdlePollingTimes)
        {
            RxPktNumNoChgCnt = 0;
            AutoRxWriteFileIndex++;
            m_Result.SetWindowText(TEXT("Rx state is idle!"));

            sprintf(RxStatisStr, "%d. Rcv Packets : %s", AutoRxWriteFileIndex, RxTempStr);
            strcat(RxStatisStr, ", Rcv CRCErr : ");
            strcat(RxStatisStr, RxTempStr1);
            WriteToRxStatisticsFile(RxStatisStr);
            OnReset();
        }
        bInQueryRx = FALSE;
    }

	
	bResult = QueryPHYLayerRxPacketReceived((LPADAPTER)Adapter.hFile, &PhyRxOK);
	sprintf(PhyRxOKstr, "%d", PhyRxOK);

	if (m_RxPhyOkCheckBox.GetCheck())
		m_RcvPhyOK.SetWindowText(PhyRxOKstr);
	else
	{
		m_RcvPhyOK.SetWindowText(TEXT("-"));
	}
	if (m_RxPhyErrCheckBox.GetCheck())
	{
		bResult = QueryPHYLayerRxPacketCRC32Error((LPADAPTER) Adapter.hFile, &PhyRxCRCErr);
		sprintf(PhyRxOKstr, "%d", PhyRxCRCErr);
		m_RcvPhyErr.SetWindowText(PhyRxOKstr);
	} 
	else
	{
		m_RcvPhyErr.SetWindowText(TEXT("-"));
	}
	
	
	
		bResult = QueryMacRxPacketReceived((LPADAPTER)Adapter.hFile, &MacRxOK);
	sprintf(MacRxOKstr, "%d", MacRxOK);
		if (m_RxMacOkCheckBox.GetCheck())
			m_RcvMacOK.SetWindowText(MacRxOKstr);
		else
		{
			m_RcvMacOK.SetWindowText(TEXT("-"));
		}
		if (m_RxMacErrCheckBox.GetCheck())
		{
			bResult = QueryMacRxPacketCRC32Error((LPADAPTER) Adapter.hFile, &MacRxCRCErr);
		sprintf(MacRxOKstr, "%d", MacRxCRCErr);
			m_RcvMacErr.SetWindowText(MacRxOKstr);
		} 
		else
		{
			m_RcvMacErr.SetWindowText(TEXT("-"));
		}
	
	PrintMsg(TEXT("<=== UpdateRxStat()\n"));
}

void CMP819xVCDlg::UpdateTxStat()
{
	BOOL	bResult;
	UINT	PhyTxOK;
	TCHAR	PhyTxOKstr[100];
	bResult = QueryPHYTxPacketSent((LPADAPTER)Adapter.hFile, &PhyTxOK);
	_stprintf(PhyTxOKstr, TEXT("%d"), PhyTxOK);

	m_XmitOK.SetWindowText(PhyTxOKstr);

	if(Adapter.PacketCount == 0)
	{
		// PMAC CCK only support 0xFFFF and don't support endless packet count.
		if(IS_CCK_RATE(Adapter.CurrentRate) && (PhyTxOK == 65535))
		{
			StartTesting_PMacPacketTx(FALSE);
			StartTesting_PMacPacketTx(TRUE);
		}
	}
	else if(PhyTxOK >= Adapter.PacketCount)
	{
		if (!TestStop)
			OnStartTesting();
	}
}

void CMP819xVCDlg::UpdateAckCounter()
{
	BOOL	bResult;
	ULONG	AckPktCount;
	TCHAR	AckPktCountstr[100];

	bResult = GetAckCounter((LPADAPTER)Adapter.hFile, &AckPktCount);
	_stprintf(AckPktCountstr, TEXT("%d"), AckPktCount);

	m_RxAckPktCounter.SetWindowText(AckPktCountstr);
}

void CMP819xVCDlg::IoMonitor()
{
	char regVal[20];
	if (Adapter.bIsMonitorOn) {
		UINT result = RegisterRead(Adapter.MonitorAddr1);
		_stprintf(regVal, TEXT("0x%08X"), result);
		m_MonitorValue1.SetWindowText(regVal);
	}
}

void CMP819xVCDlg::RSSIMonitor()
{
		char	rssiVal[20];
		BOOL    bResult;
		ULONG   RegDataValue;

		// Read back MAC register.
		bResult = ReadRSSIValue(
					(LPADAPTER)(Adapter.hFile),
					&RegDataValue
					);

		if(!bResult)
		{
			m_Result.SetWindowText(TEXT("Read RSSI FAIL"));
			ErrMsgBox(TEXT("Fail to Read RSSI !!" ));
		}
		else
		{
			_stprintf(rssiVal, TEXT("%d"), RegDataValue);
			m_MonitorRSSIValue1.SetWindowText(rssiVal);
		}
}

void CMP819xVCDlg::WPSButtonPushPolling()
{
    BOOL    bResult;
    UINT    pushed=0;

    bResult = ButtonPushed(
                (LPADAPTER) Adapter.hFile,
                ButtonWPS,
                (int*)&pushed
                );
	if(bResult)
	{
		if(pushed)
			m_Result.SetWindowText(TEXT("WPS Button is pushed!"));
		else
			m_Result.SetWindowText(TEXT("WPS Button is NOT pushed!"));
	}
	else
	{
		m_Result.SetWindowText(TEXT("Polling WPS Button failed!!"));
	}
}

void CMP819xVCDlg::HWRfStatusPolling()
{
    BOOL    bResult;
    UINT    RfStatus=0;

    bResult = QueryRfStatus(
                (LPADAPTER) Adapter.hFile,
                (int*)&RfStatus
                );
	if(bResult)
	{
		if(RfStatus == eRfOn)
			m_Result.SetWindowText(TEXT("HW RF on!"));
		else if(RfStatus == eRfOff)
			m_Result.SetWindowText(TEXT("HW RF off!"));
	}
	else
	{
		m_Result.SetWindowText(TEXT("Polling HW RF status failed!!"));
	}
}

void CMP819xVCDlg::checkPowerTableStatus()
{
	CString Str1;
	Str1.Format("%s",szPopMsg);
	
	if(CustomPowerTableStatus == 1){
		KillTimer(DIALOG_MONITOR);
		CustomPowerTableStatus=0;
		MessageBox(
			TEXT(Str1),
			TEXT("Realtek"),
			MB_OK|MB_ICONWARNING|MB_SETFOREGROUND
			);
		PostMessage(WM_QUIT);
	}else if(CustomPowerTableStatus == 2){
		KillTimer(DIALOG_MONITOR);
		CustomPowerTableStatus=0;
		if(MessageBox(TEXT(Str1),TEXT("Exiting MP Tool.."),MB_YESNO|MB_ICONWARNING|MB_SETFOREGROUND)!=IDYES)
			;
		else
			PostMessage(WM_QUIT);
	}else{
		//on exception: kill timer and reset flag silently
		KillTimer(DIALOG_MONITOR);
		CustomPowerTableStatus=0;
	}
}

#pragma endregion Timer

#pragma region [Event]-StatusChanged

void CMP819xVCDlg::OnSelchangeTestList()
{
    int i;

    i = m_TestList.GetCurSel();
    TestItem = (TEST_MODE)i;

}

void CMP819xVCDlg::OnSelchangeChannel()
{
    int     i;
    UINT	tmpChannel, tmpTxPower, tempVal;

	i = m_Channel.GetCurSel();
    tmpChannel = IndexValueConvert(ComboChannel, Index2Value, i);
	
	/* 40M, channel must be in 3~11 */
	if (IS_RF_2G_40M(Adapter.CurrentModulation, Adapter.CurrentBandWidth))
	{
		if(tmpChannel < ChannelMin40M)
			tmpChannel = ChannelMin40M;
		if(tmpChannel > ChannelMax40M)
			tmpChannel = ChannelMax40M;
	} 
	else if (IS_RF_5G_80M(Adapter.CurrentModulation, Adapter.CurrentBandWidth))
	{
		// TODO:
	}
	

	if(Adapter.CurrentChannel != tmpChannel)
	{
		UpdateChannel(tmpChannel, 1);
		tmpTxPower = GetCurrChannelTxPower();
		UpdateTxPower(tmpTxPower, 0);
	}
	else
	{
		tempVal = IndexValueConvert(ComboChannel, Value2Index, Adapter.CurrentChannel);
    	m_Channel.SetCurSel(tempVal);
	}

}

void CMP819xVCDlg::OnSelchangeRate()
{

    int		idx = m_Rate.GetCurSel();
	UINT	CurRate = Adapter.CurrentRate;
    UINT	tmpRate = IndexValueConvert(ComboDataRate, Index2Value, idx);
    UINT	tmpPreamble = Adapter.PacketPreamble;
    
	if(IS_CCK_RATE(tmpRate)) /* CCK rate */
	{
	    /* A/B/G/N all support */
	    m_Preamble.EnableWindow(TRUE);

	    if(tmpRate == RATE_CCK_1M)
	    	tmpPreamble = Long_Preamble;
	    else if(tmpPreamble != Long_Preamble && tmpPreamble != Short_Preamble)
		    tmpPreamble = Long_Preamble;
	}
	else if(IS_OFDM_RATE(tmpRate))   /* OFDM rate */
	{
	    /* only A/G/N support */
		m_Preamble.EnableWindow(FALSE);
	}
	else if(tmpRate >= RATE_MCS0)  /* MCS rate */
	{
	    /* only N support */
	    // only long/short GI supported
	    m_Preamble.EnableWindow(TRUE);

		if(tmpPreamble != Long_GI && tmpPreamble != Short_GI)
		    tmpPreamble = Long_GI;
	}
    
    if(CurRate != tmpRate)
    {
    	UpdateDataRate(tmpRate, 1);
    }

    if(Adapter.PacketPreamble != tmpPreamble)
    {
        UpdatePreamble(tmpPreamble, 0);
    }

	// Change AntennaTx List based on rate
	//if(RfMode == RF_3T3R)
	if(IS_HARDWARE_SERIES(8814A))
	{
		idx = m_AntennaTx.GetCurSel();

		m_AntennaTx.ResetContent();
		if(IS_3SS_RATE(Adapter.CurrentRate))
		{
			m_AntennaTx.AddString("1,2,3");
			m_AntennaTx.SetCurSel(0);
		}
		else if(IS_2SS_RATE(Adapter.CurrentRate) ||
				m_CheckBox_STBC.GetCheck() == BST_CHECKED)
		{
			m_AntennaTx.AddString("1,2");
			//
			//20150127, KenSun, If 8814 is configured as 2TXR, It only supports the combination of  RF path {1, 2} currently. (According to Willis's comments)
			//
			//m_AntennaTx.AddString("2,3");
			//if(idx == 2)
			//	m_AntennaTx.SetCurSel(1);
			//else
			m_AntennaTx.SetCurSel(0);
		}
		else
		{
			ShowValidTxPath();
			if(regAdapter.bEnableMultiPathFor1SSRate)
			{
				if(RfMode == RF_3T3R || RfMode == RF_2T4R || RfMode == RF_2T2R)
					m_AntennaTx.AddString("1,2");
				if(RfMode == RF_3T3R)
					m_AntennaTx.AddString("1,2,3");
			}
			m_AntennaTx.SetCurSel(0);
		}
	}

	if (Adapter.EnableAllRates == FALSE || IS_HARDWARE_SERIES(8814A))//RfMode == RF_3T3R)
    	ChangeAntenna();
}

void CMP819xVCDlg::OnSelchangeAntennaTx()
{
	ChangeAntenna();
}

void CMP819xVCDlg::OnSelchangeAntennaRx()
{
	ChangeAntenna();
}

void CMP819xVCDlg::OnSelchangeAntennaSetting()
{
	int Mode = RFPATH_5G_WLA;
	if (Adapter.CurrentModulation == WIRELESS_MODE_N_24G)
	{
		switch(m_RFPathSetting.GetCurSel())
		{
			case 3:
				Mode = RFPATH_BLUETOOTH;
				break;
			case 0:
				Mode = RFPATH_2G_BTG;
				break;
			case 1:
				Mode = RFPATH_2G_WLG;
				break;
			case 2:default:
				if (Adapter.CurrentRFPathSetting != RFPATH_5G_WLA)
					Mode = Adapter.CurrentRFPathSetting;
				else
					Mode = Adapter.DefaultRFPathSetting;
				break;
		}
	}
	else
	{
		switch(m_RFPathSetting.GetCurSel())
		{
			case 3:
				Mode = RFPATH_BLUETOOTH;
				break;

			case 0:case 1:case 2:default:
				Mode = RFPATH_5G_WLA;
				break;
		}
	}
	UpdateRFPathSetting(Mode, 0);
}


void CMP819xVCDlg::OnSelchangeAntennaSettingDuplicate()
{
	int Mode = RFPATH_5G_WLA;

	switch(m_RFPathSetting_Duplicate.GetCurSel())
	{
		case 1:
			Mode = RFPATH_BLUETOOTH;
			break;

		case 0:default:
			if (Adapter.CurrentModulation == WIRELESS_MODE_N_24G)
			{
				Mode = Adapter.DefaultRFPathSetting;
			}
			else
			{ // 5G Default Use WLA
				Mode = RFPATH_5G_WLA;
			}
			break;
	}
	UpdateRFPathSetting(Mode, 0);
}
void CMP819xVCDlg::OnSelchangeTxPower()
{
    UINT	tmpTxPower;

    tmpTxPower = GetSelectedTxPowerControl();

    if(Adapter.CurrentTxPower != tmpTxPower)
    {
		UpdateTxPower(tmpTxPower, 1);
    }

}

void CMP819xVCDlg::OnSelchangePattern()
{
    int i;

    i = m_Pattern.GetCurSel();
    Adapter.PacketPattern = Patterns[i];
    //PrintMsg(TEXT("OnSelchangePattern(): Pattern = 0x%X\n"), Adapter.PacketPattern);
}

void CMP819xVCDlg::OnSelchangePreamble()
{	// preamble depends on data rate
	// MCS rate : Short/Long GI
	// OFDM rate: doesn't make sence.
	// CCK rate : Short/Long Preamble, but CCK_1M can only be Long preamble.
    int     i;
    UINT    tmpPreamble, tempVal;

    i = m_Preamble.GetCurSel();
    tmpPreamble = IndexValueConvert(ComboPreamble, Index2Value, i);
    
	if(IS_CCK_RATE(Adapter.CurrentRate)) // Supports ONLY "Preamble"
    {
    	if(tmpPreamble != Long_Preamble && tmpPreamble != Short_Preamble)
    		tmpPreamble = Long_Preamble;
    	else if(Adapter.CurrentRate == RATE_CCK_1M || Adapter.CurrentRate == RATE_CCK_2M)
    		tmpPreamble = Long_Preamble;
    }
    else if(IS_HT_RATE(Adapter.CurrentRate)) // Supports ONLY "GI"
    {
    	if(tmpPreamble != Long_GI && tmpPreamble != Short_GI)
    		tmpPreamble = Long_GI;
    }
	else if(IS_VHT_RATE(Adapter.CurrentRate)) // Supports ONLY "GI"
    {
    	if(tmpPreamble != Long_GI && tmpPreamble != Short_GI)
    		tmpPreamble = Long_GI;
    }
    
    if (Adapter.PacketPreamble != tmpPreamble)
    {
    	UpdatePreamble(tmpPreamble, 1);
		m_Preamble.SetCurSel(--tmpPreamble);
    }
    else
    {
    	tempVal = IndexValueConvert(ComboPreamble, Value2Index, Adapter.PacketPreamble);
    	m_Preamble.SetCurSel(tempVal);
    }
}

void CMP819xVCDlg::OnSelchangeBand()
{
    int     i;
    UINT    tmpModulation, tmpRate, tmpPreamble, tmpBandwidth, tmpChannel, tmpTxPower;
	
    tmpRate = Adapter.CurrentRate;
	tmpBandwidth = Adapter.CurrentBandWidth;
	tmpPreamble = Adapter.PacketPreamble;
	tmpChannel = Adapter.CurrentChannel;
	

    i = m_Modulation.GetCurSel();
    tmpModulation = IndexValueConvert(ComboModulation, Index2Value, i);
    switch(tmpModulation)
    {
        case WIRELESS_MODE_B:
            if(tmpRate >= RATE_OFDM_6M)
                tmpRate = RATE_CCK_11M;
			tmpBandwidth = MPT_BW_20MHZ;
            break;
        case WIRELESS_MODE_G:
            if(tmpRate >= RATE_MCS0)
                tmpRate = RATE_OFDM_54M;
			tmpBandwidth = MPT_BW_20MHZ;
            break;
        case WIRELESS_MODE_N_24G:
			tmpBandwidth = MPT_BW_20MHZ;
			tmpRate = RATE_MCS0;
			tmpChannel = Channels_2_4G_20M[0];
            break;
        case WIRELESS_MODE_N_5G:
			if(1 == customBWsetting[0][1].bw_en)
			{
				tmpBandwidth = MPT_BW_40MHZ;
				tmpChannel = Channels_5G_40M[0];
			}
			else
			{
				tmpBandwidth = MPT_BW_20MHZ;
				tmpChannel = Channels_5G_20M[0];
			}
			tmpRate = RATE_MCS0;
			
            break;			
		case WIRELESS_MODE_AC_5G:
			if(1 == customBWsetting[1][2].bw_en)
			{
				tmpBandwidth = MPT_BW_80MHZ;
				tmpChannel = Channels_5G_80M[0];
			}
			else if(1 == customBWsetting[1][1].bw_en)
			{
				tmpBandwidth = MPT_BW_40MHZ;
				tmpChannel = Channels_5G_40M[0];
			}
			else
			{
				tmpBandwidth = MPT_BW_20MHZ;
				tmpChannel = Channels_5G_20M[0];
			}
			tmpRate = (bVHTEnabled? RATE_VHT1SS_MCS0:RATE_MCS0);
			
			break;
    }
	

    if (Adapter.CurrentModulation != tmpModulation)
    {
    	UpdateModulation(tmpModulation, 1);
    }

	
	//+++++++++++++++++++++++++++++++++++++++++++++
	//Add Antenna Setting for New IC
	if(IS_HARDWARE_SERIES(8821C) || IS_HARDWARE_8731AU)
	{
		if (Adapter.CurrentModulation == WIRELESS_MODE_N_24G)
		{
			UpdateRFPathSetting(Adapter.DefaultRFPathSetting, 0);
			//m_RFPathSetting.EnableWindow(TRUE);
		}
		else
		{
			UpdateRFPathSetting(RFPATH_5G_WLA, 0);
			//m_RFPathSetting.EnableWindow(FALSE);
		}
	}
	//+++++++++++++++++++++++++++++++++++++++++++++

	// Always do this for customize BW modification	
	UpdateBandwidth(tmpBandwidth, 0);

	// After Modulation & Bandwidth are updated by OID, Switch rate set on UI.
	SwitchRateSet(Adapter.CurrentModulation, Adapter.CurrentBandWidth);

    if (Adapter.CurrentRate != tmpRate)
    {
    	UpdateDataRate(tmpRate, 0);
    }

 	if (Adapter.CurrentChannel != tmpChannel)
    {
    	UpdateChannel(tmpChannel, 0);
    }

	tmpTxPower = GetCurrChannelTxPower();
	// <20120621, Kordan> Prevent from PowerByRate and Regulatory mechanism of normal driver when switching channel.
	if (IS_HARDWARE_SERIES(8188E) || IS_HARDWARE_SERIES(8188F)) 
		UpdateTxPower(tmpTxPower, 0);
	else if(Adapter.CurrentTxPower != tmpTxPower)
		UpdateTxPower(tmpTxPower, 0);

    if(IS_CCK_RATE(tmpRate)) 
    {
		m_Preamble.EnableWindow(TRUE);
        if(tmpPreamble != Long_Preamble && tmpPreamble != Short_Preamble)
            tmpPreamble = Long_Preamble;
       	if(tmpRate == RATE_CCK_1M)
       		tmpPreamble = Long_Preamble;
    }
    else if(IS_OFDM_RATE(tmpRate))
    {
        /* only A/G/N support */
		m_Preamble.EnableWindow(FALSE);
    }
    else if(tmpRate >= RATE_MCS0)  /* MCS rate */
    {
        /* only N support */
        m_Preamble.EnableWindow(TRUE);
        if(tmpPreamble != Long_GI && tmpPreamble != Short_GI)
            tmpPreamble = Long_GI;
    }
    if (Adapter.PacketPreamble != tmpPreamble)
    {
    	UpdatePreamble(tmpPreamble, 0);
    }
}


void CMP819xVCDlg::OnSelChangeBandwidth()
{
    int i;
	int idx;
    UINT tmpBandwidth, tmpChannel, tmpTxPower;

    i = m_Channel_Width.GetCurSel();

	if(Adapter.CurrentModulation == WIRELESS_MODE_N_5G || 
	Adapter.CurrentModulation == WIRELESS_MODE_AC_5G)
	{
		for(idx = 0; idx < MAX_SUPPORT_BW_NUM ; idx++)
		{
			if(i == customBWsetting[1][idx].comboxOrder)
			{
				tmpBandwidth = customBWsetting[1][idx].comboxBW;
				break;
			}
		}
	}
	else
	{
		for(idx = 0; idx < MAX_SUPPORT_BW_NUM ; idx++)
		{
			if(i == customBWsetting[0][idx].comboxOrder)
			{
				tmpBandwidth = customBWsetting[0][idx].comboxBW;
				break;
			}
		}
	}

	if (Adapter.CurrentBandWidth != tmpBandwidth)
	{
		UpdateBandwidth(tmpBandwidth, TRUE);
	}

	i = m_Channel.GetCurSel();
	tmpChannel = IndexValueConvert(ComboChannel, Index2Value, i);

	if (Adapter.CurrentChannel != tmpChannel)
	{
		UpdateChannel(tmpChannel, FALSE);
	}

	SwitchRateSet(Adapter.CurrentModulation, Adapter.CurrentBandWidth);
	OnSelchangeRate();	// Update Tx Antenna Setting when changing SignalLocation

	tmpTxPower = GetCurrChannelTxPower();
	if(Adapter.CurrentTxPower != tmpTxPower)
	{
		UpdateTxPower(tmpTxPower, FALSE);
	}
}

void CMP819xVCDlg::OnSelchangeCustomerSelPwr()
{
	switch (m_CustomerSelPwr.GetCurSel())
    {
	case 0:
		m_Result.SetWindowText(TEXT("Default"));
		m_Button_CSC.SetWindowText(TEXT("Update changes"));
		break;
	case 1:
		m_Result.SetWindowText(TEXT("Selecting.."));
		{
			CString szFilter = "Power by Rate Table File|*.txt||";
			
			CFileDialog *fd = new CFileDialog(TRUE,"txt","*.txt",OFN_HIDEREADONLY,szFilter,this);
			TCHAR currentDir[MAX_PATH];
			CString strFormatData;

			GetCurrentDirectory( MAX_PATH, currentDir );
			
			fd->m_ofn.lpstrInitialDir = currentDir;
			

			if(fd->DoModal() == IDOK) {
				
				OnInitCustomerPwr();
				m_CustomerSelPwr.AddString(fd->GetFileName());
				strcpy(CS_PWR_FILE, (LPCTSTR)fd->GetPathName());
				m_CustomerSelPwr.SetCurSel(2);
				m_Button_CSC.SetWindowText(TEXT("Update changes"));
				m_Result.SetWindowText("file copied: "+fd->GetFileName());
				
				
			} else {
				m_CustomerSelPwr.SetCurSel(0);
				m_Button_CSC.SetWindowText(TEXT("Update changes"));
				return;
			}
			delete fd;
		}
		break;
	default:
		m_Result.SetWindowText(TEXT(""));
		m_Button_CSC.SetWindowText(TEXT("Update changes"));

	}
	;
}

void CMP819xVCDlg::OnSelchangeCustomerSelPwrLmt()
{
	switch (m_CustomerSelPwrLmt.GetCurSel())
    {
	case 0:
		m_Result.SetWindowText(TEXT("Default"));
		m_Button_CSC.SetWindowText(TEXT("Update changes"));
		break;
	case 1:
		m_Result.SetWindowText(TEXT("Selecting.."));
		{
			CString szFilter = "Power Limit Table File|*.txt||";
			
			CFileDialog *fd = new CFileDialog(TRUE,"txt","*.txt",OFN_HIDEREADONLY,szFilter,this);
			TCHAR currentDir[MAX_PATH];
			CString strFormatData;

			GetCurrentDirectory( MAX_PATH, currentDir );
			
			fd->m_ofn.lpstrInitialDir = currentDir;


			if(fd->DoModal() == IDOK) {
				
				OnInitCustomerPwrLmt();
				m_CustomerSelPwrLmt.AddString(fd->GetFileName());
				strcpy(CS_PWR_LMT_FILE, (LPCTSTR)fd->GetPathName());

				m_CustomerSelPwrLmt.SetCurSel(2);
				m_Button_CSC.SetWindowText(TEXT("Update changes"));
				m_Result.SetWindowText("file copied: "+fd->GetFileName());

				

			} else {
				m_CustomerSelPwrLmt.SetCurSel(0);
				m_Button_CSC.SetWindowText(TEXT("Update changes"));
				return;
			}
			delete fd;
		}
		break;
	default:
		m_Result.SetWindowText(TEXT(""));
		m_Button_CSC.SetWindowText(TEXT("Update changes"));
		
	}
	;
}

void CMP819xVCDlg::OnSelchangeEfuseTarget()
{
    switch (m_EfuseTargetSel.GetCurSel())
    {
    case TARGET_FILE: // FILE
        m_EEByteOffset.EnableWindow(FALSE);
        m_EEByteValue.EnableWindow(FALSE);
		m_ReadEEprom.EnableWindow(TRUE);
		m_WriteEEprom.EnableWindow(TRUE);
		m_UpdateEfuse.EnableWindow(TRUE);

        m_Result.SetWindowText(TEXT("Operation Target: Map File"));
		break;
	case TARGET_EFUSE: // EFUSE
        m_EEByteOffset.EnableWindow(FALSE);
        m_EEByteValue.EnableWindow(FALSE);
		m_ReadEEprom.EnableWindow(TRUE);
		m_WriteEEprom.EnableWindow(FALSE);
		m_UpdateEfuse.EnableWindow(TRUE);
		
        m_Result.SetWindowText(TEXT("Operation Target: EFUSE"));	
		break;
	case TARGET_BYTE: // Byte
        m_EEByteOffset.EnableWindow(TRUE);
        m_EEByteValue.EnableWindow(TRUE);
		m_ReadEEprom.EnableWindow(TRUE);
		m_WriteEEprom.EnableWindow(TRUE);
		m_UpdateEfuse.EnableWindow(FALSE);
		
        m_Result.SetWindowText(TEXT("Operation Target: EFUSE Byte"));
		break;
	default:
		m_Result.SetWindowText(TEXT("Operation Target: Invalid"));
		break;
    }

}

void CMP819xVCDlg::OnChangeFileIndex()
{
    int i, stopped=0;

    i = m_FileIndex.GetCurSel();

    if( AutoTestType == AUTO_TEST_TX_SITE ||
        AutoTestType == AUTO_TEST_RX_SITE )
	{
        StopAutoTest();     //stop the original test.
        stopped = 1;
	}
    switch(i)
    {
        case AUTO_TEST_NONE:// None
            AutoTestType = AUTO_TEST_NONE;
            break;
        case AUTO_TEST_TX_SITE:// Auto test (Tx site)
            StartAutoTestTx();
            AutoTestType = AUTO_TEST_TX_SITE;
            break;
        case AUTO_TEST_RX_SITE:// Auto test (Rx site)
            StartAutoTestRx();
            AutoTestType = AUTO_TEST_RX_SITE;
            break;

        case AUTO_TEST_STOP:// Stop testing
        	if(!stopped)
            	StopAutoTest();
            m_FileIndex.SetCurSel(0);
            AutoTestType = AUTO_TEST_NONE;
            break;
        default:
            m_FileIndex.SetCurSel(0);
            AutoTestType = AUTO_TEST_NONE;
            break;
    }
}

void CMP819xVCDlg::OnSelchangeXtalXinXout() 
{
    UINT    tmpCrystalCap;

    tmpCrystalCap = m_Xtal_XinXout.GetCurSel();
    
    if(Adapter.CurrentCrystalCap != tmpCrystalCap)
    {
		UpdateCrystalCap(tmpCrystalCap, 1);
    }
	
}

void CMP819xVCDlg::OnSelchangeTxSC_Selection()
{
	BOOL    bResult;
	int nCurrSelIdx, TxSC;
	MPT_BANDWIDTHE	BWtoSet;
	nCurrSelIdx = m_TxSC_Selection.GetCurSel();
	TxSC = m_TxSC_Selection.GetItemData(nCurrSelIdx);
	
	Win32Print("(%d %x, %x) \n", nCurrSelIdx, TxSC, Adapter.TxSC);

	if(TxSC == Adapter.TxSC)
		return;
	else
		Adapter.TxSC = (DATA_SC)TxSC;

	if(Adapter.CurrentBandWidth == MPT_BW_40MHZ)
	{
		switch (TxSC){
		case DATA_SC_DONOT_CARE:
			BWtoSet = MPT_BW_40MHZ;
			break;
		case DATA_SC_20_ABOVE_OF_80MHZ:
			BWtoSet = MPT_BW_40MHZ_ABOVE;
			break;
		case DATA_SC_20_BELOW_OF_80MHZ:
			BWtoSet = MPT_BW_40MHZ_BELOW;
			break;
		}
	}
	else if(Adapter.CurrentBandWidth == MPT_BW_80MHZ)
	{
		switch (TxSC){
		case DATA_SC_DONOT_CARE:
			BWtoSet = MPT_BW_80MHZ;
			break;
		case DATA_SC_20_ABOVE_OF_80MHZ:
			BWtoSet = MPT_BW_80MHZ_20_ABOVE;
			break;
		case DATA_SC_20_BELOW_OF_80MHZ:
			BWtoSet = MPT_BW_80MHZ_20_BELOW;
			break;
		case DATA_SC_20_TOP_OF_80MHZ:
			BWtoSet = MPT_BW_80MHZ_20_TOP;
			break;
		case DATA_SC_20_BOTTOM_OF_80MHZ:
			BWtoSet = MPT_BW_80MHZ_20_BOTTOM;
			break;
		case DATA_SC_40_ABOVE_OF_80MHZ:
			BWtoSet = MPT_BW_80MHZ_40_ABOVE;
			break;
		case DATA_SC_40_BELOW_OF_80MHZ:
			BWtoSet = MPT_BW_80MHZ_40_BELOW;
			break;
		}
	}

	bResult = SetSignalLocationType((LPADAPTER) Adapter.hFile, (int)BWtoSet);
    if(!bResult)
    {
       	m_Result.SetWindowText(TEXT("Set TXSC fail."));
    }
    else
    {
       	m_Result.SetWindowText(TEXT("Set TXSC success."));
    }
}

void CMP819xVCDlg::OnSelchangeFW_Type()
{
	BOOL    bResult;
	int nCurrSelIdx, desired_fw;
	TCHAR   result_message[64];
			
	nCurrSelIdx = m_fw_type.GetCurSel();
	desired_fw = m_fw_type.GetItemData(nCurrSelIdx);
	
	Win32Print("selected item: %d, selected value: %d\n", nCurrSelIdx, desired_fw);

	bResult = DownloadFW((LPADAPTER)(Adapter.hFile), desired_fw);
	while( RT_STATUS_FAILURE == bResult)
	{
		_stprintf(result_message, TEXT("Fail to download %s."), FW_Table[desired_fw].fw_name);
		
		if (MessageBox(TEXT("Download FW failure!Retry?"), TEXT("FW"), MB_OKCANCEL) != IDOK)
		{
			ReInitFWOption();
			return;
		}
		bResult = DownloadFW((LPADAPTER)(Adapter.hFile), desired_fw);
	}

	_stprintf(result_message, TEXT("%s downloaded."), FW_Table[desired_fw].fw_name);
	
	m_Result.SetWindowText(result_message);
}

#pragma endregion StatusChanged

#pragma region [Event]-ButtonClicked

void CMP819xVCDlg::OnStartTesting()
{
    BOOL    bStart;

    TestStop =! TestStop;
    bStart =! TestStop;

	if(bStart)//Notify driver that testing will be started.
	{
		MPStartTest((LPADAPTER)(Adapter.hFile), FALSE, TestItem);
		Sleep(500);
		MPStartTest((LPADAPTER)(Adapter.hFile), TRUE, TestItem);
	}
	else
	{
		MPStartTest((LPADAPTER)(Adapter.hFile), FALSE, TestItem);
	}
    StartTesting(bStart);
}

void CMP819xVCDlg::OnCustomerConfirm()
{
	TCHAR	Buf[260];
	TCHAR	Buf2[260];
	TCHAR	SystemDir[260];
	TCHAR	targetFile[260];
	CString strFormatData;
	CString strFormatData2;
	
	CFileFind Finder;
	BOOL bFound1,bFound2;
	GetSystemDirectory(SystemDir,260);
	m_Button_CSC.GetWindowText(Buf,260);
	strFormatData2.Format("%s",Buf);

	bFound1		=FALSE;
	bFound2		=FALSE;
	
	strFormatData.Format("Default");
	strcpy(Buf, (LPCTSTR)strFormatData);
	strcpy(Buf2, (LPCTSTR)strFormatData);
	
	if(strFormatData2 == "Change settings"){
		m_CustomerSelPwr.EnableWindow();
		m_CheckBox_EnableTxPowerLimit.EnableWindow();
		if(m_CheckBox_EnableTxPowerLimit.GetCheck() == BST_CHECKED)
			m_CustomerSelPwrLmt.EnableWindow();
		m_Button_CSC.SetWindowText(TEXT("Update changes"));

	}else if(strFormatData2 == "Update changes"){
		ResetPwrTableStatus();
		ResetPwrLmtTableStatus();


		//<20130117, Kordan> If OS version is Win7 or above, disable the x64 redirection.
		OSVERSIONINFOEX osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		BOOL bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);
		if (osvi.dwMajorVersion >= 6 && osvi.dwMinorVersion >= 1) 
		{
			PVOID OldValue = NULL;
			HINSTANCE  hlibrary; 
			typedef    int        (__stdcall  *  Wow64DisableWow64FsRedirection)(LPVOID); 
			Wow64DisableWow64FsRedirection  f_Wow64DisableWow64FsRedirection  =  NULL; 
			typedef    int        (__stdcall  *  Wow64RevertWow64FsRedirection)(LPVOID); 
			Wow64RevertWow64FsRedirection  f_Wow64RevertWow64FsRedirection  =  NULL;
			hlibrary  =  LoadLibrary("Kernel32.dll"); 
			f_Wow64DisableWow64FsRedirection  =  (Wow64DisableWow64FsRedirection)  GetProcAddress(hlibrary,"Wow64DisableWow64FsRedirection"); 
			if(!f_Wow64DisableWow64FsRedirection) 
				Win32Print("Fail to redirect.");
			f_Wow64DisableWow64FsRedirection  (&OldValue); 
		}

		if(m_CustomerSelPwr.GetCurSel()!=0){
			bFound1 = Finder.FindFile(CS_PWR_FILE);
			if(bFound1){
				m_CustomerSelPwr.GetLBText(2,Buf);
				strFormatData.Format("%s\\drivers\\%s",SystemDir,Buf);
				strcpy(targetFile, (LPCTSTR)strFormatData);
				
				if (CopyFile(CS_PWR_FILE,targetFile,0) == 0)
					Win32Print("Fail to copy file.");
			
			}else
				m_CustomerSelPwr.SetCurSel(0);
		}else{
			bFound1 = TRUE;
			m_CustomerSelPwr.SetCurSel(0);
		}
		if(m_CustomerSelPwrLmt.GetCurSel()!=0){
			bFound2 = Finder.FindFile(CS_PWR_LMT_FILE);
			if(bFound2){
				m_CustomerSelPwrLmt.GetLBText(2,Buf2);
				strFormatData.Format("%s\\drivers\\%s",SystemDir,Buf2);
				strcpy(targetFile, (LPCTSTR)strFormatData);

			    
				if (CopyFile(CS_PWR_LMT_FILE,targetFile,0) == 0){
					Win32Print("Fail to copy file." );
				}
			} else {
				m_CustomerSelPwrLmt.SetCurSel(0);
			}
		} else {
			bFound2 = TRUE;
			m_CustomerSelPwrLmt.SetCurSel(0);
		}

		strFormatData.Format("1. Power-by-Rate Table: \t\t[%s]\n2. Power Limit Table: \t\t[%s]\n\nTxPowerLimit is %s, confirmed?"
							,Buf,Buf2, ((m_CheckBox_EnableTxPowerLimit.GetCheck() == BST_CHECKED))?"ENABLED":"DISABLED");

		if(bFound1 && bFound2){
			;
		}else{
			if(!bFound2){
				strFormatData2.Format("[Power Limit File not found, using default]\n%s",strFormatData);
				strFormatData.Format("%s",strFormatData2);
			}
			if(!bFound1){
				strFormatData2.Format("[Power by rate File not found, using default]\n%s",strFormatData);
				strFormatData.Format("%s",strFormatData2);
			}
		}
		if(MessageBox(TEXT(strFormatData),TEXT("Realtek"),MB_ICONQUESTION|MB_YESNO)!=IDYES)
			return;
		else{
			
			UpdateCustomPwrTableReg(Buf,Buf2);
			m_Result.SetWindowText(TEXT("ready to load"));
			
			MessageBox(
            TEXT("[ Important ]\nPlease Disable and Enable NIC manually\n in order to update the changes."),
            TEXT("Realtek"),
            MB_OK|MB_ICONWARNING|MB_SETFOREGROUND
            );
			PostMessage(WM_QUIT); return;
		}
	}
	
}

void CMP819xVCDlg::OnWriteEeprom()
{
    BOOL    bResult = FALSE;
    UINT    untmpValue;
    UINT    EEOffset, EEValue;
    UCHAR   CurByte;
    CHAR	EEData[1536];
	UCHAR   GetEEData[1024];
    UCHAR   WriteEEData[512];
    TCHAR   strBuf[4];
	
	unsigned int 	i = 0, j = 0;

    if(TARGET_BYTE == m_EfuseTargetSel.GetCurSel()) // Byte mode:
    {
        // Get byte offset of EEPROM to write.
        if( !GetValueFromControl(
                &m_EEByteOffset,
                2, // We only care about 16-bit offset.
                (ULONG*)&untmpValue)
                )
        {
            m_Result.SetWindowText(TEXT("Wrong EFUSE Offset"));
            return;
        }

        EEOffset = (ULONG)untmpValue;

        // Get byte offset of EEPROM to write.
        if( !GetValueFromControl(
                &m_EEByteValue,
                1, // Byte value.
                (ULONG*)(&untmpValue))
                )
        {
            m_Result.SetWindowText(TEXT("Wrong EFUSE Value"));
            return;
        }

        EEValue = (UCHAR)untmpValue;
       
		if (bEEWiFiMode)
		{
			// Write value to EFUSE shadow
			bResult = WriteEFuseByte((LPADAPTER)(Adapter.hFile), EEOffset, EEValue);
		}
		else
		{
			// Write value to EFUSE shadow
			bResult = WriteBTEFuseByte((LPADAPTER)(Adapter.hFile), EEOffset, EEValue);
		}
	
        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Write EFUSE Byte FAIL"));
            ErrMsgBox(TEXT("Fail to Write EFUSE Byte !!" ));
        }
        else
        {
            _stprintf(strBuf, TEXT("%02X"), EEValue);
            m_CisData.SetWindowText(strBuf);
            m_Result.SetWindowText(TEXT("Write EFUSE Byte OK"));
        }
    }
    else if (TARGET_FILE == m_EfuseTargetSel.GetCurSel()) // File mode:
    {
		if(MessageBox(TEXT("Confirm to Write ?"),TEXT("Realtek"),MB_ICONQUESTION|MB_YESNO)!=IDYES)
			return;
		if (0)//(EEDataFile = fopen(regAdapter.MapFile,"r")) == NULL)
		{
			m_Result.SetWindowText(TEXT("Open MAP File FAIL"));
			ErrMsgBox(TEXT("Fail to Open Map File!!" ));
		}
		else
		{
			ZeroMemory(EEData, sizeof(EEData));
			ZeroMemory(GetEEData, sizeof(GetEEData));
			m_CisData.GetWindowText(EEData, Adapter.EfuseMapSize+1); // includes 1 EOS.
			

			for (i = 0, j = 0; i < 512*3; ++i) {
				if (EEData[i] == ' ' || EEData[i] == EOF || EEData[i] == '\r' || EEData[i] == '\n')
					continue;
				else
					GetEEData[j++] = Asc2Hex(EEData[i]);
			}


			ZeroMemory(WriteEEData, sizeof(WriteEEData));
			for (i = 0; i < 512; ++i) 
				WriteEEData[i] = 0xFF;
			
			// <For OneByteAligned>
			if (bOneByteAlign)
			{
				bResult = TRUE;
				for(i = 0; i < 512; i++)
				{
					CurByte  = GetEEData[i*2];
					CurByte <<= 4;
					WriteEEData[i] = CurByte + GetEEData[i*2+1];
					if(!WriteEFuseByte((LPADAPTER)(Adapter.hFile), i, WriteEEData[i])){
						bResult = FALSE;
						break;
					}
				}
			}
			else
			{
				for(i = 0; i < 512; i++)
				{
					//--
					//    Byte ordering translation:
					//    from Little-Endian (GetEEData[])
					//    to Big-endian (WriteEEData[]).
					//--

					// Low-Byte of a Word.
					CurByte  = GetEEData[i*2+2];
					CurByte <<= 4;
					WriteEEData[i] = CurByte + GetEEData[i*2+3];
					//WriteEFuseByte((LPADAPTER)(Adapter.hFile), i, WriteEEData[i]);

					// High-Byte of a Word.
					i++;
					CurByte  = GetEEData[i*2-2];
					CurByte <<= 4;
					WriteEEData[i] = CurByte + GetEEData[i*2-1];
					//WriteEFuseByte((LPADAPTER)(Adapter.hFile), i, WriteEEData[i]);
				}
				bResult= WriteEFuse((LPADAPTER)(Adapter.hFile), (UCHAR*)WriteEEData);
			}

			if(!bResult)
			{
				
				m_Result.SetWindowText(TEXT("Write EFUSE FAIL"));
				ErrMsgBox(TEXT("Fail to Write EFUSE !!" ));
				
			}
			else
			{
				
				m_Result.SetWindowText(TEXT("Write EFUSE OK"));
				MessageBox("Write EFUSE OK","Realtek");
				
			}
			
		}
    }
	
}

void CMP819xVCDlg::OnReadEeprom()
{
    BOOL    bResult = FALSE;
    int     i, EELen;
	int		counter;
	char	ch;
    UINT    untmpValue;
    UINT    EEOffset, EEValue;
    TCHAR   strBuf[256], editBuf[3076], editBuf2[60];
    UCHAR   GetEEData[1024];

	memset(editBuf, 0, 3076);
	memset(editBuf2, 0, 60);

	    if(TARGET_BYTE == m_EfuseTargetSel.GetCurSel()) // Byte Mode:
	    {
	        // Get byte offset of EEPROM to read.
	        if( !GetValueFromControl(
	            &m_EEByteOffset,
	            2, // We only care about 16-bit offset.
	            (ULONG*)&untmpValue)
	            )
	        {
	            m_Result.SetWindowText(TEXT("Wrong EFUSE Offset"));
	            return;
	        }
		 
	        EEOffset = (ULONG)untmpValue;

			if (bEEWiFiMode)
			{
				// Read value from EFUSW
				bResult = ReadEFuseByte((LPADAPTER)(Adapter.hFile), EEOffset,
		                    						(UCHAR*)&EEValue);	
			}
			else
			{
				// Read value from EFUSW
				bResult = ReadBTEFuseByte((LPADAPTER)(Adapter.hFile), EEOffset,
		                    						(UCHAR*)&EEValue);			
			}

			if(!bResult)
			{
				m_Result.SetWindowText(TEXT("Get EFUSE Byte FAIL"));
				ErrMsgBox(TEXT("Fail to Read EFUSE Byte !!" ));
			}
			else
			{
				sprintf(strBuf, TEXT("%02X"), (BYTE)EEValue);
				m_CisData.SetFont(&NormalFont,TRUE);
				m_CisData.SetWindowText(strBuf);
				m_Result.SetWindowText(TEXT("Get EFUSE Byte OK"));
			}
	    }
	    else if (TARGET_FILE == m_EfuseTargetSel.GetCurSel()) // File Mode:
	    {
	    	FILE *fp = fopen(regAdapter.MapFile, "r");	
			if (fp == NULL)
			{
				CString szFilter = "EFUSE Map File|*.map||";
				CFileDialog *fd = new CFileDialog(TRUE,"map","*.map",OFN_HIDEREADONLY,szFilter,this);
				TCHAR currentDir[MAX_PATH];
				GetCurrentDirectory( MAX_PATH, currentDir );
				fd->m_ofn.lpstrInitialDir = currentDir;
				if(fd->DoModal() == IDOK) {
					if ((fp = fopen(fd->GetFileName(), "r")) == NULL)
						return;
				} else {
					return;
				}
				delete fd;
			}
			if (fp != NULL)
			{
				fseek(fp, 0 ,SEEK_END);
				LONG lSize = ftell(fp);
  				rewind(fp);
				
				counter = 0; //<vincent lan> get aligned input with counter's help
				
				// <For OneByteAligned>
				UINT blockSize = 4;
				if (bOneByteAlign)
					blockSize = 2;
				
				while((ch=fgetc(fp)) != EOF)
				{
					if(ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r'){
						;
					}else{ 
						counter++;
						if(counter%blockSize == 0){
							if(counter%32 == 0)
								sprintf(editBuf2, "%s%c\r\n", editBuf2, ch);
							else
								sprintf(editBuf2, "%s%c ", editBuf2, ch);
						}else{
							sprintf(editBuf2, "%s%c", editBuf2, ch);
						}
						lstrcat(editBuf, editBuf2);
						sprintf(editBuf2, "");
					}
				}
				
				
				fclose(fp);
				if (bOneByteAlign)
					m_CisData.SetFont(&EfuseFont_Small,TRUE);
				else
					m_CisData.SetFont(&EfuseFont,TRUE);
				m_CisData.FmtLines(TRUE);
				m_CisData.SetWindowText(editBuf);
				Adapter.EfuseMapSize = lSize;

				m_Result.SetWindowText(TEXT("Get EFUSE OK"));
			}



	    }
	    else // EFUSE Mode:
	    {
	        ZeroMemory(GetEEData, sizeof(GetEEData));
			if (bEEWiFiMode)
			{
				bResult = ReadEFuse((LPADAPTER)(Adapter.hFile), (UCHAR*)&GetEEData);	
			}
			else
			{
				bResult = ReadBTEFuse((LPADAPTER)(Adapter.hFile), (UCHAR*)&GetEEData);		
			}
		
	        if(!bResult)
	        {
				m_Result.SetWindowText(TEXT("Get EFUSE Data FAIL"));
				ErrMsgBox(TEXT("Fail to Read EFUSE !!" ));
			}
	        
	        else
	        {
				EELen=0;
				if(Adapter.EEType == EEPROM_93C56)
					EELen = 256;
				else if(Adapter.EEType == EEPROM_93C46)
					EELen = 128;
				else if(Adapter.EEType == EEPROM_BOOT_EFUSE)
				{
					if (0)
						EELen = 256;
					else
						EELen = 512; // Efuse sizes of ICs after 88E are all 512 bytes.
				}
				else
					EELen = 128;	

				if(!bEEWiFiMode)
					EELen = 1024;

				ZeroMemory(editBuf, sizeof(editBuf));

				// <For OneByteAligned>
				if (bOneByteAlign)
				{
					for (i=0; i < EELen; i++)
					{
						_stprintf( editBuf2, TEXT("%02hX"), (BYTE)GetEEData[i] );

						lstrcat(editBuf, editBuf2);
						if ((i+1)%16 == 0) 
							lstrcat(editBuf, TEXT("\r\n"));
						else
							lstrcat(editBuf, TEXT(" "));
					}
				}
				else
				{
					for (i=0; i < EELen; i+=2)
					{
						_stprintf( editBuf2, TEXT("%02hX%02hX"), (BYTE)GetEEData[i+1], (BYTE)GetEEData[i] );

						lstrcat(editBuf, editBuf2);
						if ((i+1)%16 == 0) 
							lstrcat(editBuf, TEXT("\r\n"));
						else
							lstrcat(editBuf, TEXT(" "));
					}
				}
			    if (bOneByteAlign)
					m_CisData.SetFont(&EfuseFont_Small,TRUE);
				else
					m_CisData.SetFont(&EfuseFont,TRUE);
				m_CisData.FmtLines(TRUE);
				m_CisData.SetWindowText(editBuf);

				m_Result.SetWindowText(TEXT("Get EFUSE OK"));
			}
	    }
}

void CMP819xVCDlg::OnReadMacAddr()
{
    BOOL    bResult;
    int         i;
    BYTE    ReadMAC[6];
    TCHAR   MACBuf[20], editBuf[3];

    ZeroMemory(ReadMAC, sizeof(ReadMAC));
    bResult = ReadMACAddress(
                (LPADAPTER)(Adapter.hFile),
                (char*)&ReadMAC
                );
    if(!bResult)
    {
        m_Result.SetWindowText(TEXT("Get MAC Address FAIL"));
        ErrMsgBox(TEXT("Fail to Read MAC Address !!" ));
    }
    else
    {
        ZeroMemory(MACBuf, sizeof(MACBuf));
        for (i = 0; i < 6; i ++)
        {
            _stprintf( editBuf, TEXT("%02hX"), (BYTE)ReadMAC[i] );
            lstrcat(MACBuf, editBuf);
        }

        m_SelfMacAddr.SetWindowText(MACBuf);
        m_Result.SetWindowText(TEXT("Get MAC Address OK"));
    }
}

void CMP819xVCDlg::OnWriteMac()
{
    BOOL    bResult;
    int         i;
    UINT    strLen;
    UCHAR   CurByte;
    TCHAR   tmpBuf[INPUTLEN];
    BYTE    ReadMAC[6];

    strLen = m_SelfMacAddr.GetWindowText(tmpBuf, INPUTLEN);
    if(!CheckAdd(tmpBuf, 12) || strLen != 12)
    {
        m_Result.SetWindowText(TEXT("Wrong MAC Address"));
        ErrMsgBox(TEXT("Wrong MAC Address !!" ));
    }
    else
    {
        for(i=0;i<6;i++)
        {
            CurByte  = Asc2Hex(tmpBuf[2 * i]);
            CurByte <<= 4;
            CurByte += Asc2Hex(tmpBuf[2 * i + 1]);
            ReadMAC[i] = CurByte;
        }

        bResult = WriteMACAddress(
                    (LPADAPTER)(Adapter.hFile),
                    ReadMAC
                    );
        if(!bResult)
        {
            m_Result.SetWindowText(TEXT("Write MAC Address FAIL"));
            ErrMsgBox(TEXT("Fail to Write MAC Address !!" ));
        }
        else
        {
            m_Result.SetWindowText(TEXT("Write MAC Address OK"));
        }
    }
}

void CMP819xVCDlg::OnSetDestMac()
{
    int         i;
    UINT    strLen;
    UCHAR   CurByte;
    TCHAR   tmpBuf[INPUTLEN];

    strLen = m_DestMacAddr.GetWindowText(tmpBuf, INPUTLEN);
    if(!CheckAdd(tmpBuf, 12) || strLen != 12)
    {
        m_Result.SetWindowText(TEXT("Wrong MAC Address"));
        ErrMsgBox(TEXT("Wrong MAC Address !!" ));
    }
    else
    {
        for(i=0;i<6;i++)
        {
            CurByte  = Asc2Hex(tmpBuf[2 * i]);
            CurByte <<= 4;
            CurByte += Asc2Hex(tmpBuf[2*i+1]);
            UnicastDID[i]=CurByte;
        }

        m_Result.SetWindowText(TEXT("Set Dest MAC Address OK"));
    }
}

void CMP819xVCDlg::OnRegRead()
{
    BOOL    bResult;
    UINT    RegOffset, RegDataWidth;
    ULONG   RegDataValue;
    TCHAR   RegDataBuf[16];

	if ( RegReadByExpression(&m_RegOffset) )
		return;

    // Get offset and data width of the register to read.
    if( !GetValueFromControl(
            &m_RegOffset,
            2, // 2Byte
            (ULONG*)&RegOffset)
            )
    {
        m_Result.SetWindowText(TEXT("Register Offset Error"));
        return;
    }
    RegDataWidth = 4;   // Read 4 bytes

    // Read back MAC register.
    bResult = ReadRegData(
                (LPADAPTER)(Adapter.hFile),
                RegOffset,
                RegDataWidth,
                &RegDataValue
                );

    if(!bResult)
    {
        m_Result.SetWindowText(TEXT("Read Registry FAIL"));
        ErrMsgBox(TEXT("Fail to Read Registry !!" ));
    }
    else
    {

        // Show register value read.
        ZeroMemory(RegDataBuf, sizeof(RegDataBuf));
        switch(RegDataWidth)
        {
        case 1:
            _stprintf( RegDataBuf, TEXT("%02X"), RegDataValue );
            break;

        case 2:
            _stprintf( RegDataBuf, TEXT("%04X"), RegDataValue );
            break;

        case 3:
            _stprintf( RegDataBuf, TEXT("%06X"), RegDataValue );
            break;

        case 4:
            _stprintf( RegDataBuf, TEXT("%08X"), RegDataValue );
            break;

        default:
            _stprintf( RegDataBuf, TEXT("%X"), RegDataValue );
            break;
        }

        m_RegData.SetWindowText(RegDataBuf);
        m_Result.SetWindowText(TEXT("Read Registry OK"));
    }

}

void CMP819xVCDlg::OnRegWrite()
{
    BOOL    bResult;
    UINT    dataLen;
    UINT    RegOffset, RegDataWidth;
    ULONG   RegDataValue;
    char    regDataBuf[10];

	if (RegWriteByExpression())
		return;

    if( !GetValueFromControl(
            &m_RegOffset,
            2, // 2Byte
            (ULONG*)&RegOffset)
            )
    {
        m_Result.SetWindowText(TEXT("Register Offset Error"));
        return;
    }


    dataLen = m_RegData.GetWindowText(regDataBuf, 9);
    if( !GetValueFromControl(
            &m_RegData,
            4, // 4 Bytes
            &RegDataValue)
            )
    {
        m_Result.SetWindowText(TEXT("Register Data Error"));
        return;
    }
    RegDataWidth = (dataLen + 1) / 2;

    bResult = WriteRegData(
                (LPADAPTER)Adapter.hFile,
                RegOffset,
                RegDataWidth,
                RegDataValue
                );
    if(!bResult)
    {
        m_Result.SetWindowText(TEXT("Write Registry FAIL"));
        ErrMsgBox(TEXT("Fail to Write Registry !!" ));
    }
    else
    {
        m_Result.SetWindowText(TEXT("Write Registry OK"));
    }
}

//SYSON reg base address indirect access by CM4
#define SYSON_REG_BASE_ADDR_8710B		0x40000000
//WLON reg base address indirect access by CM4  
#define WLON_REG_BASE_ADDR_8710B			0x40080000

void CMP819xVCDlg::OnSysonRegRead()
{
    BOOL    bResult, bSysOnReg = TRUE;
    UINT    RegOffset, RegDataWidth;
    ULONG   RegDataValue;
    TCHAR   RegDataBuf[16];

	if ( RegReadByExpression(&m_RegOffset) )
		return;

    // Get offset and data width of the register to read.
    if( !GetValueFromControl(
            &m_RegOffset,
            2, // 2Byte
            (ULONG*)&RegOffset)
            )
    {
        m_Result.SetWindowText(TEXT("SysOn Register Offset Error"));
        return;
    }
	
    RegDataWidth = 4 ;   // Read 4 bytes

 	if((RegOffset & 0x00000003) != 0)
       {
       	  m_Result.SetWindowText(TEXT("Err: SysOn Reg must 4 byte align"));
		  ZeroMemory(RegDataBuf, sizeof(RegDataBuf));
		   _stprintf( RegDataBuf, TEXT("%X"), 0xFFFFFFFF );
		   m_RegData.SetWindowText(RegDataBuf);
       	 return;
    	}
	
    RegOffset  += SYSON_REG_BASE_ADDR_8710B; //indicate to Driver  the reg is syson reg
	
    // Read back MAC register.
    bResult = ReadRegData(
                (LPADAPTER)(Adapter.hFile),
                RegOffset,
                RegDataWidth ,
                &RegDataValue
                );

    if(!bResult)
    {
        m_Result.SetWindowText(TEXT("Read SysOn Registry FAIL"));
        ErrMsgBox(TEXT("Fail to Read SysOn Registry !!" ));
    }
    else
    {

        // Show register value read.
        ZeroMemory(RegDataBuf, sizeof(RegDataBuf));
        switch(RegDataWidth)
        {
        case 1:
            _stprintf( RegDataBuf, TEXT("%02X"), RegDataValue );
            break;

        case 2:
            _stprintf( RegDataBuf, TEXT("%04X"), RegDataValue );
            break;

        case 3:
            _stprintf( RegDataBuf, TEXT("%06X"), RegDataValue );
            break;

        case 4:
            _stprintf( RegDataBuf, TEXT("%08X"), RegDataValue );
            break;

        default:
            _stprintf( RegDataBuf, TEXT("%X"), RegDataValue );
            break;
        }

        m_RegData.SetWindowText(RegDataBuf);
        m_Result.SetWindowText(TEXT("Read SysOn Registry OK"));
    }

}
void CMP819xVCDlg::OnSysonRegWrite()
{
    BOOL    bResult, bSysOnReg = TRUE;
    UINT    dataLen;
    UINT    RegOffset, RegDataWidth;
    ULONG   RegDataValue;
    char    regDataBuf[10];

	if (RegWriteByExpression())
		return;

    if( !GetValueFromControl(
            &m_RegOffset,
            2, // 2Byte
            (ULONG*)&RegOffset)
            )
    {
        m_Result.SetWindowText(TEXT("SysOn Register Offset Error"));
        return;
    }


    dataLen = m_RegData.GetWindowText(regDataBuf, 9);
    if( !GetValueFromControl(
            &m_RegData,
            4, // 4 Bytes
            &RegDataValue)
            )
    {
        m_Result.SetWindowText(TEXT("SysOn Register Data Error"));
        return;
    }

	
  RegDataWidth = 4;// (dataLen + 1) / 2;
  
 	if((RegOffset & 0x00000003) != 0)
       {
       	  m_Result.SetWindowText(TEXT("Err: SysOn Reg must 4 byte align"));
       	 return;
    	}

    RegOffset  += SYSON_REG_BASE_ADDR_8710B; //indicate to Driver  the reg is syson reg
    
    bResult = WriteRegData(
                (LPADAPTER)Adapter.hFile,
                RegOffset,
                RegDataWidth,
                RegDataValue
                );
    if(!bResult)
    {
        m_Result.SetWindowText(TEXT("Write SysOn Registry FAIL"));
        ErrMsgBox(TEXT("Fail to Write SysOn Registry !!" ));
    }
    else
    {
        m_Result.SetWindowText(TEXT("Write SysOn Registry OK"));
    }
}


void CMP819xVCDlg::OnRfRegRead()
{
    BOOL    bResult;
    UINT    RegOffset, RegDataWidth;
    ULONG   RegDataValue;
    char    regDataBuf[16];
    UINT    rfPath;

    rfPath = m_RfRegRfPath.GetCurSel();

    // Get offset of RF register to read.
    if( !GetValueFromControl(
        &m_RegOffset,
        1, // We only care about 8-bit offset.
        (ULONG*)&RegOffset
        ))
    {
        m_Result.SetWindowText(TEXT("Wrong RF Offset!"));
        return;
    }

	RegDataWidth = (4|(rfPath<<4));

    // Read back RF register.
    bResult = RFReadRegData(
                (LPADAPTER)(Adapter.hFile),
                RegOffset,
                RegDataWidth,
                &RegDataValue
                );
    if(!bResult)
    {
        m_Result.SetWindowText(TEXT("Read RF Registry FAIL"));
        ErrMsgBox(TEXT("Fail to Read RF Registry !!" ));
    }
    else
    {
        ZeroMemory(regDataBuf, sizeof(regDataBuf));
        sprintf(regDataBuf, TEXT("%08X"), RegDataValue );
        m_RegData.SetWindowText(regDataBuf);
        m_Result.SetWindowText(TEXT("Read RF Registry OK"));
    }
}

void CMP819xVCDlg::OnRfRegWrite()
{
    BOOL    bResult;
    UINT    offsetLen, dataLen;
    UINT    RegOffset, RegDataWidth;
    ULONG   RegDataValue;
    char    regOffBuf[4], regDataBuf[10];
    UINT    rfPath;

    rfPath = m_RfRegRfPath.GetCurSel();
    
    offsetLen = m_RegOffset.GetWindowText(regOffBuf, 4);
    dataLen = m_RegData.GetWindowText(regDataBuf, 9);

    if( !GetValueFromControl(
            &m_RegOffset,
            1, // 2Byte
            (ULONG*)&RegOffset)
            )
    {
        m_Result.SetWindowText(TEXT("Register Offset Error"));
        return;
    }

    RegDataWidth = (dataLen + 1) / 2;

    if( !GetValueFromControl(
            &m_RegData,
            4, // 4 Bytes
            &RegDataValue)
            )
    {
        m_Result.SetWindowText(TEXT("Register Data Error"));
        return;
    }

    RegOffset |= (rfPath<<12);

    bResult = RFWriteRegData(
                (LPADAPTER)Adapter.hFile,
                RegOffset,
                RegDataWidth,
                RegDataValue
                );

    if(!bResult)
    {
        m_Result.SetWindowText(TEXT("Write RF Registry FAIL"));
        ErrMsgBox(TEXT("Fail to Write RF Registry !!" ));
    }
    else
    {
        m_Result.SetWindowText(TEXT("Write RF Registry OK"));
    }
}

void CMP819xVCDlg::OnReset()
{
    BOOL    bResult, bReturn;
    int     TxOK;
	int		RxOK, RxCRCErr;
    ULONG   ResetWait, ResetWaitMax, AckPktCount;
    char        strBuf[20];

    if (bInReset)
        return;
	
	MPStartTest((LPADAPTER)(Adapter.hFile), FALSE, TestItem);
	
    bReturn = TRUE;

    ResetWait = 0;
    ResetWaitMax = 500;

    bNeedReset = TRUE;
    do {
        Sleep(1);
        ResetWait += 1;
    }while((bInQueryRx) && (bInInitialGain) && (ResetWait < ResetWaitMax));
    bNeedReset = FALSE;
    bInReset = TRUE;
    bResult =   ResetRxPacketReceived(
                (LPADAPTER)Adapter.hFile
                );
    if(!bResult)
    {
        ErrMsgBox(TEXT("Fail to Reset Rx Packet Received !!" ));
        bReturn = FALSE;
    }

    bResult = ResetTxPacketSent(
                (LPADAPTER)Adapter.hFile
                );
    if(!bResult)
    {
        ErrMsgBox(TEXT("Fail to Reset Tx Packet Sent !!" ));
        bReturn = FALSE;
    }

    bResult = QueryTxPacketSent(
                (LPADAPTER)Adapter.hFile,
                &TxOK
                );
    if(!bResult)
    {
        ErrMsgBox(TEXT("Fail to Query Tx Packet Sent !!" ));
        bReturn = FALSE;
    }
    sprintf(strBuf, TEXT("%d"), TxOK);
    m_XmitOK.SetWindowText(strBuf);

	ResetAckCounter((LPADAPTER)Adapter.hFile);
	GetAckCounter((LPADAPTER)Adapter.hFile, &AckPktCount);
	_stprintf(strBuf, TEXT("%d"), AckPktCount);

	m_RxAckPktCounter.SetWindowText(strBuf);

    bResult = QueryRxPacketReceived(
                (LPADAPTER)Adapter.hFile,
                &RxOK
                );
    if(!bResult)
    {
        ErrMsgBox(TEXT("Fail to Query Rx Packet Received !!" ));
        bReturn = FALSE;
    }
    sprintf(strBuf, TEXT("%d"), RxOK);
    m_RcvOK.SetWindowText(strBuf);

    bResult = QueryRxPacketCRC32Error(
                (LPADAPTER)Adapter.hFile,
                &RxCRCErr
                );
    if(!bResult)
    {
        ErrMsgBox(TEXT("Fail to Query Rx Packet CRC32 Error !!" ));
        bReturn = FALSE;
    }
    sprintf(strBuf, TEXT("%d"), RxCRCErr);
    m_RcvErr.SetWindowText(strBuf);
	m_RcvPhyErr.SetWindowText("0");
	m_RcvPhyOK.SetWindowText("0");
	m_RcvMacErr.SetWindowText("0");
	m_RcvMacOK.SetWindowText("0");


    if(bReturn)
        m_Result.SetWindowText(TEXT("Reset Counters OK"));
    else
        m_Result.SetWindowText(TEXT("Reset Counters Fail"));
	
	//m_Result.SetTextColor(RGB(0,255,0));     
    bInReset = FALSE;
	MPStartTest((LPADAPTER)(Adapter.hFile), TRUE, TestItem);
}

void CMP819xVCDlg::OnHlp()
{
    CAboutDlg aDlg;
    aDlg.DoModal();
}

void CMP819xVCDlg::OnFlash()
{
	CFlashDlg flash_dlg;
	flash_dlg.DoModal();
}

BOOL CMP819xVCDlg::AutoInstallSwitch()
{
	CFlashDlg	flash_dlg;
	CHAR	on_off = 0;
	
	//Query current Autoinstall status before set
	if(false == flash_dlg.QueryAutoInstallStatusCap(RT_FLASH_AUTO_INSTALL, &on_off))
	{
		m_Result.SetWindowText(TEXT("Get Autoinstall Status Fail!"));
		goto Error;
	}
	
	if( RT_AUTO_INSTALL_OFF == on_off)//Current Autoinstall status is disabled
	{
		if (MessageBox(TEXT("Current AutoInstall status is disabled !!  Are you sure enable it ?"), TEXT("AutoInstall"), MB_OKCANCEL) != IDOK)
			goto Error;
		
		if (false == flash_dlg.SetAutoInstallStatusCap(RT_FLASH_AUTO_INSTALL, RT_AUTO_INSTALL_ON))
		{
			m_Result.SetWindowText(TEXT("Fail to enable Autoinstall!"));
			goto Error;
		}
	}
	else if( RT_AUTO_INSTALL_ON == on_off)//Current Autoinstall status is enable
	{
		if (MessageBox(TEXT("Current AutoInstall status is enabled !!   Are you sure disable it ?"), TEXT("AutoInstall"), MB_OKCANCEL) != IDOK)
			goto Error;
		
		if(false == flash_dlg.SetAutoInstallStatusCap(RT_FLASH_AUTO_INSTALL, RT_AUTO_INSTALL_OFF))
		{
			m_Result.SetWindowText(TEXT("Fail to disable Autoinstall!"));
			goto Error;
		}
	}
	else
		 goto Error;

	m_Result.SetWindowText(TEXT("Set Autoinstall Status OK!"));
	
	return TRUE;

Error:
	return FALSE;	
}

BOOL CMP819xVCDlg::FlashMediaType()
{
	CFlashDlg		flash_dlg;
	CHAR		media_type = 0;
	BOOL		bContinue = FALSE;
	
	//Query current Autoinstall status before set
	if(false == flash_dlg.QueryAutoInstallStatusCap(RT_FLASH_MEDIA_TYPE, &media_type))
	{
		m_Result.SetWindowText(TEXT("Get Flash Media Type Fail!"));
		goto Error;
	}
	
	if( FLASH_DEVICE_DISK == Adapter.flash_device_type)
	{
		if( RT_MEDIA_DISK == media_type)
		{
			if (MessageBox(TEXT("Current Flash Media Type is DISK !!  Do you want to change it to be CDROM ?"), TEXT("Media Type"), MB_OKCANCEL) != IDOK)
				bContinue = TRUE;
			
			if (FALSE == bContinue)
			{
				if(false == flash_dlg.SetAutoInstallStatusCap(RT_FLASH_MEDIA_TYPE, RT_MEDIA_CDROM))
				{
					m_Result.SetWindowText(TEXT("Fail to change Media Type to be CDROM!"));
					goto Error;
				}
				else
					m_Result.SetWindowText(TEXT("Set Media Type OK!"));
			}
		}
		else if( RT_MEDIA_CDROM == media_type)
		{
			if (MessageBox(TEXT("Current Flash Media Type is CDROM !!  Do you want to change it to be DISK ?"), TEXT("Media Type"), MB_OKCANCEL) != IDOK)
				goto Error;
			
			if(false == flash_dlg.SetAutoInstallStatusCap(RT_FLASH_MEDIA_TYPE, RT_MEDIA_DISK))
			{
				m_Result.SetWindowText(TEXT("Fail to change Media Type to be DISK!"));
				goto Error;
			}
			else
				m_Result.SetWindowText(TEXT("Set Media Type OK!"));

			bContinue = TRUE;
		}
		else
			 goto Error;
	}
	else
	{
		if( RT_MEDIA_DISK == media_type)
		{
			if(false == flash_dlg.SetAutoInstallStatusCap(RT_FLASH_MEDIA_TYPE, RT_MEDIA_CDROM))
			{
				m_Result.SetWindowText(TEXT("Fail to change Media Type to be CDROM!"));
				goto Error;
			}
			else
				m_Result.SetWindowText(TEXT("Set Media Type OK!"));
		}
	}

	//Set CDROM to DISK and shall check Read-Only or Read-Write
	if(TRUE == bContinue) 
		if( FALSE == FlashAccessCap() )
			goto Error;
	
	return TRUE;

Error:
	return FALSE;	
}

BOOL CMP819xVCDlg::FlashAccessCap()
{
	CFlashDlg	flash_dlg;
	CHAR	flash_cap = 0;
	
	//Query current Autoinstall status before set
	if(false == flash_dlg.QueryAutoInstallStatusCap(RT_FLASH_ACCESS_CAP, &flash_cap))
	{
		m_Result.SetWindowText(TEXT("Get Flash Capability Fail!"));
		goto Error;
	}
	
	if( RT_ACCESS_READ_ONLY == flash_cap)
	{
		if (MessageBox(TEXT("Current capability is Read-Only !!  Do you want to change it to be Read-Write ?"), TEXT("Flash Capability"), MB_OKCANCEL) != IDOK)
			goto Error;
		
		if (false == flash_dlg.SetAutoInstallStatusCap(RT_FLASH_ACCESS_CAP, RT_ACCESS_READ_WRITE))
		{
			m_Result.SetWindowText(TEXT("Fail to change capability to be Read-Write!"));
			goto Error;
		}
	}
	else if( RT_ACCESS_READ_WRITE == flash_cap)
	{
		if (MessageBox(TEXT("Current capability is Read-Write !!  Do you want to change it to be Read-Only ?"), TEXT("Flash Capability"), MB_OKCANCEL) != IDOK)
			goto Error;
		
		if(false == flash_dlg.SetAutoInstallStatusCap(RT_FLASH_ACCESS_CAP, RT_ACCESS_READ_ONLY))
		{
			m_Result.SetWindowText(TEXT("Fail to change capability to be Read-Only!"));
			goto Error;
		}
	}
	else
		 goto Error;

	m_Result.SetWindowText(TEXT("Set Flash Capability OK!"));
	
	return TRUE;

Error:
	return FALSE;	
}

void CMP819xVCDlg::OnEnableAutoinstall()
{
	TCHAR	strBuf[64];
	BOOL	result = FALSE;
	
	if( TRUE == AutoInstallSwitch() )
		result = TRUE;
	
	if( Adapter.HardwareId == HID_8821CU && TRUE == FlashMediaType() )
		result = TRUE;
	if( Adapter.HardwareId == HID_8731AU && TRUE == FlashMediaType() )
		result = TRUE;
	if( TRUE == result )
	{
		// Read Efuse Utilization
		DLL_ReadEFuseUtilization((LPADAPTER)(Adapter.hFile), (ULONG *)&EfuseUtilize);
		_stprintf(strBuf, TEXT("%d Bytes"), (EfuseUtilize&0xFFFF));
		m_Efuse_Utilize.SetWindowText(strBuf);
	}
}

void CMP819xVCDlg::OnQuit()
{
	//<20121127> DIG is necessary in Jaguar. We turn ON as default and a button for switching.
	if (IS_11AC_SERIES())
	{
		if (bDigON)
			SetDigOnOff((LPADAPTER)Adapter.hFile, FALSE);
	}

	// Disable Ack Counter on close
	EnableAckCounter((LPADAPTER)Adapter.hFile, FALSE);
	// (Reset Rx Filter) Accept All Packets
	SetRxFilter((LPADAPTER)Adapter.hFile, FALSE);

    OnClose();
}


void CMP819xVCDlg::OnTxPowerTrack() 
{
		TxPwrTrackSwitch=!TxPwrTrackSwitch;
		if(TxPwrTrackSwitch)
		{
			m_TxPowerTrack.SetWindowText(TEXT("TxPwrTrack Stop"));
		DLL_SwitchTxPwrTrack((LPADAPTER)(Adapter.hFile), 1);
		}
		else
		{
			m_TxPowerTrack.SetWindowText(TEXT("TxPwrTrack Start"));
		DLL_SwitchTxPwrTrack((LPADAPTER)(Adapter.hFile), 0);
		}

	if (FixIdxBaseWhenPwrTrack)
	{
		UINT	i, tmpAntennaTx;
		
		i = m_AntennaTx.GetCurSel();
		tmpAntennaTx = IndexValueConvert(ComboAntennaTx, Index2Value, i);
		
		m_CheckBox_PwrIndexFromEfuse.EnableWindow(AutoloadOK && !TxPwrTrackSwitch);
		SetTxPowerComboBox(tmpAntennaTx, !TxPwrTrackSwitch);
	}
}

void CMP819xVCDlg::OnUpdateEFUSE() 
{
	TCHAR   strBuf[100];

	if(bEEWiFiMode)
	{
		// We use Efuse shadow map in driver to update true EFUSE area.
		UpdateEFuse((LPADAPTER)(Adapter.hFile));
	}
	else
	{
		// We use Efuse shadow map in driver to update true BT EFUSE area.
		UpdateBTEFuse((LPADAPTER)(Adapter.hFile));	
		UpdateBTEFuseMAP((LPADAPTER)(Adapter.hFile));			
	}
	
	// Read Efuse Utilization
	DLL_ReadEFuseUtilization((LPADAPTER)(Adapter.hFile), (ULONG *)&EfuseUtilize);
	_stprintf(strBuf, TEXT("%d %"), (EfuseUtilize&0xFFFF));        		
	m_Efuse_Utilize.SetWindowText(strBuf);	
}

void CMP819xVCDlg::OnThermalREAD() 
{
	BOOL    bResult;
	int ThermalVal=0;
	char    regDataBuf[16];

	bResult = ReadRFThermalMeter((LPADAPTER)(Adapter.hFile), &ThermalVal);

	if(!bResult)
    {
        m_Result.SetWindowText(TEXT("Read RF ThermalMeter FAIL"));
        ErrMsgBox(TEXT("Fail to Read RF ThermalMeter !!" ));
    }
    else
    {
        ZeroMemory(regDataBuf, sizeof(regDataBuf));
        sprintf(regDataBuf, TEXT("%08X"), ThermalVal );
        m_RegData.SetWindowText(regDataBuf);
        m_Result.SetWindowText(TEXT("Read RF ThermalMeter OK"));
    }
}


void CMP819xVCDlg::OnMonitorClicked()
{
    UINT    RegOffset;
    TCHAR   RegDataBuf[16];
	
    if( !GetValueFromControl(&m_RegOffset, 2, (ULONG*)&RegOffset) )
    {
        m_Result.SetWindowText(TEXT("MAC/BB Register Offset Error"));
        return;
    }
	else
		_stprintf(RegDataBuf, TEXT("Reg 0x%X"), RegOffset);

	Adapter.bIsMonitorOn = ! Adapter.bIsMonitorOn;
	Adapter.MonitorAddr1 = RegOffset;
	
	if (Adapter.bIsMonitorOn) {
		m_MonitorText1.ShowWindow(TRUE);
		m_MonitorValue1.ShowWindow(TRUE);
		m_MonitorText1.SetWindowText(RegDataBuf);
		m_ButtonMonitor.SetWindowText(TEXT("- Monitor"));
		SetTimer(IO_MONITOR, 300, NULL);
	} else {
		m_MonitorText1.ShowWindow(FALSE);
		m_MonitorValue1.ShowWindow(FALSE);
		m_MonitorText1.SetWindowText(TEXT("Monitor Reg1"));
		m_ButtonMonitor.SetWindowText(TEXT("+ Monitor"));
		KillTimer(IO_MONITOR);		
	}
	
}

void CMP819xVCDlg::OnLed1Control() 
{
	{
		LED1Switch=!LED1Switch;
		if(LED1Switch)
		{
			m_Led1.SetWindowText(TEXT("Led1 OFF"));
			if (Adapter.HardwareId == HID_8188EU || Adapter.HardwareId == HID_8188ES || 
				Adapter.HardwareId == HID_8192EU||Adapter.HardwareId == HID_8814AU)
				OnLedControl(1, 1);
			else if(Adapter.HardwareId == HID_8821CU || Adapter.HardwareId == HID_8731AU)
			{
				OnLedControl(2, 1);
			}
			else
				OnLedControl(0, 1);
		}
		else
		{
			m_Led1.SetWindowText(TEXT("Led1 ON"));
			if (Adapter.HardwareId == HID_8188EU || Adapter.HardwareId == HID_8188ES ||
				Adapter.HardwareId == HID_8192EU ||Adapter.HardwareId == HID_8814AU)
				OnLedControl(1, 0);	
			else if(Adapter.HardwareId == HID_8821CU || Adapter.HardwareId == HID_8731AU)
			{
				OnLedControl(2, 0);
			}
			else
				OnLedControl(0, 0);
		}

		return;
	}
}



void CMP819xVCDlg::OnRFPathSwitch() 
{
	BOOL	bResult;
	CString Aux = "To Aux";
	CString Main = "To Main";
	
	if(IS_HARDWARE_SERIES(8192E) || IS_HARDWARE_SERIES(8812A) || IS_HARDWARE_SERIES(8703B) ||
		IS_HARDWARE_SERIES(8821A) || IS_HARDWARE_SERIES(8822B) || IS_HARDWARE_SERIES(8723D) ||
		IS_HARDWARE_SERIES(8192F)) {
		Aux = "To BT";
		Main = "To WIFI";
	}
	else if (IS_HARDWARE_SERIES(8821C) || IS_HARDWARE_8731AU)
	{ // Show current Ant Num for 8821C
		Aux = "Ant_1";
		Main = "Ant_2";

		if (RfPathSwitchMap == DONT_CARE)
		{
			m_RfPathSwitch.SetWindowText("N/A");
			return;
		}
		else if (RfPathSwitchMap == ONLY_ANT1)
		{
			m_RfPathSwitch.SetWindowText("Ant_1");
			return;
		}
		else if (RfPathSwitchMap == ONLY_ANT2)
		{
			m_RfPathSwitch.SetWindowText("Ant_2");
			return;
		}
	}
	if ( ! regAdapter.bIsComboCard) {
		Aux = "To Aux";
		Main = "To Main";
	}

	bMain =! bMain;
	if(bMain)
		m_RfPathSwitch.SetWindowText(Aux);
	else
		m_RfPathSwitch.SetWindowText(Main);

	bResult = SetRfPathSwitch(
		(LPADAPTER)(Adapter.hFile),
		bMain);
	
	if(!bResult)
	{
		m_Result.SetWindowText(TEXT("Switch RF path FAIL"));
		ErrMsgBox(TEXT("Fail to Switch Rf path !!" ));
	}
	else
	{
		m_Result.SetWindowText(TEXT("Switch RF path OK"));
	}
}

// BT
void CMP819xVCDlg::OnBt() 
{	
	BOOLEAN bResult;
	char PktLen[30];
	UINT	 PortNo = 1, BTRate = 115200; 	

#if 0
	char tmpIn[100], tmpOut[200];
	HAL_OperationEntry((LPADAPTER) Adapter.hFile, tmpIn, tmpOut);

#else
	if (IS_11AC_SERIES() || IS_HARDWARE_SERIES(8723B) || IS_HARDWARE_SERIES(8703B) || IS_HARDWARE_SERIES(8192E) || IS_HARDWARE_SERIES(8723D))
	{
		if(OpenBtUI()) //<20130412,VincentLan> Try to open BT UI
			do { PostMessage(WM_QUIT); } while(0);
	}
	else if (IS_HARDWARE_SERIES(8723A))
	{
		bBTTest = !bBTTest;

		if(bBTTest)
		{

			bResult = StopTest(
						(LPADAPTER) Adapter.hFile
						);	
		
			bResult = SetRFOnOff(		
				(LPADAPTER)(Adapter.hFile),
				RfOff);		

			if(!bResult)
			{
				m_Result.SetWindowText(TEXT("RF off FAIL"));
				ErrMsgBox(TEXT("Fail to set RF off!!" ));		
			}

			ItemEnDisable(FALSE, TRUE);
		
			if(Adapter.HardwareId == HID_8723AU || Adapter.HardwareId == HID_8723AE)
				bResult = _RTKBT_OpenAdapter(HOST_USB, 1, 0);
			else if(Adapter.HardwareId == HID_8723AS)
			{
		    	m_BT_Port_No_text.GetWindowText(PktLen, 10);
		    	PortNo = atoi(PktLen);					
		    	m_BT_Rate_text.GetWindowText(PktLen, 20);
		    	BTRate= atoi(PktLen);	
				bResult = _RTKBT_OpenAdapter(HOST_UART, PortNo, BTRate);
			}

			if(!bResult)
			{
				_stprintf(szDbgMsg, "open BT FAIL: %d", _RTKBT_GetLastError());
				m_Result.SetWindowText(szDbgMsg);
				ErrMsgBox(TEXT("Fail to open BT !! Please check if the port number is right." ));		
			}
			
			_RTKBT_ShowUI(TRUE);
			m_BT.SetWindowText(TEXT("BT Stop"));
		}
		else
		{
			bResult = SetRFOnOff(		
				(LPADAPTER)(Adapter.hFile),
				RfOn);		

			if(!bResult)
			{
				m_Result.SetWindowText(TEXT("RF on FAIL"));
				ErrMsgBox(TEXT("Fail to set RF on!!" ));		
			}		


			if(!bResult)
			{
				m_Result.SetWindowText(TEXT("START TEST FAIL"));
				ErrMsgBox(TEXT("Fail to START TEST!!" ));		
			}

			bResult = StartTest((LPADAPTER)Adapter.hFile);
			if (!_RTKBT_CloseAdapter())
			{
		        ErrMsgBox(TEXT("Fail to close BT adapter!!" ));
			}

			ItemEnDisable(TRUE, TRUE);
			m_BT.SetWindowText(TEXT("BT Test"));
		}
	}
#endif
}

void CMP819xVCDlg::OnBtWifiEfuse() 
{
	bEEWiFiMode = !bEEWiFiMode;
	m_WriteEEprom.EnableWindow(TRUE);
	m_UpdateEfuse.EnableWindow(TRUE);		

	if (bEEWiFiMode)
	{
		m_BT_WiFi.SetWindowText(TEXT("BT"));
		m_Result.SetWindowText(TEXT("Access WiFi EFUSE"));
	}
	else
	{
		m_BT_WiFi.SetWindowText(TEXT("WiFi"));		
		m_Result.SetWindowText(TEXT("Access BT EFUSE"));
	}
}


// ODM
void CMP819xVCDlg::OnIqk()
{
	TriggerIQK((LPADAPTER)(Adapter.hFile));
}

void CMP819xVCDlg::OnLck()
{
	TriggerLCK((LPADAPTER)(Adapter.hFile));
}

void CMP819xVCDlg::OnDIG()
{
	bDigON = ! bDigON;
	SetDigOnOff((LPADAPTER)(Adapter.hFile), bDigON);
	m_Button_DIG.SetWindowText((bDigON ? TEXT("OFF DIG") : TEXT("ON DIG")));
}

void CMP819xVCDlg::OnBnClickedButtonDPK()
{
	TriggerDPK((LPADAPTER) Adapter.hFile);
}

void CMP819xVCDlg::OnBnClickedButtonU2U3Switch()
{
	if (usbMode == USB2)
		usbMode = USB3;
	else
		usbMode = USB2;

	if (usbMode == USB2) {
		SetForcedUsbMode((LPADAPTER) Adapter.hFile, USB2);
		m_Button_U2U3.SetWindowText(TEXT("To USB3"));
	} else {
		SetForcedUsbMode((LPADAPTER) Adapter.hFile, USB3);
		m_Button_U2U3.SetWindowText(TEXT("To USB2"));
	}
}

void CMP819xVCDlg::OnBnClickedCheckEnableTxPwrLmt()
{
	if(m_CheckBox_EnableTxPowerLimit.GetCheck() != BST_CHECKED){
		m_CustomerSelPwrLmt.EnableWindow(FALSE);
		CMP819xVCDlg::OnInitCustomerPwrLmt();
	}else
		m_CustomerSelPwrLmt.EnableWindow(TRUE);
}

void CMP819xVCDlg::OnBnClickedCheckPwrIndexFromEfuse()
{
	UINT i, tmpAntennaTx;

	i = m_AntennaTx.GetCurSel();
	tmpAntennaTx = IndexValueConvert(ComboAntennaTx, Index2Value, i);
	PwrIndexFromEfuse = (m_CheckBox_PwrIndexFromEfuse.GetCheck() == BST_CHECKED);

	SetTxPowerComboBox(tmpAntennaTx, !PwrIndexFromEfuse);

	if (PwrIndexFromEfuse)
		UpdateTxPower(GetCurrChannelTxPower(), 0);
}

// Others
void CMP819xVCDlg::OnBnClickedStbc()
{
	UINT tmpChannel = Channels_2_4G_20M[0];
	if (IS_11AC_SERIES()) 
	{
		if (m_CheckBox_STBC.GetCheck())
			FillTxDesc_STBC((LPADAPTER) Adapter.hFile, 0x7F);
		else
			FillTxDesc_STBC((LPADAPTER) Adapter.hFile, 0);


		SwitchRateSet(Adapter.CurrentModulation, Adapter.CurrentBandWidth);
		OnSelchangeRate();
		if (Adapter.EnableAllRates != FALSE && !IS_HARDWARE_SERIES(8814A))//RfMode != RF_3T3R)
			ChangeAntenna();
	}
}

void CMP819xVCDlg::OnBnClickedLdpc()
{
	if (IS_11AC_SERIES()) 
	{
		if (m_CheckBox_LDPC.GetCheck())
			FillTxDesc_LDPC((LPADAPTER) Adapter.hFile, 0x7F);
		else
			FillTxDesc_LDPC((LPADAPTER) Adapter.hFile, 0);
	}
}

void CMP819xVCDlg::OnBnClickedDevBB()
{
	DevDlg dbg_dlg;

	CString	Str1, Str2, Str3;
	TCHAR	Buf[10];

	//"Band: "
	m_Modulation.GetWindowText(Buf, 10);
	Str1.Format("%s%s%s", Str2, "Band: ", Buf);
	Str2.Format("%s\r\n", Str1);
	
	//"Bandwidth: "
	m_Channel_Width.GetWindowText(Buf, 10);
	Str1.Format("%s%s%s", Str2, "Bandwidth: ", Buf);
	Str2.Format("%s\r\n", Str1);

	//"Channel: "
	m_Channel.GetWindowText(Buf, 10);
	Str1.Format("%s%s%s", Str2, "Channel: ", Buf);
	Str2.Format("%s\r\n", Str1);

	//"Rx Path: "
	m_AntennaRx.GetWindowText(Buf, 10);
	Str1.Format("%s%s%s", Str2, "Rx Path: ", Buf);
	Str2.Format("%s\r\n", Str1);
	
	// Store status to global string
	BBDev_Str.Format("%s", Str2);
	
	// Open BB Dev Dialog
	dbg_dlg.DoModal();

	// Reset string
	BBDev_Str.Format("");
	
	return;
}



void CMP819xVCDlg::OnBnClickedTxPmacMode()
{
	TCHAR Buf[50];
	CString str;
	bool bEnable = FALSE;

	m_TxPMAC_Switch.GetWindowText(Buf, 50);
	str.Format("%s",Buf);
	
	if (str == "SW Tx Packet Setting"){
		m_TxPMAC_Switch.SetWindowText("HW Tx Packet Setting");
		bEnable = TRUE;

		// Reset Length
		m_PacketLength.SetWindowText("1000");
		Adapter.PacketLength = 1000;
	}
	else
	{
		m_TxPMAC_Switch.SetWindowText("SW Tx Packet Setting");
		bEnable = FALSE;
		
		// Reset Length
		m_PacketLength.SetWindowText("1000");
		Adapter.PacketLength = 1000;

		BOOLEAN bResult = SetTMacPacketTx((LPADAPTER) Adapter.hFile);
		if(!bResult)
		{
			m_Result.SetWindowText(TEXT("Set TMAC Packet Tx  FAIL"));
			ErrMsgBox(TEXT("Fail to Set TMAC Packet Tx!!" ));
		}

	}

	ShowTxPMAC(bEnable);
}

void CMP819xVCDlg::OnBnClickedTxscHelpBtn()
{
	if (bInfoDlgExist == 0){
		InfoDlg *info = new InfoDlg(this);
		BOOL bResult = info->Create();
		info->ShowWindow(SW_SHOWNORMAL);  // 猔種: ﹚璶 showWindow 玥╭ぃㄓ
	}
}

void CMP819xVCDlg::OnBnClickedButtonAckCounterEn()
{
	bAckCounterEnable = ! bAckCounterEnable;
	if (bAckCounterEnable)
	{
		EnableAckCounter((LPADAPTER)Adapter.hFile, TRUE);
		SetTimer(UPDATE_ACK_COUNTER_TIMER, 100, NULL);
	}
	else
	{
		EnableAckCounter((LPADAPTER)Adapter.hFile, FALSE);
		KillTimer(UPDATE_ACK_COUNTER_TIMER);
	}

	m_RxAckPktCounter_Enable_Btn.SetWindowText((!bAckCounterEnable ? TEXT("OFF") : TEXT("ON")));
}

void CMP819xVCDlg::OnBnClickedRxfilterBtn()
{
	bRxFilterEnable = ! bRxFilterEnable;
	if (bRxFilterEnable)
	{
		SetRxFilter((LPADAPTER)Adapter.hFile, TRUE); // Accept Physical Match Packets Only
	}
	else
	{
		SetRxFilter((LPADAPTER)Adapter.hFile, FALSE); // Accept All Packets
	}

	m_RxFilter_Enable_Btn.SetWindowText((bRxFilterEnable ? TEXT("filter on") : TEXT("filter off")));
}

#pragma endregion ButtonClicked

#pragma region [Event]-Related Sub Functions

void CMP819xVCDlg::StartTesting(BOOL bStart)
{
	GetTXParameter();

    switch (TestItem)
    {
    // Start Testing None
    case NONE:
        m_Result.SetWindowText(TEXT("Nothing To Do"));
        bStart =! bStart;
        TestStop =! TestStop;
        break;

    // Start Testing Packets Tx
    case PACKETS_TX:
		if (QueryPmacMode() != TRUE)
			StartTesting_PacketsTx(bStart);		// SW Tx
		else
			StartTesting_PMacPacketTx(bStart);	// HW Tx
        break;

    // Start Testing Packets Rx
    case PACKETS_RX:
        StartTesting_PacketsRx(bStart);
        break;

    // Start Testing Continuous Tx
    case CONTINUOUS_TX:
		if (QueryPmacMode() != TRUE)
			StartTesting_ContinuousTx(bStart);	// SW Tx
		else
			StartTesting_PMacPacketTx(bStart);	// HW Tx
        break;

    // Start Testing OFDM Single Tone Tx
    case OFDM_Single_Tone_TX:
		if (QueryPmacMode() != TRUE)
			StartTesting_SingleToneTx(bStart);	// SW Tx
		else
			StartTesting_PMacPacketTx(bStart);	// HW Tx
        break;

    // Start Testing Carrier Suppression Tx
    case CCK_Carrier_Suppression_TX:
		if(! IS_RF_2G_20M(Adapter.CurrentModulation, Adapter.CurrentBandWidth)){
			m_Result.SetWindowText(TEXT("Only Supported in 2.4G 20M"));
			bStart =! bStart;
			TestStop =! TestStop;
			break;
		}
        StartTesting_CarrierSuppressionTx(bStart);
        break;
        
	case WPS_BUTTON_PUSH:
		StartTesting_WPSButtonPush(bStart);
		break;

	case HW_RF_ON_OFF:
		StartTesting_HWRfOnOff(bStart);
		break;		

    default:
        TestStop =! TestStop;
        m_Result.SetWindowText(TEXT("Not Implement Yet"));
        m_Start.SetWindowText(TEXT("Start Testing"));
        break;
    }

    if(bStart)
        m_Start.SetWindowText(TEXT("Stop Testing"));
    else
        m_Start.SetWindowText(TEXT("Start Testing"));
}

BOOL CMP819xVCDlg::QueryPmacMode()
{
	TCHAR Buf[50];
	CString str;

	m_TxPMAC_Switch.GetWindowText(Buf, 50);
	str.Format("%s",Buf);
	
	if (str == "SW Tx Packet Setting")
		return FALSE;
	else
		return TRUE;
}


BOOL CMP819xVCDlg::OpenBtUI()
{
	TCHAR strPath[ MAX_PATH ];
	CString Str1;

	// Get public destop path
	SHGetSpecialFolderPath(
							0,       // Hwnd
							strPath, // String buffer.
							CSIDL_COMMON_DESKTOPDIRECTORY, // CSLID of folder
							FALSE ); // Create if doesn't exists?
	
	Str1.Format("%s\\RTK BT %s.lnk", strPath, GetHwTypeName());
	

	// Open link to BT UI
	strcpy(strPath, (LPCTSTR)Str1);
	HINSTANCE hRet = ShellExecute(
								GetSafeHwnd(), //Parent window
								NULL,		  //Operation to perform
								strPath,      //Path to program
								NULL,         //Parameters
								NULL,         //Default directory
								SW_SHOW);     //How to open
	if((LONG)hRet <= 32)
	{
		MessageBox("Unable to start BT UI","Uh-oh..",MB_OK);
		return FALSE;
	}
	return TRUE;
}

CString CMP819xVCDlg::GetHwTypeName()
{
	//<20130412,VincentLan> Return NIC Hardware Type Name

	CString Hw;
	if (IS_HARDWARE_SERIES(8812A)){
		if (Adapter.HardwareId == HID_8812AU)
			Hw.Format("8812AU");
		else if (Adapter.HardwareId == HID_8812AE)
			Hw.Format("8812AE");
		else if (Adapter.HardwareId == HID_8812AS)
			Hw.Format("8812AS");
	}
	else if (IS_HARDWARE_SERIES(8814A)){
		if (Adapter.HardwareId == HID_8814AU)
			Hw.Format("8814AU");
		else if (Adapter.HardwareId == HID_8814AE)
			Hw.Format("8814AE");
		else if (Adapter.HardwareId == HID_8814AS)
			Hw.Format("8814AS");
	}	
	else if (IS_HARDWARE_SERIES(8821A)){
		if (Adapter.HardwareId == HID_8821AU)
			Hw.Format("8821AU");
		else if (Adapter.HardwareId == HID_8821AE)
			Hw.Format("8821AE");
		else if (Adapter.HardwareId == HID_8821AS)
			Hw.Format("8821AS");
	}
	else if (IS_HARDWARE_SERIES(8821B)){
		if (Adapter.HardwareId == HID_8821BU)
			Hw.Format("8821BU");
		else if (Adapter.HardwareId == HID_8821BE)
			Hw.Format("8821BE");
		else if (Adapter.HardwareId == HID_8821BS)
			Hw.Format("8821BS");
	}
	else if (IS_HARDWARE_SERIES(8723B)){
		if (Adapter.HardwareId == HID_8723BU)
			Hw.Format("8723BU");
		else if (Adapter.HardwareId == HID_8723BE)
			Hw.Format("8723BE");
		else if (Adapter.HardwareId == HID_8723BS)
			Hw.Format("8723BS");
	}
	else if (IS_HARDWARE_SERIES(8192E)){
		if (Adapter.HardwareId == HID_8192EU)
			Hw.Format("8192EU");
		else if (Adapter.HardwareId == HID_8192EE)
			Hw.Format("8192EE");
		else if (Adapter.HardwareId == HID_8192ES)
			Hw.Format("8192ES");
	}
	else if (IS_HARDWARE_SERIES(8723A)){
		if (Adapter.HardwareId == HID_8723AU)
			Hw.Format("8723AU");
		else if (Adapter.HardwareId == HID_8723AE)
			Hw.Format("8723AE");
		else if (Adapter.HardwareId == HID_8723AS)
			Hw.Format("8723AS");
	}
	else if (IS_HARDWARE_SERIES(8188E)){
		if (Adapter.HardwareId == HID_8188EU)
			Hw.Format("8188EU");
		else if (Adapter.HardwareId == HID_8188EE)
			Hw.Format("8188EE");
		else if (Adapter.HardwareId == HID_8188ES)
			Hw.Format("8188ES");
	}
	else if (IS_HARDWARE_SERIES(8723D)){
		if (Adapter.HardwareId == HID_8723DU)
			Hw.Format("8723DU");
		else if (Adapter.HardwareId == HID_8723DE)
			Hw.Format("8723DE");
		else if (Adapter.HardwareId == HID_8723DS)
			Hw.Format("8723DS");
	}	
	else if (IS_HARDWARE_SERIES(8192F)){
		if (Adapter.HardwareId == HID_8192FU)
			Hw.Format("8192FU");
		else if (Adapter.HardwareId == HID_8192FE)
			Hw.Format("8192FE");
		else if (Adapter.HardwareId == HID_8192FS)
			Hw.Format("8192FS");
	}	
	else {
		if (Adapter.HardwareId == HID_8185)
			Hw.Format("8185");
		else if (Adapter.HardwareId == HID_8185B)
			Hw.Format("8185B");
		else if (Adapter.HardwareId == HID_8187)
			Hw.Format("8187");
		else if (Adapter.HardwareId == HID_8187B)
			Hw.Format("8187B");
		else if (Adapter.HardwareId == HID_8187SU)
			Hw.Format("8187SU");
		else if (Adapter.HardwareId == HID_8187SE)
			Hw.Format("8187SE");
		else if (Adapter.HardwareId == HID_8190)
			Hw.Format("8190");
		// 8192
		else if (Adapter.HardwareId == HID_8192E)
			Hw.Format("8192E");
		else if (Adapter.HardwareId == HID_8192U)
			Hw.Format("8192U");
		else if (Adapter.HardwareId == HID_8192SE)
			Hw.Format("8192SE");
		else if (Adapter.HardwareId == HID_8192SU)
			Hw.Format("8192SU");
		else if (Adapter.HardwareId == HID_8192CE)
			Hw.Format("8192CE");
		else if (Adapter.HardwareId == HID_8192CU)
			Hw.Format("8192CU");
		else if (Adapter.HardwareId == HID_8192DE)
			Hw.Format("8192DE");
		else if (Adapter.HardwareId == HID_8192DU)
			Hw.Format("8192DU");
		else
			Hw.Format("");
	}
	
	return Hw;
}

void CMP819xVCDlg::ReloadTxSC_List()
{
	UINT i = 0;
	m_TxSC_Selection.ResetContent();
	if(Adapter.CurrentBandWidth == MPT_BW_20MHZ){
		for (i = 0; i < TxSC_20M_NUM; i++)
		{
			m_TxSC_Selection.AddString(TxSC_List_20M[i].TxSC_Str);
			m_TxSC_Selection.SetItemData(i, TxSC_List_20M[i].TxSC_Val);
		}
		// Set to "FULL"
		Adapter.TxSC = DATA_SC_DONOT_CARE;
		m_TxSC_Selection.SetCurSel(0);
	}
	else if(Adapter.CurrentBandWidth == MPT_BW_40MHZ){
		for (i = 0; i < TxSC_40M_NUM; i++)
		{
			m_TxSC_Selection.AddString(TxSC_List_40M[i].TxSC_Str);
			m_TxSC_Selection.SetItemData(i, TxSC_List_40M[i].TxSC_Val);
		}

		// Set to "FULL"
		Adapter.TxSC = DATA_SC_DONOT_CARE;
		m_TxSC_Selection.SetCurSel(0);
	}
	else if(Adapter.CurrentBandWidth == MPT_BW_80MHZ){
		for (i = 0; i < TxSC_80M_NUM; i++)
		{
			m_TxSC_Selection.AddString(TxSC_List_80M[i].TxSC_Str);
			m_TxSC_Selection.SetItemData(i, TxSC_List_80M[i].TxSC_Val);
		}

		// Set to "FULL"
		Adapter.TxSC = DATA_SC_DONOT_CARE;
		m_TxSC_Selection.SetCurSel(0);
	}
	// TODO: Add TxSC Update Status Function Call

}

void CMP819xVCDlg::ResetUIByRFSetting(ON_RF_CHANGE ChangedItem)
{
	// For all IC series now
	{
		switch(ChangedItem) 
		{
			UINT i;
			TCHAR   strBuf[100];

			case ON_CHANNEL_CHANGED:
				m_Channel.ResetContent();
				for(i = 0; i < Adapter.nTotalChnlCnt; ++i)
				{
				    _stprintf((TCHAR*)(strBuf), TEXT("%d"), Adapter.Channels[i]);
				    m_Channel.AddString(strBuf);
					m_Channel.SetItemData(i, Adapter.Channels[i]);
				}						
				break;

			case ON_RATE_CHANGED:				
				break;
			case ON_BANDWIDTH_CHANGED:				
				// Reset Channels
				m_Channel.ResetContent();
				for(i = 0; i < Adapter.nTotalChnlCnt; ++i)
				{
				    _stprintf((TCHAR*)(strBuf), TEXT("%d"), Adapter.Channels[i]);
				    m_Channel.AddString(strBuf);
					m_Channel.SetItemData(i, Adapter.Channels[i]);
				}			
				m_Channel.SetCurSel(0);

				// Reset TxSC (Since 8814A)
				if ( IS_HARDWARE_SERIES(8814A) ||
					((IS_11AC_SERIES())&&(regAdapter.iMode == CREATOR_MODE)))
				{
					ReloadTxSC_List();
				}
 			 	break;
				
			case ON_MODULATION_CHANGED:

				if(Adapter.CurrentModulation == WIRELESS_MODE_N_24G)
				{
					if(Adapter.CurrentBandWidth == MPT_BW_20MHZ)
					{
						Adapter.Channels = Channels_2_4G_20M;
						Adapter.nTotalChnlCnt = ChannelNum_2_4G_20M;
					}
					else if (Adapter.CurrentBandWidth == MPT_BW_40MHZ)
					{
						Adapter.Channels = Channels_2_4G_40M;
						Adapter.nTotalChnlCnt = ChannelNum_2_4G_40M;
					}
					
					// 1. delete all Bandwidth string
					m_Channel_Width.ResetContent();

					// 2. add desire Bandwidth string according to ini file
					for(UINT i=0 ; i<MAX_SUPPORT_BW_NUM ; i++)
					{
						if(1 == customBWsetting[0][i].bw_en)
						{
							m_Channel_Width.AddString(customBWsetting[0][i].bw_Str);
						}
					}
					
				}
				else if(Adapter.CurrentModulation == WIRELESS_MODE_N_5G || 
					    Adapter.CurrentModulation == WIRELESS_MODE_AC_5G)
				{
					if(Adapter.CurrentBandWidth == MPT_BW_20MHZ)
					{
						Adapter.Channels = Channels_5G_20M;
						Adapter.nTotalChnlCnt = ChannelNum_5G_20M;
					}
					else if (Adapter.CurrentBandWidth == MPT_BW_40MHZ)
					{
						Adapter.Channels = Channels_5G_40M;
						Adapter.nTotalChnlCnt = ChannelNum_5G_40M;		
					}
					else if (Adapter.CurrentBandWidth == MPT_BW_80MHZ)
					{
						Adapter.Channels = Channels_5G_80M;
						Adapter.nTotalChnlCnt = ChannelNum_5G_80M;		
					}

					// 1. delete all Bandwidth string
					m_Channel_Width.ResetContent();

					// 2. add desire Bandwidth string according to ini file
					for(UINT i=0 ; i<MAX_SUPPORT_BW_NUM ; i++)
					{
						if(1 == customBWsetting[1][i].bw_en)
						{
							m_Channel_Width.AddString(customBWsetting[1][i].bw_Str);
						}
					}

				}					
				else 
				{
					;
				}	
				m_Channel.ResetContent();
				for(i = 0; i < Adapter.nTotalChnlCnt; i++)
				{

					if(Adapter.CurrentModulation == WIRELESS_MODE_N_5G || 
						Adapter.CurrentModulation == WIRELESS_MODE_AC_5G)
					{
						if(Adapter.CurrentBandWidth == MPT_BW_20MHZ)
							_stprintf((TCHAR*)(strBuf), TEXT("%d"), Channels_5G_20M[i]);
						else if (Adapter.CurrentBandWidth == MPT_BW_40MHZ)
							_stprintf((TCHAR*)(strBuf), TEXT("%d"), Channels_5G_40M[i]);
						else if (Adapter.CurrentBandWidth == MPT_BW_80MHZ)
							_stprintf((TCHAR*)(strBuf), TEXT("%d"), Channels_5G_80M[i]);
					}		
					else
					{
						if(Adapter.CurrentBandWidth == MPT_BW_20MHZ)
							_stprintf((TCHAR*)(strBuf), TEXT("%d"), Channels_2_4G_20M[i]);
						else if (Adapter.CurrentBandWidth == MPT_BW_40MHZ)
							_stprintf((TCHAR*)(strBuf), TEXT("%d"), Channels_2_4G_40M[i]);
					}

					m_Channel.AddString(strBuf);
					m_Channel.SetItemData(i, Adapter.Channels[i]);
				}
				break;

			default:
				break;
		}
	}
}

void CMP819xVCDlg::OnLedControl(UINT LED_Num,UINT LED_Mode)
{
	BOOL    bResult;
	
	bResult = LEDControl(
    			(LPADAPTER)(Adapter.hFile),
    			LED_Num,
    			LED_Mode
   	 		);
	if(!bResult)
    {
        m_Result.SetWindowText(TEXT("Set Led Control FAIL"));
        ErrMsgBox(TEXT("Fail to Set Led Control !!" ));
    }
    else
    {
        m_Result.SetWindowText(TEXT("Set Led Control OK"));
    }
}

void CMP819xVCDlg::ShowTxPMAC(bool bShown)
{
	m_TxPMAC_TypeSel.ShowWindow(bShown);
	m_TxPMAC_Type_Label.ShowWindow(bShown);
	m_TxPMAC_Period.ShowWindow(bShown);
	m_TxPMAC_Period_Label.ShowWindow(bShown);
}

// パ Modeless Dialog call back 虏虫絛ㄒ
void CMP819xVCDlg::ModlessDlgReturn(void)
{
	bInfoDlgExist = 0;
}

#pragma endregion Related Sub Functions

#pragma region [Event]-Others

void CMP819xVCDlg::OnPaint()
{
	//  If you add a minimize button to your dialog, you will need the code below
	//  to draw the icon.  For MFC applications using the document/view model,
	//  this is automatically done for you by the framework.
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

HCURSOR CMP819xVCDlg::OnQueryDragIcon()
{
	//  The system calls this to obtain the cursor to display while the user drags
	//  the minimized window.
    return (HCURSOR) m_hIcon;
}

void CMP819xVCDlg::OnBnClickedKfree()
{
	ULONG nKfreeValue = 0xFF;
	char regVal[20];

	GetKfreeFromEfuse((LPADAPTER)Adapter.hFile,&nKfreeValue);
	
	_stprintf(regVal, TEXT("0x%02X"), nKfreeValue);
	m_KfreeValue.SetWindowText(regVal);
}

void CMP819xVCDlg::OnClose()
{
    BOOL    bResult;

    //if(MessageBox(TEXT("Press OK to Leave ?"),TEXT("Realtek"),MB_ICONQUESTION|MB_YESNO)!=IDYES)
    //   return;

    // Free the adapter resource.
    if (TestStop == FALSE)
    {
        TestStop =! TestStop;
        StartTesting(FALSE);
    }

    CloseHandle(G_xhThrd);

    bResult = StopTest(
                (LPADAPTER) Adapter.hFile
                );
    bResult = CloseDUT(
                (LPADAPTER) Adapter.hFile
                );
	
	if (Adapter.bIsMonitorOn) 
		KillTimer(IO_MONITOR);		
	

    DeInitAdapter2k();

    EndDialog(TRUE);
}

void CMP819xVCDlg::OnBnClickedEnhanceAntiInterference()
{

	if(m_CheckBox_EnhanceAntiInterference.GetCheck() == BST_CHECKED)
	{
		SetEnhanceAntiInterference( (LPADAPTER) Adapter.hFile,TRUE);
	}
	else
	{
		SetEnhanceAntiInterference( (LPADAPTER) Adapter.hFile,FALSE);
	}

}

void CMP819xVCDlg::OnBnClickedBtPinGet()
{
	ULONG val = 0;
	TCHAR valText[2] = {0};
	BOOL result = FALSE;

	result = GetBtPinValue((LPADAPTER) Adapter.hFile, &val);

	if(result == TRUE)
	{
		valText[0] = (val & 0x1) ? '1' : '0';
	}
	else
	{
		MessageBox(TEXT("Get Fail!!"));
	}
	
	m_BtPinValue.SetWindowText(valText);
}


void CMP819xVCDlg::OnBnClickedBtPinSet()
{
	TCHAR valText[3] = {0};
	ULONG val = 0;

	m_BtPinValue.GetWindowTextA(valText, 3);

	if (valText[1] != 0)
	{
		MessageBox(TEXT("Invalid Value. One Binary Needed."));
		return;
	}

	if (valText[0] == '1')
		val |= 0x1;
	else if (valText[0] != '0')
	{
		MessageBox(TEXT("Invalid Value. One Binary Needed."));
		return;
	}

	SetBtPinValue((LPADAPTER) Adapter.hFile, val);
}

#pragma endregion Others

#pragma endregion Events



void CMP819xVCDlg::OnBnClickedButton1()
{
	// TODO: 
	//ResetEvent(handlesynEvent);
	CString str;
	GetDlgItemText(IDC_EDIT1, str.GetBufferSetLength(255), 255);
	PSTR pszMessage = str.GetBufferSetLength(255);
	DWORD cbMessage = (strlen(pszMessage) + 1) * sizeof(*pszMessage);

	// Write the message to the view.
	memcpy(pView, pszMessage, cbMessage);
	SetEvent(handlesynEvent);
}


void CMP819xVCDlg::OnBnClickedBtnStartvsa()
{
	// TODO: 在此添加控件通知处理程序代码
	demo_multi_Vsa();
}

void CMP819xVCDlg::setAutoRange()
{
	vsa_demo->setAutorange();
}


bool demo_Multi_connect_tester(int *id)
{
	int ret = 0;
	int tmp_id = -1;
	if(!TesterIpaddress[0])
	{
		GetWindowTextA( gDlg->m_edit_ip.m_hWnd, TesterIpaddress, 32);
		//get_input(TesterIpaddress);
	}

	ret = WT_Connect(TesterIpaddress, &tmp_id);

	myprintf("Connect[%d] %s %s\n", tmp_id, TesterIpaddress, (ret == WT_ERR_CODE_OK ? "OK" : "Fail"));
	if(WT_ERR_CODE_OK == ret)
	{

		if(id)
		{
			*id = tmp_id;
			connect_id = tmp_id;
		}else{
			connect_id = tmp_id;
		}
		ret = WT_GetDefaultParameter(&vsaParam, &vsgParam);
	}
	return ret == WT_ERR_CODE_OK;
}
void demo_disconnect_tester(int id)
{

	WT_DisConnect(id);

	myprintf("DisConnect[%d] %s \n", id, TesterIpaddress);

	return ;
}

void CMP819xVCDlg::demo_multi_stopVSA()
{
	for(int i = 0; i < vsa_size; i++)
	{
		vsa_demo->stopCapture();

	}
}
double getFreq(int channel)
{
	double freq = 0;
	if(channel <14) freq =2412 + (channel-1)*5 ;
	if(channel== 14) freq = 2484;
    if(channel >= 36) freq =5180 + (channel-36)*5 ;
	return freq *1e6;
}
int CMP819xVCDlg::GetDemod()
{
	int demod;
	int		idx = m_Rate.GetCurSel();
	UINT	tmpRate = IndexValueConvert(ComboDataRate, Index2Value, idx);
	if(IS_CCK_RATE(tmpRate)) /* CCK rate */
	{
		/* A/B/G/N all support */
		demod = WT_DEMOD_11B;
		return demod;
	
	}
	else if(IS_OFDM_RATE(tmpRate))   /* OFDM rate */
	{
		/* only A/G/N support */
		demod = WT_DEMOD_11AG;
		return demod;
	}
	if(Adapter.CurrentModulation == WIRELESS_MODE_N_24G)
	{
		if(Adapter.CurrentBandWidth == MPT_BW_20MHZ)
		{
			demod = WT_DEMOD_11N_20M;
		}
		else if (Adapter.CurrentBandWidth == MPT_BW_40MHZ)
		{
			demod = WT_DEMOD_11N_40M;
		}			
	}
	else if(Adapter.CurrentModulation == WIRELESS_MODE_N_5G || 
		Adapter.CurrentModulation == WIRELESS_MODE_AC_5G)
	{
		if(Adapter.CurrentBandWidth == MPT_BW_20MHZ)
		{
			demod = WT_DEMOD_11AC_20M;
		}
		else if (Adapter.CurrentBandWidth == MPT_BW_40MHZ)
		{
			demod = WT_DEMOD_11AC_40M;
		}
		else if (Adapter.CurrentBandWidth == MPT_BW_80MHZ)
		{
			demod = WT_DEMOD_11AC_80M;	
		}
	}	
	return demod;
}
//FixMe
void CMP819xVCDlg::GetVsaParameter(VsaParameter *vsaParameter)
{
	UpdateData(TRUE);
#if 1
	vsaParameter->max_power = 0.0;
	
	double freq = 0;

	int i;
	UINT	tmpChannel;
	i = m_Channel.GetCurSel();
	tmpChannel = IndexValueConvert(ComboChannel, Index2Value, i);
	freq = getFreq(tmpChannel);
	//freq
	vsaParameter->freq = freq ;///
    int demod;
	demod = GetDemod();
	//WIFI standard
	vsaParameter->demod = demod;///
	// rf port
	//vsaParameter.rfPort = WT_PORT_OFF + rf_port;
	int smp_time = 2000;//m_smp_time;
	//sample time
	vsaParameter->smp_time = smp_time;///
	//timeout waiting
	vsaParameter->TimeoutWaiting = 8;
	//trigger pretime
	vsaParameter->trig_pretime = 20;

	double trig_level = -28;//m_trig_level; 
	//trigger level
	vsaParameter->trig_level = trig_level;///-28.0;
	int trig_type ;

	trig_type = WT_TRIG_TYPE_IF;
	//trigger type
	vsaParameter->trig_type = trig_type;///

	//trigger timeout.
	//if trig_timeout > TimeoutWaiting maybe cause WT_SetVSA_AutoRange return WT_ERR_CODE_TIMEOUT
	vsaParameter->trig_timeout = 2;///2
#endif 

}

bool CMP819xVCDlg::demo_multi_Vsa()
{
	int ret = 0;
	//const int vsa_size = get_thread_config();
	if(connect_id == -1) {
		AfxMessageBox("please connect to WT200");
		return false;
	}
	vsa_size = get_thread_config();
	if(!TesterIpaddress[0])
	{
		myprintf("Please input tester ip address\n>");
		//get_input(TesterIpaddress);
	}
	myprintf("\n============================\n");
	myprintf("Demo WT-208 WIFI VSA\n");
	myprintf("Demo WIFI VSG setting is:\n");
	myprintf("\tPower:-10dBm\n");
	myprintf("\tFreq:%lf MHz\n", g_wifi_freq / 1e6);
	myprintf("\tStandard 802.11ag OFDM 54M\n");
	myprintf("\tCable loss:%lf dB\n", g_external_gain);
	myprintf("\n============================\n");
	myprintf("VSG is already ? (Y/N)\n>");

	// 	if(get_Choose())
	// 	{
	//wt208_vsa **vsa_demo = new wt208_vsa *[vsa_size];
	//vsa_demo = new wt208_vsa *[vsa_size];
	//for(int i = 0; i < vsa_size; i++)
	//{
		//vsa_demo[i] = new wt208_vsa();
		//vsa_demo[i]->set_vsa_port(i + 1);
		vsa_demo->set_vsa_port(1);
		ret = vsa_demo->initParam(connect_id);
		if(!ret){
			AfxMessageBox("initParam failed");
		}
		Sleep(20);
		GetVsaParameter(&vsaThData.vsaParam);
		setAutoRange();
		vsa_demo->startCapture();

		//Sleep(get_connect_delta_config());
	//}
#if 0
	for(int i = 0; i < vsa_size; i++)
	{
		delete vsa_demo[i];
	}
	delete []vsa_demo;
#endif
	//	}

	return true;
}

int con_flag;

void CMP819xVCDlg::OnBnClickedBtnCon()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!con_flag){
		bool ret = demo_Multi_connect_tester(NULL);
		if(!ret){
			AfxMessageBox("Connect Failed!");
		}else{
			gDlg->GetDlgItem(IDC_BTN_CON)->SetWindowText("断开");
			con_flag = 1;
		}
	}else {
		demo_multi_stopVSA();
		demo_disconnect_tester(connect_id);
		gDlg->GetDlgItem(IDC_BTN_CON)->SetWindowText("连接");
		con_flag = 0;
	}
}


void CMP819xVCDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	vsgThreadId = vsa_demo->m_ThreadId;
	//if(vsgThreadId != NULL) PostThreadMessage(vsgThreadId, WM_QUIT, 0 , 0);
	Sleep(20);
	if(vsa_demo !=NULL){
		//for(int i = 0; i < vsa_size; i++)
		//{
			//if(vsa_demo != NULL)
			//	vsa_demo->destory();
			delete vsa_demo;
		//}
		//delete []vsa_demo;
	}
	WT_DLLTerminate();
}


LRESULT CMP819xVCDlg::OnUpdateCapData(WPARAM wParam, LPARAM lParam)
{
	m_redit1.SetWindowText(_T(""));

	m_redit1.SetSel(-1,-1);
	//WCHAR buf1[2048]={0};
	//mysprintf(buf1,"%s",vsaThData.buf);
	//m_redit1.ReplaceSel(buf1);
	m_redit1.ReplaceSel(vsaThData.buf);
	return 0;
}

int CMP819xVCDlg::PrintToMemo(char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vsnprintf(memoBuf,1024, fmt, argp);
	va_end(argp);

	m_redit1.SetSel(-1,-1);
	m_redit1.ReplaceSel(memoBuf);

	return 0;
}

static int asyn_vsg()
{
	int ret = WT_ERR_CODE_OK;
	int status = 0;
	int rf_port = get_rf_vsg_config();
	char wave_path[256] = {0};
	DWORD starttime = 0, endtime = 0;

	strcpy(wave_path, VSGWavePath);
	//strcat(wave_path,"\\wave\\120M\\54 Mbps(OFDM).csv");
	vsgParam.waveType = SIG_USERFILE;//gWaveType;//;
	vsgParam.freq = g_wifi_freq;
	vsgParam.power = g_txpower;//-10.0;
	vsgParam.repeat = 1000;
	vsgParam.rfPort = rf_port;
	vsgParam.wave = wave_path;
	vsgParam.wave_gap = 50;

	gDlg->PrintToMemo("Set asyn vsg parm\r\n");
	gDlg->PrintToMemo("\tVSG path:%s\r\n", vsgParam.wave);
	gDlg->PrintToMemo("\tVSG freq:%lf MHz\r\n", vsgParam.freq / 1e6);
	gDlg->PrintToMemo("\tVSG power:%lf\r\n", vsgParam.power);
	gDlg->PrintToMemo("\tVSG repeat:%d\r\n", vsgParam.repeat);
	gDlg->PrintToMemo("\tVSG rf port:RF%d\r\n", rf_port - WT_PORT_OFF);
	gDlg->PrintToMemo("\tVSG wave gap:%lf us\r\n", vsgParam.wave_gap);
	starttime = GetTickCount();
	ret = WT_SetVSG(connect_id, &vsgParam);
	endtime = GetTickCount();
	gDlg->PrintToMemo("WT_SetVSG:%d ms\r\n", endtime - starttime);

	if(WT_ERR_CODE_OK == ret)
	{

		starttime = GetTickCount();
		ret = WT_AsynStartVSG(connect_id);
		endtime = GetTickCount();
		gDlg->PrintToMemo("WT_AsynStartVSG:%d ms\r\n", endtime - starttime);
		if(WT_ERR_CODE_OK == ret)
		{
			is_vsg_start = true;
		}
		while(1)
		{
			ret = WT_GetVSGCurrentState(connect_id, &status);
			if(WT_ERR_CODE_OK == ret &&
				WT_VSG_STATE_RUNNING != status && WT_VSG_STATE_WAITING != status)
			{
				break;
			}
			if(WT_ERR_CODE_OK != ret)
			{
				break;
			}
			Sleep(100);
		}
	}
	else
	{
		gDlg->PrintToMemo("%s set VSG fail\r\n", __FUNCTION__);
	}
	is_vsg_start = false;
	return ret;
}

static DWORD WINAPI  ThreadVSGFun(LPVOID pM)
{

	int cycle_sum = 0;
	int ret = 0;
	is_vsg_stop = false;
	char buf[1024]={0};
	MSG msg;
	while(cycle_sum++ < MAX_DEMO_LOOP /*&& !bTerminateVsg*/)
	{
		//gDlg->ClearMemo();
		gDlg->m_redit1.SetWindowText("");
		sprintf(buf,"=======Loop %d==========\r\n", cycle_sum);
		memcpy(vsaThData.buf, buf, 1024);
		::PostMessage((HWND) gDlg->m_hWnd, WM_UPDATECAPDATA, (WPARAM) 0, (LPARAM) 0 );//数据处理完毕，通知主线程
#if 0
		ret = syn_vsg();
		if(WT_ERR_CODE_OK != ret)
		{
			break;
		}
		WT_StopVSG(connect_id);
		is_vsg_start = false;
#endif
		ret = asyn_vsg();
		if(WT_ERR_CODE_OK != ret)
		{
			break;
		}
		WT_StopVSG(connect_id);
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			switch(msg.message)
			{
			case WM_QUIT://退出从线程
				ExitThread(0);
			default:
				break;

			}
		}
	}
	WT_StopVSG(connect_id);
	is_vsg_stop = true;
	is_vsg_start = false;
	//bTerminateVsg = false;
	SetEvent(threadEvent);
	return ret;
}

bool CMP819xVCDlg::demo_vsg()
{
	myprintf("Demo WIFI VSG?(Y/N)\r\n>");

	if(connect_id == -1){
		AfxMessageBox("Please connect to WT200 first");
	    OnBnClickedBtnCon();
	}
	myprintf("Put 54 Mbps(OFDM).csv waveform in current path ?(Y/N)\r\n>");

	do
	{
		is_enable_vsg = true;
		get_exe_path(VSGWavePath);
		//strcat(VSGWavePath, "\\54 Mbps(OFDM).csv");
		strcat(VSGWavePath, "\\36 Mbps(OFDM).bwv");
		if(false == find_file(VSGWavePath))
		{
			myprintf("%s not find\n", VSGWavePath);
			break;
		}
		myprintf("\r\n============================\r\n");
		myprintf("Demo WIFI VSG setting is:\r\n");
		myprintf("\tPower:-10dBm\r\n");
		myprintf("\tFreq:%lf MHz\r\n", g_wifi_freq / 1e6);
		myprintf("\tCable loss:%lf dB\r\n", g_external_gain);
		myprintf("\tStandard 802.11ag OFDM 54M\r\n");
		myprintf("\tWaveform is %s\r\n", VSGWavePath);
		myprintf("\r\n============================\r\n");
		//创建内核事件对象(自动重置，未激活状态)
		threadEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
		handle_VSG = CreateThread(NULL, 0, ThreadVSGFun, NULL, 0, &vsgThreadId);
		//CWinThread *thread1 = AfxBeginThread(ThreadVSGFun,NULL,0,0,0,0);
		//handle_VSG = thread1->m_hThread;

	}
	while(0);


	//WT_DisConnect(connect_id);

	return true;
}

void CMP819xVCDlg::OnBnClickedBtnVsg()
{
	// TODO: 在此添加控件通知处理程序代码
	demo_vsg();
}
