#include "mouse_recorder.h"

std::ostream& operator<<(std::ostream &os, const MouseData &m_data) {
	os << m_data.ms_time << " " << m_data.dx << " " << m_data.dy 
	   << " " << m_data.mleft_code << std::endl;
	return os;
}

void MouseRecorder::add_new_mouse_data(const RAWMOUSE &m_data) {
	// code derived from:
	// https://stackoverflow.com/a/19555298
	// calculate timestamp of current data point to calculate delay between data points later
	long long curr_time = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();

	mouse_data_buf.emplace_back(curr_time, static_cast<short>(m_data.lLastX),
								static_cast<short>(m_data.lLastY),
								static_cast<short>(m_data.usButtonFlags)
							   );
}

void MouseRecorder::write_buffer_to_file() {
	if (curr_weapon_name != inactive_weapon) {
		// I am saving patterns as .txt files for the sake of simplicity during development (especially given when I first developed SteadyHand).
		// Using a text format does slow down my program startup performance, but it should not make a difference during playback/aimbot mode
		// as all patterns are loaded from files into memory only once on startup.
		// If I wanted to prioritize performance more, I could store the mouse pattern data in a binary format to speed up reads/writes.
		std::string filename = "patterns/" + curr_weapon_name + ".txt";
		std::ofstream pattern_file(filename);
		int save_counter = 0;

		// keeping track of whether/when to save the mouse data- only when the user has the M1 button down on his/her mouse.
		bool to_save_curr = false;
		for (MouseData m_data : mouse_data_buf) {
			switch (m_data.mleft_code) {
				case 1:
					to_save_curr = true;
					break;
				case 2:
					to_save_curr = false;
					break;
				default:
					break;
			}

			if (to_save_curr) {
				pattern_file << m_data;
				save_counter++;
			}
		}

		// For development/debugging purposes. Shows how SteadyHand only saves mouse movements that should be recorded (when the user holds down M1 and fires the weapon in-game)
		std::cout << save_counter << " / " << mouse_data_buf.size() << " mouse movements saved." << std::endl;
		pattern_file.close();
	} else {
		std::cout << "No Weapon Detected. Did not save to file." << std::endl;
	}
}

// This MouseRecorder-specific callback function saves mouse data in order to aggregate them into "patterns" for given weapons.
LRESULT MouseRecorder::ClassWinProc(UINT msg, WPARAM w_param, LPARAM l_param) {
	switch (msg) {
		case WM_INPUT: {
			LPBYTE processed_msg = check_message_size(l_param);
			if (processed_msg == NULL) {
				return -1;
			}
			// size of message/data has been confirmed to be correct by check_message_size() above.
			RAWINPUT* raw_input = (RAWINPUT*)processed_msg;

			// if mouse input data, add it to the current buffer.
			if (raw_input->header.dwType == RIM_TYPEMOUSE) {
				add_new_mouse_data(raw_input->data.mouse);
			}

			// Keyboard hotkeys are available for the user during the recording mode in order to save/clear the mouse pattern buffer and/or exit SteadyHand.
			// This allows user to control the program without clicking/moving the mouse (which would be recorded in this mode).
			if (raw_input->header.dwType == RIM_TYPEKEYBOARD && raw_input->data.keyboard.Flags == 0) {
				switch (raw_input->data.keyboard.VKey) {
					case VirtualKeys::ENTER: {
						std::cout << "Exiting." << std::endl;
						// Exit program.
						PostQuitMessage(0);
						break;
					} case VirtualKeys::SAVE: {
						// When user prompts to save the current recorded buffer, save the data points to a pattern file.
						write_buffer_to_file();
						mouse_data_buf.clear();
						std::cout << "Mouse Data Buffer Cleared." << std::endl;
						break;
					} case VirtualKeys::CLEAR: {
						mouse_data_buf.clear();
						std::cout << "Mouse Data Buffer Cleared. Not Saved." << std::endl;
						break;
					} default: {
						break;
					}
				}
			}
			break;
		} default:
			return DefWindowProc(input_window, msg, w_param, l_param);
	}

	return 0;
}
