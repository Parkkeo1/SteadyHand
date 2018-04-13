#pragma once
#ifndef MOUSEDATA_H
#define MOUSEDATA_H

#include <chrono>
#include <cstdint>
#include <queue>
#include <mutex>
#include <iostream>
#include <fstream>
#include <Windows.h>

// code partially derived from:
// https://github.com/rspeele/MouseMeat/blob/master/Events.cpp

struct MouseData {

	long long ms_time;
	short dx;
	short dy;
	bool was_left_pressed;
	bool was_left_released;
public:
	MouseData(const long long curr_time, const short x_delta, const short y_delta, 
			  const bool left_pressed, const bool left_released) : ms_time(curr_time), 
		                                                           dx(x_delta), 
																   dy(y_delta), 
																   was_left_pressed(left_pressed), 
															       was_left_released(left_released) {};
	friend std::ostream& operator<<(std::ostream& os, const MouseData& m_data);
};

class BufferData {

	std::vector<MouseData> primary_buffer;
	std::vector<MouseData> secondary_buffer;
public:
	std::vector<MouseData> *receive_buf;
	std::vector<MouseData> *output_buf;
	std::mutex thread_sync;
	std::condition_variable thread_signal;

	BufferData();

	void BufferData::AddMouseData(const MouseData &new_data);
	void UpdateOutputBuffer();
	// NOTE: Change filename parameter to a ENUM type for each in-game weapon?
	void BufferData::SaveOutputBufToFile(const std::string filename); 
};

#endif