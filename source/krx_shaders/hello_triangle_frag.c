#include "shader_defs.h"

void fs_main(const krx_ShaderInterface* Input, krx_ShaderInterface* Output)
{
	//*(kxr_f32vec4*)&*Output[0] = def_4_1111(1.0f, 1.0f, 1.0f, 1.0f);
	krx_shader_out(kxr_f32vec4, 0) = def_4_1111(1.0f, 1.0f, 1.0f, 1.0f);
}