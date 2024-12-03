#include "../include/internals/buffer.hpp"
#include <cstring>

krxBuffer::krxBuffer(const size_t BufferSizeBytes)
{
	this->Data.resize(BufferSizeBytes);
}

void krxBuffer::update_buffer(void* Data)
{
	std::memcpy(this->Data.data(), Data, this->Data.size());
}