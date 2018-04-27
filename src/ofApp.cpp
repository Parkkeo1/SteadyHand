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

// code derived from:
// https://stackoverflow.com/a/28491549
LRESULT ofApp::StaticWinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	if (msg == WM_NCCREATE) {
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)l_param)->lpCreateParams);
		return TRUE;
	}

	return ((ofApp*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->MouseMoverProc(hwnd, msg, w_param, l_param);
}

LRESULT ofApp::MouseMoverProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	switch (msg) {
		case WM_INPUT: {
			UINT dw_size;
			GetRawInputData((HRAWINPUT)l_param, RID_INPUT, NULL, &dw_size, sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dw_size];

			// size of message/data has been confirmed to be correct.
			if (CheckMessageSize(l_param, lpb, dw_size)) {
				RAWINPUT* raw_input = (RAWINPUT*)lpb;

				if (raw_input->header.dwType == RIM_TYPEMOUSE) {
					switch (raw_input->data.mouse.usButtonFlags) {
					case 1:
						// TODO: may have to use threads? to be tested.
						is_m_left_down = true;
						MoveWithPattern(curr_weapon_pattern, is_m_left_down);
						break;
					case 2:
						is_m_left_down = false;
						break;
					default:
						break;
					}
				}
			}

			break;
	} default:
			return DefWindowProc(hwnd, msg, w_param, l_param);
	}

	return 0;
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
	std::cout << curr_state << std::endl;
}

HWND ofApp::SetupHiddenWindow() {
	HWND hidden_window = NULL;
	HINSTANCE h_instance = GetModuleHandle(NULL);
	WNDCLASS hid_wind_class;

	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/ms633576(v=vs.85).aspx
	hid_wind_class.style = 0;
	hid_wind_class.lpfnWndProc = DefWindowProc;
	hid_wind_class.cbClsExtra = 0;
	hid_wind_class.cbWndExtra = 0;
	hid_wind_class.hInstance = h_instance;
	hid_wind_class.hIcon = NULL;
	hid_wind_class.hCursor = NULL;
	hid_wind_class.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	hid_wind_class.lpszMenuName = NULL;
	hid_wind_class.lpszClassName = kClassName;

	if (RegisterClass(&hid_wind_class)) {
		hidden_window = CreateWindow(kClassName, kClassName, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, h_instance, this);
		std::cout << "Window registration successful." << std::endl;
	}
	return hidden_window;
}

void ofApp::RegisterInputDevices(HWND &hidden_window) {
	// code referenced from Microsoft MSDN documentation:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx#example_2
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = kDesktopUsage;
	Rid[0].usUsage = kMouseUsage;
	Rid[0].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[0].hwndTarget = hidden_window;

	Rid[1].usUsagePage = kDesktopUsage;
	Rid[1].usUsage = kKeyBoardUsage;
	Rid[1].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;
	Rid[1].hwndTarget = hidden_window;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]))) {
		std::cout << "Device registration successful." << std::endl;
	} else {
		std::cout << "Device registration failed." << std::endl;
	}
}

void ofApp::MouseMoverMain() {
	is_m_left_down = false;
	HWND mouse_mover_wind = SetupHiddenWindow();
	RegisterInputDevices(mouse_mover_wind);
	std::cout << "setup complete." << std::endl;

	MSG message;
	BOOL msg_code;

	// The MS MSDN-recommended way to do message loops:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644928(v=vs.85).aspx#creating_loop
	// receives and processes messages only while the program state == USING.
	while (((msg_code = GetMessage(&message, NULL, 0, 0)) != 0) && curr_state == USING) {
		if (msg_code == -1) {
			PostQuitMessage(0);
			return;
		} else {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}

bool ofApp::CheckMessageSize(LPARAM &l_param, LPBYTE &lpb, UINT &dw_size) {
	if (lpb == NULL) {
		return false;
	}

	if (GetRawInputData((HRAWINPUT)l_param, RID_INPUT, lpb, &dw_size, sizeof(RAWINPUTHEADER)) != dw_size) {
		std::cout << "GetRawInputData does not return correct size !" << std::endl;
		return false;
	}

	return true;
}




