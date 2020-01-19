#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Instance {
public:
    Instance();
    ~Instance();

private:
    bool doesEveryValidationLayersExists(const std::vector<const char*>& validationLayers, const std::vector<VkLayerProperties>& availableLayers) const;

private:
    VkInstance m_instance;
};
