#include "shader_defs.h"

krx_shader_uniform_def TriangleData_t
{
	krx_f32vec4 TriangleColor;
	krx_f32vec3 Offset;
} TriangleData;

void fs_main(const krx_ShaderInterface* Input, krx_ShaderInterface* Output, const float* Weights, const krxShaderUniformInterface Uniforms)
{
	krx_shader_out(krx_f32vec4, 0) = krx_unpack_shader_uniform(TriangleData, 0).TriangleColor;
}

const krxSM SM = 1;