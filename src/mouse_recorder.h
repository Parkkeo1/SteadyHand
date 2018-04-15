#pragma once
#include <windows.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <tuple>

#include "pattern_parser.h"

// struct for storing mouse data from Windows raw input messages.
struct MouseData {
	long long ms_time;
	short dx;
	short dy;
	short mleft_code;
public:
	MouseData(const long long curr_time, const short x_delta, 
		      const short y_delta, const short m_left_status) : ms_time(curr_time),
		                                                        dx(x_delta),
		                                                        dy(y_delta),
		                                                        mleft_code(m_left_status) {};
	friend std::ostream& operator<<(std::ostream& os, const MouseData& m_data);
};

// constants for Windows API raw input device codes.
const short kDesktopUsage = 1;
const short kMouseUsage = 2;
const short kKeyBoardUsage = 6;

// default hotkey Windows Vkey codes.
const short kExitVKey = 18; // by default it is ALT.
const short kSaveVKey = 9; // by default it is TAB.

// global variables necessary in handling windows and storing message data.
std::vector<MouseData> mouse_data_list;
WNDPROC baseWndProc;

// TODO: Add data filtering and file writing functions.
void RegisterInputDevices(HWND window);
bool CreateHiddenWindow(HINSTANCE &hInstance, HWND &window_handle);
void WriteBufferToFile(const Weapon weapon_to_save);
LRESULT CALLBACK WndProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

void RunMouseRecorder(HINSTANCE &h_inst);

