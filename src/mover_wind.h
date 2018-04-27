#pragma once
#ifndef MOVER_WIND
#define MOVER_WIND

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

class MouseMoverWindHandler {
	bool is_m_left_down;

	void RegisterInputDevices(HWND window);
	bool CreateHiddenWindow(HWND &window_handle);
	void RunMouseMoverLoop();
};

#endif // !MOVER_WIND
