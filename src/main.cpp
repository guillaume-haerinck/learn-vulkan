#include "app.h"

// Define these only in one .cpp file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

int main() {
    {
        App app;
        app.update();
    }

    return 0;
}
