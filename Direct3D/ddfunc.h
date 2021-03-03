// OPTIMIZED

//////////////////////////////////////////////////////////
//														//
//  File:  ddfunc.h										//
//														//
//	External functions for DirectDraw.					//
//														//
//////////////////////////////////////////////////////////

// PROTOTYPES ////////////////////////////////////////////

extern BOOL SelectDDDevice(void);

extern GUID inline GetGlobalDDDeviceGUID(void);

extern BOOL inline GetGlobalDDDefaultFlag(void);

extern void inline SetGlobalDDDeviceGUID(GUID guid);

extern void inline SetGlobalDDDefaultFlag(BOOL bFlag);

extern void SelectResolution(void);

extern BOOL inline GetGlobalResolutionFlag(void);

extern void inline SetGlobalResolutionFlag(BOOL bFlag);

extern BOOL InitializeDirectDraw(void);

extern void ReleaseDirectDraw(void);

extern LPDIRECTDRAW4 inline GetGlobalDirectDrawPtr(void);

extern BOOL SelectD3DDevice(void);

extern GUID inline GetGlobalD3DDeviceGUID(void);

extern BOOL inline GetGlobalD3DHardwareFlag(void);

extern void inline SetGlobalD3DDeviceGUID(GUID guid);

extern void inline SetGlobalD3DHardwareFlag(BOOL bFlag);

extern LPDIRECT3D3 inline GetGlobalDirect3DPtr(void);

extern LPDIRECTDRAWSURFACE4 inline GetGlobalFrontBufferPtr(void);

extern LPDIRECTDRAWSURFACE4 inline GetGlobalBackBufferPtr(void);

extern BOOL inline GetGlobalUse15BitFlag(void);

extern LPDIRECTDRAWCLIPPER inline GetGlobalClipperPtr(void);

extern BOOL inline GetGlobalUseZBufferFlag(void);

extern void inline SetGlobalUseZBufferFlag(BOOL bFlag);

extern LPDIRECTDRAWSURFACE4 inline GetGlobalZBufferPtr(void);

extern LPDIRECT3DDEVICE3 inline GetGlobalDirect3DDevicePtr(void);

extern LPDIRECT3DVIEWPORT3 inline GetGlobalViewportPtr(void);

extern LPPALETTEENTRY inline GetGlobalPalettePtr(void);

extern BOOL LockFrontBuffer(LPWORD* lplpwData,
							LPDWORD lpdwPitch);

extern void UnlockFrontBuffer(void);

extern BOOL LockBackBuffer(LPWORD* lplpwData,
						   LPDWORD lpdwPitch);

extern void UnlockBackBuffer(void);

extern void ClearFrontBuffer(DWORD dwX1=0xF0000000L,
							 DWORD dwY1=0,
							 DWORD dwX2=0,
							 DWORD dwY2=0);

extern void ClearBackBuffer(DWORD dwX1=0xF0000000L,
							DWORD dwY1=0,
							DWORD dwX2=0,
							DWORD dwY2=0);

extern void PerformPageFlip(void);

extern inline WORD GetGlobalPaletteValue15(BYTE cVal);

extern inline WORD GetGlobalPaletteValue16(BYTE cVal);

extern inline void SetGlobalPaletteValue15(BYTE cVal,
										   WORD wVal);

extern inline void SetGlobalPaletteValue16(BYTE cVal,
										   WORD wVal);

extern LPD3DRECT inline GetGlobalScreenRectanglePtr(void);

extern void FadeScreen(D3DCOLOR dcColor);

extern inline void GetGlobalAlphaPixelFormat(LPDDPIXELFORMAT lpDDPF);

extern void SelectViewportSize(void);

extern BOOL inline GetGlobalViewportSizeFlag(void);

extern void inline SetGlobalViewportSizeFlag(BOOL bFlag);

extern BOOL inline GetGlobalUseTripleBufferFlag(void);

extern void inline SetGlobalUseTripleBufferFlag(BOOL bFlag);

// MACROS ////////////////////////////////////////////////

#define SET_PALETTE_ENTRY(n,r,g,b) { GetGlobalPalettePtr()[n].peRed=(BYTE)r; GetGlobalPalettePtr()[n].peGreen=(BYTE)g; GetGlobalPalettePtr()[n].peBlue=(BYTE)b; GetGlobalPalettePtr()[n].peFlags=PC_NOCOLLAPSE; SetGlobalPaletteValue15(n,RGB_15(r,g,b)); SetGlobalPaletteValue16(n,RGB_16(r,g,b)); }

#define GET_PALETTE_ENTRY(n,r,g,b) { r=GetGlobalPalettePtr()[n].peRed; g=GetGlobalPalettePtr()[n].peGreen; b=GetGlobalPalettePtr()[n].peBlue; }

#define RGB_15(r,g,b) (((WORD)((b>>3)&31))+(((WORD)((g>>3)&31))<<5)+(((WORD)((r>>3)&31))<<10))

#define GETR_15(c) ((BYTE)((c>>10)&31)<<3)

#define GETG_15(c) ((BYTE)((c>>5)&31)<<3)

#define GETB_15(c) ((BYTE)(c&31)<<3)

#define RGB_16(r,g,b) (((WORD)((b>>3)&31))+(((WORD)((g>>2)&63))<<5)+(((WORD)((r>>3)&31))<<11))

#define GETR_16(c) ((BYTE)((c>>11)&31)<<3)

#define GETG_16(c) ((BYTE)((c>>5)&63)<<2)

#define GETB_16(c) ((BYTE)(c&31)<<3)

// END ///////////////////////////////////////////////////

