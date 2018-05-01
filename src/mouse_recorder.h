#pragma once
#ifndef M_RECORDER
#define M_RECORDER

#include "mouse_handler.h"

// struct for storing mouse data from Windows raw input messages.
struct MouseData {
	long long ms_time;
	short dx;
	short dy;
	short mleft_code; // 1 if m_left was pressed down, 2 if m_left was released.
public:
	MouseData(const long long curr_time, const short x_delta, 
			  const short y_delta, const short m_left_status) : ms_time(curr_time),
			  dx(x_delta),
			  dy(y_delta),
			  mleft_code(m_left_status) {};
	friend std::ostream& operator<<(std::ostream& os, const MouseData& m_data);
};

class MouseRecorder : public MouseHandler {
	std::vector<MouseData> mouse_data_buf;

	void AddNewMouseData(const RAWMOUSE &m_data);
	LRESULT ClassWinProc(UINT msg, WPARAM w_param, LPARAM l_param);

public:
	void WriteBufferToFile();
	std::vector<MouseData> &get_mouse_data_buf() { return mouse_data_buf; }
};

#endif // !MOUSE_RECORD_OBJ

