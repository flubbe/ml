/**
 * ml - simple header-only mathematics library
 * 
 * special matrices.
 * 
 * \author Felix Lubbe
 * \copyright Copyright (c) 2021
 * \license Distributed under the MIT software license (see accompanying LICENSE.txt).
 */

namespace ml
{

namespace matrices
{

/** Generate a perspective projection matrix (for a symmetric frustum). */
inline mat4x4 perspective_projection(float aspect, float fov, float znear, float zfar)
{
    /* aspect is width/height */
    const auto zrange = znear - zfar;
    const auto one_over_tan = 1.0f / std::tan(fov / 2.0f);

    return {
      {one_over_tan / aspect, 0, 0, 0},
      {0, one_over_tan, 0, 0},
      {0, 0, (zfar + znear) / zrange, 2 * znear * zfar / zrange},
      {0, 0, -1, 0}};
}

/** Generate an orthographic projection matrix. */
inline mat4x4 orthographic_projection(float left, float right, float bottom, float top, float near, float far)
{
    const auto one_over_width = 1.0f / (right - left);
    const auto one_over_height = 1.0f / (top - bottom);
    const auto one_over_depth = 1.0f / (far - near);

    return {
      {2 * one_over_width, 0, 0, -(right + left) * one_over_width},
      {0, 2 * one_over_height, 0, -(top + bottom) * one_over_height},
      {0, 0, 2 * one_over_depth, -(far + near) * one_over_depth},
      {0, 0, 0, 1}};
}

/** Generate a translation matrix. */
inline mat4x4 translation(float x, float y, float z)
{
    return {
      {1, 0, 0, x},
      {0, 1, 0, y},
      {0, 0, 1, z},
      {0, 0, 0, 1}};
}

/** Generate a 4-dimensional diagonal matrix. */
inline mat4x4 diagonal(float x, float y, float z, float w)
{
    return {
      {x, 0, 0, 0},
      {0, y, 0, 0},
      {0, 0, z, 0},
      {0, 0, 0, w}};
}

/** Generate a scaling matrix. */
inline mat4x4 scaling(float s)
{
    return {
      {s, 0, 0, 0},
      {0, s, 0, 0},
      {0, 0, s, 0},
      {0, 0, 0, 1}};
}

/** Generate a right-handed rotation matrix w.r.t. x-axis. */
inline mat4x4 rotation_x(float angle)
{
    float c = std::cos(angle);
    float s = std::sin(angle);

    return {
      {1, 0, 0, 0},
      {0, c, -s, 0},
      {0, s, c, 0},
      {0, 0, 0, 1}};
}

/** Generate a right-handed rotation matrix w.r.t. y-axis. */
inline mat4x4 rotation_y(float angle)
{
    float c = std::cos(angle);
    float s = std::sin(angle);

    return {
      {c, 0, s, 0},
      {0, 1, 0, 0},
      {-s, 0, c, 0},
      {0, 0, 0, 1}};
}

/** Generate a right-handed rotation matrix w.r.t. z-axis. */
inline mat4x4 rotation_z(float angle)
{
    float c = std::cos(angle);
    float s = std::sin(angle);

    return {
      {c, -s, 0, 0},
      {s, c, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}};
}

/** Generate a right-handed rotation matrix w.r.t. the given axis. */
inline mat4x4 rotation(ml::vec3 axis, float angle)
{
    float c = std::cos(angle);
    float s = std::sin(angle);

    float x_sqr = axis.x * axis.x;
    float y_sqr = axis.y * axis.y;
    float z_sqr = axis.z * axis.z;

    float xy = axis.x * axis.y;
    float xz = axis.x * axis.z;
    float yz = axis.y * axis.z;

    return {
      {c + x_sqr * (1 - c), xy * (1 - c) - axis.z * s, xz * (1 - c) + axis.y * s, 0},
      {xy * (1 - c) + axis.z * s, c + y_sqr * (1 - c), yz * (1 - c) - axis.x * s, 0},
      {xz * (1 - c) - axis.y * s, yz * (1 - c) + axis.x * s, c + z_sqr * (1 - c), 0},
      {0, 0, 0, 1}};
}

/** Define a viewing transformation. */
inline mat4x4 look_at(const vec3& eye, const vec3& target, const vec3& up)
{
    vec3 forward{(target - eye).normalized()};
    vec3 side{forward.cross_product(up).normalized()};
    vec3 new_up{side.cross_product(forward)};

    return mat4x4{
             {side, 0.0f},
             {new_up, 0.0f},
             {-forward, 0.0f},
             {ml::vec3::zero(), 1.0f}}
           * translation(-eye.x, -eye.y, -eye.z);
}

} /* namespace matrices */

} /* namespace ml */
