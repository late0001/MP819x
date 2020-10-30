#ifndef __INIIO_H
#define __INIIO_H

#define CONFIG_FILE	        ".\\config.ini"
#define CONFIG_CHIP_FILE	".\\ConfigChip.ini"

typedef enum _MODE
{
	DBG_MODE = 0,
	INTERNAL_MODE = 1,
	EXTERNAL_MODE = 2,
	CREATOR_MODE = 0xA
}MODE;

typedef struct _INI_REG
{
	MODE	iMode;
	ULONG	ChipId;
	ULONG	ChipVer;
	TCHAR	Name[100];
	TCHAR	Version[50];	// MP Kit Version
	TCHAR	MapFile[100];
	TCHAR	MapFile56[100];	// for eeprom 93C56
	TCHAR	MapFile_2[100];	//for D-cut, 92u
	BOOLEAN bIsComboCard;
	BOOLEAN bEnableMultiPathFor1SSRate;
}INI_REG, *PINI_REG;

extern	INI_REG		regAdapter;

BOOL InitRegAdapter();
#endif
