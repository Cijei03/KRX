#pragma once
#include "BasicTypes.hpp"
#include "Shaders.hpp"
#include <cmath>

i16vec2 NDCToPixelCoord(const f32vec4& pos, const i16vec2 screenDimensions)
{
    return i16vec2((pos.x * 0.5f + 0.5f) * screenDimensions.x, screenDimensions.y - ((pos.y * 0.5f + 0.5f) * screenDimensions.y));
}
bool isInScreenCoords(const i16vec2 screenDimensions, const i16vec2 pixelCoord)
{
    return (pixelCoord >= i16vec2(0) && pixelCoord < screenDimensions);
}

bool rasterizePoint(const f32vec4& tslPosition, const i16vec2 screenDimensions, i16vec2& pixelCoords)
{
    pixelCoords = std::move(NDCToPixelCoord(tslPosition, screenDimensions));
    return isInScreenCoords(screenDimensions, pixelCoords);
}
void interpolateLineInterface(const ShaderInterface* const in, ShaderInterface& out, const PixelShader* const ps, const TGf32* const weights)
{
    auto interpolateShaderCom = [weights](const ShaderCom& a, const ShaderCom& b, ShaderCom& out, const AttrType type) -> void
    {
        if (type == ATTR_TYPE_F32VEC3)
            out.f32v3 = a.f32v3 * weights[0] + b.f32v3 * weights[1];
    };

    //for (TGui8 i = 0; i < ps->inputCount; i++)
    {
        //auto info = ps->input[i];
        //interpolateShaderCom(in[0][info.id], in[1][info.id], out[info.id], info.type);
    }
}
TGf32 computeLineLength(const i16vec2 sourcePixel, const i16vec2 destinationPixel)
{
    return abs(sourcePixel.x - destinationPixel.x) + abs(sourcePixel.y - destinationPixel.y);
}

TGf32 getHighestValueFromVector2(f32vec2 val)
{
    val = std::move(TGabs(val));
    return (val.x > val.y ? val.x : val.y);
}
void computeLineBarycentric(const TGf32 lineLength, const i16vec2 currentPixel, const i16vec2 destinationPixel, TGf32* weights)
{
    weights[0] = computeLineLength(currentPixel, destinationPixel) * lineLength;
    weights[1] = 1.0f - weights[0];
}

template<typename T>
T min(T a, T b, T c)
{
    T cache = (a < b ? a : b);
    return (cache < c ? cache : c);
}
template<typename T>
T max(T a, T b, T c)
{
    T cache = (a > b ? a : b);
    return (cache > c ? cache : c);
}
TGi32 edgeFunction(i16vec2 p, i16vec2 v0, i16vec2 v1)
{
    return ((p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x));
}