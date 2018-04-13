#pragma once
#include "MouseData.h"

class MouseRecorder {
	BufferData mouse_buffer;
	bool is_finished_recording;
	HINSTANCE curr_instance;
	RAWINPUTDEVICE mouse_device;
	HWND hidden_window;

	void MouseRecorder::CreateHiddenWindow(HINSTANCE h_inst);
	void OutputThread(DWORD out_thread);
	void RegisterMouseDevice(); // NOTE: May need to set window (hidden window).
	void ProcessRawMouseData(const RAWMOUSE &raw_mouse_data);
	LRESULT CALLBACK WndProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);
};
