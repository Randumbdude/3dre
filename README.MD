# 3dre Engine Code Overview

This document describes the Direct3D setup and the input, math, geometry, and rendering code added to the project.

## Build setup

The top-level `CMakeLists.txt` adds the `3dre` directory. `3dre/CMakeLists.txt` defines the Windows GUI executable, sets C++20, and links the Windows and Direct3D libraries:

- `d3d11` provides the Direct3D 11 device, context, resources, and draw calls.
- `dxgi` provides the swap chain and display interfaces.
- `dxguid` provides DirectX interface identifiers.
- `d3dcompiler` compiles the small HLSL shaders used by the triangle renderer at runtime.
- `user32` and `gdi32` support Win32 window creation and messages.

Build the configured project with:

```powershell
cmake --build out/build/x64-debug
```

## Window and frame loop

`3dre/main.cpp` owns application startup and the Win32 message loop. It creates the window, creates the Direct3D device and swap chain, and creates a render-target view for the swap chain's back buffer. Each frame it:

1. Processes pending Win32 messages.
2. Binds and clears the back-buffer render target.
3. Calls `renderer::render_main()` to draw scene content.
4. Presents the back buffer through the swap chain.

The resize handler releases and recreates the render-target view after resizing the swap-chain buffers, then updates the viewport. `flags::g_running` controls whether the loop continues; Escape and window close set it to `false`.

## Keyboard input

`3dre/input/input.cpp` updates the shared key states when the window receives `WM_KEYDOWN` and `WM_KEYUP`. `WM_KILLFOCUS` clears all states so a key does not remain marked as held if the app loses focus.

Include `input/keys.h` in a source file to read a key state:

```cpp
#include "keys.h"

if (KEY_Z_PRESSED != 0)
{
    // Z is held down.
}
```

The `KEY_A_PRESSED` through `KEY_Z_PRESSED` inline integers are `1` while the corresponding letter key is held and `0` after it is released. The current handler tracks letters A through Z.

## Vector types

The vector declarations and implementations are in `3dre/math/vector/`.

- `vector2` stores `x` and `y`.
- `vector3` stores `x`, `y`, and `z`.
- `vector4` stores `x`, `y`, `z`, and `w`; this is useful for homogeneous coordinates.

All three types support vector addition and subtraction, scalar multiplication and division, dot product, squared length, length, and normalization. `vector3` also supports the cross product. Normalizing a zero vector returns a zero vector. Direct division operators do not check for a zero divisor.

Example:

```cpp
vector3 direction(3.0f, 0.0f, 4.0f);
vector3 unit_direction = direction.normalized();
float distance = direction.length();
```

## 4 by 4 matrices

`3dre/math/matrix/matrix4x4.h` and `.cpp` provide a 4 by 4 transformation matrix. The matrix stores values as `m[row][column]` and follows a row-vector convention: a point is transformed as `point * matrix`, and translation is stored in row 3. Under this convention, use `scale * rotation * translation` to apply those transforms in that order.

The default constructor creates identity. The class also provides:

- `identity()` and `zero()` factories.
- Translation, scale, and X/Y/Z rotation factories. Rotation angles are in radians.
- `transposed()` and matrix multiplication.
- `transform_point()`, which includes translation and applies perspective division when the resulting `w` is neither zero nor one.
- `transform_direction()`, which ignores translation.

Example:

```cpp
matrix4x4 model = matrix4x4::scale(1.0f, 1.0f, 1.0f)
                * matrix4x4::rotation_y(0.5f)
                * matrix4x4::translation(0.0f, 0.0f, 3.0f);
vector3 world_position = model.transform_point(vector3(1.0f, 0.0f, 0.0f));
```

## Vertex, triangle, and mesh data

The geometry types are under `3dre/graphics/types/`.

- `vertex` stores a position (`vector3 pos`), normal (`vector3 norm_pos`), and texture coordinates (`vector2 tex_cords`). Its default is at the origin, with an up-facing normal and zero UV coordinates. It can also be constructed from scalar components or vector values.
- `triangle` stores exactly three vertices in `vertices[3]` and can be constructed from three `vertex` values.
- `mesh` is CPU-side triangle data. It can add triangles, clear its data, report triangle and vertex counts, provide access to its triangles, and flatten the triangles into a contiguous `std::vector<vertex>` for a future GPU upload.

The mesh does not create or own Direct3D buffers. The renderer's sample triangle is currently defined directly in `renderer.cpp` and does not yet draw a `mesh` instance.

## Current triangle renderer

`renderer::render_main()` in `3dre/graphics/renderer.cpp` draws one cyan triangle. On its first call, it:

1. Creates a vertex buffer containing three positions in clip space.
2. Compiles the vertex and pixel shader entry points with `D3DCompile`.
3. Creates the shaders and an input layout that reads the `POSITION` values from `vertex::pos`.

Each call binds the input layout, vertex buffer, triangle-list topology, and shaders, then issues `Draw(3, 0)`. Because the positions are already in clip space, there is no camera or transform matrix involved yet. The normal and UV attributes are stored in the vertex type but are not used by this shader.

The triangle pipeline is initialized lazily. If shader compilation or resource creation fails, a short diagnostic is sent to the Visual Studio Output window with `OutputDebugStringA` and the draw is skipped.
