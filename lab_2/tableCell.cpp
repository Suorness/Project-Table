#include "tableCell.h"

HWND tableCell::createEdit(HWND hWnd)
{
	HWND hwndEdit;
	// WS_CHILD - �������� ����
	// WS_CHILD - ������������� �������
	// WS_BORDER - ����� 
	// ES_LEFT - ������������ ������ �����.
	// ES_MULTILINE - �������������� 

	hwndEdit = CreateWindowEx(
		0,
		L"edit",
		NULL,
		WS_CHILD | WS_VISIBLE |
		/*WS_BORDER |*/	ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		rect.left+1, rect.top+1, rect.right- rect.left-1,rect.bottom- rect.top-1,
		hWnd,
		NULL,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL); 
	return  hwndEdit;
}

void tableCell::setHWnd(HWND value){	hWnd = value;}
HWND tableCell::getHWnd(){	return hWnd;}

void tableCell::setRect(RECT value) {	rect = value;}

RECT tableCell::getRect(){	return rect; }

tableCell::tableCell(RECT rect)
{
	setRect(rect);
}

tableCell::tableCell(){}
tableCell::~tableCell(){}

void tableCell::changeEditSize()
{
	MoveWindow(getHWnd(), rect.left+1, rect.top+1, rect.right-rect.left-1, rect.bottom-rect.top-1, TRUE);
}