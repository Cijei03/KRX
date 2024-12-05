#pragma once
#include <cstdint>

struct ShaderAttribute
{
private:
	uint32_t Data[4];
public:
	ShaderAttribute() = default;
	~ShaderAttribute() = default;
};

using ShaderInterface = ShaderAttribute[KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT];

struct VariablesVS
{
	glm::vec4 SV_POSITION;
	const uint32_t SV_VERTEXID;
};

using krx_ShaderUniformBuffer = uint8_t*;
using krxShaderUniformInterface = krx_ShaderUniformBuffer[KRX_IMPLEMENTATION_MAX_UNIFORM_TARGET_COUNT];

using krxVertexShader = void(*)(const ShaderInterface* Input, ShaderInterface* Output, VariablesVS* krxslVariables, const krxShaderUniformInterface* Uniforms);
using krxFragmentShader = void(*)(const ShaderInterface* Input, ShaderInterface* Output, const float* Weights, const krxShaderUniformInterface* Uniforms);