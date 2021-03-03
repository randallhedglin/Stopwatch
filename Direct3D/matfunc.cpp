// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  matfunc.cpp									//
//														//
//	Materials functions.								//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// header for this module
#include"matfunc.h"
// allow access to DirectDraw
#include"ddfunc.h"
// allow access to Direct3D
#include"d3dfunc.h"
// allow access to main window
#include"winmain.h"

// ERROR MESSAGES ////////////////////////////////////////

// failed to create material
#define ERR_MATFUNC00 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MATFUNC.CPP\nError Code: 00\nUnable to create material.") }
// failed to set material
#define ERR_MATFUNC01 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MATFUNC.CPP\nError Code: 01\nUnable to set material description.") }
// failed to attach material
#define ERR_MATFUNC02 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MATFUNC.CPP\nError Code: 02\nUnable to attach material.") }
// failed to set material current
#define ERR_MATFUNC03 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MATFUNC.CPP\nError Code: 02\nUnable to set current material.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DMaterial::CD3DMaterial()				//
//		(defined in "matfunc.h")						//
//														//
//	Class constructor for materials.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

CD3DMaterial::CD3DMaterial()
{
	// reset variables
	lpMat=NULL;
	hMat=NULL;
	INIT_VAR(mat);
	lpdwLastModel=NULL;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DMaterial::Create()					//
//		(defined in "matfunc.h")						//
//														//
//	Create a Direct3D material.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CD3DMaterial::Create(void)
{
	// create material
	if(FAILED(GetGlobalDirect3DPtr()->CreateMaterial(&lpMat,NULL)))
	{
		ERR_MATFUNC00;
		return(FALSE);
	}
	// prepare material
	INIT_STRUCT(mat);
	// set material
	if(FAILED(lpMat->SetMaterial(&mat)))
	{
		ERR_MATFUNC01;
		return(FALSE);
	}
	// get handle
	if(FAILED(lpMat->GetHandle(GetGlobalDirect3DDevicePtr(),
							   &hMat)))
	{
		ERR_MATFUNC02
		return(FALSE);
	}
	// set D3D Device
	lpD3DDevice=GetGlobalDirect3DDevicePtr();
	// set pointer to last model
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
//	Function:  CD3DMaterial::Diffuse()					//
//		(defined in "matfunc.h")						//
//														//
//	Set material diffuse color.							//
//	  Parameters:										//
//		D3DVALUE,D3DVALUE,D3DVALUE,D3DVALUE: RGBA		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DMaterial::Diffuse(D3DVALUE dvRed,
						   D3DVALUE dvGreen,
						   D3DVALUE dvBlue,
						   D3DVALUE dvAlpha)
{
	// set colors
	mat.dcvDiffuse.dvR=dvRed;
	mat.dcvDiffuse.dvG=dvGreen;
	mat.dcvDiffuse.dvB=dvBlue;
	mat.dcvDiffuse.dvA=dvAlpha;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DMaterial::Ambient()					//
//		(defined in "matfunc.h")						//
//														//
//	Set material ambient color.							//
//	  Parameters:										//
//		D3DVALUE,D3DVALUE,D3DVALUE,D3DVALUE: RGBA		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DMaterial::Ambient(D3DVALUE dvRed,
						   D3DVALUE dvGreen,
						   D3DVALUE dvBlue,
						   D3DVALUE dvAlpha)
{
	// set colors
	mat.dcvAmbient.dvR=dvRed;
	mat.dcvAmbient.dvG=dvGreen;
	mat.dcvAmbient.dvB=dvBlue;
	mat.dcvAmbient.dvA=dvAlpha;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DMaterial::Specular()					//
//		(defined in "matfunc.h")						//
//														//
//	Set material specular color.						//
//	  Parameters:										//
//		D3DVALUE,D3DVALUE,D3DVALUE,D3DVALUE: RGBA		//
//		D3DVALUE: specular power						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DMaterial::Specular(D3DVALUE dvRed,
							D3DVALUE dvGreen,
							D3DVALUE dvBlue,
							D3DVALUE dvAlpha,
							D3DVALUE dvPower)
{
	// set colors
	mat.dcvSpecular.dvR=dvRed;
	mat.dcvSpecular.dvG=dvGreen;
	mat.dcvSpecular.dvB=dvBlue;
	mat.dcvSpecular.dvA=dvAlpha;
	// set power
	mat.dvPower=dvPower;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DMaterial::Emissive()					//
//		(defined in "matfunc.h")						//
//														//
//	Set material emissive color.						//
//	  Parameters:										//
//		D3DVALUE,D3DVALUE,D3DVALUE,D3DVALUE: RGBA		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DMaterial::Emissive(D3DVALUE dvRed,
							D3DVALUE dvGreen,
							D3DVALUE dvBlue,
							D3DVALUE dvAlpha)
{
	// set colors
	mat.dcvEmissive.dvR=dvRed;
	mat.dcvEmissive.dvG=dvGreen;
	mat.dcvEmissive.dvB=dvBlue;
	mat.dcvEmissive.dvA=dvAlpha;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DMaterial::Update()					//
//		(defined in "matfunc.h")						//
//														//
//	Update material settings.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DMaterial::Update(void)
{
#ifdef DEBUG_MODE
	// update
	if(FAILED(lpMat->SetMaterial(&mat)))
		ERR_MATFUNC01;
#else
	lpMat->SetMaterial(&mat);
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DMaterial::Current()					//
//		(defined in "matfunc.h")						//
//														//
//	Set this material as current.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DMaterial::Current(void)
{
	// set handle
#ifdef DEBUG_MODE
	if(FAILED(lpD3DDevice->SetLightState(D3DLIGHTSTATE_MATERIAL,
										 hMat)))
	{
		ERR_MATFUNC03;
		return;
	}
#else
	lpD3DDevice->SetLightState(D3DLIGHTSTATE_MATERIAL,
							   hMat);
#endif
	// reset global model number
	*lpdwLastModel=0xFFFFFFFF;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DMaterial::Release()					//
//		(defined in "matfunc.h")						//
//														//
//	Description.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DMaterial::Release(void)
{
	// release material
	SAFE_RELEASE(lpMat);
	// reset variables
	lpMat=NULL;
	hMat=NULL;
	INIT_VAR(mat);
}

// END ///////////////////////////////////////////////////

