// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  bmpfunc.cpp									//
//														//
//	Bitmap handling functions.							//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// header for this module
#include"bmpfunc.h"
// allow access to main window functions
#include"winmain.h"
// allow access to DirectDraw
#include"ddfunc.h"
// allow access to file functions
#include"filefunc.h"

// ERROR MESSAGES ////////////////////////////////////////

// invalid bitmap data
#define ERR_BMPFUNC00 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: BMPFUNC.CPP\nError Code: 00\nInvalid data in data file.") }
// unable to create surface
#define ERR_BMPFUNC01 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: BMPFUNC.CPP\nError Code: 01\nUnable to create bitmap surface.") }
// unable to lock surface
#define ERR_BMPFUNC02 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: BMPFUNC.CPP\nError Code: 02\nUnable to lock bitmap surface.") }
// unable to unlock surface
#define ERR_BMPFUNC03 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: BMPFUNC.CPP\nError Code: 03\nUnable to unlock bitmap surface.") }

// MESSAGES //////////////////////////////////////////////

// failed to set color key
#define MSG_BMPFUNC00 { DISPLAY_INFO(GetGlobalHWnd(),"Unable to set bitmap color key.\nGraphics may not display properly."); }
// failed to blit
#define MSG_BMPFUNC01 { DISPLAY_INFO(GetGlobalHWnd(),"Bitmap operation failed."); }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDBitmap::CDDBitmap()					//
//		(defined in "bmpfunc.h")						//
//														//
//	CDDBitmap class constructor.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

CDDBitmap::CDDBitmap()
{
	// initialize variables
	nWidth=0;
	nHeight=0;
	lpBmpSurf=NULL;
	bIsValid=FALSE;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDBitmap::Load()						//
//		(defined in "bmpfunc.h")						//
//														//
//	Loads a bitmap from an open data file.				//
//	  Parameters:										//
//		DWORD: if a texture, the texture stage+1 to be	//
//			   loaded into								//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CDDBitmap::Load(DWORD dwTextureStage1)
{
	// palette data
	LPBYTE lpPal=NULL;
	// counter	
	DWORD dwCount=0;
	// bitmap data
	LPWORD lpData=NULL;
	// bitmap data pitch
	DWORD dwPitch=NULL;
	// x & y coordinates
	int nX=0;
	int nY=0;
	// surface description
	DDSURFACEDESC2 ddsd;
	// color key
	DDCOLORKEY ddck;
	// data read
	BYTE cData=0;
	// run of pixels
	BYTE cRun=0;
	// data type
	BYTE cType=NULL;

	// read data type
	ReadData(&cType,sizeof(BYTE));
	// check data type
	if(cType!=DTF_IDPALETTE)
	{
		ERR_BMPFUNC00;
		return(FALSE);
	}
	// allocate memory for palette
	SAFE_ALLOC(lpPal,
			   LPBYTE,
			   768);
	// check for error
	if(!lpPal)
		return(FALSE);
	// read palette
	ReadData(lpPal,
			 768);
	// set entries into palette
	for(dwCount=0;dwCount<256;dwCount++)
		SET_PALETTE_ENTRY((BYTE)dwCount,
						  lpPal[dwCount*3],
						  lpPal[(dwCount*3)+1],
						  lpPal[(dwCount*3)+2]);
	// free palette memory
	SAFE_FREE(lpPal);
	// read data type
	ReadData(&cType,sizeof(BYTE));
	// check data type
	if(cType!=DTF_IDBITMAP)
	{
		ERR_BMPFUNC00;
		return(FALSE);
	}
	// set dimensions
	ReadData((LPBYTE)&nWidth,sizeof(int));
	ReadData((LPBYTE)&nHeight,sizeof(int));
	// create surface description
	INIT_STRUCT(ddsd);
	// set flags
	ddsd.dwFlags=DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
	// set dimensions based on resolution
	ddsd.dwWidth=nWidth;
	ddsd.dwHeight=nHeight;
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
		ddsd.ddsCaps.dwCaps2=DDSCAPS2_TEXTUREMANAGE|DDSCAPS2_HINTSTATIC;
	}
	// settings for normal bitmaps
	else	
		ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|
							DDSCAPS_VIDEOMEMORY;
	// create bitmap surface
	if(FAILED(GetGlobalDirectDrawPtr()->CreateSurface(&ddsd,
													  &lpBmpSurf,
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
			ddsd.ddsCaps.dwCaps2=DDSCAPS2_TEXTUREMANAGE|DDSCAPS2_HINTSTATIC;
		}
		// settings for normal bitmaps
		else	
			ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|
								DDSCAPS_SYSTEMMEMORY;
		// create bitmap surface
		if(FAILED(GetGlobalDirectDrawPtr()->CreateSurface(&ddsd,
														  &lpBmpSurf,
														  NULL)))
		{
			// we have a problem
			ERR_BMPFUNC01;
			return(FALSE);
		}
	}
	// set valid flag
	bIsValid=TRUE;
	// prepare color key
	ddck.dwColorSpaceLowValue=0;
	ddck.dwColorSpaceHighValue=0;
	// set color key
	if(FAILED(lpBmpSurf->SetColorKey(DDCKEY_SRCBLT,
									 &ddck)))
		MSG_BMPFUNC00;
	// write bitmap data
	if(!Lock(&lpData,
			 &dwPitch))
		return(FALSE);
	// reset coordinates
	nX=0;
	nY=0;
	// pointer
	DWORD dwP=0;
	// loop through y-coordinates
	while(nY<nHeight)
	{
		// read number of blank pixels
		ReadData(&cRun,sizeof(BYTE));
		// erase pixels if necessary
		if(cRun)
			for(dwCount=0;dwCount<cRun;dwCount++)
			{
				// set pixel
				lpData[dwP]=0;
				// increment x-coordinate
				dwP++;
				nX++;
				// check for overflow
				if(nX>=nWidth)
				{
					// increment y-coordinate
					nY++;
					// reset x-coordinte
					dwP=nY*dwPitch;
					nX=0;
				}
			}
		// are we finished?
		if(nY>=nHeight)
			break;
		// read number of pixels
		ReadData(&cRun,sizeof(BYTE));
		// write pixels if necessary
		if(GetGlobalUse15BitFlag())
		{
			if(cRun)
				for(dwCount=0;dwCount<cRun;dwCount++)
				{
					// read color
					ReadData(&cData,sizeof(BYTE));
					// set pixel as req'd
					lpData[dwP]=GetGlobalPaletteValue15(cData);
					// increment x-coordinate
					dwP++;
					nX++;
					// check for overflow
					if(nX>=nWidth)
					{
						// increment y-coordinate
						nY++;
						// reset x-coordinte
						dwP=nY*dwPitch;
						nX=0;
					}
				}
		}
		else
		{
			if(cRun)
				for(dwCount=0;dwCount<cRun;dwCount++)
				{
					// read color
					ReadData(&cData,sizeof(BYTE));
					// set pixel as req'd
					lpData[dwP]=GetGlobalPaletteValue16(cData);
					// increment x-coordinate
					dwP++;
					nX++;
					// check for overflow
					if(nX>=nWidth)
					{
						// increment y-coordinate
						nY++;
						// reset x-coordinte
						dwP=nY*dwPitch;
						nX=0;
					}
				}
		}
	}
	// unlock bitmap
	Unlock();
	// get back buffer
	lpBuffer=GetGlobalBackBufferPtr();
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDBitmap::IsValid()						//
//		(defined in "bmpfunc.h")						//
//														//
//	Checks for bitmap validity.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if bitmap is valid					//
//														//
//////////////////////////////////////////////////////////

BOOL CDDBitmap::IsValid(void)
{
	return(bIsValid);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDBitmap::Release()						//
//		(defined in "bmpfunc.h")						//
//														//
//	Releases a bitmap.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDBitmap::Release(void)
{
	// release bitmap surface
	SAFE_RELEASE(lpBmpSurf);
	// clear variables
	nWidth=0;
	nHeight=0;
	lpBmpSurf=NULL;
	bIsValid=FALSE;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDBitmap::Lock()						//
//		(defined in "bmpfunc.h")						//
//														//
//	Locks bitmap surface.								//
//	  Parameters:										//
//		LPWORD*: pointer to address of data				//
//		LPDWORD: pointer to pitch						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

BOOL CDDBitmap::Lock(LPWORD* lplpwData,
				     LPDWORD lpdwPitch)
{
	// surface description
	DDSURFACEDESC2 ddsd;
	INIT_STRUCT(ddsd);

	// lock surface
#ifdef DEBUG_MODE
	if(FAILED(lpBmpSurf->Lock(NULL,
							  &ddsd,
							  DDLOCK_SURFACEMEMORYPTR|
							  DDLOCK_WAIT,
							  NULL)))
	{	
		ERR_BMPFUNC02;
		return(FALSE);
	}
#else
	lpBmpSurf->Lock(NULL,
	  			    &ddsd,
					DDLOCK_SURFACEMEMORYPTR|
					DDLOCK_WAIT,
					NULL);
#endif
	// set data pointer
	*lplpwData=(LPWORD)ddsd.lpSurface;
	// set pitch
	*lpdwPitch=ddsd.lPitch>>1;
	// ok
	return(TRUE);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDBitmap::Unlock()						//
//		(defined in "bmpfunc.h")						//
//														//
//	Unlock bitmap surface.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDBitmap::Unlock(void)
{
	// unlock surface
#ifdef DEBUG_MODE
	if(FAILED(lpBmpSurf->Unlock(NULL)))
		ERR_BMPFUNC03;
#else
	lpBmpSurf->Unlock(NULL);
#endif
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDBitmap::Display()						//
//		(defined in "bmpfunc.h")						//
//														//
//	Blit bitmap to display.								//
//	  Parameters:										//
//		int,int: upper-left corner						//
//		int,int: lower-right corner or nothing to show	//
//				 normal-sized bitmap					//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDBitmap::Display(int nX1,
						int nY1,
						int nX2,
						int nY2)
{
	// destination rectangle
	RECT rDest;

	// set upper-left
	rDest.left=nX1;
	rDest.top=nY1;
	// set lower right
	if(nX2==0xF0000000L)
	{
		rDest.right=nX1+nWidth;
		rDest.bottom=nY1+nHeight;
	}
	else
	{
		rDest.right=nX2;
		rDest.bottom=nY2;
	}
	// perform blit
#ifdef DEBUG_MODE
	if(FAILED(GetGlobalFrontBufferPtr()->Blt(&rDest,
											 lpBmpSurf,
											 NULL,
											 DDBLT_KEYSRC|
											 DDBLT_WAIT,
											 NULL)))
		MSG_BMPFUNC01;
#else
	GetGlobalFrontBufferPtr()->Blt(&rDest,
								   lpBmpSurf,
								   NULL,
								   DDBLT_KEYSRC|
								   DDBLT_WAIT,
								   NULL);
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDBitmap::Buffer()						//
//		(defined in "bmpfunc.h")						//
//														//
//	Blit bitmap to buffer.								//
//	  Parameters:										//
//		int,int: upper-left corner						//
//		int,int: lower-right corner or nothing to show	//
//				 normal-sized bitmap					//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDBitmap::Buffer(int nX1,
					   int nY1,
					   int nX2,
					   int nY2)
{
	// destination rectangle
	RECT rDest;

	// set upper-left
	rDest.left=nX1;
	rDest.top=nY1;
	// set lower right
	if(nX2==0xF0000000L)
	{
		rDest.right=nX1+nWidth;
		rDest.bottom=nY1+nHeight;
	}
	else
	{
		rDest.right=nX2;
		rDest.bottom=nY2;
	}
	// perform blit
#ifdef DEBUG_MODE
	if(FAILED(lpBuffer->Blt(&rDest,
							lpBmpSurf,
							NULL,
							DDBLT_KEYSRC|
							DDBLT_WAIT,
							NULL)))
		MSG_BMPFUNC01;
#else
	lpBuffer->Blt(&rDest,
				  lpBmpSurf,
				  NULL,
				  DDBLT_KEYSRC|
				  DDBLT_WAIT,
				  NULL);
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDBitmap::BufferFast()					//
//		(defined in "bmpfunc.h")						//
//														//
//	Blit bitmap fast to buffer.							//
//	  Parameters:										//
//		int,int: upper-left corner						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDBitmap::BufferFast(int nX1,
						   int nY1)
{
	// perform blit
#ifdef DEBUG_MODE
	if(FAILED(lpBuffer->BltFast(nX1,
								nY1,
								lpBmpSurf,
								NULL,
								DDBLT_KEYSRC|
								DDBLT_WAIT)))
		MSG_BMPFUNC01;
#else
	lpBuffer->BltFast(nX1,
					  nY1,
					  lpBmpSurf,
					  NULL,
					  DDBLT_KEYSRC|
					  DDBLT_WAIT);
#endif
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDDBitmap::Shrink()						//
//		(defined in "bmpfunc.h")						//
//														//
//	Shrink bitmap in half.								//
//	  Parameters:										//
//		DWORD: texture stage							//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDDBitmap::Shrink(DWORD dwTextureStage1)
{
	// old bitmap surface
	LPDIRECTDRAWSURFACE4 lpOldBmp=NULL;
	// rectangle
	RECT rTemp;
	// surface description
	DDSURFACEDESC2 ddsd;
	// color key
	DDCOLORKEY ddck;
	
	// reset valid flag
	bIsValid=FALSE;
	// swap old with new
	SwapPointers((LPVOID*)&lpBmpSurf,
				 (LPVOID*)&lpOldBmp);
	// shrink dimsensions
	nWidth=nWidth>>1;
	nHeight=nHeight>>1;
	// create surface description
	INIT_STRUCT(ddsd);
	// set flags
	ddsd.dwFlags=DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
	// set dimensions based on resolution
	ddsd.dwWidth=nWidth;
	ddsd.dwHeight=nHeight;
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
													  &lpBmpSurf,
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
														  &lpBmpSurf,
														  NULL)))
		{
			// we have a problem
			ERR_BMPFUNC01;
			return;
		}
	}
	// set valid flag
	bIsValid=TRUE;
	// prepare color key
	ddck.dwColorSpaceLowValue=0;
	ddck.dwColorSpaceHighValue=0;
	// set color key
	if(FAILED(lpBmpSurf->SetColorKey(DDCKEY_SRCBLT,
									 &ddck)))
		MSG_BMPFUNC00;
	// set up destination rectangle
	rTemp.left=0;
	rTemp.top=0;
	rTemp.right=nWidth;
	rTemp.bottom=nHeight;
	// copy bitmap
	if(FAILED(lpBmpSurf->Blt(&rTemp,
							 lpOldBmp,
							 NULL,
							 DDBLT_WAIT,
							 NULL)))
		MSG_BMPFUNC01;
	// out with the old
	SAFE_RELEASE(lpOldBmp);
}

// include encode functions if necessary
#ifdef ENCODE_DATA

#include<io.h>

// STRUCTURES ////////////////////////////////////////////

// structure to hold bitmap file data
typedef struct TAG_BITMAP_FILE
{
	// file header
	BITMAPFILEHEADER bitmapfileheader;
	// secondary file header
	BITMAPFILEHEADER bfh;
	// bitmap header
	BITMAPINFOHEADER bitmapinfoheader;
	// secondary bitmap header
	BITMAPINFOHEADER bih;
	// palette
	PALETTEENTRY palette[256];
	// secondary palette pointer
	LPPALETTEENTRY lpPal;
	// data pointer
	LPBYTE buffer;
	// secondary data pointer
	LPBYTE lpData;
} BITMAP_FILE,*BITMAP_FILE_PTR;

// PROTOTYPES ////////////////////////////////////////////

int UnloadBitmapFile(BITMAP_FILE_PTR bitmap);

int FlipBitmap(UCHAR *image, int bytes_per_line, int height);

int LoadBitmapFile(BITMAP_FILE_PTR bitmap, char *filename);

// ERROR MESSAGES ////////////////////////////////////////

// failed to load bitmap
#define ERR_BMPFUNC00e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: BMPFUNC.CPP\nError Code: 00e\nUnable to load bitmap image.") }

// DEFINES ///////////////////////////////////////////////

// the letters "BM" which always start a bitmap
#define BITMAP_ID 0x4D42

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EncodeBitmap()							//
//		(defined in "bmpfunc.h")						//
//														//
//	Outputs bitmap file to previously opened data file.	//
//	  Parameters:										//
//		LPCSTR: filename of bitmap						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EncodeBitmap(LPCSTR lpFilename)
{
	// bitmap file storage
	BITMAP_FILE bmpfile;
	INIT_VAR(bmpfile);
	// counter
	DWORD dwCount=0;
	// compression read position
	DWORD dwReadPos=0;
	// compression saved read position
	DWORD dwReadPosSave=0;
	// compression stop position
	DWORD dwStopPos=0;
	// compression finished flag
	DWORD dwCompressionDone=0;
	
	// load image
	if(!LoadBitmapFile(&bmpfile,
					   (char*)lpFilename))
	{
		ERR_BMPFUNC00e;
		return;
	}
	// output palette ID to verification file
	WriteVerificationID(DTF_IDPALETTE);
	// output palette ID ot data file
	WriteDataID(DTF_IDPALETTE);
	// output palette data
	for(dwCount=0;dwCount<256;dwCount++)
	{
		// output color values
		WriteDataByte(bmpfile.lpPal[dwCount].peRed);
		WriteDataByte(bmpfile.lpPal[dwCount].peGreen);
		WriteDataByte(bmpfile.lpPal[dwCount].peBlue);
	}
	// output bitmap ID to verification file
	WriteVerificationID(DTF_IDBITMAP);
	// output bitmap ID ot data file
	WriteDataID(DTF_IDBITMAP);
	// output width
	WriteDataDWORD(bmpfile.bih.biWidth);
	// output height
	WriteDataDWORD(bmpfile.bih.biHeight);
	// set compression stop position
	dwStopPos=bmpfile.bih.biWidth*bmpfile.bih.biHeight;
	// loop until all pixels are compressed
	while(dwReadPos<dwStopPos)
	{
		// reset counter
		dwCount=0;
		// count blank spaces
		while(bmpfile.lpData[dwReadPos]==0)
		{
			// check for overflow
			if(dwReadPos>=dwStopPos)
			{
				// done under flag "1"
				dwCompressionDone=1;
				break;
			}
			// continue counting
			else
			{
				// increment read position & counter
				dwReadPos++;
				dwCount++;
			}
		}
		// see if we are done
		if(dwCompressionDone)
			break;
		// encode number of blank spaces
		if(dwCount>255)
			// encode "255,0" until number is <255
			while(dwCount>255)
			{
				// output numbers
				WriteDataByte(255);
				WriteDataByte(0);
				// decrement counter
				dwCount-=255;
			}
		// encode remainder
		WriteDataByte((BYTE)dwCount);
		// check to see if done
		if(dwReadPos>=dwStopPos)
			break;
		// save read position
		dwReadPosSave=dwReadPos;
		// reset counter
		dwCount=0;
		// count colored pixels
		while(bmpfile.lpData[dwReadPos]!=0)
		{
			// check for overflow
			if(dwReadPos>=dwStopPos)
			{
				// done under flag "2"
				dwCompressionDone=2;
				break;
			}
			else
			{
				// increment read position & counter
				dwReadPos++;
				dwCount++;
			}
		}
		// check to see if done
		if(dwCompressionDone)
			break;
		// encode colored pixel data
		if(dwCount>255)
			// encode data until counter <255
			while(dwCount>255)
			{
				// output 255
				WriteDataByte(255);
				// output pixels
				WriteDataBytes(&bmpfile.lpData[dwReadPosSave],
							   255);
				// output 0
				WriteDataByte(0);
				// increment saved read position
				dwReadPosSave+=255;
				// decrement counter
				dwCount-=255;
			}
		// output remainder of pixels
		WriteDataByte((BYTE)dwCount);
		// write pixels if necessary
		if(dwCount)
				WriteDataBytes(&bmpfile.lpData[dwReadPosSave],
							   dwCount);
	}
	// finish encoding based on value of done flag
	if(dwCompressionDone==1)
	{
		// enconde blank pixels
		if(dwCount>255)
			// encode "255,0" until number is <255
			while(dwCount>255)
			{
				// output numbers
				WriteDataByte(255);
				WriteDataByte(0);
				// decrement counter
				dwCount-=255;
			}
		// encode remainder
		WriteDataByte((BYTE)dwCount);
	}
	else if(dwCompressionDone==2)
	{
		// encode colored pixels
		if(dwCount>255)
			// encode data until counter <255
			while(dwCount>255)
			{
				// output 255
				WriteDataByte(255);
				// output pixels
				WriteDataBytes(&bmpfile.lpData[dwReadPosSave],
							   255);
				// output 0
				WriteDataByte(0);
				// increment saved read position
				dwReadPosSave+=255;
				// decrement counter
				dwCount-=255;
			}
		// output remainder of pixels
		WriteDataByte((BYTE)dwCount);
		// write pixels if necessary
		if(dwCount)
				WriteDataBytes(&bmpfile.lpData[dwReadPosSave],
							   dwCount);
	}
	// unload bitmap file
	UnloadBitmapFile(&bmpfile);
}

// NOTE: These functions use old-style file access because
//		 that's how they were originally written. Since
//		 they will not be included in the release version,
//		 it is nothing to be concerned about.

//////////////////////////////////////////////////////////
//														//
//	Function:  LoadBitmapFile()						//
//														//
//	Loads a bitmap file.								//
//	  Parameters:										//
//		BITMAP_FILE_PTR: structure to hold image		//
//		char*: filename								//
//	  Return Value:										//
//		int: 1 if successful							//
//														//
//////////////////////////////////////////////////////////

int LoadBitmapFile(BITMAP_FILE_PTR bitmap, char *filename)
{
	// file handle
	int file_handle=NULL;
    // index
	int index;
	// file data structure
	OFSTRUCT file_data;

	// open the file
	if((file_handle=OpenFile(filename,&file_data,OF_READ))==-1)
		return(0);
	// load header
	_lread(file_handle,&bitmap->bitmapfileheader,sizeof(BITMAPFILEHEADER));
	// check validity
	if(bitmap->bitmapfileheader.bfType!=BITMAP_ID)
	{
		// close file
		_lclose(file_handle);
		return(0);
	}
	// load info header
	_lread(file_handle,&bitmap->bitmapinfoheader,sizeof(BITMAPINFOHEADER));
	// load palette if necessary
	if(bitmap->bitmapinfoheader.biBitCount==8)
	{
		_lread(file_handle,&bitmap->palette,256*sizeof(PALETTEENTRY));
		// reverse red and blue
		for(index=0;index<256;index++)
        {
			int temp_color=bitmap->palette[index].peRed;
			bitmap->palette[index].peRed=bitmap->palette[index].peBlue;
			bitmap->palette[index].peBlue=temp_color;
			// set no collapse flag
			bitmap->palette[index].peFlags=PC_NOCOLLAPSE;
		}
	}
	// check for size
	if(!bitmap->bitmapinfoheader.biSizeImage)
		bitmap->bitmapinfoheader.biSizeImage=bitmap->bitmapinfoheader.biWidth*bitmap->bitmapinfoheader.biHeight*(bitmap->bitmapinfoheader.biBitCount>>3);
	// load image
	_lseek(file_handle,-(int)(bitmap->bitmapinfoheader.biSizeImage),SEEK_END);
	// load image
	if(bitmap->bitmapinfoheader.biBitCount==8) 
	{
		// allocate memory
		if(!(bitmap->buffer=(UCHAR*)malloc(bitmap->bitmapinfoheader.biSizeImage)))
		{
			// close the file
			_lclose(file_handle);
			return(0);
		}
		// read data
		_lread(file_handle,bitmap->buffer,bitmap->bitmapinfoheader.biSizeImage);
	}
	// we only want 8-bit
	else
	{
		// close the file
		_lclose(file_handle);
		return(0);
	}
	// close file
	_lclose(file_handle);
	// flip the bitmap
	FlipBitmap(bitmap->buffer, 
			   bitmap->bitmapinfoheader.biWidth*(bitmap->bitmapinfoheader.biBitCount/8), 
               bitmap->bitmapinfoheader.biHeight);
	// set secondary pointers
	bitmap->lpPal=bitmap->palette;
	bitmap->lpData=bitmap->buffer;
	bitmap->bfh=bitmap->bitmapfileheader;
	bitmap->bih=bitmap->bitmapinfoheader;
	// ok
	return(1);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  FlipBitmap()								//
//														//
//	Flips a loaded bitmap								//
//	  Parameters:										//
//		UCHAR*: image data								//
//		int: bytes per line								//
//		int: number of lines							//
//	  Return Value:										//
//		int: 1 if successful							//
//														//
//////////////////////////////////////////////////////////

int FlipBitmap(UCHAR *image, int bytes_per_line, int height)
{
	// temporary buffer
	UCHAR *buffer;
	// index
	int index;

	// allocate temporary buffer
	if(!(buffer=(UCHAR*)malloc(bytes_per_line*height)))
		return(0);
	// copy image temporary buffer
	memcpy(buffer,image,bytes_per_line*height);
	// flip vertically
	for(index=0;index<height;index++)
		memcpy(&image[((height-1)-index)*bytes_per_line],
			   &buffer[index*bytes_per_line],bytes_per_line);
	// release memory
	free(buffer);
	// return success
	return(1);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  UnloadBitmapFile()						//
//														//
//	Unloads a loaded bitmap								//
//	  Parameters:										//
//		BITMAP_FILE_PTR: structure holding image		//
//	  Return Value:										//
//		int: 1 if successful							//
//														//
//////////////////////////////////////////////////////////

int UnloadBitmapFile(BITMAP_FILE_PTR bitmap)
{
	if(bitmap->buffer)
	{
		// release memory
		free(bitmap->buffer);
		// reset pointer
		bitmap->buffer = NULL;
   }
	// return success
	return(1);
}

#endif

// END ///////////////////////////////////////////////////

