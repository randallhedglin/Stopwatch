// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  rdfunc.cpp									//
//														//
//	Raw data functions.									//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// header for this module
#include"rdfunc.h"
// allow access to main window functions
#include"winmain.h"
// allow access to file functions
#include"filefunc.h"

// ERROR MESSAGES ////////////////////////////////////////

// invalid data
#define ERR_RDFUNC00 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: RDFUNC.CPP\nError Code: 00\nInvalid data in data file.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CRawData::CRawData()						//
//		(defined in "rdfunc.h")							//
//														//
//	Class constructor for raw data.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

CRawData::CRawData()
{
	lpData=NULL;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CRawData::Load()							//
//		(defined in "rdfunc.h")							//
//														//
//	Loads raw data from previously open data file.		//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CRawData::Load(void)
{
	// data type
	BYTE cType=NULL;
	// data size
	DWORD dwSize=0;

	// read data type
	ReadData(&cType,sizeof(BYTE));
	// check data type
	if(cType!=DTF_IDRAWDATA)
	{
		ERR_RDFUNC00;
		return(FALSE);
	}
	// read size
	ReadData((LPBYTE)&dwSize,
			 sizeof(DWORD));
	// allocate data buffer
	SAFE_ALLOC(lpData,
			   LPVOID,
			   dwSize);
	// check for error
	if(!lpData)
		return(FALSE);
	// read data
	ReadData((LPBYTE)lpData,
			 dwSize);
	//ok
	return(TRUE);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CRawData::Data()							//
//		(defined in "rdfunc.h")							//
//														//
//	Returns pointer to previously loaded data.			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPVOID: data									//
//														//
//////////////////////////////////////////////////////////

LPVOID CRawData::Data(void)
{
	return(lpData);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CRawData::Release()						//
//		(defined in "rdfunc.h")							//
//														//
//	Frees raw data memory.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CRawData::Release(void)
{
	// free data
	SAFE_FREE(lpData);
}

// include encode functions if necessary

#ifdef ENCODE_DATA

// ERROR MESSAGES ////////////////////////////////////////

// failed to open file
#define ERR_RDFUNC00e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: RDFUNC.CPP\nError Code: 00e\nUnable to open data file.") }
// failed to read file
#define ERR_RDFUNC01e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: RDFUNC.CPP\nError Code: 01e\nUnable to read data file.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EncodeRawData()							//
//		(defined in "rdfunc.h")							//
//														//
//	Saves raw data file to open data file.				//
//	  Parameters:										//
//		LPCSTR: file name								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void EncodeRawData(LPCSTR lpFilename)
{
	// file handle
	HANDLE hFile=INVALID_HANDLE_VALUE;
	// file size
	DWORD dwSize=0;
	// data buffer
	LPVOID lpData=NULL;
	// bytes read by ReadFile
	DWORD dwBytesRead=0;
	
	// output ID to verification file
	WriteVerificationID(DTF_IDRAWDATA);
	// output ID ot data file
	WriteDataID(DTF_IDRAWDATA);
	// open input file	
	hFile=CreateFile(lpFilename,
					 GENERIC_READ,
					 NULL,
					 NULL,
					 OPEN_EXISTING,
					 FILE_FLAG_SEQUENTIAL_SCAN,
					 NULL);
	// check for error
	if(hFile==INVALID_HANDLE_VALUE)
	{
		ERR_RDFUNC00e;
		return;
	}
	// get size
	dwSize=GetFileSize(hFile,NULL);
	// output size
	WriteDataDWORD(dwSize);
	// allocate buffer
	SAFE_ALLOC(lpData,
			   LPVOID,
			   dwSize);
	// check for error
	if(!lpData)
		return;
	// read data
	ReadFile(hFile,
			 lpData,
			 dwSize,
			 &dwBytesRead,
			 NULL);
	// check for error
	if(dwBytesRead!=dwSize)
	{
		ERR_RDFUNC01e;
		// free memory
		SAFE_FREE(lpData);
		// close file
		CloseHandle(hFile);
		return;
	}
	// close file
	CloseHandle(hFile);
	// output data
	WriteDataBytes((LPBYTE)lpData,
				   dwSize);
	// free memory
	SAFE_FREE(lpData);
}

#endif

// END ///////////////////////////////////////////////////
