#ifndef UNICODE
#define UNICODE
#endif

#include "resource.h"
#include "CWindow.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

wchar_t *COverlappedWindow::nameClassWindow = L"MyClassWindow";
wchar_t *COverlappedWindow::nameWindow = L"Tic Tac Toe";

COverlappedWindow::COverlappedWindow() 
{
	sizeXOfBoard = defaultSizeOfBoard;
	sizeYOfBoard = defaultSizeOfBoard;
	initGame();
	isComputerMode = true;
}

void COverlappedWindow::initGame() 
{
	step = 1;
	field.resize(sizeXOfBoard);
	for (int x = 0; x < sizeXOfBoard; x++) {
		field[x].resize(sizeYOfBoard);
		for (int y= 0; y < sizeYOfBoard; y++)
		{
			field[x][y].figure = GAME_EMPTY;
			field[x][y].weight = 0;
			if (y % 2)
				field[x][y].isBlackCell = x % 2;
			else
				field[x][y].isBlackCell = !(x % 2);
		}
	}
	partEnd = false;
}

COverlappedWindow::~COverlappedWindow() 
{
}

HWND COverlappedWindow::getHandle()
{
	return handle;
}

HWND COverlappedWindow::getDialogHandle()
{
	return dialogId;
}

bool COverlappedWindow::withinTheWill(int x, int y)
{
	return ( x >= 0 && x < sizeXOfBoard && y >= 0 && y < sizeYOfBoard );
}

double COverlappedWindow::findMaxLenght(int x, int y, int dx, int dy, int color)
{
	int lenght = (int)field[x][y].figure == color;
	int x0=x, y0=y;
	x += dx; y += dy;
	while ( withinTheWill(x, y) && field[x][y].figure == color)
	{
		lenght++;
		x += dx; y += dy;
	}
	x = x0-dx; y = y0 - dy;
	while ( withinTheWill(x, y) && field[x][y].figure == color)
	{
		lenght++;
		x -= dx; y -= dy;
	}
	return lenght;
}


int COverlappedWindow::cellWhiteWeight(int x, int y)
{
	int toWin = min(min(sizeXOfBoard, sizeYOfBoard), 5);
	int res1 = findMaxLenght(x, y, 1, 0, GAME_TOE); 
	int res2 = findMaxLenght(x, y, 0, 1, GAME_TOE);
	int res3 = findMaxLenght(x, y, 1, 1, GAME_TOE); 
	int res4 = findMaxLenght(x, y, 1, -1, GAME_TOE);
	if (res1 >= toWin || res2 >= toWin || res3 >= toWin || res4 >= toWin)
	{
		::MessageBox(0, L"Toe WIN!!!", L"The End!", MB_OK | MB_ICONEXCLAMATION);
		this->partEnd = true;
	}
	return res1*res1+res2*res2+res3*res3+res4*res4;
}

int COverlappedWindow::cellBlackWeight(int x, int y)
{
	int toWin = min(min(sizeXOfBoard, sizeYOfBoard), 5);
	int res1 = findMaxLenght(x, y, 1, 0, GAME_CROSS);
	int res2 = findMaxLenght(x, y, 0, 1, GAME_CROSS);
	int res3 = findMaxLenght(x, y, 1, 1, GAME_CROSS);
	int res4 = findMaxLenght(x, y, 1, -1, GAME_CROSS);
	if (res1 >= toWin-1 || res2 >= toWin-1 || res3 >= toWin-1 || res4 >= toWin-1)
	{
		::MessageBox(0, L"Cross WIN!!!", L"The End!", MB_OK | MB_ICONEXCLAMATION);
		this->partEnd = true;
		return MAXINT;
	}
	return max(res1, res2, res3, res4);
}

bool COverlappedWindow::isWin(int x, int y, int player)
{
	int toWin = min(min(sizeXOfBoard, sizeYOfBoard), 5);
	int res1 = findMaxLenght(x, y, 1, 0, player);
	int res2 = findMaxLenght(x, y, 0, 1, player);
	int res3 = findMaxLenght(x, y, 1, 1, player);
	int res4 = findMaxLenght(x, y, 1, -1, player);
	return (res1 >= toWin || res2 >= toWin || res3 >= toWin || res4 >= toWin);
}

int COverlappedWindow::cellWeight(int x, int y)
{
	int lenBlack = cellBlackWeight(x, y);
	int lenWhite = cellWhiteWeight(x, y);
	return max( lenBlack, lenWhite );
}

int COverlappedWindow::weightNeighbors(int x, int y, int dx, int dy, int color)
{
	if (withinTheWill(x+dx, y+dy))
		if (field[x+dx][y+dy].figure == color)
			return 2;
		else
			return 1;
	else
		return 0;
}

int COverlappedWindow::neighbors(int x, int y, int color)
{
	return weightNeighbors(x, y, 1, 1, color)+
		weightNeighbors(x, y, 0, 1, color)+
		weightNeighbors(x, y, -1, 1, color)+
		weightNeighbors(x, y, 1, 0, color)+
		weightNeighbors(x, y, -1, 0, color)+
		weightNeighbors(x, y, 1, -1, color)+
		weightNeighbors(x, y, 0, -1, color)+
		weightNeighbors(x, y, -1, -1, color);
}

bool COverlappedWindow::checkField()
{
	for (int x = 0; x < sizeXOfBoard; x++)
		for (int y = 0; y < sizeYOfBoard; y++)
		{
			if (field[x][y].figure == GAME_EMPTY) {
				return false;
			}
		}
	return true;
}

bool COverlappedWindow::checkWins() 
{
	for (int x = 0; x < sizeXOfBoard; x++)
		for (int y = 0; y < sizeYOfBoard; y++)
		{
			if (isWin(x,y,GAME_CROSS)) {
				::MessageBox(0, L"Cross win!!!", L"The End!", MB_OK | MB_ICONEXCLAMATION);
				return true;
			}
			if (isWin(x,y,GAME_TOE)) {
				::MessageBox(0, L"Toe win!!!", L"The End!", MB_OK | MB_ICONEXCLAMATION);
				return true;
			}

		}
	return false;
}

void COverlappedWindow::moveComputer()
{
	int maxWeight = -1;
	if (checkField()) {
		::MessageBox(0, L"DRAW!!!", L"The End!", MB_OK | MB_ICONEXCLAMATION);
		partEnd = true;
		return;
	}
	if (checkWins()) {
		partEnd = true;
		return;
	}
	std::vector<std::pair<int, int>> resultOfWeight;
	int posx=0, posy=0;
	for (int x = 0; x < sizeXOfBoard; x++)
		for (int y = 0; y < sizeYOfBoard; y++)
		{
			if (!partEnd && field[x][y].figure == GAME_EMPTY)
			{
				field[x][y].weight = cellWeight(x, y);
				if (field[x][y].weight == maxWeight)
				{
					resultOfWeight.push_back(std::pair<int, int>(x, y));
				}
				if (field[x][y].weight > maxWeight)
				{
					maxWeight = field[x][y].weight;
					resultOfWeight.clear();
					resultOfWeight.push_back(std::pair<int, int>(x, y));
				}
			}
		}
	if (!(resultOfWeight.size() == 1))
	{
		std::vector<std::pair<int, int>> resultOfneighbors;	
		int maxNeighbors = -1;
		for (int i=0; i<resultOfWeight.size(); i++)
		{
			int x = resultOfWeight[0].first;
			int y = resultOfWeight[0].second;
			int countNeighbors = neighbors(x, y, GAME_CROSS);
			if (countNeighbors == maxNeighbors)
			{
				resultOfneighbors.push_back(resultOfWeight[0]);
			}
			if (countNeighbors > maxNeighbors)
			{
				countNeighbors = maxNeighbors;
				resultOfneighbors.clear();
				resultOfneighbors.push_back(std::pair<int, int>(x, y));
			}
		}
		if (!(resultOfneighbors.size() == 1))
		{
			field[resultOfneighbors[0].first][resultOfneighbors[0].second].figure = GAME_CROSS;			
		}
		else
		{
			field[resultOfneighbors[0].first][resultOfneighbors[0].second].figure = GAME_CROSS;
		}
	}
	else
		field[resultOfWeight[0].first][resultOfWeight[0].second].figure = GAME_CROSS;
}

void COverlappedWindow::onButton(int xPos, int yPos){
	if (!partEnd)
	{
		RECT rect;
		int x = xPos / GAME_CELL;
		int y = yPos / GAME_CELL;
		if (field[x][y].figure == GAME_EMPTY) {
			if (step == 1)
				field[x][y].figure = GAME_TOE;
			else field[x][y].figure = GAME_CROSS;
			if (isComputerMode) {
				moveComputer();
			} else {
				step = 1 - step;
				if (isWin(x, y, GAME_TOE)) {
					::MessageBox(0, L"Toe WIN!!!", L"The End!", MB_OK | MB_ICONEXCLAMATION);
					partEnd = true;
				}
				else if (isWin(x, y, GAME_CROSS)) {
					::MessageBox(0, L"Cross WIN!!!", L"The End!", MB_OK | MB_ICONEXCLAMATION);
					partEnd = true;
				} else if (checkField()) {
					::MessageBox(0, L"DRAW!!!", L"The End!", MB_OK | MB_ICONEXCLAMATION);
					partEnd = true;	
				}
			}
		}
		if (GetClientRect(handle, &rect)) {
			InvalidateRect(handle, &rect, false);
		}
	}
}

bool COverlappedWindow::RegisterClassA(HINSTANCE hInstance) {
	WNDCLASSEX tag;
	tag.cbSize = sizeof(WNDCLASSEX);
	tag.style = CS_HREDRAW | CS_VREDRAW;
	tag.lpfnWndProc = windowProc;
	tag.cbClsExtra = 0;
	tag.cbWndExtra = 0;
	tag.hIcon = (HICON)::LoadImage(::GetModuleHandle(0), (LPCWSTR)IDI_ICON1, IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
	tag.hIconSm = (HICON)::LoadImage(::GetModuleHandle(0), (LPCWSTR)IDI_ICON1,IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
	tag.hCursor = LoadCursor(NULL, IDC_ARROW);
	tag.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	tag.lpszMenuName = LPCWSTR(IDR_MENU1);;
	tag.lpszClassName = nameClassWindow;
	tag.hInstance = hInstance; 
	return ::RegisterClassEx(&tag);
}

bool COverlappedWindow::Create(HINSTANCE hInstance) {
	handle = ::CreateWindow(
		nameClassWindow,
		nameWindow,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_EX_LAYERED,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		sizeXOfBoard * GAME_CELL + 10,
		(sizeYOfBoard + 1) * GAME_CELL + 10,
		NULL,
		NULL,
		hInstance,
		this);	
	if (handle == 0) {
		::MessageBox(0, L"Can't got handle", L"ERROOR!", MB_OK | MB_ICONEXCLAMATION);
	}
	::SetWindowLong(handle, GWL_USERDATA, (long)this);
	return handle == 0;	
}

void COverlappedWindow::Show(int cmdShow) {
	::ShowWindow(handle, cmdShow);
	::UpdateWindow(handle);
}

void COverlappedWindow::onDestroy() {
	PostQuitMessage(0);	
}

void COverlappedWindow::onPaint() {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint( handle, &ps );
	HDC newHdc = ::CreateCompatibleDC(hdc);
	RECT rect;
	GetClientRect(handle, &rect);

	HBITMAP bitmap = ::CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
	HGDIOBJ oldbitmap = ::SelectObject(newHdc, bitmap);

	draw(newHdc);

	BitBlt( hdc, 0, 0, rect.right, rect.bottom, newHdc, 0, 0, SRCCOPY );
	
	::SelectObject(newHdc, oldbitmap);
	::DeleteObject(bitmap);
	::DeleteDC(newHdc);

	::EndPaint(handle, &ps);
}

void COverlappedWindow::onClose() {
	switch (MessageBox( handle, reinterpret_cast<LPCWSTR>(L"Are you sure?"), reinterpret_cast<LPCWSTR>(L"Attention"), MB_ICONWARNING | MB_YESNO))
	{
	case IDNO:
		break;
	case IDYES:
		onDestroy();
		break;
	}
}

void COverlappedWindow::onNewGame(){
	switch (MessageBox( handle, reinterpret_cast<LPCWSTR>(L"Do you want to start new game??"), reinterpret_cast<LPCWSTR>(L"Attention"), MB_ICONWARNING | MB_YESNO))
	{
		case IDNO:
			return;
		case IDYES:
			break;
	}
	initGame();
	RECT rect;
	if (GetClientRect(handle, &rect)) {
		InvalidateRect(handle, &rect, false);
	}
}

void COverlappedWindow::drawToe(HDC dc, int x, int y, HBRUSH mainBrush, HPEN focusBrush) {
		::SelectObject( dc, focusBrush );
		Ellipse(dc, x*GAME_CELL, y*GAME_CELL, (x+1)*GAME_CELL, (y+1)*GAME_CELL);
		::SelectObject( dc, mainBrush );
		Ellipse(dc, x*GAME_CELL + MARGIN, y*GAME_CELL + MARGIN, (x+1)*GAME_CELL- MARGIN, (y+1)*GAME_CELL- MARGIN);
}

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	return LineTo(hdc, x2, y2);
} 

void COverlappedWindow::drawNought(HDC dc, int x, int y, HPEN hpen) {
		::SelectObject( dc, hpen );		
		Line(dc, x*GAME_CELL+MARGIN, y*GAME_CELL+MARGIN, (x+1)*GAME_CELL-MARGIN, (y+1)*GAME_CELL-MARGIN);
		Line(dc, (x+1)*GAME_CELL-MARGIN, y*GAME_CELL+MARGIN, x*GAME_CELL+MARGIN, (y+1)*GAME_CELL-MARGIN);
}

void COverlappedWindow::onSettings()
{
	((COverlappedWindow*)(::GetWindowLong( handle, GWL_USERDATA )))->dialogId 
        = ::CreateDialog( ::GetModuleHandle( 0 ), MAKEINTRESOURCE( IDD_DIALOG1 ), handle, COverlappedWindow::dialogProc);
	if (!((COverlappedWindow*)(::GetWindowLong( handle, GWL_USERDATA )))->dialogId ) {
        MessageBox( NULL,
            L"Can't create dialog!!!",
            L"Error!",
            NULL );
    }
    else {
        ::ShowWindow( ((COverlappedWindow*)(::GetWindowLong( handle, GWL_USERDATA )))->dialogId, SW_SHOW );
    }
}

void COverlappedWindow::onDialogCancel(HWND dialogHandle)
{
	::DestroyWindow(dialogHandle);
}

void COverlappedWindow::onDialogOk(HWND dialogHandle)
{
	HWND size_X = ::GetDlgItem(dialogHandle, IDC_EDIT3);
    HWND size_Y = ::GetDlgItem(dialogHandle, IDC_EDIT2);

	wchar_t num[1024];
	::GetWindowText(size_X, num, sizeof(num));
	int sizeX = _wtoi(num);
	::GetWindowText(size_Y, num, sizeof(num));
	int sizeY = _wtoi(num);
	if (sizeY * sizeX > 1) {
		sizeYOfBoard = sizeY;
		sizeXOfBoard = sizeX;
		onNewGame();
		RECT rect;
		GetClientRect(handle, &rect);
		MoveWindow(handle, rect.left, rect.top, 0, 0, TRUE);
	}
	::DestroyWindow(dialogHandle);
}

BOOL __stdcall COverlappedWindow::dialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	COverlappedWindow* me =  ((COverlappedWindow*)(::GetWindowLong(::GetParent(dialogHandle) , GWL_USERDATA)));
	switch (message) {
        case WM_GETDLGCODE:
        {
            return DLGC_WANTALLKEYS;
        }
        case WM_COMMAND:
        {
            switch (LOWORD(wParam)) {
                case IDCANCEL:
                {
                    me->onDialogCancel(dialogHandle);
                    return 1;
                }
                case IDOK:
                {
                    me->onDialogOk(dialogHandle);
                    return 1;
                }
            }
        }
    }
	return 0;
}


void COverlappedWindow::onCommand(WPARAM wParam)
{
	int Id = LOWORD( wParam );
	switch ( Id ){
		case ID_NEWGAME_VSCOMPUTER:
			isComputerMode = true;
			onNewGame();
			break;
		case ID_NEWGAME_VSMAN:
			isComputerMode = false;
			onNewGame();
			break;
		case ID_MENU_SETTINGS:
			onSettings();
			break;
		case ID_MENU_EXIT:
			onClose();
			break;
		case ID_ACCELERATOR1:
			::DestroyWindow(handle);
			break;
		case ID_ACCELERATOR2:
			onNewGame();
			break;
		default:
			break;
	}
}

LRESULT __stdcall COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	COverlappedWindow* me = reinterpret_cast< COverlappedWindow* >( GetWindowLong( handle, GWL_USERDATA) );
	int Id, Event;
	switch (message) {
		case WM_GETMINMAXINFO:
			{
				MINMAXINFO *pInfo = (MINMAXINFO *)lParam;
				POINT Min = { GAME_CELL*me->sizeXOfBoard+10,  GAME_CELL*(me->sizeYOfBoard + 1) };
				POINT  Max = {  GAME_CELL*me->sizeXOfBoard+10,  GAME_CELL*(me->sizeYOfBoard + 1) };
				pInfo->ptMinTrackSize = Min; 
				pInfo->ptMaxTrackSize = Max; 
				return 0;
			}
		case WM_DESTROY:
			me->onDestroy();
			return 0;
		case WM_LBUTTONDOWN:
			me->onButton(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_PAINT:
			RECT r;
			GetClientRect(handle, &r);
			me->onPaint();
			return 0;
		case WM_CLOSE:
			me->onClose();
			return 0;
		case WM_COMMAND:
			me->onCommand(wParam);
			return 0;
	}
	return ::DefWindowProc(handle, message, wParam, lParam);
}

void COverlappedWindow::draw(HDC dc) {
	RECT mainRect;

	GetClientRect(handle, &mainRect);
	HBRUSH brush = CreateSolidBrush(RGB(250, 250, 250));
	FillRect(dc, &mainRect, brush);
	RECT myrect;
	HBRUSH cellBrush;
	cellBrush = ::CreateSolidBrush( RGB(240, 240, 240));
	HPEN hpenCross = CreatePen(PS_DASHDOT, 5, RGB(10,10,255));
	HPEN hpenToe = CreatePen(PS_DASHDOT, 5, RGB(128,64,64));
	for (int x = 0; x < sizeXOfBoard; x++)
		for (int y = 0; y < sizeYOfBoard; y++)
		{
			if (field[x][y].isBlackCell){
				myrect.right = x*GAME_CELL;
				myrect.top = y*GAME_CELL;
				myrect.left = (x+1)*GAME_CELL;
				myrect.bottom = (y+1)*GAME_CELL;
				FillRect(dc, &myrect, cellBrush);
			} 
			if (field[x][y].figure == GAME_TOE){
					if (field[x][y].isBlackCell)
						drawToe(dc, x, y, cellBrush, hpenToe);
					else
						drawToe(dc, x, y, brush, hpenToe);
			} 
			if (field[x][y].figure == GAME_CROSS) {
					drawNought(dc, x, y, hpenCross);
			}			
		}
	::DeleteObject(brush);
	::DeleteObject(cellBrush);
	::DeleteObject(hpenCross);
	::DeleteObject(hpenToe);
}