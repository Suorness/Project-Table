#pragma once
#include <windows.h>
#include "tableCell.h"
#include <vector>
class table
{
private:
	std::vector<tableCell*> vecElements;
	int cellWidth;
	int cellHeight;
	int countRow;
	int countColumn;
	int screenClientWidth;
	int screenClientHeight;
	int heightChar;
	RECT screenClientRectangle;

public:
	void setHeightChar(int value);
	void setCountRow(int value);
	int getCountRow();
	void setCountColum(int value);
	int getCountColum();
	void createCells();
	void calculatingCellSizes();
	void setScreenClientRectangle(HWND hwnd);
	void drawCells(HWND hWndParent);
	void resizingTable();
	void changeInTable(HWND hWndParent);
	void drawBorder(HDC hDc,RECT rect);
	table();
	~table();
};

