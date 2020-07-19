//
// Created by ripopov on 6/7/2020.
//

#pragma once

#include "model.h"
#include "surface.h"
#include "glm/mat4x4.hpp"

namespace gfx
{

using z_buffer_t = std::vector<std::vector<float>>;

void renderToTarget(z_buffer_t& zbuf, const Model& model, Surface &target, const glm::mat4& transform);

}
