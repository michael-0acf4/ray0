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
constexpr float width = 42;
constexpr float height = 42;
float gtime = 0;

float sdTotalScene(vec3 p) {
  vec3 transfP = applyTransf(rotateY(-gtime), p);
  transfP = applyTransf(rotateX(-gtime * 0.5), transfP);
  transfP = applyTransf(rotateZ(-gtime * 0.25), transfP);

  return sdTorus(transfP, 0.67, 0.3);
}

void shader(float &fragColor, const vec2 &fragCoord) {
  const vec2 uv((fragCoord.x - 0.5 * width) / height,
                (fragCoord.y - 0.5 * height) / height);

  const vec3 camera(0., 0., 3.); // right above the screen
  const vec3 camDir = normalize({uv.x, uv.y, -1.});

  float traveled = rayMarch({minDepth, maxDepth}, camera, camDir, sdTotalScene);
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

    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(16ms);
  }
  return 0;
}