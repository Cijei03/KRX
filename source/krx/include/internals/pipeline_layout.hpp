#pragma once
#include "view.hpp"
#include <array>
#include "constants.hpp"

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
	std::array<krxResourceView*, KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT> Attributes{};
	std::array<uint32_t, KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT> AttributesSize{};
	std::array<krxResourceView*, KRX_IMPLEMENTATION_MAX_UNIFORM_TARGET_COUNT> UniformBuffers{};
	std::array<krxResourceView*, KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT> RenderTargetOutputs{};
	krxResourceView* DepthBuffer = nullptr;
public:
	krxPipelineLayout(const std::vector<krxViewInfo>& Infos, std::vector<uint32_t> AttributesSize);
	~krxPipelineLayout() = default;
};