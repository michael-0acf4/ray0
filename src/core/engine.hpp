#pragma once

#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include "geom.hpp"

#define PI 3.1415
#define EPSILON 0.00001f
#define BIG_EPSILON 0.01f

extern std::string COLOR_STRONG;
extern std::string COLOR_LIGHT;

class Engine {
public:
  Engine(float w, float h)
      : width(w), height(h), fragments(static_cast<size_t>(w * h), 0) {}

  Engine(vec2 iResolution)
      : width(iResolution.x), height(iResolution.y),
        fragments(static_cast<size_t>(iResolution.x * iResolution.y), 0) {}

  ~Engine();

  void update(const std::function<void(float &, const vec2 &)> &fragmentShader,
              int workers = 1);
  void render(const std::string & = COLOR_LIGHT) const;
  void saveCursor();
  void restoreCursor();
  void clear();

private:
  float width, height;
  std::vector<float> fragments;

  void put(int y, int x, float fragColor);
};
