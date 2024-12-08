#include "engine.hpp"
#include "geom.hpp"

constexpr float width = 70;
constexpr float height = 10;

void shader(float &fragColor, const vec2 &fragCoord) {
  float color = fragCoord.x / width;
  if (fragCoord.y >= height / 2) {
    fragColor = color;
  } else {
    fragColor = 1 - color;
  }
}

int main() {
  Engine engine(width, height);
  engine.update(shader);
  engine.render("******+++===----:::......  ");

  return 0;
}
