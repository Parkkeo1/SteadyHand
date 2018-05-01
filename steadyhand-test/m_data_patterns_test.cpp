#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/mouse_mover.h"
#include "../src//mouse_recorder.h"

// testing pattern functions in mouse_mover

TEST_CASE("LoadPatternsTest") {
	PatternObject m4_test_pattern = MouseMover::LoadPatternFromFile("patterns/weapon_m4a1.txt");
	PatternObject blank_pattern = MouseMover::LoadPatternFromFile("patterns/test2.txt");

	REQUIRE(m4_test_pattern.size() == 1572);
	REQUIRE(blank_pattern.size() == 0);

	SECTION("LoadAllPatternsTest") {
		MouseMover test_mover;
		test_mover.LoadAllPatterns();

		REQUIRE(test_mover.get_all_patterns().size() == kWeaponNameCodes.size());
		REQUIRE(test_mover.get_all_patterns().at("weapon_m4a1") == m4_test_pattern);
	}
}

// testing MouseData struct functions in mouse_recorder

TEST_CASE("MouseDataRecorderTest") {
	MouseRecorder test_recorder;
	MouseData test_data(1, 1, 1, 1);
	MouseData throw_data(1, 1, 1, 2);

	SECTION("MouseDataStreamTest") {
		std::stringstream m_data_stream;

		m_data_stream << test_data;
		REQUIRE(m_data_stream.str() == "1 1 1 1\n");
	}

	test_recorder.set_curr_weap_name("test");
	test_recorder.get_mouse_data_buf().push_back(test_data);
	test_recorder.get_mouse_data_buf().push_back(throw_data);

	SECTION("WriteMouseDataTest") {
		test_recorder.WriteBufferToFile();

		std::ifstream test_file("patterns/test.txt");
		REQUIRE(test_file.is_open() == true);

		std::stringstream test_buf;
		test_buf << test_file.rdbuf();
		REQUIRE(test_buf.str() == "1 1 1 1\n");
	}
}
