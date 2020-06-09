//
// Created by ripopov on 6/7/2020.
//

#pragma once

#include "color.h"
#include <span>
#include <glm/vec2.hpp>

struct SDL_Surface;

namespace gfx {

/// RGBA32 Surface for software rendering, C++ wrapper over SDL_Surface
/// Pixels are in row-major order
/// Top-Left Pixel has coordinates [0][0]
class Surface {
public:
    Surface(int width, int height);
    Surface(int width, int height, Color c);
    explicit Surface(const char *img_filename);
    ~Surface();

    Surface(const Surface &) = delete;

    [[nodiscard]] int width() const noexcept;
    [[nodiscard]] int height() const noexcept;
    [[nodiscard]] int size() const noexcept;

    friend bool operator==(const Surface &sa, const Surface &sb);

    [[nodiscard]] SDL_Surface *sdlSurface() { return surf; }

    [[nodiscard]] std::span<Color> pixels() noexcept;
    [[nodiscard]] std::span<const Color> pixels() const noexcept;

    /// Note : row major order, surface[y][x]
    [[nodiscard]] std::span<Color> operator[](int row) noexcept;
    /// Column-major access
    [[nodiscard]] Color &xy(int x, int y) noexcept { return (*this)[y][x]; }
    /// Save into BMP file
    void saveBMP(const char *filename) const;

    [[nodiscard]] bool isInnerPoint(glm::ivec2 point) const;

    void drawLine(glm::ivec2 p0, glm::ivec2 p1, Color c);

private:
    explicit Surface(SDL_Surface *s);
    SDL_Surface *surf;
};

} // namespace gfx
