#include "mouse_mover.h"

const short kResetDelay = 500;
const short kManualDelay = 1;

const std::set<std::string> kWeaponNameCodes = {
	"weapon_ak47",
	"weapon_m4a1",
	"weapon_m4a1_silencer"
};

void MouseMover::LoadAllPatterns() {
	for (auto &weapon_name : kWeaponNameCodes) {
		loaded_patterns.insert({ weapon_name, LoadPatternFromFile("patterns/" + weapon_name + ".txt") });
	}
	// setting defaults.
	curr_weapon_name = *kWeaponNameCodes.begin();
	try {
		curr_weapon = &loaded_patterns.at(curr_weapon_name);
	} catch (const std::exception) {
		std::cout << "the pattern map was not initialized correctly." << std::endl;
	}
}

// loading new pattern according to newly equipped weapon.
void MouseMover::UpdateCurrPattern() {
	curr_weapon = &loaded_patterns.at(curr_weapon_name);
}

LRESULT MouseMover::ClassWinProc(UINT msg, WPARAM w_param, LPARAM l_param) {
	switch (msg) {
		case WM_INPUT: {
			LPBYTE processed_msg = CheckMessageSize(l_param);
			if (processed_msg == LPBYTE()) {
				return -1;
			}
			// size of message/data has been confirmed to be correct.
			RAWINPUT* raw_input = (RAWINPUT*)processed_msg;

			if (raw_input->header.dwType == RIM_TYPEMOUSE) {
				switch (raw_input->data.mouse.usButtonFlags) {
					case 1: {
						is_m_left_down = true;
						std::thread pattern_thread = std::thread(MoveWithPattern, curr_weapon, std::ref(is_m_left_down));
						pattern_thread.detach();
						break;
					} case 2: {
						is_m_left_down = false;
						break;
					} default: {
						break;
					}
				}
			}

			if (raw_input->header.dwType == RIM_TYPEKEYBOARD && raw_input->data.keyboard.Flags == 0) {
				if (raw_input->data.keyboard.VKey == VirtualKeys::ENTER) {
					std::cout << "ENTER key pressed." << std::endl;
					// TODO: Fix this message loop to exit properly when user presses ENTER.
					// Currently this exits the entire program.
					PostQuitMessage(0);
				}
			}
			break;
		} default:
			return DefWindowProc(input_window, msg, w_param, l_param);
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

			spray_pattern.push_back({ std::stoll(xy_info[0]) - prev_time, std::stoi(xy_info[1]), std::stoi(xy_info[2]) });
			prev_time = std::stoll(xy_info[0]);
		}
	} else {
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
void MouseMover::MoveWithPattern(const PatternObject *loaded_pattern, std::atomic<bool> &is_firing) {
	INPUT m_input_buf;
	MouseSetup(&m_input_buf);

	int x_total_dist = 0;
	int y_total_dist = 0;

	for (auto xy_delta : *loaded_pattern) {
		if (is_firing) {
			MouseMove(&m_input_buf, std::get<1>(xy_delta), std::get<2>(xy_delta));
			x_total_dist += std::get<1>(xy_delta);
			y_total_dist += std::get<2>(xy_delta);
			Sleep(kManualDelay);
		} else {
			break;
		}
	}
	Sleep(kResetDelay);
	// resetting crosshair back to original position.
	MouseMove(&m_input_buf, -x_total_dist, -y_total_dist);
}
