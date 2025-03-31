# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/stb-src")
  file(MAKE_DIRECTORY "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/stb-src")
endif()
file(MAKE_DIRECTORY
  "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/stb-build"
  "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/stb-subbuild/stb-populate-prefix"
  "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/stb-subbuild/stb-populate-prefix/tmp"
  "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/stb-subbuild/stb-populate-prefix/src/stb-populate-stamp"
  "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/stb-subbuild/stb-populate-prefix/src"
  "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/stb-subbuild/stb-populate-prefix/src/stb-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/stb-subbuild/stb-populate-prefix/src/stb-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/kevinyu/Desktop/graphics/Assignment1/build/_deps/stb-subbuild/stb-populate-prefix/src/stb-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
