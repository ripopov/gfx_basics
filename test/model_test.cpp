//
// Created by ripopov on 6/7/2020.
//

#include "model.h"
#include "render.h"
#include "tiny_obj_loader.h"
#include "gtest/gtest.h"

using namespace gfx;

namespace {

TEST(model_test, teapot_model) {

    Model tpot{"teapot/teapot.obj"};
    Surface render_target{1000, 1000, black};
    renderWireframe(tpot, render_target, 0.01f);
    render_target.saveBMP("teapot.bmp");
}

} // namespace