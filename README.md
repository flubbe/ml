
# Simple header-only mathematics library

[![Build Status](https://travis-ci.com/flubbe/ml.svg?branch=main)](https://travis-ci.com/flubbe/ml)

## Description

A collection of mathematical classes and functions I use. The library is mainly a learning tool.

The library contains:
 - 2d/3d/4d float vector classes and a 4d float matrix class: `vec2, vec3, vec4, mat4x4`
 - templated 2d vector class `tvec2<T>`
 - planes and lines: `plane`, `line<T>` (with specializations for 3d and 4d lines `line3, line4`) and `create_line` for obtaining a line through two points 
 - special matrices, e.g. for projections, translation, scaling and rotation: namespace `matrices`
 - convenient names for fixed-point types from the compositional numeric library (`fixed_t` for 15.16 signed fixed-point, `fixed_24_8_t` for signed 23.8 fixed-point and `fixed_28_4_t` for signed 27.4 fixed-point) and some utility functions (`integral_part`, `round`, `to_float` and `truncate_unchecked`)
 - fixed-point 2d vector class using the compositional numeric library: `vec2_fixed`
 - a fixed-point representation of the (closed) unit interval [0,1]: `fixed_32_t`
 - linear interpolation for general types (`lerp`), clamp vector components to unit interval (`clamp_to_unit_interval`), number truncation (`truncate_unchecked`, only for positive numbers).

## Dependencies

By default, the main library header `ml/all.h` will also include some C++ math headers, some boost math headers and parts of the compositional numeric library (CNL).
This behavior can be disabled by defining `ML_NO_DEPS` before including `all.h`, and a fine-grain control is provided by the switches `ML_NO_CPP, ML_NO_BOOST, ML_NO_CNL`.

The tests rely on the [Boost.Test](https://www.boost.org) framework.

## Setup

As a header-only library, just include `all.h`. Note that the CNL needs a C++20-compatible tool chain.

By default, SSE versions of the functions and classes are used. Set `ML_NO_SIMD` to use non-SSE versions. Set `ML_INCLUDE_SIMD` to include both SSE and non-SSE versions of `vec4` and `mat4x4` (the SSE versions are found in the namespace `simd`). Currently the library uses up to SSE 4.1.

Tested on Linux, GCC 10.2 (with C++-17 enabled), CMake 3.19.6.

## References and other libraries

 - [Compositional Numeric Library](https://github.com/johnmcfarlane/cnl) 
 - [glm](https://github.com/g-truc/glm)
 - [DirectXMath](https://github.com/microsoft/DirectXMath)
