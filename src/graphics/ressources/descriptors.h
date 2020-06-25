#pragma once

#include <vulkan/vulkan.hpp>
#include <array>

#include "graphics/setup/devices.h"
#include "buffers.h"

/**
 * @brief Describe the vertex buffer layout
 */
class VertexInputDescription
{
public:
    VertexInputDescription();
    ~VertexInputDescription();

    vk::VertexInputBindingDescription getBindingDescription();
    std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions();

private:
    vk::VertexInputBindingDescription m_description;
    std::array<vk::VertexInputAttributeDescription, 2> m_inputDescription;
};

/**
 * @brief Describe a uniform buffer layout
 */
class DescriptorSetLayout
{
public:
    DescriptorSetLayout(LogicalDevice& device);
    ~DescriptorSetLayout();

    vk::DescriptorSetLayout& get() { return m_layout.get(); }

private:
    vk::UniqueDescriptorSetLayout m_layout; // TODO FIXME unique ptr might cause some problems
};

/**
 * @brief Handles descriptor set allocation
 */
class DescriptorPool
{
public:
    DescriptorPool(LogicalDevice& device, unsigned int swapChainImagesCount);
    ~DescriptorPool();

    vk::DescriptorPool& get() { return m_descriptorPool.get(); }

private:
    LogicalDevice& m_device;
    vk::UniqueDescriptorPool m_descriptorPool;
};

/**
 * @brief Store uniform buffers
 */
class DescriptorSets
{
public:
    DescriptorSets(LogicalDevice& device, DescriptorPool& descriptorPool, DescriptorSetLayout& descriptorSetLayout, unsigned int swapChainImagesCount, UniformBuffer& uniformBuffer);
    ~DescriptorSets();

    vk::DescriptorSet& get(unsigned int index) { return m_descriptorSets.at(index); }

private:
    LogicalDevice& m_device;
    std::vector<vk::DescriptorSet> m_descriptorSets;
};

/**
 * @brief Collection of descriptor sets used in the pipeline
 */
class PipelineLayout
{
public:
	PipelineLayout(LogicalDevice& device);
	~PipelineLayout();

    vk::PipelineLayout& getLayout() { return m_pipelineLayout; }
    DescriptorSetLayout& getDescriptorSetLayout() { return m_descriptorSetLayout; }

private:
    LogicalDevice& m_device;
    DescriptorSetLayout m_descriptorSetLayout;
    vk::PipelineLayout m_pipelineLayout;
};
