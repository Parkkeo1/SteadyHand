#include "ofApp.h"

// initialize static members
const std::string ofApp::gui_title = "SteadyHand: An auto aim-assist cheat";
const std::string ofApp::curr_weapon_prefix = "Currently Equipped: ";

const std::vector<std::string> ofApp::modes = { "Active", "Recording", "Inactive" };

// NOTE: For first time use of ofxDatGui, adjust high res mode threshold in ofxDatGuiTheme.h to the window size used in main.cpp
void ofApp::setup_gui() {
	steady_hand_gui = new ofxDatGui(0, 0);
	steady_hand_gui->setTheme(new SteadyHandTheme());
	
	// title
	ofxDatGuiLabel * gui_title_label = steady_hand_gui->addLabel(gui_title);
	gui_title_label->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	
	// menu to switch between states
	ofxDatGuiDropdown * state_menu = steady_hand_gui->addDropdown("Select Program State", modes);
	state_menu->onDropdownEvent(this, &ofApp::UpdateProgramState);

	// label that displays current weapon
	gui_curr_weapon = steady_hand_gui->addLabel(curr_weapon_prefix);
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColorHex(bg_color);
	curr_state = INACTIVE;

	bg_server_thread.startThread(true);

	// preemptively load in all weapon patterns into memory for fast/efficient access and swapping b/t weapons
	mouse_mover.load_all_patterns();

	// get handle of application window and set it to use for receiving messages from Windows
	mouse_mover.set_hwnd_manually(ofGetWin32Window());
	mouse_recorder.set_hwnd_manually(ofGetWin32Window());

	setup_gui();
}

//--------------------------------------------------------------
void ofApp::update() {
	// when reading an update to the currently equipped weapon, ensure thread safety
	bg_server_thread.lock();
	std::string &new_weapon = bg_server_thread.get_equipped_weapon();
	bg_server_thread.unlock();

	// update mouse handlers with new weapon to appropriately record or playback correct weapon, depending on mode
	if (MouseHandler::weapon_names.count(new_weapon) != 0) {
		if (curr_state == USING && new_weapon != mouse_mover.get_curr_weap_name()) {
			mouse_mover.set_curr_weap_name(new_weapon);
			mouse_mover.update_current_pattern();
		} else if (curr_state == RECORDING && new_weapon != mouse_recorder.get_curr_weap_name()) {
			mouse_recorder.set_curr_weap_name(new_weapon);
		}
	}

	// manually updating GUI with currently equipped weapon.
	if (curr_state == USING) {
		gui_curr_weapon->setLabel(curr_weapon_prefix + mouse_mover.get_curr_weap_name());
	} else {
		gui_curr_weapon->setLabel(curr_weapon_prefix + mouse_recorder.get_curr_weap_name());
	}
	
}

//--------------------------------------------------------------
void ofApp::draw() {}

void ofApp::UpdateProgramState(ofxDatGuiDropdownEvent state_change) {
	state_change.target->toggle();
	// detect which mode user chose to switch to
	// cast to ProgramState enum as drop down event returns index of selected option
	curr_state = static_cast<ProgramState>(state_change.child);

	if (curr_state == USING) {
		mouse_mover.setup();
		mouse_mover.run();
	}

	if (curr_state == RECORDING) {
		mouse_recorder.setup();
		mouse_recorder.run();
	}
}




