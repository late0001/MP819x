// FlashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MP819xVC.h"
#include "FlashDlg.h"
//#include "afxdialogex.h"

BOOL  bFlashOpInProgress = FALSE;
BOOL  bFlashOpFail = FALSE;

//8188GU definition
CONTROL_UNIT	auto_install_8710BU[1] = {{0x18, 6}}; //0x18[6]: 1: Enable; 0: Disable
CONTROL_UNIT	media_type_8710BU[1] = {}; //Not use for 8188GU
CONTROL_UNIT	access_cap_8710BU[1] = {}; //Not use  for 8188GU

//8821CU definition
CONTROL_UNIT	auto_install_8821CU[3] = {{0x8, 0}, {0x6, 4}, {0xC, 1}};
CONTROL_UNIT	media_type_8821CU[1] = {{0x1A0, 1}};
CONTROL_UNIT	access_cap_8821CU[1] = {{0x1A0, 2}};

//8192FU definition
CONTROL_UNIT	auto_install_8192FU[1] = {{0x0C, 1}}; //0xC[1]: 1: Enable; 0: Disable
CONTROL_UNIT	media_type_8192FU[1] = {}; //Not use for 8192FU???
CONTROL_UNIT	access_cap_8192FU[1] = {}; //Not use for 8192FU???

// CFlashDlg dialog
IMPLEMENT_DYNAMIC(CFlashDlg, CDialog)

CFlashDlg::CFlashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlashDlg::IDD, pParent)
{

}

CFlashDlg::~CFlashDlg()
{
}

void CFlashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FLASH_DOWNLOAD, m_FlashDownload);
	DDX_Control(pDX, IDC_FLASH_VERIFY, m_FlashVerify);
	DDX_Text(pDX, IDC_IMAGE_FILE_PATH, m_ImageFilePath);
	DDX_Control(pDX, IDC_FALSH_ERASE, m_FlashErase);
	DDX_Control(pDX, IDC_ERASE_LENGTH, m_FlashEraseLength);
	DDX_Control(pDX, IDC_ERASE_START_ADDR, m_FlashEraseStartAddr);
	DDX_Control(pDX, IDC_FLASH_DL_START_ADDR, m_FlashDLStartAddr);
	DDX_Control(pDX, IDC_FLASH_PROGRESS, m_FlashProgress);
	DDX_Control(pDX, IDC_ERASE_ALL, m_FlashEraseAll);
	DDX_Control(pDX, IDC_ERASE_ISO, m_FlashEraseISO);
}

UINT CFlashDlg::QueryControlBit(RT_FLASH_ATTRIBUTE attr, PCONTROL_UNIT *ppSwitchList)
{
	if( RT_FLASH_AUTO_INSTALL == attr )
	{
		*ppSwitchList = AUTO_INSTALL_CONTROL_LIST(Adapter.HardwareId);
		return AUTO_INSTALL_CONTROL_ELEMENT(Adapter.HardwareId);
	}
	else if( RT_FLASH_MEDIA_TYPE == attr )
	{
		*ppSwitchList = MEDIA_TYPE_CONTROL_LIST(Adapter.HardwareId);
		return MEDIA_TYPE_CONTROL_ELEMENT(Adapter.HardwareId);
	}
	else if( RT_FLASH_ACCESS_CAP == attr )
	{
		*ppSwitchList = ACCESS_CAP_CONTROL_LIST(Adapter.HardwareId);
		return ACCESS_CAP_CONTROL_ELEMENT(Adapter.HardwareId);
	}

	*ppSwitchList = NULL;
	return 0;
}

void CFlashDlg::WriteControlSetting(RT_FLASH_ATTRIBUTE attr, UCHAR flash_type, UCHAR bit, UCHAR *value)
{
	if( RT_FLASH_AUTO_INSTALL == attr )
	{
		if( RT_AUTO_INSTALL_ON == flash_type )
			*value |= (1 << bit);
		else if( RT_AUTO_INSTALL_OFF == flash_type )
			*value &= ~(1 << bit);
	}
	else if( RT_FLASH_MEDIA_TYPE == attr )
	{
		if( RT_MEDIA_DISK == flash_type )
			*value |= (1 << bit);
		else if( RT_MEDIA_CDROM == flash_type )
			*value &= ~(1 << bit);
	}
	else if( RT_FLASH_ACCESS_CAP == attr )
	{
		if( RT_ACCESS_READ_WRITE == flash_type )
			*value |= (1 << bit);
		else if( RT_ACCESS_READ_ONLY == flash_type )
			*value &= ~(1 << bit);
	}
}

BEGIN_MESSAGE_MAP(CFlashDlg, CDialog)
	ON_BN_CLICKED(IDC_IMAGE_FILE_BROWSE, OnImageFileBrowse)
	ON_BN_CLICKED(IDC_FLASH_DOWNLOAD, OnFlashDownload)
	ON_BN_CLICKED(IDC_FLASH_VERIFY, OnFlashVerify)
	ON_BN_CLICKED(IDC_FALSH_ERASE, OnFlashErase)
	ON_BN_CLICKED(IDC_ERASE_LENGTH_CHECK_HEX, OnEraseLengthCheckHex)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ERASE_ALL, OnEraseAll)
	ON_BN_CLICKED(IDC_ERASE_ISO, OnEraseIso)
END_MESSAGE_MAP()

// CFlashDlg message handlers
BOOL CFlashDlg::QueryAutoInstallStatusCap(RT_FLASH_ATTRIBUTE attr, CHAR* Enable)
{
	PCONTROL_UNIT	pSwitchList = NULL;
	UCHAR			EEValue = 0;
	int				bResult = 0, elements = 0, index = 0;
	ULONG			Offset = 0;

	//reload the map file from Efuse.
	bResult = UpdateEFuseMAP((LPADAPTER)(Adapter.hFile));
	if(bResult == 0)
	{
		Win32Print("QueryAutoInstallStatusCap(): Fail to UpdateEFuseMAP\n");
		MessageBox(TEXT("Fail to UpdateEFuseMAP."));
		goto Error;
	}
	
	elements = QueryControlBit(attr, &pSwitchList);
	if( 0 == elements )
	{
		Win32Print("QueryAutoInstallStatusCap(): Fail to fetch control bit in EFUSE.\n");
		MessageBox(TEXT("Fail to fetch control bit in EFUSE."));
		goto Error;
	}
	
	Win32Print("QueryAutoInstallStatusCap(): attr(%d) ,elements (%d)\n", attr, elements);
	for( index = 0; index < elements; index++)
	{
		Offset = pSwitchList[index].address;
		
		// Read value from EFUSE
		bResult = ReadEFuseByte((LPADAPTER)(Adapter.hFile), Offset,(UCHAR*)&EEValue);
		if(bResult == FALSE)
		{
			Win32Print("QueryAutoInstallStatusCap(): Fail to ReadEFuseByte Offset (0x%x)\n", Offset);
			MessageBox(TEXT("Waring: Fail to ReadEFuseByte"));
			goto Error;
		}
		
		Win32Print("QueryAutoInstallStatusCap(): ReadEFuseByte Offset (0x%x) = 0x%x\n", Offset,EEValue);
		if(EEValue == 0xFF)//0xFF means the Efuse byte is Empty, 
		{
			MessageBox(TEXT("Waring: The Efuse is Empty !! Please write the Efuse Manually"));
			goto Error;
		}

		if(EEValue & (1 << pSwitchList[index].bit))
			*Enable = 1;
		else
		{
			*Enable = 0;
			return TRUE;
		}
	}

	return TRUE;
	
Error:
	*Enable = 0;
	return FALSE;

}

BOOL CFlashDlg::SetAutoInstallStatusCap(RT_FLASH_ATTRIBUTE attr, UCHAR flash_type)
{
	PCONTROL_UNIT	pSwitchList = NULL;
	UCHAR			EEValue = 0,EnableBit = 0;
	int				bResult = 0, elements = 0, index = 0;
	ULONG			Offset = 0;

	//reload the map file from Efuse.
	bResult = UpdateEFuseMAP((LPADAPTER)(Adapter.hFile));
	if(bResult == 0)
	{
		Win32Print("SetAutoInstallStatusCap(): Fail to UpdateEFuseMAP\n");
		MessageBox(TEXT("Fail to UpdateEFuseMAP"));
		goto Error;
	}
	
	elements = QueryControlBit(attr, &pSwitchList);
	if( 0 == elements )
	{
		Win32Print("SetAutoInstallStatusCap(): Fail to fetch control bit in EFUSE.\n");
		MessageBox(TEXT("Fail to fetch control bit in EFUSE."));
		goto Error;
	}
	
	Win32Print("SetAutoInstallStatusCap(): attr(%d), elements (%d)\n", attr, elements);
	for( index = 0; index < elements; index++)
	{
		Offset = pSwitchList[index].address;
		
		// Read value from EFUSE
		bResult = ReadEFuseByte((LPADAPTER)(Adapter.hFile), Offset,(UCHAR*)&EEValue);
		if(bResult == FALSE)
		{
			Win32Print("SetAutoInstallStatusCap(): Fail to ReadEFuseByte Offset (0x%x)\n", Offset);
			MessageBox(TEXT("Fail to ReadEFuseByte"));
			goto Error;
		}
		
		Win32Print("SetAutoInstallStatusCap(): ReadEFuseByte Offset (0x%x) = 0x%x\n", Offset,EEValue);
		if(EEValue == 0xFF)//0xFF means the Efuse byte is Empty,
		{
			MessageBox(TEXT("Waring: The Efuse is Empty !! Please write the Efuse Manually" ));
			goto Error;
		}

		WriteControlSetting(attr, flash_type, pSwitchList[index].bit, &EEValue);
		Win32Print("SetAutoInstallStatusCap(): WriteEFuseByte Offset (0x%x) = 0x%x\n", Offset,EEValue);

		bResult = WriteEFuseByte((LPADAPTER)(Adapter.hFile), Offset, EEValue);
		if(bResult == FALSE)
		{
			Win32Print("SetAutoInstallStatusCap(): Fail to WriteEFuseByte Offset (0x%x) = 0x%x\n", Offset,EEValue);
			MessageBox(TEXT("Fail to WriteEFuseByte"));
			goto Error;
		}
	}
	
	//update the map file to Efuse.	
	bResult = UpdateEFuse((LPADAPTER)(Adapter.hFile));
	if(bResult == FALSE)
	{
		MessageBox(TEXT("Fail to Update EFUSE"));
		goto Error;
	}
	
	return TRUE;
Error:
	return FALSE;
}

BOOL CFlashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Init the Flash update button 
	m_FlashDownload.EnableWindow(false);
	m_FlashVerify.EnableWindow(false);
	//m_FlashErase.EnableWindow(false);

	m_FlashProgress.SetRange(0,100);
	m_FlashProgress.SetStep(1);

	((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT(""));
	((CButton*)GetDlgItem(IDC_ERASE_LENGTH_CHECK_HEX))->SetCheck(TRUE);

	if(Adapter.flash_mode == FLASH_USER_MODE)
	{
		//hide some button for user mode
		m_FlashVerify.ShowWindow(false);
		((CButton*)GetDlgItem(IDC_STATIC_FLASH_DL_ADDR))->ShowWindow(false);
		((CButton*)GetDlgItem(IDC_FLASH_DL_START_ADDR))->ShowWindow(false);
		
		if (IS_HARDWARE_SERIES(8710B))
			m_FlashEraseAll.ShowWindow(false);
		else if (IS_HARDWARE_SERIES(8821C) || IS_HARDWARE_8731AU)
			m_FlashEraseISO.ShowWindow(false);
		else if (IS_HARDWARE_SERIES(8192F))
			m_FlashEraseISO.ShowWindow(false);
		
		m_FlashErase.ShowWindow(false);
		m_FlashEraseStartAddr.ShowWindow(false);
		m_FlashEraseLength.ShowWindow(false);
		((CButton*)GetDlgItem(IDC_STATIC_ERASE_START_ADDR))->ShowWindow(false);
		((CButton*)GetDlgItem(IDC_STATIC_ERASE_LENGTH))->ShowWindow(false);
		((CButton*)GetDlgItem(IDC_ERASE_LENGTH_CHECK_HEX))->ShowWindow(false);
		((CButton*)GetDlgItem(IDC_STATIC_ERASE_UNIT))->ShowWindow(false);
	}
		
	return TRUE;
}

void CFlashDlg::OnImageFileBrowse()
{
	const char pszFilter[] = _T("Image File|*.iso;*.bin|All Files (*.*)|*.*||");
	//const char pszFilter[] = _T("All Files (*.*)|*.*||");
	//CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,pszFilter, this); //use this release Version will crash when ImageFileDLG->DoModal() 
	CFileDialog * ImageFileDLG = new CFileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY ,pszFilter, this);
	if(ImageFileDLG == NULL)
	{
		return;
	}
	
	//ImageFileDLG->m_ofn.lpstrInitialDir = FLASH_IMAGE_FILEPATH;
	//Init the Flash update button 
	m_FlashDownload.EnableWindow(false);
	m_FlashVerify.EnableWindow(false);
	if(ImageFileDLG->DoModal() == IDOK)
	{
		m_ImageFilePath = ImageFileDLG->GetPathName();
		UpdateData(false);		
		if(m_ImageFilePath)
		{
			CString csImageFilePath = m_ImageFilePath.Right(4);
			CHAR * chImageFilePath = csImageFilePath.GetBuffer(csImageFilePath.GetLength()); 
			if (_tcscmp(strlwr(chImageFilePath), _T(".iso")) == 0 || _tcscmp(strlwr(chImageFilePath), _T(".bin")) == 0)
			{
				m_FlashDownload.EnableWindow(true);
				m_FlashVerify.EnableWindow(true);
				//UpdateData(false);	
			}
	
			csImageFilePath.ReleaseBuffer();
		}
	}

	if(ImageFileDLG)
	{
		delete ImageFileDLG;
		ImageFileDLG = NULL;
	}
}

BOOL CFlashDlg::GetDestDriveName(CHAR *Drive)
{
	BOOL ret = FALSE;
	CHAR chdrive[_MAX_DRIVE] = "C";
	CHAR chdir[_MAX_DIR];
	CHAR chfname[_MAX_FNAME];
	CHAR chext[_MAX_EXT];
	CHAR chModuleFile[_MAX_PATH];
	
	DWORD dwSize = GetModuleFileName(NULL,chModuleFile,_MAX_PATH);
	chModuleFile[dwSize] = 0;
	Win32Print( "module file name: %s\n", chModuleFile);
	if(dwSize>4&&chModuleFile[dwSize-4]=='.')
	{
		_splitpath(chModuleFile,chdrive,chdir,chfname,chext);	
		ret =  TRUE;		
	}
	else
	{
		Win32Print( "Invalid module file name: %s\n", chModuleFile);
	}
	
	strcpy(Drive, chdrive);
	Win32Print( "Drive name: %s\n", Drive);
		
	return ret;
}



BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;

	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.
	fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress( GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

	if(NULL != fnIsWow64Process)
	{
	    if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
	    {
	        //handle error
	        Win32Print("IsWow64Process GetLastError()= 0x%x\n",GetLastError());
	    }
	}
	else
	{
	        Win32Print("GetProcAddress IsWow64Process GetLastError()= 0x%x\n",GetLastError());	
	}
	
	return bIsWow64;
}


BOOL CFlashDlg::CopyImageFile(CHAR *chImageName)
{
	BOOL			ret = FALSE, bCopy = FALSE, bX64 = IsWow64();
	int 			nFolderPos = 0, nFileNameCount=0;
	PVOID		OldValue = NULL;
	HINSTANCE	hlibrary; 
	typedef    int        (__stdcall  *  Wow64DisableWow64FsRedirection)(LPVOID); 
	Wow64DisableWow64FsRedirection  f_Wow64DisableWow64FsRedirection  =  NULL; 
	typedef    int        (__stdcall  *  Wow64RevertWow64FsRedirection)(LPVOID); 
	Wow64RevertWow64FsRedirection  f_Wow64RevertWow64FsRedirection  =  NULL;
	
	if(bX64 == TRUE)
	{
		hlibrary  =  LoadLibrary("Kernel32.dll"); 

		f_Wow64DisableWow64FsRedirection  =  (Wow64DisableWow64FsRedirection)GetProcAddress(hlibrary,"Wow64DisableWow64FsRedirection"); 
		if(!f_Wow64DisableWow64FsRedirection) {
			FreeLibrary(hlibrary);
			Win32Print("GetProcAddress Wow64DisableWow64FsRedirection GetLastError()= 0x%x\n",GetLastError());
			goto Error;;
		}

		f_Wow64RevertWow64FsRedirection  =  (Wow64RevertWow64FsRedirection)GetProcAddress(hlibrary,"Wow64RevertWow64FsRedirection"); 
		if(!f_Wow64RevertWow64FsRedirection) { 
			FreeLibrary(hlibrary);
			Win32Print("GetProcAddress Wow64RevertWow64FsRedirection GetLastError()= 0x%x\n",GetLastError());
			goto Error;
		} 	
		f_Wow64DisableWow64FsRedirection(&OldValue); 

	}	
	//copy file to "X:\\Windows\\System32\\drivers\\rtflash.bin"
	GetDestDriveName(chImageName);//The current system may be located in the D disk
	strcat(chImageName, FLASH_IMAGE_PATH);
	strcat(chImageName, FLASH_IMAGE_NAME);	

	Win32Print("m_ImageFilePath %s, chImageName %s!!\n",m_ImageFilePath,chImageName);

	bCopy = CopyFile(m_ImageFilePath, chImageName, FALSE);
	
	if(bX64 == TRUE)
	{
		f_Wow64RevertWow64FsRedirection(OldValue); 
		FreeLibrary(hlibrary);
	}
	
	if( bCopy == FALSE )
	{
		Win32Print("CopyFile GetLastError()= 0x%x\n",GetLastError());
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Image File Copy Fail!!" ));
		goto Error;
	}

	ret = TRUE;

Error:
	if(ret == FALSE)
		AfxMessageBox(TEXT("Fail to Get Flash Image File !! Please check the file path and name!If fail again, please put the file to 'C:\\Windows\\System32\\drivers\\rtflash.bin'" ));	
	return ret;
}

void CFlashDlg::DeleteCopyedFile(CHAR *chImageName)
{	
	if( 0 == DeleteFile(chImageName) )
	{
		Win32Print("DeleteCopyedFile(): GetLastError()= 0x%x\n",GetLastError());
	}
}

UINT CFlashDlg::FlashThread(LPVOID pParam)  
{  
	UINT ret = 0;
	THREAD_PARAM * pThreadParam  = (THREAD_PARAM *)pParam;

	if(pThreadParam == NULL)
		return 0;
	
	if(pThreadParam->ucOpCode == FLASH_DOWNLOAD || pThreadParam->ucOpCode == FLASH_VERIFY)
	{
		ret = UpdateFlashImage((LPADAPTER)(Adapter.hFile),pThreadParam->ucOpCode,pThreadParam->uiFlashStartAddr, pThreadParam->chImageFileName);
		if(ret != 1)
		{				
			bFlashOpFail = TRUE;
		}		
	}
	else if(pThreadParam->ucOpCode == FLASH_ERASE)
	{
		ret =EraseFlash((LPADAPTER)(Adapter.hFile),pThreadParam->uiFlashStartAddr, pThreadParam->uiFlashLength);
		if(ret != 1)
		{
			bFlashOpFail = TRUE;
		}
	}
	
	return ret;
}


void CFlashDlg::DoEvent()  
{  
    MSG msg;  
    if(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))   
    {  
        ::TranslateMessage(&msg);   
        ::DispatchMessage(&msg);  
    }  
}

void CFlashDlg::FlashProcess( THREAD_PARAM *ThreadParam)
{
	CWinThread* pThread = NULL;
	 DWORD dwRet = 0;
		
 	pThread = AfxBeginThread(FlashThread,ThreadParam);
	SetTimer(FLASH_TIMER, 1000, NULL); 

	bFlashOpInProgress = TRUE;
	DoEvent();  
	do   
	{  
		dwRet = ::MsgWaitForMultipleObjects(1, &pThread->m_hThread, FALSE, INFINITE, QS_ALLINPUT);  
		if (dwRet != WAIT_OBJECT_0)  
		{  
		    DoEvent();  
		}  
	} while ((dwRet != WAIT_OBJECT_0) && (dwRet != WAIT_FAILED)); 

	bFlashOpInProgress = FALSE;
	
	KillTimer(FLASH_TIMER);	

	if(bFlashOpFail == FALSE)
		m_FlashProgress.SetPos(100);
}

BOOL CFlashDlg::CheckImageValid(UINT ImageFileLength)
{
	char tempStr[100] = {0};

	//check the valid length
	if(ImageFileLength >  Flash_Max_Image_Length(Adapter.HardwareId) )//the downloading image include all bin (FW BIN and ISO)
	{
		if( ImageFileLength != (Adapter.flash_size - Flash_FW_Start_Address(Adapter.HardwareId)))
		{
			Win32Print("Error: ImageLength(0x%x) > MAX Image Length(0x%x Bytes)!!\n", ImageFileLength,Flash_Max_Image_Length(Adapter.HardwareId));
			 sprintf(tempStr, "Error: ImageFile Length(0x%x) > MAX Length(0x%x Bytes)!!\n", ImageFileLength,Flash_Max_Image_Length(Adapter.HardwareId));
        		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT(tempStr));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CFlashDlg::CheckStartAddressVaild(UINT FlashLength, UINT StartAddr)
{
	char tempStr[100] = {0};
	
	//check the Start Address is in Range?
	if((StartAddr & 0xFFF) != 0)
	{
		Win32Print("Error: StartAddr is 0x%x, Flash Start Address must 4K alignment !!!!\n", StartAddr);
		sprintf(tempStr, "Error: StartAddr is 0x%x, Flash Start Address must 4K alignment !!!!\n", StartAddr);
        	((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT(tempStr));
		return FALSE;
	}

	//check if the start address is over flash?
	if(StartAddr >= Adapter.flash_size)
	{
		Win32Print("Error: StartAddr(0x%x) > MAX Address(0x%x)!!\n", StartAddr,Adapter.flash_size);
		sprintf(tempStr, "Error: StartAddr(0x%x) > MAX Address(0x%x)!!\n", StartAddr,Adapter.flash_size);
        	((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT(tempStr));
		return FALSE;
	}
	
	//check if the total length is exceed flash size?
	if(StartAddr + FlashLength > Adapter.flash_size)
	{
		Win32Print("Error: FlashLength(0x%x Bytes) > MAX Length(0x%x Bytes) , Please the check file length !!\n",FlashLength,Adapter.flash_size -StartAddr);
		sprintf(tempStr, "Error: FlashLength(0x%x Bytes) > MAX Length(0x%x Bytes), Please the check file length !!\n",FlashLength,Adapter.flash_size -StartAddr);
        	((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT(tempStr));
		return FALSE;
	}
	
/*In User Mode  startaddress is fixed and user cannot modify it, so not need now*/
/*
	//Special case for 8188GU.
	if( (Adapter.flash_mode == FLASH_USER_MODE) && (Adapter.HardwareId == HID_8710BU))
	{
		if((!(StartAddr == 0 && FlashLength == Adapter.flash_size))  //download total size file (image + iso) 
			&& StartAddr < Flash_ISO_Start_Address(Adapter.HardwareId) )
		{
			Win32Print("Error: StartAddr(0x%x) is protected!!\n", StartAddr);
			sprintf(tempStr, "Error: StartAddr(0x%x) is protected!!\n", StartAddr);
        		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT(tempStr));
			return FALSE;
		}
	}
*/

	return TRUE;
}


BOOL CFlashDlg::GetFlashStartAddress(UINT ImageFileLength, UINT *pDLStartAddr, UCHAR op_code)
{
	BOOL bNeedDlISOLen = FALSE ;
	UINT    DLStartAddrLen = 0, DLStartAddr = 0;
	CHAR    chDLStartAddr[24] = {0};
	TCHAR    szFormatStr[10] = {0};
		
	if( FLASH_DEBUG_MODE == Adapter.flash_mode )
	{	
		DLStartAddrLen = m_FlashDLStartAddr.GetWindowTextA(chDLStartAddr, 20);
		// Translate offset to number.
		if(DLStartAddrLen == 0 || sscanf(chDLStartAddr, TEXT("%x"), &DLStartAddr) == 0)
		{
			((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Error: Fail to get Flash Start Address !!" ));
			return FALSE;
		}
		if(DLStartAddr ==  Flash_ISO_Content_Start_Address(Adapter.HardwareId))//the downloading image is ISO, in this case need down ISO length for 8188GU and 8192FU
			bNeedDlISOLen = TRUE;			
	}
	else if( FLASH_USER_MODE == Adapter.flash_mode )
	{
		if(ImageFileLength ==  (Adapter.flash_size - Flash_FW_Start_Address(Adapter.HardwareId)))//All BIN
		{
			//We have a tool to generate a binary include FW and ISO.
			//So start address is 0 if the file size is equal flash_size.
			DLStartAddr = Flash_FW_Start_Address(Adapter.HardwareId);
		}
		else  //other length only download ISO, in this case need down ISO length for 8188GU and 8192FU
		{
			DLStartAddr = Flash_ISO_Content_Start_Address(Adapter.HardwareId);
			bNeedDlISOLen = TRUE;
		}
	}
	Win32Print("GetFlashStartAddress() DLStartAddr=0x%x \n", DLStartAddr);
	
	//check the address
	if(FALSE == CheckStartAddressVaild(ImageFileLength, DLStartAddr))
		return FALSE;

	//Only for 8188GU need download ISO length before ISO content
	if(Adapter.HardwareId == HID_8710BU )
	{
		if( FLASH_DOWNLOAD == op_code && bNeedDlISOLen == TRUE )
		{
			if(false == DownloadFlashImageLength(ImageFileLength))
				return FALSE;
		}
	}

	*pDLStartAddr = DLStartAddr;

	return TRUE;
}

BOOL CFlashDlg::DownloadFlashImageLength(UINT ImageLength)
{
	Win32Print("ImageLength=0x%x startaddress = 0x%x\n", ImageLength,Flash_ISO_Start_Address(Adapter.HardwareId));

	//Image Length need set to flash 0x80000
	if(TRUE != WriteFlash((LPADAPTER)(Adapter.hFile),(UINT)Flash_ISO_Start_Address(Adapter.HardwareId), ImageLength))
	{
		Win32Print("Error: Fail to set Image Length!!\n");
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Error: Fail to set Image Length!!" ));
		return FALSE;
	}

	return TRUE;
}

void CFlashDlg::ControlAllButton(bool Enable)
{
	m_FlashDownload.EnableWindow(Enable);   
	m_FlashVerify.EnableWindow(Enable);
	m_FlashErase.EnableWindow(Enable);
	m_FlashEraseAll.EnableWindow(Enable);
	m_FlashEraseISO.EnableWindow(Enable);
}

void CFlashDlg::OnFlashDownload()
{
	 THREAD_PARAM	ThreadParam;
	CHAR			chImageFileName[MAX_PATH] = {0};
	UINT				DLStartAddr = 0, ImageLength = 0;
	CFile				cfile;
	
	ControlAllButton(false);
	 
	ThreadParam.ucOpCode = FLASH_DOWNLOAD;
			
	//get the file length
	Win32Print("The file path is %s, \n",m_ImageFilePath);
	if (cfile.Open(m_ImageFilePath, CFile::modeRead) != TRUE)
	{
		Win32Print("Fail to get the image file length, error is 0x%x\n", GetLastError());
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Error: Fail to open the image file!!" ));	
		goto Error;
	}
	ImageLength = cfile.GetLength();
	cfile.Close();
	
	//Get the Image name and copy 	
	if(CopyImageFile(chImageFileName) == FALSE)
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Error: Fail to get  the Image name!!" ));
		goto Error;
	}
	
	if(false == CheckImageValid(ImageLength))
		goto Error;

	if(false == GetFlashStartAddress(ImageLength, &DLStartAddr, ThreadParam.ucOpCode))
		goto Error;

	Win32Print("DLStartAddr = 0x%x, ImageLength = 0x%x, flash size is 0x%x\n", DLStartAddr, ImageLength,Adapter.flash_size);

 	ThreadParam.uiFlashStartAddr =  DLStartAddr;
	ThreadParam.chImageFileName =  FLASH_IMAGE_NAME;
	((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Download Flash is in progress! Please Wait....."));

	FlashProcess(&ThreadParam);	
	
	DeleteCopyedFile(chImageFileName);
	
	if(bFlashOpFail)
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Download Flash Fail!"));
		AfxMessageBox(TEXT("Error: Download Flash Fail!!!"));
	}
	else
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Download Flash Success!"));
		AfxMessageBox(TEXT("Download Flash Success!!!"),MB_ICONINFORMATION);
	}

Error:
	ControlAllButton(true);
}


void CFlashDlg::OnFlashVerify()
{
	THREAD_PARAM	ThreadParam;
	CHAR			chImageFileName[MAX_PATH] = {0};
	UINT				DLStartAddr = 0, ImageLength = 0;
	CFile				cfile;
	
	ControlAllButton(false);
	 
	ThreadParam.ucOpCode = FLASH_VERIFY;
	
	//get the file length
	Win32Print("The file path is %s, \n",m_ImageFilePath);
	if (cfile.Open(m_ImageFilePath, CFile::modeRead) != TRUE)    
	{    
		Win32Print("Fail to get the image file length, error is 0x%x\n", GetLastError());
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Error: Fail to open the image file!" ));	
		goto Error;	  	  
	} 
	ImageLength = cfile.GetLength();  
	cfile.Close();
	
	//Get the Image name and copy 		
	if(CopyImageFile(chImageFileName) == FALSE)
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Error: Get the Image name fail!!" ));
		goto Error;
	}
	
	if(false == CheckImageValid(ImageLength))
		goto Error;

	if(false == GetFlashStartAddress(ImageLength, &DLStartAddr, ThreadParam.ucOpCode))
		goto Error;

	Win32Print("DLStartAddr=0x%x, chImageFileName = %s\n", DLStartAddr, chImageFileName);

 	ThreadParam.uiFlashStartAddr =  DLStartAddr;
	ThreadParam.chImageFileName =  FLASH_IMAGE_NAME;
	
	((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Verify Flash is in progress! Please Wait......"));
	
	FlashProcess(&ThreadParam);

	DeleteCopyedFile(chImageFileName);

	if(bFlashOpFail)
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Verify Flash Fail!"));
		AfxMessageBox(TEXT("Error: Verify Flash Fail!!!"));
	}
	else	
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Verify Flash Success!"));
		AfxMessageBox(TEXT("Verify Flash Success!!!"),MB_ICONINFORMATION);		
	}

Error:
	 ControlAllButton(true);
}


void CFlashDlg::OnFlashErase()
{
	THREAD_PARAM	ThreadParam;
	int				ret = 0;
	UINT				EraseStartAddrLen = 0, EraseLengthLen = 0;
	UINT				EraseStartAddr = 0, EraseLength = 0;
	CHAR			szEraseStartAddr[10] = {0}, szEraseLength[10]= {0};
	TCHAR			szFormatStr[10] = {0};

	ControlAllButton(false);
	
	EraseLengthLen = m_FlashEraseLength.GetWindowText(szEraseLength, 9);
	if(((CButton*)GetDlgItem(IDC_ERASE_LENGTH_CHECK_HEX))->GetCheck())
		strcpy(szFormatStr,  TEXT("%x"));
	else
		strcpy(szFormatStr,  TEXT("%d"));
	
	// Translate offset to number.
	if( (EraseLengthLen == 0) || (sscanf(szEraseLength, szFormatStr, &EraseLength) == 0) )
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Error: Get Flash Erase Length Fail !!" ));
		goto Error;
	}

	EraseStartAddrLen = m_FlashEraseStartAddr.GetWindowText(szEraseStartAddr, 9);
	// Translate offset to number.
	if( (EraseStartAddrLen == 0) || (sscanf(szEraseStartAddr, TEXT("%X"), &EraseStartAddr) == 0) )
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Error:Get Flash Erase Start Address Fail !!" ));
		goto Error;
	}

	//check the address and length
	if(FALSE == CheckStartAddressVaild(EraseLength, EraseStartAddr))
	{
	  	goto Error;
	}

	Win32Print("EraseStartAddr = 0x%x, EraseLength=0x%x\n",EraseStartAddr,  EraseLength);

	if (MessageBox(TEXT("Waring: if erase flash, the device may not be recogined next time you insert!!  Are you sure erase it ?"), TEXT("Flash"), MB_OKCANCEL) != IDOK)
		goto Error;
	
	ThreadParam.ucOpCode = FLASH_ERASE;
	ThreadParam.uiFlashStartAddr = EraseStartAddr; 
	ThreadParam.uiFlashLength = EraseLength; 
	
	((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Erase Flash is in progress! Please Wait......"));

	FlashProcess(&ThreadParam);

	if(bFlashOpFail)
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Erase Flash Fail!"));
		AfxMessageBox(TEXT("Error: Erase Flash Fail!!!"));
	}
	else	
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Erase Flash Success!"));
		AfxMessageBox(TEXT("Erase Flash Success!!!"),MB_ICONINFORMATION);		
	}
	
Error:	
	ControlAllButton(true);
	m_FlashDownload.EnableWindow(false);   
	m_FlashVerify.EnableWindow(false);
}

void CFlashDlg::OnEraseAll()
{
	DWORD			dwRet = 0;
	THREAD_PARAM	ThreadParam;
	UINT				EraseStartAddr = 0, EraseLength = 0;

	ControlAllButton(false);

	if (MessageBox(TEXT("Waring: if erase All, the device cannot be recogined next time you insert!!  Are you sure erase all ?"), TEXT("Flash"), MB_OKCANCEL) != IDOK)
		goto Error;

	EraseStartAddr = Flash_FW_Start_Address(Adapter.HardwareId);
	EraseLength = Adapter.flash_size - EraseStartAddr;
	
	ThreadParam.ucOpCode = FLASH_ERASE;
	ThreadParam.uiFlashStartAddr = EraseStartAddr;
	ThreadParam.uiFlashLength = EraseLength;
	
	((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Erase All is in progress! Please Wait......"));

	FlashProcess(&ThreadParam);

	if(bFlashOpFail)
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Erase All Fail!"));
		AfxMessageBox(TEXT("Error: Erase All Fail!!!"));
	}
	else
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Erase All Success!"));
		AfxMessageBox(TEXT("Erase All Success!!!"),MB_ICONINFORMATION);
	}
	
Error:	
	ControlAllButton(true);
	m_FlashDownload.EnableWindow(false);   
	m_FlashVerify.EnableWindow(false);
}


void CFlashDlg::OnEraseIso()
{
	 DWORD dwRet = 0;
	 THREAD_PARAM ThreadParam;
    	UINT    EraseStartAddr = 0, EraseLength = 0;

	ControlAllButton(false);
	
	if (MessageBox(TEXT("Waring: if erase ISO, the wifi driver cannot be autoinstalled next time!!  Are you sure erase ISO ?"), TEXT("Flash"), MB_OKCANCEL) != IDOK)
		goto Error;
	
	EraseStartAddr = Flash_ISO_Start_Address(Adapter.HardwareId);
	EraseLength = Adapter.flash_size - Flash_ISO_Start_Address(Adapter.HardwareId);
		
	ThreadParam.ucOpCode = FLASH_ERASE;
	ThreadParam.uiFlashStartAddr = EraseStartAddr; 
	ThreadParam.uiFlashLength = EraseLength; 
	
	((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Erase ISO is in progress! Please Wait......"));

	FlashProcess(&ThreadParam);

	if(bFlashOpFail)
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Erase ISO Fail!"));
		AfxMessageBox(TEXT("Error: Erase ISO Fail!!!"));
	}
	else	
	{
		((CButton*)GetDlgItem(IDC_FLASH_STATUS))->SetWindowText(TEXT("Erase ISO Success!"));
		AfxMessageBox(TEXT("Erase ISO Success!!!"),MB_ICONINFORMATION);		
	}
	
Error:	
	ControlAllButton(true);
	m_FlashDownload.EnableWindow(false);   
	m_FlashVerify.EnableWindow(false);
}


void CFlashDlg::OnEraseLengthCheckHex()
{
	if(((CButton*)GetDlgItem(IDC_ERASE_LENGTH_CHECK_HEX))->GetCheck())
		((CButton*)GetDlgItem(IDC_STATIC_ERASE_LENGTH))->SetWindowText(TEXT("Erase Length:0x" ));
	else 
		((CButton*)GetDlgItem(IDC_STATIC_ERASE_LENGTH))->SetWindowText(TEXT("Erase Length: " ));

}

void CFlashDlg::OnClose()
{
	if(bFlashOpInProgress == TRUE)
		AfxMessageBox(TEXT("Flash Operation is in progress !!   Can not close the dialog!!     Please wait..." ));
	else
		CDialog::OnClose();
}

void CFlashDlg::OnTimer(UINT_PTR nIDEvent)
{
	UINT FlashDLProgress = 0;
	if(FLASH_TIMER == nIDEvent)
	{
		if(QueryFlashProgress((LPADAPTER)(Adapter.hFile),&FlashDLProgress))	
			m_FlashProgress.SetPos(FlashDLProgress);
	}
	
	CDialog::OnTimer(nIDEvent);

}
