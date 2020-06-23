# learn-vulkan

Adaptation of [vulkan-tutorial](https://vulkan-tutorial.com/) website and other sources to the vulkan c++ API variation. Furthermore, vulkan objects are put into separate classes for a better understanding of lifetime and relationship between objects.

Uses HLSL as shader language, you need to [download](https://ci.appveyor.com/project/antiagainst/directxshadercompiler/branch/master/artifacts) or build [directx shader compiler](https://github.com/microsoft/DirectXShaderCompiler) and use the correct path in CMakeLists.txt for `DXC_COMPILER_PATH`

## Roadmap

- [x] Uses C++ API
- [x] Draw a triangle
- [ ] Separate all meaningfull objects into classes
- [ ] Add doxygen description for each objects
- [ ] Use Vertex Buffer
- [ ] Load a model
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
