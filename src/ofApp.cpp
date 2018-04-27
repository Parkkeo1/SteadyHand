#include "ofApp.h"

const std::string kGuiTitle = ">> SteadyHand <<";
const LPCWSTR kClassName = TEXT("SteadyHand");

// constants for Windows API raw input device codes.
const short kDesktopUsage = 1;
const short kMouseUsage = 2;
const short kKeyBoardUsage = 6;

const std::set<std::string> kWeaponNameCodes = {
	"weapon_ak47",
	"weapon_m4a1",
	"weapon_m4a1_silencer"
};

const std::vector<std::string> program_states = {
	"Aim Assist Mode",
	"Record Patterns Mode"
};

// same as 0x011726.
const int kBackGroundColor = 71462;

void ofApp::LoadAllPatterns() {
	for (auto &weapon_name : kWeaponNameCodes) {
		loaded_patterns.insert({ weapon_name, LoadPatternFromFile("patterns/" + weapon_name + ".txt") }); // tentative directory.
		// blank patterns may have been inserted for some weapons, if patterns don't exist for them or the file is not found.
	}
	// setting defaults.
	curr_weapon_name = *kWeaponNameCodes.begin();	
	try {
		curr_weapon_pattern = &loaded_patterns.at(curr_weapon_name);
	} catch (const std::exception) {
		std::cout << "the pattern map was not initialized correctly." << std::endl;
	}
}

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
	curr_state = USING;
	LoadAllPatterns();
	mouse_mover.SetupMover();
	mouse_mover.startThread(true);
	json_server_th.startThread(true);
	setup_gui();
}

//--------------------------------------------------------------
void ofApp::update() {
	std::string &new_equipped = json_server_th.get_equipped_weapon();

	if (new_equipped != curr_weapon_name && kWeaponNameCodes.count(new_equipped) != 0) {
		curr_weapon_name = new_equipped;
		std::cout << curr_weapon_name << std::endl;
		// loading new pattern according to newly equipped weapon.
		curr_weapon_pattern = &loaded_patterns.at(curr_weapon_name);
		std::cout << curr_weapon_pattern->size() << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {}

void ofApp::UpdateProgramState(ofxDatGuiDropdownEvent state_change) {
	state_change.target->toggle();
	curr_state = static_cast<ProgramState>(state_change.child);
	mouse_mover.stopThread();
	mouse_mover.set_mouse_down(false);

	if (curr_state == USING) {
		mouse_mover.startThread(true);
	}
}




