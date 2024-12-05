#include "shader_defs.h"

krx_shader_uniform_def TriangleData_t
{
	krx_f32vec4 TriangleColor;
	krx_f32vec3 Offset;
} TriangleData;

void vs_main(const krx_ShaderInterface* Input, krx_ShaderInterface* Output, krxsl_VariablesVS* krxslVariables, const krxShaderUniformInterface Uniforms)
{
	SV_POSITION = def_4_31(add_3(krx_unpack_shader_uniform(TriangleData, 0).Offset, def_3_21(unpack_f32_2(*Input[0]), 0.0f)), 1.0f);
}

void shader_attribs_requirements(uint32_t* InputRequirements)
{
	InputRequirements[0] = sizeof(krx_f32vec2);
}