#define STB_IMAGE_IMPLEMENTATION

#include <app.hpp>

int main(int argc, char *argv[]) {
  try {
    return my::App::run();
  } catch (std::exception &e) {
    SPDLOG_ERROR(e.what());
  }
}
