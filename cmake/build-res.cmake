
function(compile_hlsl_using_dxc input_file output_file target)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Compiling SPIR-V binary ${input_file}"
        DEPENDS ${input_file}
        COMMAND dxc/dxc.exe -spirv -T ${target} -E main ${input_file} -Fo ${output_file}
    )
endfunction(compile_hlsl_using_dxc)
