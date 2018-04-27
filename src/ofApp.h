#pragma once

#include <map>

#include "ofMain.h"
#include "ofxDatGui.h"
#include "threads.h"
#include "mouse_mover.h"

enum ProgramState {
	USING = 0,
	RECORDING = 1
};

extern const std::vector<std::string> program_states;
extern const std::set<std::string> kWeaponNameCodes;

class ofApp : public ofBaseApp {

private:
	ofxDatGui *steadyhand_gui;
	
	ServerThread json_server_th;
	MoverThread mouse_mover;

	std::string curr_weapon_name;
	PatternObject *curr_weapon_pattern;
	ProgramState curr_state;
	std::unordered_map<std::string, PatternObject> loaded_patterns;

	void LoadAllPatterns();

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
