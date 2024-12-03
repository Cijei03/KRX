#pragma once
#include "krx_resource.hpp"
#include <cstddef>

class krxContext;

class krxBuffer : public krxResource
{
public:
	friend class krxContext;
	krxBuffer(const size_t BufferSizeBytes);
	virtual ~krxBuffer() = default;

	void update_buffer(void* Data);
};