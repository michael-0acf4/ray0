#include "ray0.hpp"

constexpr float width = 130;
constexpr float height = 80;
constexpr float workerThreads = 4;

constexpr float RS = .125;         // singularity radius
constexpr float PS_RAD = 1.5 * RS; // photon sphere radius
constexpr float ACC_RAD = 3. * RS; // accretion disc radius

float gtime = .1;

constexpr float MAX_DEPTH = 100.;
constexpr int MAX_STEPS = 300;
constexpr float DP = 0.05;

const float tiltAngle = std::sin((PI / 3.) * 3.2);
const mat4 RX = rotateX(tiltAngle);
const mat4 RZ = rotateZ(-tiltAngle);

inline float sdAccretionDisc(vec3 p) {
  p = RZ >> (RX >> p);
  const float p1 = sdRoundedCylinder(p, ACC_RAD, ACC_RAD / 6, .001);
  const float p2 = sdSphere(p, ACC_RAD);
  return sdSmoothSubtraction(p2, p1, .5);
}

// Rough approximation of how light bends
inline float interpSpaceDistortion(float szwRad, float distSingularity) {
  const float distortionFactor = 2.79;
  const float distRatio = szwRad / distSingularity;
  return pow(distRatio, distortionFactor);
}

vec3 bendLightDirection(vec3 blPos, vec3 rayPos, vec3 rayDir) {
  const vec3 u = normalize(rayDir);

  // Points at the singularity i.e. center of the black hole
  // (u, v) angle is the maximum deviation angle starting from the current
  // unchanged light direction
  const vec3 v = normalize(blPos - rayPos);

  // In this setup we use v directly
  // let's bend u in such a way that it follows v
  // also let's consider how close it is to bend it properly
  float distHowClose = length(blPos - rayPos);
  float lerpVal = interpSpaceDistortion(RS, distHowClose);

  return normalize(lerp3(u, v, lerpVal));
}

inline float normalizedNoiseTexture(float x, float y) {
  // Noise-ish texture... a normalized chess board
  const int sx = (int)(x * 10.f);
  const int sy = (int)(y * 30.f);
  return (sx + sy) % 2 == 0 ? 0.3 : 1.;
}

inline float gridTexture(float x, float y) {
  // Note: Coord is uv normalized!
  constexpr float scale = 3.;
  const float dp = gtime / 5.;
  const float gx = fixed_fmod((x + dp) * scale, .5f);
  const float gy = fixed_fmod(y * scale, .5f);
  // dot
  // return min(1.0, step (gx + gy, .1));

  // row col
  return std::min(1.0f, step(gx, 0.1f) + step(gy, 0.1f));
}

void blackholeShader(float &fragColor, const vec2 &fragCoord) {
  const vec2 uv((fragCoord.x - 0.5 * width) / height,
                (fragCoord.y - 0.5 * height) / height);

  const vec3 camera{0., 0., 2.}; // right above the screen
  const vec3 camDir = normalize({uv.x, uv.y, -1.});

  float distTraveled = 0.05;

  const vec3 blPos(0., 0., 0.);
  vec3 rayPos = camera;
  vec3 rayDir = camDir;

  for (int i = 0; i < MAX_STEPS; i++) {
    rayDir = bendLightDirection(blPos, rayPos, rayDir); // !
    rayPos = rayPos + rayDir * DP;

    float d = sdAccretionDisc(rayPos);
    distTraveled += d;

    if (d < EPSILON || d > MAX_DEPTH)
      break;
  }

  // Base color
  float diffuse = 0.;

  // Right after the photon sphere
  if (length(vec3(uv.x, uv.y, 0.)) < PS_RAD)
    diffuse = 0.;

  // Construct the accretion disk and the photon sphere
  if (distTraveled < MAX_DEPTH) {
    const vec3 delta = rayPos - blPos;
    // Make it glow more as it approaches the center
    // and diminish it with distance
    float glow = .3 / std::pow(length(delta), 2.);
    glow = clamp(glow, 0., 1.); // remove artifacts

    // Rotation effect
    // The noise-ish texture is normalized and have deterministic values
    // rotate the uv coordinate
    constexpr float rotVel = 3.;
    const float s = std::sin(gtime * rotVel), c = std::cos(gtime * rotVel);

    // Rotated local surface
    auto rot = mat2(c, -s, s, c) * rayPos.yz();

    diffuse += glow * normalizedNoiseTexture(rot.x, rot.y);
  }

  // Background space deformation
  if (length(vec3(uv.x, uv.y, 0.)) >= PS_RAD) {
    float backCol = gridTexture(rayPos.x / 5., rayPos.y / 5.);
    diffuse += clamp(backCol, 0., .2);
  }

  fragColor = diffuse;
}

int main() {
  Engine engine(width, height);

  while (true) {

    engine.clear();
    engine.update(blackholeShader, workerThreads);

    engine.saveCursor();
    engine.render();
    engine.restoreCursor();

    gtime += .1f;

    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(16ms);
  }
  return 0;
}