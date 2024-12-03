#pragma once
#include "krx_resource.hpp"

enum class krxFormat
{
	PRESENT_FORMAT,
	UINT8_RGBA,
	UINT8_RGB,
	UINT8_RG,
	UINT8_R,
};

struct krxTextureCreationInfo
{
	krxFormat Format;
	struct
	{
		uint16_t Width;
		uint16_t Height;
	} Size;
};

class krxSwapchain;
class krxContext;

class krxTexture2D : public krxResource
{
protected:
	friend class krxSwapchain;
	friend class krxContext;
	krxTextureCreationInfo Info;

public:
	krxTexture2D(const krxTextureCreationInfo& CreationInfo);
	~krxTexture2D();
}; 