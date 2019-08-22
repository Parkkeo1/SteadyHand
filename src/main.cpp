#include "ofMain.h"
#include "ofApp.h"

#include <Windows.h>

int WINAPI WinMain(HINSTANCE h_inst, HINSTANCE h_prev_inst, LPSTR lp_cmd_line, int n_cmd_show) {
	// start application through openFrameworks
	ofSetupOpenGL(540, 400, OF_WINDOW); // 540x400 window
	ofApp * steady_hand = new ofApp();
	ofRunApp(steady_hand);
}
