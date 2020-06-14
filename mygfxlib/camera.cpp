//
// Created by ripopov on 6/13/2020.
//

#include "camera.h"
#include <glm/ext.hpp>

namespace gfx {

glm::mat4x4 Camera::viewMtx() const { return glm::lookAt(position, position + look_dir, up_dir); }
glm::mat4x4 Camera::projMtx() const { return glm::perspective(glm::radians(45.0f), aratio, 0.1f, 100.f); }

void Camera::move(float distance) { position = position + (look_dir * distance); }
void Camera::strafe(float distance) { position = position + (right_dir * distance); }
void Camera::setAspectRatio(int width, int height) { aratio = float(width) / float(height); }

void Camera::rotateRoundUp(float theta_rad) {
    auto rot = glm::rotate(theta_rad, up_dir);
    right_dir = rot * glm::vec4{right_dir, 0.0f};
    look_dir = rot * glm::vec4{look_dir, 0.0f};
}

} // namespace gfx
