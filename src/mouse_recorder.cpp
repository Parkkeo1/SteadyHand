#include "mouse_recorder.h"

std::ostream& operator<<(std::ostream &os, const MouseData &m_data) {
	os << m_data.ms_time << " " << m_data.dx << " " << m_data.dy 
	   << " " << m_data.mleft_code << std::endl;
	return os;
}

void RegisterInputDevices(HWND window) {
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
	baseWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
	std::cout << "Device registration successful." << std::endl;
}
		
bool CreateHiddenWindow(HINSTANCE &h_inst, HWND &window_handle) {
	WNDCLASS hid_wind_class;

	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/ms633576(v=vs.85).aspx
	hid_wind_class.style = 0;
	hid_wind_class.lpfnWndProc = DefWindowProc;
	hid_wind_class.cbClsExtra = 0;
	hid_wind_class.cbWndExtra = 0;
	hid_wind_class.hInstance = h_inst;
	hid_wind_class.hIcon = NULL;
	hid_wind_class.hCursor = NULL;
	hid_wind_class.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	hid_wind_class.lpszMenuName = NULL;
	hid_wind_class.lpszClassName = "test";

	if (RegisterClass(&hid_wind_class)) {
		window_handle = CreateWindow("test", "test", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, h_inst, NULL);
		std::cout << "Window registration successful." << std::endl;
		return true;		
	}
	std::cout << "Registration of the window class failed." << std::endl;
	return false;
}

void WriteBufferToFile(const Weapon weapon_to_save) {
	std::string filename = weapon_names[weapon_to_save] + ".txt";
	std::ofstream pattern_file(filename);

	// booleans for keeping track of mouse's left button.
	bool was_left_pressed = false;
	bool was_right_pressed = false;

	for (MouseData m_data : mouse_data_list) {
		// TODO: keep track of bools and save to file as appropriate.
	}
}

// method that receives messages sent by Windows to the given hidden/message-only window.
// code referenced from Microsoft MSDN documentation:
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx#standard_read
LRESULT CALLBACK WndProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
	switch (message) {
		case WM_INPUT: {
			UINT dw_size;
			GetRawInputData((HRAWINPUT)l_param, RID_INPUT, NULL, &dw_size, sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dw_size];
			if (lpb == NULL) {
				return -1;
			}

			if (GetRawInputData((HRAWINPUT)l_param, RID_INPUT, lpb, &dw_size, sizeof(RAWINPUTHEADER)) != dw_size) {
				std::cout << "GetRawInputData does not return correct size !" << std::endl;
				return -1;
			}

			// size of message/data has been confirmed to be correct.
			RAWINPUT* raw_input = (RAWINPUT*)lpb;

			// detecting type of raw input.
			if (raw_input->header.dwType == RIM_TYPEMOUSE) {
				auto mouse_data = raw_input->data.mouse;

				// code derived from:
				// https://stackoverflow.com/a/19555298
				long long curr_time = std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::system_clock::now().time_since_epoch()
				).count();

				// processing & storing this raw data is done when user stops recording.
				MouseData new_mdata(curr_time, static_cast<short>(mouse_data.lLastX), 
					                static_cast<short>(mouse_data.lLastY), 
					                static_cast<short>(mouse_data.usButtonFlags));
				mouse_data_list.push_back(new_mdata);

			} else if (raw_input->header.dwType == RIM_TYPEKEYBOARD) {
				switch (raw_input->data.keyboard.VKey) {
					case kExitVKey: {
						std::cout << "ALT key was pressed; exiting." << std::endl;
						PostQuitMessage(0);
						break;
					} case kSaveVKey: { // TODO: save instead of just printing. Save m_data for only when m1 was down.
						std::cout << "clearing vector." << std::endl;
						mouse_data_list.clear();
						break;
					} default: {
						std::cout << "undefined keypress." << std::endl;
					}
				}
			}
			break;
		} default:
			return DefWindowProc(window_handle, message, w_param, l_param);
	}
}

void RunMouseRecorder(HINSTANCE &h_inst) {
	HWND window_handle;
	CreateHiddenWindow(h_inst, window_handle);
	RegisterInputDevices(window_handle);

	std::cout << "setup complete." << std::endl;
	MSG message;
	BOOL msg_code;

	// The MS MSDN-recommended way to do message loops:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644928(v=vs.85).aspx#creating_loop
	while ((msg_code = GetMessage(&message, NULL, 0, 0)) != 0) {
		if (msg_code == -1) {
			return;
		} else {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}

int WINAPI WinMain(HINSTANCE h_inst, HINSTANCE h_prev_inst, LPSTR lp_cmd_line, int n_cmd_show) {
	// TODO: Replace this with actual stop condition, probably using ENUM states.
	RunMouseRecorder(h_inst);
	return 0;
}
