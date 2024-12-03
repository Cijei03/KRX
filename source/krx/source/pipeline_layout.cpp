#include "../include/internals/pipeline_layout.hpp"
#include "validation_layer.hpp"
#include <type_traits>
#include <cstring>

krxPipelineLayout::krxPipelineLayout(const std::vector<krxViewInfo>& Infos, std::vector<uint32_t> AttributesSize)
{
	for (auto& ViewInfo : Infos)
	{
		if (dynamic_cast<krxTexture2D*>(ViewInfo.View->Info.Resource))
		{
			auto& Value = std::get<krxTextureTarget>(ViewInfo.View->Info.Target);
			
			if (Value == krxTextureTarget::COLOR_RENDER_TARGET_OUTPUT)
			{
				if (this->RenderTargetOutputs[ViewInfo.Index] == nullptr)
				{
					this->RenderTargetOutputs[ViewInfo.Index] = ViewInfo.View;
				}
				else
				{
					krxValidationLayerMessage("PIPELINE_LAYOUT_krxPipelineLayout-COLOR_RENDER_TARGET_OUTPUT_SLOT_ALREADY_FILLED");
				}
			}
			else if (Value == krxTextureTarget::DEPTH_RENDER_TARGET_OUTPUT)
			{
				if (this->DepthBuffer == nullptr)
				{
					this->DepthBuffer = ViewInfo.View;
				}
				else
				{
					krxValidationLayerMessage("PIPELINE_LAYOUT_krxPipelineLayout-DEPTH_RENDER_TARGET_OUTPUT_SLOT_ALREADY_FILLED");
				}
			}
		}
		else if (dynamic_cast<krxBuffer*>(ViewInfo.View->Info.Resource))
		{
			auto& Value = std::get<krxBufferTarget>(ViewInfo.View->Info.Target);

			if (Value == krxBufferTarget::ATTRIBUTE_TARGET)
			{
				if (this->Attributes[ViewInfo.Index] == nullptr)
				{
					this->Attributes[ViewInfo.Index] = ViewInfo.View;
				}
				else
				{
					krxValidationLayerMessage("PIPELINE_LAYOUT_krxPipelineLayout-ATTRIBUTE_SLOT_ALREADY_FILLED");
				}
			}
			else if (Value == krxBufferTarget::SHADER_UNIFORM_TARGET)
			{
				if (this->UniformBuffers[ViewInfo.Index] == nullptr)
				{
					this->UniformBuffers[ViewInfo.Index] = ViewInfo.View;
				}
				else
				{
					krxValidationLayerMessage("PIPELINE_LAYOUT_krxPipelineLayout-UNIFORM_SLOT_ALREADY_FILLED");
				}
			}
		}
	}
	AttributesSize.resize(KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT);
	std::memcpy(this->AttributesSize.data(), AttributesSize.data(), 4 * KRX_IMPLEMENTATION_MAX_ATTRIBUTES_COUNT);
}