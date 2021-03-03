// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File: camfunc.cpp									//
//														//
//	Camera functions.									//
//														//
//////////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////////

// main include file
#include"primary.h"
// allow access to bitmaps
#include"bmpfunc.h"
// allow access to materials
#include"matfunc.h"
// allow access to textures
#include"txfunc.h"
// allow access to models
#include"mdlfunc.h"
// header for this module
#include"camfunc.h"
// allow access to main window
#include"winmain.h"

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DCamera::CD3DCamera()					//
//		(defined in "camfunc.h")						//
//														//
//	Class constructor for camera.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

CD3DCamera::CD3DCamera()
{
	// reset values
	dwNumPos=0;
	dwReadPos=0;
	dwWritePos=0;
	lpvCamera=NULL;
	INIT_VAR(vOffset);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DCamera::Initialize()					//
//		(defined in "camfunc.h")						//
//														//
//	Initialize camera object.							//
//	  Parameters:										//
//		CAMERA_INIT_PTR: initial values					//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CD3DCamera::Initialize(CAMERA_INIT_PTR lpCI)
{
	// initial position vector 
	D3DVECTOR vPos;
	// counter
	DWORD dwCount;
	
	// set number of positions
	dwNumPos=lpCI->dwNumPos;
	// allocate position memory
	SAFE_ALLOC(lpvCamera,
			   LPD3DVECTOR,
			   sizeof(D3DVECTOR)*dwNumPos);
	// check for error
	if(!lpvCamera)
		return(FALSE);
	// save offset
	vOffset=lpCI->vOffset;
	// set write position
	dwWritePos=dwNumPos-1;
	// set read position
	dwReadPos=0;
	// compute initial position
	vPos=lpCI->vPos+
		 (lpCI->vOffset.dvX*lpCI->vRight)+
		 (lpCI->vOffset.dvY*lpCI->vUp)+
		 (lpCI->vOffset.dvZ*lpCI->vAhead);
	// store initial positions
	for(dwCount=0;dwCount<dwNumPos;dwCount++)
		lpvCamera[dwCount]=vPos;
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
//	Function:  CD3DCamera::Update()						//
//		(defined in "camfunc.h")						//
//														//
//	Update camera,										//
//	  Parameters:										//
//		LPD3DVECTOR: pointer to target					//
//		LPD3DVECTOR: pointer to right vector			//
//		LPD3DVECTOR: pointer to up vector				//
//		LPD3DVECTOR: pointer to ahead vector			//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DCamera::Update(LPD3DVECTOR lpvTarget,
						LPD3DVECTOR lpvRight,
						LPD3DVECTOR lpvUp,
						LPD3DVECTOR lpvAhead)
{
	// compute position
	lpvCamera[dwWritePos]=(*lpvTarget)+
						  (vOffset.dvX*(*lpvRight))+
						  (vOffset.dvY*(*lpvUp))+
						  (vOffset.dvZ*(*lpvAhead));
	// update pointers
	dwWritePos++;
	if(dwWritePos==dwNumPos)
		dwWritePos=0;
	dwReadPos++;
	if(dwReadPos==dwNumPos)
		dwReadPos=0;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DCamera::Position()					//
//		(defined in "camfunc.h")						//
//														//
//	Get camera position.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPD3DVECTOR: pointer to camera position			//
//														//
//////////////////////////////////////////////////////////

LPD3DVECTOR CD3DCamera::Position(void)
{
	return(&lpvCamera[dwReadPos]);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DCamera::Release()					//
//		(defined in "camfunc.h")						//
//														//
//	Relase all memory for camera.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DCamera::Release(void)
{
	// free camera position memory
	SAFE_FREE(lpvCamera);
}

// END ///////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DCamera::
//		(defined in "camfunc.h")						//
//														//
//	Description.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

