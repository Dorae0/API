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

    hwnd = CreateWindow(szAppName,                  // ������ Ŭ���� �̸�
        "Win32-����",               // ������ �μ�
        WS_OVERLAPPEDWINDOW,        // ������ ���
        CW_USEDEFAULT,              // X �����ġ
        CW_USEDEFAULT,              // Y �����ġ
        300,						  // ������ ����
        300,						  // ������ ����
        NULL,                       // �θ� ������ �ڵ�
        NULL,                       // ������ �޴� �ڵ�
        hInstance,                  // �ν���Ʈ �ڵ�
        NULL);                     // ������ ����

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
    HDC			hdc;	// DC�� ���� �ڵ�
    PAINTSTRUCT ps;
    char* szText = "������ ���ڿ� ���1";
    static int	cxChar = 0, cyChar = 0, nHscrollPos = 0, nVscrollPos = 0, cxClient = 0, cyClient = 0;
    int maxnum = 0, tempnum = 0;
    char		strBuff[255];
    int			x, y, i;
    TEXTMETRIC  tm;

    switch (message)
    {
    case WM_CREATE:

        //// ��Ʈ ũ��
        hdc = GetDC(hwnd);
        GetTextMetrics(hdc, &tm);
        cyChar = tm.tmHeight;
        cxChar = tm.tmMaxCharWidth;
        ReleaseDC(hwnd, hdc);
        maxnum = 32;

        //// ��ũ�� ����
        SetScrollRange(hwnd,			//	������ �ڵ� or ��Ʈ�� �ڵ� ( SB_CTL )
            SB_VERT,		//	����
            0,			//	�ּ�
            NUMLINES - 1,	//	�ִ�
            FALSE);		//	redraw ����
        SetScrollRange(hwnd,			//	������ �ڵ� or ��Ʈ�� �ڵ� ( SB_CTL )
            SB_HORZ,		//	����
            0,			//	�ּ�
            XNUM - 1,	//	�ִ�
            FALSE);		//	redraw ����
        SetScrollPos(hwnd,
            SB_VERT,
            nVscrollPos,	// thumb ��ġ
            FALSE);		// redraw ����
        SetScrollPos(hwnd,
            SB_HORZ,
            nHscrollPos,	// thumb ��ġ
            FALSE);		// redraw ����
        return 0;

    case WM_SIZE:
        //// lParam�� ���� 2����Ʈ : Ŭ���̾�Ʈ ������ �ʺ� 
        cxClient = LOWORD(lParam);
        //// lParam�� ���� 2����Ʈ : Ŭ���̾�Ʈ ������ ���� 
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
        //// wParam�� ���� 2����Ʈ : ������ư
        ////          ���� 2����Ʈ : thumb�� ��ġ
        //// lParam : ������ �ڵ�
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

        //// ���ŵ� �����ǰ� ���� �����ǰ��� ����ġ ����
        // nVscrollPos�� ���� ��ũ�������ǿ� �ݿ����� �ʾҴ�
        if (nVscrollPos != GetScrollPos(hwnd, SB_VERT))
        {
            //// ������ �缳��
            SetScrollPos(hwnd, SB_VERT, nVscrollPos, TRUE);
            //// ��ȿȭ
            InvalidateRect(hwnd,
                NULL,	// NULL�̸� ��ü Ŭ���̾�Ʈ ����
                TRUE);	// ��ȿ������ ������ ����							   
        }

        return 0;

    case WM_HSCROLL:
        //// wParam�� ���� 2����Ʈ : ������ư
        ////          ���� 2����Ʈ : thumb�� ��ġ
        //// lParam : ������ �ڵ�
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
        //// ���ŵ� �����ǰ� ���� �����ǰ��� ����ġ ����
        // nHscrollPos�� ���� ��ũ�������ǿ� �ݿ����� �ʾҴ�
        if (nHscrollPos != GetScrollPos(hwnd, SB_HORZ))
        {
            //// ������ �缳��
            SetScrollPos(hwnd, SB_HORZ, nHscrollPos, TRUE);
            //// ��ȿȭ
            InvalidateRect(hwnd,
                NULL,	// NULL�̸� ��ü Ŭ���̾�Ʈ ����
                TRUE);	// ��ȿ������ ������ ����							   
        }

        return 0;

    case WM_PAINT:
        
        SetScrollRange(hwnd,			//	������ �ڵ� or ��Ʈ�� �ڵ� ( SB_CTL )
            SB_VERT,		//	����
            0,			//	�ּ�
            NUMLINES - cyClient / cyChar,	//	�ִ�
            FALSE);		//	redraw ����
        SetScrollRange(hwnd,			//	������ �ڵ� or ��Ʈ�� �ڵ� ( SB_CTL )
            SB_HORZ,		//	����
            0,			//	�ּ�
            XNUM - cxClient / cxChar,	//	�ִ�
            TRUE);		//	redraw ����

        XNUM = 0;

        //// DC �ڵ��� ���, ��ü Ŭ���̾�Ʈ ������ ��ȿȭ�ȴ�
        //// ��ȿ ������ ��ȿȭ�Ͽ� �ٽ� WM_PAINT �޽����� ȣ����� �ʵ��� �Ѵ�
        hdc = BeginPaint(hwnd, &ps);
        {
            for (i = 0; i < NUMLINES; i++)
            {
                x = cxChar * (0 - nHscrollPos);
                y = cyChar * (i - nVscrollPos);

                //// ���������� ���ڿ��� �����				
                wsprintf(strBuff, "Scrollbar %2d, VSPos %2d, y %3d", i, nVscrollPos, y);
                TextOut(hdc, x, y, strBuff, strlen(strBuff));

                tagSIZE tsize = { 0, 0 };
                LPSIZE size = { &tsize };
                GetTextExtentPointA(hdc, strBuff, strlen(strBuff), size);
                tempnum = floor(size->cx / cxChar + 1);
                XNUM = max(XNUM, tempnum);
            }
        }

        //// DC �ڵ� ����
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
    // DC �ڵ� ���.
    HDC hdc = GetDC(hwnd);
    // ��ȿȭ (�ٽ� WM_PAINT �޽��� ȣ�� �ȵǵ���)
    ValidateRect(hwnd, &(lpPaint->rcPaint));
    // HDC return
    return hdc;
}