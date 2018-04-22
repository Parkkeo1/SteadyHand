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

extern const std::set<std::string> kWeaponNameCodes;

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

PatternObject LoadPatternFromFile(const std::string &filename);

void MouseSetup(INPUT *input_buffer);
void MouseMove(INPUT *input_buffer, int x_delta, int y_delta);
void MoveWithPattern(const PatternObject &loaded_pattern);

#endif // !M_MOVER_H