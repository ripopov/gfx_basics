//
// Created by ripopov on 6/7/2020.
//

#include "render.h"
#include "color.h"

#include <vector>
#include <iostream>
#include <algorithm>

namespace gfx {

static glm::vec3 ToScreenSpace(glm::vec3 in, const Surface& target) {
    glm::vec3 out = in;
    out.x = (((in.x + 1) / 2) * target.width()), 0, target.width() - 1;
    out.y = (((-in.y + 1) / 2) * target.height()), 0, target.height() - 1;
    return out;
}

static bool isBackFace(const tri_vec3& ndc)
{
    auto norm = glm::cross(ndc[1] - ndc[0], ndc[2] - ndc[0]);
    return norm.z < 0;
}

glm::vec3 normal(const tri_vec3& tri)
{
    auto norm = glm::cross(tri[1] - tri[0], tri[2] - tri[0]);
    return glm::normalize(norm);
}

static std::vector<tri_vec4> clipTriangle(const tri_vec4& tri_clip)
{
    // All vertices are in front of the camera and Z inside the frustum.
    if (tri_clip[0].w > 0 && tri_clip[1].w > 0 && tri_clip[2].w > 0
        && (std::abs(tri_clip[0].z) < tri_clip[0].w)
        && (std::abs(tri_clip[1].z) < tri_clip[1].w)
        && (std::abs(tri_clip[2].z) < tri_clip[2].w)
        ) {
        return {tri_clip};
    }

    // TODO: Support triangles partially behind near plane

    return {};
}

static void drawTriangleWireframe(const tri_vec3& tri_scr, Surface& target) {

    const auto color = cyan;

    target.drawLine(glm::ivec2{tri_scr[0].x, tri_scr[0].y}, glm::ivec2{tri_scr[1].x, tri_scr[1].y}, color);

    target.drawLine(glm::ivec2{tri_scr[1].x, tri_scr[1].y}, glm::ivec2{tri_scr[2].x, tri_scr[2].y}, color);

    target.drawLine(glm::ivec2{tri_scr[2].x, tri_scr[2].y}, glm::ivec2{tri_scr[0].x, tri_scr[0].y}, color);
}

static std::array<glm::vec2, 2> boundingBox(const tri_vec3& tri_scr, int target_width, int target_height)
{
    glm::vec2 max_xy{0.0, 0.0};
    glm::vec2 clamp_xy{float(target_width - 1), float(target_height - 1)};
    glm::vec2 min_xy{clamp_xy.x, clamp_xy.y};

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

static void drawTriangle(const tri_vec3& tri_scr, const tri_vec4& tri_clip, float intensity, Surface& target, z_buffer_t& zbuf,
                         const Model& model, int shape_idx, int face_idx) {

    const auto bbox = boundingBox(tri_scr, target.width(), target.height());

    const auto mat_idx = model.shapes[shape_idx].mesh.material_ids[face_idx];
    const auto& texture = *model.mat_diff_tex[mat_idx];

    for (int x = int(bbox[0].x); x <= int(bbox[1].x); ++x) {
        for (int y = int(bbox[0].y); y <= int(bbox[1].y); ++y) {

            const auto bary = barycentric(glm::vec2{x, y}, tri_scr[0], tri_scr[1], tri_scr[2]);

            // perspective-correct interpolation coefficients
            glm::vec3 bary_persp = bary;
            for (int i = 0; i < 3; ++i) {
                bary_persp[i] = bary_persp[i] / tri_clip[i].w;
            }
            bary_persp = bary_persp / (bary_persp.x + bary_persp.y + bary_persp.z);

            if (bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0) {

                const auto txface = model.face_texcoords(model.shapes[shape_idx], face_idx);
                const auto txpoint = txface[0] * bary_persp[0] + txface[1] * bary_persp[1] + txface[2] * bary_persp[2];

                const float z = (tri_scr[0].z * bary[0]) + (tri_scr[1].z * bary[1]) + (tri_scr[2].z * bary[2]);

                if (z < zbuf[x][y]) {
                    zbuf[x][y] = z;
                    auto color = texture.xy(static_cast<int>(txpoint.x * (texture.width() - 1)),
                                            static_cast<int>(txpoint.y * (texture.height() - 1)));

                    if (intensity > 0.05) {
                        color = color * intensity;
                    } else {
                        color = color * 0.05f;
                    }

                    target.xy(x, y) = color;
                }
            }
        }
    }
}

static const glm::vec3 flash_light_dir{0,0,1};

void renderToTarget(z_buffer_t& zbuf, const Model& model, Surface& target, const glm::mat4& transform) {

    // Loop over shapes
    for (int s = 0; s < model.shapes.size(); s++) {

        const auto& shape = model.shapes[s];

        // Loop over faces(polygon)
        for (int f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            auto vertices = model.face_vertices(shape, f);

            tri_vec4 vertices_clip;
            for (int i = 0; i < 3; ++i) {
                vertices_clip[i] = transform * glm::vec4(vertices[i], 1.0f);
            }

            // Loop over clipper output
            for(const auto& clipped_tri : clipTriangle(vertices_clip))
            {
                tri_vec3 vertices_ndc;
                for (int i = 0; i < 3; ++i) {
                    vertices_ndc[i] = clipped_tri[i] / clipped_tri[i].w;
                }

                if (isBackFace(vertices_ndc))
                    continue;

                const glm::vec3 n = normal({vertices_clip[0], vertices_clip[1], vertices_clip[2]});

                const auto intensity = glm::dot(n, flash_light_dir);

                tri_vec3 vertices_screen;
                for (int i = 0; i < 3; ++i) {
                    vertices_screen[i] = ToScreenSpace(vertices_ndc[i], target);
                }

                drawTriangle(vertices_screen, vertices_clip, intensity, target, zbuf, model, s, f);
                // drawTriangleWireframe(vertices_screen, target);
            }
        }
    }
}

} // namespace gfx