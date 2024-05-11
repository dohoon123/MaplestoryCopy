#include<windows.h>
namespace Input
{
	class 
	{
	public :
		void Down(WPARAM const code)
		{
			if (!Pressed(code))
			{
				State.Pressed[code >> 0x4] ^= (0x8000 >> (code & 0xf));
				State.Changed[code >> 0x4] ^= (0x8000 >> (code & 0xf));
			}
		}
		
		void Up(WPARAM const code)
		{
			State.Pressed[code >> 0x4] ^= (0x8000 >> (code & 0xf));
			State.Changed[code >> 0x4] ^= (0x8000 >> (code & 0xf));
		}

		bool Pressed(WPARAM const code)
		{ return State.Pressed[code >> 0x4] & (0x8000 >> (code & 0xf)); }

		bool Changed(WPARAM const code)
		{
			return State.Changed[code >> 0x4] & (0x8000 >> (code & 0xf));
		}

		void Update()
		{
			ZeroMemory(State.Changed, sizeof(State.Changed));
		}

	private :
		struct 
		{
			USHORT Pressed[16];
			USHORT Changed[16];
		}State;
	
	}Key;

	POINT Cursor;
	POINT Wheel;

	bool CALLBACK isPressed(WPARAM const code) {
		return Key.Pressed(code);
	}

	
	namespace Get
	{
		namespace Key
		{
			bool Down(size_t const code)	 { return		Input::Key.Pressed(code) and Input::Key.Changed(code); }
			bool Press(size_t const code)	 { return		Input::Key.Pressed(code); }
			bool Up(size_t const code)		 { return	not Input::Key.Pressed(code) and Input::Key.Changed(code); }
		}

		namespace Cursor
		{
			long X() { return Input::Cursor.x; }
			long Y() { return Input::Cursor.y; }
		}

		namespace Wheel
		{
			long H() { return Input::Wheel.x; }
			long V() { return Input::Wheel.y; }
		}
	}



	void CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{
		switch (uMessage)
		{
		case WM_APP :
		{
			Key.Update();

			Wheel = POINT();
			return;
		}
		case WM_MOUSEWHEEL : { Wheel.y += GET_WHEEL_DELTA_WPARAM(wParameter) / WHEEL_DELTA; return; }
		case WM_MOUSEHWHEEL: { Wheel.x += GET_WHEEL_DELTA_WPARAM(wParameter) / WHEEL_DELTA; return; }
		case WM_MOUSEMOVE  :
		{
			Cursor.x = LOWORD(lParameter);
			Cursor.y = HIWORD(lParameter);
			return;
		}
		case WM_SYSKEYDOWN : { Key.Down(wParameter); return; }
		case WM_KEYDOWN    : { Key.Down(wParameter); return; }
		case WM_LBUTTONDOWN: { Key.Down(VK_LBUTTON); return; }
		case WM_RBUTTONDOWN: { Key.Down(VK_RBUTTON); return; }
		case WM_MBUTTONDOWN: { Key.Down(VK_MBUTTON); return; }
		case WM_XBUTTONDOWN: 
		{ 
			switch (GET_XBUTTON_WPARAM(wParameter))
			{
			   case XBUTTON1 : Key.Down(VK_XBUTTON1); return; 
			   case XBUTTON2 : Key.Down(VK_XBUTTON2); return;
			}
		}
		case WM_SYSKEYUP : { Key.Up(wParameter); return; }
		case WM_KEYUP    : { Key.Up(wParameter); return; }
		case WM_LBUTTONUP: { Key.Up(VK_LBUTTON); return; }
		case WM_RBUTTONUP: { Key.Up(VK_RBUTTON); return; }
		case WM_MBUTTONUP: { Key.Up(VK_MBUTTON); return; }
		case WM_XBUTTONUP:
		{
			switch (GET_XBUTTON_WPARAM(wParameter))
			{
			 case XBUTTON1 : Key.Up(VK_XBUTTON1); return;
			 case XBUTTON2 : Key.Up(VK_XBUTTON2); return;
			}
		}
		}
		return;
	}
}