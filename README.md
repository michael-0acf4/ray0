# ray0

A GLSL-like renderer on the terminal.

![screenshot](screenshots/cover.png)

## Examples

```bash
# Simple sphere
make sphere

# Raymarch on a rotating 3d object
make composition

# Blackhole
make blackhole
```

## Usage

```cpp
#include "engine.hpp"
#include "geom.hpp"

constexpr float width = 25;
constexpr float height = 25;
constexpr float ratio = width / height;
constexpr int workerThreads = 4;

void shader(float &fragColor, const vec2 &fragCoord) {
  const vec2 uv((fragCoord.x / width - 0.5) * ratio,
                (fragCoord.y / height - 0.5) * ratio);

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
  engine.update(shader, workerThreads);
  engine.render("3210 "); // You can set the color palette or use the default (none)

  return 0;
}
```

![alt text](screenshots/divided-circle.png)
