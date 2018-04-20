#pragma once

#include <Windows.h>

#include <vector>
#include <iostream>
#include <chrono>
#include <tuple>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>

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

// struct for loading a spray pattern from a file to be used to move the mouse.
struct PatternObject {
	Weapon name;
	// <unix timestamp (ms), dx, dy>
	std::vector<std::tuple<int, int, int>> movement_coords;
	// used in resetting the xhair back to center.
	int total_x_travel;
	int total_y_travel;

	PatternObject(Weapon name) : name(name), total_x_travel(0), total_y_travel(0) {};
};