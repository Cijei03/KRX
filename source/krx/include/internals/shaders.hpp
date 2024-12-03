#pragma once
#include <cstdint>
#include "constants.hpp"
#include <glm/glm.hpp>

namespace intern
{
	struct ShaderAttribute
	{
	private:
		uint32_t Data[4];
	public:
		ShaderAttribute() = default;
		~ShaderAttribute() = default;
	};

	using ShaderInterface = ShaderAttribute[KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT];
}

enum krxShaderStageBitMask : uint8_t
{
	VERTEX_SHADER_BIT = 1ul << 0,
	FRAGMENT_SHADER_BIT = 1ul << 1
};

class krxShaderPipeline;
class krxContext;

struct krxShaderModule
{
private:
	friend class krxContext;
	friend class krxShaderPipeline;
	void* shader_function = nullptr;
};