#include "ray0.hpp"

constexpr float minDepth = 0.;
constexpr float maxDepth = 100.;
constexpr float radius = 1.;
const vec2 iResolution(50, 35);

// Sphere at (0, 0, 0)
inline float sdTotalScene(vec3 p) { return length(p) - radius; }

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
  engine.update(shader);
  engine.render("******+++===----:::......  ");

  return 0;
}
