#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "BasicTypes.hpp"
#include <functional>
#include <iostream>
class Swapchain;
class Context;
class RenderTargetView;

class Surface2D
{
public:
    friend class RenderTargetView;
    friend class Swapchain;
    friend class Context;
    TGui32 width{};
    TGui32 height{};
    TextureInternalType type{};
    TextureChannels channels{};
    char* data;
    TGui8 isSwapchainBackbuffer{};
    TGui32 pixelSize{};

    ui8vec4* subData;

    TGui32 preComputedVerticalLineSize{};
private:
    TGui32 computePosition(const i16vec2 pixelCoord)
    {
        return (pixelCoord.y * this->preComputedVerticalLineSize + pixelCoord.x * pixelSize);
    }
public:
    void clear()
    {
        std::memset(this->data, 0, this->preComputedVerticalLineSize * this->height);
    }
    Surface2D() = default;
    ~Surface2D() = default;

    template<typename T>
    void setPixel(const i16vec2 pixelCoord, T value)
    {
        //const TGui32 pos = pixelCoord.y * this->width + pixelCoord.x * pixelSize;
        //*reinterpret_cast<T*>(&this->data[this->computePosition(pixelCoord)]) = std::move(value);
        this->subData[pixelCoord.y * this->width + pixelCoord.x] = value;
    }
public:
    void readPixels(const TGi16 startX, const TGi16 startY, const TGi16 width, const TGi16 height, char* destination)
    {
        for (TGui32 x = startX; x < startX + width; x++)
        {
            for (TGui32 y = startY; y < startY + height; y++)
            {
                std::memcpy(destination, &this->data[this->computePosition(i16vec2(x, y))], this->pixelSize);
                destination += this->pixelSize;
            }
        }
    }
    static Surface2D* create(TGui32 width, TGui32 height, TextureInternalType type, TextureChannels channels)
    {
        Surface2D* ptr = new Surface2D;
        ptr->type = type;
        ptr->width = width;
        ptr->height = height;
        ptr->channels = channels;
        ptr->pixelSize = type.size * channels;
        ptr->preComputedVerticalLineSize = width * ptr->pixelSize;
        {
            ptr->data = new char[width * height * channels * type.size];
            ptr->subData = reinterpret_cast<ui8vec4*>(ptr->data);
        }
        return ptr;
    }
    static void free(Surface2D** surf)
    {
        Surface2D* ptr = *surf;
        {
            delete[] ptr->data;
        }
        delete ptr;

        *surf = nullptr;
    }
};

class Swapchain
{
private:
    friend class Context;
    GC graphicsContext{};
public:
    Surface2D* presentBuffer{};
private:
    XImage* img{};
    Display* dp{};
    Window wnd{};

    void fill(char* const value)
    {        
        auto ptr = reinterpret_cast<TGui32*>(this->presentBuffer->data);
        int val = *reinterpret_cast<TGui32*>(value);

        const TGui32 count = this->presentBuffer->width * this->presentBuffer->height;
        for (TGui32 i = 0; i < count; i++)
        {
            ptr[i] = val;
        }
    }
public:
    Swapchain() = default;
    ~Swapchain() = default;
    void clear(float R, float G, float B, float A)
    {
        char color[4]
        {
            255 * B,
            255 * G,
            255 * R,
            255 * A
        };
        this->fill(color);
    }
    void present()
    {
        XPutImage(this->dp, this->wnd, this->graphicsContext, this->img, 0, 0, 0, 0, this->presentBuffer->width, this->presentBuffer->height);
    }
    static Swapchain* create(Display* dp, Window window, TGui32 width, TGui32 height)
    {
        Swapchain* ptr = new Swapchain;
        ptr->dp = dp;
        ptr->presentBuffer = Surface2D::create(width, height, TEXTURE_INTERNAL_TYPE_TYPELESS8, TEXTURE_CHANNELS_RGBA);
        ptr->presentBuffer->isSwapchainBackbuffer = true;
        ptr->wnd = window;
        ptr->graphicsContext = XCreateGC(ptr->dp, window, 0, nullptr);
        XSync(ptr->dp, 0);

        auto dpDepth = XDefaultDepth(ptr->dp, 0);
        auto dpVis = XDefaultVisual(ptr->dp, 0);

        ptr->img = XCreateImage(ptr->dp, dpVis, dpDepth, ZPixmap, 0, ptr->presentBuffer->data, width, height, 32, 0);

        return ptr;
    }
    Surface2D* getBuffer()
    {
        return this->presentBuffer;
    }
    static void free(Swapchain** swp)
    {
        Swapchain* ptr = *swp;
        ptr->img->data = nullptr;

        XDestroyImage(ptr->img);
        XFreeGC(ptr->dp, ptr->graphicsContext);

        delete ptr;
        *swp = nullptr;
    }
};


class RenderTargetView
{
public:
    using FunctionPtr = void(RenderTargetView::*)(const i16vec2, void*);
    void writeAsSwapchainView(i16vec2 coord, const f32vec4& val)
    {
        //f32vec4 value = *reinterpret_cast<f32vec4*>(val);
        
/*
        char color[4]
        {
            255 * value.z,
            255 * value.y,
            255 * value.x,
            255 * value.w
        };*/
        this->surf->setPixel<ui8vec4>(coord, ui8vec4(255 * val.x, 255 * val.y, 255 * val.z, 255 * val.w));
        //this->surf->setPixel(coord, ui8vec4(255));
        //ui8vec4(255 * val.x, 255 * val.y, 255 * val.z, 255 * val.w);
    }
    void writeAsNormalView(const i16vec2 coord, void* val)
    {
        //this->surf->setPixel(coord, val);
    }
    std::function<void(const i16vec2, void*)> writeToBuffer;
    //FunctionPtr funcStorage;
    RenderTargetViewBinding binding{};
    /*
    void writeToBuffer(const i16vec2 coord, void* val)
    {
        std::invoke(this->funcStorage, this, coord, val);
    }*/
public:
    friend class Context;
    Surface2D* surf{};
    RenderTargetView(Surface2D* ptr, RenderTargetViewBinding binding)
    {
        this->surf = ptr;
        this->binding = binding;
        if (binding == RENDER_TARGET_VIEW_BINDING_NORMAL)
        {
            //this->funcStorage = &RenderTargetView::writeAsNormalView;
            this->writeToBuffer = std::bind(&RenderTargetView::writeAsNormalView, this, std::placeholders::_1, std::placeholders::_2);
        }
        else if (binding == RENDER_TARGET_VIEW_BINDING_PRESENTATION)
        {
            //this->funcStorage = &RenderTargetView::writeAsSwapchainView;
            //this->writeToBuffer = std::bind(&RenderTargetView::writeAsSwapchainView, this, std::placeholders::_1, std::placeholders::_2);
        }
    }
public:
    RenderTargetView() = default;
    ~RenderTargetView() = default;

    static RenderTargetView* create(Surface2D* surface, RenderTargetViewBinding binding)
    {
        return new RenderTargetView(surface, binding);
    }
    
    static void free(RenderTargetView** view)
    {
        delete *view;
        *view = nullptr;
    }
};