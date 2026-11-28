/* C++ headers */
#include <array>
#include <random>

/* benchmark library */
#include <benchmark/benchmark.h>

/* user headers */
#include "ml/all.h"

using namespace ml;

// ---- SIMD vec4 benchmarks ----

static std::array<float, 4> make_data(int i)
{
    return {{
      static_cast<float>((i * 1 + 7) % 13) / 3.f,
      static_cast<float>((i * 3 + 11) % 17) / 5.f,
      static_cast<float>((i * 7 + 3) % 19) / 7.f,
      static_cast<float>((i * 11 + 5) % 23) / 11.f,
    }};
}

static void vec4_add_simd(benchmark::State& state)
{
    auto a = make_data(0);
    auto b = make_data(1);
    simd::vec4 va(a.data());
    simd::vec4 vb(b.data());
    for(auto _: state)
        benchmark::DoNotOptimize(va + vb);
}
BENCHMARK(vec4_add_simd);

static void vec4_sub_simd(benchmark::State& state)
{
    auto a = make_data(0);
    auto b = make_data(1);
    simd::vec4 va(a.data());
    simd::vec4 vb(b.data());
    for(auto _: state)
        benchmark::DoNotOptimize(va - vb);
}
BENCHMARK(vec4_sub_simd);

static void vec4_mul_scalar_simd(benchmark::State& state)
{
    auto a = make_data(0);
    simd::vec4 va(a.data());
    float s = 2.5f;
    for(auto _: state)
        benchmark::DoNotOptimize(va * s);
}
BENCHMARK(vec4_mul_scalar_simd);

static void vec4_dot_simd(benchmark::State& state)
{
    auto a = make_data(0);
    auto b = make_data(1);
    simd::vec4 va(a.data());
    simd::vec4 vb(b.data());
    for(auto _: state)
        benchmark::DoNotOptimize(dot(va, vb));
}
BENCHMARK(vec4_dot_simd);

static void vec4_length_simd(benchmark::State& state)
{
    auto a = make_data(0);
    simd::vec4 va(a.data());
    for(auto _: state)
        benchmark::DoNotOptimize(va.length());
}
BENCHMARK(vec4_length_simd);

static void vec4_normalize_simd(benchmark::State& state)
{
    auto a = make_data(0);
    simd::vec4 va(a.data());
    for(auto _: state)
        benchmark::DoNotOptimize(va.normalized());
}
BENCHMARK(vec4_normalize_simd);

static void vec4_swizzle_simd(benchmark::State& state)
{
    simd::vec4 va(1.f, 2.f, 3.f, 4.f);
    for(auto _: state)
        benchmark::DoNotOptimize(va.yxzw());
}
BENCHMARK(vec4_swizzle_simd);

// ---- Scalar vec4 benchmarks ----

static void vec4_add_scalar(benchmark::State& state)
{
    std::array<float, 4> a = {1.f, 2.f, 3.f, 4.f};
    std::array<float, 4> b = {5.f, 6.f, 7.f, 8.f};
    scalar::vec4 va(a.data());
    scalar::vec4 vb(b.data());
    for(auto _: state)
        benchmark::DoNotOptimize(va + vb);
}
BENCHMARK(vec4_add_scalar);

static void vec4_sub_scalar(benchmark::State& state)
{
    std::array<float, 4> a = {1.f, 2.f, 3.f, 4.f};
    std::array<float, 4> b = {5.f, 6.f, 7.f, 8.f};
    scalar::vec4 va(a.data());
    scalar::vec4 vb(b.data());
    for(auto _: state)
        benchmark::DoNotOptimize(va - vb);
}
BENCHMARK(vec4_sub_scalar);

static void vec4_mul_scalar_vec(benchmark::State& state)
{
    std::array<float, 4> a = {1.f, 2.f, 3.f, 4.f};
    scalar::vec4 va(a.data());
    float s = 2.5f;
    for(auto _: state)
        benchmark::DoNotOptimize(va * s);
}
BENCHMARK(vec4_mul_scalar_vec);

static void vec4_dot_scalar(benchmark::State& state)
{
    std::array<float, 4> a = {1.f, 2.f, 3.f, 4.f};
    std::array<float, 4> b = {5.f, 6.f, 7.f, 8.f};
    scalar::vec4 va(a.data());
    scalar::vec4 vb(b.data());
    for(auto _: state)
        benchmark::DoNotOptimize(dot(va, vb));
}
BENCHMARK(vec4_dot_scalar);

static void vec4_length_scalar(benchmark::State& state)
{
    std::array<float, 4> a = {1.f, 2.f, 3.f, 4.f};
    scalar::vec4 va(a.data());
    for(auto _: state)
        benchmark::DoNotOptimize(va.length());
}
BENCHMARK(vec4_length_scalar);

static void vec4_normalize_scalar(benchmark::State& state)
{
    std::array<float, 4> a = {1.f, 2.f, 3.f, 4.f};
    scalar::vec4 va(a.data());
    for(auto _: state)
        benchmark::DoNotOptimize(va.normalized());
}
BENCHMARK(vec4_normalize_scalar);

static void vec4_swizzle_scalar(benchmark::State& state)
{
    scalar::vec4 va(1.f, 2.f, 3.f, 4.f);
    for(auto _: state)
        benchmark::DoNotOptimize(va.yxzw());
}
BENCHMARK(vec4_swizzle_scalar);

BENCHMARK_MAIN();
