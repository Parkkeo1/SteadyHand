#include "mouse_handler.h"

const LPCWSTR kClassName = TEXT("SteadyHand");
const std::string kInactive = "Inactive";

// constants for Windows API raw input device codes.
const short kDesktopUsage = 1;
const short kMouseUsage = 2;
const short kKeyBoardUsage = 6;

void MouseHandler::RegisterMouse() {
	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx#example_2
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = kDesktopUsage;
	Rid[0].usUsage = kMouseUsage;
	Rid[0].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[0].hwndTarget = input_window;

	Rid[1].usUsagePage = kDesktopUsage;
	Rid[1].usUsage = kKeyBoardUsage;
	Rid[1].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[1].hwndTarget = input_window;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]))) {
		SetWindowLongPtr(input_window, GWLP_WNDPROC, (LONG_PTR)StaticWinProc);
		std::cout << "Device registration successful." << std::endl;
	}
	else {
		std::cout << "Device registration failed." << std::endl;
	}
}

// code derived from:
// https://stackoverflow.com/a/28491549
LRESULT MouseHandler::StaticWinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	MouseHandler *p_this = (MouseHandler*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (!p_this) {
		return DefWindowProc(hwnd, msg, w_param, l_param);
	}

	return p_this->ClassWinProc(msg, w_param, l_param);
}

void MouseHandler::Setup() {
	SetWindowLongPtr(input_window, GWLP_USERDATA, (LONG_PTR)this);
	RegisterMouse();
	std::cout << "setup complete." << std::endl;
}

void MouseHandler::Run() {
	// TODO: Fix this message loop to exit properly when user presses ENTER.
	while (true) {
		MSG message;
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			if (message.message == WM_QUIT) {
				break;
			} else {
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		} else {
			break;
		}
	}
}
