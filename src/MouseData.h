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

	std::chrono::milliseconds ms_time;
	std::int16_t dx;
	std::int16_t dy;
	bool is_m_left_down;
public:
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

	bool BufferData::AddMouseData(MouseData new_data);
	void UpdateOutputBuffer();
	// NOTE: Change filename parameter to a ENUM type for each in-game weapon?
	void BufferData::SaveOutputBufToFile(std::string filename); 
};

#endif