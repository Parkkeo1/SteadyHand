#include "mover_wind.h"

const LPCWSTR kClassName = TEXT("SteadyHand");

// constants for Windows API raw input device codes.
const short kDesktopUsage = 1;
const short kMouseUsage = 2;
const short kKeyBoardUsage = 6;

void MouseMoverWindHandler::RegisterInputDevices(HWND window) {
	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx#example_2
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = kDesktopUsage;
	Rid[0].usUsage = kMouseUsage;
	Rid[0].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[0].hwndTarget = window;

	Rid[1].usUsagePage = kDesktopUsage;
	Rid[1].usUsage = kKeyBoardUsage;
	Rid[1].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[1].hwndTarget = window;

	if (!RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]))) {
		std::cout << "Device registration failed." << std::endl;
		return;
	}
	// code derived from:
	// https://github.com/rspeele/MouseMeat/blob/0b5b7aea844f7ab4158b4ebd34e8c2d1e702f995/Input.cpp#L227
	base_wnd_proc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
	std::cout << "Device registration successful." << std::endl;
}
