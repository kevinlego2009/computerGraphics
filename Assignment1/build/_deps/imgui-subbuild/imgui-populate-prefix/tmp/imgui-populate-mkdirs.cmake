# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/imgui-src")
  file(MAKE_DIRECTORY "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/imgui-src")
endif()
file(MAKE_DIRECTORY
  "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/imgui-build"
  "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/imgui-subbuild/imgui-populate-prefix"
  "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/imgui-subbuild/imgui-populate-prefix/tmp"
  "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp"
  "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/imgui-subbuild/imgui-populate-prefix/src"
  "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
