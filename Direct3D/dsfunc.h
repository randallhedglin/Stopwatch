// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  dsfunc.h										//
//														//
//	External functions for DirectSound.	 NOTE:  Sound	//
//	effects in this application should be fixed at		//
//	8-bit, 11025 Hz.  This is for speed purposes.		//
//														//
//////////////////////////////////////////////////////////

// CLASSES ///////////////////////////////////////////////

class CDSSound
{
public:
	CDSSound();

	BOOL Load(void);
	void Pan(long lVal);
	void Frequency(long lInc);
	void Volume(long lVal);
	void Play(BOOL bLooping=FALSE);
	void Stop(void);
	void Release(void);

	// valid flag
	BOOL bIsValid;
	// original frequency
	long lOriginalFreq;
	// sound buffer
	LPDIRECTSOUNDBUFFER lpSndBuf;
	// optimization
	DWORD dwDummy[5];
};

// PROTOTYPES ////////////////////////////////////////////

extern BOOL InitializeDirectSound(void);

extern void ReleaseDirectSound(void);

extern void TestDirectSound(void);

extern inline BOOL GetGlobalSoundEnabledFlag(void);

extern inline void SetGlobalSoundEnabledFlag(BOOL bFlag);

extern inline void EnableSound(void);

extern inline void DisableSound(void);

extern inline LPDIRECTSOUND GetGlobalDirectSoundPtr(void);

extern inline BOOL GetGlobalDirectSoundInitializedFlag(void);

// include encode functions if necessary
#ifdef ENCODE_DATA
extern void EncodeSound(LPCSTR lpFilename);
#endif

// MACROS ////////////////////////////////////////////////

#define MULTI_TO_WIDE(m,w) MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,w,-1,m,_MAX_PATH);

#define NEW_SOUND(a) static CDSSound* a; a=NULL; SAFE_NEW(a,CDSSound);

#define DELETE_SOUND(a) if(a) { a->Release(); SAFE_DELETE(a); }

// END ///////////////////////////////////////////////////

