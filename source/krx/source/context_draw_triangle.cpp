#include <cstdint>
#include "../include/internals/shaders.hpp"
#include "../include/internals/context.hpp"
#include <cstring>
#include "shaders.hpp"
#include <iostream>

void krxContext::draw_triangle(const uint32_t VertexStart, const uint32_t VertexCount)
{
	//std::array<intern::ShaderAttribute

	std::array<const uint8_t*, KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT> InputFromBuffers{};
	std::array<uint32_t, KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT> JumpLengths = this->PipelineLayout->AttributesSize;

	krxVertexShader VertexShaderFunc = reinterpret_cast<krxVertexShader>(this->ShaderPipeline->VertexShader.shader_function);	

	for (uint32_t i = 0; i < KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT; i++)
	{
		if (this->PipelineLayout->Attributes[i] != nullptr)
		{
			InputFromBuffers[i] = reinterpret_cast<krxBuffer*>(this->PipelineLayout->Attributes[i]->Info.Resource)->Data.data();
		}
	}


	for (uint32_t v = 0; v < VertexCount; v += 3)
	{
		intern::ShaderInterface vsInputData[3];
		intern::ShaderInterface vsOutputData[3];
		glm::vec4 SV_POSITION[3];

		for (uint32_t vTriangle = 0; vTriangle < 3; vTriangle++)
		{			
			for (uint32_t i = 0; i < KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT; i++)
			{
				if (InputFromBuffers[i] != nullptr)
				{
					std::memcpy(&vsInputData[vTriangle][i], &InputFromBuffers[i], JumpLengths[i]);

					InputFromBuffers[i] += JumpLengths[i];
				}
			}

			VertexShaderFunc(&vsInputData[vTriangle], &vsOutputData[vTriangle], &SV_POSITION[vTriangle]);
			std::cout << SV_POSITION[vTriangle][0] << " " << SV_POSITION[vTriangle][1] << " " << SV_POSITION[vTriangle][2] << std::endl;
		}





		//intern::ShaderAttribute
	}
}