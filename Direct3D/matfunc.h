// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  matfunc.h									//
//														//
//	External functions for materials.					//
//														//
//////////////////////////////////////////////////////////

// CLASSES ///////////////////////////////////////////////

class CD3DMaterial
{
public:
	CD3DMaterial();

	BOOL Create(void);
	void Diffuse(D3DVALUE dvRed,
				 D3DVALUE dvGreen,
				 D3DVALUE dvBlue,
				 D3DVALUE dvAlpha);
	void Ambient(D3DVALUE dvRed,
				 D3DVALUE dvGreen,
				 D3DVALUE dvBlue,
				 D3DVALUE dvAlpha);
	void Specular(D3DVALUE dvRed,
				  D3DVALUE dvGreen,
				  D3DVALUE dvBlue,
				  D3DVALUE dvAlpha,
				  D3DVALUE dvPower);
	void Emissive(D3DVALUE dvRed,
				  D3DVALUE dvGreen,
				  D3DVALUE dvBlue,
				  D3DVALUE dvAlpha);
	void Update(void);
	void Current(void);
	void Release(void);

	// pointer to material interface
	LPDIRECT3DMATERIAL3 lpMat;
	// material structure
	D3DMATERIAL mat;
	// pointer to Direct3DDevice
	LPDIRECT3DDEVICE3 lpD3DDevice;
	// material handle
	D3DMATERIALHANDLE hMat;
	// pointer to last model number
	LPDWORD lpdwLastModel;
	// optimization
	//DWORD dwDummy[1];
};

// MACROS ////////////////////////////////////////////////

#define NEW_MATERIAL(a) static CD3DMaterial* a; a=NULL; SAFE_NEW(a,CD3DMaterial);

#define DELETE_MATERIAL(a) if(a) { a->Release(); SAFE_DELETE(a); }

// END ///////////////////////////////////////////////////

