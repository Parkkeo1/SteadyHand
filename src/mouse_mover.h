#pragma once
#ifndef M_MOVER_H
#define M_MOVER_H

#include "mouse_recorder.h"

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

PatternObject LoadPatternFromFile(const std::string &filename, const Weapon &gun_name);

void MouseSetup(INPUT *input_buffer);
void MouseMove(INPUT *input_buffer, int x_delta, int y_delta);
void MoveWithPattern(const PatternObject &loaded_pattern);

#endif // !M_MOVER_H