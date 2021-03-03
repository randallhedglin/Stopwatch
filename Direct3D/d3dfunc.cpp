// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  d3dfunc.cpp									//
//														//
//	Direct3D functions.									//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// header for this module
#include"d3dfunc.h"
// allow access to main window functions
#include"winmain.h"
// allow access to DirectDraw
#include"ddfunc.h"
// allow access to file functions
#include"filefunc.h"
// allow access to bitmap functions
#include"bmpfunc.h"

// ERROR MESSAGES ////////////////////////////////////////

// failed to begin scene
#define ERR_D3DFUNC00 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 00\nUnable to begin scene render.") }
// failed to end scene
#define ERR_D3DFUNC01 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 01\nUnable to end scene render.") }
// failed to enable fog
#define ERR_D3DFUNC02 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 02\nUnable to initialize fog effects.") }
// failed to disable fog
#define ERR_D3DFUNC03 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 03\nUnable to remove fog effects.") }
// failed to clear viewport
#define ERR_D3DFUNC04 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 04\nUnable to clear viewport / z-buffer.") }
// failed to disable z-buffer
#define ERR_D3DFUNC05 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 05\nUnable to disable z-buffer.") }
// failed to enable z-buffer
#define ERR_D3DFUNC06 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 06\nUnable to enable z-buffer.") }
// failed to disable z-buffer writes
#define ERR_D3DFUNC07 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 07\nUnable to disable z-buffer writes.") }
// failed to enable z-buffer writes
#define ERR_D3DFUNC08 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 08\nUnable to enable z-buffer writes.") }
// failed to enable backface culling
#define ERR_D3DFUNC09 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 09\nUnable to enable backface culling.") }
// failed to disable backface culling
#define ERR_D3DFUNC10 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 10\nUnable to disable backface culling.") }
// failed to set world matrix
#define ERR_D3DFUNC11 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 11\nUnable to set world matrix.") }
// failed to set view matrix
#define ERR_D3DFUNC12 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 12\nUnable to set view matrix.") }
// failed to set projection matrix
#define ERR_D3DFUNC13 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 13\nUnable to set projection matrix.") }
// failed to invert matrix
#define ERR_D3DFUNC14 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 14\nUnable to invert matrix.") }
// failed to compute
#define ERR_D3DFUNC15 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 15\nMatrix calculation failed.") }
// failed to set anti-alias
#define ERR_D3DFUNC16 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 16\nUnable to set anti-alias flag.") }
// failed to set Gouraud
#define ERR_D3DFUNC17 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 17\nUnable to set smooth shade flag.") }
// failed to set specular
#define ERR_D3DFUNC18 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 18\nUnable to set specular flag.") }
// failed to set alpha
#define ERR_D3DFUNC19 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 19\nUnable to set alpha flag.") }
// failed to set perspective-correct
#define ERR_D3DFUNC20 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 20\nUnable to set perspective correction flag.") }
// failed to set ambient light
#define ERR_D3DFUNC21 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 21\nUnable to set ambient light level.") }
// failed to set z-bias
#define ERR_D3DFUNC22 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 22\nUnable to set z-bias level.") }
// failed to set texture mode
#define ERR_D3DFUNC23 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 23\nUnable to set texture mode.") }
// failed to set texture border color
#define ERR_D3DFUNC24 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 24\nUnable to set texture border color.") }
// failed to set texture filter
#define ERR_D3DFUNC25 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 25\nUnable to set requested texture filter.") }
// failed to evict textures
#define ERR_D3DFUNC26 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 26\nUnable to clear texture buffers.") }
// failed to erase material
#define ERR_D3DFUNC27 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 27\nUnable to eliminate current material.") }
// failed to erase texture
#define ERR_D3DFUNC28 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 28\nUnable to eliminate current texture.") }
// failed to set alpha testing
#define ERR_D3DFUNC29 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: D3DFUNC.CPP\nError Code: 29\nUnable to set alpha testing flag.") }

// GLOBALS ///////////////////////////////////////////////

// global fog flag
BOOL g_bUseFog3D=FALSE;
// fog enabled flag
BOOL g_bFogEnabled=FALSE;
// global z-buffer flag
BOOL g_bUseZBuffer3D=FALSE;
// global pointer to Direct3DDevice
LPDIRECT3DDEVICE3 g_lpD3DDevice3D=NULL;
// global last model
DWORD g_dwLastModel=0xFFFFFFFF;
// alpha flag
BOOL g_bAlphaEnabled=FALSE;
// global pointer to viewport interface
LPDIRECT3DVIEWPORT3 g_lpViewport3D=NULL;
// global screen rectangle pointer
LPD3DRECT g_lprScreen3D=NULL;
// global billboard vectors
D3DVECTOR g_vBBRight=D3DVECTOR(1,0,0);
D3DVECTOR g_vBBUp=D3DVECTOR(0,1,0);
// anti-alias flag
BOOL g_bAntiAliasEnabled=FALSE;
// global model counter
DWORD g_dwModelNum=0;
// global billboard vectors
D3DVECTOR g_vBBAhead=D3DVECTOR(0,0,1);
// Gouraud flag
BOOL g_bGouraudEnabled=FALSE;
// specular flag
BOOL g_bSpecularEnabled=FALSE;
// perspective-correct flag
BOOL g_bPerspectiveCorrectEnabled=FALSE;
// linear texturing flag
BOOL g_bLinearEnabled=FALSE;

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  SetD3DGlobals()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Set up global values used in D3D functions.			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern void SetD3DGlobals(void)
{
	g_lpD3DDevice3D=GetGlobalDirect3DDevicePtr();
	g_lpViewport3D=GetGlobalViewportPtr();
	g_bUseFog3D=GetGlobalUseFogFlag();
	g_bUseZBuffer3D=GetGlobalUseZBufferFlag();
	g_lprScreen3D=GetGlobalScreenRectanglePtr();
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  BeginScene()								//
//		(defined in "d3dfunc.h")						//
//														//
//	Signals Direct3D to begin a rendering scene.		//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL BeginScene(void)
{
	// begin scene
#ifdef DEBUG_MODE
	if(FAILED(g_lpD3DDevice3D->BeginScene()))
	{
		ERR_D3DFUNC00;
		return(FALSE);
	}
#else
	g_lpD3DDevice3D->BeginScene();
#endif
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
//	Function:  EndScene()								//
//		(defined in "d3dfunc.h")						//
//														//
//	End the current scene.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EndScene(void)
{
	// end scene
#ifdef DEBUG_MODE
	if(FAILED(g_lpD3DDevice3D->EndScene()))
	{
		ERR_D3DFUNC01;
		return;
	}
#else
	g_lpD3DDevice3D->EndScene();
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  SetFogStart()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Set fog starting value.								//
//	  Parameters:										//
//		D3DVALUE: start depth of fog					//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetFogStart(D3DVALUE dvStart)
{
	// only if req'd
	if(g_bUseFog3D)
	{
#ifdef SF
		// scale end value
		dvStart*=SF;
#endif
		// set start
#ifdef DEBUG_MODE
		if(FAILED(g_lpD3DDevice3D->SetLightState(D3DLIGHTSTATE_FOGSTART,
												 *(DWORD*)(&dvStart))))
		{
			ERR_D3DFUNC02;
			return;
		}
#else
		g_lpD3DDevice3D->SetLightState(D3DLIGHTSTATE_FOGSTART,
			 						   *(DWORD*)(&dvStart));
#endif
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  SetFogEnd()								//
//		(defined in "d3dfunc.h")						//
//														//
//	Set fog ending value.								//
//	  Parameters:										//
//		D3DVALUE: end depth of fog						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetFogEnd(D3DVALUE dvEnd)
{
	// only if req'd
	if(g_bUseFog3D)
	{
#ifdef SF
		// scale end value
		dvEnd*=SF;
#endif
#ifdef DEBUG_MODE
		if(FAILED(g_lpD3DDevice3D->SetLightState(D3DLIGHTSTATE_FOGEND,
												 *(DWORD*)(&dvEnd))))
		{
			ERR_D3DFUNC02;
			return;
		}
#else
		g_lpD3DDevice3D->SetLightState(D3DLIGHTSTATE_FOGEND,
									   *(DWORD*)(&dvEnd));
#endif
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  SetFogColor()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Set color of fog.									//
//	  Parameters:										//
//		D3DCOLOR: color of fog							//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetFogColor(D3DCOLOR dcColor)
{
	// only if req'd
	if(g_bUseFog3D)
	{
#ifdef DEBUG_MODE
		if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_FOGCOLOR,
												  dcColor)))
		{
			ERR_D3DFUNC02;
			return;
		}
#else
		g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_FOGCOLOR,
										dcColor);
#endif
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EnableFog()								//
//		(defined in "d3dfunc.h")						//
//														//
//	Turns on fog effects, if enabled.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EnableFog(void)
{
	// only if req'd
	if(g_bUseFog3D)
	{
#ifdef DEBUG_MODE
		if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_FOGENABLE,
												  TRUE)))
		{
			ERR_D3DFUNC02;
			return;
		}
#else
		g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_FOGENABLE,
										TRUE);
#endif
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  DisableFog()								//
//		(defined in "d3dfunc.h")						//
//														//
//	Turn off fog effects.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void DisableFog(void)
{
	// disable fog
#ifdef DEBUG_MODE
	if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_FOGENABLE,
											  FALSE)))
	{
		ERR_D3DFUNC03;
		return;
	}
#else
	g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_FOGENABLE,
								    FALSE);
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  ClearViewport()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Clear viewport to speified color & empty z-buffer.	//
//	  Parameters:										//
//		BYTE,BYTE,BYTE: RGB value of clear color		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void ClearViewport(D3DCOLOR dcColor)
{
	// clear viewport & z-buffer
#ifdef DEBUG_MODE
	if(FAILED(g_lpViewport3D->Clear2(1,
								     g_lprScreen3D,
								     D3DCLEAR_TARGET|
								     D3DCLEAR_ZBUFFER,
								     dcColor,
								     1.0F,
								     NULL)))
	{
		ERR_D3DFUNC04;
		return;
	}
#else
	g_lpViewport3D->Clear2(1,
					       g_lprScreen3D,
					       D3DCLEAR_TARGET|
					       D3DCLEAR_ZBUFFER,
					       dcColor,
					       1.0F,
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
//	Function:  ClearZBuffer()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Clear z-buffer.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void ClearZBuffer(void)
{
	// are we using z-buffer?
	if(g_bUseZBuffer3D)
	{
		// clear z-buffer
#ifdef DEBUG_MODE
		if(FAILED(g_lpViewport3D->Clear2(1,
										 g_lprScreen3D,
										 D3DCLEAR_ZBUFFER,
										 NULL,
										 1.0F,
										 NULL)))
		{
			ERR_D3DFUNC04;
			return;
		}
#else
		g_lpViewport3D->Clear2(1,
							   g_lprScreen3D,
							   D3DCLEAR_ZBUFFER,
							   NULL,
							   1.0F,
							   NULL);
#endif
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  DisableZBuffer()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Turn off z-buffer use.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void DisableZBuffer(void)
{
	// are we using z-buffer?
	if(g_bUseZBuffer3D)
		// turn it off
#ifdef DEBUG_MODE
		if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ZENABLE,
												  FALSE)))
		{
			ERR_D3DFUNC05;
			return;
		}
#else
		g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ZENABLE,
									    FALSE);
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EnableZBuffer()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Turns on z-buffer access.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EnableZBuffer(void)
{
	// are we using z-buffer?
	if(g_bUseZBuffer3D)
		// turn it on
#ifdef DEBUG_MODE
		if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ZENABLE,
												  TRUE)))
		{
			ERR_D3DFUNC06;
			return;
		}
#else
		g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ZENABLE,
									    TRUE);
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  DisableZBufferWrites()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Block z-buffer writes but allow reads.				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void DisableZBufferWrites(void)
{
	// are we using z-buffer?
	if(g_bUseZBuffer3D)
		// turn off writes
#ifdef DEBUG_MODE
		if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE,
												  FALSE)))
		{
			ERR_D3DFUNC07;
			return;
		}
#else
		g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE,
									    FALSE);
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EnableZBufferWrites()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Allow full z-buffer access.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EnableZBufferWrites(void)
{
	// are we using z-buffer?
	if(g_bUseZBuffer3D)
		// turn on writes
#ifdef DEBUG_MODE
		if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE,
												  TRUE)))
		{
			ERR_D3DFUNC08;
			return;
		}
#else
		g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE,
									    TRUE);
#endif
}

//////////////////////////////////////////////////////////
//														//
//	Function:  EnableBackFaceCull()						//
//		(defined in "d3dfunc.h")						//
//														//
//	Enable backface culling.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EnableBackFaceCull(void)
{
	// set counter-clockwise culling
	if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_CULLMODE,
  											D3DCULL_CCW)))
	{
		ERR_D3DFUNC09;
		return;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  EnableBackFaceCullReversed()				//
//		(defined in "d3dfunc.h")						//
//														//
//	Enable reverse backface culling.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EnableBackFaceCullReverse(void)
{
	// set clockwise culling
	if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_CULLMODE,
  											D3DCULL_CW)))
	{
		ERR_D3DFUNC09;
		return;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  DisableBackFaceCull()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Turn off backface culling.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void DisableBackFaceCull(void)
{
	// disable culling
	if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_CULLMODE,
  											D3DCULL_NONE)))
	{
		ERR_D3DFUNC10;
		return;
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  SetWorldMatrix()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Set world matrix.									//
//	  Parameters:										//
//		D3DVECTOR: position of object					//
//		D3DVECTOR: object right vector					//
//		D3DVECTOR: object up vector						//
//		D3DVECTOR: object ahead vector					//
//		D3DVALUE: scale factor
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetWorldMatrix(LPD3DVECTOR lpvPos,
						   LPD3DVECTOR lpvRight,
						   LPD3DVECTOR lpvUp,
						   LPD3DVECTOR lpvAhead,
						   D3DVALUE dvScale)
{
	// position vector
	D3DVECTOR vPos;
	// final matrix
	D3DMATRIX matWorld;

#ifdef SF
	// copy position vector scaled
	vPos=(*lpvPos)*SF;
	// scale factor scaled
	dvScale*=SF;
#else
	// copy position vector
	vPos=(*lpvPos);
#endif
	if(dvScale==1)
		matWorld=D3DMATRIX(lpvRight->dvX,lpvRight->dvY,lpvRight->dvZ,0,
						   lpvUp->dvX,lpvUp->dvY,lpvUp->dvZ,0,
						   lpvAhead->dvX,lpvAhead->dvY,lpvAhead->dvZ,0,
						   vPos.dvX,vPos.dvY,vPos.dvZ,1);
	else
		matWorld=D3DMATRIX(lpvRight->dvX*dvScale,lpvRight->dvY*dvScale,lpvRight->dvZ*dvScale,0,
						   lpvUp->dvX*dvScale,lpvUp->dvY*dvScale,lpvUp->dvZ*dvScale,0,
						   lpvAhead->dvX*dvScale,lpvAhead->dvY*dvScale,lpvAhead->dvZ*dvScale,0,
						   vPos.dvX,vPos.dvY,vPos.dvZ,1);
	// set world matrix
#ifdef DEBUG_MODE
	if(FAILED(g_lpD3DDevice3D->SetTransform(D3DTRANSFORMSTATE_WORLD,
										    &matWorld)))
	{
		ERR_D3DFUNC11;
		return;
	}
#else
	g_lpD3DDevice3D->SetTransform(D3DTRANSFORMSTATE_WORLD,
								  &matWorld);
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  SetViewMatrix()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Set camera.											//
//	  Parameters:										//
//		D3DVECTOR*: camera position						//
//		D3DVECTOR*: target point						//
//		D3DVECTOR*: up vector							//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetViewMatrix(LPD3DVECTOR lpvCamera,
						  LPD3DVECTOR lpvTarget,
						  LPD3DVECTOR lpvUp)
{
	// camera vector
	D3DVECTOR vCamera;
	// target vector
	D3DVECTOR vTarget;
	// final matrix
	D3DMATRIX matView;
	// view vector
	D3DVECTOR vView;
	// up vector
	D3DVECTOR vUp;
	// right vector
	D3DVECTOR vRight;

#ifdef SF
    // copy camera and target scaled
	vCamera=(*lpvCamera)*SF;
	vTarget=(*lpvTarget)*SF;
#else
    // copy camera and target
	vCamera=*lpvCamera;
	vTarget=*lpvTarget;
#endif
	// compute view vector
	vView=Normalize(vTarget-vCamera);
	// set globals for billboarding
	SetGlobalBillboardVectors(&vView);
	// set up vector
	vUp=*lpvUp;
	// adjust up vector
	vUp-=DotProduct(*lpvUp,vView)*vView;
	// normalize up vector
    vUp=Normalize(vUp);
	// compute right vector
    vRight=CrossProduct(vUp,vView);
    // set up matrix
	matView=D3DMATRIX(vRight.x,vUp.x,vView.x,0,
					  vRight.y,vUp.y,vView.y,0,
					  vRight.z,vUp.z,vView.z,0,
					  -DotProduct(vCamera,vRight),
					  -DotProduct(vCamera,vUp),
					  -DotProduct(vCamera,vView),
					  1);
	// set view matrix
#ifdef DEBUG_MODE
	if(FAILED(g_lpD3DDevice3D->SetTransform(D3DTRANSFORMSTATE_VIEW,
										    &matView)))
	{
		ERR_D3DFUNC12;
		return;
	}
#else
	g_lpD3DDevice3D->SetTransform(D3DTRANSFORMSTATE_VIEW,
								  &matView);
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  SetProjectionMatrix()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Sets projection matrix.								//
//	  Parameters:										//
//		D3DVALUE: angle of viewing frustum				//
//		D3DVALUE: near clipping plane					//
//		D3DVALUE: far clipping plane					//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetProjectionMatrix(D3DVALUE dvAngle,
								D3DVALUE dvNear,
								D3DVALUE dvFar)
{
	// final matrix
	D3DMATRIX matProj;
	// aspect ratio
	FLOAT fAspect;
	// used in calculations
	FLOAT fW,fH,fQ;
	// my angle
	BYTE cAngle;

#ifdef SF
	// scale planes
	dvNear*=SF;
	dvFar*=SF;
#endif
	// set aspect ratio
	fAspect=0.75f;
	// set my angle
	cAngle=(BYTE)(dvAngle*20.3718327F);
    // check for errors
	if(fabs(dvFar-dvNear)<0.01f)
	{
		ERR_D3DFUNC15;
		return;
	}
    if(fabs(GetSin(cAngle))<0.01f)
	{
		ERR_D3DFUNC15;
		return;
	}
	// compute w
    fW=fAspect*(GetCos(cAngle)/GetSin(cAngle));
    // compute h
	fH=GetCos(cAngle)/GetSin(cAngle);
    // compute Q
	fQ=dvFar/(dvFar-dvNear);
	// clear matrix
	ZeroMemory(&matProj,sizeof(D3DMATRIX));
    // set up matrix
	matProj._11=fW;
    matProj._22=fH;
    matProj._33=fQ;
    matProj._34=1.0f;
    matProj._43=-fQ*dvNear;
#ifdef DEBUG_MODE
	// set projection matrix
	if(FAILED(g_lpD3DDevice3D->SetTransform(D3DTRANSFORMSTATE_PROJECTION,
										    &matProj)))
	{
		ERR_D3DFUNC13;
		return;
	}
#else
	// set projection matrix
	g_lpD3DDevice3D->SetTransform(D3DTRANSFORMSTATE_PROJECTION,
								  &matProj);
#endif
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalUseFogFlag()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Retrieves fog enabled flag.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: flag										//
//														//
//////////////////////////////////////////////////////////

extern BOOL GetGlobalUseFogFlag(void)
{
	return(g_bFogEnabled);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalUseFogFlag()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Sets fog enabled flag.								//
//	  Parameters:										//
//		BOOL: flag										//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetGlobalUseFogFlag(BOOL bFlag)
{
	g_bFogEnabled=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalUseAntiAliasFlag()				//
//		(defined in "d3dfunc.h")						//
//														//
//	Retrieve anti-alias flag.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: flag										//
//														//
//////////////////////////////////////////////////////////

extern BOOL GetGlobalUseAntiAliasFlag(void)
{
	return(g_bAntiAliasEnabled);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalUseAntiAliasFlag()				//
//		(defined in "d3dfunc.h")						//
//														//
//	Set anti-alias flag.								//
//	  Parameters:										//
//		BOOL: flag										//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetGlobalUseAntiAliasFlag(BOOL bFlag)
{
	g_bAntiAliasEnabled=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalUseGouraudFlag()				//
//		(defined in "d3dfunc.h")						//
//														//
//	Get Gouraud flag.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: flag										//
//														//
//////////////////////////////////////////////////////////

extern BOOL GetGlobalUseGouraudFlag(void)
{
	return(g_bGouraudEnabled);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalUseGouraudFlag()				//
//		(defined in "d3dfunc.h")						//
//														//
//	Set Gouraud flag.									//
//	  Parameters:										//
//		BOOL: flag										//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetGlobalUseGouraudFlag(BOOL bFlag)
{
	g_bGouraudEnabled=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalUseSpecularFlag()				//
//		(defined in "d3dfunc.h")						//
//														//
//	Get specular flag.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: flag										//
//														//
//////////////////////////////////////////////////////////

extern BOOL GetGlobalUseSpecularFlag(void)
{
	return(g_bSpecularEnabled);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalUseSpecularFlag()				//
//		(defined in "d3dfunc.h")						//
//														//
//	Set specular flag.									//
//	  Parameters:										//
//		BOOL: flag										//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetGlobalUseSpecularFlag(BOOL bFlag)
{
	g_bSpecularEnabled=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalUseAlphaFlag()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Get alpha flag.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: flag										//
//														//
//////////////////////////////////////////////////////////

extern BOOL GetGlobalUseAlphaFlag(void)
{
	return(g_bAlphaEnabled);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalUseAlphaFlag()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Set alpha flag.										//
//	  Parameters:										//
//		BOOL: flag										//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetGlobalUseAlphaFlag(BOOL bFlag)
{
	g_bAlphaEnabled=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalUsePerspectiveCorrectFlag()		//
//		(defined in "d3dfunc.h")						//
//														//
//	Get perspective-correct flag.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: flag										//
//														//
//////////////////////////////////////////////////////////

extern BOOL GetGlobalUsePerspectiveCorrectFlag(void)
{
	return(g_bPerspectiveCorrectEnabled);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalUsePerspectiveCorrectFlag()		//
//		(defined in "d3dfunc.h")						//
//														//
//	Set perspective-correct flag.						//
//	  Parameters:										//
//		BOOL: flag										//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetGlobalUsePerspectiveCorrectFlag(BOOL bFlag)
{
	g_bPerspectiveCorrectEnabled=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  UpdateSettings()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Update 3D settings.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void UpdateSettings(void)
{
	// set anti-aliasing
	if(g_bAntiAliasEnabled)
	{
		// on
		if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ANTIALIAS,
  												  D3DANTIALIAS_SORTINDEPENDENT)))
			ERR_D3DFUNC16;
	}
	else
	{
		// off
		if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ANTIALIAS,
  												  D3DANTIALIAS_NONE)))
			ERR_D3DFUNC16;
	}
	// set Gouraud
	if(g_bGouraudEnabled)
	{
		// on
		if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_SHADEMODE,
												  D3DSHADE_GOURAUD)))
			ERR_D3DFUNC17;
	}
	else
	{
		// off
		if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_SHADEMODE,
												  D3DSHADE_FLAT)))
			ERR_D3DFUNC17;
	}
	// set specular
	if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_SPECULARENABLE,
											  g_bSpecularEnabled)))
		ERR_D3DFUNC18;
	// set perspective-correct
	if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE,
											  g_bPerspectiveCorrectEnabled)))
		ERR_D3DFUNC20;
	// set texture filter
	if(!g_bLinearEnabled)	
	{
		// set magnification
		if(FAILED(g_lpD3DDevice3D->SetTextureStageState(0,
													    D3DTSS_MAGFILTER,
													    D3DTFG_POINT)))
		{
			ERR_D3DFUNC25;
			return;
		}
		// set minification
		if(FAILED(g_lpD3DDevice3D->SetTextureStageState(0,
													    D3DTSS_MINFILTER,
													    D3DTFG_POINT)))
		{
			ERR_D3DFUNC25;
			return;
		}
	}
	else
	{
		// set magnification
		if(FAILED(g_lpD3DDevice3D->SetTextureStageState(0,
													    D3DTSS_MAGFILTER,
													    D3DTFG_LINEAR)))
		{
			ERR_D3DFUNC25;
			return;
		}
		// set minification
		if(FAILED(g_lpD3DDevice3D->SetTextureStageState(0,
													    D3DTSS_MINFILTER,
													    D3DTFG_LINEAR)))
		{
			ERR_D3DFUNC25;
			return;
		}
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  SetAmbientLight()						//
//		(defined in "d3dfunc.h")						//
//														//
//	Set intensity of ambient light levels.				//
//	  Parameters:										//
//		BYTE,BYTE,BYTE: RGB intensities of light		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetAmbientLight(D3DCOLOR dcColor)
{
	// set light
#ifdef DEBUG_MODE
	if(FAILED(g_lpD3DDevice3D->SetLightState(D3DLIGHTSTATE_AMBIENT,
										     dcColor)))
		ERR_D3DFUNC21;
#else
	g_lpD3DDevice3D->SetLightState(D3DLIGHTSTATE_AMBIENT,
								   dcColor);
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  SetZBias()								//
//		(defined in "d3dfunc.h")						//
//														//
//	Set z-bias.											//
//	  Parameters:										//
//		DWORD: z-bias (0-16)							//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetZBias(DWORD dwBias)
{
	// set bias
#ifdef DEBUG_MODE
	if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ZBIAS,
											  dwBias)))
	{
		ERR_D3DFUNC22;
		return;
	}
#else
	g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ZBIAS,
								    dwBias);
#endif
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetTiledTextureMode()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Set tiled texture mode into stage.					//
//	  Parameters:										//
//		DWORD: stage to set								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetTiledTextureMode(DWORD dwStage)
{
	// set mode
	if(FAILED(g_lpD3DDevice3D->SetTextureStageState(dwStage,
												    D3DTSS_ADDRESS,
												    D3DTADDRESS_WRAP)))
	{
		ERR_D3DFUNC23;
		return;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetMirroredTextureMode()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Set mirrored texture mode into stage.				//
//	  Parameters:										//
//		DWORD: stage to set								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetMirroredTextureMode(DWORD dwStage)
{
	// set mode
	if(FAILED(g_lpD3DDevice3D->SetTextureStageState(dwStage,
 												    D3DTSS_ADDRESS,
												    D3DTADDRESS_MIRROR)))
	{
		ERR_D3DFUNC23;
		return;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetClampedTextureMode()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Set clamped texture mode into stage.				//
//	  Parameters:										//
//		DWORD: stage to set								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetClampedTextureMode(DWORD dwStage)
{
	// set mode
	if(FAILED(g_lpD3DDevice3D->SetTextureStageState(dwStage,
												    D3DTSS_ADDRESS,
												    D3DTADDRESS_CLAMP)))
	{
		ERR_D3DFUNC23;
		return;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetBorderedTextureMode()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Set bordered mode into stage.						//
//	  Parameters:										//
//		DWORD: stage to set								//
//		BYTE,BYTE,BYTE: RGB color of border.			//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetBorderedTextureMode(DWORD dwStage,
								   BYTE cRed,
								   BYTE cGreen,
								   BYTE cBlue)
{
	// set mode
	if(FAILED(g_lpD3DDevice3D->SetTextureStageState(dwStage,
												    D3DTSS_ADDRESS,
												    D3DTADDRESS_BORDER)))
	{
		ERR_D3DFUNC23;
		return;
	}
	// set color
	if(FAILED(g_lpD3DDevice3D->SetTextureStageState(dwStage,
												    D3DTSS_BORDERCOLOR,
												    RGBA_MAKE(cRed,cGreen,cBlue,0xFF))))
	{
		ERR_D3DFUNC24;
		return;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  ClearTextureBuffers()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Evict managed textures.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void ClearTextureBuffers(void)
{
	// clear textures
	if(FAILED(GetGlobalDirect3DPtr()->EvictManagedTextures()))
	{
		ERR_D3DFUNC26;
		return;
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  MatrixMultiply()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Multiply 2 matrices together.						//
//	  Parameters:										//
//		D3DMATRIX*: final matrix						//
//		D3DMATRIX*: matrix "a"							//
//		D3DMATRIX*: matrix "b"							//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void MatrixMultiply(D3DMATRIX* lpmtxQ,
						   D3DMATRIX* lpmtxA,
						   D3DMATRIX* lpmtxB)
{
    // pointers to data (speed optimization)
	FLOAT* pA=(FLOAT*)lpmtxA;
    FLOAT* pB=(FLOAT*)lpmtxB;
	FLOAT* pQ=(FLOAT*)lpmtxQ;

    // clear final matrix
	ZeroMemory(pQ,sizeof(D3DMATRIX));
	// multiply matrices
	for(WORD i=0;i<4;i++) 
        for(WORD j=0;j<4;j++) 
            for(WORD k=0;k<4;k++ ) 
				pQ[4*i+j]+=pA[4*k+j]*pB[4*i+k];
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  MatrixInvert()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Invert a matrix.									//
//	  Parameters:										//
//		D3DMATRIX*: final matrix						//
//		D3DMATRIX*: matrix								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void MatrixInvert(D3DMATRIX* lpmtxQ,
						 D3DMATRIX* lpmtxA)
{
    // behold, the magic number!
	FLOAT fDetInv;
	
	// check for impossibilities
	if(fabs(lpmtxA->_44-1.0f)>.001f)
	{
		ERR_D3DFUNC14;
		return;
	}
    if(fabs(lpmtxA->_14)>.001f||fabs(lpmtxA->_24)>.001f||fabs(lpmtxA->_34)>.001f)
	{
		ERR_D3DFUNC14;
		return;
	}
	// calculate magic number
    fDetInv=1.0f/(lpmtxA->_11*(lpmtxA->_22*lpmtxA->_33-lpmtxA->_23*lpmtxA->_32)-
                  lpmtxA->_12*(lpmtxA->_21*lpmtxA->_33-lpmtxA->_23*lpmtxA->_31)+
                  lpmtxA->_13*(lpmtxA->_21*lpmtxA->_32-lpmtxA->_22*lpmtxA->_31));
    // perform inversion
	lpmtxQ->_11=fDetInv*(lpmtxA->_22*lpmtxA->_33-lpmtxA->_23*lpmtxA->_32);
    lpmtxQ->_12=-fDetInv*(lpmtxA->_12*lpmtxA->_33-lpmtxA->_13*lpmtxA->_32);
    lpmtxQ->_13=fDetInv*(lpmtxA->_12*lpmtxA->_23-lpmtxA->_13*lpmtxA->_22);
    lpmtxQ->_14=0.0f;
    lpmtxQ->_21=-fDetInv*(lpmtxA->_21*lpmtxA->_33-lpmtxA->_23*lpmtxA->_31);
    lpmtxQ->_22=fDetInv*(lpmtxA->_11*lpmtxA->_33-lpmtxA->_13*lpmtxA->_31);
    lpmtxQ->_23=-fDetInv*(lpmtxA->_11*lpmtxA->_23-lpmtxA->_13*lpmtxA->_21);
    lpmtxQ->_24=0.0f;
    lpmtxQ->_31=fDetInv*(lpmtxA->_21*lpmtxA->_32-lpmtxA->_22*lpmtxA->_31);
    lpmtxQ->_32=-fDetInv*(lpmtxA->_11*lpmtxA->_32-lpmtxA->_12*lpmtxA->_31);
    lpmtxQ->_33=fDetInv*(lpmtxA->_11*lpmtxA->_22-lpmtxA->_12*lpmtxA->_21);
    lpmtxQ->_34=0.0f;
    lpmtxQ->_41=-(lpmtxA->_41*lpmtxQ->_11+lpmtxA->_42*lpmtxQ->_21+lpmtxA->_43*lpmtxQ->_31);
    lpmtxQ->_42=-(lpmtxA->_41*lpmtxQ->_12+lpmtxA->_42*lpmtxQ->_22+lpmtxA->_43*lpmtxQ->_32);
    lpmtxQ->_43=-(lpmtxA->_41*lpmtxQ->_13+lpmtxA->_42*lpmtxQ->_23+lpmtxA->_43*lpmtxQ->_33);
    lpmtxQ->_44=1.0f;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  TransformVector()						//
//		(defined in "d3dfunc.h")						//
//														//
//	Transform a vector based on a matrix.				//
//	  Parameters:										//
//		D3DVECTOR*: vector (will be changed!)			//
//		D3DMATRIX*: matrix								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void TransformVector(D3DVECTOR* lpv,
							D3DMATRIX* lpm)
{
	// store original vector
	float fx,fy,fz;

	// store vector
	fx=lpv->dvX;
	fy=lpv->dvY;
	fz=lpv->dvZ;
	// transform it
	lpv->dvX=(fx*lpm->_11)+(fy*lpm->_21)+(fz*lpm->_31)+(lpm->_41);
	lpv->dvY=(fx*lpm->_12)+(fy*lpm->_22)+(fz*lpm->_32)+(lpm->_42);
	lpv->dvZ=(fx*lpm->_13)+(fy*lpm->_23)+(fz*lpm->_33)+(lpm->_43);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CurrentMaterialOff()						//
//		(defined in "d3dfunc.h")						//
//														//
//	Set current material to NULL.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void CurrentMaterialOff(void)
{
	// set to NULL
#ifdef DEBUG_MODE
	if(FAILED(g_lpD3DDevice3D->SetLightState(D3DLIGHTSTATE_MATERIAL,
										     NULL)))
	{
		ERR_D3DFUNC27;
		return;
	}
#else
	g_lpD3DDevice3D->SetLightState(D3DLIGHTSTATE_MATERIAL,
								   NULL);
#endif
	// reset global model number
	g_dwLastModel=0xFFFFFFFF;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CurrentTextureOff()						//
//		(defined in "d3dfunc.h")						//
//														//
//	Set current texture to NULL.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void CurrentTextureOff(void)
{
	// set stage 0 to NULL
#ifdef DEBUG_MODE
	if(FAILED(g_lpD3DDevice3D->SetTexture(0,
										  NULL)))
	{
		ERR_D3DFUNC28;
		return;
	}
#else
	g_lpD3DDevice3D->SetTexture(0,
							    NULL);
#endif
	// reset global model number
	g_dwLastModel=0xFFFFFFFF;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EnableAlpha()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Turn on transparency if req'd.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EnableAlpha(void)
{
	// set alpha if authorized
	if(g_bAlphaEnabled)
	{
		// enable blending
#ifdef DEBUG_MODE
		if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,
												  TRUE)))
			ERR_D3DFUNC19;
#else
		g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,
									    TRUE);
#endif
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  DisableAlpha()							//
//		(defined in "d3dfunc.h")						//
//														//
//	Turn off transparency.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void DisableAlpha(void)
{
	// disable blending
#ifdef DEBUG_MODE
	if(FAILED(g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,
											  FALSE)))
		ERR_D3DFUNC19;
#else
	g_lpD3DDevice3D->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,
								    FALSE);
#endif
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalD3DLinearFlag()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Retrieve flag for linear texture mapping.			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: flag										//
//														//
//////////////////////////////////////////////////////////

extern BOOL GetGlobalD3DLinearFlag(void)
{
	return(g_bLinearEnabled);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalD3DLinearFlag()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Set linear texturing flag.							//
//	  Parameters:										//
//		BOOL: flag										//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetGlobalD3DLinearFlag(BOOL bFlag)
{
	g_bLinearEnabled=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalBillboardRightVector()			//
//		(defined in "d3dfunc.h")						//
//														//
//	Get right vector for billboarding.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPD3DVECTOR: pointer to vector					//
//														//
//////////////////////////////////////////////////////////

extern LPD3DVECTOR GetGlobalBillboardRightVector(void)
{
	return(&g_vBBRight);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalBillboardUpVector()				//
//		(defined in "d3dfunc.h")						//
//														//
//	Get up vector for billboarding.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPD3DVECTOR: pointer to vector					//
//														//
//////////////////////////////////////////////////////////

extern LPD3DVECTOR GetGlobalBillboardUpVector(void)
{
	return(&g_vBBUp);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalBillboardAheadVector()			//
//		(defined in "d3dfunc.h")						//
//														//
//	Get ahead vector for billboarding.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPD3DVECTOR: pointer to vector					//
//														//
//////////////////////////////////////////////////////////

extern LPD3DVECTOR GetGlobalBillboardAheadVector(void)
{
	return(&g_vBBAhead);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalBillboardVectors()				//
//		(defined in "d3dfunc.h")						//
//														//
//	Set vectors for billboarding.						//
//	  Parameters:										//
//		LPD3DVECTOR: pointer to view vector				//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern inline void SetGlobalBillboardVectors(LPD3DVECTOR lpvView)
{
	// flatten view vector
	g_vBBAhead.dvX=lpvView->dvX;
	g_vBBAhead.dvY=0;
	g_vBBAhead.dvZ=lpvView->dvZ;
	// normalize view vector
	g_vBBAhead=Normalize(g_vBBAhead);
	// up vector never changes, so compute right vector
	g_vBBRight=CrossProduct(g_vBBUp,g_vBBAhead);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalModelNumber()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Get current model number.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		DWORD: next number in series					//
//														//
//////////////////////////////////////////////////////////

extern DWORD GetGlobalModelNumber(void)
{
	// increment counter
	g_dwModelNum++;
	// return value
	return(g_dwModelNum);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalLastModelPtr()					//
//		(defined in "d3dfunc.h")						//
//														//
//	Get pointer to last model.							//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDWORD: pointer								//
//														//
//////////////////////////////////////////////////////////

extern LPDWORD GetGlobalLastModelPtr(void)
{
	return(&g_dwLastModel);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalLastModel()						//
//		(defined in "d3dfunc.h")						//
//														//
//	Set last model number.								//
//	  Parameters:										//
//		DWORD: value to set								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetGlobalLastModel(DWORD dwVal)
{
	g_dwLastModel=dwVal;
}

// END ///////////////////////////////////////////////////

