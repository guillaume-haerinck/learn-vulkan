#pragma once

#include <vulkan/vulkan.hpp>
#include <array>

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
    DescriptorSetLayout();
    ~DescriptorSetLayout();

private:

};

/**
 * @brief Store uniform buffers
 */
class DescriptorSet
{
public:
	DescriptorSet();
	~DescriptorSet();

private:

};

/**
 * @brief Handles descriptor set allocation
 */
class DescriptorPool
{
public:
    DescriptorPool();
    ~DescriptorPool();

private:

};

/**
 * @brief Collection of descriptor sets used in the pipeline
 */
class PipelineLayout
{
public:
	PipelineLayout();
	~PipelineLayout();

private:

};
