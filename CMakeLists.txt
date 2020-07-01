# Set standards
cmake_minimum_required(VERSION 3.7.2)
project(learn-vulkan C CXX)
set(CMAKE_CXX_STANDARD 17)

# Options
set(DXC_COMPILER_PATH "dxc/dxc.exe" CACHE FILEPATH "Path to dxc.exe, the DirectXShaderCompiler (must support spir-v codegen)")

# /////////////////////////////////////////////////////////////////////////////
# ////////////////////////////// PROJECT FILES ////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

file(GLOB_RECURSE MY_SOURCES src/*)
file(GLOB_RECURSE MY_SHADERS res/shaders/*.hlsl)

# /////////////////////////////////////////////////////////////////////////////
# /////////////////////////////// DEPENDENCIES ////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

find_package(Vulkan REQUIRED)
add_subdirectory(lib/glfw)
add_library(imgui STATIC 
    lib/imgui/imgui.cpp 
    lib/imgui/imgui_draw.cpp 
    lib/imgui/imgui_demo.cpp 
    lib/imgui/imgui_widgets.cpp
    lib/imgui/examples/imgui_impl_glfw.cpp
    lib/imgui/examples/imgui_impl_vulkan.cpp
)
target_link_libraries(imgui glfw)

# /////////////////////////////////////////////////////////////////////////////
# ///////////////////////////////// INCLUDES //////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

include_directories(
    ${Vulkan_INCLUDE_DIRS}
    lib
    lib/glm
    lib/tinygltf
    lib/json/include
    lib/imgui
    lib/imgui/examples
    lib/spdlog/include
    src
    res
)

include(cmake/build-res.cmake)

# /////////////////////////////////////////////////////////////////////////////
# /////////////////////////////////// BUILD ///////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
add_executable(${PROJECT_NAME} ${MY_SOURCES})
target_link_libraries(
    ${PROJECT_NAME}
    glfw
    imgui
    ${GLFW_LIBRARIES}
    ${Vulkan_LIBRARIES}
)

# Compile hlsl to spir-v
foreach(shader ${MY_SHADERS})
    get_filename_component(file_name ${shader} NAME_WLE)
    get_filename_component(file_dir ${shader} DIRECTORY)
    add_custom_target(${file_name} ALL DEPENDS ${file_dir}/${file_name}.spv)

    string(FIND ${file_name} "_vs" isVS REVERSE)
    if (isVS GREATER -1)
        compile_hlsl_using_dxc(${shader} ${file_dir}/${file_name}.spv "vs_6_5")
        continue()
    endif()

    string(FIND ${file_name} "_ps" isPS REVERSE)
    if (isPS GREATER -1)
        compile_hlsl_using_dxc(${shader} ${file_dir}/${file_name}.spv "ps_6_5")
        continue()
    endif()

endforeach()

if (WIN32) 
    # Copy .dll to build folder
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_BINARY_DIR}/lib/glfw/src/glfw3.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()
