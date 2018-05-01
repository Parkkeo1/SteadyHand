#pragma once
#ifndef M_MOVER_H
#define M_MOVER_H

#include "mouse_handler.h"

extern const std::set<std::string> kWeaponNameCodes;

// <unix timestamp (ms), dx, dy>
typedef std::vector<std::tuple<int, int, int>> PatternObject;
typedef std::unordered_map<std::string, PatternObject> PatternMap;

class MouseMover : public MouseHandler {
	PatternObject *curr_weapon;
	PatternMap loaded_patterns;

	std::atomic<bool> is_m_left_down;
public:
	MouseMover() : MouseHandler(), is_m_left_down(false) {}

	void LoadAllPatterns();
	void UpdateCurrPattern();
	LRESULT ClassWinProc(UINT msg, WPARAM w_param, LPARAM l_param);

	PatternMap &get_all_patterns() { return loaded_patterns; }

	static PatternObject LoadPatternFromFile(const std::string &filename);
	static void MouseSetup(INPUT *input_buffer);
	static void MouseMove(INPUT *input_buffer, int x_delta, int y_delta);
	static void MoveWithPattern(const PatternObject *loaded_pattern, std::atomic<bool> &is_firing);
};



#endif // !M_MOVER_H