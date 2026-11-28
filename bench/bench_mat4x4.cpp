/* C++ headers */
#include <array>
#include <random>

/* benchmark library */
#include <benchmark/benchmark.h>

/* user headers */
#include "ml/all.h"

using namespace ml;

// ---- SIMD mat4x4 benchmarks ----

static simd::mat4x4 make_simd_matrix(int i)
{
    return simd::mat4x4{
      {static_cast<float>(i * 1 + 1), static_cast<float>(i * 1 + 2), static_cast<float>(i * 1 + 3), 1.f},
      {static_cast<float>(i * 2 + 1), static_cast<float>(i * 2 + 2), static_cast<float>(i * 2 + 3), 1.f},
      {static_cast<float>(i * 3 + 1), static_cast<float>(i * 3 + 2), static_cast<float>(i * 3 + 3), 1.f},
      {0.f, 0.f, 0.f, 1.f},
    };
}

static void mat4x4_add_simd(benchmark::State& state)
{
    auto a = make_simd_matrix(0);
    auto b = make_simd_matrix(1);
    for(auto _: state)
        benchmark::DoNotOptimize(a + b);
}
BENCHMARK(mat4x4_add_simd);

static void mat4x4_sub_simd(benchmark::State& state)
{
    auto a = make_simd_matrix(0);
    auto b = make_simd_matrix(1);
    for(auto _: state)
        benchmark::DoNotOptimize(a - b);
}
BENCHMARK(mat4x4_sub_simd);

static void mat4x4_mul_simd(benchmark::State& state)
{
    auto a = make_simd_matrix(0);
    auto b = make_simd_matrix(1);
    for(auto _: state)
        benchmark::DoNotOptimize(a * b);
}
BENCHMARK(mat4x4_mul_simd);

static void mat4x4_mul_scalar_simd(benchmark::State& state)
{
    auto a = make_simd_matrix(0);
    float s = 2.5f;
    for(auto _: state)
        benchmark::DoNotOptimize(a * s);
}
BENCHMARK(mat4x4_mul_scalar_simd);

static void mat4x4_mul_vec_simd(benchmark::State& state)
{
    auto m = make_simd_matrix(0);
    simd::vec4 v(1.f, 2.f, 3.f, 1.f);
    for(auto _: state)
        benchmark::DoNotOptimize(m * v);
}
BENCHMARK(mat4x4_mul_vec_simd);

static void mat4x4_transpose_simd(benchmark::State& state)
{
    auto m = make_simd_matrix(0);
    for(auto _: state)
        benchmark::DoNotOptimize(m.transposed());
}
BENCHMARK(mat4x4_transpose_simd);

static void mat4x4_determinant_simd(benchmark::State& state)
{
    auto m = make_simd_matrix(0);
    for(auto _: state)
        benchmark::DoNotOptimize(m.determinant());
}
BENCHMARK(mat4x4_determinant_simd);

static void mat4x4_inverse_simd(benchmark::State& state)
{
    auto m = make_simd_matrix(0);
    for(auto _: state)
        benchmark::DoNotOptimize(m.inverse());
}
BENCHMARK(mat4x4_inverse_simd);

// ---- Scalar mat4x4 benchmarks ----

static scalar::mat4x4 make_scalar_matrix(int i)
{
    return scalar::mat4x4{
      {static_cast<float>(i * 1 + 1), static_cast<float>(i * 1 + 2), static_cast<float>(i * 1 + 3), 1.f},
      {static_cast<float>(i * 2 + 1), static_cast<float>(i * 2 + 2), static_cast<float>(i * 2 + 3), 1.f},
      {static_cast<float>(i * 3 + 1), static_cast<float>(i * 3 + 2), static_cast<float>(i * 3 + 3), 1.f},
      {0.f, 0.f, 0.f, 1.f},
    };
}

static void mat4x4_add_scalar(benchmark::State& state)
{
    auto a = make_scalar_matrix(0);
    auto b = make_scalar_matrix(1);
    for(auto _: state)
        benchmark::DoNotOptimize(a + b);
}
BENCHMARK(mat4x4_add_scalar);

static void mat4x4_sub_scalar(benchmark::State& state)
{
    auto a = make_scalar_matrix(0);
    auto b = make_scalar_matrix(1);
    for(auto _: state)
        benchmark::DoNotOptimize(a - b);
}
BENCHMARK(mat4x4_sub_scalar);

static void mat4x4_mul_scalar_mat(benchmark::State& state)
{
    auto a = make_scalar_matrix(0);
    auto b = make_scalar_matrix(1);
    for(auto _: state)
        benchmark::DoNotOptimize(a * b);
}
BENCHMARK(mat4x4_mul_scalar_mat);

static void mat4x4_mul_scalar_float(benchmark::State& state)
{
    auto a = make_scalar_matrix(0);
    float s = 2.5f;
    for(auto _: state)
        benchmark::DoNotOptimize(a * s);
}
BENCHMARK(mat4x4_mul_scalar_float);

static void mat4x4_mul_vec_scalar(benchmark::State& state)
{
    auto m = make_scalar_matrix(0);
    scalar::vec4 v(1.f, 2.f, 3.f, 1.f);
    for(auto _: state)
        benchmark::DoNotOptimize(m * v);
}
BENCHMARK(mat4x4_mul_vec_scalar);

static void mat4x4_transpose_scalar(benchmark::State& state)
{
    auto m = make_scalar_matrix(0);
    for(auto _: state)
        benchmark::DoNotOptimize(m.transposed());
}
BENCHMARK(mat4x4_transpose_scalar);

static void mat4x4_determinant_scalar(benchmark::State& state)
{
    auto m = make_scalar_matrix(0);
    for(auto _: state)
        benchmark::DoNotOptimize(m.determinant());
}
BENCHMARK(mat4x4_determinant_scalar);

static void mat4x4_inverse_scalar(benchmark::State& state)
{
    auto m = make_scalar_matrix(0);
    for(auto _: state)
        benchmark::DoNotOptimize(m.inverse());
}
BENCHMARK(mat4x4_inverse_scalar);

BENCHMARK_MAIN();
