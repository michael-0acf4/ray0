#include "ray0.hpp"

constexpr float minDepth = 0;
constexpr float maxDepth = 100;
const vec2 fResolution = {42, 42};
float gtime = 0;

float sdTotalScene(vec3 p) {
  vec3 tp = rotateZ(-gtime * 0.25) >>
            (rotateX(-gtime * 0.5) >> (rotateY(-gtime) >> p));

  return sdTorus(tp, 0.67, 0.3);
}

void shader(float &fragColor, const vec2 &fragCoord) {
  const vec2 uv = (fragCoord - 0.5 * fResolution) / fResolution.y;

  const vec3 camera(0., 0., 3.); // right above the screen
  const vec3 camDir = normalize(vec3(uv.x, uv.y, -1.));

  float traveled = rayMarch({minDepth, maxDepth}, camera, camDir, sdTotalScene);
  if (traveled <= maxDepth) {
    const vec3 contact = camera + camDir * traveled;
    const vec3 contactNormal = sceneNormalAt(contact, &sdTotalScene);
    const vec3 lightPos(1., 1., 2.);

    vec3 lightDir = normalize(lightPos - contact);

    fragColor = std::fmin(dot(lightDir, contactNormal), 1.);
  }
}

int main() {
  Engine engine(fResolution);

  while (true) {

    engine.clear();
    engine.update(shader);

    engine.saveCursor();
    engine.render();
    engine.restoreCursor();

    gtime += 0.1f;

    engine.restoreCursor();
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(16ms);
  }

  return 0;
}