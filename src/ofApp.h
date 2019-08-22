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
	INACTIVE = 2
};

// openFrameworks application class
class ofApp : public ofBaseApp {

private:
	// main gui panel
	ofxDatGui * steady_hand_gui;

	// real-time updated gui components for current user weapon
	ofxDatGuiLabel *gui_curr_weapon;
	ofxDatGuiLabel *gui_recording_weapon;
	
	// dedicated ofThread to receive real-time player data from game client
	ServerThread bg_server_thread;

	// handles the specific functionalities of SteadyHand
	MouseMover mouse_mover;
	MouseRecorder mouse_recorder;

	ProgramState curr_state;

	// GUI related settings/constants
	const static std::string gui_title;
	const static std::string curr_weapon_prefix;;
	const static int bg_color = 71462;

	// each mode's index corresponds to a ProgramState enum value
	// vector is necessary due to how ofxDatGui handles drop down menus
	const static std::vector<std::string> modes;

public:
	// setup function to setup ofxDatGui GUI
	void setup_gui();

	// handles initializing background thread, MouseMover/Recorder, and GUI window
	void setup();

	// handles real-time GUI updates of which weapon the user has currently equipped;
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
