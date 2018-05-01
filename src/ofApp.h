#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "threads.h"
#include "mouse_mover.h"
#include "mouse_recorder.h"

// enum to keep track of SteadyHand's program state
enum ProgramState {
	USING = 0,
	RECORDING = 1,
	INACTIVE
};

// vector whose strings each correspond to ProgramStates's names
// used to display in GUI
extern const std::vector<std::string> program_states;

// openFrameworks base application class
class ofApp : public ofBaseApp {

private:
	// ofxDatGui main gui panel
	ofxDatGui *steadyhand_gui;

	// gui components that are live-updated to show the 
	// current equipped weapon of the user
	ofxDatGuiLabel *gui_curr_weapon;
	ofxDatGuiLabel *gui_recording_weapon;
	
	// ofThread object that takes care of receiving game-state integration 
	// JSON payloads from the CS:GO client
	ServerThread json_server_th;

	// objects that handle the specific functionalities of SteadyHand
	MouseMover mouse_mover;
	MouseRecorder mouse_recorder;

	ProgramState curr_state;

public:
	// setup function specifically to setup the ofxDatGui GUI
	void setup_gui();

	// handles setting up the background local HTTP server, 
	// the MouseMover/Recorder objects, and the GUI window
	void setup();

	// handles real-time updates of which weapon the user has currently equipped;
	// constantly updates GUI to display this current weapon
	void update();
	void draw();

	// callback function for the GUI's dropdown menu that 
	// updates the program's current state
	void UpdateProgramState(ofxDatGuiDropdownEvent state_change);
};

// customizable SteadyHand theme, based on built-in Midnight theme.
class SteadyHandTheme : public ofxDatGuiTheme {

public:
	SteadyHandTheme() {
		font.size = 8;
		stripe.visible = false;
		color.label = hex(0xffffff);
		color.background = hex(0x011726);
		color.guiBackground = hex(0x4C5B66);
		color.inputAreaBackground = hex(0x273946);
		color.slider.fill = hex(0x0A1E2E);
		color.slider.text = hex(0xffffff);
		color.textInput.text = hex(0xffffff);
		color.textInput.highlight = hex(0x596872);
		color.textInput.backgroundOnActive = hex(0x0A1E2E);
		color.backgroundOnMouseOver = hex(0x273946);
		color.backgroundOnMouseDown = hex(0x000000);
		color.matrix.hover.button = hex(0x596872);
		color.matrix.selected.button = hex(0x60B1CC);
		color.graph.fills = hex(0x596872);
		init();
	}
};
