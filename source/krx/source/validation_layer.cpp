#include "../include/internals/validation_layer.hpp"
#include "validation_layer.hpp"

#include <iostream>

bool ValidationLayerEnabled = false;

void krxEnableValidationLayer()
{
	ValidationLayerEnabled = true;
}
void krxValidationLayerMessage(const std::string& Message)
{
	if (ValidationLayerEnabled)
	{
		std::cout << "[KRX-ERROR]: " << Message << std::endl;
	}
}