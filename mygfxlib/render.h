//
// Created by ripopov on 6/7/2020.
//

#pragma once

#include "model.h"
#include "surface.h"

namespace gfx
{

void renderWireframe(const Model& model, Surface &target, float scale = 1.0f);

}
