#include "ray0.hpp"

constexpr float minDepth = 0;
constexpr float maxDepth = 500;
const vec2 iResolution = {120, 80};
float gtime = 0;

float sdTotalScene(vec3 p) {
  vec3 tp = rotateZ(-gtime) >> (rotateX(-gtime) >> (rotateY(-gtime) >> p));

  return sdDiff(
      sdUnion(sdSphere(tp, 0.25),
              sdDiff(sdBox(tp, vec3(0.8, 0.8, 0.8)), sdSphere(tp, 1.))),
      sdTorus(tp, 1, 0.5));
}

void shader(float &fragColor, const vec2 &fragCoord) {
  const vec2 uv = (fragCoord - 0.5 * iResolution) / iResolution.y;

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
  Engine engine(iResolution);

  while (true) {

    engine.clear();

    engine.saveCursor();
    engine.update(shader, 2);
    engine.restoreCursor();

    engine.render();
    gtime += .1f;

    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(16ms);
  }
  return 0;
}