// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  filefunc.h									//
//														//
//	External functions for file handling.				//
//														//
//////////////////////////////////////////////////////////

// CONSTANTS /////////////////////////////////////////////

// data file identifiers
#define DTF_IDNEWFILE 1
#define DTF_IDPALETTE 3
#define DTF_IDBITMAP 4
#define DTF_IDFONT 5
#define DTF_IDSOUND 6
#define DTF_IDMUSIC 7
#define DTF_IDRAWDATA 8
#define DTF_IDMOVIE 8
#define DTF_IDMODEL 9
#define DTF_IDSCENERY 10
#define DTF_IDTERRAIN 11
#define DTF_IDEOF 0xFF

// STRUCTURES ////////////////////////////////////////////

// used for storage of control settings as well as for user input recording
typedef struct TAG_CONTROL_DATA
{
	// flag or value of up key
	DWORD up;
	// flag or value of down key
	DWORD down;
	// flag or value of left key
	DWORD left;
	// flag or value of right key
	DWORD right;
	// flag or value of button 1 key
	DWORD btn1;
	// flag or value of button 2 key
	DWORD btn2;
	// flag or value of button 3 key
	DWORD btn3;
	// flag or value of button 4 key
	DWORD btn4;
	// flag or value of escape key
	DWORD esc;
	// optimization
	DWORD dwDummy[7];
} CONTROL_DATA,*CONTROL_DATA_PTR;

// PROTOTYPES ////////////////////////////////////////////

extern BOOL IsExistingConfigurationFile(LPCSTR lpFilename);

extern BOOL CreateConfigurationFile(LPCSTR lpFilename);

extern BOOL OpenConfigurationFile(LPCSTR lpFilename);

extern BOOL IsValidConfigurationFile(void);

extern void CloseConfigurationFile(void);

extern GUID LoadConfigurationGUID(void);

extern BOOL LoadConfigurationFlag(void);

extern BOOL WriteConfigurationGUID(GUID guid);

extern BOOL WriteConfigurationFlag(BOOL bFlag);

extern BOOL LoadConfigurationControlData(CONTROL_DATA_PTR cdData);

extern BOOL WriteConfigurationControlData(CONTROL_DATA_PTR cdData);

extern BOOL CreateVerificationFile(LPCSTR lpFilename);

extern BOOL OpenVerificationFile(LPCSTR lpFilename);

extern BOOL IsValidVerificationFile(void);

extern void CloseVerificationFile(void);

extern BOOL CreateDataFile(LPCSTR lpFilename);

extern BOOL OpenDataFile(LPCSTR lpFilename);

extern BOOL IsValidDataFile(void);

extern void CloseDataFile(void);

extern void WriteVerificationID(BYTE cID);

extern void WriteDataID(BYTE cID);

extern BYTE ReadVerificationID(LPDWORD lpdwPos=NULL);

extern BYTE ReadDataID(void);

extern BOOL VerifyDataFiles(LPCSTR lpFilename);

extern void ReadData(LPBYTE lpDest,
					 DWORD dwNum);

extern void WriteDataDWORD(DWORD dwData);

extern DWORD ReadDataDWORD(void);

extern void WriteDataBytes(LPBYTE lpData,
						   DWORD dwNum);

extern void WriteDataByte(BYTE cData);

extern BYTE ReadDataByte(void);

extern void SetConfigurationPath(LPSTR lpPath);

extern void SetDataPath(LPSTR lpPath);

extern void SaveConfigurationDataPath(void);

extern void LoadConfigurationDataPath(void);

// END ///////////////////////////////////////////////////

