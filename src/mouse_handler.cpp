#include "mouse_handler.h" // <- detailed comments in header

// initializing static members
const std::string MouseHandler::inactive_weapon = "inactive";
const std::set<std::string> MouseHandler::weapon_names = {
	"weapon_ak47",
	"weapon_m4a1",
	"weapon_m4a1_silencer",
	"weapon_famas"
};
const LPCWSTR MouseHandler::class_name = (LPCWSTR)"SteadyHand";

void MouseHandler::register_mouse() {
	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx#example_2
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = desktop_mode_code;
	Rid[0].usUsage = mouse_input_code;
	Rid[0].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[0].hwndTarget = input_window;

	Rid[1].usUsagePage = desktop_mode_code;
	Rid[1].usUsage = keyboard_input_code;
	Rid[1].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[1].hwndTarget = input_window;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]))) {
		SetWindowLongPtr(input_window, GWLP_WNDPROC, (LONG_PTR)StaticWinProc);
		std::cout << "Device registration successful." << std::endl;
	} else {
		std::cout << "Device registration failed." << std::endl;
	}
}

LPBYTE MouseHandler::check_message_size(LPARAM &l_param) {
	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx#standard_read
	UINT dw_size;
	GetRawInputData((HRAWINPUT)l_param, RID_INPUT, NULL, &dw_size, sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dw_size];
	if (lpb == NULL) {
		return NULL;
	}

	if (GetRawInputData((HRAWINPUT)l_param, RID_INPUT, lpb, &dw_size, sizeof(RAWINPUTHEADER)) != dw_size) {
		std::cout << "GetRawInputData does not return correct size" << std::endl;
		return NULL;
	}
	return lpb;
}

// code referenced from Microsoft MSDN documentation:
// https://docs.microsoft.com/en-us/windows/win32/learnwin32/managing-application-state-#an-object-oriented-approach
LRESULT MouseHandler::StaticWinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	// this function allows for an object-oriented approach to implementing the WinProc callback function(s).
	// in this case, it is useful because I want two callback functions (one for recording, one for playback)
	// that each do different things with the user input data.
	MouseHandler *p_this = (MouseHandler*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (!p_this) {
		return DefWindowProc(hwnd, msg, w_param, l_param);
	}

	// calls the corresponding WndProc function for the current "mode"
	return p_this->ClassWinProc(msg, w_param, l_param);
}

void MouseHandler::setup() {
	// from my (basic) understanding, SetWindowLongPtr() allows for the storing of an object pointer within an application window
	// because instance data can be stored in each window. This comes in handy in the WinProc function above.
	SetWindowLongPtr(input_window, GWLP_USERDATA, (LONG_PTR)this);
	register_mouse();
	std::cout << "setup complete" << std::endl;
}

// starts basic message loop to receive messages about registered input devices from Windows
void MouseHandler::run() {
	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644928(v=vs.85).aspx#creating_loop
	while (true) {
		MSG message;
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			if (message.message == WM_QUIT) {
				break;
			} else {
				TranslateMessage(&message);
				// invokes WndProc callback function to process the message
				DispatchMessage(&message);
			}
		} else {
			break;
		}
	}
}
