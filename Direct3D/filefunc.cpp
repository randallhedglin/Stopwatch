// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  filefunc.cpp									//
//														//
//	Functions for handling file access.					//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// header for this module
#include"filefunc.h"
// allow access to window handles
#include"winmain.h"

// ERRORS ////////////////////////////////////////////////

// configuration file already open
#define ERR_FILEFUNC00 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 00\nConfiguration file already open.");
// configuration file creation failed
#define ERR_FILEFUNC01 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 01\nUnable to create configuration file.");
// configuration file write failed
#define ERR_FILEFUNC02 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 02\nUnable to write to configuration file.");
// configuration file failed to open
#define ERR_FILEFUNC03 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 03\nUnable to open configuration file.");
// configuration file read failed
#define ERR_FILEFUNC04 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 04\nUnable to read from configuration file, or it is invalid.");
// invalid configuration file
#define ERR_FILEFUNC05 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 05\nInvalid configuration file.");
// verification file already open
#define ERR_FILEFUNC06 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 06\nVerification file already open.");
// verification file creation failed
#define ERR_FILEFUNC07 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 07\nUnable to create verification file.");
// verification file write failed
#define ERR_FILEFUNC08 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 08\nUnable to write to verification file.");
// verification file failed to open
#define ERR_FILEFUNC09 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 09\nUnable to open verification file.");
// verification file read failed
#define ERR_FILEFUNC10 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 10\nUnable to read from verification file, or it is invalid.");
// invalid verification file
#define ERR_FILEFUNC11 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 11\nInvalid verification file.");
// data file already open
#define ERR_FILEFUNC12 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 12\nData file already open.");
// data file creation failed
#define ERR_FILEFUNC13 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 13\nUnable to create data file.");
// data file write failed
#define ERR_FILEFUNC14 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 14\nUnable to write to data file.");
// data file failed to open
#define ERR_FILEFUNC15 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 15\nUnable to open data file.");
// data file read failed
#define ERR_FILEFUNC16 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 16\nUnable to read from data file, or it is invalid.");
// invalid data file
#define ERR_FILEFUNC17 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 17\nInvalid data file.");
// unable to read data file info
#define ERR_FILEFUNC18 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 18\nUnable to access data file information.");
// unable to move through data file
#define ERR_FILEFUNC19 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FILEFUNC.CPP\nError Code: 19\nUnable to traverse data file.");

// MESSAGES //////////////////////////////////////////////

// reconfigure req'd
#define MSG_FILEFUNC00 DISPLAY_INFO(GetGlobalHWnd(),"Program must be reconfigured.\nOption selection windows will follow.");

// GLOBALS ///////////////////////////////////////////////

// handle to configuration file
HANDLE g_hCfgFile=INVALID_HANDLE_VALUE;
// handle to verification file
HANDLE g_hVerFile=INVALID_HANDLE_VALUE;
// handle to data file
HANDLE g_hDataFile=INVALID_HANDLE_VALUE;
// pointer to file data in memory
LPBYTE g_lpFileData=NULL;
// pointer to position in file data
DWORD g_dwFileDataPtr=0;
// verification file creation flag
BOOL bCreatingVerification=FALSE;
// configuration path
char g_lpCfgPath[_MAX_PATH];
// data path
char g_lpDataPath[_MAX_PATH];

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//  Function:  IsExistingConfigurationFile()			//
//	  (defined in "filefunc.h")							//
//														//
//  Checks for existence of a file.						//
//	  Parameters:										//
//		LPCSTR lpFilename: name of file					//
//	  Return Value:										//
//		BOOL: TRUE if file exists						//
//														//
//////////////////////////////////////////////////////////

extern BOOL IsExistingConfigurationFile(LPCSTR lpFilename)
{
	// handle to file
	HANDLE hFile=INVALID_HANDLE_VALUE;

	// set directory
	_chdir(g_lpCfgPath);
	// create file using OPEN_EXISTING flag
	hFile=CreateFile(lpFilename,
					 GENERIC_READ|
					 GENERIC_WRITE,
					 NULL,
					 NULL,
					 OPEN_EXISTING,
					 FILE_ATTRIBUTE_NORMAL|
					 FILE_FLAG_RANDOM_ACCESS,
					 NULL);
	// if file couldn't be created, it doesn't exist
	if(hFile==INVALID_HANDLE_VALUE)
		return(FALSE);
	// file does exist, so close it and return
	SAFE_CLOSE(hFile);
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  CreateConfigurationFile()				//
//	  (defined in "filefunc.h")							//
//														//
//  Creates and prepares a new configuration file.		//
//	  Parameters:										//
//		LPCSTR lpFilename: name of file					//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL CreateConfigurationFile(LPCSTR lpFilename)
{
	// initial text for configuration file
	char lpCfgText[]="[config]";
	// number of bytes written by WriteFile
	DWORD dwBytesWritten=0;

	// set directory
	_chdir(g_lpCfgPath);
	// save path
	_getcwd(g_lpCfgPath,_MAX_PATH);
	// create file
	g_hCfgFile=CreateFile(lpFilename,
						  GENERIC_READ|
						  GENERIC_WRITE,
						  NULL,
						  NULL,
						  CREATE_ALWAYS,
						  FILE_ATTRIBUTE_NORMAL|
						  FILE_FLAG_RANDOM_ACCESS,
						  NULL);
	// check for errors
	if(!IsValidConfigurationFile())
	{
		ERR_FILEFUNC01;
		return(FALSE);
	}
	// output initial information
	WriteFile(g_hCfgFile,
			  lpCfgText,
			  strlen(lpCfgText)+1,
			  &dwBytesWritten,
			  NULL);
	// check for errors
	if(dwBytesWritten!=strlen(lpCfgText)+1)
	{
		ERR_FILEFUNC02;
		return(FALSE);
	}
	// we're ok!
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  OpenConfigurationFile()					//
//	  (defined in "filefunc.h")							//
//														//
//  Opens existing configuration file and verifies it.	//
//	  Parameters:										//
//		LPCSTR lpFilename: name of file					//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL OpenConfigurationFile(LPCSTR lpFilename)
{
	// text for configuration file verification
	char lpCfgText[]="[xxxxxx]";
	// number of bytes read by ReadFile
	DWORD dwBytesRead=0;

	// set directory
	_chdir(g_lpCfgPath);
	// create file
	g_hCfgFile=CreateFile(lpFilename,
						  GENERIC_READ|
						  GENERIC_WRITE,
						  NULL,
						  NULL,
						  OPEN_EXISTING,
						  FILE_ATTRIBUTE_NORMAL|
						  FILE_FLAG_RANDOM_ACCESS,
						  NULL);
	// check for errors
	if(!IsValidConfigurationFile())
	{
		ERR_FILEFUNC03;
		return(FALSE);
	}
	// read initial information
	ReadFile(g_hCfgFile,
			 lpCfgText,
			 strlen("[config]")+1,
			 &dwBytesRead,
			 NULL);
	// check for errors
	if(dwBytesRead!=strlen("[config]")+1)
	{
		ERR_FILEFUNC04;
		// close invalid file
		SAFE_CLOSE(g_hCfgFile);
		// delete it
		DeleteFile(lpFilename);
		// create a new one
		MSG_FILEFUNC00;
		return(CreateConfigurationFile(lpFilename));
	}
	// verify file
	if(strcmp(lpCfgText,"[config]")!=0)
	{
		ERR_FILEFUNC05;
		// close invalid file
		SAFE_CLOSE(g_hCfgFile);
		// delete it
		DeleteFile(lpFilename);
		// create a new one
		MSG_FILEFUNC00;
		return(CreateConfigurationFile(lpFilename));
	}
	// we're ok!
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  IsValidConfigurationFile()				//
//	  (defined in "filefunc.h")							//
//														//
//  Checks validity of configuration file handle.		//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL IsValidConfigurationFile(void)
{
	if(g_hCfgFile==INVALID_HANDLE_VALUE)
		return(FALSE);
	else
		return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  CloseConfigurationFile()					//
//	  (defined in "filefunc.h")							//
//														//
//  Closes configuration file.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void CloseConfigurationFile(void)
{
	SAFE_CLOSE(g_hCfgFile);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  LoadConfigurationGUID()					//
//	  (defined in "filefunc.h")							//
//														//
//  Loads a GUID.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		GUID: value of GUID loaded						//
//														//
//////////////////////////////////////////////////////////

extern GUID LoadConfigurationGUID(void)
{
	// number of bytes read
	DWORD dwBytesRead=0;
	// GUID to return
	GUID guid;
	INIT_VAR(guid);
	
	// read GUID
	ReadFile(g_hCfgFile,
			 &guid,
			 sizeof(GUID),
			 &dwBytesRead,
			 NULL);
	// check for errors
	if(dwBytesRead!=sizeof(GUID))
		ERR_FILEFUNC04;
	return(guid);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  LoadConfigurationFlag()					//
//	  (defined in "filefunc.h")							//
//														//
//  Loads a flag.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: value of flag loaded						//
//														//
//////////////////////////////////////////////////////////

extern BOOL LoadConfigurationFlag(void)
{
	// number of bytes read
	DWORD dwBytesRead=0;
	// BOOL to return
	BOOL bRet=FALSE;
	
	// read flag
	ReadFile(g_hCfgFile,
			 &bRet,
			 sizeof(BOOL),
			 &dwBytesRead,
			 NULL);
	// check for errors
	if(dwBytesRead!=sizeof(BOOL))
		ERR_FILEFUNC04;
	return(bRet);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  WriteConfigurationGUID()					//
//	  (defined in "filefunc.h")							//
//														//
//  Outputs a GUID to configuration file				//
//	  Parameters:										//
//		GUID guid: GUID to output						//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL WriteConfigurationGUID(GUID guid)
{
	// number of bytes written
	DWORD dwBytesWritten=0;
	
	// write GUID
	WriteFile(g_hCfgFile,
			  &guid,
			  sizeof(GUID),
			  &dwBytesWritten,
			  NULL);
	// check for errors
	if(dwBytesWritten!=sizeof(GUID))
	{
		ERR_FILEFUNC02;
		return(FALSE);
	}
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  WriteConfigurationFlag()					//
//	  (defined in "filefunc.h")							//
//														//
//  Outputs a flag to configuration file				//
//	  Parameters:										//
//		BOOL bFlag: BOOL to output						//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL WriteConfigurationFlag(BOOL bFlag)
{
	// number of bytes written
	DWORD dwBytesWritten=0;
	
	// write flag
	WriteFile(g_hCfgFile,
			  &bFlag,
			  sizeof(BOOL),
			  &dwBytesWritten,
			  NULL);
	// check for errors
	if(dwBytesWritten!=sizeof(BOOL))
	{
		ERR_FILEFUNC02;
		return(FALSE);
	}
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  LoadConfigurationControlData()			//
//	  (defined in "filefunc.h")							//
//														//
//  Loads control data.									//
//	  Parameters:										//
//		CONTROL_DATA_PTR: pointer to data				//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL LoadConfigurationControlData(CONTROL_DATA_PTR cdData)
{
	// number of bytes read
	DWORD dwBytesRead=0;
	
	// read data
	ReadFile(g_hCfgFile,
			 cdData,
			 sizeof(CONTROL_DATA),
			 &dwBytesRead,
			 NULL);
	// check for errors
	if(dwBytesRead!=sizeof(CONTROL_DATA))
	{
		ERR_FILEFUNC04;
		return(FALSE);
	}
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  WriteConfigurationControlData()			//
//	  (defined in "filefunc.h")							//
//														//
//  Outputs control data to configuration file.			//
//	  Parameters:										//
//		CONTROL_DATA_PTR: pointer to data				//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL WriteConfigurationControlData(CONTROL_DATA_PTR cdData)
{
	// number of bytes written
	DWORD dwBytesWritten=0;
	
	// write data
	WriteFile(g_hCfgFile,
			  cdData,
			  sizeof(CONTROL_DATA),
			  &dwBytesWritten,
			  NULL);
	// check for errors
	if(dwBytesWritten!=sizeof(CONTROL_DATA))
	{
		ERR_FILEFUNC02;
		return(FALSE);
	}
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  CreateVerificationFile()					//
//	  (defined in "filefunc.h")							//
//														//
//  Creates and prepares a new verification file.		//
//	  Parameters:										//
//		LPCSTR lpFilename: name of file					//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL CreateVerificationFile(LPCSTR lpFilename)
{
	// set directory
	_chdir(g_lpDataPath);
	// create file
	g_hVerFile=CreateFile(lpFilename,
						  GENERIC_WRITE,
						  NULL,
						  NULL,
						  CREATE_ALWAYS,
						  FILE_ATTRIBUTE_NORMAL,
						  NULL);
	// check for errors
	if(!IsValidVerificationFile())
	{
		ERR_FILEFUNC07;
		return(FALSE);
	}
	// set flag
	bCreatingVerification=TRUE;
	// we're ok!
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  OpenVerificationFile()					//
//	  (defined in "filefunc.h")							//
//														//
//  Opens existing verification file.					//
//	  Parameters:										//
//		LPCSTR lpFilename: name of file					//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL OpenVerificationFile(LPCSTR lpFilename)
{
	// set directory
	_chdir(g_lpDataPath);
	// open file
	g_hVerFile=CreateFile(lpFilename,
								GENERIC_READ,
								NULL,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL|
								FILE_FLAG_SEQUENTIAL_SCAN,
								NULL);
	// check for errors
	if(!IsValidVerificationFile())
	{
		ERR_FILEFUNC09;
		return(FALSE);
	}
	// we're ok!
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  IsValidVerificationFile()				//
//	  (defined in "filefunc.h")							//
//														//
//  Checks validity of verification file handle.		//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL IsValidVerificationFile(void)
{
	if(g_hVerFile==INVALID_HANDLE_VALUE)
		return(FALSE);
	else
		return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  CloseVerificationFile()					//
//	  (defined in "filefunc.h")							//
//														//
//  Closes verification file.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void CloseVerificationFile(void)
{
	// output EOF flag if necessary
	if(bCreatingVerification)
	{
		WriteVerificationID(DTF_IDEOF);
		// reset flag
		bCreatingVerification=FALSE;
	}
	// close file
	SAFE_CLOSE(g_hVerFile);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  CreateDataFile()							//
//	  (defined in "filefunc.h")							//
//														//
//  Creates and prepares a new data file.				//
//	  Parameters:										//
//		LPCSTR lpFilename: name of file					//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL CreateDataFile(LPCSTR lpFilename)
{
	// bytes written by WriteFile()
	DWORD dwBytesWritten=0;
	
	// set directory
	_chdir(g_lpDataPath);
	// create file
	g_hDataFile=CreateFile(lpFilename,
						   GENERIC_WRITE,
						   NULL,
						   NULL,
						   CREATE_ALWAYS,
						   FILE_ATTRIBUTE_NORMAL,
						   NULL);
	// check for errors
	if(!IsValidDataFile())
	{
		ERR_FILEFUNC13;
		return(FALSE);
	}
	// output ID to verification file
	WriteVerificationID(DTF_IDNEWFILE);
	// output filename to verification file
	WriteFile(g_hVerFile,
			  lpFilename,
			  strlen(lpFilename)+1,
			  &dwBytesWritten,
			  NULL);
	// check for errors
	if(dwBytesWritten!=strlen(lpFilename)+1)
	{
		ERR_FILEFUNC08;
		return(FALSE);
	}
	// we're ok!
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  OpenDataFile()							//
//	  (defined in "filefunc.h")							//
//														//
//  Opens existing data file.							//
//	  Parameters:										//
//		LPCSTR lpFilename: name of file					//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL OpenDataFile(LPCSTR lpFilename)
{
	// set directory
	_chdir(g_lpDataPath);
	// open file for verification
	if(IsValidVerificationFile())
		g_hDataFile=CreateFile(lpFilename,
							   GENERIC_READ,
							   NULL,
							   NULL,
							   OPEN_EXISTING,
							   FILE_ATTRIBUTE_NORMAL|
							   FILE_FLAG_RANDOM_ACCESS,
							   NULL);
	// or for reading
	else
		g_hDataFile=CreateFile(lpFilename,
							   GENERIC_READ,
							   NULL,
							   NULL,
							   OPEN_EXISTING,
							   FILE_ATTRIBUTE_NORMAL|
							   FILE_FLAG_SEQUENTIAL_SCAN,
							   NULL);
	// check for errors
	if(!IsValidDataFile())
	{
		ERR_FILEFUNC15;
		return(FALSE);
	}
	// if reading, load entire file into memory (for speed)
	if(!IsValidVerificationFile())
	{
		// file information structure
		BY_HANDLE_FILE_INFORMATION bhfi;
		INIT_VAR(bhfi);
		// bytes read by ReadFile()
		DWORD dwBytesRead=0;

		// read file info
		if(!GetFileInformationByHandle(g_hDataFile,&bhfi))
		{
			ERR_FILEFUNC18;
			return(FALSE);
		}
		// allocate memory
		SAFE_ALLOC(g_lpFileData,
				   LPBYTE,
				   bhfi.nFileSizeLow);
		// check for errors
		if(!g_lpFileData)
			return(FALSE);
		// read data
		ReadFile(g_hDataFile,
				 g_lpFileData,
				 bhfi.nFileSizeLow,
				 &dwBytesRead,
				 NULL);
		// check for errors
		if(dwBytesRead!=bhfi.nFileSizeLow)
		{
			ERR_FILEFUNC16;
			return(FALSE);
		}
		// close file
		SAFE_CLOSE(g_hDataFile);
		// reset pointer
		g_dwFileDataPtr=0;
	}
	// we're ok!
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  IsValidDataFile()						//
//	  (defined in "filefunc.h")							//
//														//
//  Checks validity of data file handle.				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL IsValidDataFile(void)
{
	if(g_hDataFile==INVALID_HANDLE_VALUE)
		return(FALSE);
	else
		return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  CloseDataFile()							//
//	  (defined in "filefunc.h")							//
//														//
//  Closes data file.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern void CloseDataFile(void)
{
	// close file for verification
	if(IsValidVerificationFile())
		SAFE_CLOSE(g_hDataFile)
	// or release memory from read
	else
		SAFE_FREE(g_lpFileData);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  WriteVerificationID()					//
//		(defined in "filefunc.h")						//
//														//
//	Writes ID to verification file.						//
//	  Parameters:										//
//		BYTE: ID to write
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void WriteVerificationID(BYTE cID)
{
	// bytes written by WriteFile()
	DWORD dwBytesWritten=0;
	// storage for position
	DWORD dwPos=0;
	
	// output ID
	WriteFile(g_hVerFile,
			  &cID,
			  1,
			  &dwBytesWritten,
			  NULL);
	// check for errors
	if(dwBytesWritten!=1)
	{
		ERR_FILEFUNC08;
		return;
	}
	// get data file position if necessary
	if(IsValidDataFile())
		dwPos=SetFilePointer(g_hDataFile,
							 0,
							 NULL,
							 FILE_CURRENT);
	
	// write position
	WriteFile(g_hVerFile,
			  &dwPos,
			  sizeof(DWORD),
			  &dwBytesWritten,
			  NULL);
	// check for errors
	if(dwBytesWritten!=sizeof(DWORD))
	{
		ERR_FILEFUNC08;
		return;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  WriteDataID()							//
//		(defined in "filefunc.h")						//
//														//
//	Writes ID to data file.								//
//	  Parameters:										//
//		BYTE: ID to write								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void WriteDataID(BYTE cID)
{
	// bytes written by WriteFile()
	DWORD dwBytesWritten=0;
	
	// output ID
	WriteFile(g_hDataFile,
			  &cID,
			  1,
			  &dwBytesWritten,
			  NULL);
	// check for errors
	if(dwBytesWritten!=1)
	{
		ERR_FILEFUNC14;
		return;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  ReadVerificationID()						//
//		(defined in "filefunc.h")						//
//														//
//	Reads ID from verification file.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BYTE: ID that was read							//
//														//
//////////////////////////////////////////////////////////

extern BYTE ReadVerificationID(LPDWORD lpdwPos)
{
	// bytes read by ReadFile()
	DWORD dwBytesRead=0;
	// return value
	BYTE cID=0;
	
	// input ID
	ReadFile(g_hVerFile,
			 &cID,
			 1,
			 &dwBytesRead,
			 NULL);
	// check for errors
	if(dwBytesRead!=1)
	{
		ERR_FILEFUNC10;
		return(0);
	}
	// read position if necessary
	if(lpdwPos)
	{
		ReadFile(g_hVerFile,
				 lpdwPos,
				 sizeof(DWORD),
				 &dwBytesRead,
				 NULL);
		// check for errors
		if(dwBytesRead!=sizeof(DWORD))
		{
			ERR_FILEFUNC10;
			return(0);
		}
	}
	// read dummy data
	else
	{
		// dummy data storage
		DWORD dwDummy=0;

		ReadFile(g_hVerFile,
				 &dwDummy,
				 sizeof(DWORD),
				 &dwBytesRead,
				 NULL);
		// check for errors
		if(dwBytesRead!=sizeof(DWORD))
		{
			ERR_FILEFUNC10;
			return(0);
		}
	}
	// ok
	return(cID);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  ReadDataID()								//
//		(defined in "filefunc.h")						//
//														//
//	Reads ID from data file.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BYTE: ID that was read							//
//														//
//////////////////////////////////////////////////////////

extern BYTE ReadDataID(void)
{
	// bytes read by ReadFile()
	DWORD dwBytesRead=0;
	// return value
	BYTE cID=0;
	
	// input ID
	ReadFile(g_hDataFile,
			 &cID,
			 1,
			 &dwBytesRead,
			 NULL);
	// check for errors
	if(dwBytesRead!=1)
	{
		ERR_FILEFUNC16;
		return(0);
	}
	// ok
	return(cID);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  VerifyDataFiles()						//
//		(defined in "filefunc.h")						//
//														//
//	Scans all data files for verification.				//
//	  Parameters:										//
//		LPCSTR: name of verification file				//
//	  Return Value:										//
//		BOOL: FALSE if invalid							//
//														//
//////////////////////////////////////////////////////////

extern BOOL VerifyDataFiles(LPCSTR lpFilename)
{
	// data type storage variable
	BYTE cDataType=NULL;
	// first time flag
	BOOL bFirstTime=TRUE;
	// input storage
	BYTE cInput;
	// filename storage
	char lpDataFileName[64];
	// filename pointer
	DWORD dwDataFileNamePtr=0;
	// bytes read by ReadFile()
	DWORD dwBytesRead=0;
	// data pointer
	DWORD dwPos=0;
	
	// open verification file
	if(!OpenVerificationFile(lpFilename))
		return(FALSE);
	// verify first byte
	cDataType=ReadVerificationID();
	if(cDataType!=DTF_IDNEWFILE)
	{
		ERR_FILEFUNC17;
		return(FALSE);
	}
	// loop until EOF is reached
	while(cDataType!=DTF_IDEOF)
	{
		switch(cDataType)
		{
		// read new file
		case(DTF_IDNEWFILE):
			// close data file if necessary
			if(!bFirstTime)
				CloseDataFile();
			// clear flag
			else
				bFirstTime=FALSE;
			// reset data filename pointer
			dwDataFileNamePtr=0;
			// input byte
			ReadFile(g_hVerFile,
					 &cInput,
					 1,
					 &dwBytesRead,
					 NULL);
			// check for errors
			if(dwBytesRead!=1)
			{
				ERR_FILEFUNC16;
				return(FALSE);
			}
			// loop until NULL character is reached
			while(cInput)
			{
				// store character
				lpDataFileName[dwDataFileNamePtr++]=cInput;
				// input byte
				ReadFile(g_hVerFile,
						 &cInput,
						 1,
						 &dwBytesRead,
						 NULL);
				// check for errors
				if(dwBytesRead!=1)
				{
					ERR_FILEFUNC16;
					return(FALSE);
				}
			}
			// end string
			lpDataFileName[dwDataFileNamePtr]=0;
			// open data file
			if(!OpenDataFile(lpDataFileName))
				return(FALSE);
			break;
		default:
			// locate position in data file
			if(SetFilePointer(g_hDataFile,
							  dwPos,
							  NULL,
							  FILE_BEGIN)!=dwPos)
			{
				ERR_FILEFUNC19;
				return(FALSE);
			}
			// compare data
			if(cDataType!=ReadDataID())
			{
				ERR_FILEFUNC17;
				return(FALSE);
			}
			break;
		}
		// get next data
		cDataType=ReadVerificationID(&dwPos);
	}
	// close data file
	CloseDataFile();
	// close verification file
	CloseVerificationFile();
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  ReadData()								//
//		(defined in "filefunc.h")						//
//														//
//	Reads from data file.								//
//	  Parameters:										//
//		LPBYTE: pointer to data to be received			//
//		DWORD: number of bytes to read					//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void ReadData(LPBYTE lpDest,
					 DWORD dwNum)
{
	// copy data
	CopyMemory(lpDest,
			   &g_lpFileData[g_dwFileDataPtr],
			   dwNum);
	// update pointer
	g_dwFileDataPtr+=dwNum;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  WriteDataDWORD()							//
//		(defined in "filefunc.h")						//
//														//
//	Writes a DWORD to open data file.					//
//	  Parameters:										//
//		DWORD: value to write							//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void WriteDataDWORD(DWORD dwData)
{
	// bytes written by WriteFile()
	DWORD dwBytesWritten=0;
	
	// output DWORD
	WriteFile(g_hDataFile,
			  &dwData,
			  sizeof(DWORD),
			  &dwBytesWritten,
			  NULL);
	// check for errors
	if(dwBytesWritten!=sizeof(DWORD))
	{
		ERR_FILEFUNC14;
		return;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  ReadDataDWORD()							//
//		(defined in "filefunc.h")						//
//														//
//	Reads a DWORD from open data file.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		DWORD: value read								//
//														//
//////////////////////////////////////////////////////////

extern DWORD ReadDataDWORD(void)
{
	// bytes read by ReadFile()
	DWORD dwBytesRead=0;
	// return value
	BYTE dwData=0;
	
	// input DWORD
	ReadFile(g_hDataFile,
			 &dwData,
			 sizeof(DWORD),
			 &dwBytesRead,
			 NULL);
	// check for errors
	if(dwBytesRead!=sizeof(DWORD))
	{
		ERR_FILEFUNC16;
		return(0);
	}
	// ok
	return(dwData);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  WriteDataBytes()							//
//		(defined in "filefunc.h")						//
//														//
//	Writes bytes to open data file.						//
//	  Parameters:										//
//		LPBYTE: pointer to data							//
//		DWORD: number of bytes to write					//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void WriteDataBytes(LPBYTE lpData,
						   DWORD dwNum)
{
	// bytes written by WriteFile()
	DWORD dwBytesWritten=0;
	
	// output bytes
	WriteFile(g_hDataFile,
			  lpData,
			  dwNum,
			  &dwBytesWritten,
			  NULL);
	// check for errors
	if(dwBytesWritten!=dwNum)
	{
		ERR_FILEFUNC14;
		return;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  WriteDataByte()							//
//		(defined in "filefunc.h")						//
//														//
//	Writes single byte to open data file.				//
//	  Parameters:										//
//		BYTE: data to write								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void WriteDataByte(BYTE cData)
{
	WriteDataID(cData);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  ReadDataByte()							//
//		(defined in "filefunc.h")						//
//														//
//	Reads single byte from open data file.				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BYTE: data that was read						//
//														//
//////////////////////////////////////////////////////////

extern BYTE ReadDataByte(void)
{
	return(ReadDataID());
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetConfigurationPath()					//
//		(defined in "filefunc.h")						//
//														//
//	Sets path to read configuration file from.			//
//	  Parameters:										//
//		LPSTR: path										//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetConfigurationPath(LPSTR lpPath)
{
	// copy path
	CopyMemory(g_lpCfgPath,	
			   lpPath,
			   _MAX_PATH);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetDataPath()							//
//		(defined in "filefunc.h")						//
//														//
//	Sets path to read data files from.					//
//	  Parameters:										//
//		LPSTR: path										//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetDataPath(LPSTR lpPath)
{
	// copy path
	CopyMemory(g_lpDataPath,	
			   lpPath,
			   _MAX_PATH);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SaveConfigurationDataPath()				//
//		(defined in "filefunc.h")						//
//														//
//	Save current setting of data path.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SaveConfigurationDataPath(void)
{
	// number of bytes written
	DWORD dwBytesWritten=0;
	
	// write path
	WriteFile(g_hCfgFile,
			  g_lpDataPath,
			  _MAX_PATH,
			  &dwBytesWritten,
			  NULL);
	// check for errors
	if(dwBytesWritten!=_MAX_PATH)
	{
		ERR_FILEFUNC02;
		return;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  LoadConfigurationDataPath()				//
//		(defined in "filefunc.h")						//
//														//
//	Load setting of data path.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void LoadConfigurationDataPath(void)
{
	// number of bytes read
	DWORD dwBytesRead=0;
	
	// read path
	ReadFile(g_hCfgFile,
			 g_lpDataPath,
			 _MAX_PATH,
			 &dwBytesRead,
			 NULL);
	// check for errors
	if(dwBytesRead!=_MAX_PATH)
	{
		ERR_FILEFUNC04;
		return;
	}
}

// END ///////////////////////////////////////////////////

