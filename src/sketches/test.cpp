#include "ray0.hpp"

constexpr float width = 25;
constexpr float height = 25;
const vec2 iResolution = {width, height};

void shaderUVTest(float &fragColor, const vec2 &fragCoord) {
  const vec2 uv = (fragCoord - 0.5 * iResolution.xy()) / iResolution.y;

  if (sqrt(uv.x * uv.x + uv.y * uv.y) < .4) {
    float one = 0.25, two = 0.5, three = 0.75, four = 1.;
    if (fragCoord.x > width / 2) {
      fragColor = fragCoord.y > height / 2 ? one : two;
    } else {
      fragColor = fragCoord.y > height / 2 ? three : four;
    }
  }
}

int main() {
  Engine engine(width, height);
  engine.update(shaderUVTest, 4);
  engine.render("3210 ");

  return 0;
}
