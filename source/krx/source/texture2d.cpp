#include "../include/internals/texture2d.hpp"

krxTexture2D::krxTexture2D(const krxTextureCreationInfo& CreationInfo)
{
	this->Info = CreationInfo;

	uint32_t BytesToAllocate = CreationInfo.Size.Width * CreationInfo.Size.Height;
	if (CreationInfo.Format == krxFormat::PRESENT_FORMAT || CreationInfo.Format == krxFormat::UINT8_RGBA)
	{
		BytesToAllocate *= 4;
	}
	else if (CreationInfo.Format == krxFormat::UINT8_RGB)
	{
		BytesToAllocate *= 3;
	}
	else if (CreationInfo.Format == krxFormat::UINT8_RG)
	{
		BytesToAllocate *= 2;
	}

	this->Data.resize(BytesToAllocate);
}

krxTexture2D::~krxTexture2D()
{
	
}