#include <windows.h>
//#include "tableCell.h"
#include "table.h"
#include "resource3.h"
#include <tchar.h>

#define MAX_DIGITS 2 
#define	RELEASE 2

table* mytable;


int GetNumberFromWindow(HWND hWnd);


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK TableSettingsDialogProcedure(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	HWND hMainWnd;
	wchar_t szClassName[] = L"My Class";
	MSG msg;
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW ;
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
		szClassName, L"Lab_2", WS_OVERLAPPEDWINDOW ,
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

	HDC          hdcMem;
	HBITMAP      hbmMem;
	HANDLE       hOld;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		mytable = new table();
#ifdef RELEASE
		{
			// вызов диалогового окна
			if (DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TABLE_PROPERTY), hWnd, (DLGPROC)TableSettingsDialogProcedure))
			{
				if ((mytable->getCountColum() == 0) || (mytable->getCountRow() == 0))
				{
					MessageBox(NULL, _TEXT("Количество столбцов и строк должно быть больше чем нуль"), _TEXT("Ошибка ввода"), MB_OK);
					SendMessage(hWnd, WM_CLOSE, NULL, NULL);
				}
			}
			else
			{
				SendMessage(hWnd, WM_CLOSE, NULL, NULL);
			}
		}
#endif // REALEASE
#ifndef RELEASE

		mytable->setCountColum(10);
		mytable->setCountRow(10);
#endif
		hDC = GetDC(hWnd);
		GetTextMetrics(hDC, &tm);
		ReleaseDC(hWnd, hDC);
		mytable->setHeightChar(tm.tmHeight + tm.tmOverhang+tm.tmExternalLeading +3);

		mytable->setScreenClientRectangle(hWnd);
		mytable->calculatingCellSizes();
		mytable->createCells();
		mytable->drawCells(hWnd);
		break;
	}
	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &ps);

		//hdcMem = CreateCompatibleDC(hDC);
		//hbmMem = CreateCompatibleBitmap(hDC, ps.rcPaint.right- ps.rcPaint.left, ps.rcPaint.bottom- ps.rcPaint.top);
		//hOld = SelectObject(hdcMem, hbmMem);

		//FillRect(hdcMem, &ps.rcPaint, (HBRUSH)GetStockObject(WHITE_BRUSH));
		mytable->drawBorder(hDC, ps.rcPaint);

		//BitBlt(hDC, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, hdcMem, 0, 0, SRCCOPY);
		//SelectObject(hdcMem, hOld);
		//DeleteObject(hbmMem);
		//DeleteDC(hdcMem);

		//SelectObject(hDC, GetStockObject(SYSTEM_FIXED_FONT));
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
			mytable->changeInTable(hWnd);
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		break;
	case WM_SIZING:
	{
		mytable->setScreenClientRectangle(hWnd);
		mytable->calculatingCellSizes();
		mytable->changeInTable(hWnd);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

BOOL CALLBACK TableSettingsDialogProcedure(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SetFocus(GetDlgItem(hDlg, IDC_EDIT_ROW));
		SendMessage(GetDlgItem(hDlg, IDC_EDIT_COLUMN), EM_SETLIMITTEXT, MAX_DIGITS, NULL);
		SendMessage(GetDlgItem(hDlg, IDC_EDIT_ROW), EM_SETLIMITTEXT, MAX_DIGITS, NULL);
		return FALSE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_OK:
		{
			mytable->setCountRow(GetNumberFromWindow(GetDlgItem(hDlg, IDC_EDIT_ROW)));
			mytable->setCountColum(GetNumberFromWindow(GetDlgItem(hDlg, IDC_EDIT_COLUMN)));
			EndDialog(hDlg, TRUE);
			return TRUE;
		}
		case IDC_BUTTON_CANCEL:
		{
			EndDialog(hDlg, FALSE);
			return TRUE;
		}
		}
		break;
	}
	return FALSE;
}

int GetNumberFromWindow(HWND hWnd)
{
	TCHAR numberStr[MAX_DIGITS + 1] = { 0 };
	GetWindowText(hWnd, numberStr, sizeof(numberStr) / sizeof(TCHAR));
	return _ttoi(numberStr);
}
