#include <Windows.h>

namespace Engine
{ LRESULT CALLBACK Procedure(HWND const, UINT const, WPARAM const, LPARAM const); }

//APIENTRY : Starting point of this program
int APIENTRY WinMain
(
	_In_		HINSTANCE	const hInstance,
	_In_opt_	HINSTANCE	const hprevInstance,
	_In_		LPSTR		const lpCmdLine,
	_In_		int			const nShowCmd
)
{
	HWND hWindow = nullptr;
	{
		//set descriptor
		WNDCLASSEX wndClass		= WNDCLASSEX();
		wndClass.cbSize			= sizeof(WNDCLASSEX);
		wndClass.lpfnWndProc	= Engine::Procedure;//메세지 처리 기능
		wndClass.hInstance		= hInstance;
		wndClass.hIcon			= LoadIcon(nullptr, IDI_APPLICATION);
		wndClass.hIconSm		= LoadIcon(nullptr, IDI_APPLICATION);
		wndClass.hCursor		= LoadCursor(nullptr, IDC_ARROW);
		wndClass.hbrBackground	= static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wndClass.lpszClassName	= "Window";

		//register window descriptor
		RegisterClassEx(&wndClass);
	}

	{
		CREATESTRUCT window = CREATESTRUCT();

		window.lpszClass	= "Window";//just set above. Need to be same name.
		window.lpszName		= "Game";//Just name
		window.style		= WS_CAPTION | WS_SYSMENU;
		// WS_CAPTION : set window to have title bar
		// WS_SYSMENU : set title bar to have system menu. WS_CAPTION has to be set in advance.
		window.cx			= 1280;
		window.cy			= 720;
		window.hInstance	= hInstance;

		{
			RECT rect = RECT();
			rect.right	= window.cx;
			rect.bottom = window.cy;

			AdjustWindowRectEx(&rect, window.style, static_cast<bool>(window.hMenu), window.dwExStyle);
			//set real region(not including title bar) as 1280*720
			window.cx = rect.right - rect.left;
			window.cy = rect.bottom - rect.top;

			//locate window in the middle of the screen
			window.x = (GetSystemMetrics(SM_CXSCREEN) - window.cx) / 2;
			window.y = (GetSystemMetrics(SM_CYSCREEN) - window.cy) / 2;
		}
		//SM_CXSCREEN : Get Screen x value
		//SM_CYSCREEN : Get Screen y value

		hWindow = CreateWindowEx
		(
			window.dwExStyle,
			window.lpszClass,
			window.lpszName,
			window.style,
			window.x,
			window.y,
			window.cx,
			window.cy,
			window.hwndParent,
			window.hMenu,
			window.hInstance,
			window.lpCreateParams
		);
	}

	ShowWindow(hWindow, SW_RESTORE);

	{
		MSG msg = MSG();

		while (true)
		{
			if (PeekMessage(&msg, 
				HWND(), 
				WM_NULL, 
				WM_NULL, 
				PM_REMOVE// remove received msg
			))

			{
				if (msg.message == WM_QUIT)
					return static_cast<int>(msg.wParam);

				DispatchMessage(&msg);
			}
			else
			{
				SendMessage(hWindow, WM_APP, 0, 0);
			}
		}

	}
}