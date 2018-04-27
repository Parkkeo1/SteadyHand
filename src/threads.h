#pragma once
#ifndef THREAD
#define THREAD

#include "ofThread.h"
#include "crow.h"
#include "nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;

// thread class for running the JSON endpoint server.
class ServerThread : public ofThread {

	std::string equipped_weapon;

	void threadedFunction();

public:
	ServerThread() : equipped_weapon("") {}
	std::string &get_equipped_weapon() { return equipped_weapon; }
};

// thread class for running the mouse mover part of the program.
class MoverThread : public ofThread {
	HWND mouse_mover_wind;
	bool is_m_left_down;

	void CreateHiddenWindow();
	void RegisterMouse();
	void threadedFunction();

	static LRESULT CALLBACK StaticWinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
	LRESULT MouseMoverProc(UINT msg, WPARAM w_param, LPARAM l_param);

public:
	void SetupMover();
	void set_mouse_down(bool is_mouse_left_down) { is_m_left_down = is_mouse_left_down; }
};

#endif // !THREAD
