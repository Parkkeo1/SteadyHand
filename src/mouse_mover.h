#pragma once
#ifndef M_MOVER_H
#define M_MOVER_H

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

// struct for loading a spray pattern from a file to be used to move the mouse.
struct PatternObject {
	// <unix timestamp (ms), dx, dy>
	std::vector<std::tuple<int, int, int>> movement_coords;
	// used in resetting the xhair back to center.
	int total_x_travel;
	int total_y_travel;

	PatternObject() : total_x_travel(0), total_y_travel(0) {};

public:
	int size() { return movement_coords.size(); }
};

extern const std::set<std::string> kWeaponNameCodes;

class MouseMover {

	std::string curr_weapon_name;
	PatternObject *curr_weapon;
	std::unordered_map<std::string, PatternObject> loaded_patterns;

	HWND mouse_mover_wind;
	bool is_m_left_down;

	void CreateHiddenWindow();
	void RegisterMouse();
	static LRESULT CALLBACK StaticWinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
	LRESULT MouseMoverProc(UINT msg, WPARAM w_param, LPARAM l_param);

	PatternObject LoadPatternFromFile(const std::string &filename);
	void LoadAllPatterns();
	void MouseSetup(INPUT *input_buffer);
	void MouseMove(INPUT *input_buffer, int x_delta, int y_delta);
	void MoveWithPattern();

public:
	MouseMover() : curr_weapon_name("weapon_ak47"), mouse_mover_wind(NULL), is_m_left_down(false) {};
	void SetupMover();
	void RunMover();
	void UpdateCurrPattern();
	std::string &get_curr_weap_name() { return curr_weapon_name; }
	void set_curr_weap_name(const std::string &new_weap_name) { curr_weapon_name = new_weap_name; }
	void set_hwnd_manually(HWND window_handle) { mouse_mover_wind = window_handle; }
};

#endif // !M_MOVER_H