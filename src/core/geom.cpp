#include "geom.hpp"

// Add
vec2 operator+(const vec2 &a, const vec2 &b) {
  return vec2(a.x + b.x, a.y + b.y);
}

vec3 operator+(const vec3 &a, const vec3 &b) {
  return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

mat2 operator+(const mat2 &a, const mat2 &b) {
  return mat2(a.m00 + b.m00, a.m01 + b.m01, a.m10 + b.m10, a.m11 + b.m11);
}

mat3 operator+(const mat3 &a, const mat3 &b) {
  return mat3(a.m00 + b.m00, a.m01 + b.m01, a.m02 + b.m02, a.m10 + b.m10,
              a.m11 + b.m11, a.m12 + b.m12, a.m20 + b.m20, a.m21 + b.m21,
              a.m22 + b.m22);
}

mat4 operator+(const mat4 &a, const mat4 &b) {
  return mat4(a.m00 + b.m00, a.m01 + b.m01, a.m02 + b.m02, a.m03 + b.m03,
              a.m10 + b.m10, a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13,
              a.m20 + b.m20, a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23,
              a.m30 + b.m30, a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33);
}

// Sub

mat2 operator-(const mat2 &a, const mat2 &b) {
  return mat2(a.m00 - b.m00, a.m01 - b.m01, a.m10 - b.m10, a.m11 - b.m11);
}

mat3 operator-(const mat3 &a, const mat3 &b) {
  return mat3(a.m00 - b.m00, a.m01 - b.m01, a.m02 - b.m02, a.m10 - b.m10,
              a.m11 - b.m11, a.m12 - b.m12, a.m20 - b.m20, a.m21 - b.m21,
              a.m22 - b.m22);
}

mat4 operator-(const mat4 &a, const mat4 &b) {
  return mat4(a.m00 - b.m00, a.m01 - b.m01, a.m02 - b.m02, a.m03 - b.m03,
              a.m10 - b.m10, a.m11 - b.m11, a.m12 - b.m12, a.m13 - b.m13,
              a.m20 - b.m20, a.m21 - b.m21, a.m22 - b.m22, a.m23 - b.m23,
              a.m30 - b.m30, a.m31 - b.m31, a.m32 - b.m32, a.m33 - b.m33);
}

vec2 operator-(const vec2 &a, const vec2 &b) {
  return vec2(a.x - b.x, a.y - b.y);
}

vec3 operator-(const vec3 &a, const vec3 &b) {
  return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

// Mult

vec2 operator*(const vec2 &a, float scalar) {
  return vec2(a.x * scalar, a.y * scalar);
}

vec2 operator*(float scalar, const vec2 &a) { return a * scalar; }

vec3 operator*(const vec3 &a, float scalar) {
  return vec3(a.x * scalar, a.y * scalar, a.z * scalar);
}

vec3 operator*(float scalar, const vec3 &a) { return a * scalar; }

mat3 operator*(const mat3 &a, const mat3 &b) {
  return mat3(a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20,
              a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21,
              a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22,

              a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20,
              a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21,
              a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22,

              a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20,
              a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21,
              a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22);
}

mat3 operator*(const mat3 &a, float scalar) {
  return mat3(a.m00 * scalar, a.m01 * scalar, a.m02 * scalar, a.m10 * scalar,
              a.m11 * scalar, a.m12 * scalar, a.m20 * scalar, a.m21 * scalar,
              a.m22 * scalar);
}

mat3 operator*(float scalar, const mat3 &a) { return a * scalar; }

mat2 operator*(const mat2 &a, const mat2 &b) {
  return mat2(a.m00 * b.m00 + a.m01 * b.m10, a.m00 * b.m01 + a.m01 * b.m11,
              a.m10 * b.m00 + a.m11 * b.m10, a.m10 * b.m01 + a.m11 * b.m11);
}

mat2 operator*(const mat2 &a, float scalar) {
  return mat2(a.m00 * scalar, a.m01 * scalar, a.m10 * scalar, a.m11 * scalar);
}

mat2 operator*(float scalar, const mat2 &a) { return a * scalar; }

mat4 operator*(const mat4 &a, const mat4 &b) {
  return mat4(a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20 + a.m03 * b.m30,
              a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21 + a.m03 * b.m31,
              a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22 + a.m03 * b.m32,
              a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03 * b.m33,

              a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20 + a.m13 * b.m30,
              a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31,
              a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32,
              a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33,

              a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20 + a.m23 * b.m30,
              a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31,
              a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32,
              a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33,

              a.m30 * b.m00 + a.m31 * b.m10 + a.m32 * b.m20 + a.m33 * b.m30,
              a.m30 * b.m01 + a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31,
              a.m30 * b.m02 + a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32,
              a.m30 * b.m03 + a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33);
}

mat4 operator*(const mat4 &a, float scalar) {
  return mat4(a.m00 * scalar, a.m01 * scalar, a.m02 * scalar, a.m03 * scalar,
              a.m10 * scalar, a.m11 * scalar, a.m12 * scalar, a.m13 * scalar,
              a.m20 * scalar, a.m21 * scalar, a.m22 * scalar, a.m23 * scalar,
              a.m30 * scalar, a.m31 * scalar, a.m32 * scalar, a.m33 * scalar);
}

mat4 operator*(float scalar, const mat4 &a) { return a * scalar; }

// Transformations

vec2 operator*(const mat2 &m, const vec2 &v) {
  return vec2(m.m00 * v.x + m.m01 * v.y, m.m10 * v.x + m.m11 * v.y);
}

vec2 operator*(const mat3 &m, const vec3 &v) {
  return vec2(m.m00 * v.x + m.m01 * v.y + m.m02 * v.z,
              m.m10 * v.x + m.m11 * v.y + m.m12 * v.z);
}

vec3 operator*(const mat4 &m, const vec3 &v) {
  return vec3(m.m00 * v.x + m.m01 * v.y + m.m02 * v.z,
              m.m10 * v.x + m.m11 * v.y + m.m12 * v.z,
              m.m20 * v.x + m.m21 * v.y + m.m22 * v.z);
}

float dot(const vec2 &a, const vec2 &b) { return a.x * b.x + a.y * b.y; }

float dot(const vec3 &a, const vec3 &b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec2 operator/(const vec2 &a, const vec2 &b) {
  return vec2(a.x / b.x, a.y / b.y);
}

vec2 operator/(const vec2 &a, float scalar) {
  return vec2(a.x / scalar, a.y / scalar);
}

vec3 operator/(const vec3 &a, const vec3 &b) {
  return vec3(a.x / b.x, a.y / b.y, a.z / b.z);
}

vec3 operator/(const vec3 &a, float scalar) {
  return vec3(a.x / scalar, a.y / scalar, a.z / scalar);
}

vec3 operator>>(const mat4 &m, const vec3 &v) {
  const float x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * 1.0f;
  const float y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * 1.0f;
  const float z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * 1.0f;
  // Scaler
  const float w = m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * 1.0f;

  if (std::fabs(w) > EPSILON) {
    return vec3(x / w, y / w, z / w);
  }

  return vec3(x, y, z);
}

// Utils

float length(vec3 a) { return (float)sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }

vec3 v_max(vec3 a, vec3 b) {
  return vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

vec3 v_min(vec3 a, vec3 b) {
  return vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

vec3 normalize(vec3 a) {
  const float L = length(a);
  if (L <= EPSILON)
    return {0., 0., 0.};
  return vec3(a.x / L, a.y / L, a.z / L);
}

// Transformations

mat4 rotateY(float t) {
  const float ct = std::cos(t);
  const float st = std::sin(t);
  return mat4(ct, 0, -st, 0, 0, 1, 0, 0, st, 0, ct, 0, 0, 0, 0, 1);
}

mat4 rotateZ(float t) {
  const float ct = std::cos(t);
  const float st = std::sin(t);
  return mat4(ct, -st, 0, 0, st, ct, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

mat4 rotateX(float t) {
  const float ct = std::cos(t);
  const float st = std::sin(t);
  return mat4(1, 0, 0, 0, 0, ct, st, 0, 0, -st, ct, 0, 0, 0, 0, 1);
}

// Shapes

float sdSphere(vec3 p, float radius) { return length(p) - radius; }

float sdBox(vec3 p, vec3 b) {
  return length(vec3( // vmax
      (float)std::max((float)(fabs(p.x) - b.x), 0.f),
      (float)std::max((float)(fabs(p.y) - b.y), 0.f),
      (float)std::max((float)(fabs(p.z) - b.z), 0.f)));
}

float sdTorus(vec3 p, float tx, float ty) {
  return -ty + sqrt(p.y * p.y + std::pow(sqrt(p.x * p.x + p.z * p.z) - tx, 2));
}

float sdUnion(float distA, float distB) { return std::min(distA, distB); }

float sdInter(float distA, float distB) { return std::max(distA, distB); }

float sdDiff(float distA, float distB) { return std::max(distA, -distB); }

vec3 lerp3(vec3 colorone, vec3 colortwo, float value) {
  return colorone + (colortwo - colorone) * value;
}

float lerp(float colorone, float colortwo, float value) {
  return lerp3(vec3(colorone, 0., 0.), vec3(colortwo, 0., 0.), value).x;
}

float clamp(float value, float min, float max) {
  return std::max(min, std::min(max, value));
}

float sdRoundedCylinder(vec3 p, float ra, float rb, float h) {
  const vec3 d =
      vec3(length({p.x, 0., p.z}) - 2.0 * ra + rb, fabs(p.y) - h, 0.);
  return std::min(std::max(d.x, d.y), 0.f) + length(v_max(d, {0., 0., 0.})) -
         rb;
}

float sdSmoothSubtraction(float d1, float d2, float k) {
  const float h = clamp(0.5 - 0.5 * (d2 + d1) / k, 0.0, 1.0);
  return lerp(d2, -d1, h) + k * h * (1.0 - h);
}

float step(float a, float edge) { return a < edge ? 1. : 0.; }

float fixed_fmod(float a, float n) { return std::fmod(std::fmod(a, n) + n, n); }

// The gradient at the contact point is orthogonal to surface
// Approximating it is enough in practice..
vec3 sceneNormalAt(vec3 p, const std::function<float(const vec3 &)> &dist) {
  const float x =
      dist({p.x + EPSILON, p.y, p.z}) - dist({p.x - EPSILON, p.y, p.z});
  const float y =
      dist({p.x, p.y + EPSILON, p.z}) - dist({p.x, p.y - EPSILON, p.z});
  const float z =
      dist({p.x, p.y, p.z + EPSILON}) - dist({p.x, p.y, p.z - EPSILON});
  return normalize(vec3(x, y, z));
}

float rayMarch(std::pair<float, float> depth, const vec3 &camera,
               const vec3 &camDir,
               const std::function<float(const vec3 &)> &distanceFn) {
  float distTraveled = depth.first;
  int steps = 200;
  while (steps > 0) {
    vec3 currPos = camera + camDir * distTraveled;
    float d = distanceFn(currPos);
    distTraveled += d;

    if (d < EPSILON || distTraveled > depth.second)
      break;

    steps--;
  }

  return distTraveled;
}
