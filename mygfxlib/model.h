//
// Created by ripopov on 6/7/2020.
//

#pragma once

#include "surface.h"
#include "tiny_obj_loader.h"
#include <array>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace gfx {

using tri_vec3 = std::array<glm::vec3, 3>;

/**
 * Model, loaded from Wavefront obj files
 * A right-hand coordinate system is used to specify the coordinate locations.
 * All models will be triangulated.
 */
struct Model {
    /**
     * Load model from file, materials and textures should be located in same directory as obj file
     */
    explicit Model(const std::string& model_obj_file);

    [[nodiscard]] tri_vec3 face_vertices(const tinyobj::shape_t& shape, int face) const {
        tri_vec3 res;

        for (int v = 0; v < 3; ++v) {
            tinyobj::index_t idx = shape.mesh.indices[face * 3 + v];
            tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
            tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
            tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
            res[v] = glm::vec3{vx, vy, vz};
        }

        return res;
    }

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    /// have same size as materials. stores a pointer to diffuse texture for corresponding material.
    std::vector<const Surface*> mat_diff_tex;
    std::unordered_map<std::string, Surface> diffuse_textures;
};

} // namespace gfx
