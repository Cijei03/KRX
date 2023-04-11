#pragma once
#include <cstdint>
#include <cstddef>
#include <array>
#include <TGMath/TGMath.hpp>

using namespace TGMath;

constexpr TGi32 IMPLEMENTATION_MAX_ATTRIBS_LIMIT = 16;
constexpr TGi32 IMPLEMENTATION_MAX_OUTPUTS_LIMIT = 8;

typedef enum
{
	ATTR_TYPE_F32 = 1,
	ATTR_TYPE_F32VEC2,
	ATTR_TYPE_F32VEC3,
	ATTR_TYPE_F32VEC4,

	ATTR_TYPE_F64,
	ATTR_TYPE_F64VEC2,
	ATTR_TYPE_F64VEC3,
	ATTR_TYPE_F64VEC4,

	ATTR_TYPE_UI32,
	ATTR_TYPE_UI32VEC2,
	ATTR_TYPE_UI32VEC3,
	ATTR_TYPE_UI32VEC4,

	ATTR_TYPE_I32,
	ATTR_TYPE_I32VEC2,
	ATTR_TYPE_I32VEC3,
	ATTR_TYPE_I32VEC4
} AttrType;

// UDF = Universal Data Format
namespace UDF
{
    enum Type
    {
        FLOAT,
        INT,
        UINT,
        TYPELESS
    };
    enum DataChannels
    {
        // No Channel
        NC,
        R,
        RG,
        RGB,
        RGBA,
    };

    struct DataFormat
    {
        TGui8 type;
        TGui8 size;
        TGui8 channels;
    };
}
/*
typedef enum
{
    TEXTURE_INTERNAL_TYPE_FLOAT32,
    TEXTURE_INTERNAL_TYPE_FLOAT64,
    TEXTURE_INTERNAL_TYPE_INT32,
    TEXTURE_INTERNAL_TYPE_UINT32,
	TEXTURE_INTERNAL_TYPE_UINT16,
	TEXTURE_INTERNAL_TYPE_UINT8
} TextureInternalType;*/

//template<DataType T, DataBytesSize S>
//using TextureInternalType = DataFormat<T, S, DATA_CHANNELS_R>;

struct TextureInternalType
{
    UDF::Type type;
    TGui8 size;
};

constexpr auto TEXTURE_INTERNAL_TYPE_FLOAT32 = TextureInternalType(UDF::FLOAT, 4);
constexpr auto TEXTURE_INTERNAL_TYPE_FLOAT64 = TextureInternalType(UDF::FLOAT, 8);

constexpr auto TEXTURE_INTERNAL_TYPE_UINT8 = TextureInternalType(UDF::UINT, 1);
constexpr auto TEXTURE_INTERNAL_TYPE_UINT16 = TextureInternalType(UDF::UINT, 2);
constexpr auto TEXTURE_INTERNAL_TYPE_UINT32 = TextureInternalType(UDF::UINT, 4);
constexpr auto TEXTURE_INTERNAL_TYPE_UINT64 = TextureInternalType(UDF::UINT, 8);

constexpr auto TEXTURE_INTERNAL_TYPE_TYPELESS8 = TextureInternalType(UDF::TYPELESS, 1);
constexpr auto TEXTURE_INTERNAL_TYPE_TYPELESS16 = TextureInternalType(UDF::TYPELESS, 2);
constexpr auto TEXTURE_INTERNAL_TYPE_TYPELESS32 = TextureInternalType(UDF::TYPELESS, 4);
constexpr auto TEXTURE_INTERNAL_TYPE_TYPELESS64 = TextureInternalType(UDF::TYPELESS, 8);


typedef enum
{
    TEXTURE_CHANNELS_R = 1,
    TEXTURE_CHANNELS_RG,
    TEXTURE_CHANNELS_RGB,
    TEXTURE_CHANNELS_RGBA
} TextureChannels;

union ShaderCom
{
    TGf32 f32;
    f32vec2 f32v2;
    f32vec3 f32v3;
    f32vec4 f32v4;

    TGf64 f64;
    f64vec2 f64v2;
    f64vec3 f64v3;
    f64vec4 f64v4;

    TGui32 ui32;
};

typedef enum
{
    RENDER_TARGET_VIEW_BINDING_PRESENTATION,
    RENDER_TARGET_VIEW_BINDING_NORMAL
} RenderTargetViewBinding;

using ShaderInterface = std::array<ShaderCom, IMPLEMENTATION_MAX_ATTRIBS_LIMIT>;
using PixelResult = std::array<ShaderCom, IMPLEMENTATION_MAX_OUTPUTS_LIMIT>;