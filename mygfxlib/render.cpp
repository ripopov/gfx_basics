//
// Created by ripopov on 6/7/2020.
//

#include "render.h"
#include "color.h"

#include <vector>

namespace gfx {

static glm::ivec3 ToScreenSpace(glm::vec3 in, const Surface& target) {
    glm::ivec3 out = in;
    out.x = std::clamp<int>(int(((in.x + 1) / 2) * target.width()), 0, target.width() - 1);
    out.y = std::clamp<int>(int(((-in.y + 1) / 2) * target.height()), 0, target.height() - 1);
    return out;
}

static bool isBackFace(const tri_vec3& ndc)
{
    auto norm = glm::cross(ndc[1] - ndc[0], ndc[2] - ndc[0]);
    return norm.z < 0;
}

static bool rejectNegW(const tri_vec4& clip)
{
    if (clip[0].w < 0 && clip[1].w < 0 && clip[2].w < 0) {
        return true;
    }
    return false;
}

static bool clipTrivialReject(const tri_vec3& ndc) {

    if (std::any_of(begin(ndc), end(ndc), [](const auto& vert) {
            return (vert.x > 1 || vert.x < -1) || (vert.y > 1 || vert.y < -1) || (vert.z > 1 || vert.z < -1);
        })) {
        return true;
    }

    return false;
}

static void drawTriangleWireframe(const tri_vec3& tri_scr, Surface& target) {

    const auto color = cyan;

    target.drawLine(glm::ivec2{tri_scr[0].x, tri_scr[0].y}, glm::ivec2{tri_scr[1].x, tri_scr[1].y}, color);

    target.drawLine(glm::ivec2{tri_scr[1].x, tri_scr[1].y}, glm::ivec2{tri_scr[2].x, tri_scr[2].y}, color);

    target.drawLine(glm::ivec2{tri_scr[2].x, tri_scr[2].y}, glm::ivec2{tri_scr[0].x, tri_scr[0].y}, color);
}

static std::array<glm::vec2, 2> boundingBox(const tri_vec3& tri_scr, int target_width, int target_height)
{
    glm::vec2 max_xy{std::numeric_limits<float>::min(), std::numeric_limits<float>::min()};
    glm::vec2 min_xy{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
    glm::vec2 clamp_xy{float(target_width - 1), float(target_height - 1)};

    for (const auto& v : tri_scr) {
        for (int xy = 0; xy < 2; ++xy) {
            max_xy[xy] = std::min(clamp_xy[xy], std::max(max_xy[xy], v[xy]));
            min_xy[xy] = std::max(0.f, std::min(min_xy[xy], v[xy]));
        }
    }

    return {min_xy, max_xy};
}

static glm::vec3 barycentric(glm::vec2 point, glm::vec2 tri_a, glm::vec2 tri_b, glm::vec2 tri_c) {

    const auto ab = tri_b - tri_a;
    const auto ac = tri_c - tri_a;
    const auto pa = tri_a - point;

    const auto u = glm::cross(glm::vec3{ac.x, ab.x, pa.x}, glm::vec3{ac.y, ab.y, pa.y});

    if (std::abs(u.z) < 0.01f)
    {
        // degenerate triangle, will be discarded
        return {-1, -1, -1};
    }
    // {alpha, beta, gamma}
    return {1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
}

static void drawTriangle(const tri_vec3& tri_scr, Surface& target) {
    const auto bbox = boundingBox(tri_scr, target.width(), target.height());

    for (int x = int(bbox[0].x); x <= int(bbox[1].x); ++x) {
        for (int y = int(bbox[0].y); y <= int(bbox[1].y); ++y) {

            const auto bary = barycentric(glm::vec2{x, y}, tri_scr[0], tri_scr[1], tri_scr[2]);

            if (bary.x >= 0 && bary.y >= 0 && bary.z >= 0) {
                target.xy(x, y) = blue;
            }
        }
    }
}

void renderToTarget(const Model& model, Surface& target, const glm::mat4& transform) {

    // Loop over shapes
    for (size_t s = 0; s < model.shapes.size(); s++) {

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

            drawTriangle(vertices_screen, target);
            drawTriangleWireframe(vertices_screen, target);
        }
    }
}

} // namespace gfx