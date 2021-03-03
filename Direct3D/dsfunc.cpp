// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  dsfunc.cpp									//
//														//
//	DirectSound functions.								//
//														//
//////////////////////////////////////////////////////////

// main include file
#include"primary.h"
// header for this module
#include"dsfunc.h"
// allow access to main window functions
#include"winmain.h"
// allow access to file functions
#include"filefunc.h"

// ERROR MESSAGES ////////////////////////////////////////

// failed to init DirectSound
#define ERR_DSFUNC00 { DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DSFUNC.CPP\nError Code: 00\nUnable to initialize DirectSound.") }
// failed to set cooperative level
#define ERR_DSFUNC01 { DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DSFUNC.CPP\nError Code: 01\nUnable to set DirectSound cooperative level.") }
// invalid data file
#define ERR_DSFUNC02 { DISPLAY_FATAL_ERROR(GetGlobalHWnd(),"Error in module: DSFUNC.CPP\nError Code: 02\nInvalid data in data file.") }
// failed to create sound buffer
#define ERR_DSFUNC03 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DSFUNC.CPP\nError Code: 03\nUnable to create sound effect buffer.\nSound effects may not play correctly.") }
// failed to lock sound buffer
#define ERR_DSFUNC04 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DSFUNC.CPP\nError Code: 04\nUnable to lock sound effect buffer.\nSound effects may not play correctly.") }
// failed to unlock sound buffer
#define ERR_DSFUNC05 { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DSFUNC.CPP\nError Code: 04\nUnable to unlock sound effect buffer.\nSound effects may not play correctly.") }

// MESSAGES //////////////////////////////////////////////

// no DirectSound
#define MSG_DSFUNC00 { DISPLAY_INFO(GetGlobalHWnd(),"No valid DirectSound devices were found.\nSound effects will be disabled.") }

// GLOBALS ///////////////////////////////////////////////

// DirectSound interface
LPDIRECTSOUND g_lpDS=NULL;
// DirectSound initialized flag
BOOL g_bDSInit=FALSE;
// DirectSound enabled flag
BOOL g_bDSEnabled=FALSE;

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  InitializeDirectSound()					//											//
//		(defined in "dsfunc.h")							//
//														//
//	Start DirectSound.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

extern BOOL InitializeDirectSound(void)
{
	// create DirectSound
	if(FAILED(DirectSoundCreate(NULL,
								&g_lpDS,
								NULL)))
	{
		if(GetGlobalSoundEnabledFlag())
			ERR_DSFUNC00;
		return(FALSE);
	}
	// set cooperative level
	if(FAILED(g_lpDS->SetCooperativeLevel(GetGlobalHWnd(),
										  DSSCL_NORMAL)))
	{
		if(GetGlobalSoundEnabledFlag())
			ERR_DSFUNC01;
		return(FALSE);
	}
	// set initialized flag
	g_bDSInit=TRUE;
	// ok
	return(TRUE);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  ReleaseDirectSound()						//
//		(defined in "dsfunc.h")							//
//														//
//	Close DirectSound.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void ReleaseDirectSound(void)
{
	// release DirectSound
	SAFE_RELEASE(g_lpDS);
	// reset initialized flag
	g_bDSInit=FALSE;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  TestDirectSound()						//
//		(defined in "dsfunc.h")							//
//														//
//	Checks DirectSound and sets enabled flag as req'd.	//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void TestDirectSound(void)
{
	// temporary DirectSound pointer
	LPDIRECTSOUND lpDSTemp=NULL;
	
	// create DirectSound
	if(FAILED(DirectSoundCreate(NULL,
								&lpDSTemp,
								NULL)))
	{
		// no DirectSound present
		MSG_DSFUNC00;
		return;
	}
	// query user
	if(MessageBox(GetGlobalHWnd(),
				  "Do you want to enable sound effects?",
				  "Query",
				  MB_YESNO|
				  MB_ICONQUESTION)==IDYES)
		EnableSound();
	else
		DisableSound();
	// release temporary pointer
	SAFE_RELEASE(lpDSTemp);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalSoundEnabledFlag()				//
//		(defined in "dsfunc.h")							//
//														//
//	Retrieves sound enabled flag.						//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: value of flag								//
//														//
//////////////////////////////////////////////////////////

extern inline BOOL GetGlobalSoundEnabledFlag(void)
{
	return(g_bDSEnabled);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  SetGlobalSoundEnabledFlag()				//
//		(defined in "dsfunc.h")							//
//														//
//	Sets sound enabled flag.							//
//	  Parameters:										//
//		BOOL: value of flag								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern inline void SetGlobalSoundEnabledFlag(BOOL bFlag)
{
	g_bDSEnabled=bFlag;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  EnableSound()							//
//		(defined in "dsfunc.h")							//
//														//
//	Enable sound effects if possible.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern inline void EnableSound(void)
{
	g_bDSEnabled=TRUE;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  DisableSound()							//
//		(defined in "dsfunc.h")							//
//														//
//	Disable sound effects.								//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern inline void DisableSound(void)
{
	g_bDSEnabled=FALSE;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalDirectSoundPtr()				//
//		(defined in "dsfunc.h")							//
//														//
//	Retrieves pointer to DirectSound interface.			//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		LPDIRECTSOUND: pointer							//
//														//
//////////////////////////////////////////////////////////

extern inline LPDIRECTSOUND GetGlobalDirectSoundPtr(void)
{
	return(g_lpDS);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  GetGlobalDirectSoundInitializedFlag()	//
//		(defined in "dsfunc.h")							//
//														//
//	Retrieves flag for DirectSound init success.		//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if sound is running					//
//														//
//////////////////////////////////////////////////////////

extern inline BOOL GetGlobalDirectSoundInitializedFlag(void)
{
	return(g_bDSInit);
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDSSound::CDSSound()						//
//		(defined in "dsfunc.h")							//
//														//
//	Class constructor for sound effect.					//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

CDSSound::CDSSound()
{
	// init variables
	LPDIRECTSOUNDBUFFER lpSndBuf=NULL;
	BOOL bIsValid=FALSE;
	lOriginalFreq=0;
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDSSound::Load()							//
//		(defined in "dsfunc.h")							//
//														//
//	Load sound effect from open data file.				//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		BOOL: TRUE if successful						//
//														//
//////////////////////////////////////////////////////////

BOOL CDSSound::Load(void)
{
	// data type
	BYTE cType=NULL;
	// data size
	DWORD dwSize=0;
	// data
	LPBYTE lpData=NULL;
	// wave format
	WAVEFORMATEX wfx;
	INIT_VAR(wfx);
	// buffer description
	DSBUFFERDESC dsbd;
	// buffer sector 1 pointer
	LPBYTE lpAudioPtr1=NULL;
	// buffer sector 2 pointer
	LPBYTE lpAudioPtr2=NULL;
	// buffer sector 1 length
	DWORD dwAudioLength1=0;
	// buffer sector 2 length
	DWORD dwAudioLength2=0;

	// read data type
	ReadData(&cType,sizeof(BYTE));
	// check data type
	if(cType!=DTF_IDSOUND)
	{
		ERR_DSFUNC02;
		return(FALSE);
	}
	// read size
	ReadData((LPBYTE)&dwSize,
			 sizeof(DWORD));
	// allocate data buffer
	SAFE_ALLOC(lpData,
			   LPBYTE,
			   dwSize);
	// check for error
	if(!lpData)
		return(FALSE);
	// read data
	ReadData(lpData,
			 dwSize);
	// load into DirectSound if necessary
	if(g_bDSInit)
	{
		// set format
		wfx.wFormatTag=WAVE_FORMAT_PCM;
		// set channels
		wfx.nChannels=1;
		// set rate (fixed at 11025 Hz)
		wfx.nSamplesPerSec=11025;
		// set alignment
		wfx.nBlockAlign=1;
		// set average bps
		wfx.nAvgBytesPerSec=wfx.nSamplesPerSec*wfx.nBlockAlign;
		// set bits (fixed at 8-bit)
		wfx.wBitsPerSample=8;
		// set size
		wfx.cbSize=0;
		// set up buffer description
		INIT_STRUCT(dsbd);
		// set flags
		dsbd.dwFlags=DSBCAPS_CTRLDEFAULT|
					 DSBCAPS_STATIC|
					 DSBCAPS_LOCHARDWARE;
		// set size
		dsbd.dwBufferBytes=dwSize;
		// set format pointer
		dsbd.lpwfxFormat=&wfx;
		// set reserved flag
		dsbd.dwReserved=NULL;
		// create sound buffer
		if(FAILED(g_lpDS->CreateSoundBuffer(&dsbd,
											&lpSndBuf,
											NULL)))
		{
			// attempt to create in software memory
			INIT_VAR(wfx);
			// set format
			wfx.wFormatTag=WAVE_FORMAT_PCM;
			// set channels
			wfx.nChannels=1;
			// set rate (fixed at 11025 Hz)
			wfx.nSamplesPerSec=11025;
			// set alignment
			wfx.nBlockAlign=1;
			// set average bps
			wfx.nAvgBytesPerSec=wfx.nSamplesPerSec*wfx.nBlockAlign;
			// set bits (fixed at 8-bit)
			wfx.wBitsPerSample=8;
			// set size
			wfx.cbSize=0;
			// set up buffer description
			INIT_STRUCT(dsbd);
			// set flags
			dsbd.dwFlags=DSBCAPS_CTRLDEFAULT|
						 DSBCAPS_STATIC|
						 DSBCAPS_LOCSOFTWARE;
			// set size
			dsbd.dwBufferBytes=dwSize;
			// set format pointer
			dsbd.lpwfxFormat=&wfx;
			// set reserved flag
			dsbd.dwReserved=NULL;
			// create sound buffer
			if(FAILED(g_lpDS->CreateSoundBuffer(&dsbd,
												&lpSndBuf,
												NULL)))
			{
				ERR_DSFUNC03;
				// free memory
				SAFE_FREE(lpData);
				return(FALSE);
			}
		}
		// lock buffer
		if(FAILED(lpSndBuf->Lock(0,
								 dwSize,
								 (LPVOID*)&lpAudioPtr1,
								 &dwAudioLength1,
								 (LPVOID*)&lpAudioPtr2,
								 &dwAudioLength2,
								 DSBLOCK_FROMWRITECURSOR)))
		{
			ERR_DSFUNC04;
			// free memory
			SAFE_FREE(lpData);
			return(FALSE);
		}
		// copy sector 1
		CopyMemory(lpAudioPtr1,
				   lpData,
				   dwAudioLength1);
		// copy sector 2
		CopyMemory(lpAudioPtr2,
				   lpData+dwAudioLength1,
				   dwAudioLength2);
		// unlock buffer
		if(FAILED(lpSndBuf->Unlock(lpAudioPtr1,
								   dwAudioLength1,
								   lpAudioPtr2,
								   dwAudioLength2)))
		{
			ERR_DSFUNC05;
			// free memory
			SAFE_FREE(lpData);
			return(FALSE);
		}
		// set valid flag
		bIsValid=TRUE;
	}
	// free memory
	SAFE_FREE(lpData);
	// save frequency value
	lOriginalFreq=11025;
	//ok
	return(TRUE);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDSSound::Pan()							//
//		(defined in "dsfunc.h")							//
//														//
//	Set pan for this effect.							//
//	  Parameters:										//
//		long: pan value (-10000 to 10000)				//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDSSound::Pan(long lVal)
{
	// set pan
	if(g_bDSInit&&g_bDSEnabled&&bIsValid)
		lpSndBuf->SetPan(lVal);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDSSound::Frequency()					//
//		(defined in "dsfunc.h")							//
//														//
//	Change frequency for this sound.					//
//	  Parameters:										//
//		long: amount to add to base frequency			//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDSSound::Frequency(long lVal)
{
	// set frequency
	if(g_bDSInit&&g_bDSEnabled&&bIsValid)
		lpSndBuf->SetFrequency(lVal+lOriginalFreq);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDSSound::Volume()						//
//		(defined in "dsfunc.h")							//
//														//
//	Set volume of this effect.							//
//	  Parameters:										//
//		long: volume (0-10000)							//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDSSound::Volume(long lVal)
{
	// set volume
	if(g_bDSInit&&g_bDSEnabled&&bIsValid)
		lpSndBuf->SetVolume(lVal);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDSSound::Play()							//
//		(defined in "dsfunc.h")							//
//														//
//	Play this sound.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDSSound::Play(BOOL bLooping)
{
	if(g_bDSInit&&g_bDSEnabled&&bIsValid)
		if(bLooping)
			// repeat
			lpSndBuf->Play(NULL,
						   NULL,
						   DSBPLAY_LOOPING);
		else
			// play once
			lpSndBuf->Play(NULL,
						   NULL,
						   NULL);
}

//////////////////////////////////////
//									//
//	!! TIME INTENSIVE FUNCTION  !!  //
//									//
//////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  CDSSound::Stop()							//
//		(defined in "dsfunc.h")							//
//														//
//	Stop this sound.									//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDSSound::Stop(void)
{
	if(g_bDSInit&&g_bDSEnabled&&bIsValid)
		lpSndBuf->Stop();
}

//////////////////////////////////////////////////////////
//														//
//	Function:  CDSSound::Release()						//
//		(defined in "dsfunc.h")							//
//														//
//	Unload sound.										//
//	  Parameters:										//
//		None.											//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

void CDSSound::Release(void)
{
	// release buffer
	SAFE_RELEASE(lpSndBuf);
	// reset variables
	LPDIRECTSOUNDBUFFER lpSndBuf=NULL;
	BOOL bIsValid=FALSE;
}

// include encode functions if necessary
#ifdef ENCODE_DATA

// INCLUDES //////////////////////////////////////////////

// header for wave file loading
#include"my_mmsys.h"

// ERROR MESSAGES ////////////////////////////////////////

// failed to open wave file
#define ERR_DSFUNC00e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DSFUNC.CPP\nError Code: 00e\nUnable to open wave file for input.") }
// failed to descend wave file
#define ERR_DSFUNC01e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DSFUNC.CPP\nError Code: 01e\nUnable to descend wave file.") }
// failed to read wave file
#define ERR_DSFUNC02e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DSFUNC.CPP\nError Code: 02e\nUnable to read wave file.") }
// invalid wave file
#define ERR_DSFUNC03e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DSFUNC.CPP\nError Code: 03e\nInvalid wave file.") }
// failed to ascend wave file
#define ERR_DSFUNC04e { DISPLAY_ERROR(GetGlobalHWnd(),"Error in module: DSFUNC.CPP\nError Code: 04e\nUnable to ascend wave file.") }

// FUNCTIONS /////////////////////////////////////////////

//////////////////////////////////////////////////////////
//														//
//	Function:  EncodeSound()							//
//		(defined in "dsfunc.h")							//
//														//
//	Write sound effect to data file.					//
//	  Parameters:										//
//		LPCSTR: file name								//
//	  Return Value:										//
//		None.											//
//														//
//////////////////////////////////////////////////////////

extern void EncodeSound(LPCSTR lpFilename)
{
	// parent chunk info
	MMCKINFO Parent;
	INIT_VAR(Parent);
	// child chunk info
	MMCKINFO Child;
	INIT_VAR(Child);
	// handle to wave file
	HMMIO hWav=NULL;
	// wave format
	WAVEFORMATEX wfx;
	INIT_VAR(wfx);
	// sound data
	LPBYTE lpSndBuffer=NULL;
	// data size
	DWORD dwSize=0;

	// output ID to verification file
	WriteVerificationID(DTF_IDSOUND);
	// output ID ot data file
	WriteDataID(DTF_IDSOUND);
	// open wave file
	hWav=mmioOpen((char*)lpFilename,
				  NULL,
				  MMIO_READ|
				  MMIO_ALLOCBUF);
	// check for error
	if(!hWav)
	{
		ERR_DSFUNC00e;
		return;
	}
	// set 4cc for parent chunk
	Parent.fccType=mmioFOURCC('W','A','V','E');
	// descend into chunk
	if(mmioDescend(hWav,
				   &Parent,
				   NULL,
				   MMIO_FINDRIFF))
	{
		ERR_DSFUNC01e;
		// close file
		mmioClose(hWav,0);
		return;
	}
	// set child 4cc
	Child.ckid=mmioFOURCC('f','m','t',' ');
	// find format
	if(mmioDescend(hWav,
				   &Child,
				   &Parent,
				   0))
	{
		ERR_DSFUNC01e;
		// close file
		mmioClose(hWav,0);
		return;
	}
	// read format
	if(mmioRead(hWav,
				(char*)&wfx,
				sizeof(wfx))!=sizeof(wfx))
	{
		ERR_DSFUNC02e;
		// close file
		mmioClose(hWav,0);
		return;
	}
	// verify wave file
	if(wfx.wFormatTag!=WAVE_FORMAT_PCM)
	{
		ERR_DSFUNC03e;
		// close file
		mmioClose(hWav,0);
		return;
	}
	// ascend child
	if(mmioAscend(hWav,
				  &Child,
				  0))
	{
		ERR_DSFUNC04e;
		// close file
		mmioClose(hWav,0);
		return;
	}
	// set child 4cc
	Child.ckid=mmioFOURCC('d','a','t','a');
	// find data
	if(mmioDescend(hWav,
				   &Child,
				   &Parent,
				   MMIO_FINDCHUNK))
	{
		ERR_DSFUNC01e;
		// close file
		mmioClose(hWav,0);
		return;
	}
	// allocate sound buffer
	SAFE_ALLOC(lpSndBuffer,
			   LPBYTE,
			   Child.cksize);
	// check for error
	if(!lpSndBuffer)
	{
		// close file
		mmioClose(hWav,0);
		return;
	}
	// read data
	if(mmioRead(hWav,
				(char*)lpSndBuffer,
				Child.cksize)==-1)
	{
		ERR_DSFUNC02e;
		// close file
		mmioClose(hWav,0);
		// free data
		SAFE_FREE(lpSndBuffer);
		return;
	}
	// close file
	mmioClose(hWav,0);
	// set size
	dwSize=Child.cksize;
	// output size
	WriteDataDWORD(dwSize);
	// output data
	WriteDataBytes(lpSndBuffer,
				   dwSize);
	// free data
	SAFE_FREE(lpSndBuffer);
	// ok
	return;
}

#endif

// END ///////////////////////////////////////////////////

