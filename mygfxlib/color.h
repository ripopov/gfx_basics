//
// Created by ripopov on 6/7/2020.
//

#pragma once

#include <glm/vec4.hpp>

namespace gfx {

/// 4 byte RGBA Color
struct Color {
    uint8_t r{0};
    uint8_t g{0};
    uint8_t b{0};
    uint8_t a{255};

    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff) : r(r), g(g), b(b), a(a) {}

    Color& operator=(uint32_t rgba) {
        *reinterpret_cast<uint32_t*>(this) = rgba;
        return *this;
    }

    Color& operator*(float intensity)
    {
        r = static_cast<uint8_t>(r * intensity);
        g = static_cast<uint8_t>(g * intensity);
        b = static_cast<uint8_t>(b * intensity);
        return *this;
    }

    operator uint32_t() const { return *reinterpret_cast<const uint32_t*>(this); }
};

static_assert(sizeof(Color) == 4);

constexpr inline Color white{255, 255, 255};
constexpr inline Color black{0, 0, 0};
constexpr inline Color red{255, 0, 0};
constexpr inline Color green{0, 255, 0};
constexpr inline Color blue{0, 0, 255};
constexpr inline Color cyan{27, 161, 226};
constexpr inline Color cobalt{0, 80, 239};
constexpr inline Color teal{0, 171, 169};
constexpr inline Color violet{170, 0, 255};
constexpr inline Color orange{250, 104, 0};
constexpr inline Color pink{244, 114, 208};
constexpr inline Color brown{130, 90, 44};
constexpr inline Color olive{109, 135, 100};
constexpr inline Color steel{100, 118, 135};
constexpr inline Color yellow{227, 200, 0};
constexpr inline Color lime{164, 196, 0};
constexpr inline Color magenta{216, 0, 115};

} // namespace gfx
