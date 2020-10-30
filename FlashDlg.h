#pragma once

#include "RtlFunc.h"
#include "afxwin.h"
#include "afxcmn.h"

//8188GU definition
#define FLASH_TIMER   (0x0bda8710)
#define FLASH_FW_START_ADDRESS_8710BU	(0x0)
#define FLASH_ISO_LENGTH_RESERVED_8710BU   (0x1000)
#define FLASH_ISO_START_ADDRESS_8710BU	(0x10000)
#define AUTO_INSTALL_CONTROL_ELEMENT_8710BU	1
#define MEDIA_TYPE_CONTROL_ELEMENT_8710BU	0
#define ACCESS_CAP_CONTROL_ELEMENT_8710BU	0

//8821CU definition
#define FLASH_FW_START_ADDRESS_8821CU  (0x0)
#define FLASH_ISO_START_ADDRESS_8821CU  (0x30000)
#define AUTO_INSTALL_CONTROL_ELEMENT_8821CU	3
#define MEDIA_TYPE_CONTROL_ELEMENT_8821CU	1
#define ACCESS_CAP_CONTROL_ELEMENT_8821CU	1

//8192FU definition
#define FLASH_FW_START_ADDRESS_8192FU	(0x0)
#define FLASH_ISO_LENGTH_RESERVED_8192FU	(0x1000) 
#define FLASH_ISO_START_ADDRESS_8192FU	(0x11000) 
#define AUTO_INSTALL_CONTROL_ELEMENT_8192FU	1  
#define MEDIA_TYPE_CONTROL_ELEMENT_8192FU	0  
#define ACCESS_CAP_CONTROL_ELEMENT_8192FU	0

//General definition
#define FLASH_IMAGE_PATH  _T("\\Windows\\System32\\drivers\\")
#define FLASH_IMAGE_NAME  _T("rtflash.bin")

//Control list of Auto install
#define AUTO_INSTALL_CONTROL_ELEMENT(Hardware_ID) \
	((Hardware_ID == HID_8710BU) ? AUTO_INSTALL_CONTROL_ELEMENT_8710BU:\
	(Hardware_ID == HID_8821CU) ? AUTO_INSTALL_CONTROL_ELEMENT_8821CU:\
	(Hardware_ID == HID_8731AU) ? AUTO_INSTALL_CONTROL_ELEMENT_8821CU:\
	(Hardware_ID == HID_8192FU) ? AUTO_INSTALL_CONTROL_ELEMENT_8192FU:0)\

#define AUTO_INSTALL_CONTROL_LIST(Hardware_ID) \
	(Hardware_ID == HID_8710BU) ? auto_install_8710BU:\
	(Hardware_ID == HID_8821CU) ? auto_install_8821CU:\
	(Hardware_ID == HID_8731AU) ? auto_install_8821CU:\
	(Hardware_ID == HID_8192FU) ? auto_install_8192FU:NULL\
	
//Control list of media type
#define MEDIA_TYPE_CONTROL_ELEMENT(Hardware_ID) \
	((Hardware_ID == HID_8710BU) ? MEDIA_TYPE_CONTROL_ELEMENT_8710BU:\
	(Hardware_ID == HID_8821CU) ? MEDIA_TYPE_CONTROL_ELEMENT_8821CU:\
	(Hardware_ID == HID_8731AU) ? MEDIA_TYPE_CONTROL_ELEMENT_8821CU:\
	(Hardware_ID == HID_8192FU) ? MEDIA_TYPE_CONTROL_ELEMENT_8192FU:0)\

#define MEDIA_TYPE_CONTROL_LIST(Hardware_ID) \
	(Hardware_ID == HID_8710BU) ? media_type_8710BU:\
	(Hardware_ID == HID_8821CU) ? media_type_8821CU:\
	(Hardware_ID == HID_8731AU) ? media_type_8821CU:\
	(Hardware_ID == HID_8192FU) ? media_type_8192FU:NULL\
	
//Control list of access capability
#define ACCESS_CAP_CONTROL_ELEMENT(Hardware_ID) \
	((Hardware_ID == HID_8710BU) ? ACCESS_CAP_CONTROL_ELEMENT_8710BU:\
	(Hardware_ID == HID_8821CU) ? ACCESS_CAP_CONTROL_ELEMENT_8821CU:\
	(Hardware_ID == HID_8731AU) ? ACCESS_CAP_CONTROL_ELEMENT_8821CU:\
	(Hardware_ID == HID_8192FU) ? ACCESS_CAP_CONTROL_ELEMENT_8192FU:0)\

#define ACCESS_CAP_CONTROL_LIST(Hardware_ID) \
	(Hardware_ID == HID_8710BU) ? access_cap_8710BU:\
	(Hardware_ID == HID_8821CU) ? access_cap_8821CU:\
	(Hardware_ID == HID_8731AU) ? access_cap_8821CU:\
	(Hardware_ID == HID_8192FU) ? access_cap_8192FU:NULL\

#define Flash_FW_Start_Address(Hardware_ID) \
	((Hardware_ID == HID_8710BU) ? FLASH_FW_START_ADDRESS_8710BU:\
	(Hardware_ID == HID_8821CU) ? FLASH_FW_START_ADDRESS_8821CU:\
	(Hardware_ID == HID_8731AU) ? FLASH_FW_START_ADDRESS_8821CU:\
	(Hardware_ID == HID_8192FU) ? FLASH_FW_START_ADDRESS_8192FU:0)\
	
#define Flash_ISO_Start_Address(Hardware_ID) \
	((Hardware_ID == HID_8710BU) ? FLASH_ISO_START_ADDRESS_8710BU:\
	(Hardware_ID == HID_8821CU) ? FLASH_ISO_START_ADDRESS_8821CU:\
	(Hardware_ID == HID_8731AU) ? FLASH_ISO_START_ADDRESS_8821CU:\
	(Hardware_ID == HID_8192FU) ? FLASH_ISO_START_ADDRESS_8192FU:0)\
	
#define Flash_ISO_Content_Start_Address(Hardware_ID) \
	((Hardware_ID == HID_8710BU) ? (FLASH_ISO_START_ADDRESS_8710BU + FLASH_ISO_LENGTH_RESERVED_8710BU):\
	(Hardware_ID == HID_8821CU) ? FLASH_ISO_START_ADDRESS_8821CU:\
	(Hardware_ID == HID_8731AU) ? FLASH_ISO_START_ADDRESS_8821CU:\
	(Hardware_ID == HID_8192FU) ? (FLASH_ISO_START_ADDRESS_8192FU + FLASH_ISO_LENGTH_RESERVED_8192FU):0)\
	
#define Flash_Max_Image_Length(Hardware_ID) \
	(Adapter.flash_size - (Flash_ISO_Content_Start_Address(Hardware_ID)))

struct THREAD_PARAM {
    UCHAR	ucOpCode;
    UINT          uiFlashStartAddr;
    UINT          uiFlashLength;
    CHAR         *chImageFileName;
};

typedef struct _CONTROL_UNIT{
	ULONG	address;
	UCHAR	bit;
}CONTROL_UNIT, *PCONTROL_UNIT;

enum FLASH_OP_CODE{
	FLASH_DOWNLOAD 	= 1,
	FLASH_VERIFY		= 2,
	FLASH_ERASE		= 3,	
};

enum RT_FLASH_ATTRIBUTE{
	RT_FLASH_AUTO_INSTALL	= 1,
	RT_FLASH_MEDIA_TYPE		= 2,
	RT_FLASH_ACCESS_CAP		= 3,	
};

enum RT_AUTO_INSTALL_TYPE{
	RT_AUTO_INSTALL_OFF	= 0,
	RT_AUTO_INSTALL_ON	= 1,
};

enum RT_MEDIA_TYPE{
	RT_MEDIA_CDROM	= 0,
	RT_MEDIA_DISK	= 1,
};

enum RT_ACCESS_TYPE{
	RT_ACCESS_READ_ONLY	= 0,
	RT_ACCESS_READ_WRITE	= 1,
};

// CFlashDlg dialog
class CFlashDlg : public CDialog
{
	DECLARE_DYNAMIC(CFlashDlg)

public:
	CFlashDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFlashDlg();

// Dialog Data
	enum { IDD = IDD_FLASH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	UINT CFlashDlg::QueryControlBit(RT_FLASH_ATTRIBUTE attr, PCONTROL_UNIT *ppSwitchList);
	void CFlashDlg::WriteControlSetting(RT_FLASH_ATTRIBUTE attr, UCHAR flash_type, UCHAR bit, UCHAR *value);
	DECLARE_MESSAGE_MAP()
	
public:
	CButton m_FlashDownload;
	CButton m_FlashVerify;
	afx_msg void OnImageFileBrowse();
	CString m_ImageFilePath;

	afx_msg void OnFlashDownload();
	afx_msg void OnFlashVerify();
	BOOL CFlashDlg::QueryAutoInstallStatusCap(RT_FLASH_ATTRIBUTE attr, CHAR* Enable);
	BOOL CFlashDlg::SetAutoInstallStatusCap(RT_FLASH_ATTRIBUTE attr, UCHAR flash_type);
	static UINT FlashThread(LPVOID pParam) ;
	void DoEvent()  ;
	afx_msg void OnFlashErase();
	afx_msg void OnEraseLengthCheckHex();
	afx_msg void OnClose();

	CButton m_FlashErase;
	CButton m_FlashEraseAll;
	CButton m_FlashEraseISO;
	CEdit m_FlashEraseLength;
	CEdit m_FlashEraseStartAddr;
	CEdit m_FlashDLStartAddr;
	BOOL CopyImageFile(CHAR *chImageName);
	void DeleteCopyedFile(CHAR *chImageName);
	BOOL CheckImageValid(UINT ImageFileLength);
	BOOL CheckStartAddressVaild(UINT FlashLength, UINT StartAddr);
	BOOL GetFlashStartAddress(UINT ImageFileLength, UINT *DLStartAddr, UCHAR op_code);	
	BOOL DownloadFlashImageLength(UINT  pImageLength);
	CButton m_pImageLengthHex;
	CProgressCtrl m_FlashProgress;
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEraseAll();
	afx_msg void OnEraseIso();
	void ControlAllButton(bool Enable);
	void FlashProcess( THREAD_PARAM *ThreadParam);
	BOOL GetDestDriveName(CHAR *Drive);
};
