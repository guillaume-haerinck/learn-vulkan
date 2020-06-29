# learn-vulkan

Adaptation of [vulkan-tutorial](https://vulkan-tutorial.com/) website and other sources to the vulkan c++ API variation. Furthermore, vulkan objects are put into separate classes for a better understanding of lifetime and relationship between objects.

Uses HLSL as shader language, you need to [download](https://ci.appveyor.com/project/antiagainst/directxshadercompiler/branch/master/artifacts) or build [directx shader compiler](https://github.com/microsoft/DirectXShaderCompiler) and use the correct path in CMakeLists.txt for `DXC_COMPILER_PATH`

## Roadmap

- [x] Uses C++ API
- [x] Draw a triangle
- [x] Use Vertex Buffer
- [x] Use constant buffers in shaders
- [x] Separate all meaningfull objects into separate classes
- [ ] Add doxygen description for each objects
- [ ] Load a .gltf model
- [ ] Annotate drawcalls with debug extension

## Useful links

https://github.com/KhronosGroup/Vulkan-Hpp/tree/master/samples

https://alain.xyz/blog/raw-vulkan

https://www.jeremyong.com/c++/vulkan/graphics/rendering/2018/03/26/how-to-learn-vulkan/

https://devblogs.nvidia.com/vulkan-dos-donts/

https://vulkan-tutorial.com/

https://renderdoc.org/vulkan-in-30-minutes.html

https://github.com/vinjn/awesome-vulkan

http://32ipi028l5q82yhj72224m8j.wpengine.netdna-cdn.com/wp-content/uploads/2016/03/d3d12_vulkan_lessons_learned.pdf

https://gpuopen.com/using-the-vulkan-validation-layers/

https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator

https://github.com/SaschaWillems/Vulkan

https://github.com/khronosGroup/Vulkan-samples

### Renderer

https://github.com/bkaradzic/bgfx

https://github.com/zeux/niagara

https://github.com/pumexx/pumex

https://github.com/DiligentGraphics/DiligentEngine

### Synchronisation

https://mynameismjp.wordpress.com/2018/03/06/breaking-down-barriers-part-1-whats-a-barrier/

https://gpuopen.com/gdc-presentations/2019/gdc-2019-agtd5-breaking-down-barriers.pdf

https://www.khronos.org/assets/uploads/developers/library/2018-vulkanised/05-The%20low-level%20mysteries%20of%20pipeline%20barriers_Vulkanised2018.pdf

https://www.jeremyong.com/vulkan/graphics/rendering/2018/11/22/vulkan-synchronization-primer/

http://cpp-rendering.io/barriers-vulkan-not-difficult/

https://gpuopen.com/performance/#barrier

https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#synchronization

