
find_program(FXC_EXECUTABLE fxc.exe
    HINTS
        "$ENV{WindowsSdkDir}/bin/$ENV{WindowsSDKVersion}/x64"
        "$ENV{WindowsSdkDir}/bin/$ENV{WindowsSDKVersion}/x86"
    PATH_SUFFIXES x64 x86
)

if(NOT FXC_EXECUTABLE)
    message(FATAL_ERROR "fxc.exe not found. Install the Windows SDK.")
endif()

# Output directly into the engine's runtime shader directory.
set(SHADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/engine/shaders")

set(SHADERS
    "basic_vs.hlsl|VSMain|vs_5_0"
    "basic_ps.hlsl|PSMain|ps_5_0"
)

set(COMPILED_SHADERS)

foreach(SHADER IN LISTS SHADERS)
    string(REPLACE "|" ";" PARTS "${SHADER}")
    list(GET PARTS 0 SOURCE_NAME)
    list(GET PARTS 1 ENTRY_POINT)
    list(GET PARTS 2 PROFILE)

    string(REPLACE ".hlsl" ".cso" OUTPUT_NAME "${SOURCE_NAME}")

    set(SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_NAME}")
    set(OUTPUT_FILE "${SHADER_OUTPUT_DIR}/${OUTPUT_NAME}")

    add_custom_command(
        OUTPUT "${OUTPUT_FILE}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory
            "${SHADER_OUTPUT_DIR}"
        COMMAND "${FXC_EXECUTABLE}"
            /nologo
            /E "${ENTRY_POINT}"
            /T "${PROFILE}"
            /Fo "${OUTPUT_FILE}"
            "${SOURCE_FILE}"
        DEPENDS "${SOURCE_FILE}"
        COMMENT "Compiling ${SOURCE_NAME}"
        VERBATIM
    )

    list(APPEND COMPILED_SHADERS "${OUTPUT_FILE}")
endforeach()

add_custom_target(demo_shaders ALL
    DEPENDS ${COMPILED_SHADERS}
)

# Make sure the executable's build depends on the shader outputs.
if(TARGET 3dre)
    add_dependencies(3dre demo_shaders)
endif()
