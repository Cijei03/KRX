#pragma once

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

using krxVertexShader = void(*)(const ShaderInterface* Input, ShaderInterface* Output, VariablesVS* krxslVariables);
using krxFragmentShader = void(*)(const ShaderInterface* Input, ShaderInterface* Output, const float* Weights);