// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  dmfunc.cpp									//
//														//
//	Functions for handling DirectMusic.					//
//														//
//////////////////////////////////////////////////////////

// INCUDES ///////////////////////////////////////////////

// main include file
#include"primary.h"
// header for this module
#include"dmfunc.h"
// allow access to main window functions
#include"winmain.h"
// allow access to file functions
#include"filefunc.h"
// allow access to sound
#include"dsfunc.h"

// GLOBALS ///////////////////////////////////////////////

// song init delay counter
DWORD g_dwInitCount=0;
// music initialized flag
BOOL g_bDMInit=FALSE;
// music enabled flag
BOOL g_bDMEnabled=FALSE;
// pointer to performance interface
IDirectMusicPerformance* g_lpDM=NULL;
// pointer to loader
IDirectMusicLoader* g_lpDMLoader=NULL;
// MIDI file counter
DWORD g_dwMIDICount=0;

// DEFINES ///////////////////////////////////////////////

// name for temporary MIDI files
#define MIDI_TEMP_FILE "~mtf001_.tmp"

// ERROR MESSAGES ////////////////////////////////////////

// failed to init COM
#define ERR_DMFUNC00 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 00\nUnable to initialize COM, required for DirectMusic.\nMusic will be disabled.") }
// failed to create performance
#define ERR_DMFUNC01 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 01\nUnable to create DirectMusic performace object.\nMusic will be disabled.") }
// failed to init performance
#define ERR_DMFUNC02 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 02\nUnable to initialize DirectMusic performace object.\nMusic will be disabled.") }
// failed to add port
#define ERR_DMFUNC03 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 03\nUnable to create DirectMusic port object.\nMusic will be disabled.") }
// failed to create loader
#define ERR_DMFUNC04 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 04\nUnable to create DirectMusic loader object.\nMusic will be disabled.") }
// invalid data file
#define ERR_DMFUNC05 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 05\nInvalid data in data file.") }
// failed to open temp file
#define ERR_DMFUNC06 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 06\nUnable to open temporary MIDI file.") }
// failed to write temp file
#define ERR_DMFUNC07 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 07\nUnable to write to temporary MIDI file.") }
// failed to load MIDI
#define ERR_DMFUNC08 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 08\nUnable to load MIDI file.") }
// failed to set playback parms
#define ERR_DMFUNC09 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 09\nUnable to set MIDI file playback parameters.") }
// failed to set instrument parms
#define ERR_DMFUNC10 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 10\nUnable to set MIDI file instrument parameters.") }
// failed to set path
#define ERR_DMFUNC11 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 11\nUnable to set MIDI file path.") }
// failed to set length
#define ERR_DMFUNC12 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 12\nUnable to set MIDI file length\nMusic may not play correctly.") }

// MESSAGES //////////////////////////////////////////////

// no DirectMusic
#define MSG_DMFUNC00 { DISPLAY_INFO(GetGlobalHWnd(),"DirectMusic cannot be initialized.\nMusic will be disabled.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  InitializeDirectMusic()					//
//		(defined in "dmfunc.h")							//
//														//
//	Starts up DirectMusic if possible.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL InitializeDirectMusic(void)
{
	// only init if DirectSound is running
	if(GetGlobalDirectSoundInitializedFlag())
	{
		// initialize COM
		if(FAILED(CoInitialize(NULL)))
		{
			if(g_bDMEnabled)
				ERR_DMFUNC00;
			return(FALSE);
		}
		// create performance		
		if(FAILED(CoCreateInstance(CLSID_DirectMusicPerformance,
								   NULL,
								   CLSCTX_INPROC,
								   IID_IDirectMusicPerformance,
								   (LPVOID*)&g_lpDM)))
		{
			if(g_bDMEnabled)
				ERR_DMFUNC01;
			// un-init COM
			CoUninitialize();
			return(FALSE);
		}
		// init DirectMusic
		if(FAILED(g_lpDM->Init(NULL,
							   GetGlobalDirectSoundPtr(),
							   GetGlobalHWnd())))
		{
			if(g_bDMEnabled)
				ERR_DMFUNC02;
			// un-init COM
			CoUninitialize();
			return(FALSE);
		}
		// add port
		if(FAILED(g_lpDM->AddPort(NULL)))
		{
			if(g_bDMEnabled)
				ERR_DMFUNC03;
			// un-init COM
			CoUninitialize();
			return(FALSE);
		}
		// create loader
		if(FAILED(CoCreateInstance(CLSID_DirectMusicLoader,
								   NULL,
								   CLSCTX_INPROC,
								   IID_IDirectMusicLoader,
								   (LPVOID*)&g_lpDMLoader)))
		{
			if(g_bDMEnabled)
				ERR_DMFUNC04;
			// un-init COM
			CoUninitialize();
			return(FALSE);
		}
		// current directory
		char lpDir[_MAX_PATH];
		// wide current directory
		WCHAR lpwDir[_MAX_PATH];

		_getcwd(lpDir,_MAX_PATH);
		// convert to wide
		MULTI_TO_WIDE(lpwDir,lpDir);
		// set search directory
		if(FAILED(g_lpDMLoader->SetSearchDirectory(GUID_DirectMusicAllTypes,
												   lpwDir,
												   FALSE)))
		{
			ERR_DMFUNC11;
			return(FALSE);
		}
		// all is well
		g_bDMInit=TRUE;
	}
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  ReleaseDirectMusic()						//
//		(defined in "dmfunc.h")							//
//														//
//	Closes DirectMusic if necessary.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void ReleaseDirectMusic(void)
{
	// only close if already open
	if(g_bDMInit)
	{
		// release loader
		SAFE_RELEASE(g_lpDMLoader);
		// stop music
		if(g_lpDM)
		{
			g_lpDM->Stop(NULL,NULL,0,0);
			// close down
			g_lpDM->CloseDown();
		}
		// release DirectMusic
		SAFE_RELEASE(g_lpDM);
		// un-init DirectMusic
		g_bDMInit=FALSE;
		// un-init COM
		CoUninitialize();
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  TestDirectMusic()						//
//		(defined in "dmfunc.h")							//
//														//
//	Checks DirectMusic and prompts user to enable it.	//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void TestDirectMusic(void)
{
	// temporary DirectMusic pointer
	IDirectMusicPerformance* lpDMTemp=NULL;
	
	// initialize COM
	if(FAILED(CoInitialize(NULL)))
	{
		MSG_DMFUNC00;
		return;
	}
	// create performance		
	if(FAILED(CoCreateInstance(CLSID_DirectMusicPerformance,
							   NULL,
							   CLSCTX_INPROC,
							   IID_IDirectMusicPerformance,
							   (LPVOID*)&lpDMTemp)))
	{
		MSG_DMFUNC00;
		// un-init COM
		CoUninitialize();
		return;
	}
	// init DirectMusic
	if(FAILED(lpDMTemp->Init(NULL,
						     NULL,
						     GetGlobalHWnd())))
	{
		MSG_DMFUNC00;
		// release temporary pointer
		lpDMTemp->Stop(NULL,NULL,0,0);
		lpDMTemp->CloseDown();
		SAFE_RELEASE(lpDMTemp);
		// un-init COM
		CoUninitialize();
		return;
	}
	// add port
	if(FAILED(lpDMTemp->AddPort(NULL)))
	{
		MSG_DMFUNC00;
		// release temporary pointer
		lpDMTemp->Stop(NULL,NULL,0,0);
		lpDMTemp->CloseDown();
		SAFE_RELEASE(lpDMTemp);
		// un-init COM
		CoUninitialize();
		return;
	}
	// query user
	if(MessageBox(GetGlobalHWnd(),
				  "Do you want to enable music?",
				  "Query",
				  MB_YESNO|
				  MB_ICONQUESTION)==IDYES)
		EnableMusic();
	else
		DisableMusic();
	// stop music
	if(lpDMTemp)
	{
		lpDMTemp->Stop(NULL,NULL,0,0);
		// close down
		lpDMTemp->CloseDown();
	}
	// release temporary pointer
	SAFE_RELEASE(lpDMTemp);
	// un-init COM
	CoUninitialize();
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalMusicEnabledFlag()				//
//		(defined in "dmfunc.h")							//
//														//
//	Retrieves value of music enabled flag.				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: flag										//
//														//
//////////////////////////////////////////////////////////

extern BOOL inline GetGlobalMusicEnabledFlag(void)
{
	return(g_bDMEnabled);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalMusicEnabledFlag()				//
//		(defined in "dmfunc.h")							//
//														//
//	Sets music enabled flag.							//
//	  Parameters:										//
//		BOOL: value to set flag							//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void inline SetGlobalMusicEnabledFlag(BOOL bFlag)
{
	g_bDMEnabled=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  EnableMusic()							//
//		(defined in "dmfunc.h")							//
//														//
//	Enables music.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void inline EnableMusic(void)
{
	g_bDMEnabled=TRUE;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  DisableMusic()							//
//		(defined in "dmfunc.h")							//
//														//
//	Disables music.  (You are responsible to stop all	//
//		music currently playing.)						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void inline DisableMusic(void)
{
	g_bDMEnabled=FALSE;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalDirectMusicPtr()				//
//		(defined in "dmfunc.h")							//
//														//
//	Retrieves value of DirectMusic interface pointer.	//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		IDirectMusicPerformance*: pointer				..
//														//
//////////////////////////////////////////////////////////

extern inline IDirectMusicPerformance* GetGlobalDirectMusicPtr(void)
{
	return(g_lpDM);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDMSong::CDMSong()						//
//		(defined in "dmfunc.h")							//
//														//
//	Class constuctor for music files.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

CDMSong::CDMSong()
{
	// init variables
	lpSegment=NULL;
	lpState=NULL;
	bIsValid=FALSE;
	bIsPlaying=FALSE;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDMSong::Load()							//
//		(defined in "dmfunc.h")							//
//														//
//	Loads song file from previously open data file.		//
//	  Parameters:										//
//		MUSIC_TIME: length of song, in 1/4 notes		//
//	  Return Value:										//
//		BOOL: TRUE if successful.						//
//														//
//////////////////////////////////////////////////////////

BOOL CDMSong::Load(MUSIC_TIME mtBeats)
{
	// data type
	BYTE cType=NULL;
	// song size
	DWORD dwSize=0;
	// song data
	LPBYTE lpData=NULL;
	// handle to temporary file
	HANDLE hFile=NULL;
	// bytes written by WriteFile
	DWORD dwBytesWritten=0;
	// music object description
	DMUS_OBJECTDESC dmod;

	// read data type
	ReadData(&cType,sizeof(BYTE));
	// check data type
	if(cType!=DTF_IDMUSIC)
	{
		ERR_DMFUNC05;
		return(FALSE);
	}
	// read size
	ReadData((LPBYTE)&dwSize,sizeof(DWORD));
	// allocate memory
	SAFE_ALLOC(lpData,
			   LPBYTE,
			   dwSize);
	// check for error
	if(!lpData)
		return(FALSE);
	// read data
	ReadData(lpData,dwSize);
	// load if possible
	if(g_bDMInit)
	{
		// temporary filename
		char lpFileTemp[16];
		// wide filename
		WCHAR lpFileWide[16];

		// copy filename
		strcpy(lpFileTemp,MIDI_TEMP_FILE);
		// personalize filename
		if(g_dwMIDICount<10)
			lpFileTemp[7]=(char)('0'+g_dwMIDICount);
		else
			lpFileTemp[7]=(char)('a'+(g_dwMIDICount-10));
		// open file
		hFile=CreateFile(lpFileTemp,
						 GENERIC_WRITE,
						 NULL,
						 NULL,
						 CREATE_ALWAYS,
						 FILE_FLAG_WRITE_THROUGH,
						 NULL);
		// check for error
		if(hFile==INVALID_HANDLE_VALUE)
		{
			ERR_DMFUNC06;
			// free memory
			SAFE_FREE(lpData);
			return(FALSE);
		}
		// write data
		WriteFile(hFile,
				  (LPVOID)lpData,
				  dwSize,
				  &dwBytesWritten,
				  NULL);
		// check for error
		if(dwBytesWritten!=dwSize)
		{
			ERR_DMFUNC07;
			// close file
			CloseHandle(hFile);
			// free memory
			SAFE_FREE(lpData);
			return(FALSE);
		}
		// close file
		CloseHandle(hFile);
		// convert to wide string
		MULTI_TO_WIDE(lpFileWide,lpFileTemp);
		// set up object description
		INIT_STRUCT(dmod);
		// set class
		dmod.guidClass=CLSID_DirectMusicSegment;
		// set name
		wcscpy(dmod.wszFileName,lpFileWide);
		// set flags
		dmod.dwValidData=DMUS_OBJ_CLASS|
						 DMUS_OBJ_FILENAME;
		// load file
		if(FAILED(g_lpDMLoader->GetObject(&dmod,
										  IID_IDirectMusicSegment,
										  (LPVOID*)&lpSegment)))
		{
			ERR_DMFUNC08;
			// free memory
			SAFE_FREE(lpData);
			return(FALSE);
		}
		// set playback paramaters
		if(FAILED(lpSegment->SetParam(GUID_StandardMIDIFile,
									  -1,
									  0,
									  0,
									  (LPVOID)g_lpDM)))
		{
			ERR_DMFUNC09;
			// free memory
			SAFE_FREE(lpData);
			return(FALSE);
		}
		// set instrument parameters
		if(FAILED(lpSegment->SetParam(GUID_Download,
									  -1,
									  0,
									  0,
									  (LPVOID)g_lpDM)))
		{
			ERR_DMFUNC10;
			// free memory
			SAFE_FREE(lpData);
			return(FALSE);
		}
		// validate file
		bIsValid=TRUE;
		// delete temp file
		DeleteFile(lpFileTemp);
		// increment counter
		g_dwMIDICount++;
	}
	// free memory
	SAFE_FREE(lpData);
	// set length if necessary
	if(mtBeats)
		if(FAILED(lpSegment->SetLength(mtBeats*DMUS_PPQ)))
			ERR_DMFUNC12;
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
//	Function:  CDMSong::Play()							//
//		(defined in "dmfunc.h")							//
//														//
//	Play this song.										//
//	  Parameters:										//
//		DWORD: number of times to repeat				//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDMSong::Play(DWORD dwLoops)
{
	// reset counter to allow for initialization
	g_dwInitCount=0;
	// only play if necessary
	if(g_bDMInit&&g_bDMEnabled&&bIsValid)
	{
		// set repeats
		lpSegment->SetRepeats(dwLoops);
		// play song
		g_lpDM->PlaySegment(lpSegment,
						    DMUS_SEGF_QUEUE,
						    0,
						    &lpState);
		// set playing flag
		bIsPlaying=TRUE;
	}
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDMSong::Update()						//
//		(defined in "dmfunc.h")							//
//														//
//	Causes song to repeat continuously.					//
//	  Parameters:										//
//		BOOL: FALSE to prevent song from repeating		//
//	  Return Value:										//
//		BOOL: TRUE if song is still playing				//
//														//
//////////////////////////////////////////////////////////

BOOL CDMSong::Update(BOOL bReplay)
{
	// still playing flag
	BOOL bPlaying=FALSE;

	// check init counter
	if(g_dwInitCount<60)
	{
		// increment counter
		g_dwInitCount++;
		// still playing
		bPlaying=TRUE;
	}
	else
	{
		// is playing flag set?
		if(bIsPlaying)
			// is song over?
			if(g_lpDM->IsPlaying(lpSegment,NULL)!=S_OK)
			{
				// start it again, if necessary
				if(bReplay)
					Play();
			}
			else
				// still playing
				bPlaying=TRUE;
	}
	return(bPlaying);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDMSong::Stop()							//
//		(defined in "dmfunc.h")							//
//														//
//	Stop song.											//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDMSong::Stop(void)
{
	// is playing flag set?
	if(bIsPlaying)
	{
		// stop song
		g_lpDM->Stop(lpSegment,
				     NULL,
				     0,
				     0);
		// reset playing flag
		bIsPlaying=FALSE;
	}
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDMSong::Release()						//
//		(defined in "dmfunc.h")							//
//														//
//	Unloads file.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDMSong::Release(void)
{
	// are we valid?
	if(bIsValid)
	{
		// stop
		Stop();
		// unload
		lpSegment->SetParam(GUID_Unload,
							-1,
							0,
							0,
							(LPVOID)g_lpDM);
		// decrement counter
		g_dwMIDICount--;
	}
	// release segment
	SAFE_RELEASE(lpSegment);
	// reset variables
	lpSegment=NULL;
	lpState=NULL;
	bIsValid=FALSE;
	bIsPlaying=FALSE;
}

// include encode functions if necessary
#ifdef ENCODE_DATA

// ERROR MESSAGES ////////////////////////////////////////

// failed to open input file
#define ERR_DMFUNC00e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 00e\nUnable to open MIDI file for input.") }
// failed to read input file
#define ERR_DMFUNC01e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DMFUNC.CPP\nError Code: 01e\nUnable to read from MIDI file.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EncodeSong()								//
//		(defined in "dmfunc.h")							//
//														//
//	Outputs song to data file.							//
//	  Parameters:										//
//		LPCSTR: filename								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EncodeSong(LPCSTR lpFilename)
{
	// file handle
	HANDLE hFile=INVALID_HANDLE_VALUE;
	// file size
	DWORD dwSize=0;
	// music buffer
	LPBYTE lpData=NULL;
	// bytes read by ReadFile
	DWORD dwBytesRead=0;
	
	// output ID to verification file
	WriteVerificationID(DTF_IDMUSIC);
	// output ID ot data file
	WriteDataID(DTF_IDMUSIC);
	// open input file	
	hFile=CreateFile(lpFilename,
					 GENERIC_READ,
					 NULL,
					 NULL,
					 OPEN_EXISTING,
					 FILE_FLAG_SEQUENTIAL_SCAN,
					 NULL);
	// check for error
	if(hFile==INVALID_HANDLE_VALUE)
	{
		ERR_DMFUNC00e;
		return;
	}
	// get size
	dwSize=GetFileSize(hFile,NULL);
	// output size
	WriteDataDWORD(dwSize);
	// allocate buffer
	SAFE_ALLOC(lpData,
			   LPBYTE,
			   dwSize);
	// check for error
	if(!lpData)
		return;
	// read data
	ReadFile(hFile,
			 lpData,
			 dwSize,
			 &dwBytesRead,
			 NULL);
	// check for error
	if(dwBytesRead!=dwSize)
	{
		ERR_DMFUNC01e;
		// free memory
		SAFE_FREE(lpData);
		// close file
		CloseHandle(hFile);
		return;
	}
	// close file
	CloseHandle(hFile);
	// output data
	WriteDataBytes((LPBYTE)lpData,
				   dwSize);
	// free memory
	SAFE_FREE(lpData);
}

#endif

// END ///////////////////////////////////////////////////

