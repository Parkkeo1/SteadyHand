#include "MouseRecorder.h"

// global variable is usually bad practice but seems to be necessary (until I can find a better fix).
// WndProc cannot be a class function, but it needs to have access to a MouseRecorder object.
MouseRecorder record_app;

// WndProc cannot be a class function.
// code referenced from Microsoft MSDN:
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx
LRESULT CALLBACK WndProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
	if (message == WM_INPUT) {
		RAWINPUT raw_input;
		UINT msg_size;
		auto check_size = GetRawInputData((HRAWINPUT)l_param, RID_INPUT, &raw_input, &msg_size, sizeof(RAWINPUTHEADER));
		if (check_size != msg_size) {
			std::cout << "Received raw input was a correct size." << std::endl;
		}
		else if (raw_input.header.dwType == RIM_TYPEMOUSE) {
			// adds new mouse data after processing if valid.
			record_app.ProcessRawMouseData(raw_input.data.mouse);
		}
		return 0;
	}
	return DefWindowProc(window_handle, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE h_inst, HINSTANCE h_ignore, LPSTR lp_ignore, int n_cmd_show) {
	try {
		record_app.InitRecorder(h_inst);
		record_app.RunMouseRecorder("test.txt");

	}
	catch (const std::exception &error) {
		std::cerr << error.what() << std::endl;
		return -1;
	}
	return 0;
}