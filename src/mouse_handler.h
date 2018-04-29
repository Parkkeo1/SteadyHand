#pragma once
#ifndef M_HANDLER
#define M_HANDLER

#include <Windows.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <tuple>
#include <set>
#include <unordered_map>
#include <thread>
#include <functional>
#include <atomic>

extern const std::string kInactive;

class MouseHandler {

protected:
	HWND input_window;
	std::string curr_weapon_name;

	void RegisterMouse();

	static LRESULT CALLBACK StaticWinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
	virtual LRESULT ClassWinProc(UINT msg, WPARAM w_param, LPARAM l_param) = 0;

public:
	MouseHandler() : curr_weapon_name(kInactive), input_window(NULL) {}

	std::string &get_curr_weap_name() { return curr_weapon_name; }
	void set_curr_weap_name(const std::string &new_weap_name) { curr_weapon_name = new_weap_name; }
	void set_hwnd_manually(HWND window_handle) { input_window = window_handle; }

	void Setup();
	void Run();
};

#endif // !M_HANDLER

