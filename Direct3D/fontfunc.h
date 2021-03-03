// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  fontfunc.h									//
//														//
//	Prototypes for font handling functions.				//
//		(NOTE: Too slow for time-intensive routines!)	//
//														//
//////////////////////////////////////////////////////////

// CONSTANTS /////////////////////////////////////////////

// font identifiers
#define FTD_CAPA 0
#define FTD_LOWA 26
#define FTD_DIGT 52
#define FTD_EXCL 62
#define FTD_LPAR 63
#define FTD_RPAR 64
#define FTD_DASH 65
#define FTD_COLN 66
#define FTD_SEMI 67
#define FTD_QUOT 68
#define FTD_APOS 69
#define FTD_COMA 70
#define FTD_PERD 71
#define FTD_QUES 72

// CLASSES ///////////////////////////////////////////////

class CDDFont
{
public:
	CDDFont();

	BOOL Load(void);
	void DisplayChar(DWORD dwChar,
					 int nX,
					 int nY,
					 BOOL bTransparent=FALSE);
	void DisplayText(LPSTR lpText,
					 int nX,
					 int nY,
					 BOOL bTransparent=FALSE);
	void BufferChar(DWORD dwChar,
					int nX,
					int nY,
					BOOL bTransparent=FALSE);
	void BufferText(LPSTR lpText,
					int nX,
					int nY,
					BOOL bTransparent=FALSE);
	BOOL Lock(DWORD dwChar,
			  LPWORD* lplpwData,
			  LPDWORD lpdwPitch);
	void Unlock(DWORD dwChar);
	void Color(BYTE cRed,
			   BYTE cGreen,
			   BYTE cBlue);
	void Release(void);

	// width of font
	int nWidth;
	// height of font
	int nHeight;
	// font characters
	CDDBitmap* Chars[73];
	// optimization
	DWORD dwDummy[5];
};

// PROTOTYPES ////////////////////////////////////////////

// include encode functions if necessary
#ifdef ENCODE_DATA
extern void EncodeFont(LPSTR lpFilename);
#endif

// MACROS ////////////////////////////////////////////////

#define NEW_FONT(a) static CDDFont* a; a=NULL; SAFE_NEW(a,CDDFont);

#define DELETE_FONT(a) if(a) { a->Release(); SAFE_DELETE(a); }

// END ///////////////////////////////////////////////////

