cmake_minimum_required(VERSION 3.2)

project(nickname VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

configure_file(version.h.in ${CMAKE_BINARY_DIR}/version.h)
include_directories(${CMAKE_BINARY_DIR})

include(conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(src)
add_subdirectory(test)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT gpgolikov@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
