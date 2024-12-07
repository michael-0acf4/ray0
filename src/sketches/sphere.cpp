#include <cmath>
#include <cstring>
#include <iostream>

#include "engine.hpp"
#include "geom.hpp"

constexpr float minDepth = 0.;
constexpr float maxDepth = 100.;
constexpr float radius = 1.;
constexpr float width = 50;
constexpr float height = 35;

// Sphere at (0, 0, 0)
inline float sdTotalScene(vec3 p) { return length(p) - radius; }

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
  engine.update(shader);
  engine.render("******+++===----:::......  ");

  return 0;
}
