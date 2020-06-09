add_library(mygfx
        color.h
        model.cpp
        model.h
        render.cpp
        render.h
        surface.cpp
        surface.h
        )

target_link_libraries(mygfx PUBLIC SDL2::SDL2 SDL2::SDL2_image tinyobjloader::tinyobjloader glm)
target_include_directories(mygfx PUBLIC .)
