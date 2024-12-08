#pragma once

#include "geom.hpp"

#include <cmath>
#include <functional>

#define PI 3.1415
#define EPSILON 0.00001f
#define BIG_EPSILON 0.01f

struct vec2 {
  float x;
  float y;
  vec2(float x, float y) : x(x), y(y) {}
  void show() { printf("vec2 %f %f", x, y); }
};

struct vec3 {
  float x;
  float y;
  float z;
  vec3(float x, float y, float z) : x(x), y(y), z(z) {}
  void show() { printf("vec3 %f %f %f", x, y, z); }
};

// I know it looks ugly
struct mat4 {

  float m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31,
      m32, m33;

  mat4(float m00, float m01, float m02, float m03, float m10, float m11,
       float m12, float m13, float m20, float m21, float m22, float m23,
       float m30, float m31, float m32, float m33)
      : m00(m00), m01(m01), m02(m02), m03(m03), m10(m10), m11(m11), m12(m12),
        m13(m13), m20(m20), m21(m21), m22(m22), m23(m23), m30(m30), m31(m31),
        m32(m32), m33(m33) {}
};

float length(vec3 a);
vec3 scaleReal(vec3 a, float k);
vec3 add(vec3 a, vec3 b);
vec3 sub(vec3 a, vec3 b);
float dot(vec3 a, vec3 b);
vec3 v_max(vec3 a, vec3 b);
vec3 v_min(vec3 a, vec3 b);
vec3 normalize(vec3 a);
vec3 applyTransf(mat4 m, vec3 v);

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
float step(float a, float edge);
float fixed_fmod(float a, float n);

vec3 sceneNormalAt(vec3 p, const std::function<float(const vec3 &)> &dist);
float rayMarch(std::pair<float, float> depth, const vec3 &camera,
               const vec3 &camDir,
               const std::function<float(const vec3 &)> &distanceFn);