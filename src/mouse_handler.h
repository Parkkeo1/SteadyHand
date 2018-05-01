#pragma once
#ifndef M_HANDLER
#define M_HANDLER

#include <Windows.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <tuple>
#include <set>
#include <unordered_map>
#include <thread>
#include <functional>
#include <atomic>

extern const std::string kInactive;

// set of in-game gun names that SteadyHand can be used for
extern const std::set<std::string> kWeaponNameCodes;

// virtual key codes that Windows uses to identify keyboard presses
enum VirtualKeys {
	ENTER = 13, // ENTER
	SAVE = 85, // u
	CLEAR = 73 // i
};

// superclass for MouseMover and MouseRecorder
// contains common properties and functions needed for Windows messages processing
class MouseHandler {

protected:
	HWND input_window;
	std::string curr_weapon_name;

	// registers user's mouse and keyboard to SteadyHand's window handle
	void RegisterMouse(); 

	// function that checks an incoming Windows message to see if its size is valid before fully processing it
	static LPBYTE CheckMessageSize(LPARAM &l_param);

	// callback functions to receive Windows raw input messages
	static LRESULT CALLBACK StaticWinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
	virtual LRESULT ClassWinProc(UINT msg, WPARAM w_param, LPARAM l_param) = 0;

public:
	MouseHandler() : curr_weapon_name(kInactive), input_window(NULL) {}

	std::string &get_curr_weap_name() { return curr_weapon_name; }
	void set_curr_weap_name(const std::string &new_weap_name) { curr_weapon_name = new_weap_name; }
	void set_hwnd_manually(HWND window_handle) { input_window = window_handle; }

	// functions that set up the MouseMover/Recorder object to start receiving/processing messages from Windows
	void Setup();
	void Run();
};

#endif // !M_HANDLER

