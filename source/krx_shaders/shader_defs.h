#pragma once
#include <stdint.h>
#include "../krx/include/internals/constants.hpp"

typedef struct kxr_ShaderAttribute_t
{
	uint32_t data[4];
} kxr_ShaderAttribute;

typedef kxr_ShaderAttribute krx_ShaderInterface[KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT];

typedef void* kxr_ShaderUniformBuffer;

typedef kxr_ShaderUniformBuffer krxShaderUniformInterface[KRX_IMPLEMENTATION_MAX_UNIFORM_TARGET_COUNT];

typedef struct kxr_f32vec4_t
{
	float data[4];
} kxr_f32vec4;

typedef struct kxr_f32vec3_t
{
	float data[3];
} kxr_f32vec3;

typedef struct kxr_f32vec2_t
{
	float data[2];
} kxr_f32vec2;

kxr_f32vec4 def_4_31(const kxr_f32vec3 xyz, const float w)
{
	const float* vec3 = (const float*)(&xyz);

	kxr_f32vec4 cache = 
	{
		.data = { vec3[0], vec3[1], vec3[2], w}
	};

	return cache;
}

kxr_f32vec4 def_4_13(const float x, const kxr_f32vec3 yzw)
{
	const float* vec3 = (const float*)(&yzw);

	kxr_f32vec4 cache = 
	{
		.data = { x, vec3[0], vec3[1], vec3[2]}
	};

	return cache;
}

kxr_f32vec4 def_4_22(const kxr_f32vec2 xy, const kxr_f32vec2 zw)
{
	const float* vec2_0 = (const float*)(&xy);
	const float* vec2_1 = (const float*)(&zw);

	kxr_f32vec4 cache = 
	{
		.data = { vec2_0[0], vec2_0[1], vec2_1[0], vec2_1[1] }
	};
	return cache;
}

kxr_f32vec4 def_4_121(const float x, const kxr_f32vec2 yz, float w)
{
	const float* vec2 = (const float*)(&yz);

	kxr_f32vec4 cache = 
	{
		.data = { x, vec2[0], vec2[1], w }
	};
	return cache;
}

kxr_f32vec4 def_4_1111(const float x, const float y, const float z, const float w)
{
	kxr_f32vec4 cache = 
	{
		.data = {x , y, z, w}
	};
	return cache;
}

kxr_f32vec3 def_3_21(const kxr_f32vec2 xy, const float w)
{
	const float* vec2 = (const float*)(&xy);

	kxr_f32vec3 cache = 
	{
		.data = { vec2[0], vec2[1], w }
	};
	return cache;
}

kxr_f32vec3 def_3_12(const float x, const kxr_f32vec2 yw)
{
	const float* vec2 = (const float*)(&yw);

	kxr_f32vec3 cache = 
	{
		.data = { x, vec2[0], vec2[1] }
	};
	return cache;
}

kxr_f32vec3 def_3_111(const float x, const float y, const float z)
{
	kxr_f32vec3 cache = 
	{
		.data = { x, y, z }
	};
	return cache;
}

kxr_f32vec2 def_2_11(const float x, const float y)
{
	kxr_f32vec2 cache = 
	{
		.data = { x, y }
	};
	return cache;
}



kxr_f32vec4 unpack_f32_4(const kxr_ShaderAttribute Attr)
{
	const float* Data = (const float*)(&Attr);
	return def_4_1111(Data[0], Data[1], Data[2], Data[3]);
}
kxr_f32vec3 unpack_f32_3(const kxr_ShaderAttribute Attr)
{
	const float* Data = (const float*)(&Attr);
	return def_3_111(Data[0], Data[1], Data[2]);
}
kxr_f32vec2 unpack_f32_2(const kxr_ShaderAttribute Attr)
{
	const float* Data = (const float*)(&Attr);
	return def_2_11(Data[0], Data[1]);
}

#define krx_shader_out(Type, Index) *(Type*)&*Output[Index]
	