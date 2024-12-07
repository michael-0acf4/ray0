#include "engine.hpp"

const char *COLOR_STRONG =
    "@@@@@@@@@@%%%%%%%%#########*********++++++++===:::...  ";
const char *COLOR_LIGHT = "******+++===----:::::::::::...... ";

void Engine::render(const char *palette) const {
  float len = (float)strlen(palette);
  for (int i = 0; i < (int)height; i++) {
    for (int j = 0; j < (int)width; j++) {
      float light = clamp(fragments[i * width + j], 0, 1.);
      std::cout << palette[static_cast<int>(len * (1 - light))];
    }

    std::cout << '\n';
  }
}

void Engine::saveCursor() { std::cout << "\033[s"; }

void Engine::restoreCursor() { std::cout << "\033[u"; }

void Engine::clear() {
  for (int i = 0; i < (int)height; i++) {
    for (int j = 0; j < (int)width; fragments[i * width + j] = 0., j++)
      ;
  }
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void Engine::put(int y, int x, float fragColor) {
  if (x < 0 || x >= (int)width || y >= (int)height || y < 0)
    return;

  fragments[y * width + x] = fragColor;
}

void Engine::update(
    const std::function<void(float &, const vec2 &)> &shaderFn) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      vec2 fragCoord(x, y);
      float fragColor = fragments[y * width + x];
      float old = fragColor;

      shaderFn(fragColor, fragCoord);

      put(y, x, fragColor);
    }
  }
}

Engine::~Engine() {}
