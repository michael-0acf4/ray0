#include "engine.hpp"

std::string COLOR_STRONG =
    "@@@@@@@@@@%%%%%%%%#########*********++++++++===:::...  ";
std::string COLOR_LIGHT = "******+++===----:::::::::::...... ";

void Engine::render(const std::string &palette) const {
  float len = palette.length();
  for (int i = 0; i < (int)height; i++) {
    for (int j = 0; j < (int)width; j++) {
      int index = clamp(len * (1 - fragments[i * width + j]), 0, len - 1);
      std::cout << palette[index];
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
    const std::function<void(float &, const vec2 &)> &fragmentShader,
    int workers) {

  int maxSupported = std::thread::hardware_concurrency();
  if (maxSupported == 0) {
    // https://en.cppreference.com/w/cpp/thread/thread/hardware_concurrency
    workers = 2;
  } else {
    workers = std::max(1, std::min(maxSupported, workers));
  }

  std::vector<std::thread> threads;
  int rows = height / workers;

  auto taskSlice = [&](int startRow, int endRow) {
    for (int y = startRow; y < endRow; y++) {
      for (int x = 0; x < width; x++) {
        vec2 fragCoord(x, y);
        float fragColor = fragments[y * width + x];

        fragmentShader(fragColor, fragCoord);

        put(y, x, fragColor);
      }
    }
  };

  for (int i = 0; i < workers; i++) {
    int startRow = i * rows;
    int endRow = i == workers - 1 ? height : startRow + rows;

    threads.emplace_back(taskSlice, startRow, endRow);
    // std::thread t(taskSlice);
    // threads.push_back(std::move(t));
  }

  for (auto &thread : threads) {
    if (thread.joinable())
      thread.join();
  }
}

Engine::~Engine() {}
