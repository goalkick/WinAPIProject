#include <windows.h>
#include <windowsx.h>

#include <vector>

#define GAME_CELL 60
#define GAME_CROSS 1
#define GAME_TOE 2
#define GAME_EMPTY 0
#define MARGIN 3

#define defaultSizeOfBoard  15

class COverlappedWindow {
public:
	struct Cell
	{
		bool isBlackCell;
		int figure;
		int weight;
	};
	COverlappedWindow();
	~COverlappedWindow();

	static bool RegisterClassA(HINSTANCE hInstance);

	bool Create(HINSTANCE hInstance);
	void Show(int cmdShow);
	HWND getHandle();
	HWND getDialogHandle();

	int step;
	bool isComputerMode;
	int sizeXOfBoard;
	int sizeYOfBoard;

protected:
	void onNewGame();
	void onHelp();
	void onDestroy();
	void onPaint();
	void onClose();
	void onResize();
	void onSettings();
	bool checkField();
	bool checkWins();
	void onCommand(WPARAM wParam);
	void onDialogCancel(HWND dialogHandle);
	void onDialogOk(HWND dialogHandle);
	void drawToe(HDC, int, int, HBRUSH, HPEN);
	void drawNought(HDC, int, int, HPEN);
	bool withinTheWill(int x, int y);
	double findMaxLenght(int x, int y, int dx, int dy, int color);
	int cellBlackWeight(int x, int y);
	int cellWhiteWeight(int x, int y);	
	int cellWeight(int x, int y);
	int weightNeighbors(int x, int y, int dx, int dy, int color);
	int neighbors(int x, int y, int color);
	bool isWin(int x, int y, int player);
	void moveComputer();
	void onButton(int, int);
	void initGame();
private:
	static wchar_t* nameClassWindow; 
	static wchar_t* nameWindow; 
	
	HWND handle;
	HWND dialogId;
	std::vector <std::vector <Cell> >  field;
	bool partEnd;
	void draw(HDC dc);
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static BOOL __stdcall dialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);
};
