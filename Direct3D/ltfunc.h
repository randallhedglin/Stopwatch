// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  ltfunc.h										//
//														//
//	External functions for lights.						//
//														//
//////////////////////////////////////////////////////////

// DEFINES ///////////////////////////////////////////////

// light types
#define LT_POINT 1
#define LT_PARALLELPOINT 2
#define LT_SPOT 3
#define LT_DIRECTIONAL 4

// CLASSES ///////////////////////////////////////////////

class CD3DLight
{
public:
	CD3DLight();

	BOOL CreatePoint(void);
	BOOL CreateParallelPoint(void);
	BOOL CreateSpot(void);
	BOOL CreateDirectional(void);
	void Color(D3DVALUE dvRed,
			   D3DVALUE dvGreen,
			   D3DVALUE dvBlue);
	void Position(D3DVECTOR* lpvPos);
	void Direction(D3DVECTOR* lpvDir);
	void Range(D3DVALUE dvRange);
	void Attenuation(D3DVALUE dvAtt0,
					 D3DVALUE dvAtt1,
					 D3DVALUE dvAtt2);
	void Falloff(D3DVALUE dvFalloff,
				 D3DVALUE dvTheta,
				 D3DVALUE dvPhi);
	void On(void);
	void Off(void);
	void SpecularOn(void);
	void SpecularOff(void);
	void Update(void);
	void Release(void);

	// light interface pointer
	LPDIRECT3DLIGHT lpLight;
	// light data
	D3DLIGHT2 light;
	// light type
	DWORD dwType;
	// optimization
	DWORD dwDummy[2];
};

// MACROS ////////////////////////////////////////////////

#define NEW_LIGHT(a) static CD3DLight* a; a=NULL; SAFE_NEW(a,CD3DLight);

#define DELETE_LIGHT(a) if(a) { a->Release(); SAFE_DELETE(a); }

// END ///////////////////////////////////////////////////

