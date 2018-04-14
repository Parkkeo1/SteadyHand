#include "MouseData.h"

std::ostream& operator<<(std::ostream &os, const MouseData &m_data) {
	os << m_data.ms_time << " " << m_data.dx << " " << m_data.dy << std::endl;
	return os;
}

BufferData::BufferData() {
	receive_buf = &primary_buffer;
	output_buf = &secondary_buffer;
}

void BufferData::AddMouseDataToBuf(const MouseData &new_data) {
	std::lock_guard<std::mutex> lock(data_sync);
	receive_buf->push_back(new_data);
	thread_signal.notify_one();
}

void BufferData::SaveBufferToFile(bool &to_save_curr, std::ofstream &file_stream) {
	std::lock_guard<std::mutex> lock(output_sync);
	for (auto m_data : *output_buf) {
		if (m_data.was_left_pressed && !to_save_curr) {
			to_save_curr = true;
		} else if (m_data.was_left_released && to_save_curr) {
			to_save_curr = false;
		}

		if (to_save_curr) {
			file_stream << m_data;
		}
	}
}

void BufferData::UpdateOutputBuffer() {
	// timeout checker
	std::unique_lock<std::mutex> lock(data_sync);
	if (thread_signal.wait_for(lock, std::chrono::milliseconds(200)) == std::cv_status::timeout) {
		output_buf = &std::vector<MouseData>();
	} else {
		output_buf = receive_buf;
		receive_buf->clear();
	}
}
