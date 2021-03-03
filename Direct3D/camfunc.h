// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  camfunc.h									//
//														//
//	Camera functions.									//
//														//
//////////////////////////////////////////////////////////

// STRUCTURES ////////////////////////////////////////////

typedef struct
{
	// number of lag positions
	DWORD dwNumPos;
	// initial position
	D3DVECTOR vPos;
	// initial orientation
	D3DVECTOR vRight;
	D3DVECTOR vUp;
	D3DVECTOR vAhead;
	// offset vector
	D3DVECTOR vOffset;
	// optimization
	DWORD dwDummy[2];
} CAMERA_INIT,*CAMERA_INIT_PTR;

// CLASSES ///////////////////////////////////////////////

class CD3DCamera
{
public:
	CD3DCamera();

	BOOL Initialize(CAMERA_INIT_PTR lpCI);
	void Update(LPD3DVECTOR lpvTarget,
				LPD3DVECTOR lpvRight,
				LPD3DVECTOR lpvUp,
				LPD3DVECTOR lpvAhead);
	LPD3DVECTOR Position(void);
	void Release(void);

	// camera positions
	LPD3DVECTOR lpvCamera;
	// current write position
	DWORD dwWritePos;
	// target offset
	D3DVECTOR vOffset;
	// current read position
	DWORD dwReadPos;
	// number of positions
	DWORD dwNumPos;
	// optimization
	DWORD dwDummy[1];
};

// MACROS ////////////////////////////////////////////////

#define NEW_CAMERA(a) static CD3DCamera* a; a=NULL; SAFE_NEW(a,CD3DCamera);

#define DELETE_CAMERA(a) if(a) { a->Release(); SAFE_DELETE(a); }

// END ///////////////////////////////////////////////////