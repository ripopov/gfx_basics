# gfx_basics
Sandbox repository for learning 3D Graphics.
Following online tutorials to learn 3D basics (see Refernces section).

### Building
Currently tested only on Windows (as I'm working on my Windows laptop), with MSVC 2019.

1. Use vcpkg to install dependencies:
    - gtest
    - tinyobjloader
    - glm
    - sdl2
    - sdl2-image
    - fmt

2. Set VCPKG_ROOT environment variable to vcpkg root directory
3. Use CMake to build (Or open folder in any IDE that supports CMake)
    
### References
- https://github.com/ssloy/tinyrenderer/wiki https://habr.com/ru/post/248153/ Soft 3D render (C++): 
- http://blog.simonrodriguez.fr/articles/18-02-2017_writing_a_small_software_renderer.html Soft 3D render (In Swift)
- https://github.com/tinyobjloader/tinyobjloader OBJ file loader
- http://casual-effects.com/data/index.html Free OBJ files
- https://github.com/g-truc/glm/blob/master/manual.md GLM reference
- https://vulkan-tutorial.com/Overview Vulkan tutorial

### TODO:
- Look at https://www.assimp.org/
