
find_program(FXC_EXECUTABLE fxc.exe
    HINTS
        "$ENV{WindowsSdkDir}/bin/$ENV{WindowsSDKVersion}/x64"
        "$ENV{WindowsSdkDir}/bin/$ENV{WindowsSDKVersion}/x86"
    PATH_SUFFIXES x64 x86
)

if(NOT FXC_EXECUTABLE)
    message(FATAL_ERROR "fxc.exe not found. Install the Windows SDK.")
endif()

set(SHADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/engine/shaders")
set(SHADER_FILES
    basic_vs.hlsl
    basic_ps.hlsl
)

set(COMPILED_SHADERS)

foreach(SHADER IN LISTS SHADER_FILES)
    if(SHADER STREQUAL "basic_vs.hlsl")
        set(ENTRY_POINT VSMain)
        set(PROFILE vs_5_0)
    elseif(SHADER STREQUAL "basic_ps.hlsl")
        set(ENTRY_POINT PSMain)
        set(PROFILE ps_5_0)
    endif()

    string(REPLACE ".hlsl" ".cso" OUTPUT_NAME "${SHADER}")
    set(OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}")

    add_custom_command(
        OUTPUT "${OUTPUT_FILE}"
        COMMAND "${FXC_EXECUTABLE}"
            /nologo
            /E "${ENTRY_POINT}"
            /T "${PROFILE}"
            /Fo "${OUTPUT_FILE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}"
        COMMENT "Compiling shader ${SHADER}"
        VERBATIM
    )

    list(APPEND COMPILED_SHADERS "${OUTPUT_FILE}")
endforeach()

add_custom_target(demo_shaders ALL
    DEPENDS ${COMPILED_SHADERS}
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "${SHADER_OUTPUT_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        ${COMPILED_SHADERS}
        "${SHADER_OUTPUT_DIR}"
    COMMAND_EXPAND_LISTS
    COMMENT "Copying demo shaders beside engine"
    VERBATIM
)
