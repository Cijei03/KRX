#pragma once
#include <array>
#include <thread>
#include <condition_variable>
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
		std::array<uint16_t, KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT> RenderTargetWidths{};
		std::array<uint32_t, KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT> RenderTargetOutputsFormats{};
	};

	enum class krxPPipelineTStatus : uint8_t
	{
		UNITIALIZED,
		IDLE,
		RENDER,
		SHUTDOWN,
		CLOSED
	};

	class krxPPipelineTriangles
	{
	private:
		static constexpr uint32_t IMPL_WORKERS_COUNT = 16;

		std::array<std::thread, IMPL_WORKERS_COUNT> Workers;
		std::array<RasterizerBatch, IMPL_WORKERS_COUNT> WorkerBatches;
		std::array<krxPPipelineTStatus, IMPL_WORKERS_COUNT> WorkersStatus;
		std::array<std::condition_variable, IMPL_WORKERS_COUNT> cvs;
		void* vsOutputInterfaces;
		std::condition_variable cv;

		struct
		{
			uint32_t vID = 0;
		} DrawBuffer;

		GlobalRasterizationData GlobalRasterizerData;

		friend class ::krxContext;
		krxPPipelineTriangles();
		~krxPPipelineTriangles();

		void process(krxContext* Configuration, const uint32_t vStart, const uint32_t vCount);
		void shutdown();
	};
}