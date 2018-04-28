#include "mouse_mover.h"

const LPCWSTR kClassName = TEXT("SteadyHand");

// constants for Windows API raw input device codes.
const short kDesktopUsage = 1;
const short kMouseUsage = 2;
const short kKeyBoardUsage = 6;

// Windows's virtual keyboard key for the ENTER key.
const short kEnterVkey = 13;

const short kResetDelay = 1000;
const short kM4Delay = 1.5;

const std::set<std::string> kWeaponNameCodes = {
	"weapon_ak47",
	"weapon_m4a1",
	"weapon_m4a1_silencer"
};

void MouseMover::RegisterMouse() {
	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx#example_2
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = kDesktopUsage;
	Rid[0].usUsage = kMouseUsage;
	Rid[0].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[0].hwndTarget = mouse_mover_wind;

	Rid[1].usUsagePage = kDesktopUsage;
	Rid[1].usUsage = kKeyBoardUsage;
	Rid[1].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[1].hwndTarget = mouse_mover_wind;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]))) {
		SetWindowLongPtr(mouse_mover_wind, GWLP_WNDPROC, (LONG_PTR)StaticWinProc);
		std::cout << "Device registration successful." << std::endl;
	} else {
		std::cout << "Device registration failed." << std::endl;
	}
}

// code derived from:
// https://stackoverflow.com/a/28491549
LRESULT MouseMover::StaticWinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	MouseMover *p_this = (MouseMover*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (!p_this) {
		return DefWindowProc(hwnd, msg, w_param, l_param);
	}

	return p_this->MouseMoverProc(msg, w_param, l_param);
}

LRESULT MouseMover::MouseMoverProc(UINT msg, WPARAM w_param, LPARAM l_param) {
	switch (msg) {
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

		if (raw_input->header.dwType == RIM_TYPEMOUSE) {
			switch (raw_input->data.mouse.usButtonFlags) {
			case 1:
				is_m_left_down = true;
				std::cout << "m_left pressed." << std::endl;
				MoveWithPattern();
				break;
			case 2:
				is_m_left_down = false;
				std::cout << "m_left released." << std::endl;
				break;
			default:
				break;
			}
		}

		if (raw_input->header.dwType == RIM_TYPEKEYBOARD && raw_input->data.keyboard.Flags == 0) {
			if (raw_input->data.keyboard.VKey == kEnterVkey) {
				std::cout << "ENTER key pressed." << std::endl;
				// TODO: Fix this message loop to exit properly when user presses ENTER.
				// Currently this exits the entire program... it should only change the state of the program.
				PostQuitMessage(0);
			}
		}
		break;
	} default:
		return DefWindowProc(mouse_mover_wind, msg, w_param, l_param);
	}

	return 0;
}

// method that parses a given file to load a spray pattern.
PatternObject MouseMover::LoadPatternFromFile(const std::string &filename) {
	PatternObject spray_pattern;
	std::ifstream pattern_file(filename);
	std::string file_line;

	if (pattern_file.is_open()) {
		long long prev_time = 0;
		bool is_first_line = true;

		while (getline(pattern_file, file_line)) {
			std::istringstream in_sstream(file_line);
			std::vector<std::string> xy_info(std::istream_iterator<std::string>{in_sstream},
				std::istream_iterator<std::string>());
			if (is_first_line) {
				prev_time = std::stoll(xy_info[0]);
				is_first_line = false;
			}

			spray_pattern.movement_coords.push_back({ std::stoll(xy_info[0]) - prev_time, std::stoi(xy_info[1]), std::stoi(xy_info[2]) });
			spray_pattern.total_x_travel += std::stoi(xy_info[1]);
			spray_pattern.total_y_travel += std::stoi(xy_info[2]);
			prev_time = std::stoll(xy_info[0]);
		}
	}
	else {
		std::cout << "file not found" << std::endl;
	}
	return spray_pattern;
}

// method to setup the buffer before using SendInput.
void MouseMover::MouseSetup(INPUT *input_buffer) {
	input_buffer->type = INPUT_MOUSE;
	input_buffer->mi.dx = 0;
	input_buffer->mi.dy = 0;
	input_buffer->mi.mouseData = 0;
	input_buffer->mi.dwFlags = MOUSEEVENTF_MOVE;
	input_buffer->mi.time = 0;
	input_buffer->mi.dwExtraInfo = 0;
}

// code referenced from Microsoft MSDN:
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646310(v=vs.85).aspx
void MouseMover::MouseMove(INPUT *input_buffer, int x_delta, int y_delta) {
	input_buffer->mi.dx = x_delta;
	input_buffer->mi.dy = y_delta;
	input_buffer->mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, input_buffer, sizeof(INPUT));
}

// method that moves the mouse according to the weapon pattern.
void MouseMover::MoveWithPattern() {
	INPUT m_input_buf;
	MouseSetup(&m_input_buf);

	for (auto xy_delta : curr_weapon->movement_coords) {
		if (is_m_left_down) {
			MouseMove(&m_input_buf, std::get<1>(xy_delta), std::get<2>(xy_delta));
			Sleep(kM4Delay);
		} else {
			return;
		}
	}
	Sleep(kResetDelay);
	// resetting crosshair back to original position.
	MouseMove(&m_input_buf, -curr_weapon->total_x_travel, -curr_weapon->total_y_travel);
}

void MouseMover::LoadAllPatterns() {
	for (auto &weapon_name : kWeaponNameCodes) {
		loaded_patterns.insert({ weapon_name, LoadPatternFromFile("patterns/" + weapon_name + ".txt") });
	}
	// setting defaults.
	curr_weapon_name = *kWeaponNameCodes.begin();
	try {
		curr_weapon = &loaded_patterns.at(curr_weapon_name);
	}
	catch (const std::exception) {
		std::cout << "the pattern map was not initialized correctly." << std::endl;
	}
}

void MouseMover::SetupMover() {
	SetWindowLongPtr(mouse_mover_wind, GWLP_USERDATA, (LONG_PTR)this);
	RegisterMouse();
	std::cout << "setup complete." << std::endl;
}

void MouseMover::RunMover() {
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

// loading new pattern according to newly equipped weapon.
void MouseMover::UpdateCurrPattern() {
	curr_weapon = &loaded_patterns.at(curr_weapon_name);
}
