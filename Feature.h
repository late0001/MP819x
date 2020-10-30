#ifndef __FEATURE
#define __FEATURE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef struct _RT_PMAC_TX_INFO {
	UCHAR			bEnPMacTx:1;		// 0: Disable PMac 1: Enable PMac
	UCHAR			Mode:3;				// 0: Packet TX 3:Continuous TX
	UCHAR			Ntx:4;				// 0-7
	UCHAR			TX_RATE;			// MPT_RATE_E
	UCHAR			TX_RATE_HEX;
	UCHAR			TX_SC;
	UCHAR			bSGI:1;
	UCHAR			bSPreamble:1;
	UCHAR			bSTBC:1;
	UCHAR			bLDPC:1;
	UCHAR			NDP_sound:1;
	UCHAR			BandWidth:3;		// 0: 20 1:40 2:80Mhz
	UCHAR			m_STBC;				// bSTBC + 1
	USHORT			PacketPeriod;
	UINT			PacketCount;
	UINT			PacketLength;
	UCHAR			PacketPattern;
	USHORT			SFD;
	UCHAR			SignalField;
	UCHAR			ServiceField;
	USHORT			LENGTH;
	UCHAR			CRC16[2];
	UCHAR			LSIG[3];
	UCHAR			HT_SIG[6];
	UCHAR			VHT_SIG_A[6];
	UCHAR			VHT_SIG_B[4];
	UCHAR			VHT_SIG_B_CRC;
	UCHAR			VHT_Delimiter[4];
	UCHAR			MacAddress[6];
} RT_PMAC_TX_INFO, *PRT_PMAC_TX_INFO;


typedef struct _RT_PMAC_PKT_INFO {
	UCHAR			MCS;
	UCHAR			Nss;
	UCHAR			Nsts;
	UINT			N_sym;
	UCHAR			SIGA2B3;
} RT_PMAC_PKT_INFO, *PRT_PMAC_PKT_INFO;


void
PMAC_Enter(
	PRT_PMAC_TX_INFO	pPMacTxInfo
	);

void
PMAC_Leave(
	PRT_PMAC_TX_INFO pPMacTxInfo
	);

#endif
