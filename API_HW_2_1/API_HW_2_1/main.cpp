#include <windows.h>
#include <cmath>

#define NUMLINES	30
int XNUM = 0;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HDC __BeginPaint(HWND hwnd, LPPAINTSTRUCT lpPaint);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static CHAR szAppName[] = "GP";
    HWND         hwnd;
    MSG          msg;
    WNDCLASS     wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    RegisterClass(&wndclass);

    hwnd = CreateWindow(szAppName,                  // 윈도우 클래스 이름
        "Win32-예제",               // 윈도우 켑션
        WS_OVERLAPPEDWINDOW,        // 윈도우 모양
        CW_USEDEFAULT,              // X 출력위치
        CW_USEDEFAULT,              // Y 출력위치
        300,						  // 윈도우 넓이
        300,						  // 윈도우 높이
        NULL,                       // 부모 윈도우 핸들
        NULL,                       // 윈도우 메뉴 핸들
        hInstance,                  // 인스턴트 핸들
        NULL);                     // 사용되지 않음

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC			hdc;	// DC에 대한 핸들
    PAINTSTRUCT ps;
    char* szText = "윈도우 문자열 출력1";
    static int	cxChar = 0, cyChar = 0, nHscrollPos = 0, nVscrollPos = 0, cxClient = 0, cyClient = 0;
    int maxnum = 0, tempnum = 0;
    char		strBuff[255];
    int			x, y, i;
    TEXTMETRIC  tm;

    switch (message)
    {
    case WM_CREATE:

        //// 폰트 크기
        hdc = GetDC(hwnd);
        GetTextMetrics(hdc, &tm);
        cyChar = tm.tmHeight;
        cxChar = tm.tmMaxCharWidth;
        ReleaseDC(hwnd, hdc);
        maxnum = 32;

        //// 스크롤 설정
        SetScrollRange(hwnd,			//	윈도우 핸들 or 콘트롤 핸들 ( SB_CTL )
            SB_VERT,		//	수직
            0,			//	최소
            NUMLINES - 1,	//	최대
            FALSE);		//	redraw 여부
        SetScrollRange(hwnd,			//	윈도우 핸들 or 콘트롤 핸들 ( SB_CTL )
            SB_HORZ,		//	수평
            0,			//	최소
            XNUM - 1,	//	최대
            FALSE);		//	redraw 여부
        SetScrollPos(hwnd,
            SB_VERT,
            nVscrollPos,	// thumb 위치
            FALSE);		// redraw 여부
        SetScrollPos(hwnd,
            SB_HORZ,
            nHscrollPos,	// thumb 위치
            FALSE);		// redraw 여부
        return 0;

    case WM_SIZE:
        //// lParam의 하위 2바이트 : 클라이언트 영역의 너비 
        cxClient = LOWORD(lParam);
        //// lParam의 상위 2바이트 : 클라이언트 영역의 높이 
        cyClient = HIWORD(lParam);

        if (nVscrollPos != 0 && cyClient / cyChar > NUMLINES - nVscrollPos)
            nVscrollPos = max(0, NUMLINES - cyClient / cyChar);
        if (nHscrollPos != 0 && cxClient / cxChar > XNUM - nHscrollPos)
            nHscrollPos = max(0, XNUM - cxClient / cxChar);

        if (cyClient > NUMLINES * cyChar - 1)
            ShowScrollBar(hwnd, SB_VERT, FALSE);
        else if (cyClient == NUMLINES * cyChar - 1)
            ShowScrollBar(hwnd, SB_VERT, TRUE);
        if (cxClient > XNUM * cxChar - 1)
            ShowScrollBar(hwnd, SB_HORZ, FALSE);
        else if (cxClient == XNUM * cxChar - 1)
            ShowScrollBar(hwnd, SB_HORZ, TRUE);

        return 0;

    case WM_VSCROLL:
        //// wParam의 하위 2바이트 : 눌린버튼
        ////          상위 2바이트 : thumb의 위치
        //// lParam : 윈도우 핸들
        switch (LOWORD(wParam))
            //switch ((WORD)wParam)
        {
        case SB_LINEUP:
            nVscrollPos -= 1;
            break;

        case SB_LINEDOWN:
            nVscrollPos += 1;
            break;

        case SB_PAGEUP:
            nVscrollPos -= cyClient / cyChar;
            break;

        case SB_PAGEDOWN:
            nVscrollPos += cyClient / cyChar;
            break;

        case SB_THUMBTRACK:
            nVscrollPos = HIWORD(wParam);
            //nVscrollPos = ((wParam & 0xFFFF0000) >> 16);
            break;

        default:
            break;
        }

        nVscrollPos = max(0, min(nVscrollPos, NUMLINES - cyClient / cyChar));
        if (NUMLINES - nVscrollPos < cyClient / cyChar)
            nVscrollPos = min(NUMLINES , NUMLINES - cyClient / cyChar);

        
        //if (nVscrollPos < 0)			nVscrollPos = 0;
        //if (nVscrollPos > NUMLINES-1) nVscrollPos = NUMLINES-1;

        //// 갱신될 포지션과 현재 포지션과의 불일치 여부
        // nVscrollPos는 아직 스크롤포지션에 반영되지 않았다
        if (nVscrollPos != GetScrollPos(hwnd, SB_VERT))
        {
            //// 포지션 재설정
            SetScrollPos(hwnd, SB_VERT, nVscrollPos, TRUE);
            //// 무효화
            InvalidateRect(hwnd,
                NULL,	// NULL이면 전체 클라이언트 영역
                TRUE);	// 무효영역을 지울지 여부							   
        }

        return 0;

    case WM_HSCROLL:
        //// wParam의 하위 2바이트 : 눌린버튼
        ////          상위 2바이트 : thumb의 위치
        //// lParam : 윈도우 핸들
        switch (LOWORD(wParam))
        {

        case SB_LINELEFT:
            nHscrollPos -= 1;
            break;

        case SB_LINERIGHT:
            nHscrollPos += 1;
            break;

        case SB_PAGELEFT:
            nHscrollPos -= cxClient / cxChar;
            break;

        case SB_PAGERIGHT:
            nHscrollPos += cxClient / cxChar;
            break;

        case SB_THUMBTRACK:
            nHscrollPos = HIWORD(wParam);
            break;

        default:
            break;
        }

        nHscrollPos = max(0, min(nHscrollPos, XNUM - cxClient / cxChar));
        if (XNUM - nHscrollPos < cxClient / cxChar)
            nHscrollPos = min(XNUM, XNUM - cxClient / cxChar);
        //// 갱신될 포지션과 현재 포지션과의 불일치 여부
        // nHscrollPos는 아직 스크롤포지션에 반영되지 않았다
        if (nHscrollPos != GetScrollPos(hwnd, SB_HORZ))
        {
            //// 포지션 재설정
            SetScrollPos(hwnd, SB_HORZ, nHscrollPos, TRUE);
            //// 무효화
            InvalidateRect(hwnd,
                NULL,	// NULL이면 전체 클라이언트 영역
                TRUE);	// 무효영역을 지울지 여부							   
        }

        return 0;

    case WM_PAINT:
        
        SetScrollRange(hwnd,			//	윈도우 핸들 or 콘트롤 핸들 ( SB_CTL )
            SB_VERT,		//	수직
            0,			//	최소
            NUMLINES - cyClient / cyChar,	//	최대
            FALSE);		//	redraw 여부
        SetScrollRange(hwnd,			//	윈도우 핸들 or 콘트롤 핸들 ( SB_CTL )
            SB_HORZ,		//	수평
            0,			//	최소
            XNUM - cxClient / cxChar,	//	최대
            TRUE);		//	redraw 여부

        XNUM = 0;

        //// DC 핸들을 얻고, 전체 클라이언트 영역이 유효화된다
        //// 무효 영역을 유효화하여 다시 WM_PAINT 메시지가 호출되지 않도록 한다
        hdc = BeginPaint(hwnd, &ps);
        {
            for (i = 0; i < NUMLINES; i++)
            {
                x = cxChar * (0 - nHscrollPos);
                y = cyChar * (i - nVscrollPos);

                //// 서식형으로 문자열을 만든다				
                wsprintf(strBuff, "Scrollbar %2d, VSPos %2d, y %3d", i, nVscrollPos, y);
                TextOut(hdc, x, y, strBuff, strlen(strBuff));

                tagSIZE tsize = { 0, 0 };
                LPSIZE size = { &tsize };
                GetTextExtentPointA(hdc, strBuff, strlen(strBuff), size);
                tempnum = floor(size->cx / cxChar + 1);
                XNUM = max(XNUM, tempnum);
            }
        }

        //// DC 핸들 해제
        EndPaint(hwnd, &ps);
        return 0;


    case WM_DESTROY:
        PostQuitMessage(WM_QUIT);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

HDC __BeginPaint(HWND hwnd, LPPAINTSTRUCT lpPaint)
{
    // DC 핸들 얻기.
    HDC hdc = GetDC(hwnd);
    // 유효화 (다시 WM_PAINT 메시지 호출 안되도록)
    ValidateRect(hwnd, &(lpPaint->rcPaint));
    // HDC return
    return hdc;
}