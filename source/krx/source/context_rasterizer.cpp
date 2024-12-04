#include "../include/internals/context.hpp"

void krxContext::krxRasterizationSettings::set_primitive_type(const krxPrimitiveType PrimitiveType)
{
	this->PrimitiveType = PrimitiveType;
}
void krxContext::krxRasterizationSettings::set_facing(const krxRasterizerFacing Facing)
{
	this->Facing = Facing;
}
void krxContext::krxRasterizationSettings::set_viewport(const glm::uvec2& Position, const glm::uvec2& Size)
{
	this->Viewport =
	{
		.Position = Position,
		.Size = Size
	};
}

void krxContext::krxRasterizationSettings::enable_feature(const krxRasterizerFeature Feature)
{
	this->Features = this->Features | static_cast<uint8_t>(Feature);
}
void krxContext::krxRasterizationSettings::disable_feature(const krxRasterizerFeature Feature)
{
	this->Features &= ~static_cast<uint8_t>(Feature);
}