#pragma once
#include "BasicTypes.hpp"
#include <memory>
#include <string>
#include "BufferIterator.hpp"
#include "swp.hpp"

/* 
SHADERS SOURCE CODE

#shaderModel TSL_100
#shaderType VERTEX_SHADER

attr(0, in) f32vec3 vPos;
attr(1, in) f32vec3 vColor nodiscard;

void entry()
{
    tslPosition = f32vec4(vPos, 1.0f);
}

#shaderType PIXEL_SHADER

attr(1, in) f32vec3 vColor;

attr(0, out) f32vec4 PixelColor;

void entry()
{
    PixelColor = f32vec4(vColor, 1.0f);
}
*/

struct ExpectedCom
{
    AttrType type{};
    TGui8 id{};
};

class BasicShader
{
public:
    std::unique_ptr<ExpectedCom[]> input;
    std::unique_ptr<ExpectedCom[]> output;

    TGui8 inputCount;
    TGui8 outputCount;
};

class VertexShader : public BasicShader
{
public:
    ShaderInterface generateShaderInterfaceFromBuffer(const BufferIterator& itr)
    {
        ShaderInterface cache;
        cache[0].f32v3 = *reinterpret_cast<f32vec3*>(itr.infos[0].bufPtr);
        cache[1].f32v3 = *reinterpret_cast<f32vec3*>(itr.infos[1].bufPtr);
        return cache;
    }
    void entry(const ShaderInterface& in, ShaderInterface& out, f32vec4& tslPosition)
    {
        tslPosition = f32vec4(in[0].f32v3, 1.0f);
        out[1].f32v3 = in[1].f32v3;
    }
public:
    VertexShader()
    {
        this->inputCount = 2;
        this->outputCount = 1;

        this->input = std::make_unique<ExpectedCom[]>(this->inputCount);
        this->output = std::make_unique<ExpectedCom[]>(this->outputCount);

        this->input[0] = ExpectedCom(ATTR_TYPE_F32VEC3, 0);
        this->input[1] = ExpectedCom(ATTR_TYPE_F32VEC3, 1);

        this->output[0] = ExpectedCom(ATTR_TYPE_F32VEC3, 1);
    }
    ~VertexShader() = default;
};


class PixelShader
{
public:
    std::unique_ptr<ExpectedCom[]> output;
    TGui8 outputCount;
    void passInterface(const ShaderInterface& previousStageOutput, ShaderInterface& output)
    {
        output = previousStageOutput;
    }
    void interpolateByTwo(const ShaderInterface* const previousStageOutput, const TGf32* const weights, ShaderInterface& output)
    {
        output[1].f32v3 = previousStageOutput[0][1].f32v3 * weights[0] + previousStageOutput[1][1].f32v3;
    }
    void interpolateByThree(const ShaderInterface* const previousStageOutput, const TGf32* const weights, ShaderInterface& output)
    {
        output[1].f32v3 = previousStageOutput[0][1].f32v3 * weights[0] + previousStageOutput[1][1].f32v3 * weights[1] + previousStageOutput[2][1].f32v3 * weights[2];
    }
    void entry(const ShaderInterface* const previousStageOutput, const TGf32* const weights, RenderTargetView* views, const i16vec2 pixelCoord, ui8vec4* dst, TGui32 width)
    {
        ShaderInterface output;
        interpolateByThree(previousStageOutput, weights, output);

        auto result = f32vec4(output[1].f32v3, 1.0f);
        if (views[0].binding == RENDER_TARGET_VIEW_BINDING_PRESENTATION)
        {
            views[0].writeAsSwapchainView(pixelCoord, result);
        }
    }
public:
    PixelShader()
    {
        this->outputCount = 1;
        this->output = std::make_unique<ExpectedCom[]>(this->outputCount);
        this->output[0] = ExpectedCom(ATTR_TYPE_F32VEC3, 0);
    }
    ~PixelShader() = default;
};