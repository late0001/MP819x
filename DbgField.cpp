// DbgField.cpp : implementation file
//

#include "stdafx.h"
#include "mp819xvc.h"
#include "Dbg.h"
#include "DbgField.h"
#include "RtlFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMD_ENTRY commonTable[]={
    {
        DbgOpCommon,
        DbgCommonMode,
        "mode",
        "Debug mode switch (firmware mode/driver mode).",
        "syntax : mode <mode> \
         \n\r mode   : firm/driver"
    },
	{
        DbgOpCommon,
        DbgCommonCommand,
        "cmd",
        "Debug general command send.",
        "syntax : cmd <cmdindex> <arg1> <arg2>..<argn>"
    },
    {
        DbgOpCommon,
        DbgCommonStatistics,
        "sta",
        "MP UI counters statistics.",
        "syntax : sta <type> \
        \n\r type	: disp/clr"
    },
    {
        DbgOpCommon,
        DbgCommonChnlTxPower,
        "power",
        "MP UI Tx power index per channel display.",
        "syntax : power"
    },
    {
        DbgOpEnd,
        NULL,
        "", 
        "", 
        ""
    }
};
CMD_ENTRY *cmd_table[DbgOpMax] =
{
    &driverTable[0],
    &firmTable[0],
    &commonTable[0]
};

/////////////////////////////////////////////////////////////////////////////
// DbgField dialog


DbgField::DbgField(CWnd* pParent /*=NULL*/)
	: CDialog(DbgField::IDD, pParent)
{
	//{{AFX_DATA_INIT(DbgField)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_REALTEK_ICO);
}


void DbgField::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DbgField)
	DDX_Control(pDX, IDC_CMD_RESULT, m_CmdResult);
	DDX_Control(pDX, IDC_CMD_LINE, m_CmdLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DbgField, CDialog)
	//{{AFX_MSG_MAP(DbgField)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DbgField message handlers
BOOL DbgField::OnInitDialog()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);         // Set big icon
    //SetIcon(m_hIcon, FALSE);        // Set small icon
	//CFont *pfont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
    
    CString Title = "Realtek Debug Field Mode (Driver Mode)";
    SetWindowText(Title);
    InitFieldData();
    //m_CmdResult.SetFont(pfont);
	return TRUE;
}

BOOL DbgField::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd = GetFocus();
	if(pWnd != NULL)
	{
		if(pWnd == GetDlgItem(IDC_CMD_LINE))
		{
			if(pMsg->message == WM_CHAR || pMsg->message == WM_KEYDOWN)
			{
			    if(ProcessInChar(pMsg->message, pMsg->wParam))
	                return TRUE;
			}
		}
	}
	m_CmdLine.SetFocus();
	return CDialog::PreTranslateMessage(pMsg);
}

#define MaxOutputStrBufferSize  1024
#define MaxOutLength			0x7500
void DbgField::ResultAppendText(LPCSTR pText)
{
    int nLen, sLen;
	char *ptr_pos;
	char cpyStr[MaxOutputStrBufferSize];

    sLen = strlen(pText);
    if(sLen >= MaxOutputStrBufferSize)
        strcpy(cpyStr, "No enough buffer!");
	else
		strcpy(cpyStr, pText);

    nLen=m_CmdResult.GetWindowTextLength();
    if(nLen >= MaxOutLength)
    {
    	m_CmdResult.SetWindowText("");
    	nLen=m_CmdResult.GetWindowTextLength();
    }
    m_CmdResult.SetFocus();
    m_CmdResult.SetSel(nLen, nLen);
    
    //overwrite "\n\r" to "\r\n"
	ptr_pos = cpyStr;
    while((ptr_pos=strstr(ptr_pos, "\n\r")) != NULL)
	{
		if(*(ptr_pos-1) != '\r')	//avoid continuous \r\n\r\n..
			memcpy(ptr_pos, "\r\n", 2);
		else
			ptr_pos+=2;
	}
		
    m_CmdResult.ReplaceSel(cpyStr);

	//if(m_CmdResult.GetLineCount() >= 0x3c0)
	//	m_CmdResult.SetWindowText("");
}

BOOL DbgField::ProcessInChar(UINT message, WPARAM inVal)
{
    switch(inVal)
    {
		case 0x0a:      /* '\r': */
		case 0x0d:      /* '\n': */
			if(parseEnterCMD())
			{
				INC_CurrCMD;
				INC_TotalCMD;
			}
			FieldDATA.HistoryIndex = FieldDATA.CurrCMD;
			m_CmdLine.SetWindowText("");
			ResultAppendText("\r\n");
			return TRUE;
			
	    case 0x20:      /* ' ': */
	        if(message != WM_CHAR)
	            break;
	        else
	            return(parseSpaceCMD());
	            
	    case 0x3f:      /* '?': */
			return(parseHelpCMD());
			
		case VK_DOWN:
		case VK_UP:
		    parseArrowKey(inVal);
		    return TRUE;
    }
    return FALSE;
}

void  DbgField::InitFieldData()
{    
    FieldDATA.cmd = cmd_table[DbgOpDriver];
    FieldDATA.dbg_mode = DbgOpDriver;
    FieldDATA.CurrCMD = 0;
    FieldDATA.TotalCMD = 0;
    FieldDATA.HistoryIndex = 0;
	FieldDATA.Prompt = _T("RTL8190 > ");
}

BOOL DbgField::dbgCommonCMD(CMD_ENTRY *cmd)
{
	char token[MaxTokenNum][MaxDbgArgvLen+1];
	
	switch(cmd->CmdIndex)
	{
	case DbgCommonMode:
		strncpy(&token[0][0], &FieldDATA.intfEntry.sendData.dbgArgv[0], MaxDbgArgvLen);
		token[0][MaxDbgArgvLen] = 0;
		if(memcmp(&token[0][0], "firm", strlen(&token[0][0])) == 0)
			switchDbgMode(DbgOpFirm);
		else
			switchDbgMode(DbgOpDriver);
		return TRUE;
	case DbgCommonStatistics:
		strncpy(&token[0][0], &FieldDATA.intfEntry.sendData.dbgArgv[0], MaxDbgArgvLen);
		token[0][MaxDbgArgvLen] = 0;
		if(memcmp(&token[0][0], "disp", strlen(&token[0][0])) == 0)
			UIStatisticsDisp();
		else if(memcmp(&token[0][0], "clr", strlen(&token[0][0])) == 0)
			UIStatisticsClr();
		else
			ResultAppendText("need argument: disp/clr !");
		return TRUE;
	case DbgCommonChnlTxPower:
		UITxPowerDisplay();
		return TRUE;
	}
	return FALSE;
}

BOOL  DbgField::parseEnterCMD()
{
    CMD_ENTRY *cmd;
    LPTSTR current, lptToken[MaxTokenNum];
    int  i, matched_items=0, total_token=0, total_size=0, wait_cnt=0;
    BOOL bResult;
	CString prompt;
	char currCmd[200];
	
	m_CmdLine.GetWindowText(FieldDATA.CLI_Buffer[FieldDATA.CurrCMD]);
	
	if(!FieldDATA.CLI_Buffer[FieldDATA.CurrCMD].GetLength())
		return FALSE;
	else
	{
		strcpy(currCmd, (LPTSTR)(LPCTSTR)FieldDATA.CLI_Buffer[FieldDATA.CurrCMD]);
		prompt = FieldDATA.Prompt + FieldDATA.CLI_Buffer[FieldDATA.CurrCMD] + "\n\r";	
		ResultAppendText(prompt);
	}

	memset(&FieldDATA.intfEntry.sendData, 0, sizeof(INTF_DBG));
	current = &currCmd[0];//(LPTSTR)(LPCTSTR)currCmd;//FieldDATA.CLI_Buffer[FieldDATA.CurrCMD];
    lptToken[0] = strtok(current, " ");
	if(lptToken[0] != NULL)
		total_token++;
	while(1)
	{
		lptToken[total_token] = strtok(NULL, " ");
		if(lptToken[total_token] != NULL)
			total_token++;
		else
			break;
	}
	if(!total_token)
	{
	    ResultAppendText("--> No command find!");
		return TRUE;
	}
	cmd = cmd_table[DbgOpCommon];          //search cmd in common table
	while(cmd->OpCode != DbgOpEnd)
	{
	    if(memcmp(lptToken[0], cmd->cmd_name, strlen(lptToken[0])) == 0)   
	    {
	        matched_items++;
	        break;
	    }
	    cmd++;
	}
	if(!matched_items)
	{
	    cmd = FieldDATA.cmd;
	    while(cmd->OpCode != DbgOpEnd)
	    {
	        if(memcmp(lptToken[0], cmd->cmd_name, strlen(lptToken[0])) == 0)
	        {
	            matched_items++;
	            break;
	        }
	        cmd++;
	    }
	}
	if(matched_items)
	    FieldDATA.intfEntry.sendData.dbgOpCode = (DBG_OPCODE)cmd->OpCode;
	else
	{
	    ResultAppendText("--> No matched commands!");
		return TRUE;
	}

	FieldDATA.intfEntry.sendData.dbgArgc = total_token;
	FieldDATA.intfEntry.sendData.dbgLength = 8+
        FieldDATA.intfEntry.sendData.dbgArgc*MaxDbgArgvLen;
	FieldDATA.intfEntry.sendData.dbgCmdIndex = cmd->CmdIndex;
	memset(&FieldDATA.intfEntry.sendData.dbgArgv[0], 0, MaxDbgArgvNum*MaxDbgArgvLen);
	for(i=0; i<(total_token-1); i++)
		strcpy(&FieldDATA.intfEntry.sendData.dbgArgv[MaxDbgArgvLen*i], lptToken[i+1]);
	
	if(cmd->OpCode == DbgOpCommon)
	{
		if(cmd->CmdIndex != DbgCommonCommand)
		{
			dbgCommonCMD(cmd);
			return TRUE;
		}
		else
		{	//common debug command parse, you don't need the header and description.
			char *stop;
			FieldDATA.intfEntry.sendData.dbgOpCode = FieldDATA.dbg_mode;
			FieldDATA.intfEntry.sendData.dbgArgc = total_token-1;
			FieldDATA.intfEntry.sendData.dbgLength = 8+
				FieldDATA.intfEntry.sendData.dbgArgc*MaxDbgArgvLen;
			FieldDATA.intfEntry.sendData.dbgCmdIndex = strtol(lptToken[1], &stop, 0);
			memset(&FieldDATA.intfEntry.sendData.dbgArgv[0], 0, MaxDbgArgvNum*MaxDbgArgvLen);
			for(i=0; i<(total_token-2); i++)
				strcpy(&FieldDATA.intfEntry.sendData.dbgArgv[MaxDbgArgvLen*i], lptToken[i+2]);
		}
	}
    
    bResult = DebugCmdSend(
            (LPADAPTER)Adapter.hFile,
            8+FieldDATA.intfEntry.sendData.dbgLength,
            (UCHAR *)&FieldDATA.intfEntry.sendData
            );

	if(bResult == TRUE)
	{
		int ready=0;
		while(wait_cnt++ <= 50)
		{
			bResult = DebugCmdCheck(
			(LPADAPTER)Adapter.hFile,
			&ready
			);
			if(bResult && ready)
				break;
			else
				Sleep(100);
		}
		while(wait_cnt++<100)
		{
			bResult = DebuggCmdReturn(
			(LPADAPTER)Adapter.hFile,
			(UCHAR *)&FieldDATA.intfEntry.retnData//(UCHAR *)&returnedData[0]
			);
			if(bResult)
			{
				ResultAppendText(&FieldDATA.intfEntry.retnData.dbgRtnBuf[0]);
				if(FieldDATA.intfEntry.retnData.dbgStatus == 1 ||	//success
				   FieldDATA.intfEntry.retnData.dbgStatus == 0)		//fail
				{
					break;
				}
				//ResultAppendText(&returnedData[0]);
			}
			else
			    break;
		}
	}
	return TRUE;
}

void DbgField::parseArrowKey(WPARAM inVal)
{
    int len;
    switch(inVal)
    {
        case VK_DOWN:
            if(FieldDATA.HistoryIndex == FieldDATA.CurrCMD)
			{
				m_CmdLine.SetWindowText("");
				FieldDATA.HistoryIndex = FieldDATA.CurrCMD;
				return;
			}
			INC_HistoryIndex;
			if(FieldDATA.HistoryIndex == FieldDATA.CurrCMD)
			{
				m_CmdLine.SetWindowText("");
				FieldDATA.HistoryIndex = FieldDATA.CurrCMD;
				return;
			}
			len = FieldDATA.CLI_Buffer[FieldDATA.HistoryIndex].GetLength();
			m_CmdLine.SetWindowText(FieldDATA.CLI_Buffer[FieldDATA.HistoryIndex]);
			m_CmdLine.SetSel(len, len);
            break;
            break;
		case VK_UP:
		    if(FieldDATA.TotalCMD >= MaxCLIHistory)
		    {
		    	DEC_HistoryIndex;
					if(FieldDATA.HistoryIndex == FieldDATA.CurrCMD)
						INC_HistoryIndex;
		    }
		    else
		    {
					if(FieldDATA.HistoryIndex)
						DEC_HistoryIndex;
		    }
			len = FieldDATA.CLI_Buffer[FieldDATA.HistoryIndex].GetLength();
			m_CmdLine.SetWindowText(FieldDATA.CLI_Buffer[FieldDATA.HistoryIndex]);
			m_CmdLine.SetSel(len, len);
		    break;
    }
}

BOOL DbgField::parseHelpCMD()
{
    CMD_ENTRY *cmd, *match_cmd=NULL;
    LPTSTR current, lptToken[MaxTokenNum];
    int matched_items=0, total_token=0;
	CString prompt;
	
	m_CmdLine.GetWindowText(FieldDATA.CLI_Buffer[FieldDATA.CurrCMD]);
	current = (LPTSTR)(LPCTSTR)FieldDATA.CLI_Buffer[FieldDATA.CurrCMD];
	prompt = FieldDATA.Prompt + FieldDATA.CLI_Buffer[FieldDATA.CurrCMD] + "\n\r";

    lptToken[0] = strtok(current, " ");
	if(lptToken[0] != NULL)
		total_token++;
	while(1)
	{
		lptToken[total_token] = strtok(NULL, " ");
		if(lptToken[total_token] != NULL)
			total_token++;
		else
			break;
	}
	if(!total_token)
	{
	    cmd = cmd_table[DbgOpCommon];
	    displayaAllCMDs(cmd);
	    cmd = FieldDATA.cmd;
	    displayaAllCMDs(cmd);
		return TRUE;
	}
	cmd = cmd_table[DbgOpCommon];          //search cmd in common table
	while(cmd->OpCode != DbgOpEnd)
	{
	    if(memcmp(lptToken[0], cmd->cmd_name, strlen(lptToken[0])) == 0)
	    {
	        matched_items++;
	        match_cmd = cmd;
	        break;
	    }
	    cmd++;
	}
	if(!matched_items)
	{
	    cmd = FieldDATA.cmd;
	    while(cmd->OpCode != DbgOpEnd)
	    {
	        if(memcmp(lptToken[0], cmd->cmd_name, strlen(lptToken[0])) == 0)
	        {
	            matched_items++;
	            match_cmd = cmd;
	            break;
	        }
	        cmd++;
	    }
	}	
	ResultAppendText(prompt);
	if(matched_items)
	{
		if(match_cmd)
		{
		    ResultAppendText(match_cmd->help);
		    ResultAppendText("\n\r");
		}
	}
	else
	{
	    ResultAppendText("--> No matched commands!\n\r");
	}
    return TRUE;
}

void DbgField::switchDbgMode(DBG_OPCODE mode)
{
    CString Title;
    
    switch(mode)
    {
        case DbgOpDriver:
            FieldDATA.cmd = cmd_table[mode];
            FieldDATA.dbg_mode = mode;
            Title.Format(_T("Realtek Debug Field Mode (Driver Mode)"));
			ResultAppendText("Switch to driver mode.");
            break;
        case DbgOpFirm  :
            FieldDATA.cmd = cmd_table[mode];
            FieldDATA.dbg_mode = mode;
            Title.Format(_T("Realtek Debug Field Mode (Firmware Mode)"));
			ResultAppendText("Switch to firmware mode.");
            break;
        default:
            FieldDATA.cmd = cmd_table[DbgOpDriver];
            FieldDATA.dbg_mode = DbgOpDriver;
            Title.Format(_T("Realtek Debug Field Mode (Driver Mode)"));
			ResultAppendText("Switch to driver mode.");
            break;
    }
    SetWindowText(Title);
}

void DbgField::UIStatisticsDisp()
{
	char buf[100];
	
	sprintf(buf, "ctr_G_xhThrd_PacketsTx = %d\n\r", uistatistics.ctr_G_xhThrd_PacketsTx);
	ResultAppendText(&buf[0]);
	sprintf(buf, "ctr_threadFreeOk_PacketsTx = %d\n\r", uistatistics.ctr_threadFreeOk_PacketsTx);
	ResultAppendText(&buf[0]);
	sprintf(buf, "ctr_G_xhThrd_ContinuousTx = %d\n\r", uistatistics.ctr_G_xhThrd_ContinuousTx);
	ResultAppendText(&buf[0]);
	sprintf(buf, "ctr_threadFreeOk_ContinuousTx = %d\n\r", uistatistics.ctr_threadFreeOk_ContinuousTx);
	ResultAppendText(&buf[0]);
	sprintf(buf, "ctr_G_xhThrd_CarrierSupTx = %d\n\r", uistatistics.ctr_G_xhThrd_CarrierSupTx);
	ResultAppendText(&buf[0]);
	sprintf(buf, "ctr_threadFreeOk_CarrierSupTx = %d\n\r", uistatistics.ctr_threadFreeOk_CarrierSupTx);
	ResultAppendText(&buf[0]);	
	
	
	sprintf(buf, "ctr_PacketAllocatePacket_Ok = %d\n\r", uistatistics.ctr_PacketAllocatePacket_Ok);
	ResultAppendText(&buf[0]);	
	sprintf(buf, "ctr_PacketAllocatePacket_Fail = %d\n\r", uistatistics.ctr_PacketAllocatePacket_Fail);
	ResultAppendText(&buf[0]);	
	sprintf(buf, "ctr_PacketInitPacket_Ok = %d\n\r", uistatistics.ctr_PacketInitPacket_Ok);
	ResultAppendText(&buf[0]);	
	sprintf(buf, "ctr_PacketInitPacket_Fail = %d\n\r", uistatistics.ctr_PacketInitPacket_Fail);
	ResultAppendText(&buf[0]);	
	sprintf(buf, "ctr_PacketSendPacket_Ok = %d\n\r", uistatistics.ctr_PacketSendPacket_Ok);
	ResultAppendText(&buf[0]);	
	sprintf(buf, "ctr_PacketSendPacket_Fail = %d\n\r", uistatistics.ctr_PacketSendPacket_Fail);
	ResultAppendText(&buf[0]);	
	sprintf(buf, "ctr_PacketFreePacket_Ok = %d\n\r", uistatistics.ctr_PacketFreePacket_Ok);
	ResultAppendText(&buf[0]);	
	sprintf(buf, "ctr_PacketFreePacket_Fail = %d\n\r", uistatistics.ctr_PacketFreePacket_Fail);
	ResultAppendText(&buf[0]);
}

void DbgField::UIStatisticsClr()
{
	memset(&uistatistics, 0, sizeof(UI_COUNTER_STATISTICS));
}

void DbgField::UITxPowerDisplay()
{

}

void DbgField::displayaAllCMDs(CMD_ENTRY *cmd)
{
    char buff[200];

    while(cmd->OpCode != DbgOpEnd)
	{
	    sprintf(buff, "[%-15s\t] : %s\n\r", cmd->cmd_name, cmd->desc);
	    ResultAppendText(buff);
	    cmd++;
	}
}

BOOL DbgField::parseSpaceCMD()
{
    CMD_ENTRY *cmd;
    LPTSTR current, lptToken[MaxTokenNum];
    int matched_items=0, total_token=0;
	CString prompt;
	
	m_CmdLine.GetWindowText(FieldDATA.CLI_Buffer[FieldDATA.CurrCMD]);
	if(!FieldDATA.CLI_Buffer[FieldDATA.CurrCMD].GetLength())
		return TRUE;

	current = (LPTSTR)(LPCTSTR)FieldDATA.CLI_Buffer[FieldDATA.CurrCMD];
    lptToken[0] = strtok(current, " ");
	if(lptToken[0] != NULL)
		total_token++;
	while(1)
	{
		lptToken[total_token] = strtok(NULL, " ");
		if(lptToken[total_token] != NULL)
			total_token++;
		else
			break;
	}
	if(!total_token)
	{
	    ResultAppendText("--> No command find!");
		return TRUE;
	}
	else if(total_token > 1)
		return FALSE;

	cmd = cmd_table[DbgOpCommon];          //search cmd in common table
	while(cmd->OpCode != DbgOpEnd)
	{
	    if(memcmp(lptToken[0], cmd->cmd_name, strlen(lptToken[0])) == 0)
	    {
	        matched_items++;
	        break;
	    }
	    cmd++;
	}
	if(!matched_items)
	{
	    cmd = FieldDATA.cmd;
	    while(cmd->OpCode != DbgOpEnd)
	    {
	        if(memcmp(lptToken[0], cmd->cmd_name, strlen(lptToken[0])) == 0)
	        {
	            matched_items++;
	            break;
	        }
	        cmd++;
	    }
	}
	if(matched_items)
	{
		if(strcmp(lptToken[0], cmd->cmd_name) == 0)
			return FALSE;
		else
		{
			int len;
			len = strlen(cmd->cmd_name);
			FieldDATA.CLI_Buffer[FieldDATA.CurrCMD].Format(_T("%s"), cmd->cmd_name);
			m_CmdLine.SetWindowText(FieldDATA.CLI_Buffer[FieldDATA.CurrCMD]);
			m_CmdLine.SetSel(len, len);
		}
	}
	else
	{
		prompt = FieldDATA.Prompt + FieldDATA.CLI_Buffer[FieldDATA.CurrCMD] + "\n\r";	
		ResultAppendText(prompt);
	    ResultAppendText("--> No matched commands!\n\r");
		return TRUE;
	}
	return FALSE;
}
