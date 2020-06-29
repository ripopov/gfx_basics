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

bool isBackFace(const tri_vec3& ndc)
{
    auto norm = glm::cross(ndc[1] - ndc[0], ndc[2] - ndc[0]);
    return norm.z < 0;
}

bool rejectNegW(const tri_vec4& clip)
{
    if (clip[0].w < 0 && clip[1].w < 0 && clip[2].w < 0) {
        return true;
    }
    return false;
}

bool clipTrivialReject(const tri_vec3& ndc)
{
    for(auto& vert : ndc)
    {
        if (vert.x > 1 || vert.x < -1 )
            return true;
        if (vert.y > 1 || vert.y < -1 )
            return true;
        if (vert.z > 1 || vert.z < -1 )
            return true;
    }

    return false;
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

            tri_vec4 vertices_clip;
            for (int i = 0; i < 3; ++i) {
                vertices_clip[i] = transform * glm::vec4(vertices[i], 1.0f);
            }

            if (rejectNegW(vertices_clip))
                continue;

            tri_vec3 vertices_ndc;
            for (int i = 0; i < 3; ++i) {
                vertices_ndc[i] = vertices_clip[i] / vertices_clip[i].w;
            }

            if (clipTrivialReject(vertices_ndc))
                continue;;

            if (isBackFace(vertices_ndc))
                continue;

            tri_vec3 vertices_screen;
            for (int i = 0; i < 3; ++i) {
                vertices_screen[i] = ToScreenSpace(vertices_ndc[i], target);
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