#include <windows.h>
#include <time.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void __Line(HDC hdc, int x1, int y1, int x2, int y2, COLORREF crPen);
void __Ellipse(HDC hdc, int x1, int y1, int x2, int y2, COLORREF crPen, COLORREF crBrush);
void __Rectangle(HDC hdc, int x1, int y1, int x2, int y2, COLORREF crPen, COLORREF crBrush);
void RandRect(HDC hdc, int nClientWitdh, int nClientHeight);

//// WinMain �Լ� : ������ ���α׷��� ���� ������ ( Entry Point )
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR	  lpCmdLine,
	int		  nCmdShow)
{
	char	 szAppName[] = "Homework 3";
	HWND	 hWnd;
	MSG      msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	srand((unsigned)time(NULL));

	//// ���� Ŭ���� ���
	RegisterClass(&wndclass);

	//// ���� ����
	hWnd = CreateWindow(szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		100, 100, 250, 200,
		NULL, NULL, hInstance, NULL);

	if (!hWnd) return FALSE;

	//// ������ ������ ȭ�鿡 ǥ��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//// �޽��� ����
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;
	static int cxClient = 0, cyClient = 0;

	switch (message)
	{
	case WM_SIZE:
		//// lParam�� ���� 2����Ʈ : Ŭ���̾�Ʈ ������ �ʺ� 
		cxClient = LOWORD(lParam);
		//// lParam�� ���� 2����Ʈ : Ŭ���̾�Ʈ ������ ���� 
		cyClient = HIWORD(lParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		RandRect(hdc, cxClient, cyClient);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void __Line(HDC hdc, int x1, int y1, int x2, int y2, COLORREF crPen)
{
	HPEN	newPen, oldPen;

	newPen = CreatePen(PS_SOLID, 1, crPen);
	oldPen = (HPEN) SelectObject(hdc, newPen);
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	SelectObject(hdc, oldPen);
	DeleteObject(newPen);
}

void __Ellipse(HDC hdc, int x1, int y1, int x2, int y2, COLORREF crPen, COLORREF crBrush)
{
	HPEN	newPen, oldPen;
	HBRUSH	newBrush, oldBrush;

	newPen = CreatePen(PS_SOLID, 1, crPen);
	oldPen = (HPEN)SelectObject(hdc, newPen);

	newBrush = CreateSolidBrush(crBrush);
	oldBrush = (HBRUSH)SelectObject(hdc, newBrush);

	Ellipse(hdc, x1, y1, x2, y2);

	SelectObject(hdc, oldPen);
	DeleteObject(newPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(newBrush);
}

void __Rectangle(HDC hdc, int x1, int y1, int x2, int y2, COLORREF crPen, COLORREF crBrush)
{
	HPEN	newPen, oldPen;
	HBRUSH	newBrush, oldBrush;

	newPen = CreatePen(PS_SOLID, 1, crPen);
	oldPen = (HPEN)SelectObject(hdc, newPen);

	newBrush = CreateSolidBrush(crBrush);
	oldBrush = (HBRUSH)SelectObject(hdc, newBrush);

	Rectangle(hdc, x1, y1, x2, y2);

	SelectObject(hdc, oldPen);
	DeleteObject(newPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(newBrush);
}

void RandRect(HDC hdc, int nClientWidth, int nClientHeight)
{
	HPEN	newPen, oldPen;
	HBRUSH	newBrush, oldBrush;
	int x1, x2, y1, y2, width, height;
	newPen = CreatePen(PS_SOLID, 1, RGB(rand() % 256, rand() % 256, rand() % 256));
	oldPen = (HPEN)SelectObject(hdc, newPen);

	newBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
	oldBrush = (HBRUSH)SelectObject(hdc, newBrush);

	if (nClientWidth == 0)
		x1 = 0, x2 = 0, width = 0;
	else
		x1 = rand() % nClientWidth, x2 = rand() % nClientWidth;
	if (abs(x1 - x2) / 2 == 0)
		width = 0;
	else
		 width = rand() % (abs(x1 - x2) / 2);

	if (nClientHeight == 0)
		y1 = 0, y2 = 0, height = 0;
	else
		y1 = rand() % nClientHeight, y2 = rand() % rand() % nClientHeight;
	if (abs(y1 - y2) / 2 == 0)
		height = 0;
	else
		 height = rand() % (abs(y1 - y2) / 2);

	RoundRect(hdc, x1, y1, x2, y2, width, height);

	SelectObject(hdc, oldPen);
	DeleteObject(newPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(newBrush);
}