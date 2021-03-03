// NEEDS OPTIMIZATION

//////////////////////////////////////////////////////////
//														//
//  File:  mainprog.cpp									//
//														//
//	This is where you (or I, as the case may be)		//
//	program this Direct3D application.					//
//  NOTE:  Before compiling, move the DATA and          //
//  SOURCE directories to the application's root!!      //
//														//
//////////////////////////////////////////////////////////

// main include file
#include"..\direct3d\primary.h"
// header for this module
#include"..\direct3d\mainprog.h"
// allow access to window handles
#include"..\direct3d\winmain.h"
// allow access to file handling functions
#include"..\direct3d\filefunc.h"
// allow access to DirectDraw
#include"..\direct3d\ddfunc.h"
// allow access to DirectInput
#include"..\direct3d\difunc.h"
// allow access to bitmaps
#include"..\direct3d\bmpfunc.h"
// allow access to fonts
#include"..\direct3d\fontfunc.h"
// allow access to movies
#include"..\direct3d\mvefunc.h"
// allow access to sound
#include"..\direct3d\dsfunc.h"
// allow access to music
#include"..\direct3d\dmfunc.h"
// allow access to raw data
#include"..\direct3d\rdfunc.h"
// allow access to Direct3D
#include"..\direct3d\d3dfunc.h"
// allow access to materials
#include"..\direct3d\matfunc.h"
// allow access to lights
#include"..\direct3d\ltfunc.h"
// allow access to textures
#include"..\direct3d\txfunc.h"
// allow access to models
#include"..\direct3d\mdlfunc.h"
// allow access to scenery spheres
#include"..\direct3d\scnfunc.h"
// allow access to cameras
#include"..\direct3d\camfunc.h"

// USER INCLUDES /////////////////////////////////////////

// USER PROTOTYPES ///////////////////////////////////////

void EncodeData(void);

// MESSAGES //////////////////////////////////////////////

// configuration file creation message
#define MSG_FILEFUNC00 DISPLAY_INFO(GetGlobalHWnd(),"This program is not configured.\nOption selection windows will follow.");

// REQ'D FUNCTIONS ///////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//  Function:  __InitializeSystem()						//
//	  (defined in "mainprog.h")							//
//														//
//  Prepares system for application execution.			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL __InitializeSystem(void)
{
	// path
	char lpPath[_MAX_PATH];

	// never the same game twice!
	srand(GetTickCount());
	// get current path
	_getcwd(lpPath,_MAX_PATH);
	// add "\data" to it
	strcat(lpPath,"\\Data");
	// set config path
	SetConfigurationPath(lpPath);
	// if configuration file exists, open it and read it
	if(IsExistingConfigurationFile("config.app"))
	{
		// temporary storage for control data
		CONTROL_DATA cdTemp;
		INIT_VAR(cdTemp);
		
		// open file
		OpenConfigurationFile("config.app");
		// input DirectDraw values
		SetGlobalDDDeviceGUID(LoadConfigurationGUID());
		SetGlobalDDDefaultFlag(LoadConfigurationFlag());
		// input Direct3D values
		SetGlobalD3DDeviceGUID(LoadConfigurationGUID());
		SetGlobalD3DHardwareFlag(LoadConfigurationFlag());
		SetGlobalUseZBufferFlag(LoadConfigurationFlag());
		// input resolution values
		SetGlobalResolutionFlag(LoadConfigurationFlag());
		SetGlobalViewportSizeFlag(LoadConfigurationFlag());
		// input joystick values
		SetGlobalJoystickGUID(LoadConfigurationGUID());
		SetGlobalJoystickFlag(LoadConfigurationFlag());
		// input control data
		LoadConfigurationControlData(&cdTemp);
		SetKeyStructure(&cdTemp);
		LoadConfigurationControlData(&cdTemp);
		SetJoystickButtons(&cdTemp);
		// input DirectSound values
		SetGlobalSoundEnabledFlag(LoadConfigurationFlag());
		// input DirectMusic values
		SetGlobalMusicEnabledFlag(LoadConfigurationFlag());
		// input fog value
		SetGlobalUseFogFlag(LoadConfigurationFlag());
		// input anti-alias value
		SetGlobalUseAntiAliasFlag(LoadConfigurationFlag());
		// input Gouraud value
		SetGlobalUseGouraudFlag(LoadConfigurationFlag());
		// input specular value
		SetGlobalUseSpecularFlag(LoadConfigurationFlag());
		// input alpha value
		SetGlobalUseAlphaFlag(LoadConfigurationFlag());
		// input perspective-correct value
		SetGlobalUsePerspectiveCorrectFlag(LoadConfigurationFlag());
		// input linear texturing value
		SetGlobalD3DLinearFlag(LoadConfigurationFlag());
		// input triple buffering value
		SetGlobalUseTripleBufferFlag(LoadConfigurationFlag());
		// input data path
		LoadConfigurationDataPath();
		// close configuration file
		CloseConfigurationFile();
	}
	// otherwise create it
	else
	{
		// temporary storage for control data
		CONTROL_DATA cdTemp;
		INIT_VAR(cdTemp);
		
		// let user know what is happening
		MSG_FILEFUNC00;
		// create file
		CreateConfigurationFile("config.app");
		// let user select DirectDraw device
		if(!SelectDDDevice())
			return(FALSE);
		// output values
		WriteConfigurationGUID(GetGlobalDDDeviceGUID());
		WriteConfigurationFlag(GetGlobalDDDefaultFlag());
		// let user select Direct3D device
		if(!SelectD3DDevice())
			return(FALSE);
		// output values
		WriteConfigurationGUID(GetGlobalD3DDeviceGUID());
		WriteConfigurationFlag(GetGlobalD3DHardwareFlag());
		WriteConfigurationFlag(GetGlobalUseZBufferFlag());
		// let user select resoultion
		SelectResolution();
		// output values
		WriteConfigurationFlag(GetGlobalResolutionFlag());
		// let user select viewport size
		SelectViewportSize();
		// output values
		WriteConfigurationFlag(GetGlobalViewportSizeFlag());
		// let user select joystick device
		if(!SelectJoystick())
			return(FALSE);
		// output values
		WriteConfigurationGUID(GetGlobalJoystickGUID());
		WriteConfigurationFlag(GetGlobalJoystickFlag());
		// reset keys
		SetKeyStructure(NULL);
		// output values
		GetKeyStructure(&cdTemp);
		WriteConfigurationControlData(&cdTemp);
		// reset joystick buttons
		SetJoystickButtons(NULL);
		// output values
		GetJoystickButtons(&cdTemp);
		WriteConfigurationControlData(&cdTemp);
		// test DirectSound
		TestDirectSound();		
		// output values
		WriteConfigurationFlag(GetGlobalSoundEnabledFlag());
		// test DirectMusic
		TestDirectMusic();		
		// output values
		WriteConfigurationFlag(GetGlobalMusicEnabledFlag());
		// query for fog
		/*if(MessageBox(GetGlobalHWnd(),
					  "Do you want to enable fog effects?",
					  "Query",
					  MB_ICONQUESTION|MB_YESNO)==IDYES)*/
			SetGlobalUseFogFlag(TRUE);
		/*else
			SetGlobalUseFogFlag(FALSE);*/
		// output value
		WriteConfigurationFlag(GetGlobalUseFogFlag());
		// query for anti-alias
		/*if(MessageBox(GetGlobalHWnd(),
					  "Do you want to enable edge anti-aliasing?",
					  "Query",
					  MB_ICONQUESTION|MB_YESNO)==IDYES)
			SetGlobalUseAntiAliasFlag(TRUE);
		else*/
			SetGlobalUseAntiAliasFlag(FALSE);
		// output value
		WriteConfigurationFlag(GetGlobalUseAntiAliasFlag());
		// query for Gouraud
		if(MessageBox(GetGlobalHWnd(),
					  "Do you want to enable smooth shading?",
					  "Query",
					  MB_ICONQUESTION|MB_YESNO)==IDYES)
			SetGlobalUseGouraudFlag(TRUE);
		else
			SetGlobalUseGouraudFlag(FALSE);
		// output value
		WriteConfigurationFlag(GetGlobalUseGouraudFlag());
		// query for specular
		/*if(MessageBox(GetGlobalHWnd(),
					  "Do you want to enable specular highlights?",
					  "Query",
					  MB_ICONQUESTION|MB_YESNO)==IDYES)
			SetGlobalUseSpecularFlag(TRUE);
		else*/
			SetGlobalUseSpecularFlag(FALSE);
		// output value
		WriteConfigurationFlag(GetGlobalUseSpecularFlag());
		// query for alpha
		/*if(MessageBox(GetGlobalHWnd(),
					  "Do you want to enable alpha transparency?",
					  "Query",
					  MB_ICONQUESTION|MB_YESNO)==IDYES)*/
			SetGlobalUseAlphaFlag(TRUE);
		/*else
			SetGlobalUseAlphaFlag(FALSE);*/
		// output value
		WriteConfigurationFlag(GetGlobalUseAlphaFlag());
		// query for prespective-correct
		if(MessageBox(GetGlobalHWnd(),
					  "Do you want to enable texture perspective correction?",
					  "Query",
					  MB_ICONQUESTION|MB_YESNO)==IDYES)
			SetGlobalUsePerspectiveCorrectFlag(TRUE);
		else
			SetGlobalUsePerspectiveCorrectFlag(FALSE);
		// output value
		WriteConfigurationFlag(GetGlobalUsePerspectiveCorrectFlag());
		// query for linear texturing
		if(MessageBox(GetGlobalHWnd(),
					  "Do you want to enable bi-linear texture mapping?",
					  "Query",
					  MB_ICONQUESTION|MB_YESNO)==IDYES)
			SetGlobalD3DLinearFlag(TRUE);
		else
			SetGlobalD3DLinearFlag(FALSE);
		// output value
		WriteConfigurationFlag(GetGlobalD3DLinearFlag());
		// query for triple buffering
		if(MessageBox(GetGlobalHWnd(),
					  "Do you want to enable triple buffering?",
					  "Query",
					  MB_ICONQUESTION|MB_YESNO)==IDYES)
			SetGlobalUseTripleBufferFlag(TRUE);
		else
			SetGlobalUseTripleBufferFlag(FALSE);
		// output value
		WriteConfigurationFlag(GetGlobalUseTripleBufferFlag());
		// set data path
		SetDataPath(lpPath);		
		// save data path
		SaveConfigurationDataPath();
		// close configuration file
		CloseConfigurationFile();
	}
	// encode data
	EncodeData();
	// bye-bye Mr. Mouse!!
	while(ShowCursor(FALSE)>=0)
	{}
	// init DirectDraw
	if(!InitializeDirectDraw())
		return(FALSE);
	// update 3D settings
	UpdateSettings();
	// enable z-buffer
	EnableZBuffer();
	// enable z-buffer writes
	EnableZBufferWrites();
	// enable backface culling
	EnableBackFaceCull();
	// disable alpha
	DisableAlpha();
	// disable fog
	DisableFog();
	// set tiled texture mode
	SetTiledTextureMode(0);
	// init DirectInput
	if(!InitializeDirectInput())
		return(FALSE);
	// init DirectSound
	InitializeDirectSound();
	// init DirectMusic
	InitializeDirectMusic();
	// verify data
	if(!VerifyDataFiles("verify.rec"))
		return(FALSE);
	// all is well
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//  Function:  __RestoreSystem()						//
//	  (defined in "mainprog.h")							//
//														//
//  Returns system to its original state.				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void __RestoreSystem(void)
{
	// temporary storage for control data
	CONTROL_DATA cdTemp;
	INIT_VAR(cdTemp);
		
	// save configuration
	CreateConfigurationFile("config.app");
	// output values
	WriteConfigurationGUID(GetGlobalDDDeviceGUID());
	WriteConfigurationFlag(GetGlobalDDDefaultFlag());
	WriteConfigurationGUID(GetGlobalD3DDeviceGUID());
	WriteConfigurationFlag(GetGlobalD3DHardwareFlag());
	WriteConfigurationFlag(GetGlobalUseZBufferFlag());
	WriteConfigurationFlag(GetGlobalResolutionFlag());
	WriteConfigurationFlag(GetGlobalViewportSizeFlag());
	WriteConfigurationGUID(GetGlobalJoystickGUID());
	WriteConfigurationFlag(GetGlobalJoystickFlag());
	GetKeyStructure(&cdTemp);
	WriteConfigurationControlData(&cdTemp);
	GetJoystickButtons(&cdTemp);
	WriteConfigurationControlData(&cdTemp);
	WriteConfigurationFlag(GetGlobalSoundEnabledFlag());
	WriteConfigurationFlag(GetGlobalMusicEnabledFlag());
	WriteConfigurationFlag(GetGlobalUseFogFlag());
	WriteConfigurationFlag(GetGlobalUseAntiAliasFlag());
	WriteConfigurationFlag(GetGlobalUseGouraudFlag());
	WriteConfigurationFlag(GetGlobalUseSpecularFlag());
	WriteConfigurationFlag(GetGlobalUseAlphaFlag());
	WriteConfigurationFlag(GetGlobalUsePerspectiveCorrectFlag());
	WriteConfigurationFlag(GetGlobalD3DLinearFlag());
	WriteConfigurationFlag(GetGlobalUseTripleBufferFlag());
	SaveConfigurationDataPath();
	// close configuration file
	CloseConfigurationFile();
	// release DirectMusic
	ReleaseDirectMusic();
	// release DirectSound
	ReleaseDirectSound();
	// release DirectInput
	ReleaseDirectInput();
	// release DirectDraw
	ReleaseDirectDraw();
	// now, where did I leave that mouse?
	while(ShowCursor(TRUE)<0)
	{}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//  Function:  __MainProgramLoop()						//
//	  (defined in "mainprog.h")							//
//														//
//  Main program loop (executed once per cycle)			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: FALSE to stop execution					//
//														//
//////////////////////////////////////////////////////////

extern BOOL inline __MainProgramLoop(void)
{
	// prepare timer
	INIT_TIMER;
	// prepare frame counter
	INIT_FPS;
	// user input data storage
	static CONTROL_DATA cdCurr={0,0,0,0,0,0,0,0,0};
	static CONTROL_DATA cdNew={0,0,0,0,0,0,0,0,0};
	// end flag
	BOOL bEndExec=FALSE;
	
// BEGIN PROGRAM LOOP ////////////////////////////////////

#define ST_SELECT 1

#define ST2_SELECT_INIT 1
#define ST2_SELECT_EXEC 2
#define ST2_SELECT_DONE 3

#define ST_COMPLETE 2

// state holders
static DWORD State=ST_SELECT;
static DWORD State2=ST2_SELECT_INIT;

// permanent data items
static CDDFont* Font=NULL;
static CDSSound* Ding=NULL;

// timekeepers
static DWORD dwTimer=0;
static DWORD dwLastTick=GetTickCount();
static BOOL bRunning=FALSE;
static DWORD dwTarget=0;
static BOOL bReached=FALSE;

// check state
switch(State)
{
// INTRODUCTION //////////////////////////////////////////
case(ST_SELECT):
	switch(State2)
	{
//// INITIALIZE INTRO ////////////////////////////////////
	case(ST2_SELECT_INIT):
		// create data items
		SAFE_NEW(Font,CDDFont);
		SAFE_NEW(Ding,CDSSound);

		// load data
		OpenDataFile("intro.dat");
		{
			Font->Load();
			Ding->Load();
		}
		CloseDataFile();

		// increment state
		State2++;
		break;
//// EXECUTE INTRO ///////////////////////////////////////
	case(ST2_SELECT_EXEC):
		// check for input
		GetUserInput(&cdCurr,&cdNew);

		// increment timer if running
		if(bRunning)
		{
			DWORD dwTemp=GetTickCount();
			dwTimer+=dwTemp-dwLastTick;
			dwLastTick=dwTemp;
		}
		else
			dwLastTick=GetTickCount();
		// update flag
		if(cdNew.btn1)
		{
			if(bRunning)
				bRunning=FALSE;
			else
				bRunning=TRUE;;
		}
		// reset counter
		if(cdNew.btn4)
		{
			bRunning=FALSE;
			dwTimer=0;
		}
		// adjust target
		if(cdNew.up||cdNew.right)
		{
			dwTarget+=1000*60;
			bReached=FALSE;
		}
		if(cdNew.down||cdNew.left)
		{
			dwTarget-=1000*60;
			bReached=FALSE;
		}
		// ding if necessary
		if(dwTarget&&dwTimer>dwTarget&&!bReached)
		{
			Ding->Play();
			bReached=TRUE;
		}
		{
		// extract digits
		DWORD dwMs=dwTimer%1000;
		DWORD dwMs3=dwMs%10;
		DWORD dwMs2=((dwMs-dwMs3)/10)%10;
		DWORD dwMs1=((dwMs-((dwMs2*10)+dwMs3))/100)%10;
		DWORD dwS=((dwTimer-dwMs)/1000)%60;
		DWORD dwS2=dwS%10;
		DWORD dwS1=(dwS-dwS2)/10;
		DWORD dwM=((dwTimer-((dwS*1000)+dwMs))/(1000*60))%60;
		DWORD dwM2=dwM%10;
		DWORD dwM1=(dwM-dwM2)/10;
		DWORD dwH=((dwTimer-((dwM*60*1000)+(dwS*1000)+dwMs))/(1000*60*60))%60;;
		DWORD dwH2=dwH%10;;
		DWORD dwH1=(dwH-dwH2)/10;;
		// display time
		char s[256];
		s[0]=((char)dwH1)+'0';
		s[1]=((char)dwH2)+'0';
		s[2]=':';
		s[3]=((char)dwM1)+'0';
		s[4]=((char)dwM2)+'0';
		s[5]=':';
		s[6]=((char)dwS1)+'0';
		s[7]=((char)dwS2)+'0';
		s[8]='.';
		s[9]=((char)dwMs1)+'0';
		s[10]=((char)dwMs2)+'0';
		s[11]=((char)dwMs3)+'0';
		s[12]=0;
		Font->BufferText(s,320-(12*8),120);
		}
		{
		char s[256];
		sprintf(s,"(Space)  - Start");
		Font->BufferText(s,320-(17*8),280);
		sprintf(s,"(Shift)  - Reset");
		Font->BufferText(s,320-(17*8),300);
		sprintf(s,"(Escape) - Quit");
		Font->BufferText(s,320-(17*8),320);
		sprintf(s,"(Arrows) - Target");
		Font->BufferText(s,320-(17*8),340);
		// extract digits
		DWORD dwMs=dwTarget%1000;
		DWORD dwMs3=dwMs%10;
		DWORD dwMs2=((dwMs-dwMs3)/10)%10;
		DWORD dwMs1=((dwMs-((dwMs2*10)+dwMs3))/100)%10;
		DWORD dwS=((dwTarget-dwMs)/1000)%60;
		DWORD dwS2=dwS%10;
		DWORD dwS1=(dwS-dwS2)/10;
		DWORD dwM=((dwTarget-((dwS*1000)+dwMs))/(1000*60))%60;
		DWORD dwM2=dwM%10;
		DWORD dwM1=(dwM-dwM2)/10;
		DWORD dwH=((dwTarget-((dwM*60*1000)+(dwS*1000)+dwMs))/(1000*60*60))%60;;
		DWORD dwH2=dwH%10;;
		DWORD dwH1=(dwH-dwH2)/10;;
		// display time
		s[0]=((char)dwH1)+'0';
		s[1]=((char)dwH2)+'0';
		s[2]=':';
		s[3]=((char)dwM1)+'0';
		s[4]=((char)dwM2)+'0';
		s[5]=':';
		s[6]=((char)dwS1)+'0';
		s[7]=((char)dwS2)+'0';
		s[8]='.';
		s[9]=((char)dwMs1)+'0';
		s[10]=((char)dwMs2)+'0';
		s[11]=((char)dwMs3)+'0';
		s[12]=0;
		Font->BufferText(s,320-(12*8),400);
		}
		
		// check for exit key
		if(cdCurr.esc)
			State2++;
		break;
//// CLEANUP INTRO ///////////////////////////////////////
	case(ST2_SELECT_DONE):
		// unload data
		DELETE_FONT(Font);
		DELETE_SOUND(Ding);
		
		// bye-bye!
		bEndExec=TRUE;;
		break;
	}
	break;
}
// END PROGRAM LOOP //////////////////////////////////////

	// perform timing
	EXEC_TIMER;
	// perform frame count
	EXEC_FPS;
	// flip surfaces
	PerformPageFlip();
	// exit if necessary
	if(bEndExec)
		return(FALSE);
	// continue execution
	return(TRUE);
}

// USER FUNCTIONS ////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EncodeData()								//
//														//
//	Loads and encodes all data files to be used in this //
//		application.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void EncodeData(void)
{
// only encode data if necessary (see "primary.h")
#ifdef ENCODE_DATA
	if(MessageBox(GetGlobalHWnd(),
				  "Do you want to update data files?",
				  "Query",
				  MB_YESNO)==IDYES)
	{
		// record to verification file
		CreateVerificationFile("verify.rec");
		// intro files
		CreateDataFile("intro.dat");
			// basic font
			EncodeFont("stdvga.ftd");
			// ding
			EncodeSound("ding.wav");
		// close data file
		CloseDataFile();
		// close verification file
		CloseVerificationFile();
	}
#endif
}

// END ///////////////////////////////////////////////////

