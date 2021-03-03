// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  txfunc.cpp									//
//														//
//	Texture functions.									//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// allow access to bitmaps
#include"bmpfunc.h"
// allow access to Direct3D
#include"d3dfunc.h"
// header for this module
#include"txfunc.h"
// allow access to DirectDraw
#include"ddfunc.h"
// allow access to main window
#include"winmain.h"

// ERROR MESSAGES ////////////////////////////////////////

// failed to create texture
#define ERR_TXFUNC00 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: TXFUNC.CPP\nError Code: 00\nUnable to create texture map.") }
// failed to set texture
#define ERR_TXFUNC01 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: TXFUNC.CPP\nError Code: 01\nUnable to set texture into stage.") }
// failed to create alpha bitmap
#define ERR_TXFUNC02 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: TXFUNC.CPP\nError Code: 02\nUnable to create alpha map.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DTexture::CD3DTexture()				//
//		(defined in "txfunc.h")							//
//														//
//	Class constructor for textures.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

CD3DTexture::CD3DTexture()
{
	// reset values
	lpTex=NULL;
	lpBmp=NULL;
	lpdwLastModel=NULL;
}
	
//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DTexture::Load()						//
//		(defined in "txfunc.h")							//
//														//
//	Load texture from data file.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CD3DTexture::Load(void)
{
	// counter	
	DWORD dwCount=0;
	// bitmap data
	LPWORD lpData=NULL;
	// bitmap data pitch
	DWORD dwPitch=NULL;
	// bitmap data 2
	LPWORD lpData2=NULL;
	// bitmap data pitch 2
	DWORD dwPitch2=NULL;
	// x & y coordinates
	int nX=0;
	int nY=0;

	// new bitmap
	SAFE_NEW(lpBmp,
			 CDDBitmap);
	// load from data file
	if(!lpBmp->Load(1))
		return(FALSE);
	// create texture
	if(FAILED(lpBmp->lpBmpSurf->QueryInterface(IID_IDirect3DTexture2,
											   (LPVOID*)&lpTex)))
	{
		ERR_TXFUNC00;
		return(FALSE);
	}
	// set D3D Device
	lpD3DDevice=GetGlobalDirect3DDevicePtr();
	// set last model pointer
	lpdwLastModel=GetGlobalLastModelPtr();
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
//	Function:  CD3DTexture::Current()					//
//		(defined in "txfunc.h")							//
//														//
//	Set texture as current into its stage.				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DTexture::Current(void)
{
	// set texture into stage
#ifdef DEBUG_MODE
	if(FAILED(lpD3DDevice->SetTexture(0,
									  lpTex)))
	{
		ERR_TXFUNC01;
		return;
	}
#else
	lpD3DDevice->SetTexture(0,
							lpTex);
#endif
	// reset global model number
	*lpdwLastModel=0xFFFFFFFF;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DTexture::Release()					//
//		(defined in "txfunc.h")							//
//														//
//	Description.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DTexture::Release(void)
{
	// release texture
	SAFE_RELEASE(lpTex);
	// delete bitmap
	DELETE_BITMAP(lpBmp);
}

// include encode functions if necessary
#ifdef ENCODE_DATA

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EncodeTexture()							//
//		(defined in "txfunc.h")							//
//														//
//	Save texture to open data file.						//
//	  Parameters:										//
//		LPCSTR: filname									//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void EncodeTexture(LPCSTR lpFilename)
{
	EncodeBitmap(lpFilename);
}

#endif

// END ///////////////////////////////////////////////////

