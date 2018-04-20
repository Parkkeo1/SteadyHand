#pragma once
#ifndef MOUSE_RECORD_OBJ
#define MOUSE_RECORD_OBJ

#include <Windows.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <fstream>

enum Weapon {
	AK_47 = 0,
	M4_A4 = 1,
	M4_A1S = 2,
	UMP_45 = 3
};

const std::vector<std::string> kWeaponNames = {
	"ak_47",
	"m4_a4",
	"m4_als",
	"ump_45"
};

const LPCWSTR kClassName = TEXT("SteadyHand");

// constants for Windows API raw input device codes.
const short kDesktopUsage = 1;
const short kMouseUsage = 2;
const short kKeyBoardUsage = 6;

// default hotkey Windows Vkey codes.
const short kExitVKey = 18; // by default it is ALT.
const short kSaveVKey = 9; // by default it is TAB.

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

class MouseRecorderObj {
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

LRESULT CALLBACK WndProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

#endif // !MOUSE_RECORD_OBJ

