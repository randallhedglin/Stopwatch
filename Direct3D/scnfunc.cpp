// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  scnfunc.cpp									//
//														//
//	Scenery functions.									//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// allow access to materials
#include"matfunc.h"
// allow access to bitmaps
#include"bmpfunc.h"
// allow access to textures
#include"txfunc.h"
// allow access to models
#include"mdlfunc.h"
// header for this module
#include"scnfunc.h"
// allow access to window handles
#include"winmain.h"
// allow access to file handling functions
#include"filefunc.h"
// allow access to DirectDraw
#include"ddfunc.h"
// allow access to Direct3D
#include"d3dfunc.h"

// ERROR MESSAGES ////////////////////////////////////////

// invalid data
#define ERR_SCNFUNC00 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: SCNFUNC.CPP\nError Code: 00\nInvalid data in data file.") }
// failed to create new model
#define ERR_SCNFUNC01 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: SCNFUNC.CPP\nError Code: 01\nUnable to create scenery model.") }
// failed to lock vertex buffer
#define ERR_SCNFUNC02 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: SCNFUNC.CPP\nError Code: 02\nUnable to lock vertex buffer.") }
// failed to unlock vertex buffer
#define ERR_SCNFUNC03 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: SCNFUNC.CPP\nError Code: 03\nUnable to unlock vertex buffer.") }
// failed to optimize vertex buffer
#define ERR_SCNFUNC04 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: SCNFUNC.CPP\nError Code: 04\nUnable to optimize vertex buffer.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DScenery::CD3DScenery()				//
//		(defined in "scnfunc.h")						//
//														//
//	Class constructor for scenery.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

CD3DScenery::CD3DScenery()
{
	// reset values	
	lpMdl=NULL;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DScenery::Load()						//
//		(defined in "scnfunc.h")						//
//														//
//	Load scenery sphere from open data file.			//
//	  Parameters:										//
//		D3DVALUE: radius of sphere						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

BOOL CD3DScenery::Load(D3DVALUE dvRad)
{
	// data type
	BYTE cType=NULL;
	// counter
	DWORD dwCount=0;
	// vertex data
	D3DVERTEX* lpVtxData=NULL;

	// read data type
	ReadData(&cType,sizeof(BYTE));
	// check data type
	if(cType!=DTF_IDSCENERY)
	{
		ERR_SCNFUNC00;
		return(FALSE);
	}
	// new model
	SAFE_NEW(lpMdl,
			 CD3DModel);
	// check for error
	if(!lpMdl)
	{
		ERR_SCNFUNC01;
		return(FALSE);
	}
	// load model
	if(!lpMdl->Load())
		return(FALSE);
	// lock vertex buffer
	if(FAILED(lpMdl->lpVtx->Lock(DDLOCK_SURFACEMEMORYPTR|
								 DDLOCK_WAIT,
								 (LPVOID*)&lpVtxData,
								 NULL)))
	{
		ERR_SCNFUNC02;
		return(FALSE);
	}
	// expand vertices
	for(dwCount=0;dwCount<lpMdl->dwNumVtx;dwCount++)
	{
		lpVtxData[dwCount].dvX*=dvRad;
		lpVtxData[dwCount].dvY*=dvRad;
		lpVtxData[dwCount].dvZ*=dvRad;
	}
	// unlock vertex buffer
	if(FAILED(lpMdl->lpVtx->Unlock()))
	{
		ERR_SCNFUNC03;
		return(FALSE);
	}
	// optimize buffer 
	if(FAILED(lpMdl->lpVtx->Optimize(GetGlobalDirect3DDevicePtr(),
									 NULL)))
		ERR_SCNFUNC04;
	// create valid orientation
	lpMdl->vRight=D3DVECTOR(1,0,0);
	lpMdl->vUp=D3DVECTOR(0,1,0);
	lpMdl->vAhead=D3DVECTOR(0,0,1);
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
//	Function:  CD3DScenery::Render()					//
//		(defined in "scnfunc.h")						//
//														//
//	Render scenery sphere.								//
//	  Parameters:										//
//		D3DVECTOR*: pointer to position					//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DScenery::Render(D3DVECTOR* lpvPos)
{
	// set position
	lpMdl->Position(lpvPos);
	// no need for z-buffer writes in this step
	DisableZBufferWrites();
	// render model
	lpMdl->Render();
	// turn z-buffer back on
	EnableZBufferWrites();
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CD3DScenery::Release()					//
//		(defined in "scnfunc.h")						//
//														//
//	Free scenery memory.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CD3DScenery::Release(void)
{
	// release model
	DELETE_MODEL(lpMdl);
}

// include encode functions if necessary

#ifdef ENCODE_DATA

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EncodeScenery()							//
//		(defined in "scnfunc.h")						//
//														//
//	Output scenery data to open data file.				//
//	  Parameters:										//
//		LPCSTR: texture for top (or NULL)				//
//		LPCSTR: texture for left (or NULL)				//
//		LPCSTR: texture for front (or NULL)				//
//		LPCSTR: texture for right (or NULL)				//
//		LPCSTR: texture for back (or NULL)				//
//		LPCSTR: texture for bottom (or NULL)				//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EncodeScenery(LPCSTR lpFilename1,
						  LPCSTR lpFilename2,
						  LPCSTR lpFilename3,
						  LPCSTR lpFilename4,
						  LPCSTR lpFilename5,
						  LPCSTR lpFilename6)
{
	// number of materials
	DWORD dwNumMat=0;
	// counter
	DWORD dwCount=0;
	// number of textures
	DWORD dwNumTex=0;
	// texture counter
	DWORD dwTexNum=0;
	// number of vertices
	DWORD dwNumVtx=0;
	// vertex counter
	DWORD dwVtxNum=0;
	// instructions
	LPWORD lpIns=NULL;
	// number of instructions
	DWORD dwNumIns=0;
	// size of instruction buffer
	DWORD dwDataSize=0;
	// billboard flag
	BOOL bBillboard=FALSE;
	// temporary value
	DWORD dwTemp=0;
	// temporary value
	WORD wTemp=0;
	
	// output ID to verification file
	WriteVerificationID(DTF_IDSCENERY);
	// output ID to data file
	WriteDataID(DTF_IDSCENERY);
	// output ID to verification file
	WriteVerificationID(DTF_IDMODEL);
	// output ID to data file
	WriteDataID(DTF_IDMODEL);
	// read billboard flag
	bBillboard=FALSE;
	// output billboard flag
	WriteDataDWORD(bBillboard);
	// read number of materials
	dwNumMat=1;
	// output number of materials
	WriteDataDWORD(dwNumMat);
	// output diffuse
	WriteDataDWORD(FLOAT_TO_FIXED(1));
	WriteDataDWORD(FLOAT_TO_FIXED(1));
	WriteDataDWORD(FLOAT_TO_FIXED(1));
	WriteDataDWORD(FLOAT_TO_FIXED(1));
	// output ambient
	WriteDataDWORD(FLOAT_TO_FIXED(1));
	WriteDataDWORD(FLOAT_TO_FIXED(1));
	WriteDataDWORD(FLOAT_TO_FIXED(1));
	WriteDataDWORD(FLOAT_TO_FIXED(1));
	// output specular
	WriteDataDWORD(FLOAT_TO_FIXED(0));
	WriteDataDWORD(FLOAT_TO_FIXED(0));
	WriteDataDWORD(FLOAT_TO_FIXED(0));
	WriteDataDWORD(FLOAT_TO_FIXED(0));
	WriteDataDWORD(FLOAT_TO_FIXED(0));
	// output emissive
	WriteDataDWORD(FLOAT_TO_FIXED(1));
	WriteDataDWORD(FLOAT_TO_FIXED(1));
	WriteDataDWORD(FLOAT_TO_FIXED(1));
	WriteDataDWORD(FLOAT_TO_FIXED(1));
	// compute number of textures
	if(lpFilename1)
	{
		dwNumTex++;
		dwNumVtx+=5;
	}
	if(lpFilename2)
	{
		dwNumTex++;
		dwNumVtx+=5;
	}
	if(lpFilename3)
	{
		dwNumTex++;
		dwNumVtx+=5;
	}
	if(lpFilename4)
	{
		dwNumTex++;
		dwNumVtx+=5;
	}
	if(lpFilename5)
	{
		dwNumTex++;
		dwNumVtx+=5;
	}
	if(lpFilename6)
	{
		dwNumTex++;
		dwNumVtx+=5;
	}
	// output number of textures
	WriteDataDWORD(dwNumTex);
	// output texture info
	if(lpFilename1)
		EncodeTexture(lpFilename1);
	if(lpFilename2)
		EncodeTexture(lpFilename2);
	if(lpFilename3)
		EncodeTexture(lpFilename3);
	if(lpFilename4)
		EncodeTexture(lpFilename4);
	if(lpFilename5)
		EncodeTexture(lpFilename5);
	if(lpFilename6)
		EncodeTexture(lpFilename6);
	// output number of vertices
	WriteDataDWORD(dwNumVtx);
	// macro to output vertices
	#define OUTPUT_COORDINATES(x,y,z,tu,tv) { WriteDataDWORD(FLOAT_TO_FIXED(x)); WriteDataDWORD(FLOAT_TO_FIXED(y)); WriteDataDWORD(FLOAT_TO_FIXED(z)); WriteDataDWORD(FLOAT_TO_FIXED(tu)); WriteDataDWORD(FLOAT_TO_FIXED(tv)); }
	// sphere value
	#define v 0.707106F
	// output vertices
	if(lpFilename1)
	{
		OUTPUT_COORDINATES(-v,v,v,0,1);
		OUTPUT_COORDINATES(v,v,v,1,1);
		OUTPUT_COORDINATES(-v,v,-v,0,0);
		OUTPUT_COORDINATES(v,v,-v,1,0);
		OUTPUT_COORDINATES(0,1,0,.5F,.5F);
	}
	if(lpFilename2)
	{
		OUTPUT_COORDINATES(-v,v,-v,0,0);
		OUTPUT_COORDINATES(-v,v,v,1,0);
		OUTPUT_COORDINATES(-v,-v,-v,0,1);
		OUTPUT_COORDINATES(-v,-v,v,1,1);
		OUTPUT_COORDINATES(-1,0,0,.5F,.5F);
	}
	if(lpFilename3)
	{
		OUTPUT_COORDINATES(-v,v,v,0,0);
		OUTPUT_COORDINATES(v,v,v,1,0);
		OUTPUT_COORDINATES(-v,-v,v,0,1);
		OUTPUT_COORDINATES(v,-v,v,1,1);
		OUTPUT_COORDINATES(0,0,1,.5F,.5F);
	}
	if(lpFilename4)
	{
		OUTPUT_COORDINATES(v,v,v,0,0);
		OUTPUT_COORDINATES(v,v,-v,1,0);
		OUTPUT_COORDINATES(v,-v,v,0,1);
		OUTPUT_COORDINATES(v,-v,-v,1,1);
		OUTPUT_COORDINATES(1,0,0,.5F,.5F);
	}
	if(lpFilename5)
	{
		OUTPUT_COORDINATES(v,v,-v,0,0);
		OUTPUT_COORDINATES(-v,v,-v,1,0);
		OUTPUT_COORDINATES(v,-v,-v,0,1);
		OUTPUT_COORDINATES(-v,-v,-v,1,1);
		OUTPUT_COORDINATES(0,0,-1,.5F,.5F);
	}
	if(lpFilename6)
	{
		OUTPUT_COORDINATES(-v,-v,v,0,0);
		OUTPUT_COORDINATES(v,-v,v,1,0);
		OUTPUT_COORDINATES(-v,-v,-v,0,1);
		OUTPUT_COORDINATES(v,-v,-v,1,1);
		OUTPUT_COORDINATES(0,-1,0,.5F,.5F);
	}
	// output pre-render instructions
	dwTemp=1;
	WriteDataDWORD(dwTemp);
	wTemp=OPCODE_ENDPRERENDER;
	WriteDataBytes((LPBYTE)&wTemp,sizeof(WORD));
	// allocate memory for 100 instructions
	SAFE_ALLOC(lpIns,
			   LPWORD,
			   sizeof(WORD)*100);
	// check for error
	if(!lpIns)
		return;
	// set data size
	dwDataSize=100;
	// macro for this function
	#define ADD_INSTR(a) { if(dwNumIns==dwDataSize) { LPVOID lpTemp; SAFE_ALLOC(lpTemp,LPVOID,(dwDataSize+100)*sizeof(WORD)); if(!lpTemp) return; CopyMemory(lpTemp,lpIns,dwDataSize*sizeof(WORD)); SwapPointers(&lpTemp,(LPVOID*)&lpIns); SAFE_FREE(lpTemp); dwDataSize+=100; } lpIns[dwNumIns++]=(WORD)a; }
	// set instructions
	ADD_INSTR(OPCODE_SETMATERIAL);
	ADD_INSTR(0);
	if(lpFilename1)
	{
		// set texture
		ADD_INSTR(OPCODE_SETTEXTURE);
		ADD_INSTR(dwTexNum);
		// render triangles
		ADD_INSTR(OPCODE_TRIANGLELIST);
		ADD_INSTR(4);
		ADD_INSTR(dwVtxNum+1);
		ADD_INSTR(dwVtxNum+0);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+3);
		ADD_INSTR(dwVtxNum+1);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+2);
		ADD_INSTR(dwVtxNum+3);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+0);
		ADD_INSTR(dwVtxNum+2);
		ADD_INSTR(dwVtxNum+4);
		// increment counters
		dwTexNum++;
		dwVtxNum+=5;
	}
	if(lpFilename2)
	{
		// set texture
		ADD_INSTR(OPCODE_SETTEXTURE);
		ADD_INSTR(dwTexNum);
		// render triangles
		ADD_INSTR(OPCODE_TRIANGLELIST);
		ADD_INSTR(4);
		ADD_INSTR(dwVtxNum+0);
		ADD_INSTR(dwVtxNum+1);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+1);
		ADD_INSTR(dwVtxNum+3);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+3);
		ADD_INSTR(dwVtxNum+2);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+2);
		ADD_INSTR(dwVtxNum+0);
		ADD_INSTR(dwVtxNum+4);
		// increment counters
		dwTexNum++;
		dwVtxNum+=5;
	}
	if(lpFilename3)
	{
		// set texture
		ADD_INSTR(OPCODE_SETTEXTURE);
		ADD_INSTR(dwTexNum);
		// render triangles
		ADD_INSTR(OPCODE_TRIANGLELIST);
		ADD_INSTR(4);
		ADD_INSTR(dwVtxNum+0);
		ADD_INSTR(dwVtxNum+1);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+1);
		ADD_INSTR(dwVtxNum+3);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+3);
		ADD_INSTR(dwVtxNum+2);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+2);
		ADD_INSTR(dwVtxNum+0);
		ADD_INSTR(dwVtxNum+4);
		// increment counters
		dwTexNum++;
		dwVtxNum+=5;
	}
	if(lpFilename4)
	{
		// set texture
		ADD_INSTR(OPCODE_SETTEXTURE);
		ADD_INSTR(dwTexNum);
		// render triangles
		ADD_INSTR(OPCODE_TRIANGLELIST);
		ADD_INSTR(4);
		ADD_INSTR(dwVtxNum+0);
		ADD_INSTR(dwVtxNum+1);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+1);
		ADD_INSTR(dwVtxNum+3);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+3);
		ADD_INSTR(dwVtxNum+2);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+2);
		ADD_INSTR(dwVtxNum+0);
		ADD_INSTR(dwVtxNum+4);
		// increment counters
		dwTexNum++;
		dwVtxNum+=5;
	}
	if(lpFilename5)
	{
		// set texture
		ADD_INSTR(OPCODE_SETTEXTURE);
		ADD_INSTR(dwTexNum);
		// render triangles
		ADD_INSTR(OPCODE_TRIANGLELIST);
		ADD_INSTR(4);
		ADD_INSTR(dwVtxNum+0);
		ADD_INSTR(dwVtxNum+1);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+1);
		ADD_INSTR(dwVtxNum+3);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+3);
		ADD_INSTR(dwVtxNum+2);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+2);
		ADD_INSTR(dwVtxNum+0);
		ADD_INSTR(dwVtxNum+4);
		// increment counters
		dwTexNum++;
		dwVtxNum+=5;
	}
	if(lpFilename6)
	{
		// set texture
		ADD_INSTR(OPCODE_SETTEXTURE);
		ADD_INSTR(dwTexNum);
		// render triangles
		ADD_INSTR(OPCODE_TRIANGLELIST);
		ADD_INSTR(4);
		ADD_INSTR(dwVtxNum+0);
		ADD_INSTR(dwVtxNum+1);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+1);
		ADD_INSTR(dwVtxNum+3);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+3);
		ADD_INSTR(dwVtxNum+2);
		ADD_INSTR(dwVtxNum+4);
		ADD_INSTR(dwVtxNum+2);
		ADD_INSTR(dwVtxNum+0);
		ADD_INSTR(dwVtxNum+4);
		// increment counters
		dwTexNum++;
		dwVtxNum+=5;
	}
	ADD_INSTR(OPCODE_ENDRENDER);
	// output number of instructions
	WriteDataDWORD(dwNumIns);
	// output instructions
	WriteDataBytes((LPBYTE)lpIns,dwNumIns*sizeof(WORD));
	// free memory
	SAFE_FREE(lpIns);
}

#endif

// END ///////////////////////////////////////////////////

