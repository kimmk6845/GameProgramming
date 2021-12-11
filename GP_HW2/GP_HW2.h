#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER              // Allow use of features specific to Windows XP or later.
#define WINVER 0x0501       // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501 // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS      // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE           // Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600    // Change this to the appropriate value to target other versions of IE.
#endif

#ifndef UNICODE
#define UNICODE
#endif

#include "Resource.h"

// 필요한 헤더파일
#include <iostream>
#include <vector>		// ** vector 컨테이너 사용
#include <string>		// ** string
#include <random>		// ** rand 함수
#include <algorithm>	// ** sort

// 1.1 DX Header Files
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

// ** 학생의 정보 추가는 최대 8개
#define MAX_STUDENT 8  

// ** 1.2 안전 자원 반납 매크로
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }


// ** 1.3 클래스 기초 작업 구현
class DemoApp
{
public:
	DemoApp();
	~DemoApp();
	HRESULT Initialize(HINSTANCE hInstance);
	void RunMessageLoop();

private:
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	HRESULT CreateGridPatternBrush(ID2D1RenderTarget* pRenderTarget, ID2D1BitmapBrush** ppBitmapBrush);
	void DiscardDeviceResources();
	HRESULT OnRender();
	void OnResize(UINT width, UINT height);

	void drawInit();
	void drawTempInsertingRectangle();
	void drawTempdeletingRectangle();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	// ** 1.4 클래스 변수 선언
	HWND m_hwnd;
	ID2D1Factory* m_pD2DFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1StrokeStyle* m_pStrokeStyleDash;

	ID2D1SolidColorBrush* m_pOriginBorderBrush;
	ID2D1SolidColorBrush* m_pRectangleBorderBrush;
	ID2D1SolidColorBrush* m_pRectangleBrush[MAX_STUDENT];
	ID2D1SolidColorBrush* m_pOriginFillBrush;
	ID2D1SolidColorBrush* m_pTextBrush;
	ID2D1BitmapBrush* m_pGridPatternBitmapBrush;

	//DWrite
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
};