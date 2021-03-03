// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  bmpfunc.h									//
//														//
//	Prototypes for bitmap handling functions.			//
//														//
//////////////////////////////////////////////////////////

// CLASSES ///////////////////////////////////////////////

class CDDBitmap
{
public:
	CDDBitmap();

	BOOL Load(DWORD dwTextureStage1=0);
	void Release(void);
	BOOL Lock(LPWORD* lplpwData,
		      LPDWORD lpdwPitch);
	void Unlock(void);
	void Display(int nX1,
				 int nY1,
				 int nX2=0xF0000000L,
				 int nY2=0);
	void Buffer(int nX1,
				int nY1,
				int nX2=0xF0000000L,
				int nY2=0);
	void BufferFast(int nX1,
					int nY1);
	BOOL IsValid(void);
	void Shrink(DWORD dwTextureStage1=0);

	// dimensions
	int nWidth;
	int nHeight;
	// pointer to back buffer
	LPDIRECTDRAWSURFACE4 lpBuffer;
	// pointer to surface interface
	LPDIRECTDRAWSURFACE4 lpBmpSurf;
	// valid bitmap flag
	BOOL bIsValid;
	// optimization
	DWORD dwDummy[3];
};

// PROTOTYPES ////////////////////////////////////////////

// include encode functions if necessary
#ifdef ENCODE_DATA
extern void EncodeBitmap(LPCSTR lpFilename);
#endif

// MACROS ////////////////////////////////////////////////

#define NEW_BITMAP(a) static CDDBitmap* a; a=NULL; SAFE_NEW(a,CDDBitmap);

#define DELETE_BITMAP(a) if(a) { a->Release(); SAFE_DELETE(a); }

// END ///////////////////////////////////////////////////

