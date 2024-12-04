#include "../include/internals/context.hpp"
#include <cstring>
#include "validation_layer.hpp"

void krxContext::bind_pipeline_layout(krxPipelineLayout* Layout)
{
	this->PipelineLayout = Layout;
}

template<typename FormatPerPixel>
static void clear_surface_per_format(std::vector<uint8_t>& TextureData, const FormatPerPixel ColorPixel)
{
	FormatPerPixel* Pixels = reinterpret_cast<FormatPerPixel*>(TextureData.data());
	const uint32_t PixelsCount = TextureData.size() / sizeof(FormatPerPixel);

	for (uint32_t i = 0; i < PixelsCount; i++)
	{
		Pixels[i] = ColorPixel;
	}
}

void krxContext::clear_color_targets(const glm::vec4 RGBA)
{
	if (this->PipelineLayout == nullptr)
	{
		krxValidationLayerMessage("CONTEXT_clear_color_targets-NO_PIPELINE_LAYOUT_BOUND");
	}

	for (auto& Target : this->PipelineLayout->RenderTargetOutputs)
	{
		if (Target == nullptr)
		{
			continue;
		}
		if (reinterpret_cast<krxTexture2D*>(Target->Info.Resource)->Info.Format == krxFormat::PRESENT_FORMAT)
		{
			uint8_t ColorPixel[4] =
			{
				static_cast<uint8_t>(255 * RGBA.b),
				static_cast<uint8_t>(255 * RGBA.g),
				static_cast<uint8_t>(255 * RGBA.r),
				static_cast<uint8_t>(255 * RGBA.a),
			};

			clear_surface_per_format(reinterpret_cast<krxTexture2D*>(Target->Info.Resource)->Data, *reinterpret_cast<uint32_t*>(ColorPixel));
		}
		else if (reinterpret_cast<krxTexture2D*>(Target->Info.Resource)->Info.Format == krxFormat::UINT8_RGBA)
		{
			uint8_t ColorPixel[4] =
			{
				static_cast<uint8_t>(255 * RGBA.r),
				static_cast<uint8_t>(255 * RGBA.g),
				static_cast<uint8_t>(255 * RGBA.b),
				static_cast<uint8_t>(255 * RGBA.a),
			};

			clear_surface_per_format(reinterpret_cast<krxTexture2D*>(Target->Info.Resource)->Data, *reinterpret_cast<uint32_t*>(ColorPixel));
		}
		else if (reinterpret_cast<krxTexture2D*>(Target->Info.Resource)->Info.Format == krxFormat::UINT8_RGB)
		{
			struct uint24_t
			{
				uint8_t Data[3];
			} ColorPixel =
			{
				static_cast<uint8_t>(255 * RGBA.r),
				static_cast<uint8_t>(255 * RGBA.g),
				static_cast<uint8_t>(255 * RGBA.b),
			};

			clear_surface_per_format(reinterpret_cast<krxTexture2D*>(Target->Info.Resource)->Data, ColorPixel);
		}
		else if (reinterpret_cast<krxTexture2D*>(Target->Info.Resource)->Info.Format == krxFormat::UINT8_RG)
		{
			uint8_t ColorPixel[2] =
			{
				static_cast<uint8_t>(255 * RGBA.r),
				static_cast<uint8_t>(255 * RGBA.g),
			};

			clear_surface_per_format(reinterpret_cast<krxTexture2D*>(Target->Info.Resource)->Data, *reinterpret_cast<uint16_t*>(ColorPixel));
		}
		else if (reinterpret_cast<krxTexture2D*>(Target->Info.Resource)->Info.Format == krxFormat::UINT8_R)
		{
			uint8_t ColorPixel = static_cast<uint8_t>(255 * RGBA.r);

			clear_surface_per_format(reinterpret_cast<krxTexture2D*>(Target->Info.Resource)->Data, ColorPixel);
		}
	}
}

void krxContext::fast_clear_color_targets()
{
	if (this->PipelineLayout == nullptr)
	{
		krxValidationLayerMessage("CONTEXT_fast_clear_color_targets-NO_PIPELINE_LAYOUT_BOUND");
	}
	for (auto& Target : this->PipelineLayout->RenderTargetOutputs)
	{
		if (Target != nullptr)
		{
			std::memset(reinterpret_cast<krxTexture2D*>(Target->Info.Resource)->Data.data(), 0, reinterpret_cast<krxTexture2D*>(Target->Info.Resource)->Data.size());
		}
	}
}
void krxContext::clear_depth_target()
{
	if (this->PipelineLayout == nullptr)
	{
		krxValidationLayerMessage("CONTEXT_clear_depth_target-NO_PIPELINE_LAYOUT_BOUND");
	}
	if (this->PipelineLayout->DepthBuffer == nullptr)
	{
		krxValidationLayerMessage("CONTEXT_clear_depth_target-NO_DEPTH_RENDER_TARGET_OUTPUT_BOUND");
	}

	std::memset(reinterpret_cast<krxTexture2D*>(this->PipelineLayout->DepthBuffer)->Data.data(), 255, reinterpret_cast<krxTexture2D*>(this->PipelineLayout->DepthBuffer)->Data.size());
}

void krxContext::bind_shader_pipeline(krxShaderPipeline* Pipeline)
{
	this->ShaderPipeline = Pipeline;
}

void krxContext::draw(const uint32_t VertexStart, const uint32_t VertexCount)
{
	if (this->ShaderPipeline == nullptr)
	{
		krxValidationLayerMessage("CONTEXT_draw-NO_SHADER_PIPELINE_BOUND");
	}

	if (this->Rasterizer.PrimitiveType == krxPrimitiveType::TRIANGLES)
	{
		this->ppTriangles.process(this, VertexStart, VertexCount);
	}
}

krxContext::~krxContext()
{
	this->ppTriangles.shutdown();
}