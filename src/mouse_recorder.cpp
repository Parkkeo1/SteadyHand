#include "mouse_recorder.h"

std::ostream& operator<<(std::ostream &os, const MouseData &m_data) {
	os << m_data.ms_time << " " << m_data.dx << " " << m_data.dy 
	   << " " << m_data.mleft_code << std::endl;
	return os;
}

void MouseRecorder::AddNewMouseData(const RAWMOUSE &m_data) {
	// code derived from:
	// https://stackoverflow.com/a/19555298
	long long curr_time = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();

	MouseData new_mdata(curr_time, static_cast<short>(m_data.lLastX),
						static_cast<short>(m_data.lLastY),
						static_cast<short>(m_data.usButtonFlags));
	mouse_data_buf.push_back(new_mdata);
}

void MouseRecorder::WriteBufferToFile() {
	if (curr_weapon_name != kInactive) {
		std::string filename = "patterns/" + curr_weapon_name + ".txt";
		std::ofstream pattern_file(filename);
		int save_counter = 0;

		// keeping track of whether/when to save the mouse data.
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
		std::cout << save_counter << " / " << mouse_data_buf.size() << " mouse movements saved." << std::endl;
		pattern_file.close();
	} else {
		std::cout << "No Weapon Detected. Did not save to file." << std::endl;
	}
}

LRESULT MouseRecorder::ClassWinProc(UINT msg, WPARAM w_param, LPARAM l_param) {
	switch (msg) {
		case WM_INPUT: {
			LPBYTE processed_msg = CheckMessageSize(l_param);
			if (processed_msg == LPBYTE()) {
				return -1;
			}
			// size of message/data has been confirmed to be correct.
			RAWINPUT* raw_input = (RAWINPUT*)processed_msg;

			if (raw_input->header.dwType == RIM_TYPEMOUSE) {
				AddNewMouseData(raw_input->data.mouse);
			}

			if (raw_input->header.dwType == RIM_TYPEKEYBOARD && raw_input->data.keyboard.Flags == 0) {
				switch (raw_input->data.keyboard.VKey) {
					case VirtualKeys::ENTER: {
						std::cout << "ENTER key pressed." << std::endl;
						// TODO: Fix this message loop to exit properly when user presses ENTER.
						// Currently this exits the entire program.
						PostQuitMessage(0);
						break;
					} case VirtualKeys::SAVE: {
						WriteBufferToFile();
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
