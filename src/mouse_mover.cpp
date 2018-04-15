#include "mouse_mover.h"

// method to setup the buffer before using SendInput.
void MouseSetup(INPUT *input_buffer) {
	input_buffer->type = INPUT_MOUSE;
	input_buffer->mi.dx = 0;
	input_buffer->mi.dy = 0;
	input_buffer->mi.mouseData = 0;
	input_buffer->mi.dwFlags = MOUSEEVENTF_MOVE;
	input_buffer->mi.time = 0;
	input_buffer->mi.dwExtraInfo = 0;
}

// code referenced from Microsoft MSDN:
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646310(v=vs.85).aspx
void MouseMove(INPUT *input_buffer, int x_delta, int y_delta) {
	input_buffer->mi.dx = x_delta;
	input_buffer->mi.dy = y_delta;
	input_buffer->mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, input_buffer, sizeof(INPUT));
}

// method that moves the mouse according to the weapon pattern.
void RecoilMoveWithPattern(const PatternObject &loaded_pattern) {
	INPUT m_input_buf;
	MouseSetup(&m_input_buf);

	for (auto xy_delta : loaded_pattern.movement_coords) {
		MouseMove(&m_input_buf, std::get<1>(xy_delta), std::get<2>(xy_delta));
		// to make sure the pattern is replicated with appropriate delays between coordinates.
		Sleep(std::get<0>(xy_delta));
	}
	Sleep(10);
	// resetting crosshair back to original position.
	MouseMove(&m_input_buf, -loaded_pattern.total_x_travel, -loaded_pattern.total_y_travel); 
}
