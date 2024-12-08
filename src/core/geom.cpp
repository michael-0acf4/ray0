#include "geom.hpp"

float length(vec3 a) { return (float)sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }

// vec3

vec3 scaleReal(vec3 a, float k) { return {a.x * k, a.y * k, a.z * k}; }

vec3 add(vec3 a, vec3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }

vec3 sub(vec3 a, vec3 b) { return add(a, scaleReal(b, -1.)); }

float dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

vec3 v_max(vec3 a, vec3 b) {
  return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
}

vec3 v_min(vec3 a, vec3 b) {
  return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
}

vec3 normalize(vec3 a) {
  const float L = length(a);
  if (L <= EPSILON)
    return {0., 0., 0.};
  return {a.x / L, a.y / L, a.z / L};
}

vec3 applyTransf(mat4 m, vec3 v) {
  const float x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * 1.;
  const float y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * 1.;
  const float z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * 1.;
  const float w = m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * 1.;
  vec3 res{x, y, z};
  if (fabs(w) > EPSILON)
    return scaleReal(res, 1. / w);
  return res;
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
  return add(colorone, scaleReal(sub(colortwo, colorone), value));
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
  return normalize({x, y, z});
}

float rayMarch(std::pair<float, float> depth, const vec3 &camera,
               const vec3 &camDir,
               const std::function<float(const vec3 &)> &distanceFn) {
  float distTraveled = depth.first;
  int steps = 200;
  while (steps > 0) {
    vec3 currPos = add(camera, scaleReal(camDir, distTraveled));
    float d = distanceFn(currPos);
    distTraveled += d;

    if (d < EPSILON || distTraveled > depth.second)
      break;

    steps--;
  }

  return distTraveled;
}
