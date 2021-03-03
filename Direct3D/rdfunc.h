// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  rdfunc.h										//
//														//
//	External functions for raw data.					//
//														//
//////////////////////////////////////////////////////////

// CLASSES ///////////////////////////////////////////////

class CRawData
{
public:
	CRawData();

	BOOL Load(void);
	LPVOID Data(void);
	void Release(void);

	// pointer to data
	LPVOID lpData;
	// optimization
	DWORD dwDummy[7];
};

// PROTOTYPES ////////////////////////////////////////////

// include encode functions if necessary
#ifdef ENCODE_DATA
void EncodeRawData(LPCSTR lpFilename);
#endif

// MACROS ////////////////////////////////////////////////

#define NEW_RAWDATA(a) static CRawData* a; a=NULL; SAFE_NEW(a,CRawData);

#define DELETE_RAWDATA(a) if(a) { a->Release(); SAFE_DELETE(a); }

// END ///////////////////////////////////////////////////
