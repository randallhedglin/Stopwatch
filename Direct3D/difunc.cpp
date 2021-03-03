// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  difunc.cpp									//
//														//
//	Functions for handling DirectInput.					//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// allow access to file functions
#include"filefunc.h"
// header for this module
#include"difunc.h"
// allow access to main window functions
#include"winmain.h"

// ERROR MESSAGES ////////////////////////////////////////

// failed to create DirectInput
#define ERR_DIFUNC00 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DIFUNC.CPP\nError Code: 00\nUnable to create DirectInput interface.");
// failed to access keyboard
#define ERR_DIFUNC01 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DIFUNC.CPP\nError Code: 01\nUnable to access keyboard input.");
// failed to set keyboard data format
#define ERR_DIFUNC02 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DIFUNC.CPP\nError Code: 02\nUnable to set keyboard data format.");
// failed to set keyboard cooperative level
#define ERR_DIFUNC03 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DIFUNC.CPP\nError Code: 03\nUnable to set keyboard cooperative level.");
// failed to acquire keyboard
#define ERR_DIFUNC04 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DIFUNC.CPP\nError Code: 04\nUnable to acquire keyboard input.");
// failed to read keyboard
#define ERR_DIFUNC05 DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DIFUNC.CPP\nError Code: 05\nUnable to read keyboard input.");

// MESSAGES //////////////////////////////////////////////

// failed to poll joystick
#define MSG_DIFUNC00 DISPLAY_INFO(GetGlobalHWnd(),"Unable to poll joystick port.\nJoystick will be disabled.");
// failed to read joystick
#define MSG_DIFUNC01 DISPLAY_INFO(GetGlobalHWnd(),"Unable to read joystick port.\nJoystick will be disabled.");
// failed to find joysticks
#define MSG_DIFUNC02 DISPLAY_INFO(GetGlobalHWnd(),"Unable to find valid joystick devices.\nJoystick will be disabled.");
// only one joystick found
#define MSG_DIFUNC03 DISPLAY_INFO(GetGlobalHWnd(),"Only one joystick was detected.\nThis one will be used by default.");
// failed to create joystick device
#define MSG_DIFUNC04 DISPLAY_INFO(GetGlobalHWnd(),"Unable to create interface to selected joystick.\nJoystick will be disabled.");
// failed to set joystick cooperative level
#define MSG_DIFUNC05 DISPLAY_INFO(GetGlobalHWnd(),"Unable to set joystick cooperative level.\nJoystick will be disabled.");
// failed to set joystick data format
#define MSG_DIFUNC06 DISPLAY_INFO(GetGlobalHWnd(),"Unable to set joystick data format.\nJoystick will be disabled.");
// failed to set joystick x-axis range
#define MSG_DIFUNC07 DISPLAY_INFO(GetGlobalHWnd(),"Unable to set joystick x-axis range.\nJoystick will be disabled.");
// failed to set joystick y-axis range
#define MSG_DIFUNC08 DISPLAY_INFO(GetGlobalHWnd(),"Unable to set joystick y-axis range.\nJoystick will be disabled.");
// failed to set joystick x-axis dead zone
#define MSG_DIFUNC09 DISPLAY_INFO(GetGlobalHWnd(),"Unable to set joystick x-axis dead zone.\nJoystick will be disabled.");
// failed to set joystick y-axis dead zone
#define MSG_DIFUNC10 DISPLAY_INFO(GetGlobalHWnd(),"Unable to set joystick y-axis dead zone.\nJoystick will be disabled.");
// failed to acquire joystick
#define MSG_DIFUNC11 DISPLAY_INFO(GetGlobalHWnd(),"Unable acquire joystick device.\nJoystick will be disabled.");

// GLOBALS ///////////////////////////////////////////////

// global keyboard interface pointer
LPDIRECTINPUTDEVICE g_lpKeyboard=NULL;
// global keyboard data pointer
LPBYTE g_lpKeyData=NULL;
// global control keys
CONTROL_DATA g_cdKeys;
// global joystick active flag
BOOL g_bJoystickActive=FALSE;
// global joystick plugged in flag
BOOL g_bJoystickIn=FALSE;
// global joystick interface pointer
LPDIRECTINPUTDEVICE2 g_lpJoystick=NULL;
// global DirectInput interface pointer
LPDIRECTINPUT g_lpDI=NULL;
// global joystick state
DIJOYSTATE g_dijs;
// global joystick buttons
CONTROL_DATA g_cdJoystick;
// global GUID for joystick device
GUID g_guidJoystick;

// PROTOTYPES ////////////////////////////////////////////

BOOL AcquireKeyboard(void);

BOOL ReadKeyboard(void);

BOOL ReadJoystick(void);

BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE lpDID,
									LPVOID lpUserArg);

// STRUCTURES ////////////////////////////////////////////

// joystick device info
typedef struct
{
	// device GUID
	GUID guidJoystickGUID;
	// device name
	char lpDeviceName[81];
	// device description
	char lpDeviceDesc[81];
} JOYSTICKDEVICE,*JOYSTICKDEVICE_PTR;

// joystick device list
typedef struct
{
	// pointer to array of JOYSTICKDEVICEs
	JOYSTICKDEVICE_PTR JoystickDevicePtr;
	// number of devices in list
	DWORD dwNumDevices;
} JOYSTICKDEVICELIST,*JOYSTICKDEVICELIST_PTR;

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  InitializeDirectInput()					//
//		(defined in "difunc.h")							//
//														//
//	Lauch the DirectInput interface.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL InitializeDirectInput(void)
{
	// temporary joystick device
	LPDIRECTINPUTDEVICE lpDIDTemp=NULL;
	
	// init globals
	INIT_VAR(g_dijs);
	// create DirectInput interface
	if(FAILED(DirectInputCreate(GetGlobalHInst(),
								DIRECTINPUT_VERSION,
								&g_lpDI,
								NULL)))
	{
		ERR_DIFUNC00;
		return(FALSE);
	}
	// create keyboard device
	if(FAILED(g_lpDI->CreateDevice(GUID_SysKeyboard,
								   &g_lpKeyboard,
								   NULL)))
	{
		ERR_DIFUNC01;
		return(FALSE);
	}
	// set standard keyboard data format
	if(FAILED(g_lpKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		ERR_DIFUNC02;
		return(FALSE);
	}
	// set keyboard cooperative level
	if(FAILED(g_lpKeyboard->SetCooperativeLevel(GetGlobalHWnd(),
											    DISCL_FOREGROUND|
											    DISCL_NONEXCLUSIVE)))
	{
		ERR_DIFUNC03;
		return(FALSE);
	}
	// allocate key data memory
	SAFE_ALLOC(g_lpKeyData,
			   LPBYTE,
			   256);
	// check for errors
	if(!g_lpKeyData)
		return(FALSE);
	// acquire keyboard
	if(!AcquireKeyboard())
		return(FALSE);
	// if joystick was selected, assume it's connected
	if(g_bJoystickActive)
		g_bJoystickIn=TRUE;
	else
		g_bJoystickIn=FALSE;
	// create joystick device
	if(g_bJoystickIn)
		if(FAILED(g_lpDI->CreateDevice(g_guidJoystick,
									   &lpDIDTemp,
									   NULL)))
		{
			MSG_DIFUNC04;
			g_bJoystickIn=FALSE;
		}
	// get IDirectInputDevice2 pointer
	if(g_bJoystickIn)
		if(FAILED(lpDIDTemp->QueryInterface(IID_IDirectInputDevice2,
										    (LPVOID*)&g_lpJoystick)))
		{
			MSG_DIFUNC04;
			g_bJoystickIn=FALSE;
		}
	// set joystick cooperative level
	if(g_bJoystickIn)
		if(FAILED(g_lpJoystick->SetCooperativeLevel(GetGlobalHWnd(),
												    DISCL_BACKGROUND|
												    DISCL_NONEXCLUSIVE)))
		{
			MSG_DIFUNC05;
			g_bJoystickIn=FALSE;
		}
	// set joystick data format
	if(g_bJoystickIn)
		if(FAILED(g_lpJoystick->SetDataFormat(&c_dfDIJoystick)))
		{
			MSG_DIFUNC06;
			g_bJoystickIn=FALSE;
		}
	// set joystick x-range
	if(g_bJoystickIn)
	{
		// joystick range variable
		DIPROPRANGE JoystickRange;
		INIT_VAR(JoystickRange);

		// set range
		JoystickRange.lMin=-5;
		JoystickRange.lMax=5;
		JoystickRange.diph.dwSize=sizeof(DIPROPRANGE);
		JoystickRange.diph.dwHeaderSize=sizeof(DIPROPHEADER);
		JoystickRange.diph.dwObj=DIJOFS_X;
		JoystickRange.diph.dwHow=DIPH_BYOFFSET;
		if(FAILED(g_lpJoystick->SetProperty(DIPROP_RANGE,
										    &JoystickRange.diph)))
		{
			MSG_DIFUNC07;
			g_bJoystickIn=FALSE;
		}
	}
	// set joystick y-range
	if(g_bJoystickIn)
	{
		// joystick range variable
		DIPROPRANGE JoystickRange;
		INIT_VAR(JoystickRange);

		// set range
		JoystickRange.lMin=-5;
		JoystickRange.lMax=5;
		JoystickRange.diph.dwSize=sizeof(DIPROPRANGE);
		JoystickRange.diph.dwHeaderSize=sizeof(DIPROPHEADER);
		JoystickRange.diph.dwObj=DIJOFS_Y;
		JoystickRange.diph.dwHow=DIPH_BYOFFSET;
		if(FAILED(g_lpJoystick->SetProperty(DIPROP_RANGE,
										    &JoystickRange.diph)))
		{
			MSG_DIFUNC08;
			g_bJoystickIn=FALSE;
		}
	}
	// set joystick x-axis dead zone
	if(g_bJoystickIn)
	{
		// dead zone variable
		DIPROPDWORD JoystickDeadZone;
		INIT_VAR(JoystickDeadZone);

		// set dead zone
		JoystickDeadZone.diph.dwSize=sizeof(DIPROPDWORD);
		JoystickDeadZone.diph.dwHeaderSize=sizeof(JoystickDeadZone.diph);
		JoystickDeadZone.diph.dwObj=DIJOFS_X;
		JoystickDeadZone.diph.dwHow=DIPH_BYOFFSET;
		JoystickDeadZone.dwData=1000;
		if(FAILED(g_lpJoystick->SetProperty(DIPROP_DEADZONE,
										    &JoystickDeadZone.diph)))
		{
			MSG_DIFUNC09;
			g_bJoystickIn=FALSE;
		}
	}
	// set joystick y-axis dead zone
	if(g_bJoystickIn)
	{
		// dead zone variable
		DIPROPDWORD JoystickDeadZone;
		INIT_VAR(JoystickDeadZone);

		// set dead zone
		JoystickDeadZone.diph.dwSize=sizeof(DIPROPDWORD);
		JoystickDeadZone.diph.dwHeaderSize=sizeof(JoystickDeadZone.diph);
		JoystickDeadZone.diph.dwObj=DIJOFS_Y;
		JoystickDeadZone.diph.dwHow=DIPH_BYOFFSET;
		JoystickDeadZone.dwData=1000;
		if(FAILED(g_lpJoystick->SetProperty(DIPROP_DEADZONE,
										    &JoystickDeadZone.diph)))
		{
			MSG_DIFUNC10;
			g_bJoystickIn=FALSE;
		}
	}
	// acquire joystick
	if(g_bJoystickIn)
		if(FAILED(g_lpJoystick->Acquire()))
		{
			MSG_DIFUNC11;
			g_bJoystickIn=FALSE;
		}
	// release temporary interface
	SAFE_RELEASE(lpDIDTemp);
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  ReleaseDirectInput()						//
//		(defined in "difunc.h")							//
//														//
//	Release all storage associated with DirectInput.	//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void ReleaseDirectInput(void)
{
	// release joystick
	SAFE_RELEASE(g_lpJoystick);
	// release key data memory
	SAFE_FREE(g_lpKeyData);
	// release keyboard
	SAFE_RELEASE(g_lpKeyboard);
	// release DirectInput
	SAFE_RELEASE(g_lpDI);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalDirectInputPtr()				//
//		(defined in "difunc.h")							//
//														//
//	Retrieve pointer to DirectInput interface.			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDIRECTINPUT: pointer to interface				//
//														//
//////////////////////////////////////////////////////////

extern LPDIRECTINPUT inline GetGlobalDirectInputPtr(void)
{
	return(g_lpDI);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalKeyboardPtr()					//
//		(defined in "difunc.h")							//
//														//
//	Retrieve pointer to DirectInput keyboard interface.	//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDIRECTINPUTDEVICE: pointer to interface		//
//														//
//////////////////////////////////////////////////////////

extern LPDIRECTINPUTDEVICE inline GetGlobalKeyboardPtr(void)
{
	return(g_lpKeyboard);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalKeyDataPtr()					//
//		(defined in "difunc.h")							//
//														//
//	Retrieve pointer to key data.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPBYTE: pointer to data							//
//														//
//////////////////////////////////////////////////////////

extern LPBYTE inline GetGlobalKeyDataPtr(void)
{
	return(g_lpKeyData);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  GetUserInput()							//
//		(defined in "difunc.h")							//
//														//
//	Retrieves current values of keys and/or joystick.	//
//	  Parameters:										//
//		COTROL_DATA_PTR: address of current data		//
//		COTROL_DATA_PTR: address of new data			//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void GetUserInput(CONTROL_DATA_PTR cdCurr,
						 CONTROL_DATA_PTR cdNew)
{
	// previous control state
	static CONTROL_DATA cdOld={0,0,0,0,0,0,0,0,0};
	// clear current data structure
	ZeroMemory(cdCurr,sizeof(CONTROL_DATA));
	// clear new data structure
	ZeroMemory(cdNew,sizeof(CONTROL_DATA));
	// refresh keyboard data
	if(ReadKeyboard())
	{
		// pointer to key data
		LPDWORD lpdwKeys=(LPDWORD)&g_cdKeys;
		// pointer to current data
		LPDWORD lpdwCurr=(LPDWORD)cdCurr;
		// counter
		DWORD dwCount;

		// check each key
		for(dwCount=0;dwCount<9;dwCount++)
		{
			if(IS_KEY_DOWN(*lpdwKeys))
				*lpdwCurr=TRUE;
			// increment pointers
			lpdwKeys++;
			lpdwCurr++;
		}
	}
	// check joystick
	if(ReadJoystick())
	{
		// check axes
		if(g_dijs.lX<-2)
			cdCurr->left=TRUE;
		if(g_dijs.lX>2)
			cdCurr->right=TRUE;
		if(g_dijs.lY<-2)
			cdCurr->up=TRUE;
		if(g_dijs.lY>2)
			cdCurr->down=TRUE;
		// check buttons
		if(g_dijs.rgbButtons[g_cdJoystick.btn1]&0x80)
			cdCurr->btn1=TRUE;
		if(g_dijs.rgbButtons[g_cdJoystick.btn2]&0x80)
			cdCurr->btn2=TRUE;
		if(g_dijs.rgbButtons[g_cdJoystick.btn3]&0x80)
			cdCurr->btn3=TRUE;
		if(g_dijs.rgbButtons[g_cdJoystick.btn4]&0x80)
			cdCurr->btn4=TRUE;
	}
	// eliminate opposing inputs
	if(cdCurr->left&&cdCurr->right)
	{
		cdCurr->left=FALSE;
		cdCurr->right=FALSE;
	}
	if(cdCurr->up&&cdCurr->down)
	{
		cdCurr->up=FALSE;
		cdCurr->down=FALSE;
	}
	// pointer to current data
	LPDWORD lpdwCurr=(LPDWORD)cdCurr;
	// pointer to old data
	LPDWORD lpdwOld=(LPDWORD)&cdOld;
	// pointer to new data
	LPDWORD lpdwNew=(LPDWORD)cdNew;
	// counter
	DWORD dwCount;

	// check each key
	for(dwCount=0;dwCount<9;dwCount++)
	{
		if(*lpdwCurr&&(!(*lpdwOld)))
			*lpdwNew=TRUE;
		// increment pointers
		lpdwCurr++;
		lpdwOld++;
		lpdwNew++;
	}
	// save current data as old data
	CopyMemory(&cdOld,
			   cdCurr,
			   sizeof(CONTROL_DATA));
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetKeyStructure()						//
//		(defined in "difunc.h")							//
//														//
//	Sets current value of keys.							//
//	  Parameters:										//
//		CONTROL_DATA_PTR: address of new data			//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetKeyStructure(CONTROL_DATA_PTR cdNew)
{
	// if data is specified, use it
	if(cdNew)
		g_cdKeys=*cdNew;
	// otherwise, use defaults
	else
	{
		g_cdKeys.up=DIK_UPARROW;
		g_cdKeys.down=DIK_DOWNARROW;
		g_cdKeys.left=DIK_LEFTARROW;
		g_cdKeys.right=DIK_RIGHTARROW;
		g_cdKeys.btn1=DIK_SPACE;
		g_cdKeys.btn2=DIK_RALT;
		g_cdKeys.btn3=DIK_RCONTROL;
		g_cdKeys.btn4=DIK_RSHIFT;
		g_cdKeys.esc=DIK_ESCAPE;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetKeyStructure()						//
//		(defined in "difunc.h")							//
//														//
//	Retrieves current key structure.					//
//	  Parameters:										//
//		CONTROL_DATA_PTR: address to send new data		//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void GetKeyStructure(CONTROL_DATA_PTR cdCurr)
{
	// if pointer is specifed, copy the info
	if(cdCurr)
		*cdCurr=g_cdKeys;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  ScanKeyboard()							//
//		(defined in "difunc.h")							//
//														//
//	Scans keyboard and returns the DIK_* value and a	//
//		string describing it.							//
//	  Parameters:										//
//		LPSTR: string to receive description			//
//		LPDWORD: pointer to key value					//
//	  Return Value:										//
//		BOOL: do not use data if FALSE					//
//														//
//////////////////////////////////////////////////////////

extern BOOL ScanKeyboard(LPSTR lpText,
						 LPDWORD lpdwKey)
{
	// reset return values
	lpText[0]=0;
	*lpdwKey=0;	
	// refresh keyboard data
	if(!ReadKeyboard())
		return(FALSE);
	// macro to scan each key
	#define SCAN_KEY(a,b) { if(g_lpKeyData[a]&0x80) { strcpy(lpText,b); *lpdwKey=a; return(TRUE); } }
	// scan all keys
	SCAN_KEY(DIK_ESCAPE,"Escape");
	SCAN_KEY(DIK_1,"1");
	SCAN_KEY(DIK_2,"2");
	SCAN_KEY(DIK_3,"3");
	SCAN_KEY(DIK_4,"4");
	SCAN_KEY(DIK_5,"5");
	SCAN_KEY(DIK_6,"6");
	SCAN_KEY(DIK_7,"7");
	SCAN_KEY(DIK_8,"8");
	SCAN_KEY(DIK_9,"9");
	SCAN_KEY(DIK_0,"0");
	SCAN_KEY(DIK_MINUS,"Minus");
	SCAN_KEY(DIK_EQUALS,"Equals");
	SCAN_KEY(DIK_BACKSPACE,"Backspace");
	SCAN_KEY(DIK_TAB,"Tab");
	SCAN_KEY(DIK_Q,"Q");
	SCAN_KEY(DIK_W,"W");
	SCAN_KEY(DIK_E,"E");
	SCAN_KEY(DIK_R,"R");
	SCAN_KEY(DIK_T,"T");
	SCAN_KEY(DIK_Y,"Y");
	SCAN_KEY(DIK_U,"U");
	SCAN_KEY(DIK_I,"I");
	SCAN_KEY(DIK_O,"O");
	SCAN_KEY(DIK_P,"P");
	SCAN_KEY(DIK_LBRACKET,"Left Bracket");
	SCAN_KEY(DIK_RBRACKET,"Right Bracket");
	SCAN_KEY(DIK_RETURN,"Enter");
	SCAN_KEY(DIK_LCONTROL,"Left Control");
	SCAN_KEY(DIK_A,"A");
	SCAN_KEY(DIK_S,"S");
	SCAN_KEY(DIK_D,"D");
	SCAN_KEY(DIK_F,"F");
	SCAN_KEY(DIK_G,"G");
	SCAN_KEY(DIK_H,"H");
	SCAN_KEY(DIK_J,"J");
	SCAN_KEY(DIK_K,"K");
	SCAN_KEY(DIK_L,"L");
	SCAN_KEY(DIK_SEMICOLON,"Semicolon");
	SCAN_KEY(DIK_APOSTROPHE,"Apostrophe");
	SCAN_KEY(DIK_GRAVE,"^Grave^ Accent");
	SCAN_KEY(DIK_LSHIFT,"Left Shift");
	SCAN_KEY(DIK_BACKSLASH,"Backslash");
	SCAN_KEY(DIK_Z,"Z");
	SCAN_KEY(DIK_X,"X");
	SCAN_KEY(DIK_C,"C");
	SCAN_KEY(DIK_V,"V");
	SCAN_KEY(DIK_B,"B");
	SCAN_KEY(DIK_N,"N");
	SCAN_KEY(DIK_M,"M");
	SCAN_KEY(DIK_COMMA,"Comma");
	SCAN_KEY(DIK_PERIOD,"Period");
	SCAN_KEY(DIK_SLASH,"Slash");
	SCAN_KEY(DIK_RSHIFT,"Right Shift");
	SCAN_KEY(DIK_NUMPADSTAR,"Asterisk (Keypad)");
	SCAN_KEY(DIK_LALT,"Left Alt");
	SCAN_KEY(DIK_SPACE,"Spacebar");
	SCAN_KEY(DIK_CAPSLOCK,"Caps Lock");
	SCAN_KEY(DIK_F1,"F1");
	SCAN_KEY(DIK_F2,"F2");
	SCAN_KEY(DIK_F3,"F3");
	SCAN_KEY(DIK_F4,"F4");
	SCAN_KEY(DIK_F5,"F5");
	SCAN_KEY(DIK_F6,"F6");
	SCAN_KEY(DIK_F7,"F7");
	SCAN_KEY(DIK_F8,"F8");
	SCAN_KEY(DIK_F9,"F9");
	SCAN_KEY(DIK_F10,"F10");
	SCAN_KEY(DIK_NUMLOCK,"Num Lock");
	SCAN_KEY(DIK_SCROLL,"Scroll Lock");
	SCAN_KEY(DIK_NUMPAD7,"7 (Keypad)");
	SCAN_KEY(DIK_NUMPAD8,"8 (Keypad)");
	SCAN_KEY(DIK_NUMPAD9,"9 (Keypad)");
	SCAN_KEY(DIK_NUMPADMINUS,"Minus (Keypad)");
	SCAN_KEY(DIK_NUMPAD4,"4 (Keypad)");
	SCAN_KEY(DIK_NUMPAD5,"5 (Keypad)");
	SCAN_KEY(DIK_NUMPAD6,"6 (Keypad)");
	SCAN_KEY(DIK_NUMPADPLUS,"Plus (Keypad)");
	SCAN_KEY(DIK_NUMPAD1,"1 (Keypad)");
	SCAN_KEY(DIK_NUMPAD2,"2 (Keypad)");
	SCAN_KEY(DIK_NUMPAD3,"3 (Keypad)");
	SCAN_KEY(DIK_NUMPAD0,"0 (Keypad)");
	SCAN_KEY(DIK_NUMPADPERIOD,"Period (Keypad)");
	SCAN_KEY(DIK_F11,"F11");
	SCAN_KEY(DIK_F12,"F12");
	SCAN_KEY(DIK_F13,"F13");
	SCAN_KEY(DIK_F14,"F14");
	SCAN_KEY(DIK_F15,"F15");
	SCAN_KEY(DIK_KANA,"Kana");
	SCAN_KEY(DIK_CONVERT,"Convert");
	SCAN_KEY(DIK_NOCONVERT,"No Convert");
	SCAN_KEY(DIK_YEN,"Yen");
	SCAN_KEY(DIK_NUMPADEQUALS,"Equals (Keypad)");
	SCAN_KEY(DIK_CIRCUMFLEX,"Circumflex");
	SCAN_KEY(DIK_AT,"At");
	SCAN_KEY(DIK_COLON,"Colon");
	SCAN_KEY(DIK_UNDERLINE,"Underline");
	SCAN_KEY(DIK_KANJI,"Kanji");
	SCAN_KEY(DIK_STOP,"Stop");
	SCAN_KEY(DIK_AX,"Ax");
	SCAN_KEY(DIK_UNLABELED,"Unlabeled");
	SCAN_KEY(DIK_NUMPADENTER,"Enter (Keypad)");
	SCAN_KEY(DIK_RCONTROL,"Right Control");
	SCAN_KEY(DIK_NUMPADCOMMA,"Comma (Keypad)");
	SCAN_KEY(DIK_NUMPADSLASH,"Slash (Keypad)");
	SCAN_KEY(DIK_SYSRQ,"Print Screen");
	SCAN_KEY(DIK_RALT,"Right Alt");
	SCAN_KEY(DIK_HOME,"Home");
	SCAN_KEY(DIK_UPARROW,"Up Arrow");
	SCAN_KEY(DIK_PGUP,"Page Up");
	SCAN_KEY(DIK_LEFTARROW,"Left Arrow");
	SCAN_KEY(DIK_RIGHTARROW,"Right Arrow");
	SCAN_KEY(DIK_END,"End");
	SCAN_KEY(DIK_DOWN,"Down Arrow");
	SCAN_KEY(DIK_PGDN,"Page Down");
	SCAN_KEY(DIK_INSERT,"Insert");
	SCAN_KEY(DIK_DELETE,"Delete");
	SCAN_KEY(DIK_LWIN,"Left Windows");
	SCAN_KEY(DIK_RWIN,"Right Windows");
	SCAN_KEY(DIK_APPS,"Application");
	// nothing found
	return(FALSE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SelectJoystick()							//
//		(defined in "difunc.h")							//
//														//
//	Allows user to select a joystick.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL SelectJoystick(void)
{
	// our storage structure
	JOYSTICKDEVICELIST JoystickDeviceList;
	INIT_VAR(JoystickDeviceList);
	// used for selection window
	LPSTR lpText=NULL;
	// selection
	DWORD dwSelection=0;
	// temporary DirectInput interface pointer
	LPDIRECTINPUT lpDITemp=NULL;

	// init global variables
	INIT_VAR(g_guidJoystick);
	g_bJoystickActive=FALSE;
	// create temporary interface
	if(FAILED(DirectInputCreate(GetGlobalHInst(),
								DIRECTINPUT_VERSION,
								&lpDITemp,
								NULL)))
	{
		ERR_DIFUNC00;
		return(FALSE);
	}
	// begin enumeration
	if(FAILED(lpDITemp->EnumDevices(DIDEVTYPE_JOYSTICK,
									EnumJoysticksCallback,
									(LPVOID)&JoystickDeviceList,
									DIEDFL_ATTACHEDONLY)))
	{
		MSG_DIFUNC02;
		// release temporary pointer
		SAFE_RELEASE(lpDITemp);
		return(TRUE);
	}
	// check for error
	if(JoystickDeviceList.dwNumDevices==0)
	{
		MSG_DIFUNC02;
		// release temporary pointer
		SAFE_RELEASE(lpDITemp);
		return(TRUE);
	}
	// if only one, use it
	if(JoystickDeviceList.dwNumDevices==1)
	{
		// let user know
		MSG_DIFUNC03;
		// select it
		dwSelection=0;
	}
	// otherwise let user pick
	else
	{
		// allocate memory for window text
		SAFE_ALLOC(lpText,
				   LPSTR,
				   (JoystickDeviceList.dwNumDevices*160)+256);
		// loop until selection is made
		while(TRUE)
		{
			// counter 
			DWORD dwCount=0;
			// temporary text
			char lpTemp[8];
			INIT_VAR(lpTemp);

			// fill in text
			strcpy(lpText,"Please select a joystick device to use:\n\n");
			// add devices
			for(dwCount=0;dwCount<JoystickDeviceList.dwNumDevices;dwCount++)
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
				strcat(lpText,JoystickDeviceList.JoystickDevicePtr[dwCount].lpDeviceName);
				// add device description
				strcat(lpText," (");
				strcat(lpText,JoystickDeviceList.JoystickDevicePtr[dwCount].lpDeviceDesc);
				strcat(lpText,")\n");
			}
			// add instructions
			strcat(lpText,"\nClick 'Yes' to accept this choice or 'No' to select the next one.");
			// check for selection
			if(MessageBox(GetGlobalHWnd(),
						  lpText,
						  "Select Joystick Device",
						  MB_YESNO)==IDYES)
				break;
			// keep going
			dwSelection++;
			if(dwSelection>=JoystickDeviceList.dwNumDevices)
				dwSelection=0;			
		}
		// free memory
		SAFE_FREE(lpText);
	}
	// copy information to globals
	g_guidJoystick=JoystickDeviceList.JoystickDevicePtr[dwSelection].guidJoystickGUID;
	g_bJoystickActive=TRUE;
	// release memory
	SAFE_FREE(JoystickDeviceList.JoystickDevicePtr);
	// looks good to me
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetJoystickButtons()						//
//		(defined in "difunc.h")							//
//														//
//	Sets joystick buttons settings.						//
//	  Parameters:										//
//		CONTROL_DATA_PTR: address of new values			//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void SetJoystickButtons(CONTROL_DATA_PTR cdNew)
{
	// if pointer is specifed, copy data
	if(cdNew)
	{
		g_cdJoystick.btn1=cdNew->btn1;
		g_cdJoystick.btn2=cdNew->btn2;
		g_cdJoystick.btn3=cdNew->btn3;
		g_cdJoystick.btn4=cdNew->btn4;
	}
	// otherwise use defaults
	else
	{
		g_cdJoystick.btn1=0;
		g_cdJoystick.btn2=1;
		g_cdJoystick.btn3=2;
		g_cdJoystick.btn4=3;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetJoystickButtons()						//											//
//		(defined in "difunc.h")							//
//														//
//	Retrieves joystick button settings.					//
//	  Parameters:										//
//		CONTROL_DATA_PTR: address of values				//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void GetJoystickButtons(CONTROL_DATA_PTR cdCurr)
{
	// if pointer is specifed, use it
	if(cdCurr)
	{
		cdCurr->btn1=g_cdJoystick.btn1;
		cdCurr->btn2=g_cdJoystick.btn2;
		cdCurr->btn3=g_cdJoystick.btn3;
		cdCurr->btn4=g_cdJoystick.btn4;
	}
}
		
//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalJoystickPtr()					//
//		(defined in "difunc.h")							//
//														//
//	Retrieves pointer to joystick interface.			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDIRECTINPUTDEVICE2: pointer to interface		//
//														//
//////////////////////////////////////////////////////////

extern LPDIRECTINPUTDEVICE2 inline GetGlobalJoystickPtr(void)
{
	return(g_lpJoystick);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  AcquireKeyboard()						//
//														//
//	Acquires keyboard for input.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: FALSE if unsuccessful						//
//														//
//////////////////////////////////////////////////////////

BOOL AcquireKeyboard(void)
{
	// acquire keyboard
	if(FAILED(g_lpKeyboard->Acquire()))
	{
		ERR_DIFUNC04;
		return(FALSE);
	}
	return(TRUE);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  ReadKeyboard()							//
//														//
//	Reads keyboard input.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: FALSE if unsuccessful						//
//														//
//////////////////////////////////////////////////////////

BOOL ReadKeyboard(void)
{
	// variable to save result
	HRESULT hResult;

	// loop until valid data is returned or error is encountered
	while(hResult=g_lpKeyboard->GetDeviceState(256,
								 			   (LPVOID)g_lpKeyData)==DIERR_INPUTLOST)
	{
		if(!AcquireKeyboard())
			return(FALSE);
	}
	// check for error
	if(FAILED(hResult))
	{
		ERR_DIFUNC05;
		return(FALSE);
	}
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
//	Function:  ReadJoystick()							//
//														//
//	Reads joystick input.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: FALSE if unsuccessful						//
//														//
//////////////////////////////////////////////////////////

BOOL ReadJoystick(void)
{
	//  only read joystick if it is active ...
	if(g_bJoystickActive)
	{
		// ...  and plugged in
		if(g_bJoystickIn)
		{
			// poll joystick
			if(FAILED(g_lpJoystick->Poll()))
			{
				MSG_DIFUNC00;
				// deactivate joystick
				g_bJoystickIn=FALSE;
				return(FALSE);
			}
			// read joystick
			if(FAILED(g_lpJoystick->GetDeviceState(sizeof(DIJOYSTATE),
												   (LPVOID)&g_dijs)))
			{
				MSG_DIFUNC01;
				// deactivate joystick
				g_bJoystickIn=FALSE;
				return(FALSE);
			}
		}
		// otherwise see if joystick was re-connected
		else
		{
			// poll joystick
			if(SUCCEEDED(g_lpJoystick->Poll()))
			{
				// re-activate joystick
				g_bJoystickIn=TRUE;
				return(FALSE);
			}
		}
	}
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalJoystickFlag()					//
//		(defined in "difunc.h")							//
//														//
//	Retrieve joystick active flag.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: Value of flag.							//
//														//
//////////////////////////////////////////////////////////

extern BOOL inline GetGlobalJoystickFlag(void)
{
	return(g_bJoystickActive);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalJoystickGUID()					//
//		(defined in "difunc.h")							//
//														//
//	Retrive global joystick GUID.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		GUID: the requested GUID						//
//														//
//////////////////////////////////////////////////////////

extern GUID inline GetGlobalJoystickGUID(void)
{
	return(g_guidJoystick);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  EnumJoysticksCallback()					//
//														//
//	Enumerate available joysticks.						//
//	  Parameters:										//
//		LPCDIDEVICEINSTANCE: pointer to device data		//
//		LPVOID: user defined data						//
//	  Return Value:										//
//		This is a callback function.  Return DIENUM_-   //
//		CONTINUE to continue.							//
//														//
//////////////////////////////////////////////////////////

BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE lpDID,
								    LPVOID lpUserArg)
{
	// pointer to our data structure
	JOYSTICKDEVICELIST_PTR JoystickDeviceListPtr=(JOYSTICKDEVICELIST_PTR)lpUserArg;
	// pointer to current device
	JOYSTICKDEVICE_PTR JoystickDevicePtr=NULL;

	// create storage structure if necessary
	if(JoystickDeviceListPtr->dwNumDevices==0)
	{
		// allocate memory
		SAFE_ALLOC(JoystickDeviceListPtr->JoystickDevicePtr,
				   JOYSTICKDEVICE_PTR,
				   sizeof(JOYSTICKDEVICE));
		// check for errors
		if(!JoystickDeviceListPtr->JoystickDevicePtr)
		{	DISPLAY_INFO(GetGlobalHWnd(),"UhOh!");
			return(DIENUM_CONTINUE);
		}
		// set pointer to device data
		JoystickDevicePtr=JoystickDeviceListPtr->JoystickDevicePtr;
	}
	// otherwise expand storage structure
	else
	{
		// temporary storage
		LPVOID lpTemp=NULL;

		// allocate memory
		SAFE_ALLOC(lpTemp,
				   LPVOID,
				   sizeof(JOYSTICKDEVICE)*(JoystickDeviceListPtr->dwNumDevices+1));
		// check for errors
		if(!lpTemp)
			return(DIENUM_CONTINUE);
		// copy data
		CopyMemory(lpTemp,
				   JoystickDeviceListPtr->JoystickDevicePtr,
				   sizeof(JOYSTICKDEVICE)*JoystickDeviceListPtr->dwNumDevices);
		// swap pointers
		SwapPointers(&lpTemp,
					 (LPVOID*)&JoystickDeviceListPtr->JoystickDevicePtr);
		// out with the old!
		SAFE_FREE(lpTemp);
		// set pointer to device data
		JoystickDevicePtr=JoystickDeviceListPtr->JoystickDevicePtr;
	}
	// save GUID
	CopyMemory(&JoystickDevicePtr[JoystickDeviceListPtr->dwNumDevices].guidJoystickGUID,
			   &lpDID->guidInstance,
			   sizeof(GUID));
	// save device name
	CopyMemory(JoystickDevicePtr[JoystickDeviceListPtr->dwNumDevices].lpDeviceName,
			   lpDID->tszProductName,
			   80);
	JoystickDevicePtr[JoystickDeviceListPtr->dwNumDevices].lpDeviceName[80]=0;
	// save device description
	CopyMemory(JoystickDevicePtr[JoystickDeviceListPtr->dwNumDevices].lpDeviceDesc,
			   lpDID->tszInstanceName,
			   80);
	JoystickDevicePtr[JoystickDeviceListPtr->dwNumDevices].lpDeviceDesc[80]=0;
	// increment counter
	JoystickDeviceListPtr->dwNumDevices++;
	// ok
	return(DIENUM_CONTINUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalJoystickGUID()					//
//		(defined in "difunc.h")							//
//														//
//	Set global joystick GUID.							//
//	  Parameters:										//
//		GUID: GUID to set								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void inline SetGlobalJoystickGUID(GUID guid)
{
	g_guidJoystick=guid;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalJoystickFlag()					//
//		(defined in "difunc.h")							//
//														//
//	Set global joystick flag.							//
//	  Parameters:										//
//		BOOL: flag to set								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void inline SetGlobalJoystickFlag(BOOL bFlag)
{
	g_bJoystickActive=bFlag;
}

// END ///////////////////////////////////////////////////

