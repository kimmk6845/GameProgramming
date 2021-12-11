#pragma once

// Windows Header Files:
#include <windows.h>
// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
// DX Header Files:
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <dwmapi.h>
// Animation
#include "Animation.h"
// Sound
#include "SoundManager.h"


// 자원 안전 반환 매크로.
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

// 현재 모듈의 시작주소 얻기.
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}
#else
#define Assert(b)
#endif
#endif

// DemoApp class declaration
class DemoApp
{
public:
	DemoApp();
	~DemoApp();
	HRESULT Initialize(HINSTANCE hInstance);  // Register the window class and call methods for instantiating drawing resources
	void RunMessageLoop();  // Process and dispatch messages

private:
	HRESULT CreateDeviceIndependentResources();  // Initialize device-independent resources.
	HRESULT CreateDeviceResources();  // Initialize device-dependent resources.
	void DiscardDeviceResources();  // Release device-dependent resource.
	HRESULT OnRender();   // Draw content.

	HRESULT LoadResourceBitmap(
		ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR resourceName,
		PCWSTR resourceType,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	);

	void soundOn(DemoApp* pDemoApp, int i, bool trueorfalse);
	BOOL SetVolume(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lVolume);

	void OnResize(UINT width, UINT height);  // Resize the render target.
	BOOL addSound(HWND hWnd);

	void WriteActionInfo();			// Write Action Info (mouse cursor position, rectangle rotation & resize, etc...)

	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);  // The windows procedure.

	BOOL SetPan(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG IPan);


private:
	HWND m_hwnd;

	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;

	// Color Brush
	ID2D1SolidColorBrush* m_pSceneBrush;
	ID2D1SolidColorBrush* m_pTextBrush;
	ID2D1StrokeStyle* m_pStrokeStyleDash;
	ID2D1SolidColorBrush* m_pRedBrush;


	// DWrite
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	IDWriteTextFormat* m_score_TextFormat;

	// WIC and Bitmaps
	IWICImagingFactory* m_pWICFactory;
	ID2D1Bitmap* m_pBackgroundBitmap;
	ID2D1Bitmap* gunBitmap[3];

	ID2D1Bitmap* target_Bitmap;
	ID2D1Bitmap* trap_Bitmap;
	ID2D1Bitmap* deer_Bitmap;
	ID2D1Bitmap* rabbit_Bitmap;
	ID2D1Bitmap* shot_Bitmap;

	ID2D1Bitmap* assistant_Bitmap;
	ID2D1Bitmap* tell_Bitmap;
	ID2D1Bitmap* bird_Bitmap;
	ID2D1Bitmap* bee_Bitmap;

	ID2D1Bitmap* mite_Bitmap;


	// Bitmap brushes
	ID2D1BitmapBrush* m_pBackgroundBitmapBrush;
	ID2D1BitmapBrush* gunBrush[3];
	ID2D1RadialGradientBrush* gunOpacityBrush[3];

	ID2D1BitmapBrush* target_BitmapBrush;
	ID2D1BitmapBrush* trap_BitmapBrush;
	ID2D1BitmapBrush* deer_BitmapBrush;
	ID2D1BitmapBrush* rabbit_BitmapBrush;
	ID2D1BitmapBrush* shot_BitmapBrush;

	ID2D1BitmapBrush* assistant_BitmapBrush;
	ID2D1BitmapBrush* tell_BitmapBrush;
	ID2D1BitmapBrush* bird_BitmapBrush;
	ID2D1BitmapBrush* bee_BitmapBrush;

	ID2D1BitmapBrush* mite_BitmapBrush;


	// Geometry
	ID2D1PathGeometry* bee_PathGeometry;
	ID2D1PathGeometry* bird_PathGeometry;
	ID2D1PathGeometry* assistant_PathGeometry;
	ID2D1PathGeometry* sunGeometry;

	ID2D1RectangleGeometry* m_pRectGeo;

	// Animation
	LinearAnimation<float> m_Animation;		// 새
	LinearAnimation<float> m_Animation2;	// 벌

	DWM_TIMING_INFO bird_DwmTimingInfo;
	DWM_TIMING_INFO bee_DwmTimingInfo;

	// sound
	LPDIRECTSOUNDBUFFER g_lpDSBG[2] = { NULL };
	CSoundManager* soundManager;

};
