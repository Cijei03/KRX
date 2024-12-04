#pragma once
#include <stdint.h>
#include "../krx/include/internals/constants.hpp"

typedef struct krx_ShaderAttribute_t
{
	uint32_t data[4];
} krx_ShaderAttribute;

typedef krx_ShaderAttribute krx_ShaderInterface[KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT];

typedef void* krx_ShaderUniformBuffer;

typedef krx_ShaderUniformBuffer krxShaderUniformInterface[KRX_IMPLEMENTATION_MAX_UNIFORM_TARGET_COUNT];

typedef struct krx_f32vec4_t
{
	float data[4];
} krx_f32vec4;

typedef struct krx_f32vec3_t
{
	float data[3];
} krx_f32vec3;

typedef struct krx_f32vec2_t
{
	float data[2];
} krx_f32vec2;

typedef struct krxsl_VariablesVS_t
{
	krx_f32vec4 SV_POSITION;
	const uint32_t SV_VERTEXID;
} krxsl_VariablesVS;

typedef struct krxsl_VariablesFS_t
{
	//krx_f32vec4 SV_PIXEL_COORD;
} krxsl_VariablesFS;

krx_f32vec4 def_4_31(const krx_f32vec3 xyz, const float w)
{
	const float* vec3 = (const float*)(&xyz);

	krx_f32vec4 cache = 
	{
		.data = { vec3[0], vec3[1], vec3[2], w}
	};

	return cache;
}

krx_f32vec4 def_4_13(const float x, const krx_f32vec3 yzw)
{
	const float* vec3 = (const float*)(&yzw);

	krx_f32vec4 cache = 
	{
		.data = { x, vec3[0], vec3[1], vec3[2]}
	};

	return cache;
}

krx_f32vec4 def_4_22(const krx_f32vec2 xy, const krx_f32vec2 zw)
{
	const float* vec2_0 = (const float*)(&xy);
	const float* vec2_1 = (const float*)(&zw);

	krx_f32vec4 cache = 
	{
		.data = { vec2_0[0], vec2_0[1], vec2_1[0], vec2_1[1] }
	};
	return cache;
}

krx_f32vec4 def_4_121(const float x, const krx_f32vec2 yz, float w)
{
	const float* vec2 = (const float*)(&yz);

	krx_f32vec4 cache = 
	{
		.data = { x, vec2[0], vec2[1], w }
	};
	return cache;
}

krx_f32vec4 def_4_1111(const float x, const float y, const float z, const float w)
{
	krx_f32vec4 cache = 
	{
		.data = {x , y, z, w}
	};
	return cache;
}
krx_f32vec4 def_4_1(const float v)
{
	krx_f32vec4 cache = 
	{
		.data = {v, v, v, v}
	};
	return cache;
}

krx_f32vec3 def_3_21(const krx_f32vec2 xy, const float w)
{
	const float* vec2 = (const float*)(&xy);

	krx_f32vec3 cache = 
	{
		.data = { vec2[0], vec2[1], w }
	};
	return cache;
}

krx_f32vec3 def_3_12(const float x, const krx_f32vec2 yw)
{
	const float* vec2 = (const float*)(&yw);

	krx_f32vec3 cache = 
	{
		.data = { x, vec2[0], vec2[1] }
	};
	return cache;
}

krx_f32vec3 def_3_111(const float x, const float y, const float z)
{
	krx_f32vec3 cache = 
	{
		.data = { x, y, z }
	};
	return cache;
}

krx_f32vec3 def_3_1(const float v)
{
	krx_f32vec3 cache = 
	{
		.data = { v, v, v }
	};
	return cache;
}

krx_f32vec2 def_2_11(const float x, const float y)
{
	krx_f32vec2 cache = 
	{
		.data = { x, y }
	};
	return cache;
}

krx_f32vec2 def_2_1(const float v)
{
	krx_f32vec2 cache = 
	{
		.data = { v, v }
	};
	return cache;
}

krx_f32vec4 mul_4(const krx_f32vec4 v0, const krx_f32vec4 v1)
{
	return def_4_1111
	(
		((const float*)&v0)[0] * ((const float*)&v1)[0],
		((const float*)&v0)[1] * ((const float*)&v1)[1],
		((const float*)&v0)[2] * ((const float*)&v1)[2],
		((const float*)&v0)[3] * ((const float*)&v1)[3]
	);
}
krx_f32vec4 add_4(const krx_f32vec4 v0, const krx_f32vec4 v1)
{
	return def_4_1111
	(
		((const float*)&v0)[0] + ((const float*)&v1)[0],
		((const float*)&v0)[1] + ((const float*)&v1)[1],
		((const float*)&v0)[2] + ((const float*)&v1)[2],
		((const float*)&v0)[3] + ((const float*)&v1)[3]
	);
}
krx_f32vec4 sub_4(const krx_f32vec4 v0, const krx_f32vec4 v1)
{
	return def_4_1111
	(
		((const float*)&v0)[0] - ((const float*)&v1)[0],
		((const float*)&v0)[1] - ((const float*)&v1)[1],
		((const float*)&v0)[2] - ((const float*)&v1)[2],
		((const float*)&v0)[3] - ((const float*)&v1)[3]
	);
}
krx_f32vec4 div_4(const krx_f32vec4 v0, const krx_f32vec4 v1)
{
	return def_4_1111
	(
		((const float*)&v0)[0] / ((const float*)&v1)[0],
		((const float*)&v0)[1] / ((const float*)&v1)[1],
		((const float*)&v0)[2] / ((const float*)&v1)[2],
		((const float*)&v0)[3] / ((const float*)&v1)[3]
	);
}


krx_f32vec3 mul_3(const krx_f32vec3 v0, const krx_f32vec3 v1)
{
	return def_3_111
	(
		((const float*)&v0)[0] * ((const float*)&v1)[0],
		((const float*)&v0)[1] * ((const float*)&v1)[1],
		((const float*)&v0)[2] * ((const float*)&v1)[2]
	);
}
krx_f32vec3 add_3(const krx_f32vec3 v0, const krx_f32vec3 v1)
{
	return def_3_111
	(
		((const float*)&v0)[0] + ((const float*)&v1)[0],
		((const float*)&v0)[1] + ((const float*)&v1)[1],
		((const float*)&v0)[2] + ((const float*)&v1)[2]
	);
}
krx_f32vec3 sub_3(const krx_f32vec3 v0, const krx_f32vec3 v1)
{
	return def_3_111
	(
		((const float*)&v0)[0] - ((const float*)&v1)[0],
		((const float*)&v0)[1] - ((const float*)&v1)[1],
		((const float*)&v0)[2] - ((const float*)&v1)[2]
	);
}
krx_f32vec3 div_3(const krx_f32vec3 v0, const krx_f32vec3 v1)
{
	return def_3_111
	(
		((const float*)&v0)[0] / ((const float*)&v1)[0],
		((const float*)&v0)[1] / ((const float*)&v1)[1],
		((const float*)&v0)[2] / ((const float*)&v1)[2]
	);
}


krx_f32vec2 mul_2(const krx_f32vec2 v0, const krx_f32vec2 v1)
{
	return def_2_11
	(
		((const float*)&v0)[0] * ((const float*)&v1)[0],
		((const float*)&v0)[1] * ((const float*)&v1)[1]
	);
}
krx_f32vec2 add_2(const krx_f32vec2 v0, const krx_f32vec2 v1)
{
	return def_2_11
	(
		((const float*)&v0)[0] + ((const float*)&v1)[0],
		((const float*)&v0)[1] + ((const float*)&v1)[1]
	);
}
krx_f32vec2 sub_2(const krx_f32vec2 v0, const krx_f32vec2 v1)
{
	return def_2_11
	(
		((const float*)&v0)[0] - ((const float*)&v1)[0],
		((const float*)&v0)[1] - ((const float*)&v1)[1]
	);
}
krx_f32vec2 div_2(const krx_f32vec2 v0, const krx_f32vec2 v1)
{
	return def_2_11
	(
		((const float*)&v0)[0] / ((const float*)&v1)[0],
		((const float*)&v0)[1] / ((const float*)&v1)[1]
	);
}

krx_f32vec4 unpack_f32_4(const krx_ShaderAttribute Attr)
{
	const float* Data = (const float*)(&Attr);
	return def_4_1111(Data[0], Data[1], Data[2], Data[3]);
}
krx_f32vec3 unpack_f32_3(const krx_ShaderAttribute Attr)
{
	const float* Data = (const float*)(&Attr);
	return def_3_111(Data[0], Data[1], Data[2]);
}
krx_f32vec2 unpack_f32_2(const krx_ShaderAttribute Attr)
{
	const float* Data = (const float*)(&Attr);
	return def_2_11(Data[0], Data[1]);
}

krx_f32vec4 barycentric_interpolation_f32_4(const krx_ShaderInterface* fsInput, const float* Weights, const uint32_t AttributeIndex)
{
	const krx_f32vec4 v0 = unpack_f32_4(fsInput[0][AttributeIndex]);
	const krx_f32vec4 v1 = unpack_f32_4(fsInput[1][AttributeIndex]);
	const krx_f32vec4 v2 = unpack_f32_4(fsInput[2][AttributeIndex]);

	return add_4(add_4(mul_4(v0 , def_4_1(Weights[0])), mul_4(v1 , def_4_1(Weights[1]))), mul_4(v2 , def_4_1(Weights[2])));   
}

krx_f32vec3 barycentric_interpolation_f32_3(const krx_ShaderInterface* fsInput, const float* Weights, const uint32_t AttributeIndex)
{
	const krx_f32vec3 v0 = unpack_f32_3(fsInput[0][AttributeIndex]);
	const krx_f32vec3 v1 = unpack_f32_3(fsInput[1][AttributeIndex]);
	const krx_f32vec3 v2 = unpack_f32_3(fsInput[2][AttributeIndex]);

	return add_3(add_3(mul_3(v0, def_3_1(Weights[0])), mul_3(v1 , def_3_1(Weights[1]))), mul_3(v2 , def_3_1(Weights[2])));   
}

krx_f32vec2 barycentric_interpolation_f32_2(const krx_ShaderInterface* fsInput, const float* Weights, const uint32_t AttributeIndex)
{
	const krx_f32vec2 v0 = unpack_f32_2(fsInput[0][AttributeIndex]);
	const krx_f32vec2 v1 = unpack_f32_2(fsInput[1][AttributeIndex]);
	const krx_f32vec2 v2 = unpack_f32_2(fsInput[2][AttributeIndex]);

	return add_2(add_2(mul_2(v0, def_2_1(Weights[0])), mul_2(v1 , def_2_1(Weights[1]))), mul_2(v2 , def_2_1(Weights[2])));   
}

#define krx_shader_out(Type, Index) *(Type*)&*Output[Index]

#define SV_POSITION krxslVariables->SV_POSITION
#define SV_VERTEXID krxslVariables->SV_VERTEXID