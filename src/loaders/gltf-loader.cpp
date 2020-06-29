#include "gltf-loader.h"

// Define these only in one .cpp file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

GLTFLoader::GLTFLoader()
{
}

GLTFLoader::~GLTFLoader()
{
}

Model GLTFLoader::loadModelFromFile(const char* filePath)
{
    Model model;

    // TODO load a 3D model

    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3
    model.vertices = {
        // Front v0,v1,v2,v3
        Vertex(glm::vec3(1, 1, 1), glm::vec3(0, 0, 1), glm::vec2(1, 0)),
        Vertex(glm::vec3(-1, 1, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1), glm::vec2(0, 1)),
        Vertex(glm::vec3(1, -1, 1), glm::vec3(0, 0, 1), glm::vec2(1, 1)),
        // Right v0,v3,v4,v5
        Vertex(glm::vec3(1, 1, 1), glm::vec3(1, 0, 0), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, -1, 1), glm::vec3(1, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, -1, -1), glm::vec3(1, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(1, 1, -1), glm::vec3(1, 0, 0), glm::vec2(1, 1)),
        // Top v0,v5,v6,v1
        Vertex(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, 1, -1), glm::vec3(0, 1, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, 1, 1), glm::vec3(0, 1, 0), glm::vec2(1, 1)),
        // Left v1,v6,v7,v2
        Vertex(glm::vec3(-1, 1, 1), glm::vec3(-1, 0, 0), glm::vec2(1, 0)),
        Vertex(glm::vec3(-1, 1, -1), glm::vec3(-1, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(-1, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec3(-1, 0, 0), glm::vec2(1, 1)),
        // Bottom v7,v4,v3,v2
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, -1, 1), glm::vec3(0, -1, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec3(0, -1, 0), glm::vec2(1, 1)),
        // Back v4,v7,v6,v5
        Vertex(glm::vec3(1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(1, 0)),
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(0, 0)),
        Vertex(glm::vec3(-1, 1, -1), glm::vec3(0, 0, -1), glm::vec2(0, 1)),
        Vertex(glm::vec3(1, 1, -1), glm::vec3(0, 0, -1), glm::vec2(1, 1))
    };


    model.indices = {
        0, 1, 2,   2, 3, 0,       // front
        4, 5, 6,   6, 7, 4,       // right
        8, 9, 10,  10,11, 8,      // top
        12,13,14,  14,15,12,      // left
        16,17,18,  18,19,16,      // bottom
        20,21,22,  22,23,20		  // back
    };

	return model;
}
