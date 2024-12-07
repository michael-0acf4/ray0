#pragma once

#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include "geom.hpp"

#define PI 3.1415
#define EPSILON 0.00001f
#define BIG_EPSILON 0.01f

extern const char *COLOR_STRONG;
extern const char *COLOR_LIGHT;

class Engine {
public:
  Engine(float w, float h)
      : width(w), height(h), fragments(static_cast<size_t>(w * h), 0) {}
  ~Engine();

  void update(const std::function<void(float &, const vec2 &)> &fragmentShader);
  void render(const char *palette = COLOR_LIGHT) const;
  void saveCursor();
  void restoreCursor();
  void clear();

private:
  float width, height;
  std::vector<float> fragments;

  void put(int y, int x, float fragColor);
};
