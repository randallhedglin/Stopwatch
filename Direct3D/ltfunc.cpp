// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  ltfunc.cpp									//
//														//
//	Light functions.									//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// header for this module
#include"ltfunc.h"
// allow access to DirectDraw
#include"ddfunc.h"
// allow access to main window
#include"winmain.h"

// ERROR MESSAGES ////////////////////////////////////////

// illegal operation
#define ERR_LTFUNC00 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: LTFUNC.CPP\nError Code: 00\nIllegal operation.") }
// failed to create light
#define ERR_LTFUNC01 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: LTFUNC.CPP\nError Code: 01\nUnable to create light.") }
// failed to set light
#define ERR_LTFUNC02 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: LTFUNC.CPP\nError Code: 02\nUnable to set lighting parameters.") }
// failed to add light
#define ERR_LTFUNC03 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: LTFUNC.CPP\nError Code: 03\nUnable to add light to scene.") }
// failed to remove light
#define ERR_LTFUNC04 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: LTFUNC.CPP\nError Code: 04\nUnable to remove light from scene.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::CD3DLight()					//
//		(defined in "ltfunc.h")							//
//														//
//	Light class constructor.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

CD3DLight::CD3DLight()
{
	// reset values	
	lpLight=NULL;
	INIT_VAR(light);
	dwType=NULL;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::CreatePoint()					//
//		(defined in "ltfunc.h")							//
//														//
//	Create point light.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CD3DLight::CreatePoint(void)
{
	// create light
	if(FAILED(GetGlobalDirect3DPtr()->CreateLight(&lpLight,
												  NULL)))
	{
		ERR_LTFUNC01;
		return(FALSE);
	}
	// prepare structure
	INIT_STRUCT(light);
	// set type
	light.dltType=D3DLIGHT_POINT;
	// set color
	light.dcvColor.dvR=1.0F;
	light.dcvColor.dvG=1.0F;
	light.dcvColor.dvB=1.0F;
	light.dcvColor.dvA=1.0F;
	// set position
	light.dvPosition.dvX=0;
	light.dvPosition.dvY=0;
	light.dvPosition.dvZ=0;
	// set range
	light.dvRange=1.0F;
	// set attenuation
	light.dvAttenuation0=0.0F;
	light.dvAttenuation1=1.0F;
	light.dvAttenuation2=0.0F;
	// set light
	if(FAILED(lpLight->SetLight((LPD3DLIGHT)&light)))
	{
		ERR_LTFUNC02;
		return(FALSE);
	}
	// add light
	if(FAILED(GetGlobalViewportPtr()->AddLight(lpLight)))
	{
		ERR_LTFUNC03;
		return(FALSE);
	}
	// set type
	dwType=LT_POINT;
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::CreateParallelPoint()			//
//		(defined in "ltfunc.h")							//
//														//
//	Create a parallel point light.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CD3DLight::CreateParallelPoint(void)
{
	// create light
	if(FAILED(GetGlobalDirect3DPtr()->CreateLight(&lpLight,
												  NULL)))
	{
		ERR_LTFUNC01;
		return(FALSE);
	}
	// prepare structure
	INIT_STRUCT(light);
	// set type
	light.dltType=D3DLIGHT_PARALLELPOINT;
	// set color
	light.dcvColor.dvR=1.0F;
	light.dcvColor.dvG=1.0F;
	light.dcvColor.dvB=1.0F;
	light.dcvColor.dvA=1.0F;
	// set position
	light.dvPosition.dvX=0;
	light.dvPosition.dvY=0;
	light.dvPosition.dvZ=0;
	// set light
	if(FAILED(lpLight->SetLight((LPD3DLIGHT)&light)))
	{
		ERR_LTFUNC02;
		return(FALSE);
	}
	// add light
	if(FAILED(GetGlobalViewportPtr()->AddLight(lpLight)))
	{
		ERR_LTFUNC03;
		return(FALSE);
	}
	// set type
	dwType=LT_PARALLELPOINT;
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::CreateSpot()					//
//		(defined in "ltfunc.h")							//
//														//
//	Create spotlight.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CD3DLight::CreateSpot(void)
{
	// create light
	if(FAILED(GetGlobalDirect3DPtr()->CreateLight(&lpLight,
												  NULL)))
	{
		ERR_LTFUNC01;
		return(FALSE);
	}
	// prepare structure
	INIT_STRUCT(light);
	// set type
	light.dltType=D3DLIGHT_SPOT;
	// set color
	light.dcvColor.dvR=1.0F;
	light.dcvColor.dvG=1.0F;
	light.dcvColor.dvB=1.0F;
	light.dcvColor.dvA=1.0F;
	// set position
	light.dvPosition.dvX=0;
	light.dvPosition.dvY=0;
	light.dvPosition.dvZ=0;
	// set direction
	light.dvDirection.dvX=0;
	light.dvDirection.dvY=0;
	light.dvDirection.dvZ=1.0F;
	// set range
	light.dvRange=1.0F;
	// set falloff
	light.dvFalloff=1.0F;
	// set attenuation
	light.dvAttenuation0=0.0F;
	light.dvAttenuation1=1.0F;
	light.dvAttenuation2=0.0F;
	// set angles
	light.dvTheta=0.5F;
	light.dvPhi=1.0F;
	// set light
	if(FAILED(lpLight->SetLight((LPD3DLIGHT)&light)))
	{
		ERR_LTFUNC02;
		return(FALSE);
	}
	// add light
	if(FAILED(GetGlobalViewportPtr()->AddLight(lpLight)))
	{
		ERR_LTFUNC03;
		return(FALSE);
	}
	// set type
	dwType=LT_SPOT;
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::CreateDirectional()			//
//		(defined in "ltfunc.h")							//
//														//
//	Create directional light.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CD3DLight::CreateDirectional(void)
{
	// create light
	if(FAILED(GetGlobalDirect3DPtr()->CreateLight(&lpLight,
												  NULL)))
	{
		ERR_LTFUNC01;
		return(FALSE);
	}
	// prepare structure
	INIT_STRUCT(light);
	// set type
	light.dltType=D3DLIGHT_DIRECTIONAL;
	// set color
	light.dcvColor.dvR=1.0F;
	light.dcvColor.dvG=1.0F;
	light.dcvColor.dvB=1.0F;
	light.dcvColor.dvA=1.0F;
	// set direction
	light.dvDirection.dvX=0;
	light.dvDirection.dvY=0;
	light.dvDirection.dvZ=1.0F;
	// set light
	if(FAILED(lpLight->SetLight((LPD3DLIGHT)&light)))
	{
		ERR_LTFUNC02;
		return(FALSE);
	}
	// add light
	if(FAILED(GetGlobalViewportPtr()->AddLight(lpLight)))
	{
		ERR_LTFUNC03;
		return(FALSE);
	}
	// set type
	dwType=LT_DIRECTIONAL;
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
//	Function:  CD3DLight::Color()						//
//		(defined in "ltfunc.h")							//
//														//
//	Change light color.									//
//	  Parameters:										//
//		D3DVALUE,D3DVALUE,D3DVALUE: RGB					//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DLight::Color(D3DVALUE dvRed,
					  D3DVALUE dvGreen,
					  D3DVALUE dvBlue)
{
	// set color
	light.dcvColor.dvR=dvRed;
	light.dcvColor.dvG=dvGreen;
	light.dcvColor.dvB=dvBlue;
	light.dcvColor.dvA=1.0F;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::Position()					//
//		(defined in "ltfunc.h")							//
//														//
//	Change light position.								//
//	  Parameters:										//
//		D3DVECTOR*: new position						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DLight::Position(D3DVECTOR* lpvPos)
{
#ifdef SF
	// set position scaled
	light.dvPosition=(*lpvPos)*SF;
#else
	// set position
	light.dvPosition=*lpvPos;
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::Direction()					//
//		(defined in "ltfunc.h")							//
//														//
//	Change light direction.								//
//	  Parameters:										//
//		D3DVECTOR*: new direction						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DLight::Direction(D3DVECTOR* lpvDir)
{
	// set direction
	light.dvDirection=*lpvDir;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::Range()						//
//		(defined in "ltfunc.h")							//
//														//
//	Set light range.									//
//	  Parameters:										//
//		D3DVALUE: range									//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DLight::Range(D3DVALUE dvRange)
{
#ifdef SF
	// set range scaled
	light.dvRange=dvRange*SF;
#else
	// set range
	light.dvRange=dvRange;
#endif
}
	
//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::Attenuation()					//
//		(defined in "ltfunc.h")							//
//														//
//	Change light attenuation.							//
//	  Parameters:										//
//		D3DVALUE,D3DVALUE,D3DVALUE: attenuation values	//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DLight::Attenuation(D3DVALUE dvAtt0,
							D3DVALUE dvAtt1,
							D3DVALUE dvAtt2)
{
	// set attenuation
	light.dvAttenuation0=dvAtt0;
	light.dvAttenuation1=dvAtt1;
	light.dvAttenuation2=dvAtt2;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::Falloff()						//
//		(defined in "ltfunc.h")							//
//														//
//	Change light falloff.								//
//	  Parameters:										//
//		D3DVALUE: falloff value							//
//		D3DVALUE: outer cone angle						//
//		D3DVALUE: inner cone angle						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DLight::Falloff(D3DVALUE dvFalloff,
						D3DVALUE dvTheta,
						D3DVALUE dvPhi)
{
	// set falloff
	light.dvFalloff=dvFalloff;
	// set angles
	light.dvTheta=dvTheta;
	light.dvPhi=dvPhi;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::On()							//
//		(defined in "ltfunc.h")							//
//														//
//	Turn light on.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DLight::On(void)
{
	// check for specular & turn on
	if(light.dwFlags|D3DLIGHT_NO_SPECULAR)
		light.dwFlags=D3DLIGHT_ACTIVE|
					  D3DLIGHT_NO_SPECULAR;
	else
		light.dwFlags=D3DLIGHT_ACTIVE;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::Off()							//
//		(defined in "ltfunc.h")							//
//														//
//	Turn light off.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DLight::Off(void)
{
	// check for specular & turn off
	if(light.dwFlags|D3DLIGHT_NO_SPECULAR)
		light.dwFlags=D3DLIGHT_NO_SPECULAR;
	else
		light.dwFlags=NULL;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::SpecularOn()					//
//		(defined in "ltfunc.h")							//
//														//
//	Turn on specular highlights.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DLight::SpecularOn(void)
{
	// check for active & turn specular on
	if(light.dwFlags|D3DLIGHT_ACTIVE)
		light.dwFlags=D3DLIGHT_ACTIVE;
	else
		light.dwFlags=NULL;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::SpecularOff()					//
//		(defined in "ltfunc.h")							//
//														//
//	Turn off specular highlights.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DLight::SpecularOff(void)
{
	// check for active & turn specular off
	if(light.dwFlags|D3DLIGHT_ACTIVE)
		light.dwFlags=D3DLIGHT_ACTIVE|
				   D3DLIGHT_NO_SPECULAR;
	else
		light.dwFlags=D3DLIGHT_NO_SPECULAR;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::Update()						//
//		(defined in "ltfunc.h")							//
//														//
//	Update light values.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DLight::Update(void)
{
	// set light
#ifdef DEBUG_MODE
	if(FAILED(lpLight->SetLight((LPD3DLIGHT)&light)))
	{
		ERR_LTFUNC02;
		return;
	}
#else
	lpLight->SetLight((LPD3DLIGHT)&light);
#endif
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DLight::Release()						//
//		(defined in "ltfunc.h")							//
//														//
//	Release light.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DLight::Release(void)
{
	// remove from viewport
	if(FAILED(GetGlobalViewportPtr()->DeleteLight(lpLight)))
		ERR_LTFUNC04;
	// release light
	SAFE_RELEASE(lpLight);
	// reset values	
	lpLight=NULL;
	INIT_VAR(light);
	dwType=NULL;
}

// END ///////////////////////////////////////////////////

