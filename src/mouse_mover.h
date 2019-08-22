#pragma once
#ifndef M_MOVER_H
#define M_MOVER_H

#include "mouse_handler.h"

// PatternObject holdes tuples, which each represent a mouse coordinate point in x,y with a "delay" time value
// Thus, a vector of these tuples represents a complete mouse movement path for controlling a given weapon's recoil.
typedef std::vector<std::tuple<int, int, int>> PatternObject;

// PatternMap holds patterns for each weapon
typedef std::unordered_map<std::string, PatternObject> PatternMap;

// MouseMover takes care everything relating to simulating mouse movement and the aim assist aspect of SteadyHand
class MouseMover : public MouseHandler {

	// curr_weapon points to the current PatternObject object in the PatternMap
	PatternObject *curr_weapon;
	PatternMap loaded_patterns;

	// atomic boolean used across threads to keep track of the mouse's left button status
	std::atomic<bool> is_m_left_down;

	const static int xHairResetDelay = 250;

public:
	MouseMover() : MouseHandler(), is_m_left_down(false) {}

	// loads all available patterns from files in the patterns/ directory of the SteadyHand executable into loaded_patterns
	void load_all_patterns();

	// updates the MouseMover's current pattern that it uses to auto-aim/recoil control
	void update_current_pattern();

	// MouseMover-specific WndProc function
	LRESULT ClassWinProc(UINT msg, WPARAM w_param, LPARAM l_param);

	PatternMap &get_all_patterns() { return loaded_patterns; }

	static PatternObject load_pattern_from_file(const std::string &filename);

	// helper functions to setup and simulate mouse movements
	static void mouse_setup(INPUT *input_buffer);
	static void mouse_move(INPUT *input_buffer, int x_delta, int y_delta);

	// Executes mouse movements according to a given pattern. Stops executing the pattern when is_firing is no longer true (i.e. user stops firing in-game)
	static void move_with_pattern(const PatternObject *loaded_pattern, std::atomic<bool> &is_firing);
};

#endif // !M_MOVER_H