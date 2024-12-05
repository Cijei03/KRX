#include "../include/internals/pipeline.hpp"
#include <dlfcn.h>
#include "validation_layer.hpp"

using ShaderRequirements = void(*)(uint32_t* InputRequirements);

krxShaderPipelineSM1::krxShaderPipelineSM1(const std::vector<krxShaderCreationInfo>& ShadersCreationInfo, const krxShaderStageBitMask StagesBitMask)
{
	const krxSM ReferenceSM = 1;
	for (auto& ShaderInfo : ShadersCreationInfo)
	{
		void* Handle = dlopen(ShaderInfo.Path.data(), RTLD_NOW);

		if (*reinterpret_cast<krxSM*>(dlsym(Handle, "SM")) != ReferenceSM)
		{
			krxValidationLayerMessage("SHADER_PIPELINE_SM1-CLASS_SHADER_PIPELINE_SM1_CAN_TAKE_ONLY_SHADER_WITH_krxSM1");
			throw;
		}

		if (!this->VertexShader.shader_function && (StagesBitMask & krxShaderStageBitMask::VERTEX_SHADER_BIT) && dlsym(Handle, "vs_main"))
		{
			this->VertexShader.shader_function = dlsym(Handle, "vs_main");
			reinterpret_cast<ShaderRequirements>(dlsym(Handle, "shader_attribs_requirements"))(this->VertexShader.InputSizeRequirements);
			

		}
		else if (!this->FragmentShader.shader_function && (StagesBitMask & krxShaderStageBitMask::FRAGMENT_SHADER_BIT) && dlsym(Handle, "fs_main"))
		{
			this->FragmentShader.shader_function = dlsym(Handle, "fs_main");
		}

		this->ShaderHandles.push_back(Handle);
	}
}

krxShaderPipelineSM1::~krxShaderPipelineSM1()
{
	for (auto& Handle : this->ShaderHandles)
	{
		dlclose(Handle);
	}
}