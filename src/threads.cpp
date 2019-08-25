#include "threads.h"

void ServerThread::threadedFunction() {
	crow::SimpleApp weapon_detect_server;

	CROW_ROUTE(weapon_detect_server, "/GS").methods("POST"_method)
		([this](const crow::request &game_state_data) {

		try {
			json weapon_data = json::parse(game_state_data.body)["player"]["weapons"];
			if (!weapon_data.is_null()) {
				for (auto &weapon : weapon_data) {
					// within JSON payload, determine which weapon user is actively holding
					if (weapon["state"] == "active") {
						// mutex locking functionality from ofThread to ensure updates to this variable are thread-safe,
						// as this variable is read by other parts of program
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

	weapon_detect_server.port(8080).run();
}
