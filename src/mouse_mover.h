#pragma once
#include "pattern_parser.h"

#include <Windows.h>
#include <tuple>

void MouseSetup(INPUT *input_buffer);
void MouseMove(INPUT *input_buffer, int x_delta, int y_delta);
void RecoilMoveWithPattern(const PatternObject &loaded_pattern);
