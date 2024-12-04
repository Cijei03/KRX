#include "shader_defs.h"

void fs_main(const krx_ShaderInterface* Input, krx_ShaderInterface* Output, const float* Weights)
{
	krx_shader_out(krx_f32vec4, 0) = def_4_31(barycentric_interpolation_f32_3(Input, Weights, 0), 1.0f);
}