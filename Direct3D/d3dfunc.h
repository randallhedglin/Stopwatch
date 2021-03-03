// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  d3dfunc.h									//
//														//
//	External functions for Direct3D.					//
//														//
//////////////////////////////////////////////////////////

// PROTOTYPES ////////////////////////////////////////////

extern void SetD3DGlobals(void);

extern BOOL BeginScene(void);

extern void EndScene(void);

extern void SetFogStart(D3DVALUE dvStart);

extern void SetFogEnd(D3DVALUE dvEnd);

extern void SetFogColor(D3DCOLOR dcColor);

extern void EnableFog(void);

extern void DisableFog(void);

extern void ClearViewport(D3DCOLOR dcColor);

extern void ClearZBuffer(void);

extern void DisableZBuffer(void);

extern void EnableZBuffer(void);

extern void DisableZBufferWrites(void);

extern void EnableZBufferWrites(void);

extern void EnableBackFaceCull(void);

extern void EnableBackFaceCullReversed(void);

extern void DisableBackFaceCull(void);

extern void SetWorldMatrix(LPD3DVECTOR lpvPos,
						   LPD3DVECTOR lpvRight,
						   LPD3DVECTOR lpvUp,
						   LPD3DVECTOR lpvAhead,
						   D3DVALUE dvScale=1);

extern void SetViewMatrix(LPD3DVECTOR lpvCamera,
						  LPD3DVECTOR lpvTarget,
						  LPD3DVECTOR lpvUp);

extern void SetProjectionMatrix(D3DVALUE dvAngle,
								D3DVALUE dvNear,
								D3DVALUE dvFar);

extern BOOL GetGlobalUseFogFlag(void);

extern void SetGlobalUseFogFlag(BOOL bFlag);

extern BOOL GetGlobalUseAntiAliasFlag(void);

extern void SetGlobalUseAntiAliasFlag(BOOL bFlag);

extern BOOL GetGlobalUseGouraudFlag(void);

extern void SetGlobalUseGouraudFlag(BOOL bFlag);

extern BOOL GetGlobalUseSpecularFlag(void);

extern void SetGlobalUseSpecularFlag(BOOL bFlag);

extern BOOL GetGlobalUseAlphaFlag(void);

extern void SetGlobalUseAlphaFlag(BOOL bFlag);

extern BOOL GetGlobalUsePerspectiveCorrectFlag(void);

extern void SetGlobalUsePerspectiveCorrectFlag(BOOL bFlag);

extern void UpdateSettings(void);

extern void SetAmbientLight(D3DCOLOR dcColor);

extern void SetZBias(DWORD dwBias);

extern void SetTiledTextureMode(DWORD dwStage);

extern void SetMirroredTextureMode(DWORD dwStage);

extern void SetClampedTextureMode(DWORD dwStage);

extern void SetBorderedTextureMode(DWORD dwStage,
								   BYTE cRed,
								   BYTE cGreen,
								   BYTE cBlue);

extern BOOL GetGlobalD3DLinearFlag(void);

extern void SetGlobalD3DLinearFlag(BOOL bFlag);

extern void ClearTextureBuffers(void);

extern void MatrixMultiply(D3DMATRIX* lpmtxQ,
						   D3DMATRIX* lpmtxA,
						   D3DMATRIX* lpmtxB);

extern void MatrixInvert(D3DMATRIX* lpmtxQ,
						 D3DMATRIX* lpmtxA);

extern void TransformVector(D3DVECTOR* lpv,
							D3DMATRIX* lpm);

extern void CurrentMaterialOff(void);

extern void CurrentTextureOff(void);

extern void EnableAlpha(void);

extern void DisableAlpha(void);

extern LPD3DVECTOR GetGlobalBillboardRightVector(void);

extern LPD3DVECTOR GetGlobalBillboardUpVector(void);

extern LPD3DVECTOR GetGlobalBillboardAheadVector(void);

extern inline void SetGlobalBillboardVectors(LPD3DVECTOR lpvView);

extern DWORD GetGlobalModelNumber(void);

extern LPDWORD GetGlobalLastModelPtr(void);

extern void SetGlobalLastModel(DWORD dwVal);

// END ///////////////////////////////////////////////////

