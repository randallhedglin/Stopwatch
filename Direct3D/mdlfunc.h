// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  mdlfunc.h									//
//														//
//	External functions for models.						//
//														//
//////////////////////////////////////////////////////////

// CLASSES ///////////////////////////////////////////////

class CD3DModel
{
public:
	CD3DModel();

	BOOL Load(D3DVECTOR* lpvOffset=NULL);
	void Position(D3DVECTOR* lpvPos);
	void Orientation(D3DVECTOR* lpvRight,
					 D3DVECTOR* lpvUp,
					 D3DVECTOR* lpvAhead=NULL);
	void Yaw(D3DVALUE vInc);
	void Pitch(D3DVALUE vInc);
	void Roll(D3DVALUE vInc);
	void Right(D3DVALUE vInc);
	void Up(D3DVALUE vInc);
	void Ahead(D3DVALUE vInc);
	void Render(D3DVALUE dvScale=1);
	void RenderWireframe(D3DVALUE dvScale=1);
	void ComputeNormals(D3DVERTEX* lpVtxData);
	void ComputeExtents(D3DVERTEX* lpVtxData);
	void Release(void);

	// billboard flag
	BOOL bBillboard;
	// position
	D3DVECTOR vPos;
	// orientation
	D3DVECTOR vRight,
			  vUp,
			  vAhead;
	// pointer to last model number
	LPDWORD lpdwLastModel;
	// this model's number
	DWORD dwModelNumber;
	// pre-render instructions
	LPWORD lpPRIns;
	// material list
	CD3DMaterial** lplpMat;
	// number of materials
	DWORD dwNumMat;
	// texture list
	CD3DTexture** lplpTex;
	// number of textures
	DWORD dwNumTex;
	// instructions
	LPWORD lpIns;
	// pointer to Direct3D device
	LPDIRECT3DDEVICE3 lpD3DDevice;
	// vertex buffer
	LPDIRECT3DVERTEXBUFFER lpVtx;
	// storage for billboard vectors
	LPD3DVECTOR lpvBBRight;
	LPD3DVECTOR lpvBBUp;
	LPD3DVECTOR lpvBBAhead;
	// number of vertices
	DWORD dwNumVtx;
	// number of instructions
	DWORD dwNumIns;
	// number of pre-render instructions
	DWORD dwNumPRIns;
	// hit detection radius
	D3DVALUE dvHDRad;
	// hit detection radius squared
	D3DVALUE dvHDRadSq;
	// hit detection upper level
	D3DVALUE dvHDLevelU;
	// hit detection lower level
	D3DVALUE dvHDLevelL;
	// optimization
	DWORD dwDummy[7];
};

// PROTOTYPES ////////////////////////////////////////////

// include encode functions if necessary
#ifdef ENCODE_DATA
extern void EncodeModel(LPCSTR lpFilename);
#endif

// DEFINES ///////////////////////////////////////////////

// operation codes
#define OPCODE_ENDPRERENDER 0xFFE5
#define OPCODE_POINTLIST 0xFFE6
#define OPCODE_LINELIST 0xFFE7
#define OPCODE_LINESTRIP 0xFFE8
#define OPCODE_TRIANGLELIST 0xFFE9
#define OPCODE_TRIANGLESTRIP 0xFFEA
#define OPCODE_TRIANGLEFAN 0xFFEB
#define OPCODE_SETMATERIAL 0xFFEC
#define OPCODE_SETTEXTURE 0xFFED
#define OPCODE_ENDRENDER 0xFFEE
#define OPCODE_OPTIMIZE 0xFFEF

// shrink factor for hit detection
#define HD_SHRINK_FACTOR 0.8F

// MACROS ////////////////////////////////////////////////

#define FLOAT_TO_FIXED(a) ((long)(a*256))

#define FIXED_TO_FLOAT(a) (((float)a)*0.00390625F)

#define NEW_MODEL(a) static CD3DModel* a; a=NULL; SAFE_NEW(a,CD3DModel);

#define DELETE_MODEL(a) if(a) { a->Release(); SAFE_DELETE(a); }

// END ///////////////////////////////////////////////////
