
function(compile_hlsl_using_dxc input_file output_file target)
    add_custom_command(
        OUTPUT ${output_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Compiling SPIR-V binary ${input_file}"
        DEPENDS ${input_file}
        COMMAND ${DXC_COMPILER_PATH} -spirv -T ${target} -E main ${input_file} -Fo ${output_file}
    )
endfunction(compile_hlsl_using_dxc)
