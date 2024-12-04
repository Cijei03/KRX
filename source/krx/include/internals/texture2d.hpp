#pragma once
#include "krx_resource.hpp"
#include "processing_pipeline.hpp"

enum class krxFormat : uint32_t
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
	friend class intern::krxPPipelineTriangles;
	krxTextureCreationInfo Info;

public:
	krxTexture2D(const krxTextureCreationInfo& CreationInfo);
	virtual ~krxTexture2D() = default;

	void blit(const krxTexture2D* TextureToBlit);
}; 