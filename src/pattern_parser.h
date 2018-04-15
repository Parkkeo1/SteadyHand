#pragma once

#include <tuple>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>

enum Weapon {
	AK_47 = 0,
	M4_A4 = 1,
	M4_A1S = 2,
	UMP_45 = 3
};

const std::vector<std::string> weapon_names = {
	"ak_47",
	"m4_a4",
	"m4_als",
	"ump_45"
};

struct PatternObject {
	Weapon name;
	// <unix timestamp (ms), dx, dy>
	std::vector<std::tuple<int, int, int>> movement_coords;
	// used in resetting the xhair back to center.
	int total_x_travel;
	int total_y_travel;

	PatternObject(Weapon name) : name(name), total_x_travel(0), total_y_travel(0) {};
};

PatternObject LoadPatternFromFile(std::string filename, Weapon gun_name);
