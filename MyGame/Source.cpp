#include "resource.h"
#include "CWindow.h"
#include <CommCtrl.h>

int _stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR commandLine, int nCmdShow) 
{
	
	COverlappedWindow::RegisterClassA(hInstance);
	HACCEL haccel = ::LoadAccelerators(hInstance, LPCSTR(IDR_ACCELERATOR1)); 
	COverlappedWindow window;
	
	window.Create(hInstance);
	window.Show(nCmdShow);

	MSG msg;
    while(::GetMessage(&msg, NULL, 0, 0)) {
		if (!::TranslateAccelerator(window.getHandle(), haccel, &msg) && !::IsDialogMessage(window.getDialogHandle(), &msg)) {
        TranslateMessage( &msg );
		DispatchMessage( &msg );  
		}
    }

	::DestroyAcceleratorTable(haccel);

    return static_cast<int>(msg.wParam);
}