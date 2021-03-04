# MIT License
# 
# Copyright (c) 2021 Konstantin Rolf
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(dotsandboxes)

# CMake Version control
set(CMAKE_PROJECT_VERSION 1.0.0)
set(CMAKE_PROJECT_VERSION_MAJOR 1)
set(CMAKE_PROJECT_VERSION_MINOR 0)
set(CMAKE_PROJECT_VERSION_PATCH 0)

# The project and dependencies rely on the CXX17 standard.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# All source files in this project
set(DAB_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/platform.c"
)
# All header files in this project
set(DAB_HEADERS
   "${CMAKE_CURRENT_SOURCE_DIR}/include/platform.h"
)

# Prefer libc++ in conjunction with Clang, not on Windows
if (NOT MSVC)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT CMAKE_CXX_FLAGS MATCHES "-stdlib=libc\\+\\+")
    CHECK_CXX_COMPILER_AND_LINKER_FLAGS_BASE(HAS_LIBCPP "-stdlib=libc++" "-stdlib=libc++")
    if (HAS_LIBCPP)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -D_LIBCPP_VERSION")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
      set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
      message(STATUS "Nyrem: using libc++.")
    else()
    CHECK_CXX_COMPILER_AND_LINKER_FLAGS_BASE(HAS_LIBCPP_AND_CPPABI "-stdlib=libc++" "-stdlib=libc++ -lc++abi")
      if (HAS_LIBCPP_AND_CPPABI)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -D_LIBCPP_VERSION")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
        message(STATUS "Nyrem: using libc++ and libc++abi.")
      else()
        message(FATAL_ERROR "When Clang is used to compile Nyrem, libc++ must be available -- GCC's libstdc++ is not supported! (please insteall the libc++ development headers, provided e.g. by the packages 'libc++-dev' and 'libc++abi-dev' on Debian/Ubuntu).")
      endif()
    endif()
  endif()
endif()

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    # add_compile_options(-Wall -pedantic -Os -ffunction-sections -fdata-sections)
    # add_link_options(-Wl,--gc-sections)
    add_compile_options(-Wno-trigraphs)
endif()


set(DAB_INCLUDE  # Stores all Includes used by all targets
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(${PROJECT_NAME} ${DAB_SOURCES} ${DAB_HEADERS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_DL_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC ${DAB_INCLUDE})


