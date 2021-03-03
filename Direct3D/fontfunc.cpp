// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  fontfunc.cpp									//
//														//
//	Font handling functions.							//
//		(NOTE: Too slow for time-intensive routines!)	//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// allow access to bitmap functions
#include"bmpfunc.h"
// header for this module
#include"fontfunc.h"
// allow access to main window functions
#include"winmain.h"
// allow access to DirectDraw
#include"ddfunc.h"
// allow access to file functions
#include"filefunc.h"

// ERROR MESSAGES ////////////////////////////////////////

// invalid font data
#define ERR_FONTFUNC00 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FONTFUNC.CPP\nError Code: 00\nInvalid data in data file.") }
// failed to create surface
#define ERR_FONTFUNC01 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FONTFUNC.CPP\nError Code: 01\nUnable to create font surface.") }
// failed to create bitmap
#define ERR_FONTFUNC02 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FONTFUNC.CPP\nError Code: 02\nUnable to create font bitmap.") }

// MESSAGES //////////////////////////////////////////////

// failed to set color key
#define MSG_FONTFUNC00 { DISPLAY_INFO(GetGlobalHWnd(),"Unable to set font color key.\nGraphics may display incorrectly.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDFont::CDDFont()						//											//
//		(defined in "fontfunc.h")						//
//														//
//	CDDFont class constructor.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

CDDFont::CDDFont()
{
	// counter
	DWORD dwCount=0;
	
	// reset variables
	nWidth=0;
	nHeight=0;
	// init characters
	for(dwCount=0;dwCount<73;dwCount++)
		Chars[dwCount]=NULL;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDFont::Load()							//
//		(defined in "fontfunc.h")						//
//														//
//	Loads a font from an open data file.				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful
//														//
//////////////////////////////////////////////////////////

BOOL CDDFont::Load(void)
{
	// data type
	BYTE cType=NULL;
	// bytes to read
	DWORD dwBytesToRead=0;
	// font data
	LPBYTE lpData=NULL;
	// current read position
	DWORD dwReadPos=0;
	// current bitmask
	WORD wBitMask=NULL;
	// counter
	DWORD dwCount=0;
	// pointer to bitmap data
	LPWORD lpBmpData=NULL;
	// bitmap pitch
	DWORD dwPitch=0;
	// x-coordinate
	DWORD dwX=0;
	// y-coordinate
	DWORD dwY=0;
	// surface description
	DDSURFACEDESC2 ddsd;
	// color key
	DDCOLORKEY ddck;
	// temporary surface
	LPDIRECTDRAWSURFACE4 lpDDSTemp=NULL;

	// read data type
	ReadData(&cType,sizeof(BYTE));
	// check data type
	if(cType!=DTF_IDFONT)
	{
		ERR_FONTFUNC00;
		return(FALSE);
	}
	// read width
	ReadData((LPBYTE)&nWidth,sizeof(int));
	// read height
	ReadData((LPBYTE)&nHeight,sizeof(int));
	// compute bytes to read
	dwBytesToRead=(DWORD)(((nWidth*nHeight)/8)*73);
	// add one byte if necessary
	if((nWidth*nHeight)&0x07)
		dwBytesToRead++;
	// allocate memory
	SAFE_ALLOC(lpData,
			   LPBYTE,
			   dwBytesToRead);
	// check for errors
	if(!lpData)
		return(FALSE);
	// read font data
	ReadData(lpData,
			 dwBytesToRead);
	// set bitmask
	wBitMask=1;
	// read each character
	for(dwCount=0;dwCount<73;dwCount++)
	{
		// new bitmap
		SAFE_NEW(Chars[dwCount],CDDBitmap);
		// check for errors
		if(!Chars[dwCount])
		{
			ERR_FONTFUNC02;
			return(FALSE);
		}
		// create surface description
		INIT_STRUCT(ddsd);
		// set flags
		ddsd.dwFlags=DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
		// set dimensions
		ddsd.dwWidth=nWidth;
		ddsd.dwHeight=nHeight;
		// create in video memory if possible
		ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|
							DDSCAPS_VIDEOMEMORY;
		// create bitmap surface
		if(FAILED(GetGlobalDirectDrawPtr()->CreateSurface(&ddsd,
														  &lpDDSTemp,
														  NULL)))
		// if failed, try creating in system memory
		{
			// re-create surface description
			INIT_STRUCT(ddsd);
			// set flags
			ddsd.dwFlags=DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
			// set dimensions
			ddsd.dwWidth=nWidth;
			ddsd.dwHeight=nHeight;
			// set system memory flag
			ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|
								DDSCAPS_SYSTEMMEMORY;
			// create bitmap surface
			if(FAILED(GetGlobalDirectDrawPtr()->CreateSurface(&ddsd,
															  &lpDDSTemp,
															  NULL)))
			{
				// we have a problem
				ERR_FONTFUNC01;
				return(FALSE);
			}
		}
		// prepare color key
		ddck.dwColorSpaceLowValue=0;
		ddck.dwColorSpaceHighValue=0;
		// set color key
		if(FAILED(lpDDSTemp->SetColorKey(DDCKEY_SRCBLT,
										 &ddck)))
			MSG_FONTFUNC00;
		// copy surface pointer
		Chars[dwCount]->lpBmpSurf=lpDDSTemp;
		// set valid flag
		Chars[dwCount]->bIsValid=TRUE;
		// set dimensions
		Chars[dwCount]->nWidth=nWidth;
		Chars[dwCount]->nHeight=nHeight;
		// write bitmap data
		if(!Lock(dwCount,
				 &lpBmpData,
				 &dwPitch))
			return(FALSE);
		// output data
		for(dwY=0;dwY<(DWORD)nHeight;dwY++)
			for(dwX=0;dwX<(DWORD)nWidth;dwX++)
			{
				// check bitmask and write set or unset pixel
				if(lpData[dwReadPos]&(BYTE)wBitMask)
					lpBmpData[dwX+(dwY*dwPitch)]=0xFFFF;
				else
					lpBmpData[dwX+(dwY*dwPitch)]=0;
				// increment bitmask
				wBitMask*=2;
				// check for bitmask overflow
				if(wBitMask>255)
				{
					// reset bitmask
					wBitMask=1;
					// increment read position
					dwReadPos++;
				}
			}
		// unlock bitmap
		Unlock(dwCount);
		// shrink if necessary
		if(!GetGlobalResolutionFlag())
			Chars[dwCount]->Shrink();
		// set back buffer pointer
		Chars[dwCount]->lpBuffer=GetGlobalBackBufferPtr();
	}
	// free font data
	SAFE_FREE(lpData);
	// shrink if necessary
	if(!GetGlobalResolutionFlag())
	{
		nWidth=nWidth>>1;
		nHeight=nHeight>>1;
	}
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDFont::DisplayChar()					//
//		(defined in "fontfunc.h")						//
//														//
//	Outputs a single character to the display.			//
//	  Parameters:										//
//		DWORD: character to display						//
//		int,int: location to display					//
//		BOOL: FALSE to draw opaque						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDFont::DisplayChar(DWORD dwChar,
						  int nX,
						  int nY,
						  BOOL bTransparent)
{
	// this is our character code
	DWORD dwOutput=0xFF;
	
	// check for A-Z characters
	if(dwChar>='A'&&dwChar<='Z')
		dwOutput=(dwChar-'A')+FTD_CAPA;
	// check for a-z characters
	else if(dwChar>='a'&&dwChar<='z')
		dwOutput=(dwChar-'a')+FTD_LOWA;
	// check for 0-9 characters
	else if(dwChar>='0'&&dwChar<='9')
		dwOutput=(dwChar-'0')+FTD_DIGT;
	// check for !
	else if(dwChar=='!')
		dwOutput=FTD_EXCL;
	// check for (
	else if(dwChar=='(')
		dwOutput=FTD_LPAR;
	// check for ) 
	else if(dwChar==')')
		dwOutput=FTD_RPAR;
	// check for -
	else if(dwChar=='-')
		dwOutput=FTD_DASH;
	// check for :
	else if(dwChar==':')
		dwOutput=FTD_COLN;
	// check for ;
	else if(dwChar==';')
		dwOutput=FTD_SEMI;
	// check for " (use ^)
	else if(dwChar=='^')
		dwOutput=FTD_QUOT;
	// check for ' (use ~)
	else if(dwChar=='~')
		dwOutput=FTD_APOS;
	// check for ,
	else if(dwChar==',')
		dwOutput=FTD_COMA;
	// check for .
	else if(dwChar=='.')
		dwOutput=FTD_PERD;
	// check for ?
	else if(dwChar=='?')
		dwOutput=FTD_QUES;
	// unsupported character	
	else if(dwOutput>=73)
		return;
	// clear rectangle if not transparent
	if(!bTransparent)
		ClearFrontBuffer(nX,nY,nX+nWidth,nY+nHeight);
	// copy character
	Chars[dwOutput]->Display(nX,nY);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDFont::DisplayText()					//
//		(defined in "fontfunc.h")						//
//														//
//	Output a line of text to the display.				//
//	  Parameters:										//
//		LPSTR: text										//
//		int,int: location								//
//		BOOL: FALSE to draw opaque						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDFont::DisplayText(LPSTR lpText,
						  int nX,
						  int nY,
						  BOOL bTransparent)
{
	// counter
	DWORD dwCount=0;
	// pointer
	int nPtr=nX;

	// do not display empty strings
	if(!strlen(lpText))
		return;
	// display each character
	for(dwCount=0;dwCount<=strlen(lpText);dwCount++)
	{
		DisplayChar((DWORD)lpText[dwCount],nPtr,nY,bTransparent);
		nPtr+=nWidth;
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDFont::BufferChar()					//
//		(defined in "fontfunc.h")						//
//														//
//	Output single character to back buffer.				//
//	  Parameters:										//
//		DWORD: character to display						//
//		int,int: location to display					//
//		BOOL: FALSE to draw opaque						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDFont::BufferChar(DWORD dwChar,
						 int nX,
						 int nY,
						 BOOL bTransparent)
{
	// this is our character code
	DWORD dwOutput=0xFF;
	
	// check for A-Z characters
	if(dwChar>='A'&&dwChar<='Z')
		dwOutput=(dwChar-'A')+FTD_CAPA;
	// check for a-z characters
	else if(dwChar>='a'&&dwChar<='z')
		dwOutput=(dwChar-'a')+FTD_LOWA;
	// check for 0-9 characters
	else if(dwChar>='0'&&dwChar<='9')
		dwOutput=(dwChar-'0')+FTD_DIGT;
	// check for !
	else if(dwChar=='!')
		dwOutput=FTD_EXCL;
	// check for (
	else if(dwChar=='(')
		dwOutput=FTD_LPAR;
	// check for ) 
	else if(dwChar==')')
		dwOutput=FTD_RPAR;
	// check for -
	else if(dwChar=='-')
		dwOutput=FTD_DASH;
	// check for :
	else if(dwChar==':')
		dwOutput=FTD_COLN;
	// check for ;
	else if(dwChar==';')
		dwOutput=FTD_SEMI;
	// check for " (use ^)
	else if(dwChar=='^')
		dwOutput=FTD_QUOT;
	// check for ' (use ~)
	else if(dwChar=='~')
		dwOutput=FTD_APOS;
	// check for ,
	else if(dwChar==',')
		dwOutput=FTD_COMA;
	// check for .
	else if(dwChar=='.')
		dwOutput=FTD_PERD;
	// check for ?
	else if(dwChar=='?')
		dwOutput=FTD_QUES;
	// unsupported character	
	else if(dwOutput>=73)
		return;
	// clear rectangle if not transparent
	if(!bTransparent)
		ClearBackBuffer(nX,nY,nX+nWidth,nY+nHeight);
	// copy character
	Chars[dwOutput]->Buffer(nX,nY);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDFont::BufferText()					//
//		(defined in "fontfunc.h")						//
//														//
//	Output a line of text to back buffer.				//
//	  Parameters:										//
//		LPSTR: text										//
//		int,int: location								//
//		BOOL: FALSE to draw opaque						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDFont::BufferText(LPSTR lpText,
						 int nX,
						 int nY,
						 BOOL bTransparent)
{
	// counter
	DWORD dwCount=0;
	// pointer
	int nPtr=nX;

	// do not display empty strings
	if(!strlen(lpText))
		return;
	// display each character
	for(dwCount=0;dwCount<=strlen(lpText);dwCount++)
	{
		BufferChar((DWORD)lpText[dwCount],nPtr,nY,bTransparent);
		nPtr+=nWidth;
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDFont::Lock()							//
//		(defined in "fontfunc.h")						//
//														//
//	Locks a font character surface.						//
//	  Parameters:										//
//		DWORD: character surface to lock				//
//		LPWORD*: pointer to address of data				//
//		LPDWORD: pointer to pitch						//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CDDFont::Lock(DWORD dwChar,
				   LPWORD* lplpwData,
				   LPDWORD lpdwPitch)
{
	// lock bitmap surface
	return(Chars[dwChar]->Lock(lplpwData,lpdwPitch));
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDFont::Unlock()						//
//		(defined in "fontfunc.h")						//
//														//
//	Unlocks a font character surface.					//
//	  Parameters:										//
//		DWORD: character surface to unlock				//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDFont::Unlock(DWORD dwChar)
{
	// unlock bitmap surface
	Chars[dwChar]->Unlock();
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDFont::Color()							//
//		(defined in "fontfunc.h")						//
//														//
//	Set font color.										//
//	  Parameters:										//
//		BYTE,BYTE,BYTE: red, green and blue values		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDFont::Color(BYTE cRed,
					BYTE cGreen,
					BYTE cBlue)
{
	// x-coordinate
	DWORD dwX;
	// y-coordinate
	DWORD dwY;
	// bitmap data
	LPWORD lpData=NULL;
	// bitmap pitch
	DWORD dwPitch=0;
	// counter
	DWORD dwCount;
	// new color
	WORD wNewColor;

	// set new color
	if(GetGlobalUse15BitFlag())
		wNewColor=RGB_15(cRed,cGreen,cBlue);
	else
		wNewColor=RGB_16(cRed,cGreen,cBlue);
	// loop through each character
	for(dwCount=0;dwCount<73;dwCount++)
	{
		// lock surface
		if(!Lock(dwCount,
			    &lpData,
			    &dwPitch))
			return;
		// pointer
		DWORD dwPtr;

		// change each line
		for(dwY=0;dwY<(DWORD)nHeight;dwY++)
		{
			// set pointer
			dwPtr=dwY*dwPitch;
			// change each pixel
			for(dwX=0;dwX<(DWORD)nWidth;dwX++)
			{
				if(lpData[dwPtr])
					lpData[dwPtr]=wNewColor;
				// increment pointer
				dwPtr++;
			}
		}
		// unlock surface
		Unlock(dwCount);
	}
	// ok
	return;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDFont::Release()						//
//		(defined in "fontfunc.h")						//
//														//
//	Releases font data.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDFont::Release(void)
{
	// counter
	DWORD dwCount=0;
	
	// reset variables
	nWidth=0;
	nHeight=0;
	// delete characters
	for(dwCount=0;dwCount<73;dwCount++)
		DELETE_BITMAP(Chars[dwCount]);
}

// include encode functions if necessary
#ifdef ENCODE_DATA

// ERROR MESSAGES ////////////////////////////////////////

// failed to open font file
#define ERR_FONTFUNC00e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FONTFUNC.CPP\nError Code: 00e\nUnable to open font data file."); }
// failed to read from file
#define ERR_FONTFUNC01e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: FONTFUNC.CPP\nError Code: 01e\nUnable to read from font data file."); }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EncodeFont()								//
//		(defined in "fontfunc.h")						//
//														//
//	Outputs font file to previously opened data and		//
//		verification files.								//
//	  Parameters:										//
//		LPSTR: pointer to filename						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EncodeFont(LPSTR lpFilename)
{
	// handle to input file
	HANDLE hInputFile=INVALID_HANDLE_VALUE;
	// bytes read by ReadFile()
	DWORD dwBytesRead=0;
	// done flag
	BOOL bDone=FALSE;
	// input byte
	char cInput;
	// command storage
	char lpCmd[3];
	// width
	int nWidth=0;
	// height
	int nHeight=0;
	// flag to indicate data is ready to be read
	BOOL bDataReady=FALSE;
	// output
	WORD wOutput=NULL;
	// output bit counter
	WORD wBitNum=0;

	// output ID to verification file
	WriteVerificationID(DTF_IDFONT);
	// output ID ot data file
	WriteDataID(DTF_IDFONT);
	// open input file
	hInputFile=CreateFile(lpFilename,
						  GENERIC_READ,
						  NULL,
						  NULL,
						  OPEN_EXISTING,
						  FILE_ATTRIBUTE_NORMAL|
						  FILE_FLAG_SEQUENTIAL_SCAN,
						  NULL);
	// check for error
	if(hInputFile==INVALID_HANDLE_VALUE)
	{
		ERR_FONTFUNC00e;
		return;
	}
	// macro to simplify file reading
	#define READ_FILE(a) { ReadFile(hInputFile,&a,1,&dwBytesRead,NULL); if(dwBytesRead!=1) { ERR_FONTFUNC01e; return; } }
	// read first byte
	READ_FILE(cInput);
	// loop until done
	while(!bDone)
	{
		// check for command
		if(cInput=='/')
		{
			// read next byte
			READ_FILE(cInput);
			if(cInput=='/')
			{
				// read and store command
				READ_FILE(cInput);
				lpCmd[0]=cInput;
				READ_FILE(cInput);
				lpCmd[1]=cInput;
				lpCmd[2]=0;
				// check for "BC" (begin comment)
				if(strcmp(lpCmd,"BC")==0)
				{
					// we need to loop until we reach an "EC" (end comment)
					ReReadComment:;
						// read to next command
						while(cInput!='/')
							READ_FILE(cInput);
						// check next command
						READ_FILE(cInput);
						if(cInput=='/')
						{
							// read and store command
							READ_FILE(cInput);
							lpCmd[0]=cInput;
							READ_FILE(cInput);
							lpCmd[1]=cInput;
							lpCmd[2]=0;
							// check for "EC" (end comment)
							if(strcmp(lpCmd,"EC")!=0)
								// not done yet
								goto ReReadComment;
						}
						// false alarm; continue reading
						else
							goto ReReadComment;
				}
				// check for "SZ" (set size)
				if(strcmp(lpCmd,"SZ")==0)
				{
					// read width
					while(cInput<'0'||cInput>'9')
						READ_FILE(cInput);
					// save first digit
					nWidth=(int)((cInput-'0')*10);
					// read next digit
					READ_FILE(cInput);
					// save next digit
					nWidth+=(int)(cInput-'0');
					// output width
					WriteDataDWORD((DWORD)nWidth);
					// read next byte
					READ_FILE(cInput);
					// read height
					while(cInput<'0'||cInput>'9')
						READ_FILE(cInput);
					// save first digit
					nHeight=(int)((cInput-'0')*10);
					// read next digit
					READ_FILE(cInput);
					// save next digit
					nHeight+=(int)(cInput-'0');
					// output height
					WriteDataDWORD((DWORD)nHeight);
				}
				// check for "BD" (begin data)
				if(strcmp(lpCmd,"BD")==0)
				{
					// reset output
					wOutput=NULL;
					// reset bit counter
					wBitNum=0;
					// set data ready flag
					bDataReady=TRUE;
				}
				// check for "EF" (end file)
				if(strcmp(lpCmd,"EF")==0)
				{
					// output data if necessary
					if(wBitNum)
						WriteDataByte((BYTE)wOutput);
					// clear data ready flag
					bDataReady=FALSE;
					// set done flag
					bDone=TRUE;
				}
			}
		}
		// this is not a command, so skip it
		else
			READ_FILE(cInput);
		// are we reading data?
		if(bDataReady)
		{
			// check for "0" (pixel)
			if(cInput=='0')
			{
				// save pixel
				wOutput+=(1<<wBitNum);
				// increment bit counter
				wBitNum++;
			}	
			// check for "." (blank space)
			else if(cInput=='.')
				// increment bit counter
				wBitNum++;
			// output data if necessary
			if(wBitNum>7)
			{
				WriteDataByte((BYTE)wOutput);
				// reset output
				wOutput=NULL;
				// reset bit counter
				wBitNum=0;
			}
		}
	}
	// finished!
	SAFE_CLOSE(hInputFile);
	// ok
	return;
}

#endif

// END ///////////////////////////////////////////////////

