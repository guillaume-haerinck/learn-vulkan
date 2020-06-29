#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>
#include <tiny_gltf.h>

#include "graphics/ressources/buffers.h"

// TODO handle buffer declaration from the loader so that it only store buffer pointers and prevent useless copy

struct Model {
	std::vector<Vertex> vertices;
	std::vector<uint8_t> indices_data;
	unsigned int indicesCount;
};

class GLTFLoader {
public:
	GLTFLoader();
	~GLTFLoader();

	static Model loadModelFromFile(const char* filePath);

private:
	static vk::Format getAttributeFormat(const tinygltf::Model* model, uint32_t accessorId);
	static std::vector<uint8_t> convertUnderlyingDataStride(const std::vector<uint8_t>& src_data, uint32_t src_stride, uint32_t dst_stride);

	template <class T>
	static uint32_t toU32(T value)
	{
		static_assert(std::is_arithmetic<T>::value, "T must be numeric");

		if (static_cast<uintmax_t>(value) > static_cast<uintmax_t>(std::numeric_limits<uint32_t>::max()))
		{
			throw std::runtime_error("to_u32() failed, value is too big to be converted to uint32_t");
		}

		return static_cast<uint32_t>(value);
	}
};
