// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  txfunc.h										//
//														//
//	External functions for textures.					//
//														//
//////////////////////////////////////////////////////////

// CLASSES ///////////////////////////////////////////////

class CD3DTexture
{
public:
	CD3DTexture();
	
	BOOL Load(void);
	void Current(void);
	void Release(void);

	// pointer to Direct3DDevice
	LPDIRECT3DDEVICE3 lpD3DDevice;
	// texture interface pointer
	LPDIRECT3DTEXTURE2 lpTex;
	// pointer to last model number
	LPDWORD lpdwLastModel;
	// texture bitmap
	CDDBitmap* lpBmp;
	// optimization
	DWORD dwDummy[4];
};

// include encode functions if necessary
#ifdef ENCODE_DATA
void EncodeTexture(LPCSTR lpFilename);
#endif

// MACROS ////////////////////////////////////////////////

#define NEW_TEXTURE(a) static CD3DTexture* a; a=NULL; SAFE_NEW(a,CD3DTexture);

#define DELETE_TEXTURE(a) if(a) { a->Release(); SAFE_DELETE(a); }

// END ///////////////////////////////////////////////////