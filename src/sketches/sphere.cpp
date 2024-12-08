#include "engine.hpp"
#include "geom.hpp"

constexpr float minDepth = 0.;
constexpr float maxDepth = 100.;
constexpr float radius = 1.;
constexpr float width = 50;
constexpr float height = 35;

// Sphere at (0, 0, 0)
inline float sdTotalScene(vec3 p) { return length(p) - radius; }

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
  engine.update(shader);
  engine.render("******+++===----:::......  ");

  return 0;
}
