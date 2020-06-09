add_executable(unittest geometry_test.cpp surface_test.cpp model_test.cpp)

target_link_libraries(unittest mygfx fmt::fmt glm GTest::gtest_main)