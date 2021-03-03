// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  mvefunc.cpp									//
//														//
//	Movie handling functions.							//
//		(NOTE: Too slow for time-intensive routines!)	//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// allow access to bitmap functions
#include"bmpfunc.h"
// header for this module
#include"mvefunc.h"
// allow access to main window functions
#include"winmain.h"
// allow access to DirectDraw
#include"ddfunc.h"
// allow access to file functions
#include"filefunc.h"

// ERROR MESSAGES ////////////////////////////////////////

// invalid data
#define ERR_MVEFUNC00 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MVEFUNC.CPP\nError Code: 00\nInvalid data in data file.") }
// failed to create surface
#define ERR_MVEFUNC01 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MVEFUNC.CPP\nError Code: 01\nUnable to create bitmap surface.") }

// MESSAGES //////////////////////////////////////////////

// failed to set color key
#define MSG_MVEFUNC00 { DISPLAY_INFO(GetGlobalHWnd(),"Unable to set movie color key.\nGraphics may display incorrectly.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDMovie::CDDMovie()						//
//		(defined in "mvefunc.h")						//
//														//
//	CDDMovie class constructor.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

CDDMovie::CDDMovie()
{
	// reset values
	Bitmap=NULL;
	dwNumBmps=0;
	dwIndex=0;
	lpData=NULL;
	dwDataPtr=NULL;
	lpPalette=NULL;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDMovie::Load()							//
//		(defined in "mvefunc.h")						//
//														//
//	Loads movie from data file.							//
//	  Parameters:										//
//		DWORD: texture stage, if any, +1				//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CDDMovie::Load(DWORD dwTextureStage1)
{
	// data type
	BYTE cDataType=NULL;
	// movie size
	DWORD dwMovieSize=0;
	// palette data
	LPBYTE lpPal=NULL;
	// counter
	DWORD dwCount=0;
	// surface description
	DDSURFACEDESC2 ddsd;
	// temporary surface pointer
	LPDIRECTDRAWSURFACE4 lpDDSTemp=NULL;
	// color key
	DDCOLORKEY ddck;
	
	// check data type
	ReadData(&cDataType,1);
	if(cDataType!=DTF_IDMOVIE)
	{
		ERR_MVEFUNC00;
		return(FALSE);
	}
	// create new bitmap
	SAFE_NEW(Bitmap,CDDBitmap);
	// read movie size
	ReadData((LPBYTE)&dwMovieSize,sizeof(DWORD));
	// read width
	ReadData((LPBYTE)&Bitmap->nWidth,sizeof(DWORD));
	// read height
	ReadData((LPBYTE)&Bitmap->nHeight,sizeof(DWORD));
	// read number of bitmaps
	ReadData((LPBYTE)&dwNumBmps,sizeof(DWORD));
	// allocate memory for palette
	SAFE_ALLOC(lpPal,
			   LPBYTE,
			   768);
	// check for error
	if(!lpPal)
		return(FALSE);
	// read palette
	ReadData(lpPal,768);
	// set palette entries
	for(dwCount=0;dwCount<256;dwCount++)
		SET_PALETTE_ENTRY((BYTE)dwCount,
						  lpPal[(dwCount*3)+2],
						  lpPal[(dwCount*3)+1],
						  lpPal[dwCount*3]);
	// free palette
	SAFE_FREE(lpPal);
	// allocate local palette
	SAFE_ALLOC(lpPalette,
			   LPPALETTEENTRY,
			   sizeof(PALETTEENTRY)*256);
	// copy palette
	CopyMemory(lpPalette,
			   GetGlobalPalettePtr(),
			   sizeof(PALETTEENTRY)*256);
	// subtract width, height, numbmps, and 768 from movie size
	dwMovieSize-=780;
	// allocate movie data memory
	SAFE_ALLOC(lpData,
			   LPBYTE,
			   dwMovieSize);
	// check for error
	if(!lpData)
		return(FALSE);
	// read movie data
	ReadData(lpData,dwMovieSize);
	// create surface description
	INIT_STRUCT(ddsd);
	// set flags
	ddsd.dwFlags=DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
	// set dimensions
	ddsd.dwWidth=Bitmap->nWidth;
	ddsd.dwHeight=Bitmap->nHeight;
	// settings for textures
	if(dwTextureStage1)
	{
		// add texture flag
		ddsd.dwFlags|=DDSD_TEXTURESTAGE;
		// set texture stage
		ddsd.dwTextureStage=dwTextureStage1-1;
		// set as texture
		ddsd.ddsCaps.dwCaps=DDSCAPS_TEXTURE;
		// set as managed texture
		ddsd.ddsCaps.dwCaps2=DDSCAPS2_TEXTUREMANAGE;
	}
	// settings for normal bitmaps
	else	
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
		ddsd.dwWidth=Bitmap->nWidth;
		ddsd.dwHeight=Bitmap->nHeight;
		// settings for textures
		if(dwTextureStage1)
		{
			// add texture flag
			ddsd.dwFlags|=DDSD_TEXTURESTAGE;
			// set texture stage
			ddsd.dwTextureStage=dwTextureStage1-1;
			// set as texture
			ddsd.ddsCaps.dwCaps=DDSCAPS_TEXTURE;
			// set as managed texture
			ddsd.ddsCaps.dwCaps2=DDSCAPS2_TEXTUREMANAGE;
		}
		// settings for normal bitmaps
		else	
			ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|
								DDSCAPS_SYSTEMMEMORY;
		// create bitmap surface
		if(FAILED(GetGlobalDirectDrawPtr()->CreateSurface(&ddsd,
														  &lpDDSTemp,
														  NULL)))
		{
			// we have a problem
			ERR_MVEFUNC01;
			return(FALSE);
		}
	}
	// prepare color key
	ddck.dwColorSpaceLowValue=0;
	ddck.dwColorSpaceHighValue=0;
	// set color key
	if(FAILED(lpDDSTemp->SetColorKey(DDCKEY_SRCBLT,
									 &ddck)))
		MSG_MVEFUNC00;
	// copy surface pointer
	Bitmap->lpBmpSurf=lpDDSTemp;
	// set valid flag
	Bitmap->bIsValid=TRUE;
	// set data pointer
	dwDataPtr=0;
	// set index
	dwIndex=0;
	// shrink if necessary
	if(!GetGlobalResolutionFlag())
		Bitmap->Shrink();
	// set back buffer pointer
	Bitmap->lpBuffer=GetGlobalBackBufferPtr();
	// draw first frame
	Update();
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDMovie::Display()						//
//		(defined in "mvefunc.h")						//
//														//
//	Output contents of movie to display.				//
//	  Parameters:										//
//		int,int: position								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDMovie::Display(int nX,
					   int nY)
{
	Bitmap->Display(nX,nY);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDMovie::Buffer()						//
//		(defined in "mvefunc.h")						//
//														//
//	Output contents of movie to back buffer.			//
//	  Parameters:										//
//		int,int: position								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDMovie::Buffer(int nX,
					  int nY)
{
	Bitmap->Buffer(nX,nY);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDMovie::BufferFast()					//
//		(defined in "mvefunc.h")						//
//														//
//	Output contents of movie to back buffer fast.		//
//	  Parameters:										//
//		int,int: position								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDMovie::BufferFast(int nX,
						  int nY)
{
	Bitmap->BufferFast(nX,nY);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDMovie::Update()						//
//		(defined in "mvefunc.h")						//
//														//
//	Updates contents of movie.							//
//	  Parameters:										//
//		DWORD: number of frames to advance or nothing	//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDMovie::Update(DWORD dwNumFrames)
{
	// x-coordinate
	int nX;
	// y-coordinate
	int nY;
	// bitmap data
	LPWORD lpBmpData;
	// bitmap pitch
	DWORD dwPitch;
	// counter
	DWORD dwCount;
	// run of pixels
	BYTE cRun;
	// data
	BYTE cData;

	// lock bitmap surface
	if(!Bitmap->Lock(&lpBmpData,
					 &dwPitch))
		return;
	// repeat number of frames
	while(dwNumFrames)
	{
		// pointer
		int nP=0;

		// reset coordinates
		nX=0;
		nY=0;
		// check bit depth
		if(GetGlobalUse15BitFlag())
		{
			// loop until bitmap is updated
			while(nY<Bitmap->nHeight)
			{
				// read number to skip
				cRun=lpData[dwDataPtr++];
				// skip pixels
				if(cRun)
				{
					// increment values
					nX+=cRun;
					nP+=cRun;
					// check for overflow
					while(nX>=Bitmap->nWidth)
					{
						// reset x
						nX-=Bitmap->nWidth;
						// increment y
						nY++;
						// set pointer
						nP=nY*dwPitch;
					}
				}
				// are we done?
				if(nY>=Bitmap->nHeight)
					break;
				// read number to replace
				cRun=lpData[dwDataPtr++];
				// replace pixels
				if(cRun)
					for(dwCount=0;dwCount<cRun;dwCount++)
					{
						// read data
						cData=lpData[dwDataPtr++];
						// set pixel
						if((nX&1)+(nY&1))
							lpBmpData[nP]=GetGlobalPaletteValue15(cData);
						// increment values
						nX++;
						nP++;
						// check for overflow
						if(nX>=Bitmap->nWidth)
						{
							// reset x
							nX=0;
							// increment y
							nY++;
							// set pointer
							nP=nY*dwPitch;
						}
					}
			}
		}
		else
		{
			// loop until bitmap is updated
			while(nY<Bitmap->nHeight)
			{
				// read number to skip
				cRun=lpData[dwDataPtr++];
				// skip pixels
				if(cRun)
				{
					// increment values
					nX+=cRun;
					nP+=cRun;
					// check for overflow
					while(nX>=Bitmap->nWidth)
					{
						// reset x
						nX-=Bitmap->nWidth;
						// increment y
						nY++;
						// set pointer
						nP=nY*dwPitch;
					}
				}
				// are we done?
				if(nY>=Bitmap->nHeight)
					break;
				// read number to replace
				cRun=lpData[dwDataPtr++];
				// replace pixels
				if(cRun)
					for(dwCount=0;dwCount<cRun;dwCount++)
					{
						// read data
						cData=lpData[dwDataPtr++];
						// set pixel
						if((nX&1)+(nY&1))
							lpBmpData[nP]=GetGlobalPaletteValue16(cData);
						// increment values
						nX++;
						nP++;
						// check for overflow
						if(nX>=Bitmap->nWidth)
						{
							// reset x
							nX=0;
							// increment y
							nY++;
							// set pointer
							nP=nY*dwPitch;
						}
					}
			}
		}
		// increment frame index
		dwIndex++;
		// check for overflow
		if(dwIndex>=dwNumBmps)
		{
			// reset values
			dwIndex=0;
			dwDataPtr=0;
		}
		// decrement frame counter
		dwNumFrames--;
	}
	// unlock bitmap
	Bitmap->Unlock();
	// ok
	return;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDMovie::Release()						//
//		(defined in "mvefunc.h")						//
//														//
//	Frees all storage associated with movie.			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDMovie::Release(void)
{
	// free palette
	SAFE_FREE(lpPalette);
	// free data
	SAFE_FREE(lpData);
	// delete bitmap
	DELETE_BITMAP(Bitmap);
	// reset values
	Bitmap=NULL;
	DWORD dwNumBmps=0;
	DWORD dwIndex=0;
	LPBYTE lpData=NULL;
	DWORD dwDataPtr=NULL;
}

// ENCODE ////////////////////////////////////////////////

// include encode functions if necessary
#ifdef ENCODE_DATA

// ERROR MESSAGES ////////////////////////////////////////

// unable to open input file
#define ERR_MVEFUNC00e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MVEFUNC.CPP\nError Code: 00e\nUnable to open movie file.") } 
// unable to read input file
#define ERR_MVEFUNC01e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MVEFUNC.CPP\nError Code: 01e\nUnable to read from movie file.") } 

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EncodeMovie()							//
//														//
//	Outputs movie to open data file.  (*.MVE as created	//
//		by my "MovieMaker" program, which requires a	//
//		series of bitmaps with a common palette as		//
//		created by my "Palettizer" program.)			//
//	  Parameters:										//
//		LPCSTR: filename								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EncodeMovie(LPSTR lpFilename)
{
	// handle to input file
	HANDLE hInputFile=INVALID_HANDLE_VALUE;
	// size of file
	DWORD dwFileSize=0;
	// movie data
	LPBYTE lpData=NULL;
	// bytes read by ReadFile()
	DWORD dwBytesRead=0;
	
	// output ID to verification file
	WriteVerificationID(DTF_IDMOVIE);
	// output ID to data file
	WriteDataID(DTF_IDMOVIE);
	// open file	
	hInputFile=CreateFile(lpFilename,
						  GENERIC_READ,
						  NULL,
						  NULL,
						  OPEN_EXISTING,
						  FILE_FLAG_SEQUENTIAL_SCAN,
						  NULL);
	// check for error
	if(hInputFile==INVALID_HANDLE_VALUE)
	{
		ERR_MVEFUNC00e;
		return;
	}
	// get file size
	dwFileSize=GetFileSize(hInputFile,NULL);
	// output file size
	WriteDataDWORD(dwFileSize);
	// allocate memory
	SAFE_ALLOC(lpData,
			   LPBYTE,
			   dwFileSize);
	// check for error
	if(!lpData)
		return;
	// read data
	ReadFile(hInputFile,
			 lpData,
			 dwFileSize,
			 &dwBytesRead,
			 NULL);
	// check for error
	if(dwBytesRead!=dwFileSize)
	{
		ERR_MVEFUNC01e;
		// close file
		CloseHandle(hInputFile);
		// free memory
		SAFE_FREE(lpData);
		return;
	}
	// close file
	CloseHandle(hInputFile);
	// output data
	WriteDataBytes(lpData,dwFileSize);
	// free memory
	SAFE_FREE(lpData);
}

#endif

// END ///////////////////////////////////////////////////

