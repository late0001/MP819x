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




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma region [Utility]

UINT CMP819xVCDlg::GetCurrChannelTxPower()
{
	UINT tmpTxPower, tmpTxPowerA=0, tmpTxPowerB=0, tmpTxPowerC=0, tmpTxPowerD=0;
	
	// <20130619, Kordan> 8723B has dual RF path but only one BB. So we have to check
	// the current RF path to get the right Tx power index.
	if (IS_HARDWARE_SERIES(8723B) || IS_HARDWARE_SERIES(8703B) || IS_HARDWARE_SERIES(8723D))
	{
		UINT rfpath = (Adapter.CurrentAntenna >> 16 == ANTENNA_B) ? RfPath_B : RfPath_A;
		GetCalTxPwrIndex((LPADAPTER)(Adapter.hFile), rfpath, (int *)&tmpTxPowerA);
		tmpTxPowerB = tmpTxPowerA;
	}
	else
		GetCalTxPwrIndex((LPADAPTER)(Adapter.hFile), RfPath_A, (int *)&tmpTxPowerA);
	
	if (RfMode == RF_2T2R || RfMode == RF_2T4R|| RfMode == RF_2T3R || RfMode == RF_2T2R_GREEN)
	{
		if(!IS_HARDWARE_SERIES(8814A))
		{
			//
			//20150127, KenSun, This is the original code flow for old chips.
			//
			GetCalTxPwrIndex((LPADAPTER)(Adapter.hFile), RfPath_B, (int *)&tmpTxPowerB);
		}
		else
		{
			//
			//20150127, KenSun, If 8814 is configured as 2TXR, It only supports the combination of  RF path {1, 2} currently. (According to Willis's comments)
			//
			BYTE tx_path = (BYTE)(ValidRfPath>>8)&0xff;
			if(tx_path == 0x06)
			{
				GetCalTxPwrIndex((LPADAPTER)(Adapter.hFile), RfPath_B, (int *)&tmpTxPowerB);
				GetCalTxPwrIndex((LPADAPTER)(Adapter.hFile), RfPath_C, (int *)&tmpTxPowerC);
			}
		}
	}
	else if (RfMode == RF_3T3R && IS_HARDWARE_SERIES(8814A))
	{
		GetCalTxPwrIndex((LPADAPTER)(Adapter.hFile), RfPath_B, (int *)&tmpTxPowerB);
		GetCalTxPwrIndex((LPADAPTER)(Adapter.hFile), RfPath_C, (int *)&tmpTxPowerC);
		GetCalTxPwrIndex((LPADAPTER)(Adapter.hFile), RfPath_D, (int *)&tmpTxPowerD);
	}
	
	tmpTxPower = ((tmpTxPowerD<<24)|(tmpTxPowerC<<16)|(tmpTxPowerB<<8)|tmpTxPowerA);
	
	return(tmpTxPower);
}

BOOL CMP819xVCDlg::GetHardwareID()
{
	UINT id = (UINT)regAdapter.ChipId, ver= (UINT)regAdapter.ChipVer;
	int len = sizeof(namingTable)/ sizeof(ChipStruct);
	BOOL isMatched = FALSE;
	for (int i = 0; i < len; ++i) {
		if (id == namingTable[i].ChipID && ver == namingTable[i].ChipVer) {
			
			isMatched = TRUE;
			Adapter.HardwareId = (UINT)namingTable[i].ChipType;
			
 			Win32Print("(%x, %x) == (%x, %x) ChipType is 0x%X\n", id, ver, namingTable[i].ChipID, namingTable[i].ChipVer, (UINT)namingTable[i].ChipType);
			break;
		}
	}
	if ( ! isMatched) {
	    Adapter.HardwareId = -1;
	    return FALSE;
	}
	return TRUE;         
}

void CMP819xVCDlg::ItemEnDisable(BOOL enable,UINT Force)
{
	BOOL    bEnable;
	UINT	i, tmpAntennaTx;
	
	i = m_AntennaTx.GetCurSel();
	tmpAntennaTx = IndexValueConvert(ComboAntennaTx, Index2Value, i);
	
	if(AutoTestFlag)
    {
       	m_Start.EnableWindow(FALSE);
       	bEnable = FALSE;
    }
	else if (TxTrackFlagStop == FALSE)
	{    	
		m_Start.EnableWindow(enable);
		bEnable = enable;
	}
	else
	{
    	m_Start.EnableWindow(TRUE);
		bEnable = enable;
	}
	
	if(RfMode == RF_2T4R 
		|| RfMode == RF_2T3R
		|| RfMode == RF_2T2R 
		|| RfMode == RF_2T2R_GREEN
		|| RfMode == RF_3T3R
		|| RfMode == RF_4T4R
		|| IS_HARDWARE_SERIES(8723B)
		|| IS_HARDWARE_SERIES(8703B)
		|| IS_HARDWARE_SERIES(8723D))
		m_AntennaTx.EnableWindow(bEnable);
	
	else if(RfMode == RF_1T1R)
	{		
		// We force Tx/Rx path invisible as default. Revised by Roger, 2008.12.31.
		m_AntennaTx.EnableWindow(FALSE);
		m_AntennaRx.EnableWindow(FALSE);
		m_TxPowerB.EnableWindow(FALSE);
	}
	else
		m_AntennaRx.EnableWindow(bEnable);
	
	m_Modulation.EnableWindow(bEnable);
	m_TestList.EnableWindow(bEnable);
	m_Help.EnableWindow(bEnable);
	m_ReadMacAddr.EnableWindow(bEnable);
	m_ReadEEprom.EnableWindow(bEnable);
	m_WriteMac.EnableWindow(bEnable);
	m_SelfMacAddr.EnableWindow(bEnable);
	m_WriteEEprom.EnableWindow(bEnable);
	m_EfuseTargetSel.EnableWindow(bEnable);
	m_BT_WiFi.EnableWindow(bEnable);
	m_SetDest_Mac.EnableWindow(bEnable);
	m_DestMacAddr.EnableWindow(bEnable);	
	m_Channel_Width.EnableWindow(bEnable);
	m_Button_CSC.SetWindowText(TEXT("Change settings"));
	m_Button_CSC.EnableWindow(bEnable);
	if(m_CheckBox_EnableTxPowerLimit.IsWindowEnabled())
		m_CheckBox_EnableTxPowerLimit.EnableWindow(bEnable);
	if(m_CustomerSelPwr.IsWindowEnabled())
		m_CustomerSelPwr.EnableWindow(bEnable);
	if(m_CustomerSelPwrLmt.IsWindowEnabled())
		m_CustomerSelPwrLmt.EnableWindow(bEnable);
	
	if(bBTTest)
		m_BT.EnableWindow(TRUE);
	else
		m_BT.EnableWindow(bEnable);

	m_UpdateEfuse.EnableWindow(bEnable);
	m_TxPowerTrack.EnableWindow(bEnable);
	
	if (bEEByteMode)
	{
	    m_EEByteOffset.EnableWindow(bEnable);
	    m_EEByteValue.EnableWindow(bEnable);
	}
	
	if(TestItem != PACKETS_RX || Force)
	{
		// disabled these items
		m_Reset.EnableWindow(bEnable);
	}
	if(TestItem != PACKETS_TX || AutoTestFlag || Force)
	{
		// disabled these items
		m_TxPower.EnableWindow(bEnable);
		m_TxPowerB.EnableWindow(bEnable);
		m_Channel.EnableWindow(bEnable);
	}

	// Tx Related disabled Items when Testing
	m_Preamble.EnableWindow(bEnable);
	m_Pattern.EnableWindow(bEnable);
	m_PacketLength.EnableWindow(bEnable);
	m_TxCount.EnableWindow(bEnable);
	
	m_TxPMAC_Switch.EnableWindow(bEnable);
	m_TxPMAC_TypeSel.EnableWindow(bEnable);
	m_TxPMAC_Period.EnableWindow(bEnable);

	if (regAdapter.iMode == CREATOR_MODE) {
		m_TxSC_Selection.EnableWindow(bEnable);
	}
	if (QueryPmacMode() == TRUE) {
		m_Rate.EnableWindow(bEnable);
	}

	if (bEnable || TestItem == CONTINUOUS_TX) {
		SetTxPowerComboBox(tmpAntennaTx, TRUE);
	}
}

void CMP819xVCDlg::SwitchRateSet(WIRELESS_MODE WirelessMode, MPT_BANDWIDTHE Bandwidth) 
{
	if (Adapter.EnableAllRates) 
		return;
	else
		m_Rate.ResetContent();
	
#define IS_1T1R() ((RfMode == RF_1T2R) || (RfMode == RF_1T1R))

	if (IS_RF_2G_20M(WirelessMode, Bandwidth))
	{
	    Adapter.nTotalRateCnt = Rates_2_4G_20M_NUM;
	    UINT j = 0;
	    for (UINT i = 0; i < Adapter.nTotalRateCnt; ++i) {

			if ((IS_3SS_RATE(Rates_2_4G_20M[i].rate) || IS_4SS_RATE(Rates_2_4G_20M[i].rate)) && RfMode != RF_3T3R)
				continue;
			else if (IS_4SS_RATE(Rates_2_4G_20M[i].rate) && RfMode == RF_3T3R)
				continue;
			
			// For Verification
			if (IS_VHT_RATE(Rates_2_4G_20M[i].rate) && bVHTEnabled == FALSE)
				continue;
				
			if (m_CheckBox_STBC.GetCheck() == BST_CHECKED) {
				if (IS_CCK_RATE(Rates_2_4G_20M[i].rate) || IS_OFDM_RATE(Rates_2_4G_20M[i].rate))
					continue;
				else if (!IS_HT_1S_RATE(Rates_2_4G_20M[i].rate) && !IS_VHT_1S_RATE(Rates_2_4G_20M[i].rate))
					continue;
			} else {
				if (IS_11AC_SERIES() && IS_1T1R() && IS_2SS_RATE(Rates_2_4G_20M[i].rate)) 
	    			continue;
				else if (IS_11N_SERIES()) {
					if (Rates_2_4G_20M[i].rate >= RATE_VHT1SS_MCS0)
						continue;
					else if (IS_1T1R() && Rates_2_4G_20M[i].rate >= RATE_MCS8)
						continue;					
				}
			}

			m_Rate.AddString(Rates_2_4G_20M[i].rateStr);
	    	m_Rate.SetItemData(j++, Rates_2_4G_20M[i].rate);
			
	    }
		m_Rate.SetCurSel(IndexValueConvert(ComboDataRate, Value2Index, RATE_MCS0));
		UpdateDataRate(RATE_MCS0, 0);
    }
    else if (IS_RF_2G_40M(WirelessMode, Bandwidth))
	{
	    Adapter.nTotalRateCnt = Rates_5G_40M_NUM;

	    UINT j = 0;
	    for (UINT i = 0; i < Adapter.nTotalRateCnt; ++i) {

			if ((IS_3SS_RATE(Rates_2_4G_40M[i].rate)||IS_4SS_RATE(Rates_2_4G_40M[i].rate)) && RfMode != RF_3T3R)
				continue;
			else if (IS_4SS_RATE(Rates_2_4G_40M[i].rate) && RfMode == RF_3T3R)
				continue;
			
			// For Verification
			if (IS_VHT_RATE(Rates_2_4G_40M[i].rate) && bVHTEnabled == FALSE)
				continue;

			if (m_CheckBox_STBC.GetCheck() == BST_CHECKED) {
				if (!IS_HT_1S_RATE(Rates_2_4G_40M[i].rate) && !IS_VHT_1S_RATE(Rates_2_4G_40M[i].rate))
					continue;
			} else {
				if (IS_11AC_SERIES() && IS_1T1R() && IS_2SS_RATE(Rates_2_4G_40M[i].rate)) 					
	    			continue;
				else if (IS_11N_SERIES()) {
					if (Rates_2_4G_40M[i].rate >= RATE_VHT1SS_MCS0)
						continue;
					else if (IS_1T1R() && Rates_2_4G_40M[i].rate >= RATE_MCS8)
						continue;					
				}
			}

	    	m_Rate.AddString(Rates_2_4G_40M[i].rateStr);
	    	m_Rate.SetItemData(j++, Rates_2_4G_40M[i].rate);
			
	    }
	    m_Rate.SetCurSel(IndexValueConvert(ComboDataRate, Value2Index, RATE_MCS0));
		UpdateDataRate(RATE_MCS0, 0);
	}
    else if (IS_RF_5G_20M(WirelessMode, Bandwidth))
	{
		Adapter.nTotalRateCnt = Rates_5G_20M_NUM;
		UINT j = 0;
		for (UINT i = 0; i < Adapter.nTotalRateCnt; ++i) 
		{
			if (IS_1T1R() && IS_2SS_RATE(Rates_5G_20M[i].rate))								
				continue;
			
			// For Verification
			if (IS_VHT_RATE(Rates_5G_20M[i].rate) && bVHTEnabled == FALSE)
				continue;

			if ((IS_3SS_RATE(Rates_5G_20M[i].rate)||IS_4SS_RATE(Rates_5G_20M[i].rate)) && RfMode != RF_3T3R)
				continue;
			else if (IS_4SS_RATE(Rates_5G_20M[i].rate) && RfMode == RF_3T3R)
				continue;

			if (m_CheckBox_STBC.GetCheck() == BST_CHECKED) 
			{
				if (!IS_HT_1S_RATE(Rates_5G_20M[i].rate) && !IS_VHT_1S_RATE(Rates_5G_20M[i].rate))
					continue;
			}

			m_Rate.AddString(Rates_5G_20M[i].rateStr);
			m_Rate.SetItemData(j, Rates_5G_20M[i].rate);
			j++;
		}
		m_Rate.SetCurSel(IndexValueConvert(ComboDataRate, Value2Index, RATE_MCS0));
		UpdateDataRate(RATE_MCS0, 0);
	}
	else if (IS_RF_5G_40M(WirelessMode, Bandwidth))
	{
		Adapter.nTotalRateCnt = Rates_5G_40M_NUM;

		UINT j = 0;
		for (UINT i = 0; i < Adapter.nTotalRateCnt; ++i) 
		{
			
			if (IS_1T1R() && IS_2SS_RATE(Rates_5G_40M[i].rate))
				continue;
			
			// For Verification
			if (IS_VHT_RATE(Rates_5G_40M[i].rate) && bVHTEnabled == FALSE)
				continue;

			if ((IS_3SS_RATE(Rates_5G_40M[i].rate)||IS_4SS_RATE(Rates_5G_40M[i].rate)) && RfMode != RF_3T3R)
				continue;
			else if (IS_4SS_RATE(Rates_5G_40M[i].rate) && RfMode == RF_3T3R)
				continue;

			if (m_CheckBox_STBC.GetCheck() == BST_CHECKED) 
			{
				if (!IS_HT_1S_RATE(Rates_5G_40M[i].rate) && !IS_VHT_1S_RATE(Rates_5G_40M[i].rate))
					continue;
			}

			m_Rate.AddString(Rates_5G_40M[i].rateStr);
			m_Rate.SetItemData(j, Rates_5G_40M[i].rate);
			j++;
		}
		m_Rate.SetCurSel(IndexValueConvert(ComboDataRate, Value2Index, RATE_MCS0));
		UpdateDataRate(RATE_MCS0, 0);
	}
	else if (IS_RF_5G_80M(WirelessMode, Bandwidth))
	{
		Adapter.nTotalRateCnt = Rates_5G_80M_NUM;

		UINT j = 0;
		for (UINT i = 0; i < Adapter.nTotalRateCnt; ++i) 
		{
			
			if (IS_1T1R() && IS_2SS_RATE(Rates_5G_80M[i].rate))								
				continue;
			
			// For Verification
			if (IS_VHT_RATE(Rates_5G_80M[i].rate) && bVHTEnabled == FALSE)
				continue;

			if ((IS_3SS_RATE(Rates_5G_80M[i].rate)||IS_4SS_RATE(Rates_5G_80M[i].rate)) && RfMode != RF_3T3R)
				continue;
			else if (IS_4SS_RATE(Rates_5G_80M[i].rate) && RfMode == RF_3T3R)
				continue;

			if (m_CheckBox_STBC.GetCheck() == BST_CHECKED) 
			{
				if (!IS_HT_1S_RATE(Rates_5G_80M[i].rate) && !IS_VHT_1S_RATE(Rates_5G_80M[i].rate))
					continue;
			}

			m_Rate.AddString(Rates_5G_80M[i].rateStr);
			m_Rate.SetItemData(j, Rates_5G_80M[i].rate);
			j++;
		}
		
		m_Rate.SetCurSel(IndexValueConvert(ComboDataRate, Value2Index, RATE_VHT1SS_MCS0));
		UpdateDataRate(RATE_VHT1SS_MCS0, 0);
	}					
	
}

LPADAPTER CMP819xVCDlg::StartDutByMode(MODE _mode)
{
	BOOL result	=	FALSE;
	if (_mode != CREATOR_MODE)
		return (LPADAPTER)StartDUTwithNID(regAdapter.ChipId, regAdapter.ChipVer, Adapter.NetCfgInstanceId);
	else {
		PVOID 	handle;
		int len = sizeof(namingTable)/ sizeof(ChipStruct);

		for (int i = 0; i < len; ++i) {
			if (handle = OpenSpecificDutSilentlywithNID(namingTable[i].ChipID, namingTable[i].ChipVer, Adapter.NetCfgInstanceId)) {
				CString Title = "Realtek ";
   				Title = Title + namingTable[i].ChipName + " MP Diagnostic Program " + "(CREATOR_MODE)";
   				SetWindowText(Title);
				Adapter.HardwareId	= namingTable[i].ChipType;				
				return (LPADAPTER)handle;
			}
		}
		return NULL;
	}
}

VOID CMP819xVCDlg::ErrMsgBox(LPCTSTR msg)
{
    MessageBox(
            msg,
            TEXT("Realtek"),
            MB_OK|MB_ICONERROR|MB_SETFOREGROUND
            );
}

#pragma region [Update] Status
void CMP819xVCDlg::UpdateRFPathSetting(
	IN UINT newAntSetting,
	IN UINT disp
	)
{
	BOOL    bResult;
	UINT	tempVal;

	Adapter.CurrentRFPathSetting = (RFPATH_SETTING)newAntSetting;

	bResult = SwitchRFPathSetting(
				(LPADAPTER)(Adapter.hFile),
				Adapter.CurrentRFPathSetting
				);
	if(!bResult)
	{
		if(disp)
			m_Result.SetWindowText(TEXT("AntennaConfig FAIL"));
		else
			ErrMsgBox(TEXT("Fail to Set AntennaConfig !!" ));
	}
	else
	{
		if(disp)
	    	m_Result.SetWindowText(TEXT("AntennaConfig OK"));
	}
	// Update Selection for CREATOR Mode
	tempVal = Adapter.CurrentRFPathSetting;
	m_RFPathSetting.SetCurSel(tempVal);

	// Update Selection for Normal Mode
	if (tempVal != RFPATH_BLUETOOTH)
		m_RFPathSetting_Duplicate.SetCurSel(0);
	else
		m_RFPathSetting_Duplicate.SetCurSel(1);

}

void CMP819xVCDlg::
UpdateModulation(
	IN UINT newModulation,
	IN UINT disp
	)
{
	BOOL    bResult;
	UINT	tempVal;
	
	Adapter.CurrentModulation = (WIRELESS_MODE)newModulation;

	//bResult = SwitchBand(Adapter.CurrentModulation);
	bResult = SetModulation(
                (LPADAPTER)Adapter.hFile,
                Adapter.CurrentModulation
                );

	if(!bResult)
	{
		if(disp)
			m_Result.SetWindowText(TEXT("Modulation setting FAIL"));
		else
			ErrMsgBox(TEXT("Fail to Set Modulation !!" ));
	}
	else
	{
		if(disp)
	    	m_Result.SetWindowText(TEXT("Modulation setting OK"));
	}
	tempVal = IndexValueConvert(ComboModulation, Value2Index, Adapter.CurrentModulation);
	m_Modulation.SetCurSel(tempVal);


	// Set up channels
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

	ResetUIByRFSetting(ON_MODULATION_CHANGED);
}

void CMP819xVCDlg::
UpdateBandwidth(
	IN UINT	newBandwidth,
	IN UINT disp
	)
{
	BOOL    bResult;
	UINT	tempVal;
	
	Adapter.CurrentBandWidth = (MPT_BANDWIDTHE)newBandwidth;

	bResult = SetSignalLocationType((LPADAPTER) Adapter.hFile, (int)Adapter.CurrentBandWidth);
    if(!bResult)
    {
    	if(disp)
        	m_Result.SetWindowText(TEXT("Set Bandwidth fail."));
        else
        	ErrMsgBox(TEXT("Fail to Set Bandwidth !!" ));
    }
    else
    {
    	if(disp)
        	m_Result.SetWindowText(TEXT("Set Bandwidth success."));
    }

	if(	Adapter.CurrentModulation == WIRELESS_MODE_N_5G || 
	    Adapter.CurrentModulation == WIRELESS_MODE_AC_5G)
	{
		if(Adapter.CurrentBandWidth == MPT_BW_20MHZ)
		{
			Adapter.Channels = Channels_5G_20M;
			Adapter.nTotalChnlCnt = ChannelNum_5G_20M;
			tempVal = customBWsetting[1][0].comboxOrder;
		}
		else if(Adapter.CurrentBandWidth == MPT_BW_10MHZ)
		{
			Adapter.Channels = Channels_5G_20M;
			Adapter.nTotalChnlCnt = ChannelNum_5G_20M;
			tempVal = customBWsetting[1][5].comboxOrder;
		}
		else if(Adapter.CurrentBandWidth == MPT_BW_5MHZ)
		{
			Adapter.Channels = Channels_5G_20M;
			Adapter.nTotalChnlCnt = ChannelNum_5G_20M;
			tempVal = customBWsetting[1][4].comboxOrder;
		}
		else if (Adapter.CurrentBandWidth == MPT_BW_40MHZ)
		{
			Adapter.Channels = Channels_5G_40M;
			Adapter.nTotalChnlCnt = ChannelNum_5G_40M;
			tempVal = customBWsetting[1][1].comboxOrder;
		}
		else if (Adapter.CurrentBandWidth == MPT_BW_80MHZ)
		{
			Adapter.Channels = Channels_5G_80M;
			Adapter.nTotalChnlCnt = ChannelNum_5G_80M;		
			tempVal = customBWsetting[1][2].comboxOrder;
		}
	} 
	else if (Adapter.CurrentModulation == WIRELESS_MODE_N_24G) 
	{
		if(Adapter.CurrentBandWidth == MPT_BW_20MHZ)
		{
			Adapter.Channels = Channels_2_4G_20M;
			Adapter.nTotalChnlCnt = ChannelNum_2_4G_20M;
			tempVal = customBWsetting[0][0].comboxOrder;
		}
		else if(Adapter.CurrentBandWidth == MPT_BW_10MHZ)
		{
			Adapter.Channels = Channels_2_4G_20M;
			Adapter.nTotalChnlCnt = ChannelNum_2_4G_20M;
			tempVal = customBWsetting[0][5].comboxOrder;
		}
		else if(Adapter.CurrentBandWidth == MPT_BW_5MHZ)
		{
			Adapter.Channels = Channels_2_4G_20M;
			Adapter.nTotalChnlCnt = ChannelNum_2_4G_20M;
			tempVal = customBWsetting[0][4].comboxOrder;
		}
		else if (Adapter.CurrentBandWidth == MPT_BW_40MHZ)
		{
			Adapter.Channels = Channels_2_4G_40M;
			Adapter.nTotalChnlCnt = ChannelNum_2_4G_40M;		
			tempVal = customBWsetting[0][1].comboxOrder;
		}
	}
	ResetUIByRFSetting(ON_BANDWIDTH_CHANGED);
	
    m_Channel_Width.SetCurSel(tempVal);
}

void CMP819xVCDlg::
UpdateDataRate(
	IN UINT	newRate,
	IN UINT disp
	)
{
	BOOL    bResult;
	UINT	tempVal;
	
	Adapter.CurrentRate = (MPT_RATE_E)newRate;
	bResult = SetDataRate(
	            (LPADAPTER)(Adapter.hFile),
	            Adapter.CurrentRate
	            );
	if (!bResult)
	{
		if(disp)
	    	m_Result.SetWindowText(TEXT("Rate setting FAIL"));
	    else
	    	ErrMsgBox(TEXT("Fail to Set Data Rate !!" ));
	}
	else
	{
		if(disp)
	    	m_Result.SetWindowText(TEXT("Rate setting OK"));
	}

	tempVal = IndexValueConvert(ComboDataRate, Value2Index, Adapter.CurrentRate);
    m_Rate.SetCurSel(tempVal);
}

void CMP819xVCDlg::
UpdatePreamble(
	IN UINT newPreamble,
	IN UINT disp
	)
{
	BOOL    bResult;
	UINT	tempVal;
	
	Adapter.PacketPreamble = newPreamble;
	bResult = SetPreamble(
	            (LPADAPTER)(Adapter.hFile),
	            Adapter.PacketPreamble
	            );
	if(!bResult)
	{
		if(disp)
	    	m_Result.SetWindowText(TEXT("Preamble setting FAIL"));
	    else
	    	ErrMsgBox(TEXT("Fail to Set Preamble !!" ));
	}
	else
	{
		if(disp)
	    	m_Result.SetWindowText(TEXT("Preamble setting OK"));
	}
	tempVal = IndexValueConvert(ComboPreamble, Value2Index, Adapter.PacketPreamble);
    m_Preamble.SetCurSel(tempVal);
}

void CMP819xVCDlg::
UpdateAntennaTxRx(
	IN UINT newAntennaTx,
	IN UINT	newAntennaRx,
	IN UINT disp
	)
{
	BOOL    bResult;
	UINT	tempVal;

	tempVal = (newAntennaTx<<16)|newAntennaRx;
	if (Adapter.CurrentAntenna != tempVal)
	{
		Adapter.CurrentAntenna = tempVal;

		bResult = SetAntennaBB(
		            (LPADAPTER)(Adapter.hFile),
		            Adapter.CurrentAntenna
		            );
	
		if(!bResult)
		{
			if(disp)
				m_Result.SetWindowText(TEXT("Antenna setting FAIL"));
			else
				ErrMsgBox(TEXT("Fail to Set Antenna !!" ));
		}
		else
		{
			if(disp)
				m_Result.SetWindowText(TEXT("Antenna setting OK"));
		}
	}

	tempVal = IndexValueConvert(ComboAntennaTx, Value2Index, newAntennaTx);
	m_AntennaTx.SetCurSel(tempVal);
	tempVal = IndexValueConvert(ComboAntennaRx, Value2Index, newAntennaRx);
	m_AntennaRx.SetCurSel(tempVal);
}

void CMP819xVCDlg::
UpdateChannel(
	IN UINT newChannel,
	IN UINT disp
	)
{
	BOOL    bResult;
	UINT	tempVal;
	
	Adapter.CurrentChannel = newChannel;
	bResult = SetChannelDirectCall(
				(LPADAPTER)(Adapter.hFile),
				Adapter.CurrentChannel
				);
	if(!bResult)
	{
		if(disp)
			m_Result.SetWindowText(TEXT("Channel setting FAIL"));
		else
	    	ErrMsgBox(TEXT("Fail to Set Channel !!" ));
	}
	else
	{
		if(disp)
	    	m_Result.SetWindowText(TEXT("Channel setting OK"));
	}

	tempVal = IndexValueConvert(ComboChannel, Value2Index, Adapter.CurrentChannel);
    m_Channel.SetCurSel(tempVal);
}

void CMP819xVCDlg::
UpdateTxPower(
	IN UINT newTxPower,
	IN UINT disp
	)
{
	BOOL    bResult;

	Adapter.CurrentTxPower = newTxPower;

	bResult = SetTxPowerControl(
	            (LPADAPTER)(Adapter.hFile),
	            Adapter.CurrentTxPower
	            );
	if(!bResult)
	{
		if(disp)
	    	m_Result.SetWindowText(TEXT("Tx Power setting FAIL"));
	    else
	    	ErrMsgBox(TEXT("Fail to Set Tx Power !!" ));
	}
	else
	{
		if(disp)
	    	m_Result.SetWindowText(TEXT("Tx Power setting OK"));
	}

	SelectTxPowerControl(Adapter.CurrentTxPower);
}

void CMP819xVCDlg::
UpdateCrystalCap(
	IN UINT newCrystalCap,
	IN UINT disp
	)
{
	BOOL    bResult;
	
	Adapter.CurrentCrystalCap = newCrystalCap;
	
	bResult = 1;

	bResult = SetCrystalCap(
	            (LPADAPTER)(Adapter.hFile),
	            Adapter.CurrentCrystalCap
	            );

	if(!bResult)
	{
		if(disp)
	    	m_Result.SetWindowText(TEXT("CrystalCap setting FAIL"));
	    else
	    	ErrMsgBox(TEXT("Fail to Set CrystalCap !!" ));
	}
	else
	{
		if(disp)
	    	m_Result.SetWindowText(TEXT("CrystalCap setting OK"));
	}
	m_Xtal_XinXout.SetCurSel(newCrystalCap & 0x3F);
}

void CMP819xVCDlg::
UpdateCustomPwrTableReg(
	IN	CHAR*	PwrByRateFilename,
	IN	CHAR*	PwrLmtFilename
	)
{
	TCHAR			Buf1[MAX_PATH];
	HKEY 			hKey;
	DWORD			keyStatus	= FALSE;
	DWORD			valStatus	= FALSE;
	BOOLEAN			valid		= FALSE;
	UINT			bChecked	= FALSE;
	
	CString Str1, Str2, Str3;
	if(m_CheckBox_EnableTxPowerLimit.GetCheck() == BST_CHECKED)
		bChecked = TRUE;

	Str1.Format("\\%04d",Adapter.enumId);
	Str2.Format("%s%s","SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}"
				,Str1);
	strcpy(Buf1, (LPCTSTR)Str2);
	keyStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
							TEXT(Buf1),
							0,KEY_ALL_ACCESS|KEY_WRITE,&hKey);
	if (keyStatus == ERROR_SUCCESS) {
		
		Str3.Format("%d",bChecked);
		valStatus = RegSetValueEx(hKey, TEXT("EnableTxPowerLimit"), NULL, REG_SZ, 
							(BYTE*)Str3.GetBuffer(Str3.GetLength()), 
							Str3.GetLength()* sizeof(TCHAR));
		if(valStatus == ERROR_SUCCESS){
			;
		}
		
		Str3.Format("%s",PwrByRateFilename);
		if(Str3 == "Default") Str3.Format("");
		valStatus = RegSetValueEx(hKey, TEXT("PwrByRateFile"), NULL, REG_SZ, 
								(BYTE*)Str3.GetBuffer(Str3.GetLength()), 
								Str3.GetLength()* sizeof(TCHAR));
		if(valStatus == ERROR_SUCCESS){
			;
		}
		
		Str3.Format("%s",PwrLmtFilename);
		if(Str3 == "Default") Str3.Format("");
		valStatus = RegSetValueEx(hKey, TEXT("PwrLimitFile"), NULL, REG_SZ, 
								(BYTE*)Str3.GetBuffer(Str3.GetLength()), 
								Str3.GetLength()* sizeof(TCHAR));
		if(valStatus == ERROR_SUCCESS){
			;
		}
	}
	RegCloseKey(hKey);
}

#pragma endregion Status

#pragma region [Index/Value] Conversion


UINT CMP819xVCDlg::
IndexValueConvert(
	IN UINT ComboType,
	IN UINT TransType,
	IN UINT Invalue
	)
{
	UINT retvalue;
	BOOL bIsMatched = FALSE;

	switch(ComboType)
	{
 		case ComboDataRate		 :	{

			if (IS_RF_5G_20M(Adapter.CurrentModulation, Adapter.CurrentBandWidth)  && 
				RfMode == RF_1T1R && Adapter.EnableAllRates == FALSE)
			{
				if(m_CheckBox_STBC.GetCheck() == BST_CHECKED){
					CONVERT(TransType, 0,  RATE_MCS0, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 1,  RATE_MCS1, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 2,  RATE_MCS2, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 3,  RATE_MCS3, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 4,  RATE_MCS4, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 5,  RATE_MCS5, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 6,  RATE_MCS6, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 7,  RATE_MCS7, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 8,  RATE_VHT1SS_MCS0, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 9,  RATE_VHT1SS_MCS1, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 10,  RATE_VHT1SS_MCS2, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 11,  RATE_VHT1SS_MCS3, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 12,  RATE_VHT1SS_MCS4, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 13,  RATE_VHT1SS_MCS5, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 14,  RATE_VHT1SS_MCS6, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 15,  RATE_VHT1SS_MCS7, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 16,  RATE_VHT1SS_MCS8, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 17,  RATE_VHT1SS_MCS9, Invalue, retvalue, bIsMatched); 
				 
				}else{
					CONVERT(TransType, 0,  RATE_OFDM_6M,  Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 1,  RATE_OFDM_9M,  Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 2,  RATE_OFDM_12M, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 3,  RATE_OFDM_18M, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 4,  RATE_OFDM_24M, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 5,  RATE_OFDM_36M, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 6,  RATE_OFDM_48M, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 7,  RATE_OFDM_54M, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 8,  RATE_MCS0, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 9,  RATE_MCS1, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 10,  RATE_MCS2, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 11,  RATE_MCS3, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 12,  RATE_MCS4, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 13,  RATE_MCS5, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 14,  RATE_MCS6, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 15,  RATE_MCS7, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 16,  RATE_VHT1SS_MCS0, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 17,  RATE_VHT1SS_MCS1, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 18,  RATE_VHT1SS_MCS2, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 19,  RATE_VHT1SS_MCS3, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 20,  RATE_VHT1SS_MCS4, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 21,  RATE_VHT1SS_MCS5, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 22,  RATE_VHT1SS_MCS6, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 23,  RATE_VHT1SS_MCS7, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 24,  RATE_VHT1SS_MCS8, Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 25,  RATE_VHT1SS_MCS9, Invalue, retvalue, bIsMatched); 
				}
			} 
			else if (IS_RF_5G_40M(Adapter.CurrentModulation, Adapter.CurrentBandWidth) && 
				 	 RfMode == RF_1T1R && Adapter.EnableAllRates == FALSE)
			{
				CONVERT(TransType, 0,  RATE_MCS0, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 1,  RATE_MCS1, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 2,  RATE_MCS2, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 3,  RATE_MCS3, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 4,  RATE_MCS4, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 5,  RATE_MCS5, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 6,  RATE_MCS6, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 7,  RATE_MCS7, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 8,   RATE_VHT1SS_MCS0, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 9,   RATE_VHT1SS_MCS1, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 10,  RATE_VHT1SS_MCS2, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 11,  RATE_VHT1SS_MCS3, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 12,  RATE_VHT1SS_MCS4, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 13,  RATE_VHT1SS_MCS5, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 14,  RATE_VHT1SS_MCS6, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 15,  RATE_VHT1SS_MCS7, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 16,  RATE_VHT1SS_MCS8, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 17,  RATE_VHT1SS_MCS9, Invalue, retvalue, bIsMatched);  	
			}  
			else if (IS_RF_5G_80M(Adapter.CurrentModulation, Adapter.CurrentBandWidth) && 
					 RfMode == RF_1T1R && Adapter.EnableAllRates == FALSE)
			{

				CONVERT(TransType, 0,   RATE_VHT1SS_MCS0, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 1,   RATE_VHT1SS_MCS1, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 2,   RATE_VHT1SS_MCS2, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 3,   RATE_VHT1SS_MCS3, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 4,   RATE_VHT1SS_MCS4, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 5,   RATE_VHT1SS_MCS5, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 6,   RATE_VHT1SS_MCS6, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 7,   RATE_VHT1SS_MCS7, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 8,   RATE_VHT1SS_MCS8, Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, 9,   RATE_VHT1SS_MCS9, Invalue, retvalue, bIsMatched);  
			}                      
			else
			// RF_2.4G & Other Cases are handled here
			{				
				if (TransType == Index2Value) {
	        		retvalue = m_Rate.GetItemData(Invalue);
	        		bIsMatched = TRUE;
				} else if (TransType == Value2Index) {
	        		for (int _i = 0; _i < m_Rate.GetCount(); ++_i) {
						int i = m_Rate.GetItemData(_i);
	        			if (Invalue == i) {
	        				retvalue = _i;
	        				bIsMatched = TRUE;
							break;
	        			}
	        		}
				} else {
	        		bIsMatched = TRUE;
				} 
			}
			
			if ( !bIsMatched ) 
				ERR_INDEX_OR_VALUE_UNKNOWN(TransType, "ComboDataRate");
		} break;

		case ComboPreamble		 : {	
			if (TransType == Index2Value) 
				retvalue = ++Invalue;
			else if (TransType == Value2Index) 
				retvalue = --Invalue;
			bIsMatched = TRUE;
		} break;
		
		case ComboModulation	 : {
			CONVERT(TransType, WIRELESS_INDEX_24G, WIRELESS_MODE_N_24G, Invalue, retvalue, bIsMatched); 			
			if (IS_11AC_SERIES())
				CONVERT(TransType, WIRELESS_INDEX_5G , WIRELESS_MODE_AC_5G, Invalue, retvalue, bIsMatched);  
			else
				CONVERT(TransType, WIRELESS_INDEX_5G , WIRELESS_MODE_N_5G, Invalue, retvalue, bIsMatched);  

			if ( !bIsMatched ) ERR_INDEX_OR_VALUE_UNKNOWN(TransType, "ComboModulation");
		} break;
		
		case ComboAntennaTx: {
			if(RfMode == RF_2T4R || RfMode == RF_2T3R)
			{
				CONVERT(TransType, AntennaTx_A_Index_2T4R, ANTENNA_A , Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, AntennaTx_C_Index_2T4R, ANTENNA_C , Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, AntennaTx_AC_Index_2T4R, ANTENNA_AC, Invalue, retvalue, bIsMatched); 
				if(IS_HARDWARE_SERIES(8814A))
				{
					if(IS_2SS_RATE(Adapter.CurrentRate))
							CONVERT(TransType, 0 ,  ANTENNA_BC , Invalue, retvalue, bIsMatched);
					else 
					{
						CONVERT(TransType, AntennaTx_1_Index_2T2R ,  ANTENNA_B , Invalue, retvalue, bIsMatched);  
						CONVERT(TransType, AntennaTx_2_Index_2T2R ,  ANTENNA_C , Invalue, retvalue, bIsMatched);  	
						CONVERT(TransType, AntennaTx_12_Index_2T2R ,  ANTENNA_BC , Invalue, retvalue, bIsMatched);   
					}
				}
			}
			else if(IS_HARDWARE_SERIES(8814A) && m_CheckBox_STBC.GetCheck() == BST_CHECKED)
			{
				CONVERT(TransType, AntennaTx_12_Index_STBC,		ANTENNA_BC	  ,  Invalue, retvalue, bIsMatched);

			}
			else if(RfMode == RF_3T3R && IS_HARDWARE_SERIES(8814A))
			{
				if(IS_3SS_RATE(Adapter.CurrentRate))
				{
					CONVERT(TransType, AntennaTx_123_Index_3T3R_Nsts3,	ANTENNA_BCD	  ,  Invalue, retvalue, bIsMatched);
				}
				else if(IS_2SS_RATE(Adapter.CurrentRate))
				{
					CONVERT(TransType, AntennaTx_12_Index_3T3R_Nsts2,	ANTENNA_BC	  ,  Invalue, retvalue, bIsMatched);
					CONVERT(TransType, AntennaTx_23_Index_3T3R_Nsts2,	ANTENNA_CD	  ,  Invalue, retvalue, bIsMatched);
				}
				else
				{
					CONVERT(TransType, AntennaTx_1_Index_3T3R_Nsts1,	ANTENNA_B	  ,  Invalue, retvalue, bIsMatched);    
					CONVERT(TransType, AntennaTx_2_Index_3T3R_Nsts1,	ANTENNA_C	  ,  Invalue, retvalue, bIsMatched);    
					CONVERT(TransType, AntennaTx_3_Index_3T3R_Nsts1,	ANTENNA_D	  ,  Invalue, retvalue, bIsMatched); 
					//CONVERT(TransType, AntennaTx_Aux_Index_3T3R_Nsts1,	ANTENNA_A	  ,  Invalue, retvalue, bIsMatched);    
					if(regAdapter.iMode!=CREATOR_MODE)
					{
						CONVERT(TransType, AntennaTx_12_Index_3T3R_Nsts1,	ANTENNA_BC	  ,  Invalue, retvalue, bIsMatched);
						CONVERT(TransType, AntennaTx_123_Index_3T3R_Nsts1,	ANTENNA_BCD	  ,  Invalue, retvalue, bIsMatched); 
					}
					else
					{
						CONVERT(TransType, 3,	ANTENNA_A	  ,  Invalue, retvalue, bIsMatched);
						CONVERT(TransType, 4,	ANTENNA_BC	  ,  Invalue, retvalue, bIsMatched);
						CONVERT(TransType, 5,	ANTENNA_BCD	  ,  Invalue, retvalue, bIsMatched); 
					}
				}
			}
			else if(RfMode == RF_1T2R)
			{
				CONVERT(TransType, AntennaTx_A_Index_1T2R, ANTENNA_A, Invalue, retvalue, bIsMatched); 
				CONVERT(TransType, AntennaTx_B_Index_1T2R, ANTENNA_B, Invalue, retvalue, bIsMatched); 
			}
			else if (RfMode == RF_2T2R || RfMode == RF_2T2R_GREEN)
			{
				if (bHPConfig == TRUE)
				{
					CONVERT(TransType, 0 , ANTENNA_B , Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 1 , ANTENNA_A , Invalue, retvalue, bIsMatched); 
					CONVERT(TransType, 2 , ANTENNA_AB, Invalue, retvalue, bIsMatched); 
				}
				else
				{
					CONVERT(TransType, AntennaTx_A_Index_2T2R ,  ANTENNA_A , Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, AntennaTx_B_Index_2T2R ,  ANTENNA_B , Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, AntennaTx_AB_Index_2T2R,  ANTENNA_AB, Invalue, retvalue, bIsMatched); 
				}
				if(IS_HARDWARE_SERIES(8814A))
				{
					if(IS_2SS_RATE(Adapter.CurrentRate))
							CONVERT(TransType, 0 ,  ANTENNA_BC , Invalue, retvalue, bIsMatched);
					else
					{
						CONVERT(TransType, AntennaTx_1_Index_2T2R ,  ANTENNA_B , Invalue, retvalue, bIsMatched);  
						CONVERT(TransType, AntennaTx_2_Index_2T2R ,  ANTENNA_C , Invalue, retvalue, bIsMatched);  	
						CONVERT(TransType, AntennaTx_12_Index_2T2R ,  ANTENNA_BC , Invalue, retvalue, bIsMatched);  	
					}
				}
			}
			else if (IS_HARDWARE_SERIES(8723B) || IS_HARDWARE_SERIES(8723D))
			{
				if (bHPConfig == TRUE)
				{
					CONVERT(TransType, 1,  ANTENNA_A , Invalue, retvalue, bIsMatched);  // "2" for "S1" => PathA
					CONVERT(TransType, 0,  ANTENNA_B , Invalue, retvalue, bIsMatched);  // "1" for "S0" => PathB
				}
				else
				{
					CONVERT(TransType, AntennaTx_S1_Index_2T2R,  ANTENNA_A , Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, AntennaTx_S0_Index_2T2R,  ANTENNA_B , Invalue, retvalue, bIsMatched);  				
				}
			}
			else if (RfMode == RF_1T1R)
			{
				CONVERT(TransType, AntennaTx_A_Index_1T1R ,  ANTENNA_A , Invalue, retvalue, bIsMatched);  
			}
			
			if ( !bIsMatched ) ERR_INDEX_OR_VALUE_UNKNOWN(TransType, "(ComboAntennaTx)");
			
		} break;
		
		case ComboAntennaRx: {
			if((RfMode == RF_3T3R||RfMode == RF_2T4R) && IS_HARDWARE_SERIES(8814A))
			{
				BYTE rx_path = (BYTE)ValidRfPath & 0xff;
				if(rx_path == 0x0e)
				{
					CONVERT(TransType, 0,		ANTENNA_B,		Invalue, retvalue, bIsMatched);    
					CONVERT(TransType, 1,		ANTENNA_C,		Invalue, retvalue, bIsMatched);    
					CONVERT(TransType, 2,		ANTENNA_D,		Invalue, retvalue, bIsMatched);    
					CONVERT(TransType, 3,		ANTENNA_BC,		Invalue, retvalue, bIsMatched);   
					CONVERT(TransType, 4,		ANTENNA_CD,		Invalue, retvalue, bIsMatched);   
					CONVERT(TransType, 5,		ANTENNA_BCD,	Invalue, retvalue, bIsMatched); 
				}
				else if (rx_path == 0x0f)
				{
					CONVERT(TransType, AntennaRx_Aux_Index_3T3R,	ANTENNA_A,		Invalue, retvalue, bIsMatched);    
					CONVERT(TransType, AntennaRx_1_Index_3T3R,		ANTENNA_B,		Invalue, retvalue, bIsMatched);    
					CONVERT(TransType, AntennaRx_2_Index_3T3R,		ANTENNA_C,		Invalue, retvalue, bIsMatched);    
					CONVERT(TransType, AntennaRx_3_Index_3T3R,		ANTENNA_D,		Invalue, retvalue, bIsMatched);    
					CONVERT(TransType, AntennaRx_12_Index_3T3R,		ANTENNA_BC,		Invalue, retvalue, bIsMatched);   
					CONVERT(TransType, AntennaRx_23_Index_3T3R,		ANTENNA_CD,		Invalue, retvalue, bIsMatched);   
					CONVERT(TransType, AntennaRx_123_Index_3T3R,	ANTENNA_BCD,	Invalue, retvalue, bIsMatched); 
					CONVERT(TransType, AntennaRx_Aux123_Index_3T3R,	ANTENNA_ABCD,	Invalue, retvalue, bIsMatched); 
				}
			}
			else if(RfMode == RF_2T4R)
			{
				CONVERT(TransType, AntennaRx_A_Index_2T4R,   ANTENNA_A	  ,  Invalue, retvalue, bIsMatched       );    
				CONVERT(TransType, AntennaRx_B_Index_2T4R,   ANTENNA_B	  ,  Invalue, retvalue, bIsMatched       );    
				CONVERT(TransType, AntennaRx_C_Index_2T4R,   ANTENNA_C	  ,  Invalue, retvalue, bIsMatched       );    
				CONVERT(TransType, AntennaRx_D_Index_2T4R,   ANTENNA_D	  ,  Invalue, retvalue, bIsMatched       );    
				CONVERT(TransType, AntennaRx_AC_Index_2T4R  ,ANTENNA_AC  ,     Invalue, retvalue, bIsMatched    );   
				CONVERT(TransType, AntennaRx_BD_Index_2T4R  ,ANTENNA_BD  ,     Invalue, retvalue, bIsMatched    );   
				CONVERT(TransType, AntennaRx_ABCD_Index_2T4R,ANTENNA_ABCD,     Invalue, retvalue, bIsMatched    ); 
			}
			else if(RfMode == RF_2T3R && IS_HARDWARE_SERIES(8814A) )
			{
				CONVERT(TransType, AntennaRx_1_Index_2T3R,		ANTENNA_B,		Invalue, retvalue, bIsMatched);    
				CONVERT(TransType, AntennaRx_2_Index_2T3R,		ANTENNA_C,		Invalue, retvalue, bIsMatched);    
				CONVERT(TransType, AntennaRx_3_Index_2T3R,		ANTENNA_D,		Invalue, retvalue, bIsMatched);    
				CONVERT(TransType, AntennaRx_12_Index_2T3R,		ANTENNA_BC,		Invalue, retvalue, bIsMatched);   
				CONVERT(TransType, AntennaRx_23_Index_2T3R,		ANTENNA_CD,		Invalue, retvalue, bIsMatched);   
				CONVERT(TransType, AntennaRx_123_Index_2T3R,	ANTENNA_BCD,	Invalue, retvalue, bIsMatched); 
			}
			else if(RfMode == RF_1T2R)
			{	
				CONVERT(TransType, AntennaRx_A_Index_1T2R ,ANTENNA_A ,  Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, AntennaRx_B_Index_1T2R ,ANTENNA_B ,  Invalue, retvalue, bIsMatched);  
				CONVERT(TransType, AntennaRx_AB_Index_1T2R,ANTENNA_AB,  Invalue, retvalue, bIsMatched); 
			}
			else if (RfMode == RF_2T2R ||RfMode == RF_2T2R_GREEN)
			{
				if (bHPConfig == TRUE)
				{
					CONVERT(TransType, 0 , ANTENNA_B , Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, 1 , ANTENNA_A , Invalue, retvalue, bIsMatched); 
					CONVERT(TransType, 2 , ANTENNA_AB, Invalue, retvalue, bIsMatched); 
				}
				else
				{
					CONVERT(TransType, AntennaRx_A_Index_2T2R , ANTENNA_A , Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, AntennaRx_B_Index_2T2R , ANTENNA_B , Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, AntennaRx_AB_Index_2T2R, ANTENNA_AB, Invalue, retvalue, bIsMatched); 
				}
				if(IS_HARDWARE_SERIES(8814A))
				{
					CONVERT(TransType, AntennaRx_1_Index_2T2R ,  ANTENNA_B , Invalue, retvalue, bIsMatched);  	
					CONVERT(TransType, AntennaRx_2_Index_2T2R ,  ANTENNA_C , Invalue, retvalue, bIsMatched);  	
					CONVERT(TransType, AntennaRx_12_Index_2T2R ,  ANTENNA_BC , Invalue, retvalue, bIsMatched);  	
				}
			}
			else if (IS_HARDWARE_SERIES(8723B) || IS_HARDWARE_SERIES(8723D))
			{
				if (bHPConfig == TRUE)
				{
					CONVERT(TransType, 1,  ANTENNA_A , Invalue, retvalue, bIsMatched);  // "2" for "S1" => PathA
					CONVERT(TransType, 0,  ANTENNA_B , Invalue, retvalue, bIsMatched);  // "1" for "S0" => PathB
				}
				else
				{
					CONVERT(TransType, AntennaRx_S1_Index_2T2R,  ANTENNA_A , Invalue, retvalue, bIsMatched);  
					CONVERT(TransType, AntennaRx_S0_Index_2T2R,  ANTENNA_B , Invalue, retvalue, bIsMatched);  				
				}
			}			
			else if (RfMode == RF_1T1R)
			{
				CONVERT(TransType, AntennaRx_A_Index_1T1R , ANTENNA_A , Invalue, retvalue, bIsMatched);  
			}
		    if ( !bIsMatched ) ERR_INDEX_OR_VALUE_UNKNOWN(TransType, "ComboAntennaRx");			
		} break;

		case ComboChannel: {
			NEW_CONVERT(TransType, m_Channel, Invalue, retvalue, bIsMatched);			
		if (TransType == Index2Value) {
        	retvalue = m_Channel.GetItemData(Invalue);
        	bIsMatched = TRUE;
        } else if (TransType == Value2Index) {
        	for (int _i = 0; _i < m_Channel.GetCount(); ++_i) {
        		if (Invalue == m_Channel.GetItemData(_i)) {
        			retvalue = _i;
        			bIsMatched = TRUE;
        		}
        	}
        } else {
        	bIsMatched = TRUE;
        } 
			if ( !bIsMatched ) ERR_INDEX_OR_VALUE_UNKNOWN(TransType, "ComboChannel");
 		} break; 
	}

	return(retvalue);
}
#pragma endregion Conversion

#pragma region [Misc.]
UINT CMP819xVCDlg::GetSelectedTxPowerControl()
{
    int nCurrSelIdx;
    long lItemDataA, lItemDataB, lItemDataC=0, lItemDataD=0;

    nCurrSelIdx = m_TxPower.GetCurSel();
    lItemDataA = m_TxPower.GetItemData(nCurrSelIdx);

	if (IS_HARDWARE_SERIES(8723B) || IS_HARDWARE_SERIES(8703B) || IS_HARDWARE_SERIES(8723D))
	{
		lItemDataB = m_TxPower.GetItemData(nCurrSelIdx);
	} 
	else
	{
		nCurrSelIdx = m_TxPowerB.GetCurSel();
	    lItemDataB = m_TxPowerB.GetItemData(nCurrSelIdx);
		if (IS_HARDWARE_SERIES(8814A)) {
			nCurrSelIdx = m_TxPowerC.GetCurSel();
		    lItemDataC = m_TxPowerC.GetItemData(nCurrSelIdx);	
			nCurrSelIdx = m_TxPowerD.GetCurSel();
		    lItemDataD = m_TxPowerD.GetItemData(nCurrSelIdx);
		}
	}
    
    return (UINT)((lItemDataD<<24)|(lItemDataC<<16)|(lItemDataB<<8)|lItemDataA);
}

void CMP819xVCDlg::GetTXParameter()
{
	UINT	AntennaTx = 0;
	CHAR	PktStr[10] = {0};
	
	/*	Packet Length	*/
	OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	
    m_PacketLength.GetWindowText(PktStr, 10);
    Adapter.PacketLength = atoi(PktStr);

    /*if(Adapter.PacketLength < 64)
    {
        Adapter.PacketLength = 64;
        m_PacketLength.SetWindowText("64");
    }
	else */
	if(Adapter.PacketLength < 0)
    {
        Adapter.PacketLength = 0;
        m_PacketLength.SetWindowText("0");
    }
	else if(Adapter.PacketLength > 16000)
    {
        Adapter.PacketLength = 16000;
        m_PacketLength.SetWindowText("16000");
    }
	
	if(	QueryPmacMode() != TRUE	&&
			Adapter.PacketLength > 1600 && osvi.dwMajorVersion < 6)
    {
       // MTU of Ethernet v2 (WinXP) is 1500. 
       // Ref: http://en.wikipedia.org/wiki/Maximum_transmission_unit
       Adapter.PacketLength = 1600;
       m_PacketLength.SetWindowText("1600");
    }

	// If not, the "Packet Length" shown in MP UI means the whole length of 802.3 packet.
	// 802.3 header (14 Bytes) will be replaced to 802.11 header (24 Bytes)
	// HW will append CRC32. (4 bytes)
	//if (QueryPmacMode() != TRUE) // Don't Need this in Ndis6 MP Driver
	//	;//Adapter.PacketLength -= 14;

	Win32Print("GetTXParameter(): Packet Length= %d!\n",  Adapter.PacketLength);

	/* Packet Counts */
	m_TxCount.GetWindowText(PktStr, 10);
	Adapter.PacketCount = atoi(PktStr);

	Win32Print("GetTXParameter(): Packet Count= %d!\n",  Adapter.PacketCount);

	/* Packet Period */
	m_TxPMAC_Period.GetWindowText(PktStr, 10);
	Adapter.PacketPeriod = atoi(PktStr);
	Win32Print("GetTXParameter(): Packet Period= %d!\n",  Adapter.PacketPeriod);

	/*	Packet Type	*/
	if(m_TxPMAC_TypeSel.GetCurSel() == 0)
		Adapter.PacketType = Normal_PKT;
	else
		Adapter.PacketType = NDP_PKT;

	/*	STBC	*/
	if (m_CheckBox_STBC.GetCheck())
		Adapter.bSTBC = TRUE;
	else
		Adapter.bSTBC = FALSE;

	/*	LDPC	*/
	if (m_CheckBox_LDPC.GetCheck())
		Adapter.bLDPC = TRUE;
	else
		Adapter.bLDPC = FALSE;
}

void CMP819xVCDlg::SetRegByChannelForTest(HardwareID  HID) 
{
	// A workaround function for test chip only.
	int testListIndex = m_TestList.GetCurSel();
	int bResult1 = 0, bResult2 = 0, bResult3 = 0;
	int rateListIndex = m_Rate.GetCurSel();

	switch (HID) {
		case HID_8188EE:
			if ((testListIndex == CONT_TEST || testListIndex == TX_TEST) && IS_CCK_RATE(Adapter.CurrentRate)) 	// CCK mode
			{
				bResult1 = RFWriteRegData((LPADAPTER) Adapter.hFile, 0x19, 1, 0x63180); // Start to write
				if (Adapter.CurrentChannel == 1 || Adapter.CurrentChannel == 13)
					bResult2 = RFWriteRegData((LPADAPTER) Adapter.hFile, 0x1b, 1, 0x00184);
				else
					bResult2 = RFWriteRegData((LPADAPTER) Adapter.hFile, 0x1b, 1, 0x00000);		
				bResult3 = RFWriteRegData((LPADAPTER) Adapter.hFile, 0x19, 1, 0x63190); // End of writing
				if (!(bResult1 & bResult2 & bResult3)) ErrMsgBox(TEXT("Fail to set registers." ));
			}
			else 
			{
				bResult1 = RFWriteRegData((LPADAPTER) Adapter.hFile, 0x19, 1, 0x63180); // Start to write
				bResult2 = RFWriteRegData((LPADAPTER) Adapter.hFile, 0x1b, 1, 0x00000);		
				bResult3 = RFWriteRegData((LPADAPTER) Adapter.hFile, 0x19, 1, 0x63190); // End of writing
				if (!(bResult1 & bResult2 & bResult3)) ErrMsgBox(TEXT("Fail to set registers." ));
			}
			break;
		default:
			ErrMsgBox(TEXT("Unknown HW ID, fail to SetRegByChannelForTest" ));
			break;
	}

}


ULONG CMP819xVCDlg::XmitOnePacket()
{
    BOOL        bResult;
    UINT        count, TxOK, Xmited = 0;
    TCHAR       strBuf[100];
	UCHAR		pBuf[2048] = {0};

    bResult = QueryTxPacketSent(
                (LPADAPTER)(Adapter.hFile),
                (int*)&TxOK
                );

    _stprintf(strBuf, TEXT("%d"), TxOK);
    m_XmitOK.SetWindowText(strBuf);

    Adapter.PacketPattern = 0x00;

    {
		Adapter.PacketLength = 1400;

		Adapter.lpMem2[0]=0x08;
		Adapter.lpMem2[1]=0x00;
		Adapter.lpMem2[2]=0x00;
		Adapter.lpMem2[3]=0x00;

		memcpy(Adapter.lpMem2+4, UnicastDID, 6);
		memcpy(Adapter.lpMem2+10, Adapter.CurrentMAC, 6);

		for (UINT i = 16; i < 22; i++)
			Adapter.lpMem2[i] = i-16;

        for(count = 31; count < Adapter.PacketLength; count ++)
			Adapter.lpMem2[count]=Adapter.PacketPattern;

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
		memcpy(pBuf, Adapter.lpMem2, Adapter.PacketLength);
		//++++++++++++++++++++++++++++++++++++++++++++++++++++
		bResult = DLL_SendSpecificPacket(
					(LPADAPTER)Adapter.hFile,
					pBuf,			//Buffer
					(UINT16)Adapter.PacketLength,
					NULL			//Reserved
					);

		//++++++++++++++++++++++++++++++++++++++++++++++++++++

	}
	
    Sleep(500);
    bResult = QueryTxPacketSent(
                (LPADAPTER)(Adapter.hFile),
                (int*)&TxOK
                );
    _stprintf(strBuf, TEXT("%d"), TxOK);
    m_XmitOK.SetWindowText(strBuf);
	
    return 0;
}

void CMP819xVCDlg::SetTxPowerComboBox(UINT AntennaTx, BOOL Enable)
{
	if (Enable == FALSE)
	{
		m_TxPower.EnableWindow(FALSE);
		m_TxPowerB.EnableWindow(FALSE);
		m_TxPowerC.EnableWindow(FALSE);
		m_TxPowerD.EnableWindow(FALSE);
		return;
	}

	if(IS_HARDWARE_SERIES(8723D) || IS_HARDWARE_SERIES(8710B))
	{
		m_TxPower.EnableWindow(!(PwrIndexFromEfuse || TxPwrTrackSwitch));
		m_TxPowerB.EnableWindow(FALSE);
		m_TxPowerC.EnableWindow(FALSE);
		m_TxPowerD.EnableWindow(FALSE);
	}
	else if(IS_HARDWARE_SERIES(8822B))
	{
		m_TxPower.EnableWindow(!(PwrIndexFromEfuse || TxPwrTrackSwitch));
		m_TxPowerB.EnableWindow(!(PwrIndexFromEfuse || TxPwrTrackSwitch));
		m_TxPowerC.EnableWindow(FALSE);
		m_TxPowerD.EnableWindow(FALSE);
	}
	else if(AntennaTx == ANTENNA_A || IS_HARDWARE_SERIES(8723B) || IS_HARDWARE_SERIES(8703B))
	{
		m_TxPower.EnableWindow(TRUE);
		m_TxPowerB.EnableWindow(FALSE);
		m_TxPowerC.EnableWindow(FALSE);
		m_TxPowerD.EnableWindow(FALSE);
	}
	else if(AntennaTx == ANTENNA_B)	
	{
		m_TxPower.EnableWindow(FALSE);
		m_TxPowerB.EnableWindow(TRUE);		
		m_TxPowerC.EnableWindow(FALSE);	
		m_TxPowerD.EnableWindow(FALSE);
	}
	else if(AntennaTx == ANTENNA_C)	
	{
		m_TxPower.EnableWindow(FALSE);
		m_TxPowerB.EnableWindow(FALSE);		
		m_TxPowerC.EnableWindow(TRUE);	
		m_TxPowerD.EnableWindow(FALSE);
	}
	else if(AntennaTx == ANTENNA_D)	
	{
		m_TxPower.EnableWindow(FALSE);
		m_TxPowerB.EnableWindow(FALSE);		
		m_TxPowerC.EnableWindow(FALSE);	
		m_TxPowerD.EnableWindow(TRUE);
	}
	else if(AntennaTx == ANTENNA_AB)	
	{
		m_TxPower.EnableWindow(TRUE);
		m_TxPowerB.EnableWindow(TRUE);		
		m_TxPowerC.EnableWindow(FALSE);	
		m_TxPowerD.EnableWindow(FALSE);
	}
	else if(AntennaTx == ANTENNA_BC)	
	{
		m_TxPower.EnableWindow(FALSE);
		m_TxPowerB.EnableWindow(TRUE);
		m_TxPowerC.EnableWindow(TRUE);	
		m_TxPowerD.EnableWindow(FALSE);
	}
	else if(AntennaTx == ANTENNA_CD)	
	{
		m_TxPower.EnableWindow(FALSE);
		m_TxPowerB.EnableWindow(FALSE);
		m_TxPowerC.EnableWindow(TRUE);
		m_TxPowerD.EnableWindow(TRUE);
	}
	else if(AntennaTx == ANTENNA_ABC)
	{
		m_TxPower.EnableWindow(TRUE);
		m_TxPowerB.EnableWindow(TRUE);
		m_TxPowerC.EnableWindow(TRUE);	
		m_TxPowerD.EnableWindow(FALSE);
	}
	else if(AntennaTx == ANTENNA_BCD)
	{
		m_TxPower.EnableWindow(FALSE);
		m_TxPowerB.EnableWindow(TRUE);
		m_TxPowerC.EnableWindow(TRUE);
		m_TxPowerD.EnableWindow(TRUE);
	}
	else if(AntennaTx == ANTENNA_ABCD)
	{
		m_TxPower.EnableWindow(TRUE);
		m_TxPowerB.EnableWindow(TRUE);
		m_TxPowerC.EnableWindow(TRUE);
		m_TxPowerD.EnableWindow(TRUE);
	}
}

void CMP819xVCDlg::ChangeAntenna()
{
    UINT	i, tmpAntennaTx, tmpAntennaRx, tmpTxPower;
	
	i = m_AntennaTx.GetCurSel();
	tmpAntennaTx = IndexValueConvert(ComboAntennaTx, Index2Value, i);
	i = m_AntennaRx.GetCurSel();
	tmpAntennaRx = IndexValueConvert(ComboAntennaRx, Index2Value, i);

	if(IS_3SS_RATE(Adapter.CurrentRate) || IS_4SS_RATE(Adapter.CurrentRate))	/* MCS16~MCS31 */
	{
		if( tmpAntennaTx != ANTENNA_ABC && tmpAntennaTx != ANTENNA_BCD)
		{
			tmpAntennaTx = ANTENNA_ABC;

			if(IS_HARDWARE_SERIES(8814A))
				tmpAntennaTx = ANTENNA_BCD;
		}
	}
	else if(IS_2SS_RATE(Adapter.CurrentRate) ||
			m_CheckBox_STBC.GetCheck() == BST_CHECKED)	/* MCS8~MCS15 */
	{
		if( tmpAntennaTx != ANTENNA_AB && tmpAntennaTx != ANTENNA_BC && tmpAntennaTx != ANTENNA_CD)
		{
			tmpAntennaTx = ANTENNA_AB;
			
			if(IS_HARDWARE_SERIES(8814A))
				tmpAntennaTx = ANTENNA_BC;
		}
	}
	else if(IS_CCK_RATE(Adapter.CurrentRate)) /* CCK rate */
	{	// CCK rate needs only one antenna to rx and tx
		if(RfMode == RF_3T3R && !IS_HARDWARE_SERIES(8814A))
		{

			if( tmpAntennaTx != ANTENNA_A && tmpAntennaTx != ANTENNA_B 
				&& tmpAntennaTx != ANTENNA_C && tmpAntennaTx != ANTENNA_D)
			{
				tmpAntennaTx = ANTENNA_A;
			}
		}
		else if(RfMode == RF_2T2R || RfMode == RF_2T2R_GREEN || RfMode == RF_2T4R || RfMode == RF_2T3R )
		{	
			if (!IS_HARDWARE_SERIES(8822B)&&!IS_HARDWARE_SERIES(8814A)){ // 8822B Don't Need This Restristion
				if( tmpAntennaRx != ANTENNA_A && tmpAntennaRx != ANTENNA_B){
					tmpAntennaRx = ANTENNA_A;
				}
			}
		}
		else if(RfMode == RF_1T2R)
			tmpAntennaTx = ANTENNA_A;
	}
	else /* legacy OFDM && MCS0~MCS7 */
	{	
		if(RfMode == RF_3T3R  && !IS_HARDWARE_SERIES(8814A))
		{
			if( tmpAntennaTx != ANTENNA_A && tmpAntennaTx != ANTENNA_B 
				&& tmpAntennaTx != ANTENNA_C && tmpAntennaTx != ANTENNA_D)
			{
				tmpAntennaTx = ANTENNA_A;	
			}
		}
		else if(RfMode == RF_2T2R || RfMode == RF_2T2R_GREEN|| RfMode == RF_2T4R||RfMode == RF_2T3R)
		{	
			// <20130208, Kordan> 92E supports 2T on OFDM rates. (Asked by Edlu)
			// 8822B Don't Need This Restristion
			if (!(IS_HARDWARE_SERIES(8812A) ||IS_HARDWARE_SERIES(8192E) || IS_HARDWARE_SERIES(8822B)||IS_HARDWARE_SERIES(8814A) ||IS_HARDWARE_SERIES(8192F) ||IS_HARDWARE_SERIES(8725A))) 
			{
				if(tmpAntennaTx == ANTENNA_AB)
					tmpAntennaTx = ANTENNA_A;
			}
		}
	}

	if(RfMode == RF_2T2R || RfMode == RF_2T2R_GREEN)
	{	// Rx should be included in the Tx rf
		if (!(IS_HARDWARE_SERIES(8822B)||IS_HARDWARE_SERIES(8814A))){ // 8822B Don't Need This Restristion
		if(tmpAntennaTx==ANTENNA_A && tmpAntennaRx==ANTENNA_B)
			tmpAntennaRx = ANTENNA_A;
		if(tmpAntennaTx==ANTENNA_B && tmpAntennaRx==ANTENNA_A)
			tmpAntennaRx = ANTENNA_B;
		}
    }
	else if(RfMode == RF_1T2R)
	{			
		if(tmpAntennaTx != ANTENNA_A)
			tmpAntennaTx = ANTENNA_A;			
	}		

	if (IS_HARDWARE_SERIES(8723B) || IS_HARDWARE_SERIES(8703B) || IS_HARDWARE_SERIES(8723D)||IS_HARDWARE_SERIES(8710B))
	{
		tmpAntennaRx = tmpAntennaTx;			
	}

	UpdateAntennaTxRx(tmpAntennaTx, tmpAntennaRx, 1);
	SetTxPowerComboBox(tmpAntennaTx, TRUE);
	
	tmpTxPower = GetCurrChannelTxPower();
    if(Adapter.CurrentTxPower != tmpTxPower)
    {
    	UpdateTxPower(tmpTxPower, 0);
    }
}

UINT CMP819xVCDlg::GetEfuseSize(UINT hardwareID)
{
	return 512; // The logical EFUSE size is 512 starting from 88E to 8821A.
}


BOOL CMP819xVCDlg::
GetValueFromControl(
    IN  PVOID	pCEditlToRead,	// CEdit Control to read value.
    IN  UINT	unValueByteCnt,	// Byte count of the value expected.
    OUT ULONG	*punValueRead	// The value read.
    )
{
    CHAR    szValue[16];
    UINT    unValueChCnt;
    UINT    unValue;
    CEdit*  pEdit = (CEdit*)pCEditlToRead;

    // Initialize output value.
    *punValueRead = 0;

    // Get register offset string.
    unValueChCnt = pEdit->GetWindowText(szValue, 9); // 8 digits plus 1 EOS.

    if(unValueChCnt == 0)
        return FALSE;

    // Translate offset to number.
    if(unValueChCnt == 0 ||
        sscanf(szValue, TEXT("%X"), &unValue) == 0
        )
        return FALSE;

    if((ULONGLONG)unValue >= ((ULONGLONG)1 << (unValueByteCnt * 8)))
        return FALSE;

    // Set up value to return.
    *punValueRead = unValue;

    return TRUE;
}

BOOL CMP819xVCDlg::RegReadByExpression(PVOID pCEditlToRead)
{
    CHAR    szValue[16];
    UINT    unValueChCnt;
    UINT    unValue;
	UINT	bitStart=32, bitEnd=32, Offset, mask=0, i;
    CEdit*  pEdit = (CEdit*)pCEditlToRead;

    // Get register offset string.
    unValueChCnt = pEdit->GetWindowText(szValue, 12); // 8 digits plus 1 EOS.

    if(unValueChCnt == 0)
        return FALSE;

    // Translate offset to number.
    if(sscanf(szValue, TEXT("%X[%d:%d]"), &Offset, &bitEnd, &bitStart) == 0)
        return FALSE;

	if (bitStart == 32 || bitEnd == 32)
		return FALSE;

	unValue = RegisterRead(Offset);
	unValue >>= bitStart;
	for (i = 0; i < (bitEnd - bitStart)+1; ++i)
		mask += 1 << i;
	unValue &= mask;
	sprintf(szValue, "%X", unValue);
	m_RegData.SetWindowText(szValue);

    return TRUE;
}

BOOL CMP819xVCDlg::RegWriteByExpression()
{
    CHAR    szValue[16], szData[16];
    UINT    unValueChCnt, unValueChCnt2;
    UINT    unValue;
	UINT	bitStart=32, bitEnd=32, Offset, Data, mask=0, i;

    // Get register offset string.
    unValueChCnt = m_RegOffset.GetWindowText(szValue, 12); // 8 digits plus 1 EOS.
	unValueChCnt2 = m_RegData.GetWindowText(szData, 12); // 8 digits plus 1 EOS.

	if(unValueChCnt == 0 || unValueChCnt2 == 0)
        return FALSE;

    // Translate offset to number.
    if(sscanf(szValue, TEXT("%X[%d:%d]"), &Offset, &bitEnd, &bitStart) == 0 ||
	   sscanf(szData, TEXT("%X"), &Data) == 0	)
        return FALSE;

	if (bitStart == 32 || bitEnd == 32)
		return FALSE;

	
	for (i = 0; i < (bitEnd - bitStart)+1; ++i)
		mask += 1 << i;
	mask <<= bitStart;
	unValue = RegisterRead(Offset);
	unValue &= ( ~ mask);
	unValue |= (Data << bitStart);

	sprintf(szValue, "%X", unValue);
	bool result = (RegisterWrite(Offset, unValue) != 0);

	m_RegData.SetWindowText(szValue);

    return TRUE;
}

VOID CMP819xVCDlg::
SetTxPowerControlRange(
    UCHAR       btMin,
    UCHAR       btMax
    )
{
    UCHAR   btValue;
    TCHAR   szTmp[100];
    long	lIdx;

    // Clear content of list.
    m_TxPower.ResetContent();
    m_TxPowerB.ResetContent();
    m_TxPowerC.ResetContent();	
	m_TxPowerD.ResetContent();	

    // Add items into list.
    for( btValue = btMin; btValue <= btMax ; btValue++)
    {
        _stprintf(szTmp, TEXT("%d"), btValue);
        lIdx = m_TxPower.AddString(szTmp);
        m_TxPowerB.AddString(szTmp);
        m_TxPowerC.AddString(szTmp);
		m_TxPowerD.AddString(szTmp);

        m_TxPower.SetItemData(lIdx, btValue);
        m_TxPowerB.SetItemData(lIdx, btValue);
        m_TxPowerC.SetItemData(lIdx, btValue);		
		m_TxPowerD.SetItemData(lIdx, btValue);	
    }

	if(RfMode == RF_1T1R && ! (IS_HARDWARE_SERIES(8723B) || IS_HARDWARE_SERIES(8703B)) || IS_HARDWARE_SERIES(8723D)) {
		m_TxPowerB.EnableWindow(FALSE);
	}
	if(RfMode != RF_3T3R) {
		m_TxPowerC.EnableWindow(FALSE);		
		m_TxPowerD.EnableWindow(FALSE);	
	}
}

VOID CMP819xVCDlg::
SelectTxPowerControl(
    UINT btValue
    )
{
    int     nIdx;
    int     nNumItem;
    long	lItemData;
    long	txPwrA, txPwrB, txPwrC, txPwrD;
	
	txPwrA = (btValue&0xff);
	txPwrB = ((btValue&0xff00)>>8);
	txPwrC = ((btValue&0xff0000)>>16);
	txPwrD = ((btValue&0xff000000)>>24);
	
    nNumItem = m_TxPower.GetCount();
    for(nIdx=0; nIdx<nNumItem; nIdx++)
    {
        lItemData = m_TxPower.GetItemData(nIdx);
        if((unsigned char)lItemData == txPwrA)
        {
            m_TxPower.SetCurSel(nIdx);
            break;
        }
    }
    
    nNumItem = m_TxPowerB.GetCount();
    for(nIdx=0; nIdx<nNumItem; nIdx++)
    {
        lItemData = m_TxPowerB.GetItemData(nIdx);
        if((unsigned char)lItemData == txPwrB)
        {
            m_TxPowerB.SetCurSel(nIdx);
            break;
        }
    }

    nNumItem = m_TxPowerC.GetCount();
    for(nIdx=0; nIdx<nNumItem; nIdx++)
    {
        lItemData = m_TxPowerC.GetItemData(nIdx);
        if((unsigned char)lItemData == txPwrC)
        {
            m_TxPowerC.SetCurSel(nIdx);
            break;
        }
    }	

	nNumItem = m_TxPowerD.GetCount();
    for(nIdx=0; nIdx<nNumItem; nIdx++)
    {
        lItemData = m_TxPowerD.GetItemData(nIdx);
        if((unsigned char)lItemData == txPwrD)
        {
            m_TxPowerD.SetCurSel(nIdx);
            break;
        }
    }	
}

#pragma endregion Others

// Tooltips
VOID CMP819xVCDlg::OnTooltipInit(HWND hwnd)
{
	#include "commctrl.h"   // Tooltip
	TCHAR Buf1[50];
	sprintf(Buf1,"");
	// Reset tooltip title
	::SendMessage(hwnd, TTM_SETTITLE, TTI_NONE, (LPARAM)Buf1);
	// enable multi-line tooltips
	::SendMessage(hwnd, TTM_SETMAXTIPWIDTH, 0, 300);
	// faster response time
	::SendMessage(hwnd, TTM_SETDELAYTIME, TTDT_INITIAL, 50);
	::SendMessage(hwnd, TTM_SETDELAYTIME, TTDT_RESHOW, 0);
	
}
BOOL CMP819xVCDlg::OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TCHAR Buf[260];
	CString Str1;
	sprintf(Buf,"");
	OnTooltipInit(pNMHDR->hwndFrom);

	// Get the tooltip structure.
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;

    // Actually the idFrom holds Control's handle.
    UINT CtrlHandle = pNMHDR->idFrom;

    // Check once again that the idFrom holds handle itself.
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // Get the control's ID.
        UINT nID = ::GetDlgCtrlID( HWND( CtrlHandle ));

        // Now you have the ID. depends on control,
        // set your tooltip message.
        switch( nID )
        {
		case IDC_CHECK_ENABLE_TX_PWR_LMT:
			pTTT->lpszText = _T("Check this to enable Tx Power Limit Feature.");
			break;

		case IDC_CUSTOMER_SEL_COM:
			m_Button_CSC.GetWindowText(Buf,260);
			Str1.Format("%s",Buf);
			if(Str1 == "Change settings")
				pTTT->lpszText = _T("Change current power table settings.");
			else if(Str1 == "Update changes")
				pTTT->lpszText = _T("This action will require the device to be re-enabled.");
			else
				;
			
			break;

        case IDC_CUSTOMER_SEL_PWR:
			sprintf(Buf,"Power by rate file:");
			// Add tooltip title
			::SendMessage(pNMHDR->hwndFrom, TTM_SETTITLE, TTI_INFO, (LPARAM)Buf);
			if(m_CustomerSelPwr.GetCurSel()==2)
				pTTT->lpszText = _T(CMP819xVCDlg::CS_PWR_FILE);
			else
				pTTT->lpszText = _T("Default");
            break;

        case IDC_CUSTOMER_SEL_PWR_LMT:
			sprintf(Buf,"Power Limit file:");
			// Add tooltip title
			::SendMessage(pNMHDR->hwndFrom, TTM_SETTITLE, TTI_INFO, (LPARAM)Buf);
            if(m_CustomerSelPwrLmt.GetCurSel()==2)
				pTTT->lpszText = _T(CMP819xVCDlg::CS_PWR_LMT_FILE);
			else
				pTTT->lpszText = _T("Default");
            break;
	
		case IDC_CHECK_PHY_OK:
			pTTT->lpszText = _T("PHY \nRX \nOK");
			break;
		case IDC_CHECK_PHY_ERR:
			pTTT->lpszText = _T("PHY \nRX \nERR");
			break;
		case IDC_CHECK_MAC_OK:
			pTTT->lpszText = _T("MAC \nRX \nOK");
			break;
		case IDC_CHECK_MAC_ERR:
			pTTT->lpszText = _T("MAC \nRX \nERR");
			break;

		case IDC_PHY_RX_COUNTER_OK:
			pTTT->lpszText = _T("PHY RX OK");
			break;
		case IDC_PHY_RX_COUNTER_ERR:
			pTTT->lpszText = _T("PHY RX ERR");
			break;
		case IDC_MAC_RX_COUNTER_OK:
			pTTT->lpszText = _T("MAC RX OK");
			break;
		case IDC_MAC_RX_COUNTER_ERR:
			pTTT->lpszText = _T("MAC RX ERR");
			break;

        default:
            // Set the tooltip text.
            //pTTT->lpszText = _T("Tooltips everywhere!!!");
            break;
        }

        return TRUE;
    }

    // Not handled.
    return FALSE;
}

#pragma endregion Utility
