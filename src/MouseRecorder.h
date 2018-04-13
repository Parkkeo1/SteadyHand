#pragma once
#include "MouseData.h"

class MouseRecorder {
	BufferData mouse_buffer;
	bool is_finished_recording;
	RAWINPUTDEVICE mouse_device;

	void OutputThread(DWORD out_thread);
	void RegisterMouseDevice(); // NOTE: May need to set window (hidden window).
};
