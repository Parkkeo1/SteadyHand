#pragma once
#ifndef MOUSEDATA_H
#define MOUSEDATA_H

#include <chrono>
#include <cstdint>
#include <queue>
#include <mutex>

// code partially based on:
// https://github.com/rspeele/MouseMeat/blob/master/Events.cpp

class BufferData {
	struct MouseData {
		std::chrono::milliseconds ms_time;
		std::int16_t dx;
		std::int16_t dy;
		bool is_m_left_down;
	};

	std::vector<MouseData> active_buf;
	std::vector<MouseData> inactive_buf;
	const std::queue<MouseData> empty_buf;
public:
	std::mutex thread_sync;
	std::condition_variable thread_signal;

	// TODO: Move method implementations to .cpp

	void AddMouseData(MouseData new_data) {
		std::lock_guard<std::mutex> lock(thread_sync);
		active_buf.push_back(new_data);
		thread_signal.notify_one();
	}

	std::vector<MouseData>& GetUpdatedOutputBuffer() {
		std::lock_guard<std::mutex> lock(thread_sync);
		inactive_buf = active_buf;
		active_buf.clear();

		return inactive_buf;
	}
};





#endif