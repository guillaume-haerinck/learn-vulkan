#pragma once

#include <vector>
#include <glm/glm.hpp>

/**
 * @brief Structure of data for each vertices in the engine
 */
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    Vertex() {}
    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv)
        : position(position), normal(normal), uv(uv)
    {}
};

/**
 * @brief Usable model data to be consummed by the renderer
 * @note Can be discarded once Vulkan object has been created
 */
struct Model {
	std::vector<Vertex> vertices;
	std::vector<uint8_t> indices_data;
	unsigned int indicesCount;
};
