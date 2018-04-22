#pragma once
#ifndef SERVER_THREAD
#define SERVER_THREAD

#include "ofThread.h"
#include "crow.h"
#include "nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;

// oF documentation and examples has the function definitions in the header.
class ServerThread : public ofThread {

	std::string equipped_weapon;

	void threadedFunction() {
		crow::SimpleApp steady_hand_server;

		CROW_ROUTE(steady_hand_server, "/GS").methods("POST"_method)
			([this](const crow::request &post_request) {

			try {
				json weap_payload = json::parse(post_request.body)["player"]["weapons"];
				if (!weap_payload.is_null()) {
					for (auto &weapon : weap_payload) {
						if (weapon["state"] == "active") {
							std::string weapon_name = weapon["name"];
							lock();
							equipped_weapon = weapon_name;
							unlock();
							break;
						}
					}
				}
			} catch (json::parse_error) {
				std::cout << "JSON payload did not have appropriate data." << std::endl;
			}
			return crow::response(200);
		});

		steady_hand_server.port(8080).run();
	}

public:
	ServerThread() : equipped_weapon("") {}
	std::string &get_equipped_weapon() { return equipped_weapon; }
};

#endif // !SERVER_THREAD
