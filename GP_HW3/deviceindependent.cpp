#include "hw3.h"

using namespace D2D1;
HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	ID2D1GeometrySink* pSink = NULL;

	// Create a WIC factory.
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	if (SUCCEEDED(hr))
	{
		// Create a shared DirectWrite factory
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
		);
	}

	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Verdana",     // The font family name.
			NULL,           // The font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			13.0f,
			L"en-us",
			&m_pTextFormat
		);
	}
	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Verdana",     // The font family name.
			NULL,           // The font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			25.0f,
			L"en-us",
			&m_score_TextFormat
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDirect2dFactory->CreateRectangleGeometry(D2D1::RectF(0, 0, 100, 100), &m_pRectGeo);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDirect2dFactory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_ROUND,
				D2D1_LINE_JOIN_MITER,
				10.0f,
				D2D1_DASH_STYLE_DASH,
				0.0f),
			NULL,
			0,
			&m_pStrokeStyleDash
		);
	}

	// 새 경로
	if (SUCCEEDED(hr))
	{
		hr = m_pDirect2dFactory->CreatePathGeometry(&bird_PathGeometry);

		if (SUCCEEDED(hr))
		{
			hr = bird_PathGeometry->Open(&pSink);
			if (SUCCEEDED(hr))
			{
				D2D1_POINT_2F currentLocation = { 0, 0 };
				pSink->BeginFigure(currentLocation, D2D1_FIGURE_BEGIN_HOLLOW);
				pSink->AddBezier(D2D1::BezierSegment(D2D1::Point2F(800, 300), D2D1::Point2F(500, 200), D2D1::Point2F(1600, 135)));
				pSink->EndFigure(D2D1_FIGURE_END_OPEN);
				hr = pSink->Close();
			}
			SAFE_RELEASE(pSink);
		}
	}
	// 벌 경로
	if (SUCCEEDED(hr))
	{
		hr = m_pDirect2dFactory->CreatePathGeometry(&bee_PathGeometry);
		if (SUCCEEDED(hr))
		{
			hr = bee_PathGeometry->Open(&pSink);
			if (SUCCEEDED(hr))
			{
				D2D1_POINT_2F currentLocation = { 200, 300 };
				pSink->BeginFigure(currentLocation, D2D1_FIGURE_BEGIN_HOLLOW);
				pSink->AddBezier(D2D1::BezierSegment(D2D1::Point2F(1100,100), D2D1::Point2F(800,300), D2D1::Point2F(400, 135)));
				pSink->AddBezier(D2D1::BezierSegment(D2D1::Point2F(750, 170), D2D1::Point2F(1100, 170), D2D1::Point2F(1000, 700)));
				pSink->AddBezier(D2D1::BezierSegment(D2D1::Point2F(200, 800), D2D1::Point2F(600, 250), D2D1::Point2F(600, 300)));
				pSink->AddBezier(D2D1::BezierSegment(D2D1::Point2F(1100, 800), D2D1::Point2F(1400, 100), D2D1::Point2F(200, 300)));
				pSink->EndFigure(D2D1_FIGURE_END_OPEN);
				hr = pSink->Close();
			}
			SAFE_RELEASE(pSink);
		}
	}

	// 태양
	if (SUCCEEDED(hr))
	{
		hr = m_pDirect2dFactory->CreatePathGeometry(&sunGeometry);

		if (SUCCEEDED(hr))
		{
			hr = sunGeometry->Open(&pSink);

			if (SUCCEEDED(hr))
			{
				D2D1_POINT_2F currentLocation = { 270.0f , 255.0f };
				pSink->BeginFigure(currentLocation, D2D1_FIGURE_BEGIN_FILLED);
				pSink->AddArc(ArcSegment(Point2F(440, 255), SizeF(85, 85), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
				pSink->EndFigure(D2D1_FIGURE_END_OPEN);

				pSink->BeginFigure(currentLocation, D2D1_FIGURE_BEGIN_FILLED);
				pSink->AddArc(ArcSegment(Point2F(440, 255), SizeF(85, 85), 0, D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
				pSink->EndFigure(D2D1_FIGURE_END_OPEN);

				pSink->BeginFigure(Point2F(330, 220), D2D1_FIGURE_BEGIN_HOLLOW);
				pSink->AddBezier(BezierSegment(Point2F(330, 220), Point2F(320, 210), Point2F(310, 220)));
				pSink->AddBezier(BezierSegment(Point2F(310, 220), Point2F(300, 230), Point2F(290, 210)));
				pSink->EndFigure(D2D1_FIGURE_END_OPEN);

				pSink->BeginFigure(Point2F(380, 220), D2D1_FIGURE_BEGIN_HOLLOW);
				pSink->AddBezier(BezierSegment(Point2F(380, 220), Point2F(390, 210), Point2F(400, 220)));
				pSink->AddBezier(BezierSegment(Point2F(400, 220), Point2F(410, 230), Point2F(420, 210)));
				pSink->EndFigure(D2D1_FIGURE_END_OPEN);

				pSink->BeginFigure(Point2F(300, 280), D2D1_FIGURE_BEGIN_HOLLOW);
				pSink->AddQuadraticBezier(QuadraticBezierSegment(Point2F(355, 360), Point2F(410,280)));
				pSink->EndFigure(D2D1_FIGURE_END_OPEN);

				hr = pSink->Close();
			}
			SAFE_RELEASE(pSink);
		}
	}
	return hr;
}