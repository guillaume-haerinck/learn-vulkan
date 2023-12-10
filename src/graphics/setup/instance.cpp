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
        VK_API_VERSION_1_2
    );

    std::vector<const char*> validationLayers = {};
    std::vector<const char*> extensions = {};
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

#ifndef NDEBUG
    std::vector<const char*> wantedLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    std::vector<vk::LayerProperties> installedLayers = vk::enumerateInstanceLayerProperties();
    findBestLayers(installedLayers, wantedLayers, validationLayers);

    std::vector<const char*> wantedExtensions = {
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };
    std::vector<vk::ExtensionProperties> installedExtensions = vk::enumerateInstanceExtensionProperties();
    findBestExtensions(installedExtensions, wantedExtensions, extensions);
#endif

    for (unsigned int i = 0; i < glfwExtensionCount; i++) {
        extensions.push_back(glfwExtensions[i]);
    }

    vk::InstanceCreateInfo createInfo(
        vk::InstanceCreateFlags(),
        &appInfo,
        static_cast<uint32_t>(validationLayers.size()),
        validationLayers.data(),
        static_cast<uint32_t>(extensions.size()),
        extensions.data()
    );

    m_instance = vk::createInstance(createInfo);
}

Instance::~Instance() {
    m_instance.destroy();
}

void Instance::findBestLayers(const std::vector<vk::LayerProperties>& installed, const std::vector<const char*>& wanted, std::vector<const char*>& out) {
    for (const char* const& w : wanted) {
		for (vk::LayerProperties const& i : installed) {
			if (std::string(i.layerName.data()).compare(w) == 0) {
				out.emplace_back(w);
				break;
			}
		}
	}
}

void Instance::findBestExtensions(const std::vector<vk::ExtensionProperties>& installed, const std::vector<const char*>& wanted, std::vector<const char*>& out) {
	for (const char* const& w : wanted) {
		for (vk::ExtensionProperties const& i : installed) {
			if (std::string(i.extensionName.data()).compare(w) == 0) {
				out.emplace_back(w);
				break;
			}
		}
	}
}
