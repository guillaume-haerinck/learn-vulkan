#include "instance.h"

#include <GLFW/glfw3.h>
#include <debug_break/debug_break.h>
#include <iostream>

Instance::Instance() {
    vk::ApplicationInfo appInfo(
        "Hello Triangle",
        VK_MAKE_VERSION(1, 0, 0),
        "No Engine",
        VK_MAKE_VERSION(1, 0, 0),
        VK_API_VERSION_1_1
    );

    std::vector<const char*> layers = {};

#ifndef NDEBUG
    std::vector<const char*> wantedLayers = {
        "VK_LAYER_LUNARG_standard_validation"
    };
    std::vector<vk::LayerProperties> installedLayers = vk::enumerateInstanceLayerProperties();
    findBestLayers(installedLayers, wantedLayers, layers);
#endif

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    vk::InstanceCreateInfo createInfo(
        vk::InstanceCreateFlags(),
        &appInfo,
        static_cast<uint32_t>(layers.size()),
        layers.data(),
        glfwExtensionCount,
        glfwExtensions
    );

    m_instance = vk::createInstance(createInfo);
}

Instance::~Instance() {
    m_instance.destroy();
}

void Instance::findBestLayers(const std::vector<vk::LayerProperties>& installed, const std::vector<const char*>& wanted, std::vector<const char*>& out) {
    for (const char* const& w : wanted) {
		for (vk::LayerProperties const& i : installed) {
			if (std::string(i.layerName).compare(w) == 0) {
				out.emplace_back(w);
				break;
			}
		}
	}
}
