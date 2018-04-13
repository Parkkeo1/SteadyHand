#include "MouseRecorder.h"

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
	mouse_device.hwndTarget = NULL;
	RegisterRawInputDevices(&mouse_device, 1, sizeof(devices[0]));
}
