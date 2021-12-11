#include "hw3.h"
#include <ctime>
#include <string>

using namespace D2D1;

// 현재 윈도우 창의 크기
FLOAT winWidth;
FLOAT winHeight;

int score = 0;

// 현재 마우스 위치 좌표 
D2D_POINT_2F currentMousePosition;
D2D_POINT_2F UpMousePosition;
// 총 위치
D2D1_RECT_F gunRectangle[3] = { RectF(300.0f, 600.0f, 400.0f, 700.0f), RectF(650.0f, 600.0f, 750.0f, 700.0f), RectF(1000.0f, 600.0f, 1100.0f, 700.0f) };

// 표적지 랜덤 위치
D2D1_RECT_F targetRandomRectangle[4] = { RectF(150,300,250,400), RectF(475,450,575,550),RectF(800,300,900,400),RectF(1125,450,1225,550) };

D2D1_RECT_F targetRectangle = targetRandomRectangle[0];
D2D1_RECT_F deerRectangle = targetRandomRectangle[1];
D2D1_RECT_F trapRectangle = targetRandomRectangle[2];
D2D1_RECT_F rabbitRectangle = targetRandomRectangle[3];

D2D1_SIZE_F rtSize;
D2D1_SIZE_U ch0Size, ch1Size;

// Flag
BOOL isPistol = false; BOOL isShotgun = false; BOOL isRifle = false;
BOOL isSelecting = false;

clock_t shotTime;	// 마우스 땔 때 shot 비트맵 출력하기 위한 시간
time_t targetTime;	// 표적지 맞춰야 하는 시간

enum Gun {
	Pistol = 0,
	Rifle = 1,
	Shotgun = 2,
	init = 3
};

std::string getramenString[3] = { "권총", "라이플", "샷건" };

Gun currentGun = init;
Gun selectingGun = init;

boolean isMouseIn(D2D1_POINT_2F inputMouse, D2D1_RECT_F candidateRect) {
	if (inputMouse.x >= candidateRect.left && inputMouse.x <= candidateRect.right
		&& inputMouse.y >= candidateRect.top && inputMouse.y <= candidateRect.bottom)
		return true;
	else
		return false;
}

// mouse cursor position, rectangle rotation & resize
void DemoApp::WriteActionInfo()
{
	m_pRenderTarget->SetTransform(Matrix3x2F::Identity());

	// debug
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	D2D1_SIZE_U size = SizeU(rc.right - rc.left, rc.bottom - rc.top);

	WCHAR szText[250];
	swprintf_s(szText, L"마우스x : %1.f\n마우스y : %1.f\n시간: %d",
		currentMousePosition.x, currentMousePosition.y, time(NULL) - targetTime);

	m_pRenderTarget->DrawText(
		szText,
		wcslen(szText),
		m_pTextFormat,
		RectF(10.0f, 10.0f, 200.0f, 200.0f),
		m_pTextBrush
	);

	swprintf_s(szText, L"SCORE : %d", score);

	m_pRenderTarget->DrawText(
		szText,
		wcslen(szText),
		m_score_TextFormat,
		RectF(600.0f, 15.0f, 800.f, 240.f),
		m_pTextBrush
	);

	m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
}

D2D1_RECT_F getRect(int width, int height) {
	return RectF(0, 0, width, height);
}

// This method discards device-specific resources if the Direct3D device dissapears during execution and recreates the resources the next time it's invoked.
HRESULT DemoApp::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		rtSize = m_pRenderTarget->GetSize();

		// Get ready to draw.
		m_pRenderTarget->BeginDraw();

		// Reset to the identity transform.
		m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
		// Clear the render target contents.
		m_pRenderTarget->Clear(ColorF(ColorF::White));

		D2D1_RECT_F rcBrushRect = RectF(0, 0, 100, 100);
		D2D1_RECT_F rcBrushRect2 = RectF(0, 0, 150, 100);
		// 배경 그리기 
		m_pRenderTarget->FillRectangle(&RectF(0, 0, rtSize.width, rtSize.height), m_pBackgroundBitmapBrush);

	
		// 총 그리기
		m_pRenderTarget->SetTransform(Matrix3x2F::Translation(300, 600));
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		m_pRenderTarget->FillGeometry(m_pRectGeo,
			gunBrush[0], gunOpacityBrush[0]);
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

		m_pRenderTarget->SetTransform(Matrix3x2F::Translation(650, 600));
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		m_pRenderTarget->FillGeometry(m_pRectGeo,
			gunBrush[1], gunOpacityBrush[1]);
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

		m_pRenderTarget->SetTransform(Matrix3x2F::Translation(1000, 600));
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		m_pRenderTarget->FillGeometry(m_pRectGeo,
			gunBrush[2], gunOpacityBrush[2]);
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

		// 총 받침 선반
		m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
		D2D1_RECT_F rect = RectF(600,700,800,800);
		m_pSceneBrush->SetColor(ColorF(ColorF::Black, 2.f));
		m_pRenderTarget->DrawRectangle(rect, m_pSceneBrush, 1.0f, m_pStrokeStyleDash);
		m_pSceneBrush->SetColor(ColorF(ColorF::DarkGreen, 1.f));
		m_pRenderTarget->FillRectangle(rect, m_pSceneBrush);

		D2D1_RECT_F rect1 = RectF(250, 700, 450, 800);
		m_pSceneBrush->SetColor(ColorF(ColorF::Black, 2.f));
		m_pRenderTarget->DrawRectangle(rect1, m_pSceneBrush, 1.0f, m_pStrokeStyleDash);
		m_pSceneBrush->SetColor(ColorF(ColorF::DarkGreen, 1.f));
		m_pRenderTarget->FillRectangle(rect1, m_pSceneBrush);

		D2D1_RECT_F rect2 = RectF(950, 700, 1150, 800);
		m_pSceneBrush->SetColor(ColorF(ColorF::Black, 2.f));
		m_pRenderTarget->DrawRectangle(rect2, m_pSceneBrush, 1.0f, m_pStrokeStyleDash);
		m_pSceneBrush->SetColor(ColorF(ColorF::DarkGreen, 1.f));
		m_pRenderTarget->FillRectangle(rect2, m_pSceneBrush);

		// 부사수 그리기
		if (isSelecting == false) {
			targetTime = time(NULL);
			srand(time(NULL));
			int i = rand() % 3;
			selectingGun = Gun(i);
			isSelecting = true;	
		}
		m_pRenderTarget->SetTransform(Matrix3x2F::Translation(50, 500));
		m_pRenderTarget->FillRectangle(&RectF(0, 0, rtSize.width, rtSize.height), assistant_BitmapBrush);

		// 부사수 말
		m_pRenderTarget->SetTransform(Matrix3x2F::Translation(50, 450));
		m_pRenderTarget->FillRectangle(&getRect(100, 100), tell_BitmapBrush);
		const char* multiByte = getramenString[selectingGun].c_str();
		TCHAR temp[15]; memset(temp, 0, sizeof(temp));
		MultiByteToWideChar(CP_ACP, MB_COMPOSITE, multiByte, -1, temp, 15);
		WCHAR szText[250];
		swprintf_s(szText, L" %s을\n 이용해!", temp);
		m_pRenderTarget->DrawText(
			szText,
			wcslen(szText),
			m_pTextFormat,
			RectF(20.0f, 30.0f, 100.0f, 100.0f),
			m_pTextBrush
		);

		if (isSelecting == true)
		{
			// 표적지 그리기 (랜덤 적용 x)
			Matrix3x2F t1, t2, t3, t4;
			t1 = Matrix3x2F::Translation(150, 300);
			t2 = Matrix3x2F::Translation(475, 450);
			t3 = Matrix3x2F::Translation(800, 300);
			t4 = Matrix3x2F::Translation(1125, 450);

			if (time(NULL) - targetTime >= 2)	// 총 선택시간 2초 후 표적지 생성
			{
				m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
				m_pRenderTarget->SetTransform(t1);
				m_pRenderTarget->FillRectangle(&rcBrushRect, target_BitmapBrush);

				m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
				m_pRenderTarget->SetTransform(t2);
				m_pRenderTarget->FillRectangle(&rcBrushRect, deer_BitmapBrush);

				m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
				m_pRenderTarget->SetTransform(t3);
				m_pRenderTarget->FillRectangle(&rcBrushRect, trap_BitmapBrush);

				m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
				m_pRenderTarget->SetTransform(t4);
				m_pRenderTarget->FillRectangle(&rcBrushRect, rabbit_BitmapBrush);
			}
		}

		// 태양
		m_pRenderTarget->SetTransform(Matrix3x2F::Translation(850, -100));
		m_pSceneBrush->SetColor(ColorF(ColorF::Red, 1.f));
		m_pRenderTarget->FillGeometry(sunGeometry, m_pSceneBrush);
		m_pSceneBrush->SetColor(ColorF(ColorF::Black, 1.f));
		m_pRenderTarget->DrawGeometry(sunGeometry, m_pSceneBrush, 1.f);

		// 마우스 놓을 때 shot
		if (double((clock() - shotTime)) / CLOCKS_PER_SEC <= 0.5)
		{
			m_pRenderTarget->SetTransform(Matrix3x2F::Translation(UpMousePosition.x - 50, UpMousePosition.y - 50));
			m_pRenderTarget->FillRectangle(&rcBrushRect, shot_BitmapBrush);

			m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
		}

		//  마우스 움직임 따라서 총 따라옴
		if (isPistol == true) {
			m_pRenderTarget->SetTransform(Matrix3x2F::Translation(currentMousePosition.x - 50, currentMousePosition.y - 50));
			m_pRenderTarget->FillRectangle(&rcBrushRect, gunBrush[0]);
		}
		else if (isRifle == true) {
			m_pRenderTarget->SetTransform(Matrix3x2F::Translation(currentMousePosition.x - 50, currentMousePosition.y - 50));
			m_pRenderTarget->FillRectangle(&rcBrushRect, gunBrush[1]);
		}
		else if (isShotgun == true) {
			m_pRenderTarget->SetTransform(Matrix3x2F::Translation(currentMousePosition.x - 50, currentMousePosition.y - 50));
			m_pRenderTarget->FillRectangle(&rcBrushRect, gunBrush[2]);
		}
		m_pRenderTarget->SetTransform(Matrix3x2F::Identity());

		/* 새 */
		static float float_time = 0.0f;
		float length = m_Animation.GetValue(float_time);
		D2D1_POINT_2F point;
		D2D1_POINT_2F tangent;
		D2D1_MATRIX_3X2_F triangleMatrix;
		hr = bird_PathGeometry->ComputePointAtLength(
			length,
			NULL,
			&point,
			&tangent);
		Assert(SUCCEEDED(hr));
		// 이동
		ch0Size = bird_Bitmap->GetPixelSize();
		Matrix3x2F tmp = Matrix3x2F::Translation(
			point.x - ch0Size.width / 2,
			point.y - ch0Size.height / 2
		);
		triangleMatrix = Matrix3x2F(
			-tangent.x, -tangent.y,
			tangent.y, -tangent.x,
			point.x - ch0Size.width / 2,
			point.y - ch0Size.height / 2
		);

		// 그리기
		m_pRenderTarget->SetTransform(tmp);
		m_pRenderTarget->FillRectangle(&rcBrushRect, bird_BitmapBrush);

		// 애니메이션 끝날 때 조건
		if (float_time >= m_Animation.GetDuration())
		{
			float_time = 0.0f;
		}
		else
		{
			float_time += static_cast<float>(bird_DwmTimingInfo.rateCompose.uiDenominator) /
				static_cast<float>(bird_DwmTimingInfo.rateCompose.uiNumerator);
		}

		/* 벌 */
		static float float_time1 = 0.0f;
		float length1 = m_Animation2.GetValue(float_time1);
		D2D1_POINT_2F point1;
		D2D1_POINT_2F tangent1;
		D2D1_MATRIX_3X2_F triangleMatrix1;
		hr = bee_PathGeometry->ComputePointAtLength(
			length1,
			NULL,
			&point1,
			&tangent1);
		Assert(SUCCEEDED(hr));
		ch1Size = bee_Bitmap->GetPixelSize();
		Matrix3x2F tmp1 = Matrix3x2F::Translation(
			point1.x - ch1Size.width / 2,
			point1.y - ch1Size.height / 2
		);
		triangleMatrix1 = Matrix3x2F(
			-tangent1.x, -tangent1.y,
			tangent1.y, -tangent1.x,
			point1.x - ch1Size.width / 2,
			point1.y - ch1Size.height / 2
		);
		Matrix3x2F rotation = Matrix3x2F::Rotation(
			30.0f, Point2F(0, 0)
		);
		m_pRenderTarget->SetTransform(rotation * tmp1);
		m_pRenderTarget->FillRectangle(&rcBrushRect, bee_BitmapBrush);
		if (float_time1 >= m_Animation2.GetDuration())
		{
			float_time1 = 0.0f;
		}
		else
		{
			float_time1 += static_cast<float>(bee_DwmTimingInfo.rateCompose.uiDenominator) /
				static_cast<float>(bee_DwmTimingInfo.rateCompose.uiNumerator);
		}

		WriteActionInfo(); // Write mouse cursor position, rectangle rotation & resize, etc...

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

// Handles window messages.
LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		DemoApp* pDemoApp = (DemoApp*)pcs->lpCreateParams;

		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pDemoApp));

		result = 1;
	}
	else
	{
		DemoApp* pDemoApp = reinterpret_cast<DemoApp*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));

		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			// 윈도우 창 크기 변환 막기
			case WM_GETMINMAXINFO:
			{
				MINMAXINFO* mmi = (MINMAXINFO*)lParam;
				mmi->ptMinTrackSize.x = 1400;
				mmi->ptMinTrackSize.y = 800;
				mmi->ptMaxTrackSize.x = 1400;
				mmi->ptMaxTrackSize.y = 800;
			}
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_LBUTTONUP:
			{
				UpMousePosition.x = LOWORD(lParam);
				UpMousePosition.y = HIWORD(lParam);

				if (isPistol || isRifle || isShotgun)
				{
					shotTime = clock();

					if (isPistol == true)
						currentGun = Pistol;
					else if (isRifle == true)
						currentGun = Rifle;
					else if (isShotgun == true)
						currentGun = Shotgun;
					
					if (currentGun == selectingGun) {
						if (isMouseIn(currentMousePosition, targetRectangle) == true) score += 100;
						else if (isMouseIn(currentMousePosition, trapRectangle) == true) score -= 50;	// 총은 맞으나 트랩 명중
						else if (isMouseIn(currentMousePosition, deerRectangle) == true || isMouseIn(currentMousePosition, rabbitRectangle) == true) score -= 150;  // 동물 명중
					}
					else if (currentGun != selectingGun) {
						if (isMouseIn(currentMousePosition, targetRectangle) == true) score += 30;
						else if (isMouseIn(currentMousePosition, trapRectangle) == true) score -= 80;	// 트랩 명중
						else if (isMouseIn(currentMousePosition, deerRectangle) == true || isMouseIn(currentMousePosition, rabbitRectangle) == true) score -= 150;  // 동물 명중
					}
					else score += 0;
				}

				currentGun = init;
				selectingGun = init;
				isSelecting = false;

				isPistol = false; isRifle = false; isShotgun = false;	// 마우스 떼면 드래깅되는 총 사라짐
				InvalidateRect(hwnd, NULL, FALSE);
				break;
			}
			case WM_LBUTTONDOWN:
			{
				if (isMouseIn(currentMousePosition, gunRectangle[0]) == true) {
					isPistol = true;
				}
				else if (isMouseIn(currentMousePosition, gunRectangle[1]) == true) {
					isRifle = true;
				}
				else if (isMouseIn(currentMousePosition, gunRectangle[2]) == true) {
					isShotgun = true;
				}
				InvalidateRect(hwnd, NULL, FALSE);
				break;
			}

			case WM_MOUSEMOVE:
			{
				currentMousePosition.x = LOWORD(lParam);
				currentMousePosition.y = HIWORD(lParam);

				if (time(NULL) - targetTime > 5)	// 사격 제한 시간(3초 이내에 사격하지 않을 경우 -20점)
				{
					score -= 20;
					currentGun = init;
					selectingGun = init;
					isSelecting = false;
					isPistol = false; isRifle = false; isShotgun = false;
					InvalidateRect(hwnd, NULL, FALSE);
				}

				InvalidateRect(hwnd, NULL, FALSE);
				break;
			}

			case WM_PAINT:
			{
				pDemoApp->OnRender();

				// Do not call ValidateRect so that the window will be redrawn.
				//ValidateRect(hwnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}