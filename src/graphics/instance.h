#pragma once

#include <vulkan/vulkan.h>

class Instance {
public:
    Instance();
    ~Instance();

private:
    VkInstance m_instance;
};
