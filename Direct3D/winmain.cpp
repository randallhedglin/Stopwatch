// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  winmain.cpp									//
//														//
//  Handles necessary Windows tasks for this Direct3D	//
//	application.  Fill in blanks in "wininfo.h" to		//
//  allow this module to function.						//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// header for this module
#include"winmain.h"
// required for window creation
#include"..\source\wininfo.h"
// allow access to main program functions
#include"mainprog.h"
// allow access to DirectDraw functions
#include"ddfunc.h"

// ERROR MESSAGES ////////////////////////////////////////

// register window class error
#define ERROR_WINMAIN00 DISPLAY_FATAL_ERROR(g_hWnd,"Error in module: WINMAIN.CPP\nError Code: 00\n\nUnable to register window class.");
// window creation error
#define ERROR_WINMAIN01 DISPLAY_FATAL_ERROR(g_hWnd,"Error in module: WINMAIN.CPP\nError Code: 01\n\nUnable to create main window.");

// PROTOTYPES ////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hWnd,
							UINT Msg,
							WPARAM wParam,
							LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow);

// GLOBALS ///////////////////////////////////////////////

// stores handle to application instance
HINSTANCE g_hInst=NULL;
// stores handle to main window
HWND g_hWnd=NULL;
// used for resource tracking
DWORD __g_dwResources=0;
// trig tables
D3DVALUE __g_dvSinTab[256];
D3DVALUE __g_dvCosTab[256];
// random float table
float __g_fRandTab[NUM_RANDOM_NUMBERS];
// random float table pointer
DWORD __g_dwFRandTabPtr=0;
// random long table
long __g_lRandTab[NUM_RANDOM_NUMBERS];
// random long table pointer
DWORD __g_dwLRandTabPtr=0;

// CONSTANTS /////////////////////////////////////////////

// unique class name (actually a GUID in string form)
#define STR_APPCLASS "{8916BA00-8FA9-11d4-ACC2-52544C046578}"

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//  Function:  WindowProc()								//
//														//
//  Handles window messages.							//
//	  Parameters:										//
//		HWND hWnd: handle to window recieving message	//
//		UINT Msg: message value							//
//		WPARAM wParam: WPARAM value of message			//
//		LPARAM lParam: LPARAM value of message			//
//	  Return Value:										//
//		This is a Windows callback function.  Returning //
//		a value of 0 signifies that the message was		//
//		handled.										//
//														//
//////////////////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hWnd,
							UINT Msg,
							WPARAM wParam,
							LPARAM lParam)
{
	// check to see which message was sent
	switch(Msg)
	{
	// prevent screen saver activation
	case(WM_SYSCOMMAND):
		// check for screen saver or monitor shutdown
		if(wParam&0xFFF0==SC_SCREENSAVE||
		   wParam&0xFFF0==SC_MONITORPOWER)
		    // go ahead! turn it off! i dare ya!
			return(0);
		// we don't care what it was, so let Windows take care of it
		return(DefWindowProc(hWnd,Msg,wParam,lParam));
	// process close message
	case(WM_CLOSE):
		// destroy window
		PostMessage(hWnd,WM_DESTROY,NULL,NULL);
		// return to Windows
		return(0);
	// process destroy message
	case(WM_DESTROY):
		// quit application
		PostQuitMessage(NULL);
		// return to Windows
		return(0);
	// process default message
	default:
		// if the message wasn't one of the above, then
		// we don't care what the message was, so let
		// Windows handle it
		return(DefWindowProc(hWnd,Msg,wParam,lParam));
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//  Function:  WinMain()								//
//														//
//  Primary execution loop for all Windows programs.	//
//	  Parameters:										//
//		HINSTANCE hInstance: handle to this application //
//							 instance					//
//		HINSTANCE hPrevInstance: handle to previous		//
//								 application instance	//
//		LPSTR lpCmdLine: value of command line			//
//		int nCmdShow: Windows show command (maximized,	//
//					  minimized, etc.)					//
//	  Return Value:										//
//		This is a Windows callback function.  Returning //
//		a value of 0 signifies that the application has	//
//		completed.										//
//														//
//////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	// stores data for window class
	WNDCLASSEX WndClassEx;
	INIT_VAR(WndClassEx);
	// used for message processing
	MSG Msg;
	INIT_VAR(Msg);
	// flag to prevent multiple restores
	BOOL bClosingApp=FALSE;
	// float counter
	D3DVALUE dvCount=0;
	// handle to set window priority
	HANDLE hProc=NULL;
	// DWORD counter
	DWORD dwCount=0;
	
	// store global HINSTANCE for later reference
	g_hInst=hInstance;
	// set size
	WndClassEx.cbSize=sizeof(WNDCLASSEX);
	// set window style	
	WndClassEx.style=CS_DBLCLKS|
					 CS_OWNDC|
					 CS_HREDRAW|
					 CS_VREDRAW;
	// set window proc to our own WindowProc()
	WndClassEx.lpfnWndProc=WindowProc;
	// set window HINST
	WndClassEx.hInstance=hInstance;
	// load icon if specified...
	if(ID_APPICON)
	{
		WndClassEx.hIcon=LoadIcon(hInstance,ID_APPICON);
		WndClassEx.hIconSm=LoadIcon(hInstance,ID_APPICON);
	}
	// otherwise load default icon
	else
	{
		WndClassEx.hIcon=LoadIcon(NULL,IDI_APPLICATION);
		WndClassEx.hIconSm=LoadIcon(NULL,IDI_APPLICATION);
	}
	// load cursor if specified...
	if(ID_APPCURSOR)
		WndClassEx.hCursor=LoadCursor(hInstance,ID_APPCURSOR);
	// otherwise load default cursor
	else
		WndClassEx.hCursor=LoadCursor(NULL,IDC_ARROW);
	// set background brush
	WndClassEx.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	// set menu name
	WndClassEx.lpszMenuName=ID_APPMENU;
	// set class name
	WndClassEx.lpszClassName=STR_APPCLASS;
	// register class
	if(!RegisterClassEx(&WndClassEx))
	{
		ERROR_WINMAIN00;
		return(0);
	}
	// create window
	g_hWnd=CreateWindowEx(NULL,
						  STR_APPCLASS,
						  STR_APPTITLE,
						  WS_POPUP|WS_VISIBLE,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);
	// check for window creation errors
	if(!g_hWnd)
	{
		ERROR_WINMAIN01;
		return(0);
	}
	// call initialization function
	if(!__InitializeSystem())
	{
		// problem encountered, restore system
		__RestoreSystem();
		// destroy window
		DestroyWindow(g_hWnd);
		return(0);
	}
	// create trig tables
	for(dvCount=0;dvCount<256;dvCount++)
	{
		__g_dvSinTab[(int)dvCount]=sinf(dvCount*0.02454369260617F);
		__g_dvCosTab[(int)dvCount]=cosf(dvCount*0.02454369260617F);
	}
	// create random float table
	for(dwCount=0;dwCount<NUM_RANDOM_NUMBERS;dwCount++)
	{
		// get random number
		__g_fRandTab[dwCount]=((float)((rand()%10000)+1))/10000;
		// randomly make negative
		if(rand()&1)
			__g_fRandTab[dwCount]=-__g_fRandTab[dwCount];
	}
	// create random long table
	for(dwCount=0;dwCount<NUM_RANDOM_NUMBERS;dwCount++)
		__g_lRandTab[dwCount]=(long)rand();
	// set our process to #1 priority
	hProc=GetCurrentProcess();
	SetPriorityClass(hProc,REALTIME_PRIORITY_CLASS);
	// begin program loop
	while(TRUE)
	{
		// check for pending messages
		if(PeekMessage(&Msg,g_hWnd,0,0,PM_REMOVE))
		{
			// if we are quitting, do so now
			if(Msg.message==WM_QUIT)
				break;
			// otherwise keep going
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		// call main loop if not closing
		if(!bClosingApp)
			if(!__MainProgramLoop())
			{
				// we are now closing
				bClosingApp=TRUE;
				// call restore function
				__RestoreSystem();
				// check for unreleased resources
				if(__g_dwResources)
				{
					// text
					char lpText[256];

					// create message
					sprintf(lpText,"ResourceTracker Info:\nThis application has left %d resources open.\nYour system may become unstable.",__g_dwResources);
					DISPLAY_INFO(GetGlobalHWnd(),lpText);
				}
				// send close message
				PostMessage(GetGlobalHWnd(),
							WM_CLOSE,
							NULL,
							NULL);
			}
	}
	// adios, sayonara, and bye-bye!
	return(Msg.wParam);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalHInst()							//
//	  (defined in "winmain.h")							//
//														//
//  Retrieves handle to application instance.			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		HINSTANCE to application instance					//
//														//
//////////////////////////////////////////////////////////

extern HINSTANCE inline GetGlobalHInst(void)
{
	return(g_hInst);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalHWnd()							//
//	  (defined in "winmain.h")							//
//														//
//  Retrieves handle to main window.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		HWND to main window								//
//														//
//////////////////////////////////////////////////////////

extern HWND inline GetGlobalHWnd(void)
{
	return(g_hWnd);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//  Function:  SwapPointers()							//
//		(defined in "primary.h")						//
//														//
//  Switches two pointers.								//
//	  Parameters:										//
//		LPVOID* lpA: first pointer						//
//		LPVOID* lpB: second pointer						//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SwapPointers(LPVOID* lpA,LPVOID* lpB)
{
	// temporary pointer
	LPVOID lpC;

	// swap
	lpC=*lpA;
	*lpA=*lpB;
	*lpB=lpC;
}

//////////////////////////////////////////////////////////
//														//
//  Function:  __AddResource()							//
//		(defined in "primary.h")						//
//														//
//  Used internally for resource tracking.				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void __AddResource(void)
{
	__g_dwResources++;
}

//////////////////////////////////////////////////////////
//														//
//  Function:  __SubtractResource()						//
//		(defined in "primary.h")						//
//														//
//  Used internally for resource tracking.				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void __SubtractResource(void)
{
	__g_dwResources--;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//  Function:  GetSin()									//
//		(defined in "primary.h")						//
//														//
//  Retrieve sine from lookup table.					//
//	  Parameters:										//
//		BYTE: angle (1/256ths of a circle)				//
//	  Return Value:										//
//		D3DVALUE: sine									//
//														//
//////////////////////////////////////////////////////////

extern D3DVALUE inline GetSin(BYTE cVal)
{
	return(__g_dvSinTab[cVal]);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//  Function:  GetCos()									//
//		(defined in "primary.h")						//
//														//
//  Retrieve cosine from lookup table.					//
//	  Parameters:										//
//		BYTE: angle (1/256ths of a circle)				//
//	  Return Value:										//
//		D3DVALUE: cosine								//
//														//
//////////////////////////////////////////////////////////

extern D3DVALUE inline GetCos(BYTE cVal)
{
	return(__g_dvCosTab[cVal]);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  DisplayError()							//
//		(defined in "primary.h")						//
//														//
//  Display an error message window.					//
//	  Parameters:										//
//		HWND: handle to parent window					//
//		LPCSTR: text									//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void DisplayError(HWND hWnd,
						 LPCSTR lpText)
{
	// set GDI surface
	if(GetGlobalDirectDrawPtr())
		GetGlobalDirectDrawPtr()->FlipToGDISurface();
	// display cursor
	ShowCursor(TRUE);
	// display window
	MessageBox(hWnd,
			   lpText,
			   "Error",
			   MB_OK|MB_ICONINFORMATION);
	// remove cursor
	ShowCursor(FALSE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  DisplayFatalError()						//
//		(defined in "primary.h")						//
//														//
//  Display a fatal error message window.				//
//	  Parameters:										//
//		HWND: handle to parent window					//
//		LPCSTR: text									//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void DisplayFatalError(HWND hWnd,
							  LPCSTR lpText)
{	
	// set GDI surface
	if(GetGlobalDirectDrawPtr())
		GetGlobalDirectDrawPtr()->FlipToGDISurface();
	// display cursor
	ShowCursor(TRUE);
	// display window
	MessageBox(hWnd,
			   lpText,
			   "Fatal Error",
			   MB_OK|MB_ICONSTOP);
	// remove cursor
	ShowCursor(FALSE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  DisplayInfo()							//
//		(defined in "primary.h")						//
//														//
//  Display an information message window.				//
//	  Parameters:										//
//		HWND: handle to parent window					//
//		LPCSTR: text									//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void DisplayInfo(HWND hWnd,
						LPCSTR lpText)
{
	// set GDI surface
	if(GetGlobalDirectDrawPtr())
		GetGlobalDirectDrawPtr()->FlipToGDISurface();
	// display cursor
	ShowCursor(TRUE);
	// display window
	MessageBox(hWnd,
			   lpText,
			   "Info",
			   MB_OK|MB_ICONINFORMATION);
	// remove cursor
	ShowCursor(FALSE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetRandomFloat()							//
//		(defined in "primary.h")						//
//														//
//  Returns a random number between -1 and 1			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		float: number									//
//														//
//////////////////////////////////////////////////////////

extern float GetRandomFloat(void)
{
	// increment pointer
	__g_dwFRandTabPtr++;
	// check for overflow
	if(__g_dwFRandTabPtr>=NUM_RANDOM_NUMBERS)
		__g_dwFRandTabPtr=0;
	// return random float
	return(__g_fRandTab[__g_dwFRandTabPtr]);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetRandomLong()							//
//		(defined in "primary.h")						//
//														//
//  Returns a long random number						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		long: number									//
//														//
//////////////////////////////////////////////////////////

extern long GetRandomLong(void)
{
	// increment pointer
	__g_dwLRandTabPtr++;
	// check for overflow
	if(__g_dwLRandTabPtr>=NUM_RANDOM_NUMBERS)
		__g_dwLRandTabPtr=0;
	// return random long
	return(__g_lRandTab[__g_dwLRandTabPtr]);
}

// END ///////////////////////////////////////////////////

