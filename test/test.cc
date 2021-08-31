#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <gtest/gtest.h>

#include <iostream>

GTEST_TEST(test, test) {

  glm::vec3 vec{};

  auto new_vec =
      glm::vec4(vec.x, vec.y, vec.z, 0) * glm::eulerAngleXYZ(10, 10, 0);

  std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
  std::cout << new_vec.x << " " << new_vec.y << " " << new_vec.z << std::endl;
}
