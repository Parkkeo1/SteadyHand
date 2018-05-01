#pragma once
#ifndef THREAD
#define THREAD

#include "ofThread.h"
#include "mouse_mover.h"
#include "crow.h"
#include "nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;

// thread class for running the JSON endpoint server;
// runs in the background as a continuous, separate thread while SteadyHand's main thread runs
class ServerThread : public ofThread {

	// currently equipped gun of the user in-game
	std::string equipped_weapon;

	// once called, starts the Crow (i.e. Flask in C++) local server that listens to localhost:8080/GS
	// and receives JSON payloads sent by the CS:GO client to this address;
	// handles automatically detecting the current weapon equipped by the user
	void threadedFunction();

public:
	ServerThread() : equipped_weapon("") {}
	std::string &get_equipped_weapon() { return equipped_weapon; }
};

#endif // !THREAD
