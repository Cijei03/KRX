#pragma once
#include "shaders.hpp"
#include <string>

struct krxShaderCreationInfo
{
	std::string Path;
};

class krxContext;

class krxShaderPipeline
{
private:
	friend class krxContext;
	krxShaderModule VertexShader;
	krxShaderModule FragmentShader;

	std::vector<void*> ShaderHandles;
public:
	krxShaderPipeline(const std::vector<krxShaderCreationInfo>& ShadersCreationInfo, const krxShaderStageBitMask StagesBitMask);
	~krxShaderPipeline();
};