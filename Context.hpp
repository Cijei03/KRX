#pragma once
#include "BasicTypes.hpp"
#include "swp.hpp"
#include "Buffer.hpp"
#include "BufferLayout.hpp"
#include "BufferIterator.hpp"
#include "Shaders.hpp"
#include "Rasterizer.hpp"
#include "Viewport.hpp"

#include <functional>

typedef enum
{
    PRIMITIVE_TYPE_POINTS,
    PRIMITIVE_TYPE_LINES,
    PRIMITIVE_TYPE_TRIANGLES,
} PrimitiveType;

typedef enum
{
    RASTERIZER_FACING_CW,
    RASTERIZER_FACING_CCW
} RasterizerFacing;

class Context
{
protected:
    std::unique_ptr<Buffer*[]> bufs = std::make_unique<Buffer*[]>(IMPLEMENTATION_MAX_ATTRIBS_LIMIT);
    std::unique_ptr<BufferLayout*[]> bufsLayouts = std::make_unique<BufferLayout*[]>(IMPLEMENTATION_MAX_ATTRIBS_LIMIT);

    std::unique_ptr<RenderTargetView*[]> RTViews = std::make_unique<RenderTargetView*[]>(IMPLEMENTATION_MAX_OUTPUTS_LIMIT);

    VertexShader* vsPtr{};
    PixelShader* psPtr{};

    std::unique_ptr<ShaderInterface[]> shadersInput;
    std::unique_ptr<ShaderInterface[]> shadersOutput;

    std::unique_ptr<BufferIterator[]> bufIterators = std::make_unique<BufferIterator[]>(3);

    Viewport viewport;
private:
    std::function<void(TGui32)> executeDrawCall;
    std::function<void(TGui32)> makeIteratable;
    std::function<bool(TGi32*)> pixelIsInTriangle;

    static bool checkPixelCW(TGi32* signs)
    {
        return (signs[0] > 0 && signs[1] > 0 && signs[2] > 0);
    }
    static bool checkPixelCCW(TGi32* signs)
    {
        return (signs[0] < 0 && signs[1] < 0 && signs[2] < 0);
    }
    
    void prepareIterators(TGui8 count, TGui32 vStart)
    {
        for (TGui8 i = 0; i < count; i++)
        {
            this->bufIterators[i].makeIteratable(this->bufs, this->bufsLayouts, vStart + i);
        }
    }
    void writePixelResult(i16vec2 pixelCoord, PixelResult& result)
    {
        // Firstly map pixel coord from viewport to texture.
        pixelCoord = pixelCoord + this->viewport.position;
        for (TGui8 i = 0; i < 1; i++)
        {
            /*
            //TGui8 id = this->psPtr->output[i].id;
            if (i == this->swpSurfaceBinded)
            {
                this->RTViews[i]->writeAsSwapchainView(pixelCoord, &result[i]);
                continue;
            }
            this->RTViews[i]->writeAsNormalView(pixelCoord, &result[i]);
            */
            
            //this->RTViews[0]->writeToBuffer(pixelCoord, &result[0]);
        }

        /*
        // Write.
        if (this->swpSurfaceBinded != -1)
        {
            this->RTViews[this->swpSurfaceBinded]->writeAsSwapchainView(pixelCoord, &result[this->swpSurfaceBinded]);
            for (TGui8 i = 0; i < this->psPtr->outputCount; i++)
            {
                if (i != this->swpSurfaceBinded)
                {
                    this->RTViews[this->psPtr->output[i].id]->writeAsNormalView(pixelCoord, &result[this->psPtr->output[i].id]);
                }
            }
        }
        else
        {
            for (TGui8 i = 0; i < this->psPtr->outputCount; i++)
            {
                this->RTViews[this->psPtr->output[i].id]->writeAsNormalView(pixelCoord, &result[this->psPtr->output[i].id]);
            }
        }*/
    }
/*
    void drawPoints(TGui32 vCount)
    {
        i16vec2 pixelCoord;
        for (TGui32 v = 0; v < vCount; v++)
        {
            // Sample buffers.
            this->bufIterators[0].prepareShaderInterface(this->shadersInput[0], *this->vsPtr);

            // Process by shaders.
            {
                // Process by Vertex Shader.
                this->vsPtr->entry(this->shadersInput[0], this->shadersOutput[0], this->tslPosition[0]);

                // Process by Tessellation Control Shader
                // Process by Tessellation Evaluation Shader
                // Process by Geometry Shader

                // Rasterize, process by Pixel Shader and write results.
                if (rasterizePoint(this->tslPosition[0], this->viewport.dimensions, pixelCoord))
                {
                    this->psPtr->passInterface(*this->shadersOutput.get());
                    this->psPtr->entry(this->psResult);
                    //this->psPtr->entry(this->shadersOutput[0], this->outputs);
                    //this->writePixelResult(pixelCoord);
                }
            }
            
            this->bufIterators[0].incrementIterator(1);
        }
    }*/
    /*void drawLines(TGui32 vCount)
    {
        // Declare variables.
        std::array<TGf32, 2> weights;

        for (TGui32 v = 0; v + 2 < vCount; v += 2)
        {
            // Sample buffers.
            this->bufIterators[0].prepareShaderInterface(this->shadersInput[0], *this->vsPtr);
            this->bufIterators[1].prepareShaderInterface(this->shadersInput[1], *this->vsPtr);

            // Process by shaders.
            {
                // Process by Vertex Shader.
                this->vsPtr->entry(this->shadersInput[0], this->shadersOutput[0], this->tslPosition[0]);
                this->vsPtr->entry(this->shadersInput[1], this->shadersOutput[1], this->tslPosition[1]);

                // Process by Tessellation Control Shader
                // Process by Tessellation Evaluation Shader
                // Process by Geometry Shader

                // Rasterize, process by Pixel Shader and write results.
                {
                    // Transform from NDC to screen space.
                    std::array<i16vec2, 2> pixelCoords
                    {
                        NDCToPixelCoord(this->tslPosition[0], this->viewport.dimensions),
                        NDCToPixelCoord(this->tslPosition[1], this->viewport.dimensions)
                    };
                    

                    // Compute line direction.
                    float lineLength = std::move(1 / computeLineLength(pixelCoords[0], pixelCoords[1]));
                    f32vec2 lineDir = std::move(TGVectorCast<TGf32>(pixelCoords[1] - pixelCoords[0]));
                    {                        
                        float highestValue = getHighestValueFromVector2(lineDir);
                        lineDir = lineDir / highestValue;
                    }

                    // Prepare to rasterize.
                    f32vec2 unnormalizedCoords = TGVectorCast<TGf32>(pixelCoords[0]);
                    bool reachedX = false;
                    bool reachedY = false;

                    // Rasterize.
                    while (!reachedX || !reachedY)
                    {
                        unnormalizedCoords = unnormalizedCoords + lineDir;

                        pixelCoords[0] = TGVectorCast<TGi16>(unnormalizedCoords);

                        if (isInScreenCoords(this->viewport.dimensions, pixelCoords[0]))
                        {
                            computeLineBarycentric(lineLength, pixelCoords[0], pixelCoords[1], weights.data());
                            this->psPtr->interpolateByTwo(this->shadersOutput.get(), weights.data());
                            this->psPtr->entry(this->psResult);

                            //this->writePixelResult(pixelCoords[0]);
                        }

                        if (pixelCoords[0].x == pixelCoords[1].x)
                            reachedX = true;
                        if (pixelCoords[0].y == pixelCoords[1].y)
                            reachedY = true;
                    }
                }
            }
            
            this->bufIterators[0].incrementIterator(2);
            this->bufIterators[1].incrementIterator(2);
        }
    }*/
    void drawTriangles(TGui32 vCount)
    {
        ui8vec4* p = this->RTViews[0]->surf->subData;

        // Declare variables.
        std::array<f32vec4, 3> tslPosition;
        std::array<TGf32, 3> weights;
        PixelResult psResult;

        for (TGui32 v = 0; v < vCount; v += 3)
        {
            // Process by shaders.
            {
                // Sample buffers and process by Vertex Shader.
                this->vsPtr->entry(this->vsPtr->generateShaderInterfaceFromBuffer(this->bufIterators[0]), this->shadersOutput[0], tslPosition[0]);
                this->vsPtr->entry(this->vsPtr->generateShaderInterfaceFromBuffer(this->bufIterators[1]), this->shadersOutput[1], tslPosition[1]);
                this->vsPtr->entry(this->vsPtr->generateShaderInterfaceFromBuffer(this->bufIterators[2]), this->shadersOutput[2], tslPosition[2]);

                // Process by Tessellation Control Shader
                // Process by Tessellation Evaluation Shader
                // Process by Geometry Shader

                // Rasterize, process by Pixel Shader and write results.
                {
                    // Transform from NDC to screen space.
                    std::array<i16vec2, 3> pixelCoords
                    {
                        NDCToPixelCoord(tslPosition[0], this->viewport.dimensions),
                        NDCToPixelCoord(tslPosition[1], this->viewport.dimensions),
                        NDCToPixelCoord(tslPosition[2], this->viewport.dimensions)
                    };

                    // Find coords bounds where is triangle.
                    const auto minX = min<TGi32>(pixelCoords[0].x, pixelCoords[1].x, pixelCoords[2].x);
                    const auto minY = min<TGi32>(pixelCoords[0].y, pixelCoords[1].y, pixelCoords[2].y);

                    const auto maxX = max<TGi32>(pixelCoords[0].x, pixelCoords[1].x, pixelCoords[2].x);
                    const auto maxY = max<TGi32>(pixelCoords[0].y, pixelCoords[1].y, pixelCoords[2].y);

                    // Find area of triangle.
                    float triangleAreaMultiplier = 1 / float(edgeFunction(pixelCoords[0], pixelCoords[1], pixelCoords[2]));

                    // Prepare some variables.
                    i16vec2 currentPixel(minX, minY);
                    std::array<TGi32, 3> signs{};

                    while (currentPixel.y < maxY)
                    {
                        currentPixel.x = minX;
                        bool foundTrianglePoint{};
                        while (currentPixel.x < maxX)
                        {
                            signs[0] = edgeFunction(currentPixel, pixelCoords[0], pixelCoords[1]);
                            signs[1] = edgeFunction(currentPixel, pixelCoords[1], pixelCoords[2]);
                            signs[2] = edgeFunction(currentPixel, pixelCoords[2], pixelCoords[0]);

                            //if (this->pixelIsInTriangle(signs.data()))
                            if (this->checkPixelCCW(signs.data()))
                            {
                                foundTrianglePoint = true;

                                weights[0] = TGf32(signs[0]) * triangleAreaMultiplier;
                                weights[1] = TGf32(signs[1]) * triangleAreaMultiplier;
                                weights[2] = TGf32(signs[2]) * triangleAreaMultiplier;

                                //this->psPtr->interpolateByThree(this->shadersOutput.get(), weights.data());
                                this->psPtr->entry(this->shadersOutput.get(), weights.data(), this->RTViews.get()[0], currentPixel, p, this->viewport.dimensions.x);
                            }
                            else if (foundTrianglePoint)
                                break;
                            currentPixel.x++;
                        }
                        currentPixel.y++;
                    }
                }
            }
            this->bufIterators[0].incrementIterator(3);
            this->bufIterators[1].incrementIterator(3);
            this->bufIterators[2].incrementIterator(3);
        }
    }
public:
    Context() = default;
    ~Context() = default;

    void setPrimitiveType(PrimitiveType pType)
    {
        if (pType == PRIMITIVE_TYPE_POINTS)
        {
            //this->executeDrawCall = std::bind(&Context::drawPoints, this, std::placeholders::_1);
            this->makeIteratable = std::bind(&Context::prepareIterators, this, 1, std::placeholders::_1);
            this->shadersInput = std::make_unique<ShaderInterface[]>(1);
            this->shadersOutput = std::make_unique<ShaderInterface[]>(1);
        }
        else if (pType == PRIMITIVE_TYPE_LINES)
        {
            //this->executeDrawCall = std::bind(&Context::drawLines, this, std::placeholders::_1);
            this->makeIteratable = std::bind(&Context::prepareIterators, this, 2, std::placeholders::_1);
            this->shadersInput = std::make_unique<ShaderInterface[]>(2);
            this->shadersOutput = std::make_unique<ShaderInterface[]>(2);
        }
        else if (pType == PRIMITIVE_TYPE_TRIANGLES)
        {
            this->executeDrawCall = std::bind(&Context::drawTriangles, this, std::placeholders::_1);
            this->makeIteratable = std::bind(&Context::prepareIterators, this, 3, std::placeholders::_1);
            this->shadersInput = std::make_unique<ShaderInterface[]>(3);
            this->shadersOutput = std::make_unique<ShaderInterface[]>(3);
        }
    }
    void setRasterizerState(RasterizerFacing facing)
    {
        if (facing == RASTERIZER_FACING_CW)
        {
            this->pixelIsInTriangle = Context::checkPixelCW;
        }
        else if (facing == RASTERIZER_FACING_CCW)
        {
            this->pixelIsInTriangle = Context::checkPixelCCW;
        }
    }
    void bindBuffer(Buffer& ptr, TGui8 bindIndex)
    {
        this->bufs[bindIndex] = &ptr;
    }
    void bindBufferLayout(BufferLayout& ptr, TGui8 bindIndex)
    {
        this->bufsLayouts[bindIndex] = &ptr;
    }
    void bindVertexShader(VertexShader& vs)
    {
        this->vsPtr = &vs;
    }
    void bindPixelShader(PixelShader& ps)
    {
        this->psPtr = &ps;
    }
    void bindRenderTargetView(RenderTargetView& view, TGui8 bindIndex)
    {
        this->RTViews[bindIndex] = &view;
    }
    void bindViewport(const Viewport& viewport)
    {
        this->viewport = viewport;
    }
    void draw(TGui32 vStart, TGui32 vCount)
    {
        // Make buffers iteratable.
        this->makeIteratable(vStart);
        // Draw.
        this->executeDrawCall(vCount);
    }
    void clearBindedTargetViews()
    {
        for (TGui8 i = 0; i < IMPLEMENTATION_MAX_OUTPUTS_LIMIT; i++)
        {
            if (this->RTViews[i] != nullptr)
            {
                this->RTViews[i]->surf->clear();
            }
        }
    }
};