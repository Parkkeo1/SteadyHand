#include "MouseRecorder.h"

// method that receives messages sent by Windows to the given hidden/message-only window.
// code referenced from Microsoft MSDN documentation:
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx#standard_read
LRESULT CALLBACK WndProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
	switch (message) {
		case WM_INPUT: {
			// TODO: refactor this big function into smaller chunks.
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
				auto mouse_data = raw_input->data.mouse;

				// code derived from:
				// https://stackoverflow.com/a/19555298
				long long curr_time = std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::system_clock::now().time_since_epoch()
					).count();

				// recording new mouse movement as object.
				MouseData new_mdata(curr_time, static_cast<short>(mouse_data.lLastX), 
					static_cast<short>(mouse_data.lLastY), 
					static_cast<short>(mouse_data.usButtonFlags));
				mouse_recorder.get_mouse_data_list().push_back(new_mdata);

			} else if (raw_input->header.dwType == RIM_TYPEKEYBOARD && raw_input->data.keyboard.Flags == 0) {
				switch (raw_input->data.keyboard.VKey) {
				case kExitVKey: {
					std::cout << "ALT key was pressed; exiting." << std::endl;
					PostQuitMessage(0);
					break;
				} case kSaveVKey: {
					// saving the movements stored in the mouse data vector.
					mouse_recorder.WriteBufferToFile();
					std::cout << mouse_recorder.get_mouse_data_list().size() << " mouse movements saved." << std::endl;
					mouse_recorder.get_mouse_data_list().clear();
					break;
				} default: {
					std::cout << "undefined keypress." << std::endl;
				}
				}
			}
			break;
		} default: {
			break;
		}
	}
	return DefWindowProc(window_handle, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE h_inst, HINSTANCE h_prev_inst, LPSTR lp_cmd_line, int n_cmd_show) {
	bool is_recording = true;
	Weapon ump_weap = UMP_45;

	mouse_recorder.set_curr_weapon(ump_weap);
	// TODO: Replace this with actual stop condition, probably using ENUM states.
	mouse_recorder.RunMouseRecorder(h_inst, is_recording);
	return 0;
}