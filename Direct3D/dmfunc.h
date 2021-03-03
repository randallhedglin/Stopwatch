// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  dmfunc.h										//
//														//
//	External functions for DirectMusic.					//
//														//
//////////////////////////////////////////////////////////

// CLASSES ///////////////////////////////////////////////

class CDMSong
{
public:
	CDMSong();

	BOOL Load(MUSIC_TIME mtBeats=0);
	void Play(DWORD dwLoops=0);
	BOOL Update(BOOL bReplay=TRUE);
	void Stop(void);
	void Release(void);

	// valid flag
	BOOL bIsValid;
	// playing flag
	BOOL bIsPlaying;
	// pointer to segment
	IDirectMusicSegment* lpSegment;
	// pointer to segment state
	IDirectMusicSegmentState* lpState;
	// optimization
	DWORD dwDummy[4];
};

// PROTOTYPES ////////////////////////////////////////////

extern BOOL InitializeDirectMusic(void);

extern void ReleaseDirectMusic(void);

extern void TestDirectMusic(void);

extern BOOL inline GetGlobalMusicEnabledFlag(void);

extern void inline SetGlobalMusicEnabledFlag(BOOL bFlag);

extern void inline EnableMusic(void);

extern void inline DisableMusic(void);

extern inline IDirectMusicPerformance* GetGlobalDirectMusicPtr(void);

// include encode functions if necessary
#ifdef ENCODE_DATA
extern void EncodeSong(LPCSTR lpFilename);
#endif

// MACROS ////////////////////////////////////////////////

#define NEW_SONG(a) static CDMSong* a; a=NULL; SAFE_NEW(a,CDMSong);

#define DELETE_SONG(a) if(a) { a->Release(); SAFE_DELETE(a); }

// END ///////////////////////////////////////////////////
