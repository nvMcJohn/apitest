#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

struct Vec2 { float x, y; };
struct Vec3 { float x, y, z; };
struct Vec4 { float x, y, z, w; };
struct Matrix { Vec4 x, y, z, w; };

inline Vec3 operator-(Vec3 a, Vec3 b)
{
    Vec3 r = { a.x - b.x, a.y - b.y, a.z - b.z };
    return r;
}

inline Vec3 operator-(Vec3 a)
{
    Vec3 r = { -a.x, -a.y, -a.z };
    return r;
}

inline Vec3 operator+(Vec3 a, Vec3 b)
{
    Vec3 r = { a.x + b.x, a.y + b.y, a.z + b.z };
    return r;
}

inline Vec3 operator*(Vec3 a, float s)
{
    Vec3 r = { a.x * s, a.y * s, a.z * s };
    return r;
}

inline Vec3 operator*(float s, Vec3 a)
{
    return a * s;
}

inline Vec3 operator*(Vec3 a, Vec3 b)
{
    Vec3 r = { a.x * b.x, a.y * b.y, a.z * b.z };
    return r;
}

inline Vec3 operator/(Vec3 a, float s)
{
    Vec3 r = { a.x / s, a.y / s, a.z / s };
    return r;
}

inline Matrix operator*(Matrix a, Matrix b)
{
    Matrix r;

    r.x.x = b.x.x*a.x.x + b.y.x*a.x.y + b.z.x*a.x.z + b.w.x*a.x.w;
    r.x.y = b.x.y*a.x.x + b.y.y*a.x.y + b.z.y*a.x.z + b.w.y*a.x.w;
    r.x.z = b.x.z*a.x.x + b.y.z*a.x.y + b.z.z*a.x.z + b.w.z*a.x.w;
    r.x.w = b.x.w*a.x.x + b.y.w*a.x.y + b.z.w*a.x.z + b.w.w*a.x.w;

    r.y.x = b.x.x*a.y.x + b.y.x*a.y.y + b.z.x*a.y.z + b.w.x*a.y.w;
    r.y.y = b.x.y*a.y.x + b.y.y*a.y.y + b.z.y*a.y.z + b.w.y*a.y.w;
    r.y.z = b.x.z*a.y.x + b.y.z*a.y.y + b.z.z*a.y.z + b.w.z*a.y.w;
    r.y.w = b.x.w*a.y.x + b.y.w*a.y.y + b.z.w*a.y.z + b.w.w*a.y.w;

    r.z.x = b.x.x*a.z.x + b.y.x*a.z.y + b.z.x*a.z.z + b.w.x*a.z.w;
    r.z.y = b.x.y*a.z.x + b.y.y*a.z.y + b.z.y*a.z.z + b.w.y*a.z.w;
    r.z.z = b.x.z*a.z.x + b.y.z*a.z.y + b.z.z*a.z.z + b.w.z*a.z.w;
    r.z.w = b.x.w*a.z.x + b.y.w*a.z.y + b.z.w*a.z.z + b.w.w*a.z.w;

    r.w.x = b.x.x*a.w.x + b.y.x*a.w.y + b.z.x*a.w.z + b.w.x*a.w.w;
    r.w.y = b.x.y*a.w.x + b.y.y*a.w.y + b.z.y*a.w.z + b.w.y*a.w.w;
    r.w.z = b.x.z*a.w.x + b.y.z*a.w.y + b.z.z*a.w.z + b.w.z*a.w.w;
    r.w.w = b.x.w*a.w.x + b.y.w*a.w.y + b.z.w*a.w.z + b.w.w*a.w.w;

    return r;
}

inline float radians(float x)
{
    return x * (float)M_PI / 360.0f;
}

inline float length(Vec3 a)
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

inline float dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 normalize(Vec3 a)
{
    float inv_len = 1.0f / length(a);
    Vec3 r = { a.x * inv_len, a.y * inv_len, a.z * inv_len };
    return r;
}

inline Vec3 cross(Vec3 a, Vec3 b)
{
    Vec3 r = { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
    return r;
}

inline Matrix transpose(Matrix m)
{
    Matrix r = 
    {
        m.x.x, m.y.x, m.z.x, m.w.x,
        m.x.y, m.y.y, m.z.y, m.w.y,
        m.x.z, m.y.z, m.z.z, m.w.z,
        m.x.w, m.y.w, m.z.w, m.w.w
    };

    return r;
}

inline Matrix matrix_identity()
{
    Matrix m =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return m;
}

inline Matrix matrix_rotation_z(float angle)
{
    float s = sinf(angle);
    float c = cosf(angle);

    Matrix r =
    {
        c,   -s,    0.0f, 0.0f,
        s,    c,    0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    return r;
}

inline Matrix matrix_look_at(Vec3 eye, Vec3 at, Vec3 up)
{
    Vec3 z_axis = normalize(eye - at);
    Vec3 x_axis = normalize(cross(up, z_axis));
    Vec3 y_axis = cross(z_axis, x_axis);

    float x_offset = -dot(x_axis, eye);
    float y_offset = -dot(y_axis, eye);
    float z_offset = -dot(z_axis, eye);
    Matrix r = 
    {
        x_axis.x, x_axis.y, x_axis.z, x_offset,
        y_axis.x, y_axis.y, y_axis.z, y_offset,
        z_axis.x, z_axis.y, z_axis.z, z_offset,
        0.0f,     0.0f,     0.0f,     1.0f
    };

    return r;
}

inline Matrix matrix_perspective_rh_gl(float fov_y, float aspect_ratio, float znear, float zfar)
{
    float height = 1.0f / tan(fov_y);
    float width = height / aspect_ratio;

    Matrix m =
    {
        width, 0.0f, 0.0f, 0.0f,
        0.0f, height, 0.0f, 0.0f,
        0.0f, 0.0f, -(zfar + znear) / (zfar - znear), -(2 * zfar * znear) / (zfar - znear),
        0.0f, 0.0f, -1.0f, 0.0f
    };

    return m;
}
