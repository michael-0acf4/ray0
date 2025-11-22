#include "ray0.hpp"

const vec2 fResolution = {64, 64};
float t = 0.0;

void fakeShader(float &fragColor, const vec2 &fragCoord) {
  vec2 FC = fragCoord;
  vec2 r = fResolution;
  vec3 o;

  // https://www.shadertoy.com/view/WfS3Dd
  // https://gist.github.com/rexim/ef86bf70918034a5a57881456c0a0ccf
  // https://x.com/XorDev/status/1894123951401378051
  vec2 p = (FC * 2. - r) / r.y, l, i,
       v = p * (l += vec2(4. - 4. * abs(.7 - dot(p, p))));
  for (; i.y++ < 8.; o += (v_sin(vec3(v.x, v.y, v.y)) + 1.) * abs(v.x - v.y))
    v += v_cos(v.yx() * i.y + i + t) / i.y + .7;
  o = v_tanh(5. * v_exp(l.x - 4. - p.y * vec3(-1, 1, 2)) / o);
  // --------------------

  fragColor = o.avg();
}

int main() {
  Engine engine(fResolution);
  int workerThreads = 2;

  while (true) {

    engine.clear();
    engine.update(fakeShader, workerThreads);

    engine.saveCursor();
    engine.render(COLOR_STRONG);
    engine.restoreCursor();

    t += 0.1f;

    engine.restoreCursor();
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(16ms);
  }

  return 0;
}
