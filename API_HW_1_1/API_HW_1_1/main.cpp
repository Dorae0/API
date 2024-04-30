#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//// 메인 함수 : 윈도우 프로그램의 최초 진입점 ( Entry Point )
int WINAPI WinMain(HINSTANCE hInstance,		// = handle Instance, 프로그램의 인스턴스 핸들. 쉽게 말하면 프로그램의 주소값.
    HINSTANCE hPrevInstance,	// = handle Previous Instance, "이전 프로그램"의 인스턴스 핸들
                                // 16비트 윈도우 시절 hPrevInstance가 NULL이 아니면 이것은 이미 실행 중인 프로그램의 복사물에 대한 인스턴스 핸들로 사용될 수 있었다. 그래서 실제 데이터를 복사하기 위해선 GetInstanceData를 사용하여 주 창의 핸들을 복사하고 주 창과 통신이 가능하였다. 그리고 주 창이 등록한 클래스를 계속 사용할 수 있었기 때문에 두 번째 이후의 인스턴스에서는 클래스 등록을 생략하였다.
                                // 그러나 이제, 32비트 윈도우에서는 더 이상 이것을 사용하지 않는다.Win32 에서는 모듈 / 인스턴스와 관련된 것은 사라졌으며 두 번째 이후의 인스턴스에서 클래스 등록의 생략따윈 하면 안된다.즉, 모든 프로그램은 별도의 주소 공간을 가진다는 의미이며.hPrevInstance에는 항상 NULL이 넘어오는 것이다.
    PSTR szCmdLine,				// 명령 라인에서 프로그램 구동 시 전달할 문자열
    int iCmdShow)			    // 윈도우에 출력될 형태
{
    static TCHAR szAppName[] = TEXT("HelloWin"); // 어플리케이션 이름.
    HWND         hwnd;		// = handle window : 윈도우 핸들. // 핸들이란? 리소스의 메모리 주소를 정수로 치환한 값이다.
    MSG          msg;			// = message : 메시지.
    WNDCLASS     wndclass;	// = window class : 윈도우 창을 만들 때 필요한 정보들. 아래에서 선언한다

    wndclass.style = CS_HREDRAW | CS_VREDRAW; // style : CS_HREDRAW : 수직 크기가 변할 경우 창을 다시 그림.
                                                       //       : CS_VREDRAW : 수평 크기가 변할 경우 창을 다시 그림.

    wndclass.lpfnWndProc = WndProc;					// 윈도우의 메시지 처리 함수를 지정한다. 처리함수의 이름은 물론 정할 수 있지만 보통 WndProc로 정해져 있다.
    wndclass.cbClsExtra = 0;                       // ┌일종의 예약 영역. 윈도우즈가 내부적으로 사용하며 아주 특수한 목적에 사용되는 여분의 공간이다.   // Class Extra
    wndclass.cbWndExtra = 0;                       // └예약 영역을 사용하지 않을 경우는 0으로 지정한다.                                            // Window Extra
    wndclass.hInstance = hInstance;               // 이 윈도우 클래스를 사용하는 프로그램의 번호이며 이 값은 WinMain의 인수로 전달된 hInstance값을 그대로 대입해주면 된다.
    wndclass.hIcon = LoadIcon(NULL,			// 최소화되었을 경우 출력될 아이콘을 LoadIcon 함수를 사용하여 지정한다.
        IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL,         // 사용할 마우스 커서를 LoadCursor 함수를 사용하여 지정한다.
        IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 윈도우의 배경 색상을 지정한다. 

    wndclass.lpszMenuName = NULL;                    // 프로그램이 사용할 메뉴를 지정한다. 메뉴를 사용하지 않는 경우 NULL
    wndclass.lpszClassName = szAppName;               // Window Class 이름을 정의한다. 

    //// 윈도우 클래스 등록
    // CreateWindow()로 윈도우를 생성하기 전에 반드시 등록되어 있어야 한다
    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), // 등록되지 않았다면, 창 표시.
            szAppName, MB_ICONINFORMATION);
        return 0;
    }

    //// 윈도우 생성
    hwnd = CreateWindow(szAppName,                  // lpClassName    윈도우 클래스명
        TEXT("The Hello Program"), // lpWindowName	윈도우 타이틀바 // 버튼이라면 텍스트
        WS_OVERLAPPEDWINDOW,        // dwStyle        윈도우 스타일
        CW_USEDEFAULT,              // x              윈도우 생성 X좌표
        CW_USEDEFAULT,              // y              윈도우 생성 Y좌표
        CW_USEDEFAULT,              // nWidth         윈도우 너비
        CW_USEDEFAULT,              // nHeight        윈도우 높이
        NULL,                       // hWndParent     부모의 핸들. 팝업 윈도우의 경우 NULL 가능.
        NULL,                       // hMenu          메뉴의 핸들 혹은 자식 윈도우의 ID. // 팝업 윈도우는 NULL 가능. 
        hInstance,                  // hInstance      윈도우를 생성할 인스턴스 핸들
        NULL);                     // lpParam        응용프로그램이 MDI 클라이언트 윈도우를 생성하면 CLIENTCREATESTRUCT 구조체 지정. / MDI 클라이언트 윈도우가 MDI 자식 윈도우를 생성하면 MDICREATESTRUCT 구조체를 지정. 사용하지 않을 경우 NULL 값을 지정.

//// 생성된 윈도우를 화면에 표시
    ShowWindow(hwnd, iCmdShow);

    UpdateWindow(hwnd);			                    // 클라이언트 영역 값을 업데이트할 핸들 값 지정. (창에 WM_PAINT를 보내서 수정)

    //// 메시지 루프
    while (GetMessage(&msg,
        NULL, 0, 0))
    {
        TranslateMessage(&msg);			            // 전달된 메시지가 WM_KEYDOWN인지, 눌려진 키가 문자키인지 검사해보고 조건이 맞을 경우 WM_CHAR 메시지를 만들어 메시지 큐에 덧붙인다.
        DispatchMessage(&msg);			            // 문자 입력이 아닐 경우에는 위 함수에서 아무 일도 하지 않고 이 함수에 의해 WndProc로 보내진다.
    }
    return msg.wParam;                                // wParam return.
}

//// LRESULT == long
//// Window Procedure는 프로그램에서 호출하는 것이 아니라 Windows로 부터 호출된다 ( CALLBACK )

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC         hdc;      // Handle Device Context. 출력에 필요한 정보를 가지는 데이터 구조체 (좌표, 색, 굵기 등) : DC의 주소를 가리킴.
    PAINTSTRUCT ps;		// 해당 어플이 소유한 창의 클라이언트 영역을 그리는 데 사용.
    RECT        rect;     // 사각형 표현 클래스.

    switch (message)
    {
        // 윈도우가 표시되기 전, 윈도우가 생성 될때 
    case WM_CREATE:
        MessageBox(hwnd,						    // 알림창	
            TEXT("WM_CREATE "),		    // 출력할 내용	
            TEXT("메시지"),				    // 박스 제목
            0);						    // 유형 지정	
        return 0;

        // 화면이 무효화 ( 갱신 )
    case WM_PAINT:                                     // Paint라면.
        hdc = BeginPaint(hwnd, &ps);			    // hwnd : 다시 칠할 창에 대한 핸들, ps : PAINTSTRUCT 구조체에 대한 포인터.	
                                                    // 1. 그리기를 위해 DC 핸들을 구한다. 이 핸들은 PAINTSTRUCT 구조체의 hdc멤버에도 대입되며 BeginPaint 함수의 리턴값으로 돌려지기도 한다.
                                                    // 2. 그리기에 필요한 정보를 PAINTSTRUCT 구조체에 채운다. 이 구조체에는 최소한의 무효영역과 배경 삭제에 대한 정보, DC 핸들등이 기록되어 있으며 응용 프로그램은 이 정보를 참조하여 그리기를 더 빠르게 할 수 있다.
                                                    // 3. 클리핑 영역을 조사하여 DC에 설정하여 클리핑 영역 바깥으로 출력되지 않도록 한다.
                                                    // 4. 무효 영역을 유효화하여 다시 무효 영역이 생기기전에 WM_PAINT 메시지가 호출되지 않도록 한다.
                                                    // 5. 캐럿이 있으면 캐럿을 숨겨 파괴되지 않도록 한다. (커서 = 캐럿)
                                                    // 6. 무효 영역이 배경을 지우도록 요청하면 WM_ERASEBKGND 메시지를 보내 배경을 지우도록 한다. 또한 WM_NCPAINT 메시지를 보내 비작업 영역을 그리도록 한다.

        GetClientRect(hwnd, &rect);				// 윈도우 클라이언트 영역에서의 좌표값 반환.

        DrawText(hdc,                              // Handle Device Context
            TEXT("Hello, Windows!"),         // 그릴 텍스트 지정.
            -1,								// 문자열의 길이. -1로 끝난다면 null로 끝나는 문자열에 대한 포인터로 간주하고 문자 수를 자동으로 계산.
            &rect,                            // RECT 구조체 포인터.
            DT_SINGLELINE | DT_CENTER | DT_VCENTER); // 텍스트 서식 지정 메서드.

        EndPaint(hwnd, &ps);                      // 그리기 끝.
        return 0;

        // 윈도우 종료 시도 ( 종료버튼등 )
    case WM_DESTROY:		                            // Destroy라면.
        PostQuitMessage(0);						// nExitCode : 애플리케이션 종료 코드. WM_QUIT 메시지의 wParam 매개 변수로 사용됨.
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);	    // 처리하지 않은 메시지의 디폴트 처리. 
}