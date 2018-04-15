#include "src\mouse_mover.h"
#include "src\pattern_parser.h"

int main() {
	PatternObject m4_pattern = LoadPatternFromFile("m4_a4.txt", M4_A4);

	while (true) {
		Sleep(3000);
		MoveWithPattern(m4_pattern);
	}
	return 0;
}