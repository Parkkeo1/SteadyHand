#include "pattern_parser.h"

PatternObject LoadPatternFromFile(std::string filename, Weapon gun_name) {
	PatternObject spray_pattern(gun_name);
	std::ifstream pattern_file(filename);
	std::string file_line;

	if (pattern_file.is_open()) {
		long long prev_time = 0;

		if (getline(pattern_file, file_line)) {
			std::istringstream in_sstream(file_line);
			std::vector<std::string> first_xy_info(std::istream_iterator<std::string>{in_sstream}, 
				                                   std::istream_iterator<std::string>());
			prev_time = std::stoll(first_xy_info[0]);
			spray_pattern.movement_coords.push_back({ 0, std::stoi(first_xy_info[1]), std::stoi(first_xy_info[2]) });
		} else {
			// if the first line is empty, then just return immediately. No point in going further.
			return spray_pattern;
		}

		while (getline(pattern_file, file_line)) {
			std::istringstream in_sstream(file_line);
			std::vector<std::string> xy_info(std::istream_iterator<std::string>{in_sstream}, 
				                             std::istream_iterator<std::string>());

			spray_pattern.movement_coords.push_back({ std::stoll(xy_info[0]) - prev_time, 
				                                      std::stoi(xy_info[1]), std::stoi(xy_info[2]) });
			spray_pattern.total_x_travel += std::stoi(xy_info[1]);
			spray_pattern.total_y_travel += std::stoi(xy_info[2]);
			prev_time = std::stoll(xy_info[0]);
		}
	} else {
		std::cout << "file not found" << std::endl;
	}
	return spray_pattern;
}
