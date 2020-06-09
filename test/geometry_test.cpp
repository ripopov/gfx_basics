//
// Created by ripopov on 6/7/2020.
//

// Include all GLM core / GLSL features
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
// Include all GLM extensions
#include <fmt/format.h>
#include <glm/ext.hpp> // perspective, translate, rotate
#include <gtest/gtest.h>

namespace {

void print_m4(const glm::mat4& m4) {
    for (int row = 0; row < 4; ++row) {
        fmt::print("|{:10.4f} {:10.4f} {:10.4f} {:10.4f} |\n", m4[0][row], m4[1][row], m4[2][row], m4[3][row]);
    }
}

TEST(geometry_test, glm_test) {

    std::cout << sizeof(glm::vec4) << std::endl;

    glm::vec4 v = {2, 0, 0, 1};

    v.r;

    auto m4 = glm::identity<glm::mat4>();
    auto rot = glm::rotate(m4, glm::radians(90.0f), glm::vec3{0, 0, 1});

    print_m4(rot);

    std::cout << glm::to_string(rot * v) << std::endl;
    std::cout << glm::to_string(v * rot) << std::endl;
}

} // namespace