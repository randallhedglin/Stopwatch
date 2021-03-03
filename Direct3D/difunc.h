// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  difunc.h										//
//														//
//	External functions for DirectInput.					//
//														//
//////////////////////////////////////////////////////////

// PROTOTYPES ////////////////////////////////////////////

extern BOOL InitializeDirectInput(void);

extern void ReleaseDirectInput(void);

extern LPDIRECTINPUT inline GetGlobalDirectInputPtr(void);

extern LPDIRECTINPUTDEVICE inline GetGlobalKeyboardPtr(void);

extern LPBYTE inline GetGlobalKeyDataPtr(void);

extern void GetUserInput(CONTROL_DATA_PTR cdCurr,
						 CONTROL_DATA_PTR cdNew);

extern void SetKeyStructure(CONTROL_DATA_PTR cdNew);

extern void GetKeyStructure(CONTROL_DATA_PTR cdCurr);

extern BOOL ScanKeyboard(LPSTR lpText,
						 LPDWORD lpdwKey);

extern BOOL SelectJoystick(void);

extern void SetJoystickButtons(CONTROL_DATA_PTR cdNew);

extern void GetJoystickButtons(CONTROL_DATA_PTR cdCurr);
		
extern LPDIRECTINPUTDEVICE2 inline GetGlobalJoystickPtr(void);

extern GUID inline GetGlobalJoystickGUID(void);

extern void inline SetGlobalJoystickGUID(GUID guid);

extern BOOL inline GetGlobalJoystickFlag(void);

extern void inline SetGlobalJoystickFlag(BOOL bFlag);

// MACROS ////////////////////////////////////////////////

#define IS_KEY_DOWN(a) (GetGlobalKeyDataPtr()[a]&0x80)

// END ///////////////////////////////////////////////////

