# CMakeList.txt : CMake project for 3dre, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (3dre WIN32 "main.cpp")

set_target_properties(3dre PROPERTIES
  CXX_STANDARD 17
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

# TODO: Add tests and install targets if needed.
