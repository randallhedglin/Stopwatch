// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  primary.h									//
//														//
//  Contains #include commands for use in all modules	//
//	of this application.  Also includes some useful		//
//	macros.												//
//														//
//////////////////////////////////////////////////////////

// flag telling us we are in debug mode (omit from release version)
//#define DEBUG_MODE

// flag to include data encoding (omit from release version)
//#define ENCODE_DATA

// flag indicating whether to use my inline assembly or not
#define USE_ASSEMBLER

// MFC bad!
#define WIN32_LEAN_AND_MEAN
// STRICT good!
#define STRICT
// Direct3D made easy
#define D3D_OVERLOADS

// miscellaneous headers
#include<objbase.h>
#include<windows.h>
#include<windowsx.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<malloc.h>
#include<ddraw.h>
#include<d3d.h>
#include<d3dtypes.h>
#include<d3dcaps.h>
#include<dinput.h>
#include<mmreg.h>
#include<dsound.h>
#include<wchar.h>
#include<dmdls.h>
#include<dmerror.h>
#include<dmksctrl.h>
#include<dmusici.h>
#include<dmusicc.h>
#include<dmusicf.h>
#include<direct.h>

// PROTOTYPES ////////////////////////////////////////////

extern void __AddResource(void);

extern void __SubtractResource(void);

extern D3DVALUE inline GetSin(BYTE cVal);

extern D3DVALUE inline GetCos(BYTE cVal);

extern void SwapPointers(LPVOID* lpA,
						 LPVOID* lpB);

extern void DisplayError(HWND hWnd,
						 LPCSTR lpText);

extern void DisplayFatalError(HWND hWnd,
							  LPCSTR lpText);

extern void DisplayInfo(HWND hWnd,
						LPCSTR lpText);

extern float GetRandomFloat(void);

extern long GetRandomLong(void);

// MACROS ////////////////////////////////////////////////

#define INIT_STRUCT(a) { ZeroMemory(&a,sizeof(a)); a.dwSize=sizeof(a); }

#define INIT_VAR(a) { ZeroMemory(&a,sizeof(a)); }

#define DISPLAY_ERROR(a,b) { DisplayError(a,b); }

#define DISPLAY_FATAL_ERROR(a,b) { DisplayFatalError(a,b); }

#define DISPLAY_INFO(a,b) { DisplayInfo(a,b); }

#define SAFE_RELEASE(a) { if(a) { a->Release(); a=NULL; } }

#define SAFE_CLOSE(a) { if(a!=INVALID_HANDLE_VALUE) if(CloseHandle(a)) a=INVALID_HANDLE_VALUE; }

#define SAFE_ALLOC(a,b,c) { a=(b)malloc(c); if(!a) DISPLAY_ERROR(GetGlobalHWnd(),"General Error: 00\nUnable to allocate memory.") else { ZeroMemory((LPVOID)a,c); __AddResource(); } }

#define SAFE_FREE(a) { if(a) { free(a); a=NULL; __SubtractResource(); } }

#define INIT_TIMER static DWORD __dwTickCount=GetTickCount()+16; static DWORD __dwFrameSkip=1; static DWORD __dwCarryOver=0;

#define GET_SKIP __dwFrameSkip

#define EXEC_TIMER if(GetTickCount()<__dwTickCount) { __dwFrameSkip=1; __dwCarryOver=0; while(GetTickCount()<__dwTickCount) { } } else { __dwCarryOver+=GetTickCount()-__dwTickCount; __dwFrameSkip=1; while(__dwCarryOver>=16) { __dwCarryOver-=16; __dwFrameSkip++; } } __dwTickCount=GetTickCount()+16;

#define INIT_FPS static DWORD __dwFrameNo=0; static DWORD __dwFPS=0; static DWORD __dwFPSTickCount=GetTickCount()+1000;

#define EXEC_FPS __dwFrameNo++; if(GetTickCount()>__dwFPSTickCount) { __dwFPS=__dwFrameNo; __dwFrameNo=0; __dwFPSTickCount=GetTickCount()+1000; }

#define GET_FPS __dwFPS

#define SAFE_NEW(a,b) { a=new b(); if(a) __AddResource(); else DISPLAY_ERROR(GetGlobalHWnd(),"General Error: 01\nUnable to create new class instance.") }

#define SAFE_DELETE(a) { if(a) { delete a; a=NULL; __SubtractResource(); } }

#define DEG_TO_RAD(a) (a*0.017453292F)

// scale factor & reciprocal
#define SF 0.01F
#define SF1 100.0F

#define NUM_RANDOM_NUMBERS 256

#define MAKE_NONZERO(a) { if(a>=0&&a<0.00000001F) a=0.00000001F; else if(a<0&&a>-0.00000001F) a=-0.00000001F; }

// END ///////////////////////////////////////////////////

