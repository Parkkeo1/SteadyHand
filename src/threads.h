#pragma once
#ifndef THREAD
#define THREAD

#include "ofThread.h"
#include "mouse_mover.h"
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

#endif // !THREAD
