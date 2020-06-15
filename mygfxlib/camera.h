//
// Created by ripopov on 6/13/2020.
//

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace gfx {

class Camera {
public:

    glm::mat4x4 viewMtx() const;
    glm::mat4x4 projMtx() const;

    // Move forward / backward
    void move(float distance);
    // Move left/right
    void strafe(float distance);
    // Rotate around Up axis
    void rotateRoundUp(float theta_rad);
    // Rotate around Right dir axis
    void rotateRoundRight(float theta_rad);

    void setAspectRatio(int width, int height);

private:
    glm::vec3 position{0, 0, -4};
    glm::vec3 right_dir{-1, 0, 0};
    glm::vec3 up_dir{0, 1, 0};
    glm::vec3 look_dir{0, 0, 1};
    float aratio = 1.0f;
};

} // namespace gfx
