#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "threads.h"
#include "mouse_mover.h"

enum ProgramState {
	USING = 0,
	RECORDING = 1,
	INACTIVE = 2
};

extern const std::vector<std::string> program_states;

class ofApp : public ofBaseApp {

private:
	ofxDatGui *steadyhand_gui;
	ofxDatGuiLabel *gui_using_weapon;
	
	ServerThread json_server_th;
	MouseMover mouse_mover;

	ProgramState curr_state;
	std::string curr_weap_name;

public:
	void setup_gui();
	void setup();
	void update();
	void draw();

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
