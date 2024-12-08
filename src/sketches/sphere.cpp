#include "ray0.hpp"

constexpr float minDepth = 0.;
constexpr float maxDepth = 100.;
constexpr float radius = 1.;
float gtime = 1.;
const vec2 fResolution(40, 32);

// Sphere at (0, 0, 0)
inline float sdTotalScene(vec3 p) { return length(p) - radius; }

void shader(float &fragColor, const vec2 &fragCoord) {
  const vec2 uv = (fragCoord - 0.5 * fResolution) / fResolution.y;

  const vec3 camera(0, 0, 3); // right above the screen
  const vec3 camDir = normalize(vec3(uv.x, uv.y, -1));

  float traveled = rayMarch({minDepth, maxDepth}, camera, camDir, sdTotalScene);

  if (traveled <= maxDepth) {
    const vec3 contact = camera + camDir * traveled;
    const vec3 contactNormal = sceneNormalAt(contact, &sdTotalScene);
    const vec3 lightPos(2, 2, 3);
    const vec3 lightDir = normalize(lightPos - contact);
    const vec3 reflectedDir = reflect(lightPos, contactNormal);

    float t = gtime;
    float c = std::cos(t * 0.5), s = std::sin(t);
    mat3 rot = mat3(c, s, 0, -s, c, 0, 0, 0, 1);

    float diffuse = clamp(dot(rot * lightDir, contactNormal), 0, 1);

    // very rough
    float reflContrib = dot(camDir, reflectedDir);

    fragColor = 0.9 * diffuse + 0.1 * reflContrib;
  } else {
    fragColor = 0.1;
  }
}

int main() {
  Engine engine(fResolution);
  engine.update(shader);

  while (true) {

    engine.clear();
    engine.update(shader, 2);

    engine.saveCursor();
    engine.render();
    engine.restoreCursor();

    gtime += .1f;

    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(16ms);
  }
  return 0;
}
