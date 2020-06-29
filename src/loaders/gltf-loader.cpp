#include "gltf-loader.h"


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>
#include <iostream>

GLTFLoader::GLTFLoader()
{
}

GLTFLoader::~GLTFLoader()
{
}

Model GLTFLoader::loadModelFromFile(const char* filePath)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	// Load model
	bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filePath);
	if (!ret)
		assert(false && "Invalid model filepath");

	auto& gltf_mesh = model.meshes.at(0);
	auto& gltf_primitive = gltf_mesh.primitives.at(0);

	// Get positions (required)
	const float* pos = nullptr;
	auto& accessor = model.accessors[gltf_primitive.attributes.find("POSITION")->second];
	size_t vertex_count = accessor.count;
	auto& buffer_view = model.bufferViews[accessor.bufferView];
	pos = reinterpret_cast<const float*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));

	// Get normals
	const float* normals = nullptr;
	if (gltf_primitive.attributes.find("NORMAL") != gltf_primitive.attributes.end())
	{
		accessor = model.accessors[gltf_primitive.attributes.find("NORMAL")->second];
		buffer_view = model.bufferViews[accessor.bufferView];
		normals = reinterpret_cast<const float*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));
	}

	// Get texture coordinates
	const float* uvs = nullptr;
	if (gltf_primitive.attributes.find("TEXCOORD_0") != gltf_primitive.attributes.end())
	{
		accessor = model.accessors[gltf_primitive.attributes.find("TEXCOORD_0")->second];
		buffer_view = model.bufferViews[accessor.bufferView];
		uvs = reinterpret_cast<const float*>(&(model.buffers[buffer_view.buffer].data[accessor.byteOffset + buffer_view.byteOffset]));
	}

	Model parsedModel;
	for (size_t v = 0; v < vertex_count; v++) {
		Vertex vertex;
		vertex.position = glm::vec4(glm::make_vec3(&pos[v * 3]), 1.0f);
		vertex.normal = glm::normalize(glm::vec3(normals ? glm::make_vec3(&normals[v * 3]) : glm::vec3(0.0f)));
		vertex.uv = uvs ? glm::make_vec2(&uvs[v * 2]) : glm::vec3(0.0f);
		parsedModel.vertices.push_back(vertex);
	}

	// Get indices
	if (gltf_primitive.indices >= 0) {
		auto& accessor = model.accessors.at(gltf_primitive.indices);
		auto& bufferView = model.bufferViews.at(accessor.bufferView);
		auto& buffer = model.buffers.at(bufferView.buffer);

		size_t stride = accessor.ByteStride(bufferView);
		size_t startByte = accessor.byteOffset + bufferView.byteOffset;
		size_t endByte = startByte + accessor.count * stride;

		auto format = getAttributeFormat(&model, gltf_primitive.indices);
		std::vector<uint8_t> indices_data = { buffer.data.begin() + startByte, buffer.data.begin() + endByte };
		parsedModel.indicesCount = vertex_count; // wrong

		// Target R32Uint format indices
		// FIXME wrong indices count
		switch (format) {
		case vk::Format::eR16Uint:
			indices_data = convertUnderlyingDataStride(indices_data, 2, 4);
			//parsedModel.indicesCount = indices_data.size(); // X2 ?
			break;

		case vk::Format::eR8Uint:
			indices_data = convertUnderlyingDataStride(indices_data, 1, 4);
			//parsedModel.indicesCount = indices_data.size(); // X4 ?
			break;

		default:
			break;
		}

		parsedModel.indices_data = indices_data;
	} else {
		std::cout << "[gltf loader] Non-indexed drawing not handled yet, artifacts will appear" << std::endl;
	}

	return parsedModel;
}

std::vector<uint8_t> GLTFLoader::convertUnderlyingDataStride(const std::vector<uint8_t>& src_data, uint32_t src_stride, uint32_t dst_stride)
{
	auto elem_count = toU32(src_data.size()) / src_stride;

	std::vector<uint8_t> result(elem_count * dst_stride);

	for (uint32_t idxSrc = 0, idxDst = 0;
		idxSrc < src_data.size() && idxDst < result.size();
		idxSrc += src_stride, idxDst += dst_stride)
	{
		std::copy(src_data.begin() + idxSrc, src_data.begin() + idxSrc + src_stride, result.begin() + idxDst);
	}

	return result;
}

vk::Format GLTFLoader::getAttributeFormat(const tinygltf::Model* model, uint32_t accessorId)
{
	auto& accessor = model->accessors.at(accessorId);

	vk::Format format;

	switch (accessor.componentType)
	{
	case TINYGLTF_COMPONENT_TYPE_BYTE:
	{
		static const std::map<int, vk::Format> mapped_format = { {TINYGLTF_TYPE_SCALAR, vk::Format::eR8Sint},
															  {TINYGLTF_TYPE_VEC2, vk::Format::eR8G8Sint},
															  {TINYGLTF_TYPE_VEC3, vk::Format::eR8G8B8Sint},
															  {TINYGLTF_TYPE_VEC4, vk::Format::eR8G8B8A8Sint} };

		format = mapped_format.at(accessor.type);

		break;
	}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
	{
		static const std::map<int, vk::Format> mapped_format = { {TINYGLTF_TYPE_SCALAR, vk::Format::eR8Uint} };
															  //{TINYGLTF_TYPE_VEC2, VK_FORMAT_R8G8_UINT},
															  //{TINYGLTF_TYPE_VEC3, VK_FORMAT_R8G8B8_UINT},
															  //{TINYGLTF_TYPE_VEC4, VK_FORMAT_R8G8B8A8_UINT} };

		static const std::map<int, vk::Format> mapped_format_normalize = { {TINYGLTF_TYPE_SCALAR, vk::Format::eR8Unorm} };
																		//{TINYGLTF_TYPE_VEC2, VK_FORMAT_R8G8_UNORM},
																		//{TINYGLTF_TYPE_VEC3, VK_FORMAT_R8G8B8_UNORM},
																		//{TINYGLTF_TYPE_VEC4, VK_FORMAT_R8G8B8A8_UNORM} };

		if (accessor.normalized)
		{
			format = mapped_format_normalize.at(accessor.type);
		}
		else
		{
			format = mapped_format.at(accessor.type);
		}

		break;
	}
	case TINYGLTF_COMPONENT_TYPE_SHORT:
	{
		static const std::map<int, vk::Format> mapped_format = { {TINYGLTF_TYPE_SCALAR, vk::Format::eR8Sint } };
															  //{TINYGLTF_TYPE_VEC2, VK_FORMAT_R8G8_SINT},
															  //{TINYGLTF_TYPE_VEC3, VK_FORMAT_R8G8B8_SINT},
															  //{TINYGLTF_TYPE_VEC4, VK_FORMAT_R8G8B8A8_SINT} };

		format = mapped_format.at(accessor.type);

		break;
	}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
	{
		static const std::map<int, vk::Format> mapped_format = { {TINYGLTF_TYPE_SCALAR, vk::Format::eR16Uint } };
															  //{TINYGLTF_TYPE_VEC2, VK_FORMAT_R16G16_UINT},
															  //{TINYGLTF_TYPE_VEC3, VK_FORMAT_R16G16B16_UINT},
															  //{TINYGLTF_TYPE_VEC4, VK_FORMAT_R16G16B16A16_UINT} };

		static const std::map<int, vk::Format> mapped_format_normalize = { {TINYGLTF_TYPE_SCALAR, vk::Format::eR16Unorm } };
																		//{TINYGLTF_TYPE_VEC2, VK_FORMAT_R16G16_UNORM},
																		//{TINYGLTF_TYPE_VEC3, VK_FORMAT_R16G16B16_UNORM},
																		//{TINYGLTF_TYPE_VEC4, VK_FORMAT_R16G16B16A16_UNORM} };

		if (accessor.normalized)
		{
			format = mapped_format_normalize.at(accessor.type);
		}
		else
		{
			format = mapped_format.at(accessor.type);
		}

		break;
	}
	case TINYGLTF_COMPONENT_TYPE_INT:
	{
		static const std::map<int, vk::Format> mapped_format = { {TINYGLTF_TYPE_SCALAR, vk::Format::eR32Sint } };
															  //{TINYGLTF_TYPE_VEC2, VK_FORMAT_R32G32_SINT},
															  //{TINYGLTF_TYPE_VEC3, VK_FORMAT_R32G32B32_SINT},
															  //{TINYGLTF_TYPE_VEC4, VK_FORMAT_R32G32B32A32_SINT} };

		format = mapped_format.at(accessor.type);

		break;
	}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
	{
		static const std::map<int, vk::Format> mapped_format = { {TINYGLTF_TYPE_SCALAR, vk::Format::eR32Uint} };
															  //{TINYGLTF_TYPE_VEC2, VK_FORMAT_R32G32_UINT},
															  //{TINYGLTF_TYPE_VEC3, VK_FORMAT_R32G32B32_UINT},
															  //{TINYGLTF_TYPE_VEC4, VK_FORMAT_R32G32B32A32_UINT} };

		format = mapped_format.at(accessor.type);

		break;
	}
	case TINYGLTF_COMPONENT_TYPE_FLOAT:
	{
		static const std::map<int, vk::Format> mapped_format = { {TINYGLTF_TYPE_SCALAR, vk::Format::eR32Sfloat} };
															  //{TINYGLTF_TYPE_VEC2, VK_FORMAT_R32G32_SFLOAT},
															  //{TINYGLTF_TYPE_VEC3, VK_FORMAT_R32G32B32_SFLOAT},
															  //{TINYGLTF_TYPE_VEC4, VK_FORMAT_R32G32B32A32_SFLOAT} };

		format = mapped_format.at(accessor.type);

		break;
	}
	default:
	{
		format = vk::Format::eUndefined;
		break;
	}
	}

	return format;
};

