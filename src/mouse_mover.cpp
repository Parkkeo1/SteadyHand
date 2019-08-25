#include "mouse_mover.h"

void MouseMover::load_all_patterns() {
	// for all weapons, load in their saved "patterns"
	for (auto &weapon_name : weapon_names) {
		// for development purposes: read .txt files from hardcoded patterns/ directory
		loaded_patterns.insert({ weapon_name, load_pattern_from_file("patterns/" + weapon_name + ".txt") });
	}

	// setting defaults.
	curr_weapon_name = *weapon_names.begin();
	try {
		curr_weapon = &loaded_patterns.at(curr_weapon_name);
	} catch (const std::exception) {
		std::cout << "the pattern map was not initialized correctly." << std::endl;
	}
}

// loads new pattern according to newly equipped weapon (after curr_weapon_name has recently been updated)
void MouseMover::update_current_pattern() {
	curr_weapon = &loaded_patterns.at(curr_weapon_name);
}

LRESULT MouseMover::ClassWinProc(UINT msg, WPARAM w_param, LPARAM l_param) {
	switch (msg) {
		case WM_INPUT: {
			LPBYTE processed_msg = check_message_size(l_param);
			if (processed_msg == NULL) {
				return -1;
			}
			// size of message/data has been confirmed to be correct.
			RAWINPUT* raw_input = (RAWINPUT*)processed_msg;

			if (raw_input->header.dwType == RIM_TYPEMOUSE) {
				switch (raw_input->data.mouse.usButtonFlags) {
					case 1: {
						is_m_left_down = true;
						// move mouse using a separate thread in order to not block rest of the program while actively using the aimbot
						std::thread pattern_thread = std::thread(move_with_pattern, curr_weapon, std::ref(is_m_left_down));
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
					std::cout << "Exiting." << std::endl;
					// Exits program.
					PostQuitMessage(0);
				}
			}
			break;
		} default:
			return DefWindowProc(input_window, msg, w_param, l_param);
	}

	return 0;
}

// method that parses a given file to create a PatternObject
PatternObject MouseMover::load_pattern_from_file(const std::string &filename) {
	PatternObject spray_pattern;
	std::ifstream pattern_file(filename);
	std::string file_line;

	if (pattern_file.is_open()) {
		long long prev_time = 0;
		bool is_first_line = true;

		// read line-by-line the saved data file and add to the PatternObject.
		while (getline(pattern_file, file_line)) {
			std::istringstream in_sstream(file_line);
			std::vector<std::string> xy_info(std::istream_iterator<std::string>{in_sstream},
				std::istream_iterator<std::string>());
			if (is_first_line) {
				prev_time = std::stoll(xy_info[0]);
				is_first_line = false;
			}

			int delay_time = std::stoll(xy_info[0]);

			// save x,y coordinates AND delay since last data point
			spray_pattern.push_back({ delay_time - prev_time, std::stoi(xy_info[1]), std::stoi(xy_info[2]) });
			prev_time = delay_time;
		}
	} else {
		std::cout << "file not found" << std::endl;
	}

	return spray_pattern;
}

// method to setup the buffer before using SendInput.
void MouseMover::mouse_setup(INPUT *input_buffer) {
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
void MouseMover::mouse_move(INPUT *input_buffer, int x_delta, int y_delta) {
	input_buffer->mi.dx = x_delta;
	input_buffer->mi.dy = y_delta;
	input_buffer->mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, input_buffer, sizeof(INPUT)); // Win32 API function to move mouse
}

// method that moves the mouse according to the weapon pattern, automatically controlling the recoil of the weapon in-game in real-time.
void MouseMover::move_with_pattern(const PatternObject *loaded_pattern, std::atomic<bool> &is_firing) {
	INPUT m_input_buf;
	mouse_setup(&m_input_buf);

	int x_total_dist = 0;
	int y_total_dist = 0;

	for (auto xy_delta : *loaded_pattern) {
		// while firing (and this condition is checked every iteration so that SteadyHand stops moving the mouse once user stops firing the weapon),
		// execute the current weapon's saved pattern in order and thus control the weapon's recoil in-game.
		if (is_firing) {
			mouse_move(&m_input_buf, std::get<1>(xy_delta), std::get<2>(xy_delta));
			x_total_dist += std::get<1>(xy_delta);
			y_total_dist += std::get<2>(xy_delta);

			// To mimic the real-life delay between different mouse coordinates when a human is moving a mouse, 
			// implement a delay between each mouse movement to make sure the pattern is not executed too quickly;
			// the aimbot must correctly match the recoil of the weapon being fired in-game in real-time, ideally bullet-by-bullet.

			// Disclaimer: Due to hidden/implicit delays between each iteration of the loop, 
			// it is possible for this loop to "fall behind" the firing rate of the weapon in-game.
			// Sometimes better to just implement a constant delay between the data points.
			Sleep(std::get<0>(xy_delta));
			// Sleep(1);
		} else {
			break;
		}
	}
	Sleep(xHairResetDelay);
	// resetting crosshair back to original position.
	mouse_move(&m_input_buf, -x_total_dist, -y_total_dist);
}
