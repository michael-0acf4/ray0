#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
#include <iostream>

#include <vector>

#include "engine.hpp"
#include "geom.hpp"

constexpr float minDepth = 0;
constexpr float maxDepth = 100;
constexpr float width = 120;
constexpr float height = 80;
float gtime = 0;

// core
float sdTotalScene(vec3 p) {
  vec3 transf_p = applyTransf(rotateY(-gtime), p);
  transf_p = applyTransf(rotateX(-gtime), transf_p);
  transf_p = applyTransf(rotateZ(-gtime), transf_p);
  // return sdTorus(transf_p, 1, 0.5);
  return sdDiff(sdUnion(sdSphere(transf_p, 0.25),
                        sdDiff(sdBox(transf_p, vec3(0.8, 0.8, 0.8)),
                               sdSphere(transf_p, 1.))),
                sdTorus(transf_p, 1, 0.5));
}

// Unlike the blackhole
// there is no need for a thread per fragment
// in this particular implementation of ray marching
inline float rayMarch(vec3 camera, vec3 cam_dir) {
  float distTraveled = minDepth;
  int steps = 200;
  while (steps > 0) {
    vec3 currPos = add(camera, scaleReal(cam_dir, distTraveled));
    float d = sdTotalScene(currPos);
    distTraveled += d;

    if (d < EPSILON || distTraveled > maxDepth)
      break;

    steps--;
  }

  return distTraveled;
}

void shader(float &fragColor, const vec2 &fragCoord) {
  const vec2 uv((fragCoord.x - 0.5 * width) / height,
                (fragCoord.y - 0.5 * height) / height);

  const vec3 camera(0., 0., 3.); // right above the screen
  const vec3 camDir = normalize({uv.x, uv.y, -1.});

  float traveled = rayMarch(camera, camDir);
  if (traveled <= maxDepth) {
    const vec3 contact = add(camera, scaleReal(camDir, traveled));
    const vec3 contactNormal = sceneNormalAt(contact, &sdTotalScene);
    const vec3 lightPos(1., 1., 2.);

    vec3 lightDir = normalize(sub(lightPos, contact));

    fragColor = std::fmin(dot(lightDir, contactNormal), 1.);
  }
}

int main() {
  Engine engine(width, height);

  while (true) {
    engine.saveCursor();

    engine.clear();
    engine.update(shader);
    engine.render();
    gtime += .1f;

    engine.restoreCursor();

    // attempt 60fps
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(16ms);
  }
  return 0;
}