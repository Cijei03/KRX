#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <glm/glm.hpp>
#include "texture2d.hpp"

class krxSwapchain
{
private:
	GC GraphicsContext{};
	krxTexture2D* BackBuffer = nullptr;
	XImage* FrontBuffer{};
	Display* dp{};
	Window wnd{};
public:
	krxSwapchain(Display* dp, Window wnd, const glm::uvec2& Size);
	~krxSwapchain();

	void present();
	krxTexture2D* const get_buffer();
};