#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//// ���� �Լ� : ������ ���α׷��� ���� ������ ( Entry Point )
int WINAPI WinMain(HINSTANCE hInstance,		// = handle Instance, ���α׷��� �ν��Ͻ� �ڵ�. ���� ���ϸ� ���α׷��� �ּҰ�.
    HINSTANCE hPrevInstance,	// = handle Previous Instance, "���� ���α׷�"�� �ν��Ͻ� �ڵ�
                                // 16��Ʈ ������ ���� hPrevInstance�� NULL�� �ƴϸ� �̰��� �̹� ���� ���� ���α׷��� ���繰�� ���� �ν��Ͻ� �ڵ�� ���� �� �־���. �׷��� ���� �����͸� �����ϱ� ���ؼ� GetInstanceData�� ����Ͽ� �� â�� �ڵ��� �����ϰ� �� â�� ����� �����Ͽ���. �׸��� �� â�� ����� Ŭ������ ��� ����� �� �־��� ������ �� ��° ������ �ν��Ͻ������� Ŭ���� ����� �����Ͽ���.
                                // �׷��� ����, 32��Ʈ �����쿡���� �� �̻� �̰��� ������� �ʴ´�.Win32 ������ ��� / �ν��Ͻ��� ���õ� ���� ��������� �� ��° ������ �ν��Ͻ����� Ŭ���� ����� �������� �ϸ� �ȵȴ�.��, ��� ���α׷��� ������ �ּ� ������ �����ٴ� �ǹ��̸�.hPrevInstance���� �׻� NULL�� �Ѿ���� ���̴�.
    PSTR szCmdLine,				// ��� ���ο��� ���α׷� ���� �� ������ ���ڿ�
    int iCmdShow)			    // �����쿡 ��µ� ����
{
    static TCHAR szAppName[] = TEXT("HelloWin"); // ���ø����̼� �̸�.
    HWND         hwnd;		// = handle window : ������ �ڵ�. // �ڵ��̶�? ���ҽ��� �޸� �ּҸ� ������ ġȯ�� ���̴�.
    MSG          msg;			// = message : �޽���.
    WNDCLASS     wndclass;	// = window class : ������ â�� ���� �� �ʿ��� ������. �Ʒ����� �����Ѵ�

    wndclass.style = CS_HREDRAW | CS_VREDRAW; // style : CS_HREDRAW : ���� ũ�Ⱑ ���� ��� â�� �ٽ� �׸�.
                                                       //       : CS_VREDRAW : ���� ũ�Ⱑ ���� ��� â�� �ٽ� �׸�.

    wndclass.lpfnWndProc = WndProc;					// �������� �޽��� ó�� �Լ��� �����Ѵ�. ó���Լ��� �̸��� ���� ���� �� ������ ���� WndProc�� ������ �ִ�.
    wndclass.cbClsExtra = 0;                       // �������� ���� ����. ������� ���������� ����ϸ� ���� Ư���� ������ ���Ǵ� ������ �����̴�.   // Class Extra
    wndclass.cbWndExtra = 0;                       // ������ ������ ������� ���� ���� 0���� �����Ѵ�.                                            // Window Extra
    wndclass.hInstance = hInstance;               // �� ������ Ŭ������ ����ϴ� ���α׷��� ��ȣ�̸� �� ���� WinMain�� �μ��� ���޵� hInstance���� �״�� �������ָ� �ȴ�.
    wndclass.hIcon = LoadIcon(NULL,			// �ּ�ȭ�Ǿ��� ��� ��µ� �������� LoadIcon �Լ��� ����Ͽ� �����Ѵ�.
        IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL,         // ����� ���콺 Ŀ���� LoadCursor �Լ��� ����Ͽ� �����Ѵ�.
        IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // �������� ��� ������ �����Ѵ�. 

    wndclass.lpszMenuName = NULL;                    // ���α׷��� ����� �޴��� �����Ѵ�. �޴��� ������� �ʴ� ��� NULL
    wndclass.lpszClassName = szAppName;               // Window Class �̸��� �����Ѵ�. 

    //// ������ Ŭ���� ���
    // CreateWindow()�� �����츦 �����ϱ� ���� �ݵ�� ��ϵǾ� �־�� �Ѵ�
    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), // ��ϵ��� �ʾҴٸ�, â ǥ��.
            szAppName, MB_ICONINFORMATION);
        return 0;
    }

    //// ������ ����
    hwnd = CreateWindow(szAppName,                  // lpClassName    ������ Ŭ������
        TEXT("The Hello Program"), // lpWindowName	������ Ÿ��Ʋ�� // ��ư�̶�� �ؽ�Ʈ
        WS_OVERLAPPEDWINDOW,        // dwStyle        ������ ��Ÿ��
        CW_USEDEFAULT,              // x              ������ ���� X��ǥ
        CW_USEDEFAULT,              // y              ������ ���� Y��ǥ
        CW_USEDEFAULT,              // nWidth         ������ �ʺ�
        CW_USEDEFAULT,              // nHeight        ������ ����
        NULL,                       // hWndParent     �θ��� �ڵ�. �˾� �������� ��� NULL ����.
        NULL,                       // hMenu          �޴��� �ڵ� Ȥ�� �ڽ� �������� ID. // �˾� ������� NULL ����. 
        hInstance,                  // hInstance      �����츦 ������ �ν��Ͻ� �ڵ�
        NULL);                     // lpParam        �������α׷��� MDI Ŭ���̾�Ʈ �����츦 �����ϸ� CLIENTCREATESTRUCT ����ü ����. / MDI Ŭ���̾�Ʈ �����찡 MDI �ڽ� �����츦 �����ϸ� MDICREATESTRUCT ����ü�� ����. ������� ���� ��� NULL ���� ����.

//// ������ �����츦 ȭ�鿡 ǥ��
    ShowWindow(hwnd, iCmdShow);

    UpdateWindow(hwnd);			                    // Ŭ���̾�Ʈ ���� ���� ������Ʈ�� �ڵ� �� ����. (â�� WM_PAINT�� ������ ����)

    //// �޽��� ����
    while (GetMessage(&msg,
        NULL, 0, 0))
    {
        TranslateMessage(&msg);			            // ���޵� �޽����� WM_KEYDOWN����, ������ Ű�� ����Ű���� �˻��غ��� ������ ���� ��� WM_CHAR �޽����� ����� �޽��� ť�� �����δ�.
        DispatchMessage(&msg);			            // ���� �Է��� �ƴ� ��쿡�� �� �Լ����� �ƹ� �ϵ� ���� �ʰ� �� �Լ��� ���� WndProc�� ��������.
    }
    return msg.wParam;                                // wParam return.
}

//// LRESULT == long
//// Window Procedure�� ���α׷����� ȣ���ϴ� ���� �ƴ϶� Windows�� ���� ȣ��ȴ� ( CALLBACK )

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC         hdc;      // Handle Device Context. ��¿� �ʿ��� ������ ������ ������ ����ü (��ǥ, ��, ���� ��) : DC�� �ּҸ� ����Ŵ.
    PAINTSTRUCT ps;		// �ش� ������ ������ â�� Ŭ���̾�Ʈ ������ �׸��� �� ���.
    RECT        rect;     // �簢�� ǥ�� Ŭ����.

    switch (message)
    {
        // �����찡 ǥ�õǱ� ��, �����찡 ���� �ɶ� 
    case WM_CREATE:
        MessageBox(hwnd,						    // �˸�â	
            TEXT("WM_CREATE "),		    // ����� ����	
            TEXT("�޽���"),				    // �ڽ� ����
            0);						    // ���� ����	
        return 0;

        // ȭ���� ��ȿȭ ( ���� )
    case WM_PAINT:                                     // Paint���.
        hdc = BeginPaint(hwnd, &ps);			    // hwnd : �ٽ� ĥ�� â�� ���� �ڵ�, ps : PAINTSTRUCT ����ü�� ���� ������.	
                                                    // 1. �׸��⸦ ���� DC �ڵ��� ���Ѵ�. �� �ڵ��� PAINTSTRUCT ����ü�� hdc������� ���ԵǸ� BeginPaint �Լ��� ���ϰ����� �������⵵ �Ѵ�.
                                                    // 2. �׸��⿡ �ʿ��� ������ PAINTSTRUCT ����ü�� ä���. �� ����ü���� �ּ����� ��ȿ������ ��� ������ ���� ����, DC �ڵ���� ��ϵǾ� ������ ���� ���α׷��� �� ������ �����Ͽ� �׸��⸦ �� ������ �� �� �ִ�.
                                                    // 3. Ŭ���� ������ �����Ͽ� DC�� �����Ͽ� Ŭ���� ���� �ٱ����� ��µ��� �ʵ��� �Ѵ�.
                                                    // 4. ��ȿ ������ ��ȿȭ�Ͽ� �ٽ� ��ȿ ������ ��������� WM_PAINT �޽����� ȣ����� �ʵ��� �Ѵ�.
                                                    // 5. ĳ���� ������ ĳ���� ���� �ı����� �ʵ��� �Ѵ�. (Ŀ�� = ĳ��)
                                                    // 6. ��ȿ ������ ����� ���쵵�� ��û�ϸ� WM_ERASEBKGND �޽����� ���� ����� ���쵵�� �Ѵ�. ���� WM_NCPAINT �޽����� ���� ���۾� ������ �׸����� �Ѵ�.

        GetClientRect(hwnd, &rect);				// ������ Ŭ���̾�Ʈ ���������� ��ǥ�� ��ȯ.

        DrawText(hdc,                              // Handle Device Context
            TEXT("Hello, Windows!"),         // �׸� �ؽ�Ʈ ����.
            -1,								// ���ڿ��� ����. -1�� �����ٸ� null�� ������ ���ڿ��� ���� �����ͷ� �����ϰ� ���� ���� �ڵ����� ���.
            &rect,                            // RECT ����ü ������.
            DT_SINGLELINE | DT_CENTER | DT_VCENTER); // �ؽ�Ʈ ���� ���� �޼���.

        EndPaint(hwnd, &ps);                      // �׸��� ��.
        return 0;

        // ������ ���� �õ� ( �����ư�� )
    case WM_DESTROY:		                            // Destroy���.
        PostQuitMessage(0);						// nExitCode : ���ø����̼� ���� �ڵ�. WM_QUIT �޽����� wParam �Ű� ������ ����.
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);	    // ó������ ���� �޽����� ����Ʈ ó��. 
}