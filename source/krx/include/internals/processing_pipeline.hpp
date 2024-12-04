#pragma once
#include <array>
#include <thread>
#include <glm/glm.hpp>
#include "constants.hpp"

class krxContext;

namespace intern
{
	using check_pixel_func = bool(*)(int32_t*);

	struct RasterizerBatch
	{
		glm::i16vec2 StartData;
		glm::i16vec2 BatchSize;
	};
	struct GlobalRasterizationData
	{
		glm::i16vec2 MinTriangleBound;
		glm::i16vec2 MaxTriangleBound;
		glm::i16vec2 NDCs[3];
		check_pixel_func check_pixel = nullptr;
		void* fs_main = nullptr;
		float TriangleAreaMultiplier;
		std::array<uint8_t*, KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT> RenderTargetOutputs{};
		std::array<uint16_t, KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT> RenderTargetWidths;
		std::array<uint32_t, KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT> RenderTargetOutputsFormats{};
	};

	class krxPPipelineTriangles
	{
	private:
		std::array<std::thread, 16> Workers;
		std::array<RasterizerBatch, 16> WorkerBatches;

		struct
		{
			uint32_t vID = 0;
		} DrawBuffer;

		GlobalRasterizationData GlobalRasterizerData;

		friend class ::krxContext;
		krxPPipelineTriangles(krxContext* Configuration, const uint32_t vStart, const uint32_t vCount);
		~krxPPipelineTriangles() = default;
	};
}