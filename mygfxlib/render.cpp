//
// Created by ripopov on 6/7/2020.
//

#include "render.h"
#include "color.h"

#include <vector>

namespace gfx {

glm::ivec3 ToScreenSpace(glm::vec3 in, const Surface& target) {
    glm::ivec3 out = in;
    out.x = std::clamp<int>(int(((in.x + 1) / 2) * target.width()), 0, target.width() - 1);
    out.y = std::clamp<int>(int(((-in.y + 1) / 2) * target.height()), 0, target.height() - 1);
    return out;
}

void renderWireframe(const Model& model, Surface& target, const glm::mat4& transform) {
    std::vector<Color> colors{red, green, blue};

    // Loop over shapes
    for (size_t s = 0; s < model.shapes.size(); s++) {

        const auto color = colors[s % colors.size()];
        const auto& shape = model.shapes[s];

        // Loop over faces(polygon)
        size_t index_offset = 0;

        for (int f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            auto vertices = model.face_vertices(shape, f);

            tri_vec3 vertices_screen;
            for (int i = 0; i < vertices_screen.size(); ++i) {
                glm::vec4 verth = transform * glm::vec4 (vertices[i], 1.0f);
                vertices[i] = verth / verth.w;
                vertices_screen[i] = ToScreenSpace(vertices[i], target);
            }

            target.drawLine(glm::ivec2{vertices_screen[0].x, vertices_screen[0].y},
                            glm::ivec2{vertices_screen[1].x, vertices_screen[1].y}, color);

            target.drawLine(glm::ivec2{vertices_screen[1].x, vertices_screen[1].y},
                            glm::ivec2{vertices_screen[2].x, vertices_screen[2].y}, color);

            target.drawLine(glm::ivec2{vertices_screen[2].x, vertices_screen[2].y},
                            glm::ivec2{vertices_screen[0].x, vertices_screen[0].y}, color);
        }
    }
}

} // namespace gfx