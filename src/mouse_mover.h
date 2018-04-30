#pragma once
#ifndef M_MOVER_H
#define M_MOVER_H

#include "mouse_handler.h"

extern const std::set<std::string> kWeaponNameCodes;

// struct for loading a spray pattern from a file to be used to move the mouse.
struct PatternObject {
	// <unix timestamp (ms), dx, dy>
	std::vector<std::tuple<int, int, int>> movement_coords;
};

class MouseMover : public MouseHandler {
	PatternObject *curr_weapon;
	std::unordered_map<std::string, PatternObject> loaded_patterns;

	std::atomic<bool> is_m_left_down;
	PatternObject LoadPatternFromFile(const std::string &filename);

public:
	MouseMover() : MouseHandler(), is_m_left_down(false) {}

	void LoadAllPatterns();
	void UpdateCurrPattern();
	LRESULT ClassWinProc(UINT msg, WPARAM w_param, LPARAM l_param);
};

void MouseSetup(INPUT *input_buffer);
void MouseMove(INPUT *input_buffer, int x_delta, int y_delta);
void MoveWithPattern(const PatternObject *loaded_pattern, std::atomic<bool> &is_firing);

#endif // !M_MOVER_H