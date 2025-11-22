#pragma once

#include <cmath>
#include <cstdio>
#include <functional>

#define PI 3.1415
#define EPSILON 0.00001f
#define BIG_EPSILON 0.01f

struct vec2 {
  float x, y;

  vec2(float x) : x(x), y(x) {}
  vec2(float x, float y) : x(x), y(y) {}
  vec2() : x(0), y(0) {}
  void show() const { std::printf("vec2\n%f %f\n", x, y); }
  float avg() const { return (x + y) / 2; }

  // Swizzling
  vec2 xy() const { return *this; }
  vec2 yx() const { return vec2(y, x); }
};

struct vec3 {
  float x, y, z;

  vec3(float x) : x(x), y(x), z(x) {}
  vec3(float x, float y, float z) : x(x), y(y), z(z) {}
  vec3() : x(0), y(0), z(0) {}

  void show() const { std::printf("vec3\n%f %f %f\n", x, y, z); }

  float avg() const { return (x + y + z) / 3; }

  // Swizzling
  vec2 xy() const { return vec2(x, y); }
  vec2 yz() const { return vec2(y, z); }
  vec2 xz() const { return vec2(x, z); }

  vec3 xyz() const { return *this; }
  vec3 yzx() const { return vec3(y, z, x); }
  vec3 zxy() const { return vec3(z, x, y); }
};

struct mat2 {
  float m00, m01;
  float m10, m11;

  mat2() : m00(0), m01(0), m10(0), m11(0) {}

  mat2(float v) : m00(v), m01(v), m10(v), m11(v) {}

  mat2(float m00, float m01, float m10, float m11)
      : m00(m00), m01(m01), m10(m10), m11(m11) {}

  float *operator[](int r) { return reinterpret_cast<float *>(this) + r * 2; }
  float &operator()(int i) { return (*this)[i / 2][i % 2]; }

  void show() const { std::printf("mat2\n%f %f\n%f %f\n", m00, m01, m10, m11); }
};

struct mat3 {
  float m00, m01, m02;
  float m10, m11, m12;
  float m20, m21, m22;

  mat3()
      : m00(0), m01(0), m02(0), m10(0), m11(0), m12(0), m20(0), m21(0), m22(0) {
  }

  mat3(float v)
      : m00(v), m01(v), m02(v), m10(v), m11(v), m12(v), m20(v), m21(v), m22(v) {
  }

  mat3(float m00, float m01, float m02, float m10, float m11, float m12,
       float m20, float m21, float m22)
      : m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12), m20(m20),
        m21(m21), m22(m22) {}

  float *operator[](int r) { return reinterpret_cast<float *>(this) + r * 3; }
  float &operator()(int i) { return (*this)[i / 3][i % 3]; }

  void show() const {
    std::printf("mat3\n%f %f %f\n%f %f %f\n%f %f %f\n", m00, m01, m02, m10, m11,
                m12, m20, m21, m22);
  }
};

struct mat4 {
  float m00, m01, m02, m03;
  float m10, m11, m12, m13;
  float m20, m21, m22, m23;
  float m30, m31, m32, m33;

  mat4()
      : m00(0), m01(0), m02(0), m03(0), m10(0), m11(0), m12(0), m13(0), m20(0),
        m21(0), m22(0), m23(0), m30(0), m31(0), m32(0), m33(0) {}

  mat4(float m00, float m01, float m02, float m03, float m10, float m11,
       float m12, float m13, float m20, float m21, float m22, float m23,
       float m30, float m31, float m32, float m33)
      : m00(m00), m01(m01), m02(m02), m03(m03), m10(m10), m11(m11), m12(m12),
        m13(m13), m20(m20), m21(m21), m22(m22), m23(m23), m30(m30), m31(m31),
        m32(m32), m33(m33) {}

  mat4(float v)
      : m00(v), m01(v), m02(v), m03(v), m10(v), m11(v), m12(v), m13(v), m20(v),
        m21(v), m22(v), m23(v), m30(v), m31(v), m32(v), m33(v) {}

  float *operator[](int r) { return reinterpret_cast<float *>(this) + r * 4; }
  float &operator()(int i) { return (*this)[i / 4][i % 4]; }

  void show() const {
    std::printf("mat4\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
                m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30,
                m31, m32, m33);
  }
};

vec2 operator+(const vec2 &a, const vec2 &b);
vec3 operator+(const vec3 &a, const vec3 &b);
mat2 operator+(const mat2 &a, const mat2 &b);
mat3 operator+(const mat3 &a, const mat3 &b);
mat4 operator+(const mat4 &a, const mat4 &b);

template <class T> T &operator+=(T &a, const T &b) {
  a = a + b;
  return a;
}

template <class T> T &operator-=(T &a, const T &b) {
  a = a - b;
  return a;
}

vec2 operator-(const vec2 &a, const vec2 &b);
vec3 operator-(const vec3 &a, const vec3 &b);
mat2 operator-(const mat2 &a, const mat2 &b);
mat3 operator-(const mat3 &a, const mat3 &b);
mat4 operator-(const mat4 &a, const mat4 &b);

vec2 operator*(const vec2 &a, const vec2 &b);
vec3 operator*(const vec3 &a, const vec3 &b);
vec2 operator*(const vec2 &a, float scalar);
vec2 operator*(float scalar, const vec2 &a);
vec3 operator*(const vec3 &a, float scalar);
vec3 operator*(float scalar, const vec3 &a);
mat2 operator*(const mat2 &a, float scalar);
mat2 operator*(float scalar, const mat2 &a);
mat2 operator*(const mat2 &a, const mat2 &b);
mat3 operator*(const mat3 &a, const mat3 &b);
mat3 operator*(const mat3 &a, float scalar);
mat3 operator*(float scalar, const mat3 &a);
mat4 operator*(const mat4 &a, const mat4 &b);
mat4 operator*(const mat4 &a, float scalar);
mat4 operator*(float scalar, const mat4 &a);

vec2 operator*(const mat2 &m, const vec2 &v);
vec3 operator*(const mat3 &m, const vec3 &v);
vec3 operator*(const mat4 &m, const vec3 &v);

float dot(const vec3 &a, const vec3 &b);
float dot(const vec2 &a, const vec2 &b);

vec2 operator/(const vec2 &a, const vec2 &b);
vec2 operator/(const vec2 &a, float scalar);
vec3 operator/(const vec3 &a, const vec3 &b);
vec3 operator/(const vec3 &a, float scalar);

// Multiply and re-scale (homogenous coordinates)
vec3 operator>>(const mat4 &m, const vec3 &v);

float length(vec2 a);
float length(vec3 a);

vec2 normalize(vec2 a);
vec3 normalize(vec3 a);

vec2 v_max(vec2 a, vec2 b);
vec3 v_max(vec3 a, vec3 b);
vec2 v_min(vec2 a, vec2 b);
vec3 v_min(vec3 a, vec3 b);
vec2 v_sin(const vec2 &a);
vec3 v_sin(const vec3 &a);
vec2 v_cos(const vec2 &a);
vec3 v_cos(const vec3 &a);
vec2 v_tan(const vec2 &a);
vec3 v_tan(const vec3 &a);
vec3 v_tanh(const vec3 &a);
vec3 v_tanh(const vec3 &a);
vec2 v_exp(const vec2 &a);
vec3 v_exp(const vec3 &a);
vec2 v_log(const vec2 &a);
vec3 v_log(const vec3 &a);

mat4 rotateY(float t);
mat4 rotateZ(float t);
mat4 rotateX(float t);

float sdSphere(vec3 p, float radius);
float sdBox(vec3 p, vec3 b);
float sdTorus(vec3 p, float tx, float ty);
float sdUnion(float distA, float distB);
float sdInter(float distA, float distB);
float sdDiff(float distA, float distB);
float sdRoundedCylinder(vec3 p, float ra, float rb, float h);
float sdSmoothSubtraction(float d1, float d2, float k);

vec3 lerp3(vec3 colorone, vec3 colortwo, float value);
float lerp(float colorone, float colortwo, float value);
float clamp(float value, float min, float max);
vec3 reflect(const vec3 &incident, const vec3 &normal);
float step(float a, float edge);
float fixed_fmod(float a, float n);

vec3 sceneNormalAt(vec3 p, const std::function<float(const vec3 &)> &dist);
float rayMarch(std::pair<float, float> depth, const vec3 &camera,
               const vec3 &camDir,
               const std::function<float(const vec3 &)> &distanceFn);
