#include "../include/internals/swapchain.hpp"

krxSwapchain::krxSwapchain(Display* dp, Window wnd, const glm::uvec2& Size)
{
	this->dp = dp;
	this->BackBuffer = new krxTexture2D
	(
		krxTextureCreationInfo
		{
			.Format = krxFormat::PRESENT_FORMAT,
			.Size =
			{
				.Width = static_cast<uint16_t>(Size.x),
				.Height = static_cast<uint16_t>(Size.y)
			}
		}
	);
	this->wnd = wnd;
	this->GraphicsContext = XCreateGC(this->dp, this->wnd, 0, nullptr);
	XSync(this->dp, 0);

	auto dpDepth = XDefaultDepth(this->dp, 0);
	auto dpVis = XDefaultVisual(this->dp, 0);

	this->FrontBuffer = XCreateImage
	(
		this->dp, 
		dpVis, 
		dpDepth,
		ZPixmap, 
		0, 
		reinterpret_cast<char*>(this->BackBuffer->Data.data()), 
		this->BackBuffer->Info.Size.Width, 
		this->BackBuffer->Info.Size.Height, 
		32, 
		0
	);
}

void krxSwapchain::present()
{
	XPutImage(this->dp, this->wnd, this->GraphicsContext, this->FrontBuffer, 0, 0, 0, 0, this->BackBuffer->Info.Size.Width, this->BackBuffer->Info.Size.Height);
}

krxTexture2D* const krxSwapchain::get_buffer()
{
	return this->BackBuffer;
}

krxSwapchain::~krxSwapchain()
{
	XDestroyImage(this->FrontBuffer);
	XFreeGC(this->dp, this->GraphicsContext);
	delete this->BackBuffer;
}