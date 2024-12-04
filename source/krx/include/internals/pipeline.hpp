#pragma once
#include <string>
#include "processing_pipeline.hpp"
#include "constants.hpp"

class krxShaderPipeline;
class krxContext;

struct krxShaderModule
{
private:
	friend class krxContext;
	friend class krxShaderPipeline;
	friend class intern::krxPPipelineTriangles;
	void* shader_function = nullptr;
	uint32_t InputSizeRequirements[KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT]{};
};

enum krxShaderStageBitMask : uint8_t
{
	VERTEX_SHADER_BIT = 1ul << 0,
	FRAGMENT_SHADER_BIT = 1ul << 1
};

struct krxShaderCreationInfo
{
	std::string Path;
};

class krxShaderPipeline
{
private:
	friend class krxContext;
	friend class intern::krxPPipelineTriangles;
	krxShaderModule VertexShader;
	krxShaderModule FragmentShader;

	std::vector<void*> ShaderHandles;
public:
	krxShaderPipeline(const std::vector<krxShaderCreationInfo>& ShadersCreationInfo, const krxShaderStageBitMask StagesBitMask);
	~krxShaderPipeline();
};