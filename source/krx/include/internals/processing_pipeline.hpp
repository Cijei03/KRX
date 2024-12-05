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
	using depth_test_func = bool(*)(uint8_t*, const int16_t, const glm::i16vec2, const uint16_t);
	using blend_func = glm::u8vec4(*)(const glm::u8vec4&, const glm::u8vec4&);

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
		int16_t NDCDepths[3];
		check_pixel_func check_pixel = nullptr;
		void* fs_main = nullptr;
		float TriangleAreaMultiplier;
		std::array<uint8_t*, KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT> RenderTargetOutputs{};
		std::array<uint16_t, KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT> RenderTargetWidths{};
		std::array<uint32_t, KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT> RenderTargetOutputsFormats{};

		std::array<void*, KRX_IMPLEMENTATION_MAX_UNIFORM_TARGET_COUNT> ShaderUniformTargets{};

		blend_func blend = nullptr;
		struct
		{
			uint8_t* DepthBuffer = nullptr;
			bool DepthTestEnabled = false;
			depth_test_func test_depth = nullptr;
			uint16_t DepthBufferWidth = 0;
		} DepthTestInfo;
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
		const uint32_t WorkersCount = std::thread::hardware_concurrency() * 2;

		std::vector<std::thread> Workers = std::vector<std::thread>(WorkersCount);
		std::vector<RasterizerBatch> WorkerBatches = std::vector<RasterizerBatch>(WorkersCount);
		std::vector<krxPPipelineTStatus> WorkersStatus = std::vector<krxPPipelineTStatus>(WorkersCount);
		std::vector<std::condition_variable> cvs = std::vector<std::condition_variable>(WorkersCount);

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