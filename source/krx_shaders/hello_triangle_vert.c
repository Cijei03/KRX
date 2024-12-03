#include "shader_defs.h"

void vs_main(const krx_ShaderInterface* Input, krx_ShaderInterface* Output, kxr_f32vec4* SV_POSITION)
{
	*SV_POSITION = def_4_22(unpack_f32_2(*Input[0]), def_2_11(0.0f, 1.0f));
}