#pragma once

#include "model.h"

#include <vector>

class BinaryLoader
{
public:
	BinaryLoader();
	~BinaryLoader();

	static Model loadModelFromFile(const char* vertexFilePath, const char* indexFilePath);
	static void readBufferFromFile(const char* filepath, std::vector<char>& buffer);

private:

};

