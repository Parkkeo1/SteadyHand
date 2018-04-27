#include "ofApp.h"

const std::string kGuiTitle = ">> SteadyHand <<";

const std::vector<std::string> program_states = {
	"Aim Assist Mode",
	"Record Patterns Mode",
	"Inactive"
};

// same as 0x011726.
const int kBackGroundColor = 71462;

void ofApp::setup_gui() { // NOTE: Adjust high res mode threshold in ofxDatGuiTheme.h to the window size used in main.cpp
	ofxDatGui* steadyhand_gui = new ofxDatGui(0, 0);
	steadyhand_gui->setTheme(new SteadyHandTheme());
	
	// title heading in gui
	ofxDatGuiLabel *gui_title = steadyhand_gui->addLabel(kGuiTitle);
	gui_title->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	
	// menu to switch between states
	ofxDatGuiDropdown *state_menu = steadyhand_gui->addDropdown("Select Program State", program_states);
	state_menu->onDropdownEvent(this, &ofApp::UpdateProgramState);
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColorHex(kBackGroundColor);
	curr_state = INACTIVE;
	mouse_mover.set_hwnd_manually(ofGetWin32Window());
	json_server_th.startThread(true);
	setup_gui();
}

//--------------------------------------------------------------
void ofApp::update() {
	std::string &new_equipped = json_server_th.get_equipped_weapon();

	if (new_equipped != mouse_mover.get_curr_weap_name() && kWeaponNameCodes.count(new_equipped) != 0) {
		mouse_mover.set_curr_weap_name(new_equipped);
		mouse_mover.UpdateCurrPattern();
		std::cout << new_equipped << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {}

void ofApp::UpdateProgramState(ofxDatGuiDropdownEvent state_change) {
	state_change.target->toggle();
	curr_state = static_cast<ProgramState>(state_change.child);

	if (curr_state == USING) {
		mouse_mover.SetupMover();
		mouse_mover.RunMover();
	}
}




