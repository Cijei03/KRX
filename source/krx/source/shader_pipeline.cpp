#include "../include/internals/pipeline.hpp"
#include <dlfcn.h>

krxShaderPipeline::krxShaderPipeline(const std::vector<krxShaderCreationInfo>& ShadersCreationInfo, const krxShaderStageBitMask StagesBitMask)
{
	for (auto& ShaderInfo : ShadersCreationInfo)
	{
		void* Handle = dlopen(ShaderInfo.Path.data(), RTLD_NOW);

		if (!this->VertexShader.shader_function && (StagesBitMask & krxShaderStageBitMask::VERTEX_SHADER_BIT) && dlsym(Handle, "vs_main"))
		{
			this->VertexShader.shader_function = dlsym(Handle, "vs_main");
		}
		else if (!this->FragmentShader.shader_function && (StagesBitMask & krxShaderStageBitMask::FRAGMENT_SHADER_BIT) && dlsym(Handle, "fs_main"))
		{
			this->FragmentShader.shader_function = dlsym(Handle, "fs_main");
		}

		this->ShaderHandles.push_back(Handle);
	}
}

krxShaderPipeline::~krxShaderPipeline()
{
	for (auto& Handle : this->ShaderHandles)
	{
		dlclose(Handle);
	}
}