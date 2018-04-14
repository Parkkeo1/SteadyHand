#pragma once
#ifndef MOUSERECORDER_H
#define MOUSERECORDER_H

#include "MouseData.h"

class MouseRecorder {
	BufferData mouse_buffer;
	bool is_finished_recording;
	HINSTANCE curr_instance;
	RAWINPUTDEVICE mouse_device;
	HWND hidden_window;

public:
	MouseRecorder() : mouse_buffer(), is_finished_recording(false) {};

	void InitRecorder(HINSTANCE h_inst);
	void CreateHiddenWindow();
	void OutputThread();
	void RegisterMouseDevice();
	void ProcessRawMouseData(const RAWMOUSE &raw_mouse_data);
	void RunMouseRecorder(const std::string &filename);
};

#endif
