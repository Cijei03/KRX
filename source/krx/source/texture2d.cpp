#include "../include/internals/texture2d.hpp"
#include <cstring>
#include "validation_layer.hpp"

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

void krxTexture2D::blit(const krxTexture2D* TextureToBlit)
{
	if (this->Info.Size.Width != TextureToBlit->Info.Size.Width || this->Info.Size.Height != TextureToBlit->Info.Size.Height)
	{
		krxValidationLayerMessage("TEXTURE2D_blit-BLIT_CAN_BE_PERFORMED_ONLY_ON_TEXTURES_WITH_SAME_WIDTH_AND_HEIGHT");
		return;
	}
	if (this->Info.Format == TextureToBlit->Info.Format)
	{
		std::memcpy(this->Data.data(), TextureToBlit->Data.data(), this->Data.size());
		return;
	}

	if (this->Info.Format == krxFormat::PRESENT_FORMAT)
	{
		if (TextureToBlit->Info.Format == krxFormat::UINT8_RGBA)
		{
			for (uint32_t i = 0; i < this->Data.size(); i += 4)
			{
				this->Data[i] = TextureToBlit->Data[i + 2];
				this->Data[i + 1] = TextureToBlit->Data[i + 1];
				this->Data[i + 2] = TextureToBlit->Data[i];
				this->Data[i + 3] = TextureToBlit->Data[i + 3];				
			}
		}
		else if (TextureToBlit->Info.Format == krxFormat::UINT8_RGB)
		{
			for (uint32_t i = 0, s = 0; s < this->Data.size(); i += 3, s += 4)
			{
				this->Data[s] = TextureToBlit->Data[i + 2];
				this->Data[s + 1] = TextureToBlit->Data[i + 1];
				this->Data[s + 2] = TextureToBlit->Data[i];
				this->Data[s + 3] = 255;
			}
		}
		else
		{
			krxValidationLayerMessage("TEXTURE2D_blit-BLITTING_TO_PRESENT_FORMAT_IS_ALLOWED_ONLY_FROM_RGB_AND_RGBA_FORMATS");
		}
	}
	/*
	else if (this->Info.Format == krxFormat::UINT8_RGB)
	{
		if (TextureToBlit->Info.Format == krxFormat::UINT8_RG)
		{
			for (uint32_t i = 0, s = 0; s < this->Data.size(); i += 2, s += 3)
			{
				this->Data[s] = TextureToBlit->Data[i];
				this->Data[s + 1] = TextureToBlit->Data[i + 1];
			}
		}
		else if (TextureToBlit->Info.Format == krxFormat::UINT8_R)
		{
			for (uint32_t i = 0, s = 0; s < this->Data.size(); i += 1, s += 3)
			{
				this->Data[s] = TextureToBlit->Data[i];
			}
		}
	}
	else if (this->Info.Format == krxFormat::UINT8_RG)
	{
		if (TextureToBlit->Info.Format == krxFormat::UINT8_RGB)
		{
			for (uint32_t i = 0, s = 0; s < this->Data.size(); i += 3, s += 2)
			{
				this->Data[s] = TextureToBlit->Data[i];
				this->Data[s + 1] = TextureToBlit->Data[i + 1];
			}
		}
		else if (TextureToBlit->Info.Format == krxFormat::UINT8_R)
		{
			for (uint32_t i = 0, s = 0; s < this->Data.size(); i += 1, s += 3)
			{
				this->Data[s] = TextureToBlit->Data[i];
			}
		}
	}
	*/
}