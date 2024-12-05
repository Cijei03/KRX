#include "shader_defs.h"

void vs_main(const krx_ShaderInterface* Input, krx_ShaderInterface* Output, krxsl_VariablesVS* krxslVariables, const krxShaderUniformInterface Uniforms)
{
	const krx_f32vec3 ColorData[3] = 
	{
		def_3_111(1.0f, 0.0f, 0.0f),
		def_3_111(0.0f, 1.0f, 0.0f),
		def_3_111(0.0f, 0.0f, 1.0f)
	};

	krx_shader_out(krx_f32vec3, 0) = ColorData[SV_VERTEXID];
	SV_POSITION = def_4_22(unpack_f32_2(*Input[0]), def_2_11(0.0f, 1.0f));
}

void shader_attribs_requirements(uint32_t* InputRequirements)
{
	InputRequirements[0] = sizeof(krx_f32vec2);
}

const krxSM SM = 1;