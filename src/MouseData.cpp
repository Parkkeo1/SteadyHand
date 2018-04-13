#include "MouseData.h"

std::ostream& operator<<(std::ostream &os, const MouseData &m_data) {
	os << m_data << " " << m_data.dx << " " << m_data.dy 
	   << " " << m_data.was_left_pressed << "\n";
	return os;
}

BufferData::BufferData() {
	receive_buf = &primary_buffer;
	output_buf = &secondary_buffer;
}

void BufferData::AddMouseData(const MouseData &new_data) {
	std::lock_guard<std::mutex> lock(thread_sync);
	// only adding when player is firing the weapon.
	if (new_data.was_left_pressed) { 
		receive_buf->push_back(new_data);
	}
	thread_signal.notify_one();
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
void BufferData::SaveOutputBufToFile(const std::string filename) {
	std::ofstream file_stream(filename);
	for (MouseData m_data : *output_buf) {
		// making sure only mouse movements when user was firing are written to file.
		if (m_data.was_left_pressed) {
			file_stream << m_data;
		}
	}
	file_stream.close();
}
