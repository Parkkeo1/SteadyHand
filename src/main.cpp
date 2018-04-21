#include "mouse_recorder.h"
#include "mouse_mover.h"

int WINAPI WinMain(HINSTANCE h_inst, HINSTANCE h_prev_inst, LPSTR lp_cmd_line, int n_cmd_show) {
	bool is_recording = true;
	Weapon ump_weap = UMP_45;

	//mouse_recorder.set_curr_weapon(ump_weap);
	//// TODO: Replace this with actual stop condition, probably using ENUM states.
	//mouse_recorder.RunMouseRecorder(h_inst, is_recording);

	PatternObject m4_pattern = LoadPatternFromFile("../patterns/m4_a4.txt", AK_47);
	while (true) {
		Sleep(3000);
		MoveWithPattern(m4_pattern);
	}
	return 0;
}