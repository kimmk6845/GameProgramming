// GP_HW1_201501414김민기.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "GP_HW1_201501414김민기.h"

using namespace std;

#define MAX_LOADSTRING 100
#define MAX_STUDENT 8                           // ** 학생의 정보 추가는 최대 8개

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
string Firstname[10] = { "부천","서울","인천","부산","울산","대구","대전","제주","강원","전주"};

// ** 학생에 대한 빈 vector 선언
vector<student> v;

// ** 박스 크기
int boxWidth = 300;
int boxHeight = 60;

// ** 박스 좌표
int boxX = 400;
int boxY = 450;

// ** 이벤트 마우스 좌표
int x, y;

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


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GPHW1201501414, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GPHW1201501414));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GPHW1201501414));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GPHW1201501414);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT OnPaint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);    // ** 그리기 준비, 그리기 정보를 ps 구조체에 채워서 두 번째 인자값으로 리턴 --> HDC 타입의 DC핸들을 리턴 --> 이 값은 ps 구조체의 hdc 필드를 통해서 접근
    
    // ** 학생 정보 박스들을 담는 박스 모양 그리기
    HPEN hpen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));           // ** 펜 생성
    HPEN hpenOld = (HPEN)SelectObject(hdc, (HGDIOBJ)hpen);      // ** 펜 선택
    Rectangle(hdc, boxX - 10, boxY - (boxHeight * 7) - 10, boxX + boxWidth + 10, boxY + boxHeight + 10);    // ** 박스 그리기
    hpen = (HPEN)SelectObject(hdc, hpenOld);                    // ** 기존 펜 선택
    DeleteObject(hpen);                                         // ** 생성한 펜 삭제

    HBRUSH hbr, hbrOld;
    for (int i = 0; i < v.size(); i++)
    {
        string name = v[i].name;
        int score = v[i].score;
        string scoreTxt = to_string(score);

        const char* n = name.c_str();       // ** textout의 인자로 사용하기 위함(string형을 char*형으로 바꿈)
        const char* s = scoreTxt.c_str();

        hbr = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));  // 박스를 채울 브러쉬 생성
        hbrOld = (HBRUSH)SelectObject(hdc, hbr);

        Rectangle(hdc, boxX, boxY, boxX + boxWidth, boxY + boxHeight);          // ** 학생 데이터 박스 그리기

        TextOut(hdc, boxX + boxWidth/2, boxY + boxHeight/2, n, strlen(n));      //  ** 박스 중앙에 학생의 이름
        TextOut(hdc, boxX + boxWidth/2 - 30, boxY + boxHeight/2, s, strlen(s)); // ** 학생의 점수

        boxY -= boxHeight;      // ** 다음 박스를 그리기 위한 y좌표 갱신

        SelectObject(hdc, hbrOld);
        DeleteObject(hbr);
    }

    EndPaint(hWnd, &ps);
    return 0;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:                            // ** 왼쪽 마우스 클릭에 대한 이벤트
        x = LOWORD(lParam), y = HIWORD(lParam);     // ** 클릭한 마우스의 x좌표와 y좌표 가져옴

        // ** 데이터 삭제
        if (boxX <= x && x <= boxX + boxWidth && boxY + boxHeight <= y && y <= boxY + boxHeight * 2) {
            v.pop_back();                           // ** 가장 뒤에 정보를 pop 시킴
            boxY = 450;                             // ** 값이 변하는 박스의 Y좌표를 초기화시키고 다시 그리도록 함
            InvalidateRect(hWnd, NULL, TRUE);       // ** 윈도우 전체를 갱신
            
            // ** 1개 남은 데이터를 삭제하면, 박스가 먼저 사라지고 메시지 박스를 출력하도록 InvalidateRect 함수 뒤에 넣음
            if (v.size() == 0)                      // ** 벡터 사이즈가 0이 되면 메시지 박스 출력
                ::MessageBox(0, "더 이상 삭제할 수 없습니다.", "Empty Vector", MB_OK);
            break;
        }

        if (v.empty())                              // ** vector가 빈 상태면
        {
            student s;
            insert(s);                              // ** 학생 정보를 넣음

            InvalidateRect(hWnd, NULL, FALSE);      // ** False를 넣어 배경은 그대로 두고 갱신
        }
        else {
            if (v.size() != MAX_STUDENT) {          // ** 데이터를 삽입
               
                student s;
                insert(s);
                sort(v.begin(), v.end(), compare);  // ** 벡터를 삽입하고 compare 함수를 이용해 내림차순으로 정렬

                boxY = 450;
                InvalidateRect(hWnd, NULL, TRUE);   // ** 갱신
            }
            else {                                  // ** 학생 정보가 8개로 가득차면 메시지 출력
                ::MessageBox(0, "벡터가 가득찼습니다.", "Full Vector", MB_OK);
                break;
            }
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        OnPaint(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
    
// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
