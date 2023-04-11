#define GLFW_EXPOSE_NATIVE_X11

#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Context.hpp"

constexpr uint32_t wndWidth = 1280;
constexpr uint32_t wndHeight = 720;

GLFWwindow* wnd{};

Buffer createBuffer()
{
    f32vec3 vertices[6]
    {
        // Position                             // Color
        f32vec3{-0.8f, -0.5f, 0.0f},            f32vec3(1.0f, 0.0f, 0.0f),
        f32vec3{-0.3f, 0.5f, 0.0f},             f32vec3(0.0f, 1.0f, 0.0f),
        f32vec3{0.2f, -0.5f, 0.0f},            f32vec3(0.0f, 0.0f, 1.0f)
    };

    return Buffer::create(vertices, sizeof(vertices));
}
Buffer createBuffer2()
{
    f32vec3 vertices[6]
    {
        // Position                             // Color
        f32vec3{-0.2f, -0.5f, 0.0f},            f32vec3(1.0f, 0.0f, 0.5f),
        f32vec3{0.3f, 0.5f, 0.0f},             f32vec3(1.0f, 0.5f, 1.0f),
        f32vec3{0.8f, -0.5f, 0.0f},              f32vec3(1.0f, 1.0f, 1.0f)
    };

    return Buffer::create(vertices, sizeof(vertices));
}
Buffer createBuffer3()
{
    f32vec3 vertices[6]
    {
        // Position                             // Color
        f32vec3{-0.5f, 0.8f, 0.0f},            f32vec3(0.0f, 0.0f, 1.0f),
        f32vec3{0.5f, 0.8f, 0.0f},             f32vec3(0.0f, 1.0f, 1.0f),
        f32vec3{0.0f, -0.8f, 0.0f},            f32vec3(1.0f, 1.0f, 1.0f)
    };

    return Buffer::create(vertices, sizeof(vertices));
}
BufferLayout createBufferLayout()
{
    BufferLayout cache;
    const auto stride = sizeof(f32vec3) * 2;
    cache.setAttrib(ATTR_TYPE_F32VEC3, 0, 0, stride);
    cache.setAttrib(ATTR_TYPE_F32VEC3, 12, 1, stride);

    return cache;
}

void setTitle(TGui32 fps)
{
    std::stringstream stream;
    stream << "CPU Processing Test | FPS: " << fps;
    glfwSetWindowTitle(wnd, stream.str().data());
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    wnd = glfwCreateWindow(wndWidth, wndHeight, "CPU Processing Test", nullptr, nullptr);

    auto swp = Swapchain::create(glfwGetX11Display(), glfwGetX11Window(wnd), wndWidth, wndHeight);
    auto buf = createBuffer();
    auto buf2 = createBuffer2();
    auto buf3 = createBuffer3();
    auto bufLay = createBufferLayout();
    Context cxt{};
    
    VertexShader vs;
    PixelShader ps;

    cxt.setPrimitiveType(PRIMITIVE_TYPE_TRIANGLES);
    cxt.bindBufferLayout(bufLay, 0);
    cxt.bindVertexShader(vs);
    cxt.bindPixelShader(ps);

    RenderTargetView* view = RenderTargetView::create(swp->getBuffer(), RENDER_TARGET_VIEW_BINDING_PRESENTATION);

    cxt.bindRenderTargetView(*view, 0);

    auto subView = Viewport::create(i16vec2(0, 0), i16vec2(500, 300));
    auto mainView = Viewport::create(i16vec2(0), i16vec2(wndWidth, wndHeight));

    cxt.setRasterizerState(RASTERIZER_FACING_CCW);
    cxt.bindViewport(*mainView);

    while (!glfwWindowShouldClose(wnd))
    {
        static TGui32 fps{};
        glfwPollEvents();
        
        //swp->clear(0.0f, 0.0f, 0.0f, 1.0f);
        //cxt.clearBindedTargetViews();

        cxt.bindBuffer(buf, 0);
        cxt.draw(0, 3);

        cxt.bindBuffer(buf2, 0);
        //cxt.draw(0, 3);

        //cxt.bindViewport(*subView);

        cxt.bindBuffer(buf3, 0);
        //cxt.draw(0, 3);

        //swp->present();

        fps++;
        if (glfwGetTime() >= 1.0)
        {
            setTitle(fps);
            fps = 0;
            glfwSetTime(0.0);
        }
    }

    Swapchain::free(&swp);
    Buffer::free(buf);
    Buffer::free(buf2);
    Buffer::free(buf3);

    RenderTargetView::free(&view);

    Viewport::free(&mainView);
    Viewport::free(&subView);


    glfwTerminate();
}