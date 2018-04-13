#include "MouseData.h"

std::ostream& operator<<(std::ostream &os, const MouseData &m_data) {
	os << m_data.ms_time.count() << " " << m_data.dx << " " << m_data.dy 
	   << " " << m_data.is_m_left_down << "\n";
	return os;
}

BufferData::BufferData() {
	receive_buf = &primary_buffer;
	output_buf = &secondary_buffer;
}

// returns true if mouse's left-click was held down in data.
bool BufferData::AddMouseData(MouseData new_data) {
	std::lock_guard<std::mutex> lock(thread_sync);
	// only adding when player is firing the weapon.
	if (new_data.is_m_left_down) { 
		receive_buf->push_back(new_data);
	}
	thread_signal.notify_one();
	return new_data.is_m_left_down;
}

void BufferData::UpdateOutputBuffer() {
	// timeout checker
	std::unique_lock<std::mutex> lock(thread_sync);
	if (thread_signal.wait_for(lock, std::chrono::milliseconds(200)) == std::cv_status::timeout) {
		output_buf = &std::vector<MouseData>();
	} else {
		output_buf = receive_buf;
		receive_buf->clear();
	}
}

// may change to ENUM type instead of filename in the future.
void BufferData::SaveOutputBufToFile(std::string filename) {
	std::ofstream file_stream(filename);
	for (MouseData m_data : *output_buf) {
		file_stream << m_data;
	}
	file_stream.close();
}
