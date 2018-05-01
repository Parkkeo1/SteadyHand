#pragma once
#ifndef M_MOVER_H
#define M_MOVER_H

#include "mouse_handler.h"

// PatternObject holds all x, y coordinates with their timestamps
// used to simulate mouse movement paths parsed from pattern files
typedef std::vector<std::tuple<int, int, int>> PatternObject;
typedef std::unordered_map<std::string, PatternObject> PatternMap;

// MouseMover takes care everything relating to simulating mouse movement 
// and the aim assist aspect of SteadyHand
class MouseMover : public MouseHandler {

	PatternObject *curr_weapon;
	PatternMap loaded_patterns;

	// atomic boolean used across threads to keep track of the mouse's left button
	std::atomic<bool> is_m_left_down;
public:
	MouseMover() : MouseHandler(), is_m_left_down(false) {}

	// loads all available patterns from files in the patterns/ directory of the SteadyHand executable
	// stores all of the patterns into the PatternMap, loaded_patterns
	void LoadAllPatterns();

	// updates the MouseMover's current pattern that it uses to auto-aim/recoil control
	// to its curr_weapon_name attribute
	void UpdateCurrPattern();

	// implemented OOP WndProc function
	LRESULT ClassWinProc(UINT msg, WPARAM w_param, LPARAM l_param);

	PatternMap &get_all_patterns() { return loaded_patterns; }

	// used by LoadAllPatterns() to load PatternObjects from files, 
	// given a string filename param for each file
	static PatternObject LoadPatternFromFile(const std::string &filename);

	// functions to setup and simulate mouse movements
	static void MouseSetup(INPUT *input_buffer);
	static void MouseMove(INPUT *input_buffer, int x_delta, int y_delta);

	// uses the two helper functions above to execute mouse movements according to a given pattern
	// stops executing the pattern when is_firing is no longer true (i.e. user stops firing in-game)
	static void MoveWithPattern(const PatternObject *loaded_pattern, std::atomic<bool> &is_firing);
};

#endif // !M_MOVER_H