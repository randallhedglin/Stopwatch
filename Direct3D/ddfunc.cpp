// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  ddfunc.cpp									//
//														//
//	Functions for handling DirectDraw.					//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// header for this module
#include"ddfunc.h"
// allow access to window handles
#include"winmain.h"
// allow access to Direct3D
#include"d3dfunc.h"
// allow access to material functions
#include"matfunc.h"

// MESSAGES //////////////////////////////////////////////

// selecting default DirectDraw device
#define MSG_DDFUNC00 DISPLAY_INFO(GetGlobalHWnd(),"Only one valid DirectDraw device was found.\nThis device will be selected by default.");
// selecting default Direct3D device
#define MSG_DDFUNC01 DISPLAY_INFO(GetGlobalHWnd(),"Only one valid Direct3D device was found.\nThis device will be selected by default.");
// couldn't retrieve pixel format
#define MSG_DDFUNC02 DISPLAY_INFO(GetGlobalHWnd(),"Unable to retrieve the selected DirectDraw device's pixel format.\nGraphics may display incorrectly.");
// couldn't retrieve z-buffer pixel format
#define MSG_DDFUNC03 DISPLAY_INFO(GetGlobalHWnd(),"Unable to retrieve the selected Direct3D device's z-buffer pixel format.\nGraphics may display incorrectly.");
// couldn't create z-buffer
#define MSG_DDFUNC04 DISPLAY_INFO(GetGlobalHWnd(),"Unable to create Direct3D z-buffer.\nGraphics may display incorrectly.");
// couldn't attach z-buffer
#define MSG_DDFUNC05 DISPLAY_INFO(GetGlobalHWnd(),"Unable to attach Direct3D z-buffer.\nGraphics may display incorrectly.");
// couldn't enable color keying
#define MSG_DDFUNC06 DISPLAY_INFO(GetGlobalHWnd(),"Unable to set color keying.\nGraphics may display incorrectly.");

// ERRORS ////////////////////////////////////////////////

// DirectDraw enumeration failed
#define ERR_DDFUNC00 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 00\nUnable to enumerate DirectDraw devices.");
// no valid DirectDraw devices found
#define ERR_DDFUNC01 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 01\nValid DirectDraw device not found.");
// DirectDrawCreate failed
#define ERR_DDFUNC02 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 02\nUnable to create DirectDraw interface for the selected device.");
// query IDirectDraw4 interface  failed
#define ERR_DDFUNC03 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 03\nUnable to create DirectDraw4 interface for the selected device.");
// set cooperative level failed
#define ERR_DDFUNC04 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 04\nUnable to set required DirectDraw cooperative level.");
// set display mode failed
#define ERR_DDFUNC05 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 05\nUnable to set selected display mode.");
// Direct3D enumeration failed
#define ERR_DDFUNC06 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 06\nUnable to enumerate Direct3D devices.");
// no valid Direct3D devices found
#define ERR_DDFUNC07 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 07\nValid Direct3D device not found.");
// failed to init Direct3D
#define ERR_DDFUNC08 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 08\nUnable to create Direct3D interface.");
// failed to init front buffer
#define ERR_DDFUNC09 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 09\nUnable to create primary display surface.");
// failed to find back buffer
#define ERR_DDFUNC10 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 10\nUnable to find secondary display surface.");
// failed to create clipper
#define ERR_DDFUNC11 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 11\nUnable to create DirectDrawClipper interface.");
// failed to set clipper
#define ERR_DDFUNC12 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 12\nUnable to install DirectDrawClipper interface.");
// failed to create Direct3DDevice
#define ERR_DDFUNC13 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 13\nUnable to initialize selected Direct3D device.");
// failed to create viewport
#define ERR_DDFUNC14 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 14\nUnable to create Direct3D viewport.");
// failed to add viewport
#define ERR_DDFUNC15 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 15\nUnable to add viewport to Direct3D device.");
// failed to set viewport
#define ERR_DDFUNC16 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 16\nUnable to set viewport parameters.");
// failed to set current viewport
#define ERR_DDFUNC17 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 17\nUnable to set current Direct3D viewport.");
// failed to lock front buffer
#define ERR_DDFUNC18 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 18\nUnable to lock primary display surface.");
// failed to lock back buffer
#define ERR_DDFUNC19 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 19\nUnable to lock secondary display surface.");
// failed to unlock front buffer
#define ERR_DDFUNC20 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 20\nUnable to unlock primary display surface.");
// failed to unlock back buffer
#define ERR_DDFUNC21 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 21\nUnable to unlock secondary display surface.");
// failed to clear front buffer
#define ERR_DDFUNC22 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 22\nUnable to clear primary display surface.");
// failed to clear back buffer
#define ERR_DDFUNC23 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 23\nUnable to clear secondary display surface.");
// failed to attach clipper
#define ERR_DDFUNC24 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 24\nUnable to attach DirectDraw clipper object.");
// failed to set fog state
#define ERR_DDFUNC25 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 25\nUnable to set fog state.");
// falied to set refernece alpha value
#define ERR_DDFUNC26 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 26\nUnable to set reference alpha value.");
// falied to set alpha compare function
#define ERR_DDFUNC27 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 27\nUnable to set alpha compare function.");
// falied to set source blend value
#define ERR_DDFUNC28 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 28\nUnable to set alpha source value.");
// falied to set destination blend value
#define ERR_DDFUNC29 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 29\nUnable to set alpha destination value.");
// falied to render overlay
#define ERR_DDFUNC30 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 30\nUnable to render screen overlay.");
// falied to enumerate alpha formats
#define ERR_DDFUNC31 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 31\nUnable to enumerate alpha texture formats.");
// falied to find alpha format
#define ERR_DDFUNC32 DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DDFUNC.CPP\nError Code: 32\nUnable to locate suitable alpha texture formats.");

// PROTOTYPES ////////////////////////////////////////////

BOOL WINAPI EnumDirectDrawCallback(LPGUID lpGUID,
								   LPSTR lpDeviceDesc,
								   LPSTR lpDeviceName,
								   LPVOID lpUserArg);

BOOL WINAPI EnumDirect3DCallback(LPGUID lpGUID,
								 LPSTR lpDeviceDesc,
								 LPSTR lpDeviceName,
								 LPD3DDEVICEDESC lpdevHardware,
								 LPD3DDEVICEDESC lpdevSoftware,
								 LPVOID lpUserArg);

BOOL WINAPI EnumZBufferCallback(LPDDPIXELFORMAT lpDDPF,
								LPVOID lpUserArg);

BOOL WINAPI EnumAlphaTextureCallback(LPDDPIXELFORMAT lpDDPF,
									 LPVOID lpUserArg);

// STRUCTURES ////////////////////////////////////////////

// DirectDraw device info
typedef struct
{
	// device GUID
	GUID guidDeviceGUID;
	// device description
	char lpDeviceDesc[81];
	// device name
	char lpDeviceName[81];
	// hardware capabilities
	DDCAPS ddcapsHAL;
	// software capabilities
	DDCAPS ddcapsHEL;
	// default driver flag
	BOOL bIsDefault;
} DDDEVICE,*DDDEVICE_PTR;

// DirectDraw device list
typedef struct
{
	// pointer to array of DDDEVICEs
	DDDEVICE_PTR DDDevicePtr;
	// number of devices in list
	DWORD dwNumDevices;
} DDDEVICELIST,*DDDEVICELIST_PTR;

// Direct3D device info
typedef struct
{
	// device GUID
	GUID guidDeviceGUID;
	// device description
	char lpDeviceDesc[81];
	// device name
	char lpDeviceName[81];
	// hardware capabilities
	D3DDEVICEDESC devHardware;
	// software capabilities
	D3DDEVICEDESC devSoftware;
	// hardware flag
	BOOL bIsHardware;
	// z-buffer flag
	BOOL bUseZBuffer;
} D3DDEVICE,*D3DDEVICE_PTR;

// Direct3D device list
typedef struct
{
	// pointer to array of D3DDEVICEs
	D3DDEVICE_PTR D3DDevicePtr;
	// number of devices in list
	DWORD dwNumDevices;
} D3DDEVICELIST,*D3DDEVICELIST_PTR;

// GLOBALS ///////////////////////////////////////////////

// global pointer to secondary DirectDrawSurface interface
LPDIRECTDRAWSURFACE4 g_lpBackBuffer=NULL;
// global pointer to primary DirectDrawSurface interface
LPDIRECTDRAWSURFACE4 g_lpFrontBuffer=NULL;
// global pointer to Direct3DDevice interface
LPDIRECT3DDEVICE3 g_lpD3DDevice=NULL;
// index for screen overlay
WORD g_lpIndex[6];
// global DirectDraw device default flag
BOOL g_bIsDefault=FALSE;
// global hi-res flag
BOOL g_bHiRes=FALSE;
// array of vertices for screen overlay
D3DTLVERTEX g_lpOverlayVerts[4];
// global DirectDraw device GUID
GUID g_guidDDDevice;
// global Direct3D device GUID
GUID g_guidD3DDevice;
// global pointer to DirectDraw interface
LPDIRECTDRAW4 g_lpDD=NULL;
// global Direct3D device hardware flag
BOOL g_bIsHardware=FALSE;
// global Direct3D device z-buffer required flag
BOOL g_bUseZBuffer=FALSE;
// global use triple buffering flag
BOOL g_bUseTripleBuffer=FALSE;
// global pointer to Direct3D interface
LPDIRECT3D3 g_lpD3D=NULL;
// global flag to use 15-bit data instead of 16-bit
BOOL g_bUse15Bit=FALSE;
// global pointer to DirectDrawClipper interface
LPDIRECTDRAWCLIPPER g_lpClipper=NULL;
// global pointer to z-buffer DirectDrawSurface interface
LPDIRECTDRAWSURFACE4 g_lpZBuffer=NULL;
// global pointer to viewport interface
LPDIRECT3DVIEWPORT3 g_lpViewport=NULL;
// global pointer to palette array
LPPALETTEENTRY g_lpPalette;
// global 3d screen coordinates
D3DRECT g_rScreen;
// global 2d screen coordinates
RECT g_rScreen2;
// global 15 bit color values
WORD g_wPal15[256];
// global 16 bit color values
WORD g_wPal16[256];
// alpha texture pixel format
DDPIXELFORMAT g_ddpfAlpha;
// viewport size flag
BOOL g_bViewportSize=NULL;

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//  Function:  EnumDirectDrawCallback()					//
//														//
//  Enumerates all DirectDraw devices.					//
//	  Parameters:										//
//		LPGUID lpGUID: GUID of device					//
//		LPSTR lpDeviceDesc: description of device		//
//		LPSTR lpDeviceName: name of device				//
//		LPVOID lpUserArg: user supplied data			//
//	  Return Value:										//
//		This is a DirectDraw callback function.  Return	//
//		DDENUMRET_OK to continue or DDENUMRET_CANCEL	//
//		to stop enumeration.							//
//														//
//////////////////////////////////////////////////////////

BOOL WINAPI EnumDirectDrawCallback(LPGUID lpGUID,
								   LPSTR lpDeviceDesc,
								   LPSTR lpDeviceName,
								   LPVOID lpUserArg)
{
	// temporary DirectDraw interface
	LPDIRECTDRAW lpDDTemp=NULL;
	// pointer to our data structure
	DDDEVICELIST_PTR DDDeviceListPtr=(DDDEVICELIST_PTR)lpUserArg;
	// pointer to current device
	DDDEVICE_PTR DDDevicePtr=NULL;
	// hardware capabilities
	DDCAPS ddcapsHAL;
	// software capabilities
	DDCAPS ddcapsHEL;

	// ensure valid device
	if(FAILED(DirectDrawCreate(lpGUID,
							   &lpDDTemp,
							   NULL)))
		return(DDENUMRET_OK);
	// retrieve capabilities
	INIT_STRUCT(ddcapsHAL);
	INIT_STRUCT(ddcapsHEL);
	if(FAILED(lpDDTemp->GetCaps(&ddcapsHAL,
								&ddcapsHEL)))
	{
		// no good
		SAFE_RELEASE(lpDDTemp);
		return(DDENUMRET_OK);
	}
	// no more need for DirectDraw
	SAFE_RELEASE(lpDDTemp);
	// create storage structure if necessary
	if(DDDeviceListPtr->dwNumDevices==0)
	{
		// allocate memory
		SAFE_ALLOC(DDDeviceListPtr->DDDevicePtr,
				   DDDEVICE_PTR,
				   sizeof(DDDEVICE));
		// check for errors
		if(!DDDeviceListPtr->DDDevicePtr)
			return(DDENUMRET_OK);
		// set pointer to device data
		DDDevicePtr=DDDeviceListPtr->DDDevicePtr;
	}
	// otherwise expand storage structure
	else
	{
		// temporary storage
		LPVOID lpTemp=NULL;

		// allocate memory
		SAFE_ALLOC(lpTemp,
				   LPVOID,
				   sizeof(DDDEVICE)*(DDDeviceListPtr->dwNumDevices+1));
		// check for errors
		if(!lpTemp)
			return(DDENUMRET_OK);
		// swap pointers
		SwapPointers(&lpTemp,
					 (LPVOID*)&DDDeviceListPtr->DDDevicePtr);
		// copy data
		CopyMemory(DDDeviceListPtr->DDDevicePtr,
				   lpTemp,
				   sizeof(DDDEVICE)*DDDeviceListPtr->dwNumDevices);
		// out with the old!
		SAFE_FREE(lpTemp);
		// set pointer to device data
		DDDevicePtr=DDDeviceListPtr->DDDevicePtr;
	}
	// if GUID is non-default, save it
	if(lpGUID)
	{
		CopyMemory(&DDDevicePtr[DDDeviceListPtr->dwNumDevices].guidDeviceGUID,
				   lpGUID,
				   sizeof(GUID));
		DDDevicePtr[DDDeviceListPtr->dwNumDevices].bIsDefault=FALSE;
	}
	// otherwise GUID is invalid and not needed
	else
		DDDevicePtr[DDDeviceListPtr->dwNumDevices].bIsDefault=TRUE;
	// save device description
	CopyMemory(DDDevicePtr[DDDeviceListPtr->dwNumDevices].lpDeviceDesc,
			   lpDeviceDesc,
			   80);
	DDDevicePtr[DDDeviceListPtr->dwNumDevices].lpDeviceDesc[80]=0;
	// save device name
	CopyMemory(DDDevicePtr[DDDeviceListPtr->dwNumDevices].lpDeviceName,
			   lpDeviceName,
			   80);
	DDDevicePtr[DDDeviceListPtr->dwNumDevices].lpDeviceName[80]=0;
	// save HAL caps
	CopyMemory(&DDDevicePtr[DDDeviceListPtr->dwNumDevices].ddcapsHAL,
			   &ddcapsHAL,
			   sizeof(DDCAPS));
	// save HEL caps
	CopyMemory(&DDDevicePtr[DDDeviceListPtr->dwNumDevices].ddcapsHEL,
			   &ddcapsHEL,
			   sizeof(DDCAPS));
	// increment counter
	DDDeviceListPtr->dwNumDevices++;
	// keep going
	return(DDENUMRET_OK);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  SelectDDDevice()							//
//	  (defined in "ddfunc.h")							//
//														//
//  Detects valid DirectDraw devices.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL SelectDDDevice(void)
{
	// our storage structure
	DDDEVICELIST DDDeviceList;
	INIT_VAR(DDDeviceList);
	// used for selection window
	LPSTR lpText=NULL;
	// selection
	DWORD dwSelection=0;

	// init global variables
	INIT_VAR(g_guidDDDevice);
	// begin enumeration
	if(FAILED(DirectDrawEnumerate((LPDDENUMCALLBACK)EnumDirectDrawCallback,
								  (LPVOID)&DDDeviceList)))
	{
		ERR_DDFUNC00;
		return(FALSE);
	}
	// check for error
	if(DDDeviceList.dwNumDevices==0)
	{
		ERR_DDFUNC01;
		return(FALSE);
	}
	// if only one, use it
	if(DDDeviceList.dwNumDevices==1)
	{
		// let user know
		MSG_DDFUNC00;
		// select it
		dwSelection=0;
	}
	// otherwise let user pick
	else
	{
		// allocate memory for window text
		SAFE_ALLOC(lpText,
				   LPSTR,
				   (DDDeviceList.dwNumDevices*160)+256);
		// loop until selection is made
		while(TRUE)
		{
			// counter 
			DWORD dwCount=0;
			// temporary text
			char lpTemp[8];
			INIT_VAR(lpTemp);

			// fill in text
			strcpy(lpText,"Please select a DirectDraw device to use:\n\n");
			// add devices
			for(dwCount=0;dwCount<DDDeviceList.dwNumDevices;dwCount++)
			{
				// add arrow to current selection
				if(dwCount==dwSelection)
					strcat(lpText,"--> ");
				// otherwise add spaces
				else
					strcat(lpText,"     ");
				// add number
				sprintf(lpTemp,"%d ",dwCount+1);
				strcat(lpText,lpTemp);
				// add device name
				strcat(lpText,DDDeviceList.DDDevicePtr[(DDDeviceList.dwNumDevices-1)-dwCount].lpDeviceName);
				// add device description
				strcat(lpText," (");
				strcat(lpText,DDDeviceList.DDDevicePtr[(DDDeviceList.dwNumDevices-1)-dwCount].lpDeviceDesc);
				strcat(lpText,")\n");
			}
			// add instructions
			strcat(lpText,"\nClick 'Yes' to accept this choice or 'No' to select the next one.");
			// check for selection
			if(MessageBox(GetGlobalHWnd(),
						  lpText,
						  "Select DirectDraw Device",
						  MB_YESNO)==IDYES)
				break;
			// keep going
			dwSelection++;
			if(dwSelection>=DDDeviceList.dwNumDevices)
				dwSelection=0;			
		}
		// free memory
		SAFE_FREE(lpText);
	}
	// copy information to globals
	dwSelection=(DDDeviceList.dwNumDevices-1)-dwSelection;
	g_guidDDDevice=DDDeviceList.DDDevicePtr[dwSelection].guidDeviceGUID;
	g_bIsDefault=DDDeviceList.DDDevicePtr[dwSelection].bIsDefault;
	// release memory
	SAFE_FREE(DDDeviceList.DDDevicePtr);
	// looks good to me
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalDDDeviceGUID()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Retrieve selected DirectDraw device GUID			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		GUID: selected DirectDraw device GUID			//
//														//
//////////////////////////////////////////////////////////

extern GUID inline GetGlobalDDDeviceGUID(void)
{
	return(g_guidDDDevice);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalDDDefaultFlag()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Retrieve selected DirectDraw device default flag.	//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: selected DirectDraw device default flag	//
//														//
//////////////////////////////////////////////////////////

extern BOOL inline GetGlobalDDDefaultFlag(void)
{
	return(g_bIsDefault);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  SetGlobalDDDeviceGUID()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Set current DirectDraw device GUID					//
//	  Parameters:										//
//		GUID guid: selected DirectDraw device GUID		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void inline SetGlobalDDDeviceGUID(GUID guid)
{
	g_guidDDDevice=guid;
}

//////////////////////////////////////////////////////////
//														//
//  Function:  SetGlobalDDDefaultFlag()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Set current DirectDraw device default flag.			//
//	  Parameters:										//
//		BOOL: selected DirectDraw device default flag	//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void inline SetGlobalDDDefaultFlag(BOOL bFlag)
{
	g_bIsDefault=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//  Function:  SelectResolution()						//
//	  (defined in "ddfunc.h")							//
//														//
//  Allows user to select hi- or low-resoultion			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SelectResolution(void)
{
	while(TRUE)
	{
		if(MessageBox(GetGlobalHWnd(),
					  "Please select screen resolution.\n\n--> 1. High resolution (640x480)\n     2. Low resolution (320x240)\n\nClick 'Yes' to accept this choice or 'No' to select the next one.",
				      "Select Resolution",
				      MB_YESNO)==IDYES)
		{
			g_bHiRes=TRUE;
			break;
		}
		if(MessageBox(GetGlobalHWnd(),
					  "Please select screen resolution.\n\n     1. High resolution (640x480)\n--> 2. Low resolution (320x240)\n\nClick 'Yes' to accept this choice or 'No' to select the next one.",
				      "Select Resolution",
				      MB_YESNO)==IDYES)
		{
			g_bHiRes=FALSE;
			break;
		}
	}
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalResolutionFlag()				//
//	  (defined in "ddfunc.h")							//
//														//
//  Get current DirectDraw resolution flag.				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: value of DirectDraw resolution flag		//
//														//
//////////////////////////////////////////////////////////

extern BOOL inline GetGlobalResolutionFlag(void)
{
	return(g_bHiRes);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  SetGlobalResolutionFlag()				//
//	  (defined in "ddfunc.h")							//
//														//
//  Set current DirectDraw resolution flag.				//
//	  Parameters:										//
//		BOOL: selected DirectDraw resolution flag		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void inline SetGlobalResolutionFlag(BOOL bFlag)
{
	g_bHiRes=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//  Function:  InitializeDirectDraw()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Launch the DirectDraw/3D interface.  All globals	//
//		must be set before calling this function.		//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL InitializeDirectDraw(void)
{
	// temporary DirectDraw interface pointer
	LPDIRECTDRAW lpDDTemp=NULL;
	// surface description
	DDSURFACEDESC2 ddsd;
	// surface capabilities
	DDSCAPS2 ddscaps; 
	// current pixel format
	DDPIXELFORMAT ddpf;
	// pointer to region data for clipper
	LPRGNDATA lpRgnData=NULL;
	// pointer to rectangle data for clipper
	LPRECT lpRect=NULL;
	// viewport data structure
	D3DVIEWPORT2 viewdata;
	// counter
	DWORD dwCount=0;

	// create IDirectDraw interface for specific device
	if(!g_bIsDefault)
	{
		if(FAILED(DirectDrawCreate(&g_guidDDDevice,
								   &lpDDTemp,
								   NULL)))
		{
			ERR_DDFUNC02;
			return(FALSE);
		}
	}
	// or default device
	else
	{
		if(FAILED(DirectDrawCreate(NULL,
								   &lpDDTemp,
								   NULL)))
		{
			ERR_DDFUNC02;
			return(FALSE);
		}
	}
	// get IDirectDraw4 interface
	if(FAILED(lpDDTemp->QueryInterface(IID_IDirectDraw4,
									   (LPVOID*)&g_lpDD)))
	{
		ERR_DDFUNC03;
		// release temporary pointer
		SAFE_RELEASE(lpDDTemp);
		return(FALSE);
	}
	// release temporary pointer
	SAFE_RELEASE(lpDDTemp);
	// initialize Direct3D
	if(FAILED(g_lpDD->QueryInterface(IID_IDirect3D3,
									 (LPVOID*)&g_lpD3D)))
	{
		ERR_DDFUNC08;
		return(FALSE);
	}
	// set cooperative level
	if(FAILED(g_lpDD->SetCooperativeLevel(GetGlobalHWnd(),
										  DDSCL_FULLSCREEN|
										  DDSCL_ALLOWMODEX|
										  DDSCL_EXCLUSIVE|
										  DDSCL_ALLOWREBOOT|
										  DDSCL_FPUSETUP)))
	{
		ERR_DDFUNC04;
		return(FALSE);
	}
	// set high resolution
	if(g_bHiRes)
	{
		if(FAILED(g_lpDD->SetDisplayMode(640,
									     480,
									     16,
									     NULL,
									     NULL)))
		{
			ERR_DDFUNC05;
			return(FALSE);
		}
	}
	// or low resolution
	else
	{
		if(FAILED(g_lpDD->SetDisplayMode(320,
									     240,
									     16,
									     NULL,
									     NULL)))
		{
			ERR_DDFUNC05;
			return(FALSE);
		}
	}
	// prepare primary surface description
	INIT_STRUCT(ddsd);
	// set flags
	ddsd.dwFlags=DDSD_CAPS|
				 DDSD_BACKBUFFERCOUNT;
	// set required caps
	ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE|
						DDSCAPS_FLIP|
						DDSCAPS_3DDEVICE|
						DDSCAPS_COMPLEX;
	// hardware must be in video memory
	if(GetGlobalD3DHardwareFlag())
		ddsd.ddsCaps.dwCaps|=DDSCAPS_VIDEOMEMORY;
	// software must be in system memory
	else	
		ddsd.ddsCaps.dwCaps|=DDSCAPS_SYSTEMMEMORY;
	// set back buffer count
	if(g_bUseTripleBuffer)
		ddsd.dwBackBufferCount=2;
	else
		ddsd.dwBackBufferCount=1;
	// create primary surface
	if(FAILED(g_lpDD->CreateSurface(&ddsd,
								    &g_lpFrontBuffer,
								    NULL)))
	{
		// prepare primary surface description
		INIT_STRUCT(ddsd);
		// set flags
		ddsd.dwFlags=DDSD_CAPS|
					 DDSD_BACKBUFFERCOUNT;
		// set required caps
		ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE|
							DDSCAPS_FLIP|
							DDSCAPS_3DDEVICE|
							DDSCAPS_COMPLEX;
		// hardware must be in video memory
		if(GetGlobalD3DHardwareFlag())
			ddsd.ddsCaps.dwCaps|=DDSCAPS_VIDEOMEMORY;
		// software must be in system memory
		else	
			ddsd.ddsCaps.dwCaps|=DDSCAPS_SYSTEMMEMORY;
		// set back buffer count (double buffer only)
		ddsd.dwBackBufferCount=1;
		// create primary surface
		if(FAILED(g_lpDD->CreateSurface(&ddsd,
										&g_lpFrontBuffer,
										NULL)))
		{
			ERR_DDFUNC09;
			return(FALSE);
		}
	}
	// prepare secondary surface
	ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
	// get secondary surface
	if(FAILED(g_lpFrontBuffer->GetAttachedSurface(&ddscaps,
												  &g_lpBackBuffer)))
	{
		ERR_DDFUNC10;
		return(FALSE);
	}
	// determine 15-bit status
	INIT_STRUCT(ddpf);
	// retrieve pixel format
	if(FAILED(g_lpFrontBuffer->GetPixelFormat(&ddpf)))
		MSG_DDFUNC02;
	// set flag if necessary
	if(ddpf.dwRGBBitCount==15)
		g_bUse15Bit=TRUE;
	// create DirectDrawClipper interface
	if(FAILED(g_lpDD->CreateClipper(NULL,
								    &g_lpClipper,
								    NULL)))
	{
		ERR_DDFUNC11;
		return(FALSE);
	}
	// set up region data
	SAFE_ALLOC(lpRgnData,
			   LPRGNDATA,
			   sizeof(RGNDATAHEADER)+sizeof(RECT));
	// check for errors
	if(!lpRgnData)
		return(FALSE);
	// fill in structure
	lpRgnData->rdh.dwSize=sizeof(RGNDATAHEADER);
	// indicate rectangles
	lpRgnData->rdh.iType=RDH_RECTANGLES;
	// the only rectangle will be the entire display
	lpRgnData->rdh.nCount=1;
	// set size of RECT
	lpRgnData->rdh.nRgnSize=sizeof(RECT);
	// set origin coordinates
	lpRgnData->rdh.rcBound.left=0;
	lpRgnData->rdh.rcBound.top=0;
	g_rScreen.x1=0;
	g_rScreen.y1=0;
	g_rScreen2.left=0;
	g_rScreen2.top=0;
	// set coordinates for high resolution
	if(g_bHiRes)
	{
		lpRgnData->rdh.rcBound.right=640;
		lpRgnData->rdh.rcBound.bottom=480;
		g_rScreen.x2=640;
		g_rScreen.y2=480;
		g_rScreen2.right=640;
		g_rScreen2.bottom=480;
	}
	// or low resoultion
	else
	{
		lpRgnData->rdh.rcBound.right=320;
		lpRgnData->rdh.rcBound.bottom=240;
		g_rScreen.x2=320;
		g_rScreen.y2=240;
		g_rScreen2.right=320;
		g_rScreen2.bottom=240;
	}
	// set rectangle pointer
	lpRect=(LPRECT)lpRgnData->Buffer;
	// fill in rectangle
	lpRect->left=0;
	lpRect->top=0;
	lpRect->right=lpRgnData->rdh.rcBound.right;
	lpRect->bottom=lpRgnData->rdh.rcBound.bottom;
	// set clip list
	if(FAILED(g_lpClipper->SetClipList(lpRgnData,NULL)))
	{
		ERR_DDFUNC12;
		// release memory
		SAFE_FREE(lpRgnData);
		return(FALSE);
	}
	// release memory
	SAFE_FREE(lpRgnData);
	// attach clipper
	if(FAILED(g_lpBackBuffer->SetClipper(g_lpClipper)))
	{
		ERR_DDFUNC24;
		return(FALSE);
	}
	// set up z-buffer if necessary
	if(g_bUseZBuffer)
	{
		// find a valid z-buffer format
		if(FAILED(g_lpD3D->EnumZBufferFormats(g_guidD3DDevice,
											  (LPD3DENUMPIXELFORMATSCALLBACK)EnumZBufferCallback,
											  &ddpf)))
			MSG_DDFUNC02;
		// make sure it's good
		if(ddpf.dwSize!=sizeof(DDPIXELFORMAT))
			MSG_DDFUNC02;
		// fill in z-buffer surface description
		INIT_STRUCT(ddsd);
		// set flags
		ddsd.dwFlags=DDSD_WIDTH|
					 DDSD_HEIGHT|
					 DDSD_CAPS|
					 DDSD_PIXELFORMAT;
		// indicate z-buffer
		ddsd.ddsCaps.dwCaps=DDSCAPS_ZBUFFER;
		// set hi-res size
		if(g_bHiRes)
		{
			ddsd.dwWidth=640;
			ddsd.dwHeight=480;
		}
		// or low-res size
		else
		{
			ddsd.dwWidth=320;
			ddsd.dwHeight=240;
		}
		// software rasterizers must use z-buffers in system memory 
		if(!g_bIsHardware)
			ddsd.ddsCaps.dwCaps|=DDSCAPS_SYSTEMMEMORY;
		// copy pixel format
		CopyMemory(&ddsd.ddpfPixelFormat,
				   &ddpf,
				   sizeof(DDPIXELFORMAT));
		// create z-buffer
		if(FAILED(g_lpDD->CreateSurface(&ddsd,
										&g_lpZBuffer,
										NULL)))
			MSG_DDFUNC04;
		// attach z-buffer
		if(FAILED(g_lpBackBuffer->AddAttachedSurface(g_lpZBuffer)))
			MSG_DDFUNC05;
	}
	// create Direct3DDevice
	if(FAILED(g_lpD3D->CreateDevice(g_guidD3DDevice,
									g_lpBackBuffer,
									&g_lpD3DDevice,
									NULL)))
	{
		ERR_DDFUNC13;
		return(FALSE);
	}
 	// create viewport
	if(FAILED(g_lpD3D->CreateViewport(&g_lpViewport,NULL)))
	{
		ERR_DDFUNC14;
		return(FALSE);
	}
	// add viewport to 3d device
	if(FAILED(g_lpD3DDevice->AddViewport(g_lpViewport)))
	{
		ERR_DDFUNC15;
		return(FALSE);
	}
	// set up viewport data
	INIT_STRUCT(viewdata);
	// if hi-res, set size
	if(g_bHiRes)	
	{
		// set viewport size
		switch(g_bViewportSize)
		{
		case(0):
			viewdata.dwX=0;
			viewdata.dwY=0;
			viewdata.dwWidth=640;
			viewdata.dwHeight=480;
			break;
		case(1):
			viewdata.dwX=80;
			viewdata.dwY=60;
			viewdata.dwWidth=480;
			viewdata.dwHeight=320;
			break;
		case(2):
			viewdata.dwX=160;
			viewdata.dwY=120;
			viewdata.dwWidth=320;
			viewdata.dwHeight=240;
			break;
		}
	}
	// same for low res
	else
	{
		viewdata.dwX=0;
		viewdata.dwY=0;
		viewdata.dwWidth=320;
		viewdata.dwHeight=240;
	}
	// set clipping parameters
	viewdata.dvClipX=-1.0F;
	viewdata.dvClipWidth=2.0F;
	viewdata.dvClipY=1.0F;
	viewdata.dvClipHeight=2.0F;
	viewdata.dvMinZ=0.0F;
	viewdata.dvMaxZ=1.0F;
	// set viewport data
	if(FAILED(g_lpViewport->SetViewport2(&viewdata)))
	{
		ERR_DDFUNC16;
		return(FALSE);
	}
	// set as current viewport
	if(FAILED(g_lpD3DDevice->SetCurrentViewport(g_lpViewport)))
	{
		ERR_DDFUNC17;
		return(FALSE);
	}
	// init palette storage structure (for bitmap loading)
	SAFE_ALLOC(g_lpPalette,
			   LPPALETTEENTRY,
			   sizeof(PALETTEENTRY)*256);
	// set up random palette
	SET_PALETTE_ENTRY(0,0,0,0);
	for(dwCount=1;dwCount<255;dwCount++)
		SET_PALETTE_ENTRY((BYTE)dwCount,
						  rand()%255,
						  rand()%255,
						  rand()%255);
	SET_PALETTE_ENTRY(255,255,255,255);
	// use linear fog
	if(GetGlobalUseFogFlag())
	if(FAILED(GetGlobalDirect3DDevicePtr()->SetLightState(D3DLIGHTSTATE_FOGMODE,
														  D3DFOG_LINEAR)))
		ERR_DDFUNC25;
	// set refernece alpha value
	if(FAILED(GetGlobalDirect3DDevicePtr()->SetRenderState(D3DRENDERSTATE_ALPHAREF,
														   0x01)))
		ERR_DDFUNC26;
	// set alpha compare function
	if(FAILED(GetGlobalDirect3DDevicePtr()->SetRenderState(D3DRENDERSTATE_ALPHAFUNC,
														   D3DCMP_GREATEREQUAL)))
		ERR_DDFUNC27;
	// set source blend value
	if(FAILED(GetGlobalDirect3DDevicePtr()->SetRenderState(D3DRENDERSTATE_SRCBLEND,
														   D3DBLEND_SRCALPHA)))
		ERR_DDFUNC28;
	// set destination blend value
	if(FAILED(GetGlobalDirect3DDevicePtr()->SetRenderState(D3DRENDERSTATE_DESTBLEND,
														   D3DBLEND_INVSRCALPHA)))
		ERR_DDFUNC29;
	// set up overlay vertices
	if(GetGlobalResolutionFlag())
	{
		g_lpOverlayVerts[0]=D3DTLVERTEX(D3DVECTOR(0,0,0),0.1F,D3DRGBA(0,0,0,0),NULL,0,0);
		g_lpOverlayVerts[1]=D3DTLVERTEX(D3DVECTOR(640,0,0),0.1F,D3DRGBA(0,0,0,0),NULL,0,0);
		g_lpOverlayVerts[2]=D3DTLVERTEX(D3DVECTOR(0,480,0),0.1F,D3DRGBA(0,0,0,0),NULL,0,0);
		g_lpOverlayVerts[3]=D3DTLVERTEX(D3DVECTOR(640,480,0),0.1F,D3DRGBA(0,0,0,0),NULL,0,0);
	}
	else
	{
		g_lpOverlayVerts[0]=D3DTLVERTEX(D3DVECTOR(0,0,0),0.1F,D3DRGBA(0,0,0,0),NULL,0,0);
		g_lpOverlayVerts[1]=D3DTLVERTEX(D3DVECTOR(320,0,0),0.1F,D3DRGBA(0,0,0,0),NULL,0,0);
		g_lpOverlayVerts[2]=D3DTLVERTEX(D3DVECTOR(0,240,0),0.1F,D3DRGBA(0,0,0,0),NULL,0,0);
		g_lpOverlayVerts[3]=D3DTLVERTEX(D3DVECTOR(320,240,0),0.1F,D3DRGBA(0,0,0,0),NULL,0,0);
	}
	// set up index
	g_lpIndex[0]=0;
	g_lpIndex[1]=1;
	g_lpIndex[2]=2;
	g_lpIndex[3]=3;
	g_lpIndex[4]=2;
	g_lpIndex[5]=1;
	/*
	// find a useful alpha pixel format if necessary
	if(GetGlobalUseAlphaFlag())
	{
		// enumerate formats
		if(FAILED(g_lpD3DDevice->EnumTextureFormats((LPD3DENUMPIXELFORMATSCALLBACK)EnumAlphaTextureCallback,
													(LPVOID)&g_ddpfAlpha)))
		{
			ERR_DDFUNC31;
			return(FALSE);
		}
		// make sure it's good
		if(g_ddpfAlpha.dwSize!=sizeof(DDPIXELFORMAT))
		{
			ERR_DDFUNC32;
			return(FALSE);
		}
	}*/
	// alpha surfaces eliminated because I discovered color keying!
	if(FAILED(g_lpD3DDevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE,
											TRUE)))
		MSG_DDFUNC06;
	// clear surfaces
	if(g_bUseTripleBuffer)
	{
		ClearFrontBuffer();
		PerformPageFlip();
	}
	ClearFrontBuffer();
	ClearBackBuffer();
	// set Direct3D globals
	SetD3DGlobals();
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  ReleaseDirectDraw()						//
//	  (defined in "ddfunc.h")							//
//														//
//  Unload DirectDraw and release all storage			//
//		associated with it.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void ReleaseDirectDraw(void)
{
	// return display to normal
	if(g_lpDD)
	{
		// restore display mode
		g_lpDD->RestoreDisplayMode();
		// restore cooperative level
		g_lpDD->SetCooperativeLevel(GetGlobalHWnd(),
								    DDSCL_NORMAL);
	}
	// free palette memory
	SAFE_FREE(g_lpPalette);
	// release viewport
	SAFE_RELEASE(g_lpViewport);
	// release Direct3DDevice
	SAFE_RELEASE(g_lpD3DDevice);
	// release z-buffer
	SAFE_RELEASE(g_lpZBuffer);
	// release DirectDrawClipper
	SAFE_RELEASE(g_lpClipper);
	// release secondary DirectDrawSurface
	SAFE_RELEASE(g_lpBackBuffer);
	// release primary DirectDrawSurface
	SAFE_RELEASE(g_lpFrontBuffer);
	// release Direct3D
	SAFE_RELEASE(g_lpD3D);
	// release DirectDraw
	SAFE_RELEASE(g_lpDD);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalDirectDrawPtr()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Retrieve pointer to DirectDraw interface.			//
//		associated with it.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDIRECTDRAW4: pointer to DirectDraw interface  //
//														//
//////////////////////////////////////////////////////////

extern LPDIRECTDRAW4 inline GetGlobalDirectDrawPtr(void)
{
	return(g_lpDD);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  EnumDirect3DCallback()					//
//														//
//  Enumerates all Direct3D								//
//	  Parameters:										//
//		LPGUID lpGUID: GUID of device					//
//		LPSTR lpDeviceName: name of device				//
//		LPSTR lpDeviceDesc: description of device		//
//		LPD3DDEVICEDESC lpdevHardware: hardware			//
//									   description		//
//		LPD3DDEVICEDESC lpdevSoftware: software			//
//									   description		//
//		LPVOID lpUserArg: user supplied data			//
//	  Return Value:										//
//		This is a Direct3D callback function.  Return	//
//		D3DENUMRET_OK to continue or D3DENUMRET_CANCEL	//
//		to stop enumeration.							//
//														//
//////////////////////////////////////////////////////////

BOOL WINAPI EnumDirect3DCallback(LPGUID lpGUID,
								 LPSTR lpDeviceName,
								 LPSTR lpDeviceDesc,
								 LPD3DDEVICEDESC lpdevHardware,
								 LPD3DDEVICEDESC lpdevSoftware,
								 LPVOID lpUserArg)
{
	// pointer to our data structure
	D3DDEVICELIST_PTR D3DDeviceListPtr=(D3DDEVICELIST_PTR)lpUserArg;
	// pointer to current device
	D3DDEVICE_PTR D3DDevicePtr=NULL;
	// hardware flag
	BOOL bIsHardware=FALSE;
	// flag indicating a software rasterizer was already found
	static BOOL bSoftwareFound=FALSE;

	// reject reference rasterizer - much too slow!
	if((*lpGUID)==IID_IDirect3DRefDevice)
		return(D3DENUMRET_OK);
	// check to see if this is hardware
	if(lpdevHardware)
	{
		if(lpdevHardware->dwFlags)
			bIsHardware=TRUE;
	}
	// if software, reject if one was already found
	else
	{
		if(bSoftwareFound)
			return(D3DENUMRET_OK);
	}
	// if hardware, check for required bit depth
	if(bIsHardware)
	{
		if(!(lpdevHardware->dwDeviceRenderBitDepth&DDBD_16))
			return(D3DENUMRET_OK);
	}
	// same for software
	else
	{
		if(!(lpdevSoftware->dwDeviceRenderBitDepth&DDBD_16))
			return(D3DENUMRET_OK);
	}
	// if hardware, reject mono devices
	if(bIsHardware)
	{
		if(lpdevHardware->dcmColorModel==D3DCOLOR_MONO) 
			return(D3DENUMRET_OK);
 	}
	// same for software
	else
	{
		if(lpdevSoftware->dcmColorModel==D3DCOLOR_MONO) 
			return(D3DENUMRET_OK);
	}
	// create storage structure if necessary
	if(D3DDeviceListPtr->dwNumDevices==0)
	{
		// allocate memory
		SAFE_ALLOC(D3DDeviceListPtr->D3DDevicePtr,
				   D3DDEVICE_PTR,
				   sizeof(D3DDEVICE));
		// check for errors
		if(!D3DDeviceListPtr->D3DDevicePtr)
			return(D3DENUMRET_OK);
		// set pointer to device data
		D3DDevicePtr=D3DDeviceListPtr->D3DDevicePtr;
	}
	// otherwise expand storage structure
	else
	{
		// temporary storage
		LPVOID lpTemp=NULL;

		// allocate memory
		SAFE_ALLOC(lpTemp,
				   LPVOID,
				   sizeof(D3DDEVICE)*(D3DDeviceListPtr->dwNumDevices+1));
		// check for errors
		if(!lpTemp)
			return(D3DENUMRET_OK);
		// swap pointers
		SwapPointers(&lpTemp,
					 (LPVOID*)&D3DDeviceListPtr->D3DDevicePtr);
		// copy data
		CopyMemory(D3DDeviceListPtr->D3DDevicePtr,
				   lpTemp,
				   sizeof(D3DDEVICE)*D3DDeviceListPtr->dwNumDevices);
		// out with the old!
		SAFE_FREE(lpTemp);
		// set pointer to device data
		D3DDevicePtr=D3DDeviceListPtr->D3DDevicePtr;
	}
	// save GUID
	CopyMemory(&D3DDevicePtr[D3DDeviceListPtr->dwNumDevices].guidDeviceGUID,
			   lpGUID,
			   sizeof(GUID));
	// save device description
	CopyMemory(D3DDevicePtr[D3DDeviceListPtr->dwNumDevices].lpDeviceDesc,
			   lpDeviceDesc,
			   80);
	D3DDevicePtr[D3DDeviceListPtr->dwNumDevices].lpDeviceDesc[80]=0;
	// save device name
	CopyMemory(D3DDevicePtr[D3DDeviceListPtr->dwNumDevices].lpDeviceName,
			   lpDeviceName,
			   80);
	D3DDevicePtr[D3DDeviceListPtr->dwNumDevices].lpDeviceName[80]=0;
	// save hardware caps
	CopyMemory(&D3DDevicePtr[D3DDeviceListPtr->dwNumDevices].devHardware,
			   lpdevHardware,
			   sizeof(D3DDEVICEDESC));
	// save software caps
	CopyMemory(&D3DDevicePtr[D3DDeviceListPtr->dwNumDevices].devSoftware,
			   lpdevSoftware,
			   sizeof(D3DDEVICEDESC));
	// save hardware flag
	D3DDevicePtr[D3DDeviceListPtr->dwNumDevices].bIsHardware=bIsHardware;
	// if hardware, set z-buffer required flag
	if(bIsHardware)
	{
		if(lpdevHardware->dwDevCaps&D3DPRASTERCAPS_ZBUFFERLESSHSR)
			D3DDevicePtr[D3DDeviceListPtr->dwNumDevices].bUseZBuffer=FALSE;
		else
			D3DDevicePtr[D3DDeviceListPtr->dwNumDevices].bUseZBuffer=TRUE;
	}
	// same for software
	else
	{
		if(lpdevSoftware->dwDevCaps&D3DPRASTERCAPS_ZBUFFERLESSHSR)
			D3DDevicePtr[D3DDeviceListPtr->dwNumDevices].bUseZBuffer=FALSE;
		else
			D3DDevicePtr[D3DDeviceListPtr->dwNumDevices].bUseZBuffer=TRUE;
	}
	// increment counter
	D3DDeviceListPtr->dwNumDevices++;
	// keep going
	return(D3DENUMRET_OK);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  SelectD3DDevice()						//
//	  (defined in "ddfunc.h")							//
//														//
//  Detects valid Direct3D devices.  Note: Do not call  //
//		this function until SelectDDDevice() has been	//
//		called or the global DirectDraw variables have	//
//		been properly initialized.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL SelectD3DDevice(void)
{
	// our storage structure
	D3DDEVICELIST D3DDeviceList;
	INIT_VAR(D3DDeviceList);
	// used for selection window
	LPSTR lpText=NULL;
	// selection
	DWORD dwSelection=0;
	// temporary DirectDraw interface
	LPDIRECTDRAW lpDDTemp;
	// temporary Direct3D interface
	LPDIRECT3D3 lpD3DTemp;

	// init global variables
	INIT_VAR(g_guidD3DDevice);
	// init temporary DirectDraw interface from specific device
	if(!g_bIsDefault)
	{
		if(FAILED(DirectDrawCreate(&g_guidDDDevice,
								   &lpDDTemp,
								   NULL)))
		{
			ERR_DDFUNC02;
			return(FALSE);
		}
	}
	// or default device
	else
	{
		if(FAILED(DirectDrawCreate(NULL,
								   &lpDDTemp,
								   NULL)))
		{
			ERR_DDFUNC02;
			return(FALSE);
		}
	}
	// create temporary Direct3D interface
	if(FAILED(lpDDTemp->QueryInterface(IID_IDirect3D3,
									   (LPVOID*)&lpD3DTemp)))
	{
		ERR_DDFUNC08;
		return(FALSE);
	}	
	// begin enumeration
	if(FAILED(lpD3DTemp->EnumDevices((LPD3DENUMDEVICESCALLBACK)EnumDirect3DCallback,
									 (LPVOID)&D3DDeviceList)))
	{
		ERR_DDFUNC06;
		return(FALSE);
	}
	// release temporary variables
	SAFE_RELEASE(lpD3DTemp);
	SAFE_RELEASE(lpDDTemp);
	// check for error
	if(D3DDeviceList.dwNumDevices==0)
	{
		ERR_DDFUNC07;
		return(FALSE);
	}
	// if only one, use it
	if(D3DDeviceList.dwNumDevices==1)
	{
		// let user know
		MSG_DDFUNC01;
		// select it
		dwSelection=0;
	}
	// otherwise let user pick
	else
	{
		// allocate memory for window text
		SAFE_ALLOC(lpText,
				   LPSTR,
				   (D3DDeviceList.dwNumDevices*160)+256);
		// loop until selection is made
		while(TRUE)
		{
			// counter 
			DWORD dwCount=0;
			// temporary text
			char lpTemp[8];
			INIT_VAR(lpTemp);

			// fill in text
			strcpy(lpText,"Please select a Direct3D device to use:\n\n");
			// add devices
			for(dwCount=0;dwCount<D3DDeviceList.dwNumDevices;dwCount++)
			{
				// add arrow to current selection
				if(dwCount==dwSelection)
					strcat(lpText,"--> ");
				// otherwise add spaces
				else
					strcat(lpText,"     ");
				// add number
				sprintf(lpTemp,"%d ",dwCount+1);
				strcat(lpText,lpTemp);
				// add device name
				strcat(lpText,D3DDeviceList.D3DDevicePtr[(D3DDeviceList.dwNumDevices-1)-dwCount].lpDeviceName);
				// add device description
				strcat(lpText," (");
				strcat(lpText,D3DDeviceList.D3DDevicePtr[(D3DDeviceList.dwNumDevices-1)-dwCount].lpDeviceDesc);
				strcat(lpText,")\n");
			}
			// add instructions
			strcat(lpText,"\nClick 'Yes' to accept this choice or 'No' to select the next one.");
			// check for selection
			if(MessageBox(GetGlobalHWnd(),
						  lpText,
						  "Select Direct3D Device",
						  MB_YESNO)==IDYES)
				break;
			// keep going
			dwSelection++;
			if(dwSelection>=D3DDeviceList.dwNumDevices)
				dwSelection=0;			
		}
		// free memory
		SAFE_FREE(lpText);
	}
	// copy information to globals
	dwSelection=(D3DDeviceList.dwNumDevices-1)-dwSelection;
	g_guidD3DDevice=D3DDeviceList.D3DDevicePtr[dwSelection].guidDeviceGUID;
	g_bIsHardware=D3DDeviceList.D3DDevicePtr[dwSelection].bIsHardware;
	g_bUseZBuffer=D3DDeviceList.D3DDevicePtr[dwSelection].bUseZBuffer;
	// release memory
	SAFE_FREE(D3DDeviceList.D3DDevicePtr);
	// looks good to me
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalD3DDeviceGUID()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Retrieve selected Direct3D device GUID				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		GUID: selected Direct3D device GUID				//
//														//
//////////////////////////////////////////////////////////

extern GUID inline GetGlobalD3DDeviceGUID(void)
{
	return(g_guidD3DDevice);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalD3DHardwareFlag()				//
//	  (defined in "ddfunc.h")							//
//														//
//  Retrieve selected Direct3D device hardware flag.	//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: selected Direct3D device hardware flag	//
//														//
//////////////////////////////////////////////////////////

extern BOOL inline GetGlobalD3DHardwareFlag(void)
{
	return(g_bIsHardware);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  SetGlobalD3DDeviceGUID()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Set current Direct3D device GUID					//
//	  Parameters:										//
//		GUID guid: selected Direct3D device GUID		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void inline SetGlobalD3DDeviceGUID(GUID guid)
{
	g_guidD3DDevice=guid;
}

//////////////////////////////////////////////////////////
//														//
//  Function:  SetGlobalD3DHardwareFlag()				//
//	  (defined in "ddfunc.h")							//
//														//
//  Set current Direct3D device hardware flag.			//
//	  Parameters:										//
//		BOOL: selected Direct3D device hardware flag	//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void inline SetGlobalD3DHardwareFlag(BOOL bFlag)
{
	g_bIsHardware=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalDirect3DPtr()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Retrieve pointer to Direct3D interface.				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDIRECT3D3: pointer to Direct3D interface		//
//														//
//////////////////////////////////////////////////////////

extern LPDIRECT3D3 inline GetGlobalDirect3DPtr(void)
{
	return(g_lpD3D);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalFrontBufferPtr()				//
//	  (defined in "ddfunc.h")							//
//														//
//  Retrieve pointer to primary DirectDrawSurface		//
//		interface.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDIRECTDRAWSURFACE4: pointer to DirectDraw-	//
//							  Surface					//
//														//
//////////////////////////////////////////////////////////

extern LPDIRECTDRAWSURFACE4 inline GetGlobalFrontBufferPtr(void)
{
	return(g_lpFrontBuffer);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalBackBufferPtr()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Retrieve pointer to secondary DirectDrawSurface		//
//		interface.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDIRECTDRAWSURFACE4: pointer to DirectDraw-	//
//							  Surface					//
//														//
//////////////////////////////////////////////////////////

extern LPDIRECTDRAWSURFACE4 inline GetGlobalBackBufferPtr(void)
{
	return(g_lpBackBuffer);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalUse15BitFlag()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Query flag which determines whether this device		//
//		must use 15-bit data instead of 16-bit.			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if 15-bit is required				//
//														//
//////////////////////////////////////////////////////////

extern BOOL inline GetGlobalUse15BitFlag(void)
{
	return(g_bUse15Bit);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalClipperPtr()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Retrieve pointer to DirectDrawClipper interface.	//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDIRECTDRAWCLIPPER: pointer to DirectDraw-		//
//							  Clipper interface			//
//														//
//////////////////////////////////////////////////////////

extern LPDIRECTDRAWCLIPPER inline GetGlobalClipperPtr(void)
{
	return(g_lpClipper);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalUseZBufferFlag()				//
//	  (defined in "ddfunc.h")							//
//														//
//  Query flag which determines whether this device		//
//		must use z-buffer.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if z-buffer is required				//
//														//
//////////////////////////////////////////////////////////

extern BOOL inline GetGlobalUseZBufferFlag(void)
{
	return(g_bUseZBuffer);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  SetGlobalUseZBufferFlag()				//
//	  (defined in "ddfunc.h")							//
//														//
//  Set flag which determines whether this device		//
//		must use z-buffer.								//
//	  Parameters:										//
//		BOOL: TRUE if z-buffer is required				//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void inline SetGlobalUseZBufferFlag(BOOL bFlag)
{
	g_bUseZBuffer=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//  Function:  EnumZBufferCallback()					//
//														//
//	Record a useful z-buffer pixel format				//
//	  Parameters:										//
//		LPDDPIXELFORMAT lpDDPF: pixel format that was	//
//								found					//
//		LPVOID lpUserArg: pointer to the pixel format	//
//						  that we want to keep			//
//	  Return Value:										//
//		This is a Direct3D callback function.  Return	//
//		D3DENUMRET_OK to continue or D3DENUMRET_CANCEL	//
//		to stop.										//
//														//
//////////////////////////////////////////////////////////

BOOL WINAPI EnumZBufferCallback(LPDDPIXELFORMAT lpDDPF,
								LPVOID lpUserArg)
{
	// make sure this is a z-buffer
	if(lpDDPF->dwFlags==DDPF_ZBUFFER)
		// check for required bit depth
		if(lpDDPF->dwZBufferBitDepth==16)
		{
			// save it
			CopyMemory(lpUserArg,
					   lpDDPF,
					   sizeof(DDPIXELFORMAT));
			// no need to continue
			return(D3DENUMRET_CANCEL);	
		}	
	// no good; continue
	return(D3DENUMRET_OK);	
}

//////////////////////////////////////////////////////////
//														//
//  Function:  EnumAlphaTextureCallback()				//
//														//
//	Record a useful alpha texture pixel format			//
//	  Parameters:										//
//		LPDDPIXELFORMAT lpDDPF: pixel format that was	//
//								found					//
//		LPVOID lpUserArg: pointer to the pixel format	//
//						  that we want to keep			//
//	  Return Value:										//
//		This is a DirectDraw callback function.  Return	//
//		DDENUMRET_OK to continue or DDENUMRET_CANCEL	//
//		to stop.										//
//														//
//////////////////////////////////////////////////////////

BOOL WINAPI EnumAlphaTextureCallback(LPDDPIXELFORMAT lpDDPF,
									 LPVOID lpUserArg)
{
	// alpha bit counter
	DWORD dwAlphaBitCount=0;
	// alpha mask
	DWORD dwAlphaMask=0;

	// make sure we have a pixel format
	if(!lpDDPF)
		return(DDENUMRET_OK);
	// count the alpha bits for the pixel format
	if(lpDDPF->dwFlags&DDPF_ALPHAPIXELS)
		for(dwAlphaMask=lpDDPF->dwRGBAlphaBitMask;dwAlphaMask;dwAlphaMask>>=1)
			dwAlphaBitCount+=(dwAlphaMask&0x1);
	// make sure format has alpha
	if(dwAlphaBitCount!=1)
		return(DDENUMRET_OK);
	// skip bumpmapping modes
	if(lpDDPF->dwFlags&(DDPF_BUMPLUMINANCE|DDPF_BUMPDUDV))
		return(DDENUMRET_OK);
	// skip luminance-only or alpha-only modes
	if(lpDDPF->dwFlags&(DDPF_LUMINANCE|DDPF_ALPHA))
		return(DDENUMRET_OK);
	// skip any yuv or 4cc modes
	if(lpDDPF->dwFlags&(DDPF_YUV|DDPF_FOURCC))
		return(DDENUMRET_OK);
	// check for 8-bit palettized formats
	if(lpDDPF->dwFlags&DDPF_PALETTEINDEXED8)
		return(DDENUMRET_OK);
	// skip any non-RGB formats
	if(!(lpDDPF->dwFlags&DDPF_RGB))
		return(DDENUMRET_OK);
	// check bits-per-pixel
	if(lpDDPF->dwRGBBitCount!=16)
		return DDENUMRET_OK;
	// save it
	CopyMemory(lpUserArg,
			   lpDDPF,
			   sizeof(DDPIXELFORMAT));
	// no need to continue
	return(D3DENUMRET_CANCEL);	
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalZBufferPtr()					//
//	  (defined in "ddfunc.h")							//
//														//
//  Retrieve pointer to z-buffer DirectDrawSurface		//
//		interface.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDIRECTDRAWSURFACE4: pointer to DirectDraw-	//
//							  Surface					//
//														//
//////////////////////////////////////////////////////////

extern LPDIRECTDRAWSURFACE4 inline GetGlobalZBufferPtr(void)
{
	return(g_lpZBuffer);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalDirect3DDevicePtr()				//
//		(defined in "ddfunc.h")							//
//														//
//	Retrieve pointer to Direct3DDevice interface.		//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDIRECT3DDEVICE3: pointer to Direct3DDevice	//
//						   interface					//
//														//
//////////////////////////////////////////////////////////

extern LPDIRECT3DDEVICE3 inline GetGlobalDirect3DDevicePtr(void)
{
	return(g_lpD3DDevice);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalViewportPtr()					//
//		(defined in "ddfunc.h")							//
//														//
//	Retrieve pointer to Direct3DViewport interface.		//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDIRECT3DVIEWPORT3: pointer to Direct3D-		//
//							 Viewport interface			//
//														//
//////////////////////////////////////////////////////////

extern LPDIRECT3DVIEWPORT3 inline GetGlobalViewportPtr(void)
{
	return(g_lpViewport);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalPalettePtr()					//
//		(defined in "ddfunc.h")							//
//														//
//	Retrieve pointer to palette array.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPPALETTEENTRY: pointer to array				//
//														//
//////////////////////////////////////////////////////////

extern LPPALETTEENTRY inline GetGlobalPalettePtr(void)
{
	return(g_lpPalette);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  LockFrontBuffer()						//
//		(defined in "ddfunc.h")							//
//														//
//	Lock front buffer and return necessary data.		//
//	  Parameters:										//
//		LPWORD*: pointer to address of data				//
//		LPDWORD: pointer to pitch						//
//	  Return Value:										//
//		BOOL: TRUE if successful.  (Do not use returned	//
//			  data if FALSE)							//
//														//
//////////////////////////////////////////////////////////

extern BOOL LockFrontBuffer(LPWORD* lplpwData,
							LPDWORD lpdwPitch)
{
	// surface description
	DDSURFACEDESC2 ddsd;
	INIT_STRUCT(ddsd);

	// lock buffer
#ifdef DEBUG_MODE
	if(FAILED(g_lpFrontBuffer->Lock(NULL,
								    &ddsd,
								    DDLOCK_SURFACEMEMORYPTR|
								    DDLOCK_WAIT,
								    NULL)))
	{
		ERR_DDFUNC18;
		return(FALSE);
	}
#else
	g_lpFrontBuffer->Lock(NULL,
						  &ddsd,
						  DDLOCK_SURFACEMEMORYPTR|
						  DDLOCK_WAIT,
						  NULL);
#endif
	// fill in return values
	*lplpwData=(LPWORD)ddsd.lpSurface;
	*lpdwPitch=ddsd.lPitch>>1;
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  UnlockFrontBuffer()						//
//		(defined in "ddfunc.h")							//
//														//
//	Unlock previously locked buffer.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void UnlockFrontBuffer(void)
{
#ifdef DEBUG_MODE
	if(FAILED(g_lpFrontBuffer->Unlock(NULL)))
		ERR_DDFUNC19;
#else
	g_lpFrontBuffer->Unlock(NULL);
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  LockBackBuffer()							//
//		(defined in "ddfunc.h")							//
//														//
//	Lock back buffer and return necessary data.			//
//	  Parameters:										//
//		LPWORD*: pointer to address of data				//
//		LPDWORD: pointer to pitch						//
//	  Return Value:										//
//		BOOL: TRUE if successful.  (Do not use returned	//
//			  data if FALSE)							//
//														//
//////////////////////////////////////////////////////////

extern BOOL LockBackBuffer(LPWORD* lplpwData,
						   LPDWORD lpdwPitch)
{
	// surface description
	DDSURFACEDESC2 ddsd;
	INIT_STRUCT(ddsd);

	// lock buffer
#ifdef DEBUG_MODE
	if(FAILED(g_lpBackBuffer->Lock(NULL,
								   &ddsd,
								   DDLOCK_SURFACEMEMORYPTR|
								   DDLOCK_WAIT,
								   NULL)))
	{
		ERR_DDFUNC19;
		return(FALSE);
	}
#else
	g_lpBackBuffer->Lock(NULL,
						 &ddsd,
						 DDLOCK_SURFACEMEMORYPTR|
						 DDLOCK_WAIT,
						 NULL);
#endif
	// fill in return values
	*lplpwData=(LPWORD)ddsd.lpSurface;
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
//	Function:  UnlockBackBuffer()						//
//		(defined in "ddfunc.h")							//
//														//
//	Unlock previously locked buffer.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void UnlockBackBuffer(void)
{
#ifdef DEBUG_MODE
	if(FAILED(g_lpBackBuffer->Unlock(NULL)))
		ERR_DDFUNC21;
#else
	g_lpBackBuffer->Unlock(NULL);
#endif
}

//////////////////////////////////////////////////////////
//														//
//	Function:  ClearFrontBuffer()						//
//		(defined in "ddfunc.h")							//
//														//
//	Clears either a specified rectangle of the front	//
//		buffer or the entire surface.					//
//	  Parameters:										//
//		None if clearing entire surface.				//
//		DWORD: x-coordinate or upper-left corner		//
//		DWORD: y-coordinate or upper-left corner		//
//		DWORD: x-coordinate or lower-right corner		//
//		DWORD: y-coordinate or lower-right corner		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void ClearFrontBuffer(DWORD dwX1,
							 DWORD dwY1,
							 DWORD dwX2,
							 DWORD dwY2)
{
	// rectangle to clear
	RECT rect;
	// blit fx structure
	DDBLTFX ddbltfx;

	// see if values were supplied
	if(dwX1!=0xF0000000L)
	{
		// if yes, use them
		rect.left=dwX1;
		rect.top=dwY1;
		rect.right=dwX2;
		rect.bottom=dwY2;
	}
	else
	{
		// set values
		rect.left=0;
		rect.top=0;
		rect.right=g_rScreen.x2;
		rect.bottom=g_rScreen.y2;
	}
	// prepare blit fx
	INIT_STRUCT(ddbltfx);
	// clear it (fill color is 0)
#ifdef DEBUG_MODE
	if(FAILED(g_lpFrontBuffer->Blt(&rect,
								   NULL,
								   NULL,
								   DDBLT_COLORFILL|
								   DDBLT_WAIT,
								   &ddbltfx)))
		ERR_DDFUNC22;
#else
	g_lpFrontBuffer->Blt(&rect,
						 NULL,
						 NULL,
						 DDBLT_COLORFILL|
						 DDBLT_WAIT,
						 &ddbltfx);
#endif
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  ClearBackBuffer()						//
//		(defined in "ddfunc.h")							//
//														//
//	Clears either a specified rectangle of the back		//
//		buffer or the entire surface.					//
//	  Parameters:										//
//		None if clearing entire surface.				//
//		DWORD: x-coordinate or upper-left corner		//
//		DWORD: y-coordinate or upper-left corner		//
//		DWORD: x-coordinate or lower-right corner		//
//		DWORD: y-coordinate or lower-right corner		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void ClearBackBuffer(DWORD dwX1,
							DWORD dwY1,
							DWORD dwX2,
							DWORD dwY2)
{
	// blit fx structure
	DDBLTFX ddbltfx;
	// rectangle to clear
	RECT rect;

	// prepare blit fx (fill color is 0)
	INIT_STRUCT(ddbltfx);
	// see if values were supplied
	if(dwX1!=0xF0000000L)
	{
		// if yes, use them
		rect.left=dwX1;
		rect.top=dwY1;
		rect.right=dwX2;
		rect.bottom=dwY2;
		// clear it
#ifdef DEBUG_MODE
		if(FAILED(g_lpBackBuffer->Blt(&rect,
									  NULL,
									  NULL,
									  DDBLT_COLORFILL|
									  DDBLT_WAIT,
									  &ddbltfx)))
			ERR_DDFUNC23;
#else
		g_lpBackBuffer->Blt(&rect,
							NULL,
							NULL,
							DDBLT_COLORFILL|
							DDBLT_WAIT,
							&ddbltfx);
#endif
	}
	else
	{
		// clear all
#ifdef DEBUG_MODE
		if(FAILED(g_lpBackBuffer->Blt(&g_rScreen2,
									  NULL,
									  NULL,
									  DDBLT_COLORFILL|
									  DDBLT_WAIT,
									  &ddbltfx)))
			ERR_DDFUNC23;
#else
		g_lpBackBuffer->Blt(&g_rScreen2,
							NULL,
							NULL,
							DDBLT_COLORFILL|
							DDBLT_WAIT,
							&ddbltfx);
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
//	Function:  PerformPageFlip()						//
//		(defined in "ddfunc.h")							//
//														//
//	Flip front and back buffers.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void PerformPageFlip(void)
{
	while(FAILED(g_lpFrontBuffer->Flip(NULL,DDFLIP_WAIT)))
	{}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalScreenRectanglePtr()			//
//		(defined in "ddfunc.h")							//
//														//
//	Get screen coordinates.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPD3DRECT: pointer								//
//														//
//////////////////////////////////////////////////////////

extern LPD3DRECT inline GetGlobalScreenRectanglePtr(void)
{
	return(&g_rScreen);	
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalPaletteValue15()				//
//		(defined in "ddfunc.h")							//
//														//
//	Return 15-bit palette color value					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		WORD: value										//
//														//
//////////////////////////////////////////////////////////

extern inline WORD GetGlobalPaletteValue15(BYTE cVal)
{
	return(g_wPal15[cVal]);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalPaletteValue16()				//
//		(defined in "ddfunc.h")							//
//														//
//	Return 15-bit palette color value					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		WORD: value										//
//														//
//////////////////////////////////////////////////////////

extern inline WORD GetGlobalPaletteValue16(BYTE cVal)
{
	return(g_wPal16[cVal]);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalPaletteValue15()				//
//		(defined in "ddfunc.h")							//
//														//
//	Set 15-bit palette color value						//
//	  Parameters:										//
//		BYTE: color number								//
//		WORD: color value								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern inline void SetGlobalPaletteValue15(BYTE cVal,
										   WORD wVal)
{
	g_wPal15[cVal]=wVal;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalPaletteValue16()				//
//		(defined in "ddfunc.h")							//
//														//
//	Set 16-bit palette color value						//
//	  Parameters:										//
//		BYTE: color number								//
//		WORD: color value								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern inline void SetGlobalPaletteValue16(BYTE cVal,
										   WORD wVal)
{
	g_wPal16[cVal]=wVal;
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  FadeScreen()								//
//		(defined in "ddfunc.h")							//
//														//
//	Set linear texturing flag.							//
//	  Parameters:										//
//		D3DVALUE: red									//
//		D3DVALUE: green									//
//		D3DVALUE: blue									//
//		D3DVALUE: alpha									//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void FadeScreen(D3DCOLOR dcColor)
{
	// no need for z-buffer
	DisableZBuffer();
	// turn off texture
	CurrentTextureOff();
	// set vertex colors
	g_lpOverlayVerts[0].dcColor=dcColor;
	g_lpOverlayVerts[1].dcColor=dcColor;
	g_lpOverlayVerts[2].dcColor=dcColor;
	g_lpOverlayVerts[3].dcColor=dcColor;
	// render overlay
#ifdef DEBUG_MODE
	if(FAILED(g_lpD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
												  D3DFVF_TLVERTEX,
												  g_lpOverlayVerts,
												  4,
												  g_lpIndex,
												  6,
												  D3DDP_DONOTCLIP|
												  D3DDP_DONOTLIGHT|
												  D3DDP_DONOTUPDATEEXTENTS)))
		ERR_DDFUNC30;
#else
	g_lpD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
										D3DFVF_TLVERTEX,
										g_lpOverlayVerts,
										4,
										g_lpIndex,
										6,
										D3DDP_DONOTCLIP|
										D3DDP_DONOTLIGHT|
										D3DDP_DONOTUPDATEEXTENTS);
#endif
	// turn on z-buffer
	EnableZBuffer();
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalAlphaPixelFormat()				//
//		(defined in "ddfunc.h")							//
//														//
//	Return 16-bit alpha pixel format					//
//	  Parameters:										//
//		LPDDPIXELFORMAT: format							//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern inline void GetGlobalAlphaPixelFormat(LPDDPIXELFORMAT lpDDPF)
{
	// copy info
	CopyMemory(lpDDPF,
			   &g_ddpfAlpha,
			   sizeof(DDPIXELFORMAT));
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SelectViewportSize()						//
//		(defined in "ddfunc.h")							//
//														//
//	Let user select size of viewport.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SelectViewportSize(void)
{
	// only choose if hi-res was selected
	if(!g_bHiRes)
	{
		g_bViewportSize=2;
		return;
	}
	// loop until selection is made
	while(TRUE)
	{
		if(MessageBox(GetGlobalHWnd(),
					  "Please select viewport size.\n\n--> 1. 640x480\n     2. 480x320\n     3. 320x240\n\nClick 'Yes' to accept this choice or 'No' to select the next one.",
				      "Select Resolution",
				      MB_YESNO)==IDYES)
		{
			g_bViewportSize=0;
			break;
		}
		if(MessageBox(GetGlobalHWnd(),
					  "Please select viewport size.\n\n     1. 640x480\n--> 2. 480x320\n     3. 320x240\n\nClick 'Yes' to accept this choice or 'No' to select the next one.",
				      "Select Resolution",
				      MB_YESNO)==IDYES)
		{
			g_bViewportSize=1;
			break;
		}
		if(MessageBox(GetGlobalHWnd(),
					  "Please select viewport size.\n\n     1. 640x480\n     2. 480x320\n--> 3. 320x240\n\nClick 'Yes' to accept this choice or 'No' to select the next one.",
				      "Select Resolution",
				      MB_YESNO)==IDYES)
		{
			g_bViewportSize=2;
			break;
		}
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalViewportSizeFlag()				//
//		(defined in "ddfunc.h")							//
//														//
//	Get value of viewport size flag.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: flag										//
//														//
//////////////////////////////////////////////////////////

extern BOOL inline GetGlobalViewportSizeFlag(void)
{
	return(g_bViewportSize);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalViewportSizeFlag()				//
//		(defined in "ddfunc.h")							//
//														//
//	Set value of viewport size flag.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: flag										//
//														//
//////////////////////////////////////////////////////////

extern void inline SetGlobalViewportSizeFlag(BOOL bFlag)
{
	g_bViewportSize=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//  Function:  GetGlobalUseTripleBufferFlag()			//
//	  (defined in "ddfunc.h")							//
//														//
//  Query triple buffering flag.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if triple buffer is reqested			//
//														//
//////////////////////////////////////////////////////////

extern BOOL inline GetGlobalUseTripleBufferFlag(void)
{
	return(g_bUseTripleBuffer);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  SetGlobalUseTripleBufferFlag()			//
//	  (defined in "ddfunc.h")							//
//														//
//  Set triple buffering flag.							//
//	  Parameters:										//
//		BOOL: flag										//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void inline SetGlobalUseTripleBufferFlag(BOOL bFlag)
{
	g_bUseTripleBuffer=bFlag;
}

// END ///////////////////////////////////////////////////


