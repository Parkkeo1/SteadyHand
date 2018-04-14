#include "MouseRecorder.h"
#include "main.h"

void MouseRecorder::InitRecorder(HINSTANCE h_inst) {
	this->curr_instance = h_inst;
	this->RegisterMouseDevice();
	this->CreateHiddenWindow();
}

void MouseRecorder::CreateHiddenWindow() {
	// code derived from:
	// https://stackoverflow.com/a/4081383
	WNDCLASS message_only;
	message_only.lpfnWndProc = WndProc;
	message_only.hInstance = curr_instance;
	message_only.lpszClassName = "MouseRecord";
	
	if (RegisterClass(&message_only)) {
		this->hidden_window = CreateWindow("MouseRecord", "MouseRecord", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, curr_instance, NULL);
	}
}

void MouseRecorder::OutputThread() {
	try {
		std::ofstream file_stream("test.txt");
		bool to_save_curr = false;
		while (!is_finished_recording) { 
			// TODO: update is_finished_recording condition through GUI button: RECORD.
			mouse_buffer.UpdateOutputBuffer();
			mouse_buffer.SaveBufferToFile(to_save_curr, file_stream);
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
	mouse_buffer.AddMouseDataToBuf(new_m_data);
}

void MouseRecorder::RunMouseRecorder(const std::string &filename) {
	std::thread out_thread(&MouseRecorder::OutputThread, this);

	// code referenced from Microsoft MSDN:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644936(v=vs.85).aspx
	BOOL msg_code;
	MSG message;
	while ((msg_code = GetMessage(&message, hidden_window, 0, 0)) != 0) {
		if (msg_code == -1) {
			// handle the error and possibly exit
		} else {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
	out_thread.join();
}
