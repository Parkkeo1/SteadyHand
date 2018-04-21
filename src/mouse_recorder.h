#pragma once
#ifndef MOUSE_RECORD_OBJ
#define MOUSE_RECORD_OBJ

#include <Windows.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <tuple>

extern const std::vector<std::string> kWeaponNames;

enum Weapon {
	AK_47 = 0,
	M4_A4 = 1,
	M4_A1S = 2,
	UMP_45 = 3
};

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

class MouseDataRecorder {
	std::vector<MouseData> mouse_data_list;
	Weapon curr_weapon;
	WNDPROC base_wnd_proc;

public:
	std::vector<MouseData> &get_mouse_data_list() { return mouse_data_list; }
	Weapon &get_curr_weapon() { return curr_weapon; }
	WNDPROC &get_base_wnd_proc() { return base_wnd_proc; }

	void set_curr_weapon(Weapon new_curr_weapon) { curr_weapon = new_curr_weapon; }

	void RegisterInputDevices(HWND window);
	bool CreateHiddenWindow(HINSTANCE &h_instance, HWND &window_handle);
	void WriteBufferToFile();
	void RunMouseRecorder(HINSTANCE &h_instance, bool &is_recording);
};

extern MouseDataRecorder mouse_recorder;

LRESULT CALLBACK WndProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

#endif // !MOUSE_RECORD_OBJ

