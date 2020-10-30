#include "stdafx.h"
#include "IniIO.h"
#include <io.h>

INI_REG		regAdapter;

void 
SetupRegAdapter (
    INI_REG     *regAdapter,
    LPCTSTR     section, 
    LPCTSTR     defaultChipName, 
    LPCTSTR     defaultChipVersion, 
    LPCTSTR     defaultMapfile
    )
{
	// Get Name
	GetPrivateProfileString(
			section,
			"Name",
			defaultChipName,
			regAdapter->Name,
			100,
			CONFIG_FILE
			);
	
	// Get MP Kit Version
	GetPrivateProfileString(
			section,
			"Version",
			defaultChipVersion,
			regAdapter->Version,
			100,
			CONFIG_FILE
			);
	
	// Get Map File
	 GetPrivateProfileString(
			section,
			"Map",
			defaultMapfile,
			regAdapter->MapFile,
			100,
			CONFIG_FILE
			);
}


BOOL
InitRegAdapter()
/*++

Routine Description:

	This routine read the configuration.

Arguments:

Return Value:

	Result - TRUE: Success.

--*/
{
	MODE 	defaultMode = EXTERNAL_MODE;
	ULONG	defaultChipId = 0x8192;
	ULONG	defaultChipVer = 0x3;	
	int		mode, combo,bMultipathFor1SSRate;
	FILE	*configFile;

	if((configFile = fopen(CONFIG_FILE,"r")) == NULL)
		MessageBox(NULL, TEXT("Read config.ini fail!"), "ERROR", MB_OK|MB_ICONERROR|MB_SETFOREGROUND);		
	else
		fclose(configFile);
	if((configFile = fopen(CONFIG_CHIP_FILE,"r")) == NULL)
		MessageBox(NULL, TEXT("Read ConfigChip.ini fail!"), "ERROR", MB_OK|MB_ICONERROR|MB_SETFOREGROUND);		
	else
		fclose(configFile);

	// Get Mode
	mode = GetPrivateProfileInt(
						"USER",
						"Mode",
						(int)defaultMode,
						CONFIG_CHIP_FILE
						);
	switch(mode)
	{
	    case 0:  	regAdapter.iMode = DBG_MODE     ; break;
	    case 1:  	regAdapter.iMode = INTERNAL_MODE; break;
	    case 2:  	regAdapter.iMode = EXTERNAL_MODE; break;
		case 0xA:	regAdapter.iMode = CREATOR_MODE ; break; 
	    default: 	regAdapter.iMode = EXTERNAL_MODE; break;
	}

	// Get Combo
	combo = GetPrivateProfileInt(
						"USER",
						"Combo",
						0,
						CONFIG_CHIP_FILE
						);
	switch(combo)
	{
		case 0:  	regAdapter.bIsComboCard = FALSE; break;
	    case 1:  	regAdapter.bIsComboCard = TRUE;  break;
	    default: 	regAdapter.bIsComboCard = FALSE; break;
	}

	// Get Chip ID
	regAdapter.ChipId = GetPrivateProfileInt(
						"USER",
						"ChipId",
						(int)defaultChipId,
						CONFIG_CHIP_FILE
						);

	// Get Chip Ver
	regAdapter.ChipVer = GetPrivateProfileInt(
						"USER",
						"ChipVer",
						(int)defaultChipVer,
						CONFIG_CHIP_FILE
						);
	bMultipathFor1SSRate = GetPrivateProfileInt(
						"USER",
						"EnableMultiPathFor1SSRate",
						0,
						CONFIG_CHIP_FILE
						);
	if(bMultipathFor1SSRate)
		regAdapter.bEnableMultiPathFor1SSRate = TRUE;
	else
		regAdapter.bEnableMultiPathFor1SSRate = FALSE;
	if(regAdapter.ChipId == 0x8192 && regAdapter.ChipVer == 0x4)

        SetupRegAdapter (&regAdapter, "WLAN_8192SU", "RTL8192SU", "1.0.0.2008", "819xsu.map");

	else if(regAdapter.ChipId == 0x8192 && regAdapter.ChipVer == 0x3)
	
        SetupRegAdapter (&regAdapter, "WLAN_8192SE", "RTL8192SE", "1.0.0.2007", "819xse.map");

	else if(regAdapter.ChipId == 0x8192 && regAdapter.ChipVer == 0x5)

        SetupRegAdapter (&regAdapter, "WLAN_8192CE", "RTL8192CE", "0.0001.0922.2009", "819xce.map");

	else if(regAdapter.ChipId == 0x8192 && regAdapter.ChipVer == 0x6)

        SetupRegAdapter (&regAdapter, "WLAN_8192CU", "RTL8192CU", "0.0001.0922.2009", "819xcu.map");
		
	else if(regAdapter.ChipId == 0x8723 && regAdapter.ChipVer == 0x1)
	
        SetupRegAdapter (&regAdapter, "WLAN_8723AE", "RTL8723AE", "0.0001.0113.2011", "8723ae.map");
		
	else if(regAdapter.ChipId == 0x8723 && regAdapter.ChipVer == 0x2)
	
        SetupRegAdapter (&regAdapter, "WLAN_8723AU", "RTL8723AU", "0.0001.0301.2011", "8723au.map");

	else if(regAdapter.ChipId == 0x8723 && regAdapter.ChipVer == 0x3)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8723AS", "RTL8723AS", "0.0001.0819.2011", "8723as.map");
			
	else if(regAdapter.ChipId == 0x8188 && regAdapter.ChipVer == 0x1)
		
        SetupRegAdapter (&regAdapter, "WLAN_8188EE", "RTL8188EE", "0.0001.0113.2011", "8188ee.map");

	else if(regAdapter.ChipId == 0x8188 && regAdapter.ChipVer == 0x2)
	
        SetupRegAdapter (&regAdapter, "WLAN_8188EUS", "RTL8188EU", "0.0001.0301.2011", "8188eu.map");
		
	else if(regAdapter.ChipId == 0x8188 && regAdapter.ChipVer == 0x3)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8189ES", "RTL8188ES", "0.0001.0819.2011", "8188es.map");
		
	else if(regAdapter.ChipId == 0x8812 && regAdapter.ChipVer == 0x1)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8812AE", "RTL8812AE", "0.0001.0416.2012", "8812ae.map");
		
	else if(regAdapter.ChipId == 0x8812 && regAdapter.ChipVer == 0x2)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8812AU", "RTL8812AU", "0.0001.0405.2012", "8812au.map");
	
	else if(regAdapter.ChipId == 0x8812 && regAdapter.ChipVer == 0x3)	
	
        ;//SetupRegAdapter (&regAdapter, "WLAN_8812AU", "RTL8812AU", "0.0001.0405.2012", "8812.map");
        
	else if(regAdapter.ChipId == 0x8821 && regAdapter.ChipVer == 0x1)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8821AE", "RTL8821AE", "0.0001.0416.2012", "8821ae.map");
		
	else if(regAdapter.ChipId == 0x8821 && regAdapter.ChipVer == 0x2)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8821AU", "RTL8821AU", "0.0001.0405.2012", "8821au.map");
	
	else if(regAdapter.ChipId == 0x8821 && regAdapter.ChipVer == 0x3)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8821AS", "RTL8821AS", "0.0001.0405.2012", "8821as.map");
	
	else if(regAdapter.ChipId == 0x8192E && regAdapter.ChipVer == 0x1)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8192EE", "RTL8192EE", "0.0001.0416.2012", "8192ee.map");
		
	else if(regAdapter.ChipId == 0x8192E && regAdapter.ChipVer == 0x2)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8192EU", "RTL8192EU", "0.0001.0405.2012", "8192eu.map");
	
	else if(regAdapter.ChipId == 0x8192E && regAdapter.ChipVer == 0x3)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8192ES", "RTL8192ES", "0.0001.0405.2012", "8192es.map");

	else if(regAdapter.ChipId == 0x8723B && regAdapter.ChipVer == 0x1)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8723BE", "RTL8723BE", "0.0001.0416.2012", "8723be.map");
		
	else if(regAdapter.ChipId == 0x8723B && regAdapter.ChipVer == 0x2)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8723BU", "RTL8723BU", "0.0001.0405.2012", "8723bu.map");
	
	else if(regAdapter.ChipId == 0x8723B && regAdapter.ChipVer == 0x3)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8723BS", "RTL8723BS", "0.0001.0405.2012", "8723bs.map");
	
	else if(regAdapter.ChipId == 0x8814 && regAdapter.ChipVer == 0x1)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8814AE", "RTL8814AE", "0.0001.1017.2013", "8814ae.map");

	else if(regAdapter.ChipId == 0x8814 && regAdapter.ChipVer == 0x2)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8814AU", "RTL8814AU", "0.0001.1017.2013", "8814au.map");

	else if(regAdapter.ChipId == 0x8814 && regAdapter.ChipVer == 0x3)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8814AS", "RTL8814AS", "0.0001.1017.2013", "8814as.map");
	
	else if(regAdapter.ChipId == 0x8822B && regAdapter.ChipVer == 0x1)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8822BE", "RTL8822BE", "0.0001.1020.2014", "8822be.map");

	else if(regAdapter.ChipId == 0x8822B && regAdapter.ChipVer == 0x2)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8822BU", "RTL8822BU", "0.0001.1020.2014", "8822bu.map");

	else if(regAdapter.ChipId == 0x8822B && regAdapter.ChipVer == 0x3)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8822BS", "RTL8822BS", "0.0001.1020.2014", "8822bs.map");

	else if(regAdapter.ChipId == 0x8703B && regAdapter.ChipVer == 0x1)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8703BE", "RTL8703BE", "0.0001.1020.2014", "8703be.map");

	else if(regAdapter.ChipId == 0x8703B && regAdapter.ChipVer == 0x2)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8703BU", "RTL8703BU", "0.0001.1020.2014", "8703bu.map");

	else if(regAdapter.ChipId == 0x8703B && regAdapter.ChipVer == 0x3)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8703BS", "RTL8703BS", "0.0001.1020.2014", "8703bs.map");

	else if(regAdapter.ChipId == 0x8188F && regAdapter.ChipVer == 0x1)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8188FE", "RTL8188FE", "0.0001.1020.2014", "8188fe.map");

	else if(regAdapter.ChipId == 0x8188F && regAdapter.ChipVer == 0x2)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8188FU", "RTL8188FU", "0.0001.1020.2014", "8188fu.map");

	else if(regAdapter.ChipId == 0x8188F && regAdapter.ChipVer == 0x3)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8188FS", "RTL8188FS", "0.0001.1020.2014", "8188fs.map");

	else if(regAdapter.ChipId == 0x8723D && regAdapter.ChipVer == 0x1)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8723DE", "RTL8723DE", "0.0001.1020.2014", "8723de.map");

	else if(regAdapter.ChipId == 0x8723D && regAdapter.ChipVer == 0x2)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8723DU", "RTL8723DU", "0.0001.1020.2014", "8723du.map");

	else if(regAdapter.ChipId == 0x8723D && regAdapter.ChipVer == 0x3)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8723DS", "RTL8723DS", "0.0001.1020.2014", "8723ds.map");

	else if(regAdapter.ChipId == 0x8821C && regAdapter.ChipVer == 0x1)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8821CE", "RTL8821CE", "0.0001.1020.2014", "8821ce.map");

	else if(regAdapter.ChipId == 0x8821C && regAdapter.ChipVer == 0x2)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8821CU", "RTL8821CU", "0.0001.1020.2014", "8821cu.map");
	
	else if(regAdapter.ChipId == 0x8731A && regAdapter.ChipVer == 0x2)	

		SetupRegAdapter (&regAdapter, "WLAN_8731AU", "RTL8731AU", "0.0001.1020.2014", "8821cu.map");

	else if(regAdapter.ChipId == 0x8821C && regAdapter.ChipVer == 0x3)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8821CS", "RTL8821CS", "0.0001.1020.2014", "8821cs.map");
	
	else if(regAdapter.ChipId == 0x8733 && regAdapter.ChipVer == 0x3)	

		SetupRegAdapter (&regAdapter, "WLAN_8733AS", "RTL8733AS", "0.0001.1020.2014", "8821cs.map");

	else if(regAdapter.ChipId == 0x8710B && regAdapter.ChipVer == 0x2)
	
        SetupRegAdapter (&regAdapter, "WLAN_8188GU", "RTL8188GU", "0.0001.1020.2014", "8188gu.map");

	else if(regAdapter.ChipId == 0x8192F && regAdapter.ChipVer == 0x1)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8192FE", "RTL8192FE", "0.0001.1020.2014", "8192fe.map");
		
	else if(regAdapter.ChipId == 0x8192F && regAdapter.ChipVer == 0x2)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8192FU", "RTL8192FU", "0.0001.1020.2014", "8192fu.map");
	
	else if(regAdapter.ChipId == 0x8192F && regAdapter.ChipVer == 0x3)	
	
        SetupRegAdapter (&regAdapter, "WLAN_8192FS", "RTL8192FS", "0.0001.1020.2014", "8192fs.map");
	
	else if((regAdapter.ChipId == 0x8725A) && regAdapter.ChipVer == 0x2){	
	
        SetupRegAdapter (&regAdapter, "WLAN_8725AU", "RTL8725AU", "0.0001.1020.2014", "8725au.map");
	}
	else if((regAdapter.ChipId == 0x8725A) && regAdapter.ChipVer == 0x3){	
	
        SetupRegAdapter (&regAdapter, "WLAN_8725AS", "RTL8725AS", "0.0001.1020.2014", "8725as.map");
	}
	else 
		return FALSE;


	return TRUE;			
}
