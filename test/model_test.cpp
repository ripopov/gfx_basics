//
// Created by ripopov on 6/7/2020.
//

#include "model.h"
#include "render.h"
#include "tiny_obj_loader.h"
#include "gtest/gtest.h"
#include <glm/ext/matrix_transform.hpp>

using namespace gfx;

namespace {

TEST(model_test, teapot_model) {

    Model tpot{"teapot/teapot.obj"};
    Surface render_target{1000, 1000, black};

    auto m4 = glm::identity<glm::mat4>();
    auto rot = glm::rotate(m4, glm::radians(30.0f), glm::vec3{0.5, 0, 0.5});

    renderWireframe(tpot, render_target, rot, .01f);
    render_target.saveBMP("teapot.bmp");
}

} // namespace