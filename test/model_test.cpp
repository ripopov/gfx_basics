//
// Created by ripopov on 6/7/2020.
//

#include "model.h"
#include "render.h"
#include "camera.h"
#include "tiny_obj_loader.h"
#include "gtest/gtest.h"
#include <glm/ext/matrix_transform.hpp>

using namespace gfx;

namespace {

TEST(model_test, teapot_model) {

    Model tpot{"teapot/teapot.obj"};
    Surface render_target{1000, 1000, black};

    auto m4 = glm::identity<glm::mat4>();
    auto rot = glm::rotate(m4, glm::radians(30.0f), glm::vec3{1.0f,0.0,0.0});
    auto modelMtx = glm::scale(rot, glm::vec3{0.01f, 0.01f, 0.01f});

    gfx::Camera cam;
    cam.move(1.5f);
    cam.rotateRoundUp(glm::radians(15.0f));
    cam.strafe(1.0f);
    auto trans = cam.projMtx()*cam.viewMtx()*modelMtx;

    renderWireframe(tpot, render_target, trans);
    render_target.saveBMP("teapot.bmp");
}


TEST(model_test, box_model) {
    Model tpot{"cube/cube.obj"};
    Surface render_target{1000, 1000, black};

    gfx::Camera cam;
    cam.move(-1.0);
    cam.strafe(1.0f);
    auto trans = cam.projMtx() * cam.viewMtx();

    renderWireframe(tpot, render_target, trans);
    render_target.saveBMP("cube.bmp");
}

} // namespace