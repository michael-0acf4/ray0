#include "ray0.hpp"

const vec2 iResolution = {25, 25};

void shaderUVTest(float &fragColor, const vec2 &fragCoord) {
  const vec2 uv = (fragCoord - 0.5 * iResolution) / iResolution.y;

  if (sqrt(uv.x * uv.x + uv.y * uv.y) < .4) {
    float one = 0.25, two = 0.5, three = 0.75, four = 1.;
    if (fragCoord.x > iResolution.x / 2) {
      fragColor = fragCoord.y > iResolution.y / 2 ? one : two;
    } else {
      fragColor = fragCoord.y > iResolution.y / 2 ? three : four;
    }
  }
}

int main() {
  Engine engine(iResolution);
  engine.update(shaderUVTest, 4);
  engine.render("3210 ");

  return 0;
}
