// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  scnfunc.h									//
//														//
//	External functions for scenery spheres.				//
//														//
//////////////////////////////////////////////////////////

// CLASSES ///////////////////////////////////////////////

class CD3DScenery
{
public:
	CD3DScenery();

	BOOL Load(D3DVALUE dvRad);
	void Render(D3DVECTOR* lpvPos);
	void Release(void);

	// sphere is actually a model
	CD3DModel* lpMdl;
	// optimization
	DWORD dwDummy[7];
};

// PROTOTYPES ////////////////////////////////////////////

// include encode functions if necessary
#ifdef ENCODE_DATA
extern void EncodeScenery(LPCSTR lpFilename1=NULL,
						  LPCSTR lpFilename2=NULL,
						  LPCSTR lpFilename3=NULL,
						  LPCSTR lpFilename4=NULL,
						  LPCSTR lpFilename5=NULL,
						  LPCSTR lpFilename6=NULL);
#endif

// MACROS ////////////////////////////////////////////////

#define NEW_SCENERY(a) static CD3DScenery* a; a=NULL; SAFE_NEW(a,CD3DScenery);

#define DELETE_SCENERY(a) if(a) { a->Release(); SAFE_DELETE(a); }

// END ///////////////////////////////////////////////////

