#include "pattern_parser.h"

PatternObject LoadPatternFromFile(std::string filename, Weapon gun_name) {
	PatternObject spray_pattern(gun_name);
	std::ifstream pattern_file(filename);
	std::string file_line;

	if (pattern_file.is_open()) {
		long long prev_time = 0;
		bool is_first_line = true;

		// TODO: test this fix.
		while (getline(pattern_file, file_line)) {
			std::istringstream in_sstream(file_line);
			std::vector<std::string> xy_info(std::istream_iterator<std::string>{in_sstream}, 
				                             std::istream_iterator<std::string>());
			if (is_first_line) {
				prev_time = std::stoll(xy_info[0]);
				is_first_line = false;
			}

			spray_pattern.movement_coords.push_back({ std::stoll(xy_info[0]) - prev_time, std::stoi(xy_info[1]), std::stoi(xy_info[2]) });
			spray_pattern.total_x_travel += std::stoi(xy_info[1]);
			spray_pattern.total_y_travel += std::stoi(xy_info[2]);
			prev_time = std::stoll(xy_info[0]);
		}
	} else {
		std::cout << "file not found" << std::endl;
	}


	return spray_pattern;
}
