//
// Created by ripopov on 6/7/2020.
//

#include "surface.h"
#include "SDL_image.h"
#include "SDL_surface.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace gfx {

Surface::Surface(int width, int height) {
    const uint32_t rmask = 0x000000ff;
    const uint32_t gmask = 0x0000ff00;
    const uint32_t bmask = 0x00ff0000;
    const uint32_t amask = 0xff000000;

    surf = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
    assert(surf->format->format == SDL_PIXELFORMAT_RGBA32);
}

Surface::Surface(int width, int height, Color color) : Surface(width, height) {
    std::generate(pixels().begin(), pixels().end(), [color] { return color; });
}

Surface::Surface(const char* filename) {
    auto* s = IMG_Load(filename);
    if (s == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }
    if (s->format->format != SDL_PIXELFORMAT_RGBA32) {
        SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
        SDL_Surface* output = SDL_ConvertSurface(s, format, 0);
        SDL_FreeFormat(format);
        SDL_FreeSurface(s);
        s = output;
    }

    assert(s->format->format == SDL_PIXELFORMAT_RGBA32);
    surf = s;
}

Surface::Surface(SDL_Surface* s) : surf(s) {}

Surface::~Surface() { SDL_FreeSurface(surf); }

int Surface::width() const noexcept { return surf->w; }
int Surface::height() const noexcept { return surf->h; }
int Surface::size() const noexcept { return height() * width(); }

bool operator==(const Surface& sa, const Surface& sb) {
    return sa.height() == sb.height() && sa.width() == sb.width() &&
           std::equal(sa.pixels().begin(), sa.pixels().end(), sb.pixels().begin());
}

std::span<Color> Surface::pixels() noexcept {
    return std::span<Color>(reinterpret_cast<Color*>(surf->pixels), height() * width());
}

std::span<const Color> Surface::pixels() const noexcept {
    return std::span<const Color>(reinterpret_cast<Color*>(surf->pixels), height() * width());
}

std::span<Color> Surface::operator[](int row) noexcept {
    assert(row < height());
    return std::span<Color>(reinterpret_cast<Color*>(surf->pixels) + row * width(), width());
}

std::span<const Color> Surface::operator[](int row) const noexcept {
    assert(row < height());
    return std::span<const Color>(reinterpret_cast<Color*>(surf->pixels) + row * width(), width());
}

void Surface::saveBMP(const char* filename) const {
    auto ret = SDL_SaveBMP(surf, filename);
    if (ret != 0) {
        throw std::runtime_error(SDL_GetError());
    }
}

bool Surface::isInnerPoint(glm::ivec2 point) const {
    return (point.x >= 0 && point.x < width()) && (point.y >= 0 && point.y < height());
}

void Surface::drawLine(glm::ivec2 p0, glm::ivec2 p1, Color c) {
    assert(isInnerPoint(p0));
    assert(isInnerPoint(p1));

    // Swap points and coordinates if necessary, so we have a sloppy line
    // drawn from left to right
    bool steep = false;
    if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y)) {
        steep = true;
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
    }

    if (p0.x > p1.x) {
        std::swap(p0, p1);
    }

    // Using Bresenham's line generation algorithm
    const int dx = p1.x - p0.x;
    const int dy = p1.y - p0.y;
    const int derror = std::abs(dy) * 2;
    int error = 0;
    int y = p0.y;

    for (int x = p0.x; x <= p1.x; ++x) {
        if (steep) {
            xy(y, x) = c;
        } else {
            xy(x, y) = c;
        }

        error += derror;
        if (error > dx) {
            y += (p1.y > p0.y ? 1 : -1);
            error -= dx * 2;
        }
    }
}

} // namespace gfx
