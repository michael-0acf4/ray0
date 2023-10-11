#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>

#include "utils.h"

// FIXME: per sub-region should be doable
// use thread per fragment
#define USE_THREAD 1

constexpr float width = 130;
constexpr float height = 80;

// black hole configuration
constexpr float RS = .125;         // singularity radius
constexpr float PS_RAD = 1.5 * RS; // photon sphere radius
constexpr float ACC_RAD = 3. * RS; // accretion disc radius

float gtime = .1;

// ray marching configuration
constexpr float MAX_DEPTH = 100.;
constexpr int MAX_STEPS = 300;
constexpr float DP = 0.05;

// core
inline float sdAccretionDisc(vec3 p) {
  p = applyTransf(rotateX(-std::sin((PI / 3.) * 3.2)), p);
  p = applyTransf(rotateZ(-std::sin((PI / 3.) * 3.2)), p);
  const float p1 = sdRoundedCylinder(p, ACC_RAD, ACC_RAD / 6, .001);
  const float p2 = sdSphere(p, ACC_RAD);
  return sdSmoothSubtraction(p2, p1, .5);
}

// rough approximation of how light bends
inline float interpSpaceDistortion(float sz_rad, float dist_singularity) {
  const float distortion_factor = 2.79;
  const float dist_ratio = sz_rad / dist_singularity;
  return pow(dist_ratio, distortion_factor);
}

vec3 bendLightDirection(vec3 bl_pos, vec3 ray_pos, vec3 ray_dir) {
  // points at the current direction
  const vec3 u = normalize(ray_dir);
  // points at the singularity i.e. center of the black hole
  // (u, v) angle is the maximum deviation angle starting from the current
  // unchanged light direction
  const vec3 v = normalize(sub(bl_pos, ray_pos));

  // in this setup we use v directly
  // let's bend u in such a way that it follows v
  // also let's consider how close it is to bend it properly
  float dist_how_close = length(sub(bl_pos, ray_pos));
  float lerp_val = interpSpaceDistortion(RS, dist_how_close);

  return normalize(lerp3(u, v, lerp_val));
}

inline float normalizedNoiseTexture(float x, float y) {
  // noise-ish texture... a normalized chess board
  const int sx = (int)(x * 10.f);
  const int sy = (int)(y * 30.f);
  if ((sx + sy) % 2 == 0)
    return 0.3;
  return 1.;
}

// coord is uv normalized !
inline float gridTexture(float coord_x, float coord_y) {
  constexpr float scale = 3.;
  const float dp = gtime / 5.;
  const float gx = fixed_fmod((coord_x + dp) * scale, .5f);
  const float gy = fixed_fmod(coord_y * scale, .5f);
  // dot
  // return min(1.0, step (gx + gy, .1));

  // row col
  return std::min(1.0f, step(gx, 0.1f) + step(gy, 0.1f));
}

void fragmentHandler(float x, float y, t_screen *screen) {
  // we define a direction for each pixel
  const vec3 camera{0., 0., 2.}; // the camera must be above
  const vec3 cam_dir = normalize({x, y, -1.});

  // ray marching
  float dtravel = 0.05;

  const vec3 bl_pos(0., 0., 0.);
  vec3 ray_pos = camera;
  vec3 ray_dir = cam_dir;

  for (int i = 0; i < MAX_STEPS; i++) {
    ray_dir = bendLightDirection(bl_pos, ray_pos, ray_dir);
    ray_pos = add(ray_pos, scaleReal(ray_dir, DP));

    float d = sdAccretionDisc(ray_pos);
    dtravel += d;

    if (d < EPSILON)
      break;
    if (d > MAX_DEPTH)
      break;
  }

  // base color
  float diffuse = 0.;

  // just after the photon sphere
  if (length(vec3(x, y, 0.)) < PS_RAD)
    diffuse = 0.;

  // construct the accretion disk and the photon sphere
  if (dtravel < MAX_DEPTH) {
    const vec3 delta = sub(ray_pos, bl_pos);
    // make it glow more as it approaches the center
    // create glow and diminish it with distance
    float glow = .3 / std::pow(length(delta), 2.);
    glow = clamp(glow, 0., 1.); // remove artifacts

    // rotation effect
    // the noise-ish texture is normalized and have deterministic values
    // rotate the uv coordinate
    constexpr float rot_vel = 3.;
    const float s = std::sin(gtime * rot_vel), c = std::cos(gtime * rot_vel);
    const float tuv_x = ray_pos.y;
    const float tuv_y = ray_pos.z;
    const float rot_x = c * tuv_x - s * tuv_y;
    const float rot_y = s * tuv_x + c * tuv_y;

    float tex_color = normalizedNoiseTexture(rot_x, rot_y);

    // total color
    diffuse += glow * tex_color;
  }

  // background space deformation
  if (length(vec3(x, y, 0.)) >= PS_RAD) {
    float back_col = gridTexture(ray_pos.x / 5., ray_pos.y / 5.);
    diffuse += clamp(back_col, 0., .2);
  }

  // end ray marching
  const char pixel =
      screen->computeColorGivenDiffuseLight(diffuse, COLOR_STRONG);

  // texture coords ---> screen coords
  // -0.5  0.5	 ---> -width/2 width/2
  // -0.5  0.5	 ---> -height/2 height/2
  const int screen_x = (int)((x + 0.5) * width),
            screen_y = (int)((y + 0.5) * height);
  screen->put(screen_y, screen_x, pixel);
}

void computeScreenBufferConcurrent(t_screen *screen) {
  // motivation : the bigger the screensize, the more the steps
  // we can also assign an arbitrary step
  const float dp = 1.f / std::max(height, width);

  // normalized coordinates centered at (0., 0.)
  constexpr float sy = -0.5, ey = 0.5;
  constexpr float sx = -0.5, ex = 0.5;
  const float ratio = width / height;

  // iterate through the texture coordinate

  std::vector<std::thread> frag_workers;
  for (float y = sy; y < ey; y += dp) {
    for (float x = sx; x < ex; x += dp) {
      // FIXME: spread work accross max threads (thread spawn has significant
      // overhead)
#if USE_THREAD == 1
      std::thread worker(fragmentHandler, x, y * ratio, screen);
      frag_workers.push_back(std::move(worker));
#else
      fragmentHandler(x * ratio, y * ratio, screen);
#endif
    }
  }

#if USE_THREAD == 1
  // for each subtask, sync with the main thread i.e. wait for all threads to
  // terminate
  for (auto &worker : frag_workers)
    worker.join();
#endif
}

int main() {
  t_screen screen(width, height);
  screen.clear();

#if USE_THREAD == 1
  std::cout << "/!\\ Preparing ~" << pow(std::max(height, width), 2.f)
            << " parallel tasks\n";
#endif

  while (true) {
    screen.saveCursor();
    computeScreenBufferConcurrent(&screen);
    screen.show();
    screen.restoreCursor();
    gtime += .2;

    // attempt 60fps
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(16ms);
  }
  return 0;
}