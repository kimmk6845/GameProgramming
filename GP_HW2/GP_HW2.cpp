// GP_HW2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "GP_HW2.h"

#define MAX_LOADSTRING 100
#define GAP 30

using namespace std;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


// ** 학생 정보 구조체
struct student {
    string name;
    int score;
};
// ** 학생 이름 배열
string Lastname[10] = { "김", "이", "박", "서", "정", "윤", "한", "가", "오", "장" };
string Firstname[10] = { "부천","서울","인천","부산","울산","대구","대전","제주","강원","전주" };

// ** 학생에 대한 빈 vector 선언
vector<student> v;

// ** 마우스 현재 좌표와 마지막으로 클릭된 좌표
D2D_POINT_2F now;
D2D_POINT_2F last;

int r_width = 50;
int window = 800;

int centerX, centerY = 0;

// ** 화면에 표시될 박스의 회전인자, 크기조정인자, 학생 데이터 박스 개수
float angle = 0;
float boxSize = 0;
int vectorSize = 0;

boolean isInserting = false;
boolean isInserted = false;
boolean isDeleted = false;
boolean isDeleting = false;

D2D1_RECT_F rectangle;

// ** 학생 정보를 벡터에 넣는 함수
void insert(student& s)
{
    random_device rd;  // ** 시드 값을 얻기 위한 random_device 생성
    mt19937 gen(rd()); // ** rd를 통해 난수 생성 엔진 초기화 --> 난수 생성 속도를 위함 (초기화하지 않으면 os가 주변 환경과 무작위적 상호작용)
    uniform_int_distribution<int> dis(50, 99);     // ** 50 ~ 99까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의

    s.name = Lastname[dis(gen) % 10] + Firstname[dis(gen) % 10];
    s.score = dis(gen);
    v.push_back(s);
}

// ** 비교 함수(내림차순)
bool compare(const student& s1, const student& s2)
{
    return s1.score > s2.score;
}

// ** 2.1 생성자
DemoApp::DemoApp() :
    m_hwnd(NULL),
    m_pD2DFactory(NULL),
    m_pRenderTarget(NULL),
    m_pStrokeStyleDash(NULL),
    m_pGridPatternBitmapBrush(NULL),
    m_pOriginBorderBrush(NULL),
    m_pRectangleBorderBrush(NULL),
    m_pRectangleBrush{ NULL, },
    m_pOriginFillBrush(NULL),
    m_pTextBrush(NULL),
    m_pDWriteFactory(NULL),
    m_pTextFormat(NULL)
{
}

// ** 소멸자
DemoApp::~DemoApp()
{
    SAFE_RELEASE(m_pD2DFactory);
    SAFE_RELEASE(m_pRenderTarget);
    SAFE_RELEASE(m_pStrokeStyleDash);
    SAFE_RELEASE(m_pGridPatternBitmapBrush);
    SAFE_RELEASE(m_pOriginBorderBrush);
    SAFE_RELEASE(m_pRectangleBorderBrush);
    for (int i = 0; i < MAX_STUDENT; i++)
        SAFE_RELEASE(m_pRectangleBrush[i]);     // 학생 수만큼 생성된 박스 브러쉬 자원 반납
    SAFE_RELEASE(m_pOriginFillBrush);
    SAFE_RELEASE(m_pTextBrush);
    SAFE_RELEASE(m_pDWriteFactory);
    SAFE_RELEASE(m_pTextFormat);
}

// ** 2.2 메시지 루프 함수
void DemoApp::RunMessageLoop()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// ** 2.3 초기화 함수
HRESULT DemoApp::Initialize(HINSTANCE hInstance)
{
    // 장치 독립적 자원 생성
    HRESULT hr = CreateDeviceIndependentResources();

    if (SUCCEEDED(hr))
    {
        // 윈도우 클래스 등록
        WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = DemoApp::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(LONG_PTR);
        wcex.hInstance = hInstance;
        wcex.hbrBackground = NULL;
        wcex.lpszMenuName = NULL;
        wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
        wcex.lpszClassName = L"HW2";
        RegisterClassEx(&wcex);

        // 윈도우 생성
        m_hwnd = CreateWindow(  
            L"HW2", L"HW#2 변환 응용프로그램",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            window + 250, window, NULL, NULL, hInstance, this
        );
        hr = m_hwnd ? S_OK : E_FAIL;
        if (SUCCEEDED(hr))
        {
            ShowWindow(m_hwnd, SW_SHOWNORMAL);
            UpdateWindow(m_hwnd);
        }
    }

    return hr;
}


// ** 2.4 진입점 함수
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    if (SUCCEEDED(CoInitialize(NULL)))
    {
        DemoApp app;

        if (SUCCEEDED(app.Initialize(hInstance)))   // COM 라이브러리 로딩 및 초기화
        {
            app.RunMessageLoop();
        }
        CoUninitialize();   // COM 라이브러리 언로드
    }
    return 0;
}

///////////////
//           //
//   3단계   //
//           //
///////////////

// ** 3.1 장치 독립적 자원 생성 함수
HRESULT DemoApp::CreateDeviceIndependentResources()
{
    HRESULT hr = S_OK;

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(   // DirectWrite 팩토리 생성
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pDWriteFactory->CreateTextFormat( // 텍스트 포멧 생성
            L"Verdana", NULL, 
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            13.0f,
            L"en-us",
            &m_pTextFormat
        );
        m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);            // 문단 가운데 정렬
        m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);  // 문장 내의 수직 정렬
    }
    return hr;
}

// ** 3.2 장치 의존적 자원 생성 함수
HRESULT DemoApp::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    if (!m_pRenderTarget)   // 렌더타겟 유효할 땐 실행 x
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);     // rc를 이용해 그리기 영역 크기 초기화
        hr = m_pD2DFactory->CreateHwndRenderTarget(     // 렌더타겟 생성
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &m_pRenderTarget
        );

        if (SUCCEEDED(hr))
        {
            // 변환 전 외곽선 브러쉬 생성
            hr = m_pRenderTarget->CreateSolidColorBrush(    
                D2D1::ColorF(D2D1::ColorF::Gray),
                &m_pOriginBorderBrush);
        }
        
        if (SUCCEEDED(hr))
        {
            // 변환 후 외곽선 영역 브러쉬 생성
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::White),
                &m_pOriginFillBrush);
        }

        if (SUCCEEDED(hr))
        {
            // Text 브러쉬 생성
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Black),
                &m_pTextBrush);
        }

        if (SUCCEEDED(hr))
        {
            // 학생 데이터 박스 외곽선 브러쉬 생성
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Black),
                &m_pRectangleBorderBrush);
        }

        float color[3];
        for (int i = 0; i < MAX_STUDENT; i++) {
            color[0] = rand() % 255 / 100;
            color[1] = rand() % 255 / 100;
            color[2] = rand() % 255 / 100;
            if (SUCCEEDED(hr))
            {
                // 학생 데이터 박스 색상 브러쉬 생성
                hr = m_pRenderTarget->CreateSolidColorBrush(
                    D2D1::ColorF( color[0], color[1], color[2], 0.5),
                    &m_pRectangleBrush[i]);
            }
        }

        // 격자모양 패턴 브러쉬 생성
        if (SUCCEEDED(hr))
        {
            hr = CreateGridPatternBrush(m_pRenderTarget, &m_pGridPatternBitmapBrush);
        }
    }

    return hr;
}

// 격자 모양 만들기
HRESULT DemoApp::CreateGridPatternBrush(ID2D1RenderTarget* pRenderTarget, ID2D1BitmapBrush** ppBitmapBrush)
{
    HRESULT hr = S_OK;

    ID2D1BitmapRenderTarget* pCompatibleRenderTarget = NULL;
    hr = pRenderTarget->CreateCompatibleRenderTarget(
        D2D1::SizeF(10.0f, 10.0f),
        &pCompatibleRenderTarget);

    if (SUCCEEDED(hr))
    {
        // 패턴 그리기
        ID2D1SolidColorBrush* pGridBrush = NULL;
        hr = pCompatibleRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(0.93f, 0.94f, 0.96f, 1.0f),
            &pGridBrush);

        if (SUCCEEDED(hr))
        {
            pCompatibleRenderTarget->BeginDraw();
            pCompatibleRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.0f, 10.0f, 1.0f), pGridBrush);
            pCompatibleRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.0f, 1.0f, 10.0f), pGridBrush);
            hr = pCompatibleRenderTarget->EndDraw();

            if (hr == D2DERR_RECREATE_TARGET)
            {
                DiscardDeviceResources();
            }
            if (SUCCEEDED(hr))
            {
                // 렌더타겟으로부터 비트맵을 얻음
                ID2D1Bitmap* pGridBitmap = NULL;
                hr = pCompatibleRenderTarget->GetBitmap(&pGridBitmap);

                if (SUCCEEDED(hr))
                {
                    // 비트맵 브러쉬 생성
                    hr = m_pRenderTarget->CreateBitmapBrush(
                        pGridBitmap,
                        D2D1::BitmapBrushProperties(
                            D2D1_EXTEND_MODE_WRAP,
                            D2D1_EXTEND_MODE_WRAP),
                        ppBitmapBrush
                    );

                    pGridBitmap->Release();
                }
            }

            pGridBrush->Release();
        }

        pCompatibleRenderTarget->Release();
    }

    return hr;
}

// ** 3.3 장치 의존적 자원 반납 함수
void DemoApp::DiscardDeviceResources()
{
    SAFE_RELEASE(m_pRenderTarget);
    SAFE_RELEASE(m_pRectangleBorderBrush);
    SAFE_RELEASE(m_pOriginFillBrush);
    SAFE_RELEASE(m_pOriginBorderBrush);
    SAFE_RELEASE(m_pTextBrush);
    SAFE_RELEASE(m_pGridPatternBitmapBrush);
    for (int i = 0; i < MAX_STUDENT; i++)
        SAFE_RELEASE(m_pRectangleBrush[i]);     // 학생 수만큼 생성된 박스 브러쉬 자원 반납
}


//////////////
//          //
//  4단계   //
//          //
//////////////

// ** 너비:높이 = 2:1인 가상 박스와 좌상단의 마우스 좌표, Rotation인자, Scale인자, 박스 개수
void DemoApp::drawInit()
{
    m_pOriginBorderBrush->SetOpacity(0.3);
    D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
    centerX = rtSize.width / 2;
    centerY = rtSize.height / 2;

    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    rectangle = D2D1::Rect(centerX - r_width, GAP, centerX + r_width, GAP + r_width);     // 가상 박스의 너비:높이 = 2:1
    m_pRenderTarget->FillRectangle(rectangle, m_pOriginBorderBrush);                      // 가상 박스 그리기

    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    WCHAR szText[100];

    // 텍스트 정보 출력
    swprintf_s(szText, L"마우스 X 좌표: %.2f\n마우스 Y 좌표: %.2f\n회전 인자: %.2f\n크기조정 인자: %.2f\n박스 개수 : %d\n",
        now.x, now.y, angle, boxSize, v.size());

    m_pRenderTarget->DrawText(szText, wcslen(szText), m_pTextFormat,
        D2D1::RectF(10.0f, 10.5f, 236.0f, 190.5f),
        m_pTextBrush);
}

// ** 다음 박스
D2D1_RECT_F getNextRect(int vectorSize) {
    float top = (window - GAP) - r_width * (vectorSize + 2);    // 다음에 그려질 박스의 좌표
    return D2D1::RectF(centerX - r_width * 2, top, centerX + r_width * 2, top + r_width);         // 높이:너비 = 1:4의 비율
}

// ** 박스 삽입하는 함수
void DemoApp::drawTempInsertingRectangle() {        
    D2D1_MATRIX_3X2_F translation = D2D1::Matrix3x2F::Translation(now.x - last.x, now.y - last.y);  // 이동 변환을 이용해 드래깅을 하면 박스가 끌려오는 것을 구현

    boxSize = ((now.y - last.y) / (getNextRect(vectorSize).top - r_width)) + 1;                     // 목표 지점까지 커지는 크기 조정 인자
    D2D1_MATRIX_3X2_F scale = D2D1::Matrix3x2F::Scale(D2D1::Size(boxSize, 1.0f), D2D1::Point2F(rectangle.left, rectangle.top));

    angle = (last.y - now.y) / (getNextRect(vectorSize).top - r_width) * 360;                       // 반시계방향으로 회전하는 회전 인자 (음의 값)
    D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F((rectangle.right + rectangle.left) / 2, (rectangle.top + rectangle.bottom) / 2));

    m_pRenderTarget->SetTransform(scale * rotation * translation);

    m_pRenderTarget->FillRectangle(rectangle, m_pOriginBorderBrush);
    
}

// ** 박스 삭제하는 함수
void DemoApp::drawTempdeletingRectangle() {         
    vectorSize--;
    D2D1_MATRIX_3X2_F translation = D2D1::Matrix3x2F::Translation(now.x - last.x, now.y - last.y);      // 드래깅으로 박스가 끌려옴

    if (now.y <= last.y)        // 위쪽으로 드래깅할 경우
        boxSize = (getNextRect(vectorSize).top - r_width) / ((last.y - now.y) + (getNextRect(vectorSize).top - r_width));       // 가상 박스까지는 0.5배, 그 위로 갈수록 작아짐
    else                        // 아래쪽으로 드래깅할 경우
        boxSize = ((now.y - last.y) / (getNextRect(vectorSize).top - r_width)) + 1;         // 삽입할 때와 마찬가지로 인자 값이 커짐
    D2D1_MATRIX_3X2_F scale = D2D1::Matrix3x2F::Scale(D2D1::Size(boxSize, 1.0f), D2D1::Point2F(last.x, last.y));

    angle = (now.y - last.y) / (getNextRect(vectorSize).top - r_width) * 360 * -1;                      // 시계방향으로 회전하는 회전 인자 (양의 값)
    D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(last.x, last.y));

    m_pRenderTarget->SetTransform(scale * rotation * translation);
    m_pRenderTarget->FillRectangle(getNextRect(vectorSize), m_pOriginBorderBrush);

    vectorSize++;

}

// ** 4.1 윈도우 메시지 처리를 위한 프로시저 함수
LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        DemoApp* pDemoApp = (DemoApp*)pcs->lpCreateParams;

        SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pDemoApp));

        return 1;
    }
    else {
        DemoApp* pDemoApp = reinterpret_cast<DemoApp*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)));

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
            return 0;

            case WM_DISPLAYCHANGE:
            {
                InvalidateRect(hwnd, NULL, FALSE);
            }
            return 0;
            // ** 좌클릭 버튼을 누를 때 이벤트
            case WM_LBUTTONDOWN:
            {
                // 가상 박스 영역이 클릭됐을 때
                if (LOWORD(lParam) >= rectangle.left && LOWORD(lParam) <= rectangle.right
                    && HIWORD(lParam) >= rectangle.top && HIWORD(lParam) <= rectangle.bottom) {

                    if (vectorSize == MAX_STUDENT) {    // 8개이상의 데이터를 삽입하려고 할 때
                        isInserting = false;
                        ::MessageBox(0, L"최대 8개까지만 가능합니다.", L"벡터가 가득 찼습니다.", MB_OK);
                        ::MessageBeep(MB_OK);
                        break;
                    }

                    isInserting = true; // 삽입모드 On
                    last.x = LOWORD(lParam);
                    last.y = HIWORD(lParam);
                }
                else if (v.size() == 0)         // 데이터가 없을 때 허상의 박스가 끌려오는 것(삭제동작)을 방지
                    break;
                else {
                    D2D1_RECT_F lastRectangle = getNextRect(vectorSize - 1);
                    // 가장 위에 있는 데이터가 클릭됐을 때
                    if (LOWORD(lParam) >= lastRectangle.left && LOWORD(lParam) <= lastRectangle.right
                        && HIWORD(lParam) >= lastRectangle.top && HIWORD(lParam) <= lastRectangle.bottom) {
                        isDeleting = true; // 삭제모드 On
                        last.x = LOWORD(lParam);
                        last.y = HIWORD(lParam);
                    }
                }
                break;
            } 
            //** 좌클릭 버튼을 땔 때 이벤트
            case WM_LBUTTONUP:
            {
                now.x = LOWORD(lParam);
                now.y = HIWORD(lParam);
                if (isInserting == true) {  // 현재 삽입모드일 때
                    // 마우스를 때는 위치가 데이터 박스가 들어갈 자리면
                    D2D1_RECT_F nowvector = getNextRect(vectorSize);
                    if (LOWORD(lParam) >= nowvector.left && LOWORD(lParam) <= nowvector.right
                        && HIWORD(lParam) >= nowvector.top && HIWORD(lParam) <= nowvector.bottom) {
                        isInserted = true;      // 삽입완료 On
                        isInserting = false;    // 삽입모드 Off
                    }
                    InvalidateRect(hwnd, NULL, false);
                }
                else if (isDeleting == true) {  // 현재 삭제모드일 때
                    // 마우스를 때는 위치가 가상 박스 안에 들어와있으면
                    if (LOWORD(lParam) >= rectangle.left && LOWORD(lParam) <= rectangle.right
                        && HIWORD(lParam) >= rectangle.top && HIWORD(lParam) <= rectangle.bottom) {
                        isDeleted = true;       // 삭제완료 On
                        isDeleting = false;     // 삭제모드 Off
                    }
                    InvalidateRect(hwnd, NULL, false);
                }
                // 초기화
                angle = 0;
                boxSize = 0;
                isInserting = false;
                isDeleting = false;
                InvalidateRect(hwnd, NULL, false);

                return 0;
            }
            case WM_PAINT:
            {
                pDemoApp->OnRender();
                ValidateRect(hwnd, NULL);
                return 0;
            }
            case WM_MOUSEMOVE:
            {
                now.x = LOWORD(lParam);
                now.y = HIWORD(lParam);
                InvalidateRect(hwnd, NULL, FALSE);
            }
            break;
            case WM_DESTROY:
            {
                PostQuitMessage(0);
            }
            return 1;
            }
        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

// ** 4.2 윈도우 다시 그리는 OnRender함수
HRESULT DemoApp::OnRender()
{
    HRESULT hr = CreateDeviceResources();   // 렌더할 때마다 의존적 자원들 생성 함수 호출

    if (SUCCEEDED(hr))
    {
        m_pRenderTarget->BeginDraw();       // 그리기 시작

        m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());    //변환 행렬을 항등행렬로

        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));  //창을 흰색으로 클리어

        D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

        m_pRenderTarget->FillRectangle(         // ** 격자 패턴 그리기
            D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height),
            m_pGridPatternBitmapBrush);

        drawInit();     // 텍스트 정보와 가상 박스 그리기

        // 삭제모드 중이거나 삭제를 완료하는 시점일 때
        if (isDeleting == true || isDeleted == true) {
            vectorSize--;
        }
        for (int i = 0; i < vectorSize; i++) {  // 벡터 컨테이너의 데이터 그리기
            m_pRenderTarget->FillRectangle(getNextRect(i), m_pRectangleBrush[i]);
            const char* multiByte = v[i].name.c_str();
            TCHAR temp[15];
            memset(temp, 0, sizeof(temp));
            MultiByteToWideChar(CP_ACP, MB_COMPOSITE, multiByte, -1, temp, 15);

            // 이름과 점수 출력
            static WCHAR nameAndScore[100];
            swprintf_s(nameAndScore, L"%s %d\n", temp, v[i].score);
            // Text 그리기
            m_pRenderTarget->DrawText(nameAndScore, wcslen(nameAndScore), m_pTextFormat, getNextRect(i), m_pTextBrush);
            // 상자 테두리 그리기
            m_pRenderTarget->DrawRectangle(getNextRect(i), m_pRectangleBorderBrush);
        }
        // 학생 데이터를 박스에 넣었으니 일시적으로 줄였던 vectorSize를 다시 복구시켜줌
        if (isDeleting == true || isDeleted == true) {
            vectorSize++;
        }

        // 삽입모드
        if (isInserting == true) {
            drawTempInsertingRectangle();
            m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());    
            m_pRenderTarget->DrawRectangle(getNextRect(vectorSize), m_pOriginBorderBrush);  // 삽일할 박스를 놓아야 할 장소를 그림
        }

        // 삭제모드
        if (isDeleting == true) {
            drawTempdeletingRectangle();
        }

        // 삽입완료
        if (isInserted == true) {
            vectorSize++;
            student s;

            insert(s);  // 벡터 컨테이너에 학생 데이터 삽입
            sort(v.begin(), v.end(), compare);
            isInserted = false;
        }
        // 삭제완료
        if (isDeleted == true) {
            vectorSize--;
            v.pop_back();
            isDeleted = false;
        }
        
        hr = m_pRenderTarget->EndDraw();     // **그리기 수행**
    }

    if (hr == D2DERR_RECREATE_TARGET)   // EndDraw 리턴값이 무효화값이면, 의존적 자원들 반납 후 다시 생성해야 함
    {
        hr = S_OK;
        DiscardDeviceResources();
    }

    return hr;
}

// ** 4.3 창 크기 바뀔때 조치 취하는 OnResize함수
void DemoApp::OnResize(UINT width, UINT height)
{
    if (m_pRenderTarget)
    {
        m_pRenderTarget->Resize(D2D1::SizeU(width, height));
    }
}