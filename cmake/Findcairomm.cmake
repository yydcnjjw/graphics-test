find_package(PkgConfig REQUIRED)

if (PKG_CONFIG_FOUND)
  pkg_check_modules(cairomm REQUIRED cairomm-1.0)
endif()
