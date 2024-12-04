#pragma once
#include "krx_resource.hpp"
#include <cstddef>
#include "processing_pipeline.hpp"

class krxContext;

class krxBuffer : public krxResource
{
public:
	friend class intern::krxPPipelineTriangles;
	friend class krxContext;
	krxBuffer(const size_t BufferSizeBytes);
	virtual ~krxBuffer() = default;

	void update_buffer(void* Data);
};