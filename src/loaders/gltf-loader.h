#pragma once

#include <vector>

#include "graphics/ressources/buffers.h"

// TODO handle buffer declaration from the loader so that it only store buffer pointers and prevent useless copy

struct Model {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

class GLTFLoader {
public:
	GLTFLoader();
	~GLTFLoader();

	static Model loadModelFromFile(const char* filePath);

private:

};
