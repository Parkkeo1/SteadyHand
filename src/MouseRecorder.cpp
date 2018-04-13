#include "MouseRecorder.h"

void MouseRecorder::CreateHiddenWindow(HINSTANCE h_inst) {
	// code derived from:
	// https://stackoverflow.com/a/4081383
	WNDCLASSEX message_only;
	message_only.cbSize = sizeof(WNDCLASSEX);
	message_only.lpfnWndProc = DefWindowProc;
	message_only.hInstance = curr_instance;
	message_only.lpszClassName = "MouseRecord";
	
	if (RegisterClassEx(&message_only)) {
		hidden_window = CreateWindowEx(0, "MouseRecord", "MouseRecord", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
	}
}

void MouseRecorder::OutputThread(DWORD out_thread) {
	try {
		while (!is_finished_recording) {
			mouse_buffer.UpdateOutputBuffer();
			mouse_buffer.SaveOutputBufToFile("test.txt");
			// TODO: update is_finished_recording condition through GUI button: RECORD.
		}
	} catch (const std::exception &err) {
		std::cerr << err.what() << std::endl;
	}
}

void MouseRecorder::RegisterMouseDevice() {
	// settings options for registering 1 mouse.
	mouse_device.usUsagePage = 0x01;
	mouse_device.usUsage = 0x02;
	mouse_device.dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	mouse_device.hwndTarget = hidden_window;
	RegisterRawInputDevices(&mouse_device, 1, sizeof(mouse_device));
}

void MouseRecorder::ProcessRawMouseData(const RAWMOUSE &raw_mouse_data) {
	bool left_pressed;
	bool left_released;

	// translating raw button data
	switch (raw_mouse_data.ulButtons) {
		case 1:
			left_pressed = true;
			left_released = false;
			break;
		case 2:
			left_pressed = false;
			left_released = true;
		default:
			left_pressed = false;
			left_released = false;
	}
	// code derived from:
	// https://stackoverflow.com/a/19555298
	long long curr_time = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();

	MouseData new_m_data(curr_time, raw_mouse_data.lLastX, raw_mouse_data.lLastY, 
						 left_pressed, left_released);
	mouse_buffer.AddMouseData(new_m_data);
}

// code referenced from Microsoft MSDN:
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx
LRESULT MouseRecorder::WndProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
	if (message == WM_INPUT) {
		RAWINPUT raw_input;
		UINT msg_size;
		auto check_size = GetRawInputData((HRAWINPUT)l_param, RID_INPUT, &raw_input, &msg_size, sizeof(RAWINPUTHEADER));
		if (check_size != msg_size) {
			std::cout << "Received raw input was a correct size." << std::endl;
		} else if (raw_input.header.dwType == RIM_TYPEMOUSE) {
			// adds new mouse data after processing if valid.
			ProcessRawMouseData(raw_input.data.mouse);
		}
		return 0;
	}
	return DefWindowProc(window_handle, message, w_param, l_param);
}
