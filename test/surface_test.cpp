//
// Created by ripopov on 6/7/2020.
//

#include "color.h"
#include "surface.h"
#include "gtest/gtest.h"

using namespace gfx;

namespace {

TEST(surface_test, bmp_surface) {
    Surface s1{100, 100, black};
    for (int row = 0; row < s1.height(); ++row) {
        s1[row][20] = red;
        s1[row][40] = green;
        s1[row][60] = blue;
    }
    s1.saveBMP("bmp_surface_s1.bmp");
    Surface s2("bmp_surface_s1.bmp");
    EXPECT_EQ(s1, s2);
    s2[10][11] = white;
    EXPECT_NE(s1, s2);
}

TEST(surface_test, draw_line) {
    Surface s1{200, 100, black};

    // From center
    {
        // right
        s1.drawLine({98, 49}, {199, 49}, white);
        // left
        s1.drawLine({98, 48}, {0, 48}, red);
        // down
        s1.drawLine({101, 51}, {101, 99}, green);
        // up
        s1.drawLine({100, 51}, {100, 0}, blue);
    }

    // From top-left
    {
        s1.drawLine({0, 0}, {97, 20}, teal);
        s1.drawLine({0, 0}, {97, 40}, cyan);
        s1.drawLine({0, 0}, {60, 45}, violet);
    }

    // From top-right
    {
        s1.drawLine({199, 0}, {103, 20}, cobalt);
        s1.drawLine({199, 0}, {103, 40}, orange);
        s1.drawLine({199, 0}, {135, 45}, pink);
    }

    // From bottom-left
    {
        s1.drawLine({0, 99}, {60, 54}, brown);
        s1.drawLine({0, 99}, {97, 54}, olive);
        s1.drawLine({0, 99}, {97, 70}, steel);
    }

    // From bottom-right
    {
        s1.drawLine({199, 99}, {60, 54}, yellow);
        s1.drawLine({199, 99}, {97, 54}, lime);
        s1.drawLine({199, 99}, {97, 70}, magenta);
    }

    s1.saveBMP("draw_line.bmp");
}

} // namespace
