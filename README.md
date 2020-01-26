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
