//#ifndef __tableCell__
//#define __tableCell__
#pragma once
#include <windows.h>
//#include "tableCell.cpp"
class tableCell
{
private:
	HWND hWnd; // ���������� �����
	//HWND pHWnd; //� ����� ��� ���
	//HINSTANCE hInstanceApp; // � ����
	RECT rect; // ������������ �����
	

public:

	void setHWnd(HWND value);
	HWND getHWnd();

	RECT getRect();
	void setRect(RECT value);

	void changeEditSize();
	HWND createEdit(HWND hWnd);

	tableCell(RECT rect);
	tableCell();
	~tableCell();
};

//#endif