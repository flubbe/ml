#pragma once

#include <vector>
#include <random>

/*
 * Helpers.
 */

void random_initialize_real_std_vector(
  std::size_t n,
  std::vector<ml::vec4>& v)
{
    std::random_device rnd_device;
    std::mt19937 mersenne_engine{rnd_device()};
    std::uniform_real_distribution<float> dist{-1, 1};

    auto gen = [&dist, &mersenne_engine]()
    {
        return ml::vec4{dist(mersenne_engine), dist(mersenne_engine), dist(mersenne_engine), dist(mersenne_engine)};
    };

    v.resize(n);
    generate(std::begin(v), std::end(v), gen);
}

/*
 * memory alignment.
 */

inline bool is_aligned(
  const void* ptr,
  std::uintptr_t alignment) noexcept
{
    auto iptr = reinterpret_cast<std::uintptr_t>(ptr);
    return !(iptr % alignment);
}

namespace ml
{

inline std::ostream& operator<<(
  std::ostream& os,
  const mat3x3& m)
{
    return os
           << "[[" << m.rows[0].x << ", " << m.rows[0].y << ", " << m.rows[0].z << "], "
           << "[" << m.rows[1].x << ", " << m.rows[1].y << ", " << m.rows[1].z << "], "
           << "[" << m.rows[2].x << ", " << m.rows[2].y << ", " << m.rows[2].z << "]]";
}

inline std::ostream& operator<<(
  std::ostream& os,
  const std::optional<mat3x3>& m)
{
    if(m.has_value())
    {
        return os << m.value();
    }

    return os << "<none>";
}

inline std::ostream& operator<<(
  std::ostream& os,
  const mat4x4& m)
{
    return os
           << "[[" << m.rows[0].x << ", " << m.rows[0].y << ", " << m.rows[0].z << ", " << m.rows[0].w << "], "
           << "[" << m.rows[1].x << ", " << m.rows[1].y << ", " << m.rows[1].z << ", " << m.rows[1].w << "], "
           << "[" << m.rows[2].x << ", " << m.rows[2].y << ", " << m.rows[2].z << ", " << m.rows[2].w << "], "
           << "[" << m.rows[3].x << ", " << m.rows[3].y << ", " << m.rows[3].z << ", " << m.rows[3].w << "]]";
}

inline std::ostream& operator<<(
  std::ostream& os,
  const std::optional<mat4x4>& m)
{
    if(m.has_value())
    {
        return os << m.value();
    }

    return os << "<none>";
}

#ifdef ML_INCLUDE_SIMD

namespace simd
{

inline std::ostream& operator<<(
  std::ostream& os,
  const mat4x4& m)
{
    return os
           << "[[" << m.rows[0].x << ", " << m.rows[0].y << ", " << m.rows[0].z << ", " << m.rows[0].w << "], "
           << "[" << m.rows[1].x << ", " << m.rows[1].y << ", " << m.rows[1].z << ", " << m.rows[1].w << "], "
           << "[" << m.rows[2].x << ", " << m.rows[2].y << ", " << m.rows[2].z << ", " << m.rows[2].w << "], "
           << "[" << m.rows[3].x << ", " << m.rows[3].y << ", " << m.rows[3].z << ", " << m.rows[3].w << "]]";
}

inline std::ostream& operator<<(
  std::ostream& os,
  const std::optional<mat4x4>& m)
{
    if(m.has_value())
    {
        return os << m.value();
    }

    return os << "<none>";
}

}    // namespace simd

#endif

}    // namespace ml
