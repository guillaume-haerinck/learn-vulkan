
# -spirv build to spirV
# -Zi enables debug information (shader source readable in renderdoc)
# -Zpr Pack matrices in row-major order (vulkan way)
# -T shader target ps_6_0, ..., ps_6_6, vs_6_0, ...
# -E Entry point name
# -Fo Output file

function(compile_hlsl_using_dxc input_file output_file target)
    add_custom_command(
        OUTPUT ${output_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Compiling SPIR-V binary ${input_file}"
        DEPENDS ${input_file}
        COMMAND ${DXC_COMPILER_PATH} -spirv -Zi -Zpr -T ${target} -E main ${input_file} -Fo ${output_file}
    )
endfunction(compile_hlsl_using_dxc)
