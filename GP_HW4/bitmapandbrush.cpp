#include "hw3.h"

HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black),
				&m_pTextBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black, 1.f),
				&m_pSceneBrush);
		}

		/* 배경 및 물체의 비트맵생성 */
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"BACKGROUND",
				L"PNG",
				size.width,
				size.height,
				&m_pBackgroundBitmap
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"PISTOL",
				L"PNG",
				100,
				100,
				&gunBitmap[0]
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"RIFLE",
				L"PNG",
				100,
				100,
				&gunBitmap[1]
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"SHOTGUN",
				L"PNG",
				100,
				100,
				&gunBitmap[2]
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"Bee",
				L"PNG",
				100,
				100,
				&bee_Bitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"TARGET",
				L"PNG",
				100,
				100,
				&target_Bitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"TRAP",
				L"PNG",
				100,
				100,
				&trap_Bitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"DEER",
				L"PNG",
				100,
				100,
				&deer_Bitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"RABBIT",
				L"PNG",
				100,
				100,
				&rabbit_Bitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"SHOT",
				L"PNG",
				100,
				100,
				&shot_Bitmap
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"BIRD",
				L"PNG",
				100,
				100,
				&bird_Bitmap
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"ASSISTANT",
				L"PNG",
				200,
				300,
				&assistant_Bitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"TELL",
				L"PNG",
				100,
				100,
				&tell_Bitmap
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"MITE",
				L"PNG",
				20,
				20,
				&mite_Bitmap
			);
		}

		/* Brush 생성 */
		if (SUCCEEDED(hr))
		{
			D2D1_BITMAP_BRUSH_PROPERTIES propertiesXClampYClamp = D2D1::BitmapBrushProperties(
				D2D1_EXTEND_MODE_CLAMP,
				D2D1_EXTEND_MODE_CLAMP,
				D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
			);
			// 배경
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					m_pBackgroundBitmap,
					propertiesXClampYClamp,
					&m_pBackgroundBitmapBrush
				);
			}

			if (SUCCEEDED(hr))
			{
				// Create a Red brush.
				hr = m_pRenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Red),
					&m_pRedBrush);
			}

			// 권총, 라이플, 샷건
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					gunBitmap[0],
					&gunBrush[0]
				);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					gunBitmap[1],
					&gunBrush[1]
				);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					gunBitmap[2],
					&gunBrush[2]
				);
			}

			// 타겟
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					target_Bitmap, propertiesXClampYClamp,
					&target_BitmapBrush);
			}
			// 함정 타겟
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					trap_Bitmap, propertiesXClampYClamp,
					&trap_BitmapBrush);
			}
			// 사슴
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					deer_Bitmap, propertiesXClampYClamp,
					&deer_BitmapBrush);
			}
			// 토끼
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					rabbit_Bitmap, propertiesXClampYClamp,
					&rabbit_BitmapBrush);
			}
			// 발사 효과
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					shot_Bitmap, propertiesXClampYClamp,
					&shot_BitmapBrush);
			}

			// 부사수
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					assistant_Bitmap,
					propertiesXClampYClamp,
					&assistant_BitmapBrush
				);
			}
			// 부사수 말
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					tell_Bitmap,
					propertiesXClampYClamp,
					&tell_BitmapBrush
				);
			}
			
			// 새
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					bird_Bitmap,
					propertiesXClampYClamp,
					&bird_BitmapBrush
				);
			}
			// 벌
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					bee_Bitmap,
					propertiesXClampYClamp,
					&bee_BitmapBrush
				);
			}
			//진드기
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					mite_Bitmap,
					propertiesXClampYClamp,
					&mite_BitmapBrush
				);
			}

			// 방사형 계조 붓
			ID2D1GradientStopCollection* pGradientStops = NULL;
			static const D2D1_GRADIENT_STOP gradientStops[] =
			{
				{   0.f,  D2D1::ColorF(D2D1::ColorF::Black, 1.0f)  },
				{   1.f,  D2D1::ColorF(D2D1::ColorF::White, 0.0f)  },
			};

			hr = m_pRenderTarget->CreateGradientStopCollection(gradientStops, 2, &pGradientStops);
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateRadialGradientBrush(
					D2D1::RadialGradientBrushProperties(D2D1::Point2F(50, 50), D2D1::Point2F(25, 25), 100, 100),
					pGradientStops,
					&gunOpacityBrush[0]
				);
			}
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateRadialGradientBrush(
					D2D1::RadialGradientBrushProperties(D2D1::Point2F(50, 50), D2D1::Point2F(25, 25), 100, 100),
					pGradientStops,
					&gunOpacityBrush[1]
				);
			}
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateRadialGradientBrush(
					D2D1::RadialGradientBrushProperties(D2D1::Point2F(50, 50), D2D1::Point2F(25, 25), 100, 100),
					pGradientStops,
					&gunOpacityBrush[2]
				);
			}
			pGradientStops->Release();
		}

	}

	return hr;
}