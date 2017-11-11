#include "table.h"
#include <vector>

void table::setCountColum(int value) { countColumn = value; }
int table::getCountColum() { return countColumn; }
void table::setHeightChar(int value)
{
	heightChar = value;
}
void table::setCountRow(int value) { countRow = value; }
int table::getCountRow() { return countRow; }



void table::setScreenClientRectangle(HWND hWnd)
{
	RECT screenClientRectangle;
	GetClientRect(hWnd, &screenClientRectangle);
	screenClientWidth = screenClientRectangle.right - screenClientRectangle.left;
	screenClientHeight = screenClientRectangle.bottom - screenClientRectangle.top;
}


//void table::resizingTable()
//{
//	int x = 0;
//	int y = 0;
//	int j = 0;
//	for (const auto& cell : vecElements)
//	{
//		RECT newRect;
//		newRect.left = x;
//		x += cellWidth;
//		newRect.right = x;
//		newRect.top = y;
//		newRect.bottom = y + cellHeight;
//		j++;
//		if (j >= getCountColum())
//		{
//			j = 0;
//			y += cellHeight;
//			x = 0;
//		}
//		cell->setRect(newRect);
//		cell->changeEditSize();
//	}
//
//}
void table::changeInTable(HWND hWndParent)
{
	int i = 1;
	int j = 0;
	int maxHeightInRow = 0;
	int standartLineCount = cellHeight / heightChar;
	std::vector<int> maxHeight(getCountRow());

	for (const auto& cell : vecElements)
	{
		int heightInCell = SendMessage(cell->getHWnd(), EM_GETLINECOUNT, 0, 0);
		if (heightInCell > maxHeightInRow)
		{
			maxHeightInRow = heightInCell;
		}

		if (i == getCountColum())
		{
			maxHeight[j] = maxHeightInRow;
			maxHeightInRow = 0;
			i = 0;
			j++;
		}
		i++;
	}
	i = 1;
	j = 0;
	int y = 0;
	int x = 0;
	for (const auto& cell : vecElements)
	{

		RECT newRect = cell->getRect();
		RECT oldRect = cell->getRect();
		newRect.left = x;
		x += cellWidth;
		newRect.right = x;
		newRect.top = y;
		newRect.bottom = y + maxHeight[j] * heightChar;
		cell->setRect(newRect);
		if (!EqualRect(&oldRect, &newRect))
		{
			cell->changeEditSize();
		}
		if (i == getCountColum())
		{
			y += maxHeight[j] * heightChar;
			x = 0;
			i = 0;
			j++;
		}
		i++;

	}
}
void table::drawBorder(HDC hDc,RECT rect)
{
	POINT point;
	
	point.y = rect.top;	
	rect.right = rect.left + countColumn*cellWidth;
	for (int i = 0; i <= countColumn; i++)
	{
		point.x = rect.left;
		MoveToEx(hDc, point.x, point.y, NULL);
		point.x = rect.right;
		LineTo(hDc, point.x, point.y);
		point.y += cellHeight;
	}

	rect.bottom = rect.top + cellHeight*countRow;
	point.x = rect.left;
	point.y = rect.top;

	for (int i = 0; i <= countRow; i++)
	{
		MoveToEx(hDc, point.x, point.y,NULL);
		point.y = rect.bottom;
		LineTo(hDc, point.x, point.y);
		point.y = rect.top;
		point.x += cellWidth;
	}
}
void table::calculatingCellSizes()
{
	cellWidth = screenClientWidth / getCountColum();
	cellHeight = heightChar;//screenClientHeight / getCountRow();

}

void table::drawCells(HWND hWndParent)
{
	for (const auto& cell : vecElements)
	{
		HWND hEdit = cell->createEdit(hWndParent);
		cell->setHWnd(hEdit);
	}
}
void table::createCells()
{
	int x = 0;
	int y = 0;
	for (int i = 0; i < getCountRow(); i++)
	{
		x = 0;
		for (int j = 0; j < getCountColum(); j++)
		{
			RECT rect;
			rect.left = x;
			x += cellWidth;
			rect.right = x;
			rect.top = y;
			rect.bottom = y + cellHeight;

			tableCell* cell = new tableCell(rect);
			vecElements.push_back(cell);
		}
		y += cellHeight;
	}
}

table::~table() {}
table::table() {}