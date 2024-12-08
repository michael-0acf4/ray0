#include "ray0.hpp"

const vec2 fResolution = {25, 25};

void shaderUVTest(float &fragColor, const vec2 &fragCoord) {
  const vec2 uv = (fragCoord - 0.5 * fResolution) / fResolution.y;

  if (length(uv) < 0.4) {
    float one = 0.25, two = 0.5, three = 0.75, four = 1;
    if (fragCoord.x > fResolution.x / 2) {
      fragColor = fragCoord.y > fResolution.y / 2 ? one : two;
    } else {
      fragColor = fragCoord.y > fResolution.y / 2 ? three : four;
    }
  }
}

int main() {
  Engine engine(fResolution);
  engine.update(shaderUVTest, 4);
  engine.render("3210 ");

  return 0;
}
