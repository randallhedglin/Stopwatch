// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  mvefunc.h									//												//
//														//
//	Movie handling function prototypes.  NOTE: Movies	//
//	are slow and should not be used when speed is		//
//	important.											//
//														//
//////////////////////////////////////////////////////////

// CLASSES ///////////////////////////////////////////////

class CDDMovie
{
public:
	CDDMovie();

	BOOL Load(DWORD dwTextureStage1=0);
	void Display(int nX,
				 int nY);
	void Buffer(int nX,
				int nY);
	void BufferFast(int nX,
					int nY);
	void Update(DWORD dwNumFrames=1);
	void Release(void);

	// bitmap surface
	CDDBitmap* Bitmap;
	// bitmap data
	LPBYTE lpData;
	// current bitmap
	DWORD dwIndex;
	// number of bitmaps in loop
	DWORD dwNumBmps;
	// pointer in data
	DWORD dwDataPtr;
	// palette
	LPPALETTEENTRY lpPalette;
	// optimization
	DWORD dwDummy[2];
};

// PROTOTYPES ////////////////////////////////////////////

// include encode functions if necessary
#ifdef ENCODE_DATA
extern void EncodeMovie(LPSTR lpFilename);
#endif

// MACROS ////////////////////////////////////////////////

#define NEW_MOVIE(a) static CDDMovie* a; a=NULL; SAFE_NEW(a,CDDMovie);

#define DELETE_MOVIE(a) if(a) { a->Release(); SAFE_DELETE(a); }

// END ///////////////////////////////////////////////////
