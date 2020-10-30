#include "stdafx.h"
#include "MP819xVCDlg.h"
#include "Feature.h"

#pragma region [PMAC TX]

void
PMAC_Get_TX_Param(
	PRT_PMAC_TX_INFO	pPMacTxInfo
	)
{
    MPT_RATE_E		TX_RATE = Adapter.CurrentRate;
	UCHAR			BandWidth, tmpU1B, Idx;

	// TX_RATE
	pPMacTxInfo->TX_RATE = TX_RATE;

	//	Ntx
	pPMacTxInfo->Ntx = 0;
	for(Idx = 16; Idx < 20; Idx++)
	{
		tmpU1B = (Adapter.CurrentAntenna >> Idx) & 1;
		if(tmpU1B)
			pPMacTxInfo->Ntx++;
	}

	// TXSC
	pPMacTxInfo->TX_SC = Adapter.TxSC;
	
	// STBC 
	pPMacTxInfo->bSTBC = Adapter.bSTBC;

	// LDPC 
	pPMacTxInfo->bLDPC = Adapter.bLDPC;

	// SGI
	if(Adapter.PacketPreamble == Short_Preamble)
		pPMacTxInfo->bSPreamble = TRUE;
	else if(Adapter.PacketPreamble == Short_GI)
		pPMacTxInfo->bSGI = TRUE;
	else{
		pPMacTxInfo->bSPreamble = FALSE;
		pPMacTxInfo->bSGI = FALSE;
	}

	// BandWidth
	if(Adapter.CurrentBandWidth == MPT_BW_40MHZ)
	{
		if(pPMacTxInfo->TX_SC ==  0)
			BandWidth = 1;
		else
			BandWidth = 0;
	}
	else if(Adapter.CurrentBandWidth == MPT_BW_80MHZ)
	{
		if(pPMacTxInfo->TX_SC == 0)
			BandWidth = 2;
		else if(pPMacTxInfo->TX_SC == 9 || pPMacTxInfo->TX_SC == 10)
			BandWidth = 1;
		else
			BandWidth = 0;
	}
	else
		BandWidth = 0;

	pPMacTxInfo->BandWidth = BandWidth;
	
	// Packet NDP
	pPMacTxInfo->NDP_sound = (Adapter.PacketType == NDP_PKT)?TRUE:FALSE;

	// Packet Period
	pPMacTxInfo->PacketPeriod = Adapter.PacketPeriod;

	// Packet Pattern
	pPMacTxInfo->PacketPattern = Adapter.PacketPattern;

	// Packet Count
	pPMacTxInfo->PacketCount = Adapter.PacketCount;

	// Packet Length
	pPMacTxInfo->PacketLength = Adapter.PacketLength;

	// Mac Address 
	memcpy(pPMacTxInfo->MacAddress, UnicastDID, 6);
}


void
PMAC_Enter(
	PRT_PMAC_TX_INFO	pPMacTxInfo
	)
{
	PMAC_Get_TX_Param(pPMacTxInfo);
	pPMacTxInfo->bEnPMacTx = TRUE;
	pPMacTxInfo->Mode = TestItem;
}

void
PMAC_Leave(
	PRT_PMAC_TX_INFO pPMacTxInfo
	)
{
	pPMacTxInfo->bEnPMacTx = FALSE;
}

#pragma endregion PMAC TX
