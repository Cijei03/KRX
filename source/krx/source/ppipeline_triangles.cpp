#include "../include/internals/processing_pipeline.hpp"
#include "../include/internals/constants.hpp"
#include "../include/internals/context.hpp"
#include "shaders.hpp"
#include <cstring>
#include "rasterization_utils.hpp"

#include <iostream>

static bool depth_test_enabled(uint8_t* DepthBuffer, const int16_t PixelDepth, const glm::i16vec2 CurrentPixel, const uint16_t DepthBufferWidth)
{
	if (PixelDepth > 255) return false;
	if (PixelDepth < 0) return false;

	if (DepthBuffer[CurrentPixel.y * DepthBufferWidth + CurrentPixel.x] > PixelDepth)
	{
		DepthBuffer[CurrentPixel.y * DepthBufferWidth + CurrentPixel.x] = static_cast<uint8_t>(PixelDepth);
		return true;
	}
	return false;
}
static bool depth_test_disabled(uint8_t* DepthBuffer, const int16_t PixelDepth, const glm::i16vec2 CurrentPixel, const uint16_t DepthBufferWidth)
{
	if (PixelDepth > 255) return false;
	if (PixelDepth < 0) return false;

	return true;
}

static glm::u8vec4 blend_mode_enabled(const glm::u8vec4& fsOutput, const glm::u8vec4& BackBufferColor)
{
	const float SrcAlpha = static_cast<float>(fsOutput[3]) / 255.0f;
	const float DstAlpha = 1.0f - SrcAlpha;
	return glm::u8vec4(fsOutput[0] * SrcAlpha, fsOutput[1] * SrcAlpha, fsOutput[2] * SrcAlpha, fsOutput[3] * SrcAlpha) + glm::u8vec4(BackBufferColor[0] * DstAlpha, BackBufferColor[1] * DstAlpha, BackBufferColor[2] * DstAlpha, BackBufferColor[3] * DstAlpha);
}
static glm::u8vec4 blend_mode_disabled(const glm::u8vec4& fsOutput, const glm::u8vec4& BackBufferColor)
{
	return glm::u8vec4(fsOutput[0], fsOutput[1], fsOutput[2], 255);
}

static void batch_main(intern::RasterizerBatch* Batch, intern::GlobalRasterizationData* GlobalData, const ShaderInterface* fsInput, std::condition_variable* cv, intern::krxPPipelineTStatus* Status)
{
	std::mutex m;
	std::unique_lock<std::mutex> Lock(m);
	*Status = intern::krxPPipelineTStatus::IDLE;
	cv->notify_one();
	while (true)
	{
		cv->notify_one();
		cv->wait_for(Lock, std::chrono::milliseconds(500), [Status](){ return *Status != intern::krxPPipelineTStatus::IDLE; });

		if (*Status == intern::krxPPipelineTStatus::RENDER)
		{
			glm::i16vec2 CurrentPixel = Batch->StartData;
			const glm::i16vec2 EndData = CurrentPixel + Batch->BatchSize;
			ShaderInterface FragmentShaderOutput{};
			glm::f32vec3 Weights;
			
			while (CurrentPixel.y < EndData.y)
			{
				CurrentPixel.x = Batch->StartData.x;
				bool foundTriangle = false;

				while (CurrentPixel.x < EndData.x)
				{
					int32_t signs[3] =
					{
						edge_function(CurrentPixel, GlobalData->NDCs[0], GlobalData->NDCs[1]),
						edge_function(CurrentPixel, GlobalData->NDCs[1], GlobalData->NDCs[2]),
						edge_function(CurrentPixel, GlobalData->NDCs[2], GlobalData->NDCs[0])
					};

					if (GlobalData->check_pixel(signs))
					{
						foundTriangle = true;
						Weights = glm::f32vec3
						(
							static_cast<float>(signs[0]) * GlobalData->TriangleAreaMultiplier,
							static_cast<float>(signs[1]) * GlobalData->TriangleAreaMultiplier,
							static_cast<float>(signs[2]) * GlobalData->TriangleAreaMultiplier
						);

						const int16_t PixelDepth = Weights[0] * GlobalData->NDCDepths[0] + Weights[1] * GlobalData->NDCDepths[1] + Weights[2] * GlobalData->NDCDepths[2];

						if (!GlobalData->DepthTestInfo.test_depth(GlobalData->DepthTestInfo.DepthBuffer, PixelDepth, CurrentPixel, GlobalData->DepthTestInfo.DepthBufferWidth))
						{
							CurrentPixel.x++;
							continue;
						}

						reinterpret_cast<krxFragmentShader>(GlobalData->fs_main)(fsInput, &FragmentShaderOutput, reinterpret_cast<float*>(&Weights), reinterpret_cast<const krxShaderUniformInterface*>(GlobalData->ShaderUniformTargets.data()));

						for (uint8_t i = 0; i < KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT; i++)
						{
							const krxFormat TargetFormat = static_cast<krxFormat>(GlobalData->RenderTargetOutputsFormats[i]);

							if (GlobalData->RenderTargetOutputs[i] != nullptr)
							{
								if (TargetFormat == krxFormat::PRESENT_FORMAT)
								{
									const auto PositionInBuffer = reinterpret_cast<glm::u8vec4*>(GlobalData->RenderTargetOutputs[i]) + CurrentPixel.y * GlobalData->RenderTargetWidths[i] + CurrentPixel.x;
									const float* Ptr = reinterpret_cast<const float*>(&FragmentShaderOutput[i]);

									*PositionInBuffer = GlobalData->blend(glm::u8vec4(Ptr[2] * 255, Ptr[1] * 255, Ptr[0] * 255, Ptr[3] * 255), *PositionInBuffer);
								}
							}
						}
					}
					else if (foundTriangle)
					{
						break;
					}
					
					CurrentPixel.x++;
				}
				CurrentPixel.y++;
			}
			*Status = intern::krxPPipelineTStatus::IDLE;
		}
		else if (*Status == intern::krxPPipelineTStatus::SHUTDOWN)
		{
			break;
		}
	}

	*Status = intern::krxPPipelineTStatus::CLOSED;
	cv->notify_one();
}

namespace intern
{
	void krxPPipelineTriangles::process(krxContext* Configuration, const uint32_t vStart, const uint32_t vCount)
	{
		if (Configuration->Rasterizer.Facing == krxRasterizerFacing::CCW)
		{
			this->GlobalRasterizerData.check_pixel = check_pixel_ccw;
		}
		else
		{
			this->GlobalRasterizerData.check_pixel = check_pixel_cw;
		}

		if (Configuration->IsBlendingEnabled)
		{
			this->GlobalRasterizerData.blend = blend_mode_enabled;
		}
		else
		{
			this->GlobalRasterizerData.blend = blend_mode_disabled;
		}

		for (uint8_t i = 0; i < KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT; i++)
		{
			this->GlobalRasterizerData.RenderTargetOutputs[i] = reinterpret_cast<krxTexture2D*>(Configuration->PipelineLayout->RenderTargetOutputs[0]->Info.Resource)->Data.data();
			this->GlobalRasterizerData.RenderTargetOutputsFormats[i] = static_cast<uint32_t>(reinterpret_cast<krxTexture2D*>(Configuration->PipelineLayout->RenderTargetOutputs[0]->Info.Resource)->Info.Format);
			this->GlobalRasterizerData.RenderTargetWidths[i] = reinterpret_cast<krxTexture2D*>(Configuration->PipelineLayout->RenderTargetOutputs[0]->Info.Resource)->Info.Size.Width;
		}

		std::array<const uint8_t*, KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT> InputFromBuffers{};
		const auto InputJumpLengths = Configuration->ShaderPipeline->VertexShader.InputSizeRequirements;

		krxVertexShader VertexShaderFunc = reinterpret_cast<krxVertexShader>(Configuration->ShaderPipeline->VertexShader.shader_function);
		this->GlobalRasterizerData.fs_main = Configuration->ShaderPipeline->FragmentShader.shader_function;

		for (uint32_t i = 0; i < KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT; i++)
		{
			if (Configuration->PipelineLayout->Attributes[i] != nullptr)
			{
				InputFromBuffers[i] = reinterpret_cast<krxBuffer*>(Configuration->PipelineLayout->Attributes[i]->Info.Resource)->Data.data() + InputJumpLengths[i] * vStart;
			}
		}

		for (uint8_t i = 0; i < KRX_IMPLEMENTATION_MAX_UNIFORM_TARGET_COUNT; i++)
		{
			if (Configuration->PipelineLayout->UniformBuffers[i] != nullptr)
			{
				this->GlobalRasterizerData.ShaderUniformTargets[i] = reinterpret_cast<krxBuffer*>(Configuration->PipelineLayout->UniformBuffers[i]->Info.Resource)->Data.data();
			}
		}

		this->GlobalRasterizerData.DepthTestInfo.DepthTestEnabled = (Configuration->Rasterizer.Features & static_cast<uint8_t>(krxRasterizerFeature::DEPTH_TESTING));
		if (Configuration->PipelineLayout->DepthBuffer != nullptr)
		{
			this->GlobalRasterizerData.DepthTestInfo.DepthBuffer = reinterpret_cast<krxTexture2D*>(Configuration->PipelineLayout->DepthBuffer->Info.Resource)->Data.data();
			this->GlobalRasterizerData.DepthTestInfo.DepthBufferWidth = reinterpret_cast<krxTexture2D*>(Configuration->PipelineLayout->DepthBuffer->Info.Resource)->Info.Size.Width;
			this->GlobalRasterizerData.DepthTestInfo.test_depth = depth_test_enabled;
		}
		else
		{
			this->GlobalRasterizerData.DepthTestInfo.test_depth = depth_test_disabled;
		}
		
		

		for (uint32_t v = vStart; v < vStart + vCount; v += 3)
		{
			// Declare krxsl variables.
			ShaderInterface vsInputData[3];

			VariablesVS ShaderVariables[3]
			{
				{
					.SV_POSITION = glm::vec4(0),
					.SV_VERTEXID = v
				},
				{
					.SV_POSITION = glm::vec4(0),
					.SV_VERTEXID = v + 1
				},
				{
					.SV_POSITION = glm::vec4(0),
					.SV_VERTEXID = v + 2
				}
			};

			// Read buffers input and process them through Vertex Shader.
			for (uint32_t vTriangle = 0; vTriangle < 3; vTriangle++)
			{			
				for (uint32_t i = 0; i < KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT; i++)
				{
					if (InputJumpLengths[i] != 0)
					{
						std::memcpy(&vsInputData[vTriangle][i], InputFromBuffers[i], InputJumpLengths[i]);
						InputFromBuffers[i] += InputJumpLengths[i];
					}
				}

				VertexShaderFunc(&vsInputData[vTriangle], &reinterpret_cast<ShaderInterface*>(this->vsOutputInterfaces)[vTriangle], &ShaderVariables[vTriangle], reinterpret_cast<const krxShaderUniformInterface*>(this->GlobalRasterizerData.ShaderUniformTargets.data()));
			}

			// Prepare for rasterization.
			this->GlobalRasterizerData.NDCs[0] = ndc_to_pixel_coord(ShaderVariables[0].SV_POSITION, Configuration->Rasterizer.Viewport.Size);
			this->GlobalRasterizerData.NDCs[1] = ndc_to_pixel_coord(ShaderVariables[1].SV_POSITION, Configuration->Rasterizer.Viewport.Size);
			this->GlobalRasterizerData.NDCs[2] = ndc_to_pixel_coord(ShaderVariables[2].SV_POSITION, Configuration->Rasterizer.Viewport.Size);

			this->GlobalRasterizerData.NDCDepths[0] = 255 * (ShaderVariables[0].SV_POSITION.z * 0.5f + 0.5f);
			this->GlobalRasterizerData.NDCDepths[1] = 255 * (ShaderVariables[1].SV_POSITION.z * 0.5f + 0.5f);
			this->GlobalRasterizerData.NDCDepths[2] = 255 * (ShaderVariables[2].SV_POSITION.z * 0.5f + 0.5f);

			this->GlobalRasterizerData.MinTriangleBound.x = min(this->GlobalRasterizerData.NDCs[0].x, this->GlobalRasterizerData.NDCs[1].x, this->GlobalRasterizerData.NDCs[2].x);
			this->GlobalRasterizerData.MinTriangleBound.y = min(this->GlobalRasterizerData.NDCs[0].y, this->GlobalRasterizerData.NDCs[1].y, this->GlobalRasterizerData.NDCs[2].y);
			this->GlobalRasterizerData.MaxTriangleBound.x = max(this->GlobalRasterizerData.NDCs[0].x, this->GlobalRasterizerData.NDCs[1].x, this->GlobalRasterizerData.NDCs[2].x);
			this->GlobalRasterizerData.MaxTriangleBound.y = max(this->GlobalRasterizerData.NDCs[0].y, this->GlobalRasterizerData.NDCs[1].y, this->GlobalRasterizerData.NDCs[2].y);

			this->GlobalRasterizerData.MinTriangleBound.x = (this->GlobalRasterizerData.MinTriangleBound.x < 0 ? 0 : this->GlobalRasterizerData.MinTriangleBound.x);
			this->GlobalRasterizerData.MinTriangleBound.y = (this->GlobalRasterizerData.MinTriangleBound.y < 0 ? 0 : this->GlobalRasterizerData.MinTriangleBound.y);
			this->GlobalRasterizerData.MaxTriangleBound.x = (this->GlobalRasterizerData.MaxTriangleBound.x > Configuration->Rasterizer.Viewport.Position.x + Configuration->Rasterizer.Viewport.Size.x ? 0 : this->GlobalRasterizerData.MaxTriangleBound.x);
			this->GlobalRasterizerData.MaxTriangleBound.y = (this->GlobalRasterizerData.MaxTriangleBound.y > Configuration->Rasterizer.Viewport.Position.y + Configuration->Rasterizer.Viewport.Size.y ? 0 : this->GlobalRasterizerData.MaxTriangleBound.y);
			

			this->GlobalRasterizerData.TriangleAreaMultiplier = 1.0f / static_cast<float>(edge_function(this->GlobalRasterizerData.NDCs[0], this->GlobalRasterizerData.NDCs[1], this->GlobalRasterizerData.NDCs[2]));

			// Prepare batches.
			const auto BatchSizeX = this->GlobalRasterizerData.MaxTriangleBound.x - this->GlobalRasterizerData.MinTriangleBound.x;
			const auto BatchSizeY = (this->GlobalRasterizerData.MaxTriangleBound.y - this->GlobalRasterizerData.MinTriangleBound.y) / this->Workers.size();

			glm::i16vec2 StartData = this->GlobalRasterizerData.MinTriangleBound;
			const glm::i16vec2 ViewportSize = static_cast<glm::i16vec2>(Configuration->Rasterizer.Viewport.Size);

			for (uint8_t i = 0; i < this->Workers.size(); i++)
			{
				const glm::i16vec2 BatchSize = glm::i16vec2(BatchSizeX, BatchSizeY);
				this->WorkerBatches[i] = RasterizerBatch
				{
					.StartData = StartData,
					.BatchSize = BatchSize
				};

				this->WorkersStatus[i] = krxPPipelineTStatus::RENDER;
				this->cvs[i].notify_one();

				StartData.y += BatchSize.y;
			}

			for (uint8_t i = 0; i < this->Workers.size(); i++)
			{
				std::mutex m;
				std::unique_lock<std::mutex> Lock(m);
				auto Status = &this->WorkersStatus[i];
				this->cvs[i].wait(Lock, [Status](){ return *Status == krxPPipelineTStatus::IDLE; });
			}

		}
	}
	void krxPPipelineTriangles::shutdown()
	{
		for (uint8_t i = 0; i < this->Workers.size(); i++)
		{
			this->WorkersStatus[i] = krxPPipelineTStatus::SHUTDOWN;
			this->cvs[i].notify_one();

			std::mutex m;
			std::unique_lock<std::mutex> Lock(m);
			auto Status = &this->WorkersStatus[i];
			this->cvs[i].wait(Lock, [Status](){ return *Status == krxPPipelineTStatus::CLOSED; });
		}
	}

	krxPPipelineTriangles::krxPPipelineTriangles()
	{
		ShaderInterface* fsInterfaces = new ShaderInterface[3];
		this->vsOutputInterfaces = reinterpret_cast<void*>(fsInterfaces);

		for (uint8_t i = 0; i < this->Workers.size(); i++)
		{
			this->WorkersStatus[i] = krxPPipelineTStatus::UNITIALIZED;

			std::mutex m;
			std::unique_lock<std::mutex> Lock(m);
			this->Workers[i] = std::thread(batch_main, &this->WorkerBatches[i], &this->GlobalRasterizerData, reinterpret_cast<ShaderInterface*>(this->vsOutputInterfaces), &this->cvs[i], &this->WorkersStatus[i]);
			this->Workers[i].detach();
			auto Status = &this->WorkersStatus[i];
			this->cvs[i].wait(Lock, [Status](){ return *Status == krxPPipelineTStatus::IDLE; });
		}
	}
	krxPPipelineTriangles::~krxPPipelineTriangles()
	{
		delete[] reinterpret_cast<ShaderInterface*>(this->vsOutputInterfaces);
	}
}