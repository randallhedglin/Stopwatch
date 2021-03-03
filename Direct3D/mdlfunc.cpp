// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  mdlfunc.cpp									//
//														//
//	Model rendering functions.							//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// allow access to window handles
#include"winmain.h"
// allow access to file handling functions
#include"filefunc.h"
// allow access to DirectDraw
#include"ddfunc.h"
// allow access to bitmaps
#include"bmpfunc.h"
// allow access to materials
#include"matfunc.h"
// allow access to textures
#include"txfunc.h"
// header for this module
#include"mdlfunc.h"
// allow access to Direct3D
#include"d3dfunc.h"

// ERROR MESSAGES ////////////////////////////////////////

// invalid data
#define ERR_MDLFUNC00 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MDLFUNC.CPP\nError Code: 00\nInvalid data in data file.") }
// failed to create vertex buffer
#define ERR_MDLFUNC01 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MDLFUNC.CPP\nError Code: 01\nUnable to create vertex buffer.") }
// failed to lock vertex buffer
#define ERR_MDLFUNC02 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MDLFUNC.CPP\nError Code: 02\nUnable to lock vertex buffer.") }
// failed to unlock vertex buffer
#define ERR_MDLFUNC03 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MDLFUNC.CPP\nError Code: 03\nUnable to unlock vertex buffer.") }
// failed to optimize vertex buffer
#define ERR_MDLFUNC04 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MDLFUNC.CPP\nError Code: 04\nUnable to optimize vertex buffer.") }
// failed to render primitive
#define ERR_MDLFUNC05 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MDLFUNC.CPP\nError Code: 05\nUnable to render primitive.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::CD3DModel()					//
//		(defined in "mdlfunc.h")						//
//														//
//	Class constructor for models.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

CD3DModel::CD3DModel()
{
	// reset values
	lplpMat=NULL;
	dwNumMat=0;
	lplpTex=NULL;
	dwNumTex=0;
	lpVtx=NULL;
	dwNumVtx=0;
	lpIns=NULL;
	dwNumIns=0;
	INIT_VAR(vPos);
	INIT_VAR(vRight);
	INIT_VAR(vUp);
	INIT_VAR(vAhead);
	bBillboard=FALSE;
	lpvBBRight=NULL;
	lpvBBUp=NULL;
	lpvBBAhead=NULL;
	lpPRIns=NULL;
	dwNumPRIns=0;
	lpdwLastModel=NULL;
	dwModelNumber=0;
	lpD3DDevice=NULL;
	dvHDRad=0;
	dvHDRadSq=0;
	dvHDLevelU=0;
	dvHDLevelL=0;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::Load()						//
//		(defined in "mdlfunc.h")						//
//														//
//	Loads model from open data file.					//
//	  Parameters:										//
//		D3DVECTOR* pointer to offset vector				//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CD3DModel::Load(D3DVECTOR* lpvOffset)
{
	// data type
	BYTE cType=NULL;
	// counter
	DWORD dwCount=0;
	// DWORD color values (RGBA)
	DWORD dwR=0,
		  dwG=0,
		  dwB=0,
		  dwA=0;
	// DWORD specular power
	DWORD dwPwr=0;
	// color values (RGBA)
	D3DVALUE dvR=0,
			 dvG=0,
			 dvB=0,
			 dvA=0;
	// specular power
	D3DVALUE dvPwr=0;
	// vertex buffer description
	D3DVERTEXBUFFERDESC vbd;
	INIT_VAR(vbd);
	// pointer to vertices
	D3DVERTEX* lpVtxData=NULL;
	// long xyzuv values
	long lX=0,
		 lY=0,
		 lZ=0,
		 lTU=0,
		 lTV=0;
	// xyzuv values
	D3DVALUE dvX=0,
			 dvY=0,
			 dvZ=0,
			 dvTU=0,
			 dvTV=0;
	// optimize flag
	BOOL bOptimize=FALSE;

	// copy Direct3D device
	lpD3DDevice=GetGlobalDirect3DDevicePtr();
	// read data type
	ReadData(&cType,sizeof(BYTE));
	// check data type
	if(cType!=DTF_IDMODEL)
	{
		ERR_MDLFUNC00;
		return(FALSE);
	}
	// read billboard flag
	ReadData((LPBYTE)&bBillboard,
			 sizeof(BOOL));
	// read number of materials
	ReadData((LPBYTE)&dwNumMat,
			 sizeof(DWORD));
	// read materials if necessary
	if(dwNumMat)
	{
		// allocate memory
		SAFE_ALLOC(lplpMat,
				   CD3DMaterial**,
				   sizeof(CD3DMaterial*)*dwNumMat);
		// check for error
		if(!lplpMat)
			return(FALSE);
		// read materials
		for(dwCount=0;dwCount<dwNumMat;dwCount++)
		{
			// new material
			SAFE_NEW(lplpMat[dwCount],CD3DMaterial);
			// create material
			if(!lplpMat[dwCount]->Create())
				return(FALSE);
			// read values
			ReadData((LPBYTE)&dwR,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwG,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwB,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwA,
					 sizeof(DWORD));
			// convert values
			dvR=FIXED_TO_FLOAT(dwR);
			dvG=FIXED_TO_FLOAT(dwG);
			dvB=FIXED_TO_FLOAT(dwB);
			dvA=FIXED_TO_FLOAT(dwA);
			// set diffuse
			lplpMat[dwCount]->Diffuse(dvR,dvG,dvB,dvA);
			// read values
			ReadData((LPBYTE)&dwR,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwG,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwB,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwA,
					 sizeof(DWORD));
			// convert values
			dvR=FIXED_TO_FLOAT(dwR);
			dvG=FIXED_TO_FLOAT(dwG);
			dvB=FIXED_TO_FLOAT(dwB);
			dvA=FIXED_TO_FLOAT(dwA);
			// set ambient
			lplpMat[dwCount]->Ambient(dvR,dvG,dvB,dvA);
			// read values
			ReadData((LPBYTE)&dwR,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwG,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwB,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwA,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwPwr,
					 sizeof(DWORD));
			// convert values
			dvR=FIXED_TO_FLOAT(dwR);
			dvG=FIXED_TO_FLOAT(dwG);
			dvB=FIXED_TO_FLOAT(dwB);
			dvA=FIXED_TO_FLOAT(dwA);
			dvPwr=FIXED_TO_FLOAT(dwPwr);
			// set specular
			lplpMat[dwCount]->Specular(dvR,dvG,dvB,dvA,dvPwr);
			// read values
			ReadData((LPBYTE)&dwR,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwG,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwB,
					 sizeof(DWORD));
			ReadData((LPBYTE)&dwA,
					 sizeof(DWORD));
			// convert values
			dvR=FIXED_TO_FLOAT(dwR);
			dvG=FIXED_TO_FLOAT(dwG);
			dvB=FIXED_TO_FLOAT(dwB);
			dvA=FIXED_TO_FLOAT(dwA);
			// set emissive
			lplpMat[dwCount]->Emissive(dvR,dvG,dvB,dvA);
			// update material
			lplpMat[dwCount]->Update();
		}
	}
	// read number of textures
	ReadData((LPBYTE)&dwNumTex,
			 sizeof(DWORD));
	// read textures if necessary
	if(dwNumTex)
	{
		// allocate memory
		SAFE_ALLOC(lplpTex,
				   CD3DTexture**,	   
				   sizeof(CD3DTexture*)*dwNumTex);
		// check for error
		if(!lplpTex)
			return(FALSE);
		// load textures
		for(dwCount=0;dwCount<dwNumTex;dwCount++)
		{
			// new texture
			SAFE_NEW(lplpTex[dwCount],CD3DTexture);
			// load texture
			lplpTex[dwCount]->Load();
		}
	}
	// read number of vertices
	ReadData((LPBYTE)&dwNumVtx,
			 sizeof(DWORD));
	// prepare buffer description
	INIT_STRUCT(vbd);
	// set caps
	vbd.dwCaps=NULL;
	// set format
	vbd.dwFVF=D3DFVF_VERTEX;
	// set number of vertices
	vbd.dwNumVertices=dwNumVtx;
	// set memory flag if software
	if(!GetGlobalD3DHardwareFlag())
		vbd.dwCaps|=D3DVBCAPS_SYSTEMMEMORY;
	// create vertex buffer
	if(FAILED(GetGlobalDirect3DPtr()->CreateVertexBuffer(&vbd,
														 &lpVtx,
														 NULL,
														 NULL)))
	{
		ERR_MDLFUNC01;
		return(FALSE);
	}
	// lock vertex buffer
	if(FAILED(lpVtx->Lock(DDLOCK_SURFACEMEMORYPTR|
						  DDLOCK_WAIT,
						  (LPVOID*)&lpVtxData,
						  NULL)))
	{
		ERR_MDLFUNC02;
		return(FALSE);
	}
	// read vertices
	for(dwCount=0;dwCount<dwNumVtx;dwCount++)
	{
		// read values
		ReadData((LPBYTE)&lX,
				 sizeof(long));
		ReadData((LPBYTE)&lY,
				 sizeof(long));
		ReadData((LPBYTE)&lZ,
				 sizeof(long));
		ReadData((LPBYTE)&lTU,
				 sizeof(long));
		ReadData((LPBYTE)&lTV,
				 sizeof(long));
		// convert values
		dvX=FIXED_TO_FLOAT(lX);
		dvY=FIXED_TO_FLOAT(lY);
		dvZ=FIXED_TO_FLOAT(lZ);
		dvTU=FIXED_TO_FLOAT(lTU);
		dvTV=FIXED_TO_FLOAT(lTV);
		// store values
		lpVtxData[dwCount].dvX=dvX;
		lpVtxData[dwCount].dvY=dvY;
		lpVtxData[dwCount].dvZ=dvZ;
		lpVtxData[dwCount].dvTU=dvTU;
		lpVtxData[dwCount].dvTV=dvTV;
	}
	// read number of pre-render instructions
	ReadData((LPBYTE)&dwNumPRIns,
			 sizeof(DWORD));
	// allocate memory
	SAFE_ALLOC(lpPRIns,
			   LPWORD,
			   sizeof(WORD)*dwNumPRIns);
	// check for error
	if(!lpPRIns)
		return(FALSE);
	// read data
	for(dwCount=0;dwCount<dwNumPRIns;dwCount++)
		ReadData((LPBYTE)&lpPRIns[dwCount],
				 sizeof(WORD));
	// read number of instructions
	ReadData((LPBYTE)&dwNumIns,
			 sizeof(DWORD));
	// allocate memory
	SAFE_ALLOC(lpIns,
			   LPWORD,
			   sizeof(WORD)*dwNumIns);
	// check for error
	if(!lpIns)
		return(FALSE);
	for(dwCount=0;dwCount<dwNumIns;dwCount++)
	{
		// read data
		ReadData((LPBYTE)&lpIns[dwCount],
				 sizeof(WORD));
		// check for optimize flag
		if(lpIns[dwCount]==OPCODE_OPTIMIZE)
			bOptimize=TRUE;
	}
	// compute normals
	ComputeNormals(lpVtxData);
	// compute extents
	ComputeExtents(lpVtxData);
	// unlock vertex buffer
	if(FAILED(lpVtx->Unlock()))
	{
		ERR_MDLFUNC03;
		return(FALSE);
	}
	// optimize buffer if necessary
	if(bOptimize)
		if(FAILED(lpVtx->Optimize(lpD3DDevice,
								  NULL)))
			ERR_MDLFUNC04;
	// create valid orientation
	vRight=D3DVECTOR(1,0,0);
	vUp=D3DVECTOR(0,1,0);
	vAhead=D3DVECTOR(0,0,1);
	// set pointers to billboard vectors
	lpvBBRight=GetGlobalBillboardRightVector();
	lpvBBUp=GetGlobalBillboardUpVector();
	lpvBBAhead=GetGlobalBillboardAheadVector();
	// get pointer to last model
	lpdwLastModel=GetGlobalLastModelPtr();
	// assign model number
	dwModelNumber=GetGlobalModelNumber();
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
//	Function:  CD3DModel::Position()					//
//		(defined in "mdlfunc.h")						//
//														//
//	Set model's position.								//
//	  Parameters:										//
//		D3DVECTOR*: pointer to new position vector		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::Position(D3DVECTOR* lpvPos)
{
	vPos=*lpvPos;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::Orientation()					//
//		(defined in "mdlfunc.h")						//
//														//
//	Set model's orientation vectors.					//
//	  Parameters:										//
//		D3DVECTOR*: pointer to right vector				//
//		D3DVECTOR*: pointer to up vector				//
//		D3DVECTOR*: pointer to ahead vector (or NULL)	//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::Orientation(D3DVECTOR* lpvRight,
							D3DVECTOR* lpvUp,
							D3DVECTOR* lpvAhead)
{
	// if this is a billboard, use billboard vectors
	if(bBillboard)
	{
		// only set if this is first occurrence
		if(*lpdwLastModel!=dwModelNumber)
		{
			// set vectors
			vRight=*lpvBBRight;
			vUp=*lpvBBUp;
			vAhead=*lpvBBAhead;
		}
	}
	else
	{
		// copy right & up vectors
		vRight=*lpvRight;
		vUp=*lpvUp;
		// compute ahead vector if necessary
		if(!lpvAhead)
			vAhead=CrossProduct(vRight,vUp);
		else
			vAhead=*lpvAhead;
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::Yaw()							//
//		(defined in "mdlfunc.h")						//
//														//
//	Increase or decrease yaw value.						//
//	  Parameters:										//
//		D3DVALUE: amount								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::Yaw(D3DVALUE vInc)
{
	// compute new ahead vector
	vAhead=Normalize(vAhead+(vRight*vInc));
	// compute new right vector
	vRight=CrossProduct(vUp,vAhead);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::Pitch()						//
//		(defined in "mdlfunc.h")						//
//														//
//	Change pitch of model.								//
//	  Parameters:										//
//		D3DVALUE: amount								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::Pitch(D3DVALUE vInc)
{
	// compute new ahead vector
	vAhead=Normalize(vAhead+(vUp*vInc));
	// compute new up vector
	vUp=CrossProduct(vAhead,vRight);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::Roll()						//
//		(defined in "mdlfunc.h")						//
//														//
//	Change roll of model.								//
//	  Parameters:										//
//		D3DVALUE: amount								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::Roll(D3DVALUE vInc)
{
	// compute new ahead vector
	vRight=Normalize(vRight-(vUp*vInc));
	// compute new up vector
	vUp=CrossProduct(vAhead,vRight);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::Right()						//
//		(defined in "mdlfunc.h")						//
//														//
//	Move model right.									//
//	  Parameters:										//
//		D3DVALUE: amount								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::Right(D3DVALUE vInc)
{
	// change position
	vPos+=vRight*vInc;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::Up()							//
//		(defined in "mdlfunc.h")						//
//														//
//	Move model up.										//
//	  Parameters:										//
//		D3DVALUE: amount								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::Up(D3DVALUE vInc)
{
	// change position
	vPos+=vUp*vInc;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::Ahead()						//
//		(defined in "mdlfunc.h")						//
//														//
//	Move model ahead.									//
//	  Parameters:										//
//		D3DVALUE: amount								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::Ahead(D3DVALUE vInc)
{
	// change position
	vPos+=vAhead*vInc;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::Render()						//
//		(defined in "mdlfunc.h")						//
//														//
//	Render model at current position and orientation.	//
//	  Parameters:										//
//		D3DVALUE: scale									//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::Render(D3DVALUE dvScale)
{
	// instruction counter
	DWORD dwInsNum=0;
	// done flag
	BOOL bDone=FALSE;
	// vertex quantity
	DWORD dwVtxQty;

	// set world matrix
	SetWorldMatrix(&vPos,&vRight,&vUp,&vAhead,dvScale);
	// read pre-render instructions if necessary
	if(*lpdwLastModel!=dwModelNumber)
	{
		while(!bDone)
			// read instruction
			switch(lpPRIns[dwInsNum++])
			{
				// end render
				case(OPCODE_ENDPRERENDER):
					bDone=TRUE;
					break;
				// set current material
				case(OPCODE_SETMATERIAL):
					// check for no material
					if(lpPRIns[dwInsNum]>dwNumMat)
						CurrentMaterialOff();
					// set material
					else
						lplpMat[lpPRIns[dwInsNum++]]->Current();
					break;
				// set current texture
				case(OPCODE_SETTEXTURE):
					// check for no texture
					if(lpPRIns[dwInsNum]>dwNumTex)
						CurrentTextureOff();
					// set texture
					else
						lplpTex[lpPRIns[dwInsNum++]]->Current();
					break;
			}
		// reset flag
		bDone=FALSE;	
		// reset instruction number
		dwInsNum=0;
		// set last model
		*lpdwLastModel=dwModelNumber;
	}
	// read instructions
	while(!bDone)
		// read instruction
		switch(lpIns[dwInsNum++])
		{
			// render triangle list
			case(OPCODE_TRIANGLELIST):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				dwVtxQty=(dwVtxQty<<1)+dwVtxQty;
				// draw triangle list
#ifdef DEBUG_MODE
				if(FAILED(lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST,
																			   lpVtx,
																			   &lpIns[dwInsNum],
																			   dwVtxQty,
																			   D3DDP_DONOTUPDATEEXTENTS)))
					ERR_MDLFUNC05;
#else
				lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST,
																	 lpVtx,
																	 &lpIns[dwInsNum],
																	 (dwVtxQty<<1)+dwVtxQty,
																	 D3DDP_DONOTUPDATEEXTENTS);
#endif
				// increment instruction counter
				dwInsNum+=dwVtxQty;
				break;
			// set current material
			case(OPCODE_SETMATERIAL):
				// check for no material
				if(lpIns[dwInsNum]>dwNumMat)
					CurrentMaterialOff();
				// set material
				else
					lplpMat[lpIns[dwInsNum++]]->Current();
				break;
			// set current texture
			case(OPCODE_SETTEXTURE):
				// check for no texture
				if(lpIns[dwInsNum]>dwNumTex)
					CurrentTextureOff();
				// set texture
				else
					lplpTex[lpIns[dwInsNum++]]->Current();
				break;
			// end render
			case(OPCODE_ENDRENDER):
				bDone=TRUE;
				break;
			// render line list
			case(OPCODE_LINELIST):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// draw line list
#ifdef DEBUG_MODE
				if(FAILED(lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_LINELIST,
																			   lpVtx,
																			   &lpIns[dwInsNum],
																			   dwVtxQty<<1,
																			   D3DDP_DONOTUPDATEEXTENTS)))
					ERR_MDLFUNC05;
#else
				lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_LINELIST,
																	 lpVtx,
																	 &lpIns[dwInsNum],
																	 dwVtxQty<<1,
																	 D3DDP_DONOTUPDATEEXTENTS);
#endif
				// increment instruction counter
				dwInsNum+=dwVtxQty<<1;
				break;
			// render triangle strip
			case(OPCODE_TRIANGLESTRIP):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// draw triangle strip
#ifdef DEBUG_MODE
				if(FAILED(lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLESTRIP,
																			   lpVtx,
																			   &lpIns[dwInsNum],
																			   dwVtxQty+2,
																			   D3DDP_DONOTUPDATEEXTENTS)))
					ERR_MDLFUNC05;
#else
				lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLESTRIP,
																	 lpVtx,
																	 &lpIns[dwInsNum],
																	 dwVtxQty+2,
																	 D3DDP_DONOTUPDATEEXTENTS);
#endif
				// increment instruction counter
				dwInsNum+=dwVtxQty+2;
				break;
			// render triangle fan
			case(OPCODE_TRIANGLEFAN):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// draw triangle fan
#ifdef DEBUG_MODE
				if(FAILED(lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLEFAN,
																			   lpVtx,
																			   &lpIns[dwInsNum],
																			   dwVtxQty+2,
																			   D3DDP_DONOTUPDATEEXTENTS)))
					ERR_MDLFUNC05;
#else
				lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLEFAN,
																	 lpVtx,
																	 &lpIns[dwInsNum],
																	 dwVtxQty+2,
																	 D3DDP_DONOTUPDATEEXTENTS);
#endif
				// increment instruction counter
				dwInsNum+=dwVtxQty+2;
				break;
			// render line strip
			case(OPCODE_LINESTRIP):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// draw line strip
#ifdef DEBUG_MODE
				if(FAILED(lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_LINESTRIP,
																			   lpVtx,
																			   &lpIns[dwInsNum],
																			   dwVtxQty+1,
																			   D3DDP_DONOTUPDATEEXTENTS)))
					ERR_MDLFUNC05;
#else
				lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_LINESTRIP,
																	 lpVtx,
																	 &lpIns[dwInsNum],
																	 dwVtxQty+1,
																	 D3DDP_DONOTUPDATEEXTENTS);
#endif
				// increment instruction counter
				dwInsNum+=dwVtxQty+1;
				break;
			// render point list			
			case(OPCODE_POINTLIST):
				// draw point list
#ifdef DEBUG_MODE
				if(FAILED(lpD3DDevice->DrawPrimitiveVB(D3DPT_POINTLIST,
																		lpVtx,
																		lpIns[dwInsNum++],
																		lpIns[dwInsNum++],
																		D3DDP_DONOTUPDATEEXTENTS)))
					ERR_MDLFUNC05;
#else
				lpD3DDevice->DrawPrimitiveVB(D3DPT_POINTLIST,
															  lpVtx,
															  lpIns[dwInsNum++],
															  lpIns[dwInsNum++],
															  D3DDP_DONOTUPDATEEXTENTS);
#endif
				break;
		}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::RenderWireframe()				//
//		(defined in "mdlfunc.h")						//
//														//
//	Render wire frame using currently selected material.//
//	  Parameters:										//
//		D3DVALUE: scale									//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::RenderWireframe(D3DVALUE dvScale)
{
	// instruction counter
	DWORD dwInsNum=0;
	// done flag
	BOOL bDone=FALSE;
	// vertex quantity
	DWORD dwVtxQty;

	// set world matrix
	SetWorldMatrix(&vPos,&vRight,&vUp,&vAhead,dvScale);
	// no texture
	CurrentTextureOff();
	// reset current model
	*lpdwLastModel=0xFFFFFFFF;
	// set to wireframe mode
	lpD3DDevice->SetRenderState(D3DRENDERSTATE_FILLMODE,
								D3DFILL_WIREFRAME);
	// read instructions
	while(!bDone)
		// read instruction
		switch(lpIns[dwInsNum++])
		{
			// render triangle list
			case(OPCODE_TRIANGLELIST):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				dwVtxQty=(dwVtxQty<<1)+dwVtxQty;
				// draw triangle list
#ifdef DEBUG_MODE
				if(FAILED(lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST,
																			   lpVtx,
																			   &lpIns[dwInsNum],
																			   dwVtxQty,
																			   D3DDP_DONOTUPDATEEXTENTS)))
					ERR_MDLFUNC05;
#else
				lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST,
																	 lpVtx,
																	 &lpIns[dwInsNum],
																	 (dwVtxQty<<1)+dwVtxQty,
																	 D3DDP_DONOTUPDATEEXTENTS);
#endif
				// increment instruction counter
				dwInsNum+=dwVtxQty;
				break;
			// set current material
			case(OPCODE_SETMATERIAL):
				// check for no material
				if(lpIns[dwInsNum]>dwNumMat)
					CurrentMaterialOff();
				// set material
				else
					lplpMat[lpIns[dwInsNum++]]->Current();
				break;
			// set current texture
			case(OPCODE_SETTEXTURE):
				// check for no texture
				if(lpIns[dwInsNum]>dwNumTex)
					CurrentTextureOff();
				// set texture
				else
					lplpTex[lpIns[dwInsNum++]]->Current();
				break;
			// end render
			case(OPCODE_ENDRENDER):
				bDone=TRUE;
				break;
			// render line list
			case(OPCODE_LINELIST):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// draw line list
#ifdef DEBUG_MODE
				if(FAILED(lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_LINELIST,
																			   lpVtx,
																			   &lpIns[dwInsNum],
																			   dwVtxQty<<1,
																			   D3DDP_DONOTUPDATEEXTENTS)))
					ERR_MDLFUNC05;
#else
				lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_LINELIST,
																	 lpVtx,
																	 &lpIns[dwInsNum],
																	 dwVtxQty<<1,
																	 D3DDP_DONOTUPDATEEXTENTS);
#endif
				// increment instruction counter
				dwInsNum+=dwVtxQty<<1;
				break;
			// render triangle strip
			case(OPCODE_TRIANGLESTRIP):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// draw triangle strip
#ifdef DEBUG_MODE
				if(FAILED(lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLESTRIP,
																			   lpVtx,
																			   &lpIns[dwInsNum],
																			   dwVtxQty+2,
																			   D3DDP_DONOTUPDATEEXTENTS)))
					ERR_MDLFUNC05;
#else
				lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLESTRIP,
																	 lpVtx,
																	 &lpIns[dwInsNum],
																	 dwVtxQty+2,
																	 D3DDP_DONOTUPDATEEXTENTS);
#endif
				// increment instruction counter
				dwInsNum+=dwVtxQty+2;
				break;
			// render triangle fan
			case(OPCODE_TRIANGLEFAN):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// draw triangle fan
#ifdef DEBUG_MODE
				if(FAILED(lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLEFAN,
																			   lpVtx,
																			   &lpIns[dwInsNum],
																			   dwVtxQty+2,
																			   D3DDP_DONOTUPDATEEXTENTS)))
					ERR_MDLFUNC05;
#else
				lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLEFAN,
																	 lpVtx,
																	 &lpIns[dwInsNum],
																	 dwVtxQty+2,
																	 D3DDP_DONOTUPDATEEXTENTS);
#endif
				// increment instruction counter
				dwInsNum+=dwVtxQty+2;
				break;
			// render line strip
			case(OPCODE_LINESTRIP):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// draw line strip
#ifdef DEBUG_MODE
				if(FAILED(lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_LINESTRIP,
																			   lpVtx,
																			   &lpIns[dwInsNum],
																			   dwVtxQty+1,
																			   D3DDP_DONOTUPDATEEXTENTS)))
					ERR_MDLFUNC05;
#else
				lpD3DDevice->DrawIndexedPrimitiveVB(D3DPT_LINESTRIP,
																	 lpVtx,
																	 &lpIns[dwInsNum],
																	 dwVtxQty+1,
																	 D3DDP_DONOTUPDATEEXTENTS);
#endif
				// increment instruction counter
				dwInsNum+=dwVtxQty+1;
				break;
			// render point list			
			case(OPCODE_POINTLIST):
				// draw point list
#ifdef DEBUG_MODE
				if(FAILED(lpD3DDevice->DrawPrimitiveVB(D3DPT_POINTLIST,
																		lpVtx,
																		lpIns[dwInsNum++],
																		lpIns[dwInsNum++],
																		D3DDP_DONOTUPDATEEXTENTS)))
					ERR_MDLFUNC05;
#else
				lpD3DDevice->DrawPrimitiveVB(D3DPT_POINTLIST,
															  lpVtx,
															  lpIns[dwInsNum++],
															  lpIns[dwInsNum++],
															  D3DDP_DONOTUPDATEEXTENTS);
#endif
				break;
		}
	// set to solid mode
	lpD3DDevice->SetRenderState(D3DRENDERSTATE_FILLMODE,
								D3DFILL_SOLID);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::ComputeNormals()				//
//		(defined in "mdlfunc.h")						//
//														//
//	Computes normals of model.							//
//	  Parameters:										//
//		D3DVERTEX*: pointer to vertex data				//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::ComputeNormals(D3DVERTEX* lpVtxData)
{
	// instruction counter
	DWORD dwInsNum=0;
	// done flag
	BOOL bDone=FALSE;
	// vertex quantity
	DWORD dwVtxQty;
	// vertex indices
	DWORD dwVtx1,dwVtx2,dwVtx3;
	// vertices
	D3DVECTOR vVtx1,vVtx2,vVtx3;
	// vectors
	D3DVECTOR vVec1,vVec2,vVec3;
	// normal vectors
	D3DVECTOR vNVec1,vNVec2,vNVec3;
	// current vectors
	D3DVECTOR vCur1,vCur2,vCur3;
	// counter
	DWORD dwCount;

	// reset normals
	for(dwCount=0;dwCount<dwNumVtx;dwCount++)
	{
		lpVtxData[dwCount].dvNX=0;
		lpVtxData[dwCount].dvNY=0;
		lpVtxData[dwCount].dvNZ=0;
	}
	// read instructions
	while(!bDone)
		// read instruction
		switch(lpIns[dwInsNum++])
		{
			// render triangle list
			case(OPCODE_TRIANGLELIST):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// average normals for each triangle
				for(dwCount=0;dwCount<dwVtxQty;dwCount++)
				{
					// read vertices
					dwVtx1=lpIns[dwInsNum++];
					dwVtx2=lpIns[dwInsNum++];
					dwVtx3=lpIns[dwInsNum++];
					// get points
					vVtx1.dvX=lpVtxData[dwVtx1].dvX;
					vVtx1.dvY=lpVtxData[dwVtx1].dvY;
					vVtx1.dvZ=lpVtxData[dwVtx1].dvZ;
					vVtx2.dvX=lpVtxData[dwVtx2].dvX;
					vVtx2.dvY=lpVtxData[dwVtx2].dvY;
					vVtx2.dvZ=lpVtxData[dwVtx2].dvZ;
					vVtx3.dvX=lpVtxData[dwVtx3].dvX;
					vVtx3.dvY=lpVtxData[dwVtx3].dvY;
					vVtx3.dvZ=lpVtxData[dwVtx3].dvZ;
					// compute vectors
					vVec1=vVtx3-vVtx2;
					vVec2=vVtx1-vVtx2;
					// normalize vectors
					vNVec1=Normalize(vVec1);
					vNVec2=Normalize(vVec2);
					// compute polygon normal
					vVec3.dvX=(vNVec1.dvY*vNVec2.dvZ)-(vNVec1.dvZ*vNVec2.dvY);
					vVec3.dvY=(vNVec1.dvZ*vNVec2.dvX)-(vNVec1.dvX*vNVec2.dvZ);
					vVec3.dvZ=(vNVec1.dvX*vNVec2.dvY)-(vNVec1.dvY*vNVec2.dvX);
					// compute normal
					vNVec3=Normalize(vVec3);
					// get current vectors
					vCur1.dvX=lpVtxData[dwVtx1].dvNX;
					vCur1.dvY=lpVtxData[dwVtx1].dvNY;
					vCur1.dvZ=lpVtxData[dwVtx1].dvNZ;
					vCur2.dvX=lpVtxData[dwVtx2].dvNX;
					vCur2.dvY=lpVtxData[dwVtx2].dvNY;
					vCur2.dvZ=lpVtxData[dwVtx2].dvNZ;
					vCur3.dvX=lpVtxData[dwVtx3].dvNX;
					vCur3.dvY=lpVtxData[dwVtx3].dvNY;
					vCur3.dvZ=lpVtxData[dwVtx3].dvNZ;
					// add vectors
					vCur1+=vNVec3;
					vCur2+=vNVec3;
					vCur3+=vNVec3;
					// normalize vectors
					vNVec1=Normalize(vCur1);
					vNVec2=Normalize(vCur2);
					vNVec3=Normalize(vCur3);
					// store new vectors
					lpVtxData[dwVtx1].dvNX=vNVec1.dvX;
					lpVtxData[dwVtx1].dvNY=vNVec1.dvY;
					lpVtxData[dwVtx1].dvNZ=vNVec1.dvZ;
					lpVtxData[dwVtx2].dvNX=vNVec2.dvX;
					lpVtxData[dwVtx2].dvNY=vNVec2.dvY;
					lpVtxData[dwVtx2].dvNZ=vNVec2.dvZ;
					lpVtxData[dwVtx3].dvNX=vNVec3.dvX;
					lpVtxData[dwVtx3].dvNY=vNVec3.dvY;
					lpVtxData[dwVtx3].dvNZ=vNVec3.dvZ;
				}
				break;
			// render triangle strip
			case(OPCODE_TRIANGLESTRIP):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// read first two vertices
				dwVtx1=lpIns[dwInsNum++];
				dwVtx2=lpIns[dwInsNum++];
				// average normals for each triangle
				for(dwCount=0;dwCount<dwVtxQty;dwCount++)
				{
					// read next vertex
					dwVtx3=lpIns[dwInsNum++];
					// get points
					vVtx1.dvX=lpVtxData[dwVtx1].dvX;
					vVtx1.dvY=lpVtxData[dwVtx1].dvY;
					vVtx1.dvZ=lpVtxData[dwVtx1].dvZ;
					vVtx2.dvX=lpVtxData[dwVtx2].dvX;
					vVtx2.dvY=lpVtxData[dwVtx2].dvY;
					vVtx2.dvZ=lpVtxData[dwVtx2].dvZ;
					vVtx3.dvX=lpVtxData[dwVtx3].dvX;
					vVtx3.dvY=lpVtxData[dwVtx3].dvY;
					vVtx3.dvZ=lpVtxData[dwVtx3].dvZ;
					// compute vectors
					if(dwCount&1)
					{
						// clockwise
						vVec2=vVtx3-vVtx2;
						vVec1=vVtx1-vVtx2;
					}
					else
					{
						// counter-clockwise
						vVec1=vVtx3-vVtx2;
						vVec2=vVtx1-vVtx2;
					}
					// normalize vectors
					vNVec1=Normalize(vVec1);
					vNVec2=Normalize(vVec2);
					// compute polygon normal
					vVec3.dvX=(vNVec1.dvY*vNVec2.dvZ)-(vNVec1.dvZ*vNVec2.dvY);
					vVec3.dvY=(vNVec1.dvZ*vNVec2.dvX)-(vNVec1.dvX*vNVec2.dvZ);
					vVec3.dvZ=(vNVec1.dvX*vNVec2.dvY)-(vNVec1.dvY*vNVec2.dvX);
					// compute normal
					vNVec3=Normalize(vVec3);
					// get current vectors
					vCur1.dvX=lpVtxData[dwVtx1].dvNX;
					vCur1.dvY=lpVtxData[dwVtx1].dvNY;
					vCur1.dvZ=lpVtxData[dwVtx1].dvNZ;
					vCur2.dvX=lpVtxData[dwVtx2].dvNX;
					vCur2.dvY=lpVtxData[dwVtx2].dvNY;
					vCur2.dvZ=lpVtxData[dwVtx2].dvNZ;
					vCur3.dvX=lpVtxData[dwVtx3].dvNX;
					vCur3.dvY=lpVtxData[dwVtx3].dvNY;
					vCur3.dvZ=lpVtxData[dwVtx3].dvNZ;
					// add vectors
					vCur1+=vNVec3;
					vCur2+=vNVec3;
					vCur3+=vNVec3;
					// normalize vectors
					vNVec1=Normalize(vCur1);
					vNVec2=Normalize(vCur2);
					vNVec3=Normalize(vCur3);
					// store new vectors
					lpVtxData[dwVtx1].dvNX=vNVec1.dvX;
					lpVtxData[dwVtx1].dvNY=vNVec1.dvY;
					lpVtxData[dwVtx1].dvNZ=vNVec1.dvZ;
					lpVtxData[dwVtx2].dvNX=vNVec2.dvX;
					lpVtxData[dwVtx2].dvNY=vNVec2.dvY;
					lpVtxData[dwVtx2].dvNZ=vNVec2.dvZ;
					lpVtxData[dwVtx3].dvNX=vNVec3.dvX;
					lpVtxData[dwVtx3].dvNY=vNVec3.dvY;
					lpVtxData[dwVtx3].dvNZ=vNVec3.dvZ;
					// move vertex 2 to 1 and 3 to 2
					dwVtx1=dwVtx2;
					dwVtx2=dwVtx3;
				}
				break;
			// render triangle fan
			case(OPCODE_TRIANGLEFAN):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// read first two vertices
				dwVtx1=lpIns[dwInsNum++];
				dwVtx2=lpIns[dwInsNum++];
				// average normals for each triangle
				for(dwCount=0;dwCount<dwVtxQty;dwCount++)
				{
					// read next vertex
					dwVtx3=lpIns[dwInsNum++];
					// get points
					vVtx1.dvX=lpVtxData[dwVtx1].dvX;
					vVtx1.dvY=lpVtxData[dwVtx1].dvY;
					vVtx1.dvZ=lpVtxData[dwVtx1].dvZ;
					vVtx2.dvX=lpVtxData[dwVtx2].dvX;
					vVtx2.dvY=lpVtxData[dwVtx2].dvY;
					vVtx2.dvZ=lpVtxData[dwVtx2].dvZ;
					vVtx3.dvX=lpVtxData[dwVtx3].dvX;
					vVtx3.dvY=lpVtxData[dwVtx3].dvY;
					vVtx3.dvZ=lpVtxData[dwVtx3].dvZ;
					// compute vectors
					vVec1=vVtx3-vVtx2;
					vVec2=vVtx1-vVtx2;
					// normalize vectors
					vNVec1=Normalize(vVec1);
					vNVec2=Normalize(vVec2);
					// compute polygon normal
					vVec3.dvX=(vNVec1.dvY*vNVec2.dvZ)-(vNVec1.dvZ*vNVec2.dvY);
					vVec3.dvY=(vNVec1.dvZ*vNVec2.dvX)-(vNVec1.dvX*vNVec2.dvZ);
					vVec3.dvZ=(vNVec1.dvX*vNVec2.dvY)-(vNVec1.dvY*vNVec2.dvX);
					// compute normal
					vNVec3=Normalize(vVec3);
					// get current vectors
					vCur1.dvX=lpVtxData[dwVtx1].dvNX;
					vCur1.dvY=lpVtxData[dwVtx1].dvNY;
					vCur1.dvZ=lpVtxData[dwVtx1].dvNZ;
					vCur2.dvX=lpVtxData[dwVtx2].dvNX;
					vCur2.dvY=lpVtxData[dwVtx2].dvNY;
					vCur2.dvZ=lpVtxData[dwVtx2].dvNZ;
					vCur3.dvX=lpVtxData[dwVtx3].dvNX;
					vCur3.dvY=lpVtxData[dwVtx3].dvNY;
					vCur3.dvZ=lpVtxData[dwVtx3].dvNZ;
					// add vectors
					vCur1+=vNVec3;
					vCur2+=vNVec3;
					vCur3+=vNVec3;
					// normalize vectors
					vNVec1=Normalize(vCur1);
					vNVec2=Normalize(vCur2);
					vNVec3=Normalize(vCur3);
					// store new vectors
					lpVtxData[dwVtx1].dvNX=vNVec1.dvX;
					lpVtxData[dwVtx1].dvNY=vNVec1.dvY;
					lpVtxData[dwVtx1].dvNZ=vNVec1.dvZ;
					lpVtxData[dwVtx2].dvNX=vNVec2.dvX;
					lpVtxData[dwVtx2].dvNY=vNVec2.dvY;
					lpVtxData[dwVtx2].dvNZ=vNVec2.dvZ;
					lpVtxData[dwVtx3].dvNX=vNVec3.dvX;
					lpVtxData[dwVtx3].dvNY=vNVec3.dvY;
					lpVtxData[dwVtx3].dvNZ=vNVec3.dvZ;
					// move vertex from 3 to 2
					dwVtx2=dwVtx3;
				}
				break;
			// set current texture
			case(OPCODE_SETTEXTURE):
				dwInsNum++;
				break;
			// set current material
			case(OPCODE_SETMATERIAL):
				dwInsNum++;
				break;
			// end render
			case(OPCODE_ENDRENDER):
				bDone=TRUE;
				break;
			// render line list
			case(OPCODE_LINELIST):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// average normals for each line
				for(dwCount=0;dwCount<dwVtxQty;dwCount++)
				{
					// read vertices
					dwVtx1=lpIns[dwInsNum++];
					dwVtx2=lpIns[dwInsNum++];
					// get points
					vVtx1.dvX=lpVtxData[dwVtx1].dvX;
					vVtx1.dvY=lpVtxData[dwVtx1].dvY;
					vVtx1.dvZ=lpVtxData[dwVtx1].dvZ;
					vVtx2.dvX=lpVtxData[dwVtx2].dvX;
					vVtx2.dvY=lpVtxData[dwVtx2].dvY;
					vVtx2.dvZ=lpVtxData[dwVtx2].dvZ;
					// compute vectors
					vVec1=vVtx1-vVtx2;
					vVec2=vVtx2-vVtx1;
					// compute normals
					vNVec1=Normalize(vVec1);
					vNVec2=Normalize(vVec2);
					// get current vectors
					vCur1.dvX=lpVtxData[dwVtx1].dvNX;
					vCur1.dvY=lpVtxData[dwVtx1].dvNY;
					vCur1.dvZ=lpVtxData[dwVtx1].dvNZ;
					vCur2.dvX=lpVtxData[dwVtx2].dvNX;
					vCur2.dvY=lpVtxData[dwVtx2].dvNY;
					vCur2.dvZ=lpVtxData[dwVtx2].dvNZ;
					// add vectors
					vCur1+=vNVec1;
					vCur2+=vNVec2;
					// normalize vectors
					vNVec1=Normalize(vCur1);
					vNVec2=Normalize(vCur2);
					// store new vectors
					lpVtxData[dwVtx1].dvNX=vNVec1.dvX;
					lpVtxData[dwVtx1].dvNY=vNVec1.dvY;
					lpVtxData[dwVtx1].dvNZ=vNVec1.dvZ;
					lpVtxData[dwVtx2].dvNX=vNVec2.dvX;
					lpVtxData[dwVtx2].dvNY=vNVec2.dvY;
					lpVtxData[dwVtx2].dvNZ=vNVec2.dvZ;
				}
				break;
			// render line strip
			case(OPCODE_LINESTRIP):
				// save number of vertices
				dwVtxQty=lpIns[dwInsNum++];
				// read first vertex
				dwVtx1=lpIns[dwInsNum++];
				// average normals for each line
				for(dwCount=0;dwCount<dwVtxQty;dwCount++)
				{
					// read next vertex
					dwVtx2=lpIns[dwInsNum++];
					// get points
					vVtx1.dvX=lpVtxData[dwVtx1].dvX;
					vVtx1.dvY=lpVtxData[dwVtx1].dvY;
					vVtx1.dvZ=lpVtxData[dwVtx1].dvZ;
					vVtx2.dvX=lpVtxData[dwVtx2].dvX;
					vVtx2.dvY=lpVtxData[dwVtx2].dvY;
					vVtx2.dvZ=lpVtxData[dwVtx2].dvZ;
					// compute vectors
					vVec1=vVtx1-vVtx2;
					vVec2=vVtx2-vVtx1;
					// compute normals
					vNVec1=Normalize(vVec1);
					vNVec2=Normalize(vVec2);
					// get current vectors
					vCur1.dvX=lpVtxData[dwVtx1].dvNX;
					vCur1.dvY=lpVtxData[dwVtx1].dvNY;
					vCur1.dvZ=lpVtxData[dwVtx1].dvNZ;
					vCur2.dvX=lpVtxData[dwVtx2].dvNX;
					vCur2.dvY=lpVtxData[dwVtx2].dvNY;
					vCur2.dvZ=lpVtxData[dwVtx2].dvNZ;
					// add vectors
					vCur1+=vNVec1;
					vCur2+=vNVec2;
					// normalize vectors
					vNVec1=Normalize(vCur1);
					vNVec2=Normalize(vCur2);
					// store new vectors
					lpVtxData[dwVtx1].dvNX=vNVec1.dvX;
					lpVtxData[dwVtx1].dvNY=vNVec1.dvY;
					lpVtxData[dwVtx1].dvNZ=vNVec1.dvZ;
					lpVtxData[dwVtx2].dvNX=vNVec2.dvX;
					lpVtxData[dwVtx2].dvNY=vNVec2.dvY;
					lpVtxData[dwVtx2].dvNZ=vNVec2.dvZ;
					// move vertex 2 to vertex 1
					dwVtx1=dwVtx2;
				}
				break;
			// render point list			
			case(OPCODE_POINTLIST):
				dwInsNum+=2;
				break;
			// optimize
			case(OPCODE_OPTIMIZE):
				// change to ENDRENDER
				lpIns[dwInsNum-1]=OPCODE_ENDRENDER;
				break;
		}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::Release()						//
//		(defined in "mdlfunc.h")						//
//														//
//	Free model.											//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::Release(void)
{
	// counter
	DWORD dwCount=0;

	// release all materials
	if(dwNumMat)
		for(dwCount=0;dwCount<dwNumMat;dwCount++)
			DELETE_MATERIAL(lplpMat[dwCount]);
	// release material memory
	SAFE_FREE(lplpMat);
	// release all textures
	if(dwNumTex)
		for(dwCount=0;dwCount<dwNumTex;dwCount++)
			DELETE_TEXTURE(lplpTex[dwCount]);
	// release texture memory
	SAFE_FREE(lplpTex);
	// release vertex buffer
	SAFE_RELEASE(lpVtx);
	// free instructions
	SAFE_FREE(lpIns);
	// free pre-render instructions
	SAFE_FREE(lpPRIns);
	// reset values
	lplpMat=NULL;
	dwNumMat=0;
	lplpTex=NULL;
	dwNumTex=0;
	lpVtx=NULL;
	dwNumVtx=0;
	lpIns=NULL;
	dwNumIns=0;
	INIT_VAR(vPos);
	INIT_VAR(vRight);
	INIT_VAR(vUp);
	INIT_VAR(vAhead);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DModel::ComputeExtents()				//
//		(defined in "mdlfunc.h")						//
//														//
//	Computes extents of model.							//
//	  Parameters:										//
//		D3DVERTEX*: pointer to vertex data				//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DModel::ComputeExtents(D3DVERTEX* lpVtxData)
{
	// counter
	DWORD dwCount;

	// reset values
	dvHDRadSq=0;
	dvHDLevelU=0;
	dvHDLevelL=0;
	// loop through each vertex
	for(dwCount=0;dwCount<dwNumVtx;dwCount++)
	{
		// temporary value
		D3DVALUE dvTemp;

		// compute radius
		dvTemp=(lpVtxData[dwCount].dvX*lpVtxData[dwCount].dvX)+
			   (lpVtxData[dwCount].dvZ*lpVtxData[dwCount].dvZ);
		// compare value
		if(dvHDRadSq<dvTemp)
			dvHDRadSq=dvTemp;
		// find height
		dvTemp=lpVtxData[dwCount].dvY;
		// compare value
		if(dvHDLevelU<dvTemp)
			dvHDLevelU=dvTemp;
		// compare value
		if(dvHDLevelL>dvTemp)
			dvHDLevelL=dvTemp;
	}
	// compute radius
	dvHDRad=sqrtf(dvHDRadSq);
	// shrink values
	dvHDRad*=HD_SHRINK_FACTOR;
	dvHDLevelU*=HD_SHRINK_FACTOR;
	dvHDLevelL*=HD_SHRINK_FACTOR;
	// re-compute radius squared
	dvHDRadSq=dvHDRad*dvHDRad;
}

// include encode functions if necessary
#ifdef ENCODE_DATA

// PROTOTYPES ////////////////////////////////////////////

void GetChar(LPBYTE lpc);

void GetDWORD(LPDWORD lpdw);

void GetFloat(float* lpf);

void GetString(LPSTR lpstr);

// GLOBALS ///////////////////////////////////////////////

HANDLE g_hModelFile=INVALID_HANDLE_VALUE;

// ERROR MESSAGES ////////////////////////////////////////

// failed to open file
#define ERR_MDLFUNC00e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MDLFUNC.CPP\nError Code: 00e\nUnable to open input file.") }
// failed to read file
#define ERR_MDLFUNC01e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: MDLFUNC.CPP\nError Code: 01e\nUnable to read input file.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EncodeModel()							//
//		(defined in "mdlfunc.h")						//
//														//
//	Save model to previously open data file.			//
//	  Parameters:										//
//		LPCSTR: filename								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EncodeModel(LPCSTR lpFilename)
{
	// number of materials
	DWORD dwNumMat=0;
	// material names
	LPSTR lpMatNames=NULL;
	// counter
	DWORD dwCount=0;
	// secondary counter
	DWORD dwCount2=0;
	// number of textures
	DWORD dwNumTex=0;
	// texture names
	LPSTR lpTexNames=NULL;
	// number of vertices
	DWORD dwNumVtx=0;
	// vertex names
	LPSTR lpVtxNames=NULL;
	// instructions
	LPWORD lpIns=NULL;
	// number of instructions
	DWORD dwNumIns=0;
	// size of instruction buffer
	DWORD dwDataSize=0;
	// done flag
	BOOL bDone=FALSE;
	// text storage
	char lpText[32];
	// temporary value
	DWORD dwTemp=0;
	
	// output ID to verification file
	WriteVerificationID(DTF_IDMODEL);
	// output ID to data file
	WriteDataID(DTF_IDMODEL);
	// open file
	g_hModelFile=CreateFile(lpFilename,
							GENERIC_READ,
							NULL,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL|
							FILE_FLAG_SEQUENTIAL_SCAN,
							NULL);
	// check for error
	if(!g_hModelFile)
	{
		ERR_MDLFUNC00e;
		return;
	}
	// read billboard flag
	GetDWORD(&dwTemp);
	// output billboard flag
	WriteDataDWORD(dwTemp);
	// read number of materials
	GetDWORD(&dwNumMat);
	// output number of materials
	WriteDataDWORD(dwNumMat);
	// output material info
	if(dwNumMat)
	{
		// allow 32 bytes for each material name
		SAFE_ALLOC(lpMatNames,
				   LPSTR,
				   dwNumMat*32);
		// check for error
		if(!lpMatNames)
			return;
		// read material info
		for(dwCount=0;dwCount<dwNumMat;dwCount++)
		{
			// get name
			GetString(&lpMatNames[dwCount*32]);
			// read data
			for(dwCount2=0;dwCount2<17;dwCount2++)
			{
				// storage for data
				float fVal=0;
				// output data
				DWORD dwVal=0;
				
				// read data
				GetFloat(&fVal);
				// convert data
				dwVal=FLOAT_TO_FIXED(fVal);
				// output data
				WriteDataDWORD(dwVal);
			}
		}
	}
	// read number of textures
	GetDWORD(&dwNumTex);
	// output number of textures
	WriteDataDWORD(dwNumTex);
	// output texture info
	if(dwNumTex)
	{
		// allow 32 bytes for each texture name
		SAFE_ALLOC(lpTexNames,
				   LPSTR,
				   dwNumTex*32);
		// check for error
		if(!lpTexNames)
			return;
		// read texture info
		for(dwCount=0;dwCount<dwNumTex;dwCount++)
		{
			// filename storage
			char lpFile[32];
			// output value
			DWORD dwVal=0;

			// get name
			GetString(&lpTexNames[dwCount*32]);
			// get filename
			GetString(lpFile);
			// encode texture
			EncodeTexture(lpFile);
		}
	}
	// read number of vertices
	GetDWORD(&dwNumVtx);
	// output number of vertices
	WriteDataDWORD(dwNumVtx);
	// output vertices
	if(dwNumVtx)
	{
		// allow 32 bytes for each vertex name
		SAFE_ALLOC(lpVtxNames,
				   LPSTR,
				   dwNumVtx*32);
		// check for error
		if(!lpVtxNames)
			return;
		// read vertex info
		for(dwCount=0;dwCount<dwNumVtx;dwCount++)
		{
			// get name
			GetString(&lpVtxNames[dwCount*32]);
			// output values
			for(dwCount2=0;dwCount2<5;dwCount2++)
			{
				// storage for data
				float fVal=0;
				// output data
				long lVal=0;
				
				// read data
				GetFloat(&fVal);
				// convert data
				lVal=FLOAT_TO_FIXED(fVal);
				// output data
				WriteDataDWORD(lVal);
			}
		}
	}
	// macros for this function
	#define ADD_INSTR(a) { if(dwNumIns==dwDataSize) { LPVOID lpTemp; SAFE_ALLOC(lpTemp,LPVOID,(dwDataSize+100)*sizeof(WORD)); if(!lpTemp) return; CopyMemory(lpTemp,lpIns,dwDataSize*sizeof(WORD)); SwapPointers(&lpTemp,(LPVOID*)&lpIns); SAFE_FREE(lpTemp); dwDataSize+=100; } lpIns[dwNumIns++]=(WORD)a; }
	#define MATERIAL_TO_WORD(a,b) { if(dwNumMat) { WORD wC; for(wC=0;wC<dwNumMat;wC++) if(!strcmp(a,&lpMatNames[wC*32])) b=wC; } else b=0xFFFF; } 
	#define TEXTURE_TO_WORD(a,b) { if(dwNumTex) { WORD wC; for(wC=0;wC<dwNumTex;wC++) if(!strcmp(a,&lpTexNames[wC*32])) b=wC; } else b=0xFFFF; }
	#define VERTEX_TO_WORD(a,b) { if(dwNumVtx) { WORD wC; for(wC=0;wC<dwNumVtx;wC++) if(!strcmp(a,&lpVtxNames[wC*32])) b=wC; } else b=0xFFFF; } 
	// allocate memory for pre-render instructions
	SAFE_ALLOC(lpIns,
			   LPWORD,
			   sizeof(WORD)*100);
	// check for error
	if(!lpIns)
		return;
	// set data size
	dwDataSize=100;
	// read instructions
	while(!bDone)
	{
		// read instruction
		GetString(lpText);
		if(!strcmp("SETMATERIAL",lpText))
		{
			// output value
			DWORD dwVal=0;

			// add instruction
			ADD_INSTR(OPCODE_SETMATERIAL);
			// read material name
			GetString(lpText);
			// convert material
			MATERIAL_TO_WORD(lpText,dwVal);
			// output value
			ADD_INSTR(dwVal);
		}
		if(!strcmp("SETTEXTURE",lpText))
		{
			// output value
			DWORD dwVal=0;

			// add instruction
			ADD_INSTR(OPCODE_SETTEXTURE);
			// read texture name
			GetString(lpText);
			// convert texture
			TEXTURE_TO_WORD(lpText,dwVal);
			// output value
			ADD_INSTR(dwVal);
		}
		if(!strcmp("ENDPRERENDER",lpText))
		{
			// add instruction
			ADD_INSTR(OPCODE_ENDPRERENDER);
			// done
			bDone=TRUE;
		}
	}
	// output number of instructions
	WriteDataDWORD(dwNumIns);
	// output instructions
	WriteDataBytes((LPBYTE)lpIns,dwNumIns*sizeof(WORD));
	// free memory
	SAFE_FREE(lpIns);
	// reset number of instructions
	dwNumIns=0;
	// allocate memory for render instructions
	SAFE_ALLOC(lpIns,
			   LPWORD,
			   sizeof(WORD)*100);
	// check for error
	if(!lpIns)
		return;
	// set data size
	dwDataSize=100;
	// reset flag
	bDone=FALSE;
	// read instructions
	while(!bDone)
	{
		// read instruction
		GetString(lpText);
		// process instruction
		if(!strcmp("POINTLIST",lpText))
		{
			// output value
			DWORD dwVal=0;

			// output instruction
			ADD_INSTR(OPCODE_POINTLIST);
			// read vertex
			GetString(lpText);
			// convert vertex
			VERTEX_TO_WORD(lpText,dwVal);
			// output vertex
			ADD_INSTR(dwVal);
			// read quantity
			GetDWORD(&dwVal);
			// output quantity
			ADD_INSTR(dwVal);
		}
		if(!strcmp("LINELIST",lpText))
		{
			// output value
			DWORD dwVal=0;

			// output instruction
			ADD_INSTR(OPCODE_LINELIST);
			// read quantity
			GetDWORD(&dwVal);
			// output quantity
			ADD_INSTR(dwVal);
			// save quantity
			dwCount2=dwVal;
			// process vertices
			for(dwCount=0;dwCount<dwCount2*2;dwCount++)
			{
				// read vertex
				GetString(lpText);
				// convert vertex
				VERTEX_TO_WORD(lpText,dwVal);
				// output vertex
				ADD_INSTR(dwVal);
			}
		}
		if(!strcmp("LINESTRIP",lpText))
		{
			// output value
			DWORD dwVal=0;

			// output instruction
			ADD_INSTR(OPCODE_LINESTRIP);
			// read quantity
			GetDWORD(&dwVal);
			// output quantity
			ADD_INSTR(dwVal);
			// save quantity
			dwCount2=dwVal;
			// process vertices
			for(dwCount=0;dwCount<dwCount2+1;dwCount++)
			{
				// read vertex
				GetString(lpText);
				// convert vertex
				VERTEX_TO_WORD(lpText,dwVal);
				// output vertex
				ADD_INSTR(dwVal);
			}
		}
		if(!strcmp("TRIANGLELIST",lpText))
		{
			// output value
			DWORD dwVal=0;

			// output instruction
			ADD_INSTR(OPCODE_TRIANGLELIST);
			// read quantity
			GetDWORD(&dwVal);
			// output quantity
			ADD_INSTR(dwVal);
			// save quantity
			dwCount2=dwVal;
			// process vertices
			for(dwCount=0;dwCount<dwCount2*3;dwCount++)
			{
				// read vertex
				GetString(lpText);
				// convert vertex
				VERTEX_TO_WORD(lpText,dwVal);
				// output vertex
				ADD_INSTR(dwVal);
			}
		}
		if(!strcmp("TRIANGLESTRIP",lpText))
		{
			// output value
			DWORD dwVal=0;

			// output instruction
			ADD_INSTR(OPCODE_TRIANGLESTRIP);
			// read quantity
			GetDWORD(&dwVal);
			// output quantity
			ADD_INSTR(dwVal);
			// save quantity
			dwCount2=dwVal;
			// process vertices
			for(dwCount=0;dwCount<dwCount2+2;dwCount++)
			{
				// read vertex
				GetString(lpText);
				// convert vertex
				VERTEX_TO_WORD(lpText,dwVal);
				// output vertex
				ADD_INSTR(dwVal);
			}
		}
		if(!strcmp("TRIANGLEFAN",lpText))
		{
			// output value
			DWORD dwVal=0;

			// output instruction
			ADD_INSTR(OPCODE_TRIANGLEFAN);
			// read quantity
			GetDWORD(&dwVal);
			// output quantity
			ADD_INSTR(dwVal);
			// save quantity
			dwCount2=dwVal;
			// process vertices
			for(dwCount=0;dwCount<dwCount2+2;dwCount++)
			{
				// read vertex
				GetString(lpText);
				// convert vertex
				VERTEX_TO_WORD(lpText,dwVal);
				// output vertex
				ADD_INSTR(dwVal);
			}
		}
		if(!strcmp("SETMATERIAL",lpText))
		{
			// output value
			DWORD dwVal=0;

			// add instruction
			ADD_INSTR(OPCODE_SETMATERIAL);
			// read material name
			GetString(lpText);
			// convert material
			MATERIAL_TO_WORD(lpText,dwVal);
			// output value
			ADD_INSTR(dwVal);
		}
		if(!strcmp("SETTEXTURE",lpText))
		{
			// output value
			DWORD dwVal=0;

			// add instruction
			ADD_INSTR(OPCODE_SETTEXTURE);
			// read texture name
			GetString(lpText);
			// convert texture
			TEXTURE_TO_WORD(lpText,dwVal);
			// output value
			ADD_INSTR(dwVal);
		}
		if(!strcmp("ENDRENDER",lpText))
		{
			// add instruction
			ADD_INSTR(OPCODE_ENDRENDER);
			// done
			bDone=TRUE;
		}
		if(!strcmp("OPTIMIZE",lpText))
		{
			// add instruction
			ADD_INSTR(OPCODE_OPTIMIZE);
		}
	}
	// output number of instructions
	WriteDataDWORD(dwNumIns);
	// output instructions
	WriteDataBytes((LPBYTE)lpIns,dwNumIns*sizeof(WORD));
	// free memory
	SAFE_FREE(lpIns);
	SAFE_FREE(lpVtxNames);
	SAFE_FREE(lpTexNames);
	SAFE_FREE(lpMatNames);
	// close file
	CloseHandle(g_hModelFile);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetChar()								//
//														//
//	Read character from open model file.				//
//	  Parameters:										//
//		LPBYTE: pointer to byte							//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void GetChar(LPBYTE lpc)
{
	// bytes read by ReadFile()	
	DWORD dwBytesRead=0;

	// read byte
	ReadFile(g_hModelFile,
			 (LPVOID)lpc,
			 sizeof(BYTE),
			 &dwBytesRead,
			 NULL);
	// check for error
	if(dwBytesRead!=sizeof(BYTE))
	{
		ERR_MDLFUNC01e;
		return;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetDWORD()								//
//														//
//	Read DWORD from open model file.					//
//	  Parameters:										//
//		LPDWORD: pointer to data						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void GetDWORD(LPDWORD lpdw)
{
	// data
	BYTE cData=0;
	// return value
	DWORD dwRetVal=0;

	// read character
	GetChar(&cData);
	// check for comment
	if(cData=='*')
	{
		// read to end of line
		while(cData!=10)
			GetChar(&cData);
		// re-call this function
		GetDWORD(lpdw);
		return;
	}
	// read to end of file
	while(cData!=13)
	{
		// multiply value
		dwRetVal*=10;
		// add new value
		dwRetVal+=(long)(cData-'0');
		// get next value
		GetChar(&cData);
	}
	// get next line
	GetChar(&cData);
	// copy value
	*lpdw=dwRetVal;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetFloat()								//
//														//
//	Read float from open model file.					//
//	  Parameters:										//
//		float*: pointer to data							//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void GetFloat(float* lpf)
{
	// data
	BYTE cData=0;
	// return value left of decimal
	float fRetVal1=0;
	// return value right of decimal
	float fRetVal2=0;
	// return value
	float fRetVal=0;
	// counter
	DWORD dwCount=0;
	// negative flag
	BOOL bNegative=FALSE;

	// read character
	GetChar(&cData);
	// check for comment
	if(cData=='*')
	{
		// read to end of line
		while(cData!=10)
			GetChar(&cData);
		// re-call this function
		GetFloat(lpf);
		return;
	}
	// check for negative
	if(cData=='-')
	{
		// set flag
		bNegative=TRUE;
		// next byte
		GetChar(&cData);
	}
	// read left of decimal
	while(cData!='.')
	{
		// multiply value
		fRetVal1*=10;
		// add value
		fRetVal1+=(float)(cData-'0');
		// next byte
		GetChar(&cData);
	}
	// skip decimal
	GetChar(&cData);
	// read to end of line
	while(cData!=13)
	{
		// multiply value
		fRetVal2*=10;
		// add value
		fRetVal2+=(float)(cData-'0');
		// next byte
		GetChar(&cData);
		// increment place counter
		dwCount++;
	}
	// get next line
	GetChar(&cData);
	// convert decimal value
	for(;dwCount>0;dwCount--)
		// divide value
		fRetVal2/=10;
	// compute return value
	fRetVal=fRetVal1+fRetVal2;
	// return negative or positive
	if(bNegative)
		*lpf=-fRetVal;
	else	
		*lpf=fRetVal;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetString()								//
//														//
//	Read string from open model file.					//
//	  Parameters:										//
//		LPSTR: pointer to string						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void GetString(LPSTR lpstr)
{
	// data
	BYTE cData=0;
	// string position
	DWORD dwStrPos=0;

	// read character
	GetChar(&cData);
	// check for comment
	if(cData=='*')
	{
		// read to end of line
		while(cData!=10)
			GetChar(&cData);
		// re-call this function
		GetString(lpstr);
		return;
	}
	// read to end of line
	while(cData!=13)
	{
		// store byte
		lpstr[dwStrPos++]=cData;
		// next byte
		GetChar(&cData);
	}
	// get next line
	GetChar(&cData);
	// set NULL terminator
	lpstr[dwStrPos]=0;
}

#endif

// END ///////////////////////////////////////////////////

