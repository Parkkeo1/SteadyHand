#pragma once

#include "mouse_pattern_data.h"

PatternObject LoadPatternFromFile(const std::string &filename, const Weapon &gun_name);

void MouseSetup(INPUT *input_buffer);
void MouseMove(INPUT *input_buffer, int x_delta, int y_delta);
void MoveWithPattern(const PatternObject &loaded_pattern);
