# CMakeList.txt : CMake project for 3dre, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (3dre WIN32 "main.cpp"
  "3dre.h"
  flags.h
  input/input.cpp
  input/input.h
  input/keys.h
  processor/scheduler.h
  processor/scheduler.cpp
  graphics/renderer.cpp
  graphics/renderer.h
  )

set_target_properties(3dre PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

# Link the Windows Direct3D 11 and Win32 libraries.
target_link_libraries(3dre PRIVATE
  d3d11
  dxgi
  dxguid
  user32
  gdi32
)

target_include_directories(3dre PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/input
${CMAKE_CURRENT_SOURCE_DIR}/graphics
${CMAKE_CURRENT_SOURCE_DIR}/processor
)


# TODO: Add tests and install targets if needed.
