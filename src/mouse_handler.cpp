#include "mouse_handler.h"

// string constants used for setting up program.
const LPCWSTR kClassName = (LPCWSTR)"SteadyHand";
const std::string kInactive = "Inactive";

// constants for Windows API raw input device codes.
const short kDesktopMode = 1;
const short kMouseInput = 2;
const short kKeyBoardInput = 6;

const std::set<std::string> kWeaponNameCodes = {
	"weapon_ak47",
	"weapon_m4a1",
	"weapon_m4a1_silencer",
	"weapon_famas"
};

void MouseHandler::RegisterMouse() {
	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx#example_2
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = kDesktopMode;
	Rid[0].usUsage = kMouseInput;
	Rid[0].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[0].hwndTarget = input_window;

	Rid[1].usUsagePage = kDesktopMode;
	Rid[1].usUsage = kKeyBoardInput;
	Rid[1].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[1].hwndTarget = input_window;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]))) {
		SetWindowLongPtr(input_window, GWLP_WNDPROC, (LONG_PTR)StaticWinProc);
		std::cout << "Device registration successful." << std::endl;
	} else {
		std::cout << "Device registration failed." << std::endl;
	}
}

LPBYTE MouseHandler::CheckMessageSize(LPARAM &l_param) {
	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx#standard_read
	UINT dw_size;
	GetRawInputData((HRAWINPUT)l_param, RID_INPUT, NULL, &dw_size, sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dw_size];
	if (lpb == NULL) {
		return LPBYTE();
	}

	if (GetRawInputData((HRAWINPUT)l_param, RID_INPUT, lpb, &dw_size, sizeof(RAWINPUTHEADER)) != dw_size) {
		std::cout << "GetRawInputData does not return correct size !" << std::endl;
		return LPBYTE();
	}
	return lpb;
}

// code referenced from Microsoft MSDN documentation:
// https://msdn.microsoft.com/en-us/library/windows/desktop/ff381400(v=vs.85).aspx
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
	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644928(v=vs.85).aspx#creating_loop
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
