#pragma once
#include "view.hpp"
#include <array>
#include "constants.hpp"
#include "processing_pipeline.hpp"

struct krxViewInfo
{
	uint8_t Index;
	krxResourceView* View = nullptr;
};

class krxContext;

class krxPipelineLayout
{
private:
	friend class krxContext;
	friend class intern::krxPPipelineTriangles;
	std::array<krxResourceView*, KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT> Attributes{};
	std::array<krxResourceView*, KRX_IMPLEMENTATION_MAX_UNIFORM_TARGET_COUNT> UniformBuffers{};
	std::array<krxResourceView*, KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT> RenderTargetOutputs{};
	krxResourceView* DepthBuffer = nullptr;
public:
	krxPipelineLayout(const std::vector<krxViewInfo>& Infos);
	~krxPipelineLayout() = default;
};