#include "../include/internals/processing_pipeline.hpp"
#include "../include/internals/constants.hpp"
#include "../include/internals/context.hpp"
#include "shaders.hpp"
#include <cstring>
#include "rasterization_utils.hpp"

#include <iostream>

static void batch_main(intern::RasterizerBatch* Batch, intern::GlobalRasterizationData* GlobalData, const ShaderInterface* fsInput, bool canprint)
{
	glm::i16vec2 CurrentPixel = Batch->StartData;
	const glm::i16vec2 EndData = CurrentPixel + Batch->BatchSize;
	ShaderInterface FragmentShaderOutput{};
	glm::f32vec3 Weights;
	
	while (CurrentPixel.y < EndData.y)
	{
		CurrentPixel.x = Batch->StartData.x;

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
				Weights = glm::f32vec3
				(
					static_cast<float>(signs[0]) * GlobalData->TriangleAreaMultiplier,
					static_cast<float>(signs[1]) * GlobalData->TriangleAreaMultiplier,
					static_cast<float>(signs[2]) * GlobalData->TriangleAreaMultiplier
				);

				reinterpret_cast<krxFragmentShader>(GlobalData->fs_main)(fsInput, &FragmentShaderOutput, reinterpret_cast<float*>(&Weights));

				for (uint8_t i = 0; i < KRX_IMPLEMENTATION_MAX_RENDER_TARGET_OUTPUT_COUNT; i++)
				{
					const krxFormat TargetFormat = static_cast<krxFormat>(GlobalData->RenderTargetOutputsFormats[i]);

					if (GlobalData->RenderTargetOutputs[i] != nullptr)
					{
						if (TargetFormat == krxFormat::PRESENT_FORMAT)
						{
							const float* Ptr = reinterpret_cast<const float*>(&FragmentShaderOutput[i]);
							const auto PositionInBuffer = reinterpret_cast<glm::u8vec4*>(GlobalData->RenderTargetOutputs[i]);
							PositionInBuffer[CurrentPixel.y * GlobalData->RenderTargetWidths[i] + CurrentPixel.x] = glm::u8vec4(Ptr[2] * 255, Ptr[1] * 255, Ptr[0] * 255, Ptr[3] * 255);
						}
					}
				}
			}
			
			CurrentPixel.x++;
		}
		CurrentPixel.y++;
	}
}

namespace intern
{
	krxPPipelineTriangles::krxPPipelineTriangles(krxContext* Configuration, const uint32_t vStart, const uint32_t vCount)
	{
		if (Configuration->Rasterizer.Facing == krxRasterizerFacing::CCW)
		{
			this->GlobalRasterizerData.check_pixel = check_pixel_ccw;
		}
		else
		{
			this->GlobalRasterizerData.check_pixel = check_pixel_cw;
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

		for (uint32_t v = vStart; v < vStart + vCount; v += 3)
		{
			// Declare krxsl variables.
			ShaderInterface vsInputData[3];
			ShaderInterface vsOutputInterfaces[3];
			ShaderInterface fsInputInterface;

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

				VertexShaderFunc(&vsInputData[vTriangle], &vsOutputInterfaces[vTriangle], &ShaderVariables[vTriangle]);
			}

			// Prepare for rasterization.
			this->GlobalRasterizerData.NDCs[0] = ndc_to_pixel_coord(ShaderVariables[0].SV_POSITION, Configuration->Rasterizer.Viewport.Size);
			this->GlobalRasterizerData.NDCs[1] = ndc_to_pixel_coord(ShaderVariables[1].SV_POSITION, Configuration->Rasterizer.Viewport.Size);
			this->GlobalRasterizerData.NDCs[2] = ndc_to_pixel_coord(ShaderVariables[2].SV_POSITION, Configuration->Rasterizer.Viewport.Size);

			this->GlobalRasterizerData.MinTriangleBound.x = min(this->GlobalRasterizerData.NDCs[0].x, this->GlobalRasterizerData.NDCs[1].x, this->GlobalRasterizerData.NDCs[2].x);
			this->GlobalRasterizerData.MinTriangleBound.y = min(this->GlobalRasterizerData.NDCs[0].y, this->GlobalRasterizerData.NDCs[1].y, this->GlobalRasterizerData.NDCs[2].y);
			this->GlobalRasterizerData.MaxTriangleBound.x = max(this->GlobalRasterizerData.NDCs[0].x, this->GlobalRasterizerData.NDCs[1].x, this->GlobalRasterizerData.NDCs[2].x);
			this->GlobalRasterizerData.MaxTriangleBound.y = max(this->GlobalRasterizerData.NDCs[0].y, this->GlobalRasterizerData.NDCs[1].y, this->GlobalRasterizerData.NDCs[2].y);

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
				this->Workers[i] = std::thread(batch_main, &this->WorkerBatches[i], &this->GlobalRasterizerData, vsOutputInterfaces, (i == 0));

				StartData.y += BatchSize.y;
			}

			for (uint8_t i = 0; i < this->Workers.size(); i++)
			{
				this->Workers[i].join();
			}

		}
	}
}