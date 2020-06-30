# Learn Vulkan

Adaptation of [vulkan-tutorial](https://vulkan-tutorial.com/) website and other sources to the Vulkan C++ API variation. Furthermore, vulkan objects are put into separate classes for a better understanding of lifetime and relationship between objects.

## Getting started

### Download

1. Get [Vulkan SDK](https://vulkan.lunarg.com/) and install it. 

2. Clone the repo with its dependencies.

   `git clone --recurse-submodules https://github.com/guillaume-haerinck/learn-vulkan`

3. As the project uses HLSL as shader language, you need to [download](https://ci.appveyor.com/project/antiagainst/directxshadercompiler/branch/master/artifacts) (_artifact tab_) or build [directx shader compiler](https://github.com/microsoft/DirectXShaderCompiler) and use the correct path in CMakeLists.txt for `DXC_COMPILER_PATH`. The build must support SPIR-V generation.

### Compile

The project uses CMake to generate project files. You can open it directly with visual studio as a cmake project.

### Run

Simply run the executable, but beware that the working directory must be this folder to get the shaders.

## Roadmap

- [x] Uses C++ API
- [x] Draw a triangle
- [x] Use Vertex Buffer
- [x] Use constant buffers in shaders
- [x] Separate all meaningfull objects into separate classes
- [x] Load a .gltf model
- [x] ImGui support
- [ ] Handle textures
- [ ] Defered shading
- [ ] Annotate drawcalls with debug extension

## Useful links

### Samples

| Link                                                         | Author        | Description                             |
| ------------------------------------------------------------ | ------------- | --------------------------------------- |
| [Vulkan HPP](https://github.com/KhronosGroup/Vulkan-Hpp/tree/master/samples) | Khronos       | Exemples of the official C++ Vulkan API |
| [Vulkan](https://github.com/SaschaWillems/Vulkan)            | Sacha Willems | Broad exemples of the API               |
| [Vulkan samples](https://github.com/khronosGroup/Vulkan-samples) | Khronos       | Official exemples with best practices   |

### API Overview

| Link                                                         | Author                     | Description                                                  |
| ------------------------------------------------------------ | -------------------------- | ------------------------------------------------------------ |
| [Raw vulkan](https://alain.xyz/blog/raw-vulkan)              | Alain Gavan                | Broad presentation of the API                                |
| [How to learn vulkan](https://www.jeremyong.com/c++/vulkan/graphics/rendering/2018/03/26/how-to-learn-vulkan/) | Jeremy Yong                | Advices on getting started with Vulkan                       |
| [Vulkan do and donts](https://devblogs.nvidia.com/vulkan-dos-donts/) | NVidia                     | Advices on common performance and development practices      |
| [Vulkan in 30 minutes](https://renderdoc.org/vulkan-in-30-minutes.html) | Baldurk (RenderDoc author) | Presentation of the API                                      |
| [Lets learn vulkan](https://gpuopen.com/learn/lets-learn-vulkan/) | GPU Open (AMD)             | Many articles on Vulkan                                      |
| [NVidia dev blog](https://developer.nvidia.com/Vulkan)       | NVidia                     | Many articles on Vulkan                                      |
| [Moving to vulkan](https://www.khronos.org/assets/uploads/developers/library/2016-uk-chapter-moving-to-vulkan/Moving-to-Vulkan_Khronos-UK_May16.pdf) | Khronos                    | Excellent presentation on the how and the why of Vulkan when it came out |

### Libraries

| Link                                                         | Author         | Description                                          |
| ------------------------------------------------------------ | -------------- | ---------------------------------------------------- |
| [Vulkan Memory Allocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) | GPU Open (AMD) | Handles this difficult subject better than you could |

### Renderer

| Link                                                         | Author | Description |
| ------------------------------------------------------------ | ------ | ----------- |
| [BGFX](https://github.com/bkaradzic/bgfx)                    |        |             |
| [Niagara](https://github.com/zeux/niagara)                   |        |             |
| [Pumex](https://github.com/pumexx/pumex)                     |        |             |
| [Diligent Engine](https://github.com/DiligentGraphics/DiligentEngine) |        |             |

### Synchronisation

| Link                                                         | Author                           | Description |
| ------------------------------------------------------------ | -------------------------------- | ----------- |
| [Breaking down barriers](https://mynameismjp.wordpress.com/2018/03/06/breaking-down-barriers-part-1-whats-a-barrier/) | MJP                              |             |
| [Breaking down barriers - GDC 2019](https://gpuopen.com/gdc-presentations/2019/gdc-2019-agtd5-breaking-down-barriers.pdf) | Matt Pettineo                    |             |
| [Low level mysteries of pipeline barriers - Vulkanised 2018](https://www.khronos.org/assets/uploads/developers/library/2018-vulkanised/05-The%20low-level%20mysteries%20of%20pipeline%20barriers_Vulkanised2018.pdf) | Frederic Garnier, Andrew Garrard |             |
| [Vulkan synchronisation primer](https://www.jeremyong.com/vulkan/graphics/rendering/2018/11/22/vulkan-synchronization-primer/) | Jeremy Yong                      |             |
| [Barriers in vulkan are not difficult](http://cpp-rendering.io/barriers-vulkan-not-difficult/) | Antoine Morrier                  |             |
| [Vulkan spec on synchronisation](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#synchronization) | Khronos                          |             |

### Performance

| Link                                                        | Author         | Description |
| ----------------------------------------------------------- | -------------- | ----------- |
| [GPU Open on performance](https://gpuopen.com/performance/) | GPU Open (AMD) |             |

### Misc

| Link                                                         | Author                                          | Description |
| ------------------------------------------------------------ | ----------------------------------------------- | ----------- |
| [Awesome Vulkan](https://github.com/vinjn/awesome-vulkan)    |                                                 |             |
| [A trip through the graphic pipeline](https://fgiesen.wordpress.com/2011/07/09/a-trip-through-the-graphics-pipeline-2011-index/) | [Fabian Giesen](https://fgiesen.wordpress.com/) |             |











