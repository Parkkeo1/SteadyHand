#pragma once
#include "MouseData.h"

class MouseRecorder {
	BufferData mouse_buffer;
	bool is_finished_recording;

	void OutputThread(DWORD out_thread) {
		try {

			while (!is_finished_recording) {
				mouse_buffer.UpdateOutputBuffer();
				mouse_buffer.SaveOutputBufToFile("test.txt");
				// update is_finished_recording condition.
			}
		} catch (const std::exception &err) {
			std::cerr << err.what() << std::endl;
		}
	}
};
