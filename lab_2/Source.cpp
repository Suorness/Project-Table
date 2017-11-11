#include <windows.h>
#include <string>
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	HWND hMainWnd;
	wchar_t szClassName[] = L"My Class";
	MSG msg;
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Cannot register class", L"Error", MB_OK);
		return 0;
	}

	hMainWnd = CreateWindow(
		szClassName, L"Lab_2", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
		(HWND)NULL, (HMENU)NULL,
		(HINSTANCE)hInstance, NULL
	);
	if (!hMainWnd)
	{
		MessageBox(NULL, L"Cannot create main window", L"Error", MB_OK);
		return 0;
	}

	ShowWindow(hMainWnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0)) //выход при получении сообщения о выходе
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	static string text;
	string symb;

	static int cxChar, cyChar, cxClient, cyClient;
	static int nCharPerLine, nClientLines;
	static int xCaret = 0, yCaret = 0;
	int curIndex;
	int nLines;

	int nTailChar;

	int x, y, i;
	switch (uMsg)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		SelectObject(hDC, GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hDC, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight;
		ReleaseDC(hWnd, hDC);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		nCharPerLine = max(1, cxClient / cxChar);
		nClientLines = max(1, cyClient / cyChar);
		if (hWnd == GetFocus())
			SetCaretPos(xCaret * cxChar, yCaret * cyChar);
		break;
	case WM_SETFOCUS:
		CreateCaret(hWnd, NULL, 0, cyChar);
		SetCaretPos(xCaret * cxChar, yCaret * cyChar);
		ShowCaret(hWnd);
		break;
	case WM_KILLFOCUS:
		HideCaret(hWnd);
		DestroyCaret();
		break;

	case WM_KEYDOWN:
		nLines = text.size() / nCharPerLine;
		nTailChar = text.size() % nCharPerLine;
		switch (wParam)
		{
		case VK_HOME: xCaret = 0;
			break;

		case VK_END: if (yCaret == nLines)
			xCaret = nTailChar;
					 else
						 xCaret = nCharPerLine - 1;
			break;
		case VK_PRIOR: yCaret = 0;
			break;
		case VK_NEXT: yCaret = nLines;
			xCaret = nTailChar;
			break;
		case VK_LEFT: xCaret = max(xCaret - 1, 0);
			break;
		case VK_RIGHT: xCaret = min(xCaret + 1, nCharPerLine - 1);
			if ((yCaret == nLines) && (xCaret > nTailChar))
				xCaret = nTailChar;
			break;
		case VK_UP: yCaret = max(yCaret - 1, 0);
			break;
		case VK_DOWN: yCaret = min(yCaret + 1, nLines);
			if ((yCaret == nLines) && (xCaret > nTailChar))
				xCaret = nTailChar;
			break;
		case VK_DELETE: curIndex = yCaret * nCharPerLine + xCaret;
			if (curIndex < text.size()) {
				text.erase(curIndex, 1);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		SetCaretPos(xCaret * cxChar, yCaret * cyChar);
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case '\b': // 'backspace'
			if (xCaret > 0)
			{
				xCaret--;
				SendMessage(hWnd, WM_KEYDOWN, VK_DELETE, 1);
			}
			break;
		case '\t': // 'tab'
			do { SendMessage(hWnd, WM_CHAR, ' ', 1L); } while (xCaret % 8 != 0);
			break;
		case '\r': case '\n':
			for (i = 0; i < nCharPerLine - xCaret; ++i)
				text += ' ';
			xCaret = 0;
			if (++yCaret == nClientLines) {
				MessageBox(hWnd, L"Нет места на окне", L"Error", MB_OK);
				yCaret--;
			}
			break;
		default: // ëþáîé äðóãîé ñèìâîë
			curIndex = yCaret * nCharPerLine + xCaret;
			if (curIndex == text.size())
				text += (char)wParam;
			else {
				symb = (char)wParam;
				text.insert(curIndex, symb);
			}
			InvalidateRect(hWnd, NULL, TRUE);
			if (++xCaret == nCharPerLine) {
				xCaret = 0;
				if (++yCaret == nClientLines) {
					MessageBox(hWnd, L"Íåò ìåñòà â îêíå", L"Error", MB_OK);
					yCaret--;
				}
			}
			break;
		}
		SetCaretPos(xCaret * cxChar, yCaret * cyChar);

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		SelectObject(hDC, GetStockObject(SYSTEM_FIXED_FONT));
		if (text.size()) {
			nLines = text.size() / nCharPerLine;
			nTailChar = text.size() % nCharPerLine;
			//for (y = 0; y < nLines; ++y)
			//	TextOut(hDC, 0, y*cyChar, text.c_str() + y*nCharPerLine,
			//		nCharPerLine);
			//TextOut(hDC, 0, y*cyChar, text.c_str() + y*nCharPerLine,
			//	nTailChar);
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}
