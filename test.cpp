#include <cmath>
#include <cstring>
#include <iostream>

#include "engine.hpp"
#include "geom.hpp"

constexpr float width = 25;
constexpr float height = 25;

void shader(float &fragColor, const vec2 &fragCoord) {
  const vec2 uv((fragCoord.x - 0.5 * width) / height,
                (fragCoord.y - 0.5 * height) / height);

  float one = 0.25, two = 0.5, three = 0.75, four = 1.;
  if (fragCoord.x > width / 2) {
    if (fragCoord.y > height / 2) {
      fragColor = one;
    } else {
      fragColor = two;
    }
  } else {
    if (fragCoord.y > height / 2) {
      fragColor = three;
    } else {
      fragColor = four;
    }
  }
}

int main() {
  Engine engine(width, height);
  engine.update(shader);
  engine.render(".=#~");

  return 0;
}
