#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

#include "graphics/setup/devices.h"

//////////////////////////////////////////////////////
////////////////// CONSTANT BUFFER ///////////////////
//////////////////////////////////////////////////////

class ConstantBuffer {
public:
    ConstantBuffer();
    ~ConstantBuffer();

private:
};

//////////////////////////////////////////////////////
////////////////////// PIPELINE //////////////////////
//////////////////////////////////////////////////////

class Pipeline {
public:
    Pipeline(LogicalDevice& device);
    ~Pipeline();

private:
    std::vector<char> readBinaryFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code, LogicalDevice& device);
};
