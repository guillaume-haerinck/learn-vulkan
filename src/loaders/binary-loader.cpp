#include "binary-loader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

BinaryLoader::BinaryLoader()
{
}

BinaryLoader::~BinaryLoader()
{
}

Model BinaryLoader::loadModelFromFile(const char* vertexFilePath, const char* indexFilePath)
{
	auto vertexdata = std::vector<char>();
	readBufferFromFile(vertexFilePath, vertexdata);

	const int posByteSize = sizeof(float) * 3;
	const int normalByteSize = sizeof(float) * 3;
	const int uvByteSize = sizeof(float) * 2;
	const int elementByteSize = posByteSize + normalByteSize + uvByteSize;
	const int verticesCount = vertexdata.size() / elementByteSize;

	Model model;
	for (size_t i = 0; i < verticesCount; i++) {
		Vertex vertex;
		auto pos = reinterpret_cast<const float*>(vertexdata.data() + elementByteSize * i);
		auto normal = reinterpret_cast<const float*>(vertexdata.data() + posByteSize + elementByteSize * i);
		auto uv = reinterpret_cast<const float*>(vertexdata.data() + posByteSize + normalByteSize + elementByteSize * i);

		vertex.position = glm::make_vec3(pos);
		vertex.normal = glm::make_vec3(normal);
		vertex.uv = glm::make_vec2(uv);
		model.vertices.push_back(vertex);
	}

	auto indexData = std::vector<char>();
	readBufferFromFile(indexFilePath, indexData);
	const int indiceByteSize = sizeof(uint8_t);
	const int indicesCount = indexData.size() / indiceByteSize;
	model.indicesCount = verticesCount;

	for (size_t i = 0; i < indicesCount; i++) {
		auto indice = reinterpret_cast<const uint8_t*>(indexData.data() + indiceByteSize * i);
		model.indicesData.push_back(*indice);
	}

	return model;
}

void BinaryLoader::readBufferFromFile(const char* filepath, std::vector<char>& buffer) {
	std::ifstream stream(filepath, std::ios::in | std::ios::binary);
	if (!stream)
		std::cerr << "[BinaryLoader] File does not exist : " << filepath << std::endl;
	
	stream.seekg(0, stream.end);
	const int vertexFileByteSize = stream.tellg();
	stream.seekg(0, stream.beg);

	buffer.resize(vertexFileByteSize);
	if (!stream.read(buffer.data(), vertexFileByteSize))
        std::cerr << "[BinaryLoader] Cannot read file : " << filepath << std::endl;
}
