#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

class Instance {
public:
    Instance();
    ~Instance();

    vk::Instance& get() { return m_instance; }

private:
    void findBestLayers(const std::vector<vk::LayerProperties>& installed, const std::vector<const char*>& wanted, std::vector<const char*>& out);
    void findBestExtensions(const std::vector<vk::ExtensionProperties>& installed, const std::vector<const char*>& wanted, std::vector<const char*>& out);

private:
    vk::Instance m_instance;
};
