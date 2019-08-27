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
	HWND input_window; // handle to this application's window. Necessary for receiving messages from Windows
	std::string curr_weapon_name;

	// registers user's mouse and keyboard as raw input devices to SteadyHand's window handle
	void register_mouse();

	const static std::string inactive_weapon;
	const static LPCWSTR class_name;

	// constants for Windows API raw input device codes.
	const static int desktop_mode_code = 1;
	const static int mouse_input_code = 2;
	const static int keyboard_input_code = 6;

	// function that checks an incoming Windows message to see if its size is valid before program processes it
	static LPBYTE check_message_size(LPARAM &l_param);

	static LRESULT CALLBACK StaticWinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

	// subclasses implement this callback function to receive Windows raw input messages,  for either recording for playback of mouse input
	virtual LRESULT ClassWinProc(UINT msg, WPARAM w_param, LPARAM l_param) = 0;

public:
	// weapons currently supported by SteadyHand
	// to add new weapons, just enter the name codes of any automatic firing weapon (into literal in mouse_handler.cpp) from CS:GO and record for it
	const static std::set<std::string> weapon_names;

	MouseHandler() : curr_weapon_name(inactive_weapon), input_window(NULL) {}

	std::string & get_curr_weap_name() { return curr_weapon_name; }
	void set_curr_weap_name(const std::string &new_weap_name) { curr_weapon_name = new_weap_name; }
	void set_hwnd_manually(HWND window_handle) { input_window = window_handle; }

	// functions that set up the MouseHandler object to start receiving/processing messages from Windows
	void setup();
	void run();
};

#endif // !M_HANDLER

