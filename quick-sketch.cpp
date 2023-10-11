#include <cmath>
#include <cstring>
#include <iostream>

#include "not-interesting.h"

const float min_depth = 0.;
const float max_depth = 100.;
const float radius = 1.;
const float width = 50;
const float height = 35;
const char *color = "******+++===----:::......  "; // dark ---> bright

// core
// distance function for the scene
// let's try a sphere at (0, 0, 0)
inline float sdTotalScene(vec3 p) { return length(p) - radius; }

// for ray marching the gradient at the contact point is orthogonal
// to the contact surface
// let's approximate the gradient vector with that information
inline vec3 sceneNormalAt(vec3 p) {
  return normalize({sdTotalScene({p.x + EPSILON, p.y, p.z}) -
                        sdTotalScene({p.x - EPSILON, p.y, p.z}),
                    sdTotalScene({p.x, p.y + EPSILON, p.z}) -
                        sdTotalScene({p.x, p.y - EPSILON, p.z}),
                    sdTotalScene({p.x, p.y, p.z + EPSILON}) -
                        sdTotalScene({p.x, p.y, p.z - EPSILON})});
}

inline float rayMarch(vec3 camera, vec3 cam_dir) {
  float d_traveled = min_depth; // basically at the start of the screen, ie. 0
  int steps = 200;
  while (steps > 0) {
    vec3 current_pos = add(camera, scaleReal(cam_dir, d_traveled));
    float d = sdTotalScene(current_pos);
    d_traveled += d;
    if (d < EPSILON)
      break;
    if (d_traveled > max_depth)
      break;
    steps--;
  }
  return d_traveled;
}

void draw(float elapsedTime = 1.) {
  const float sh = -height / 2, eh = height / 2;
  const float sx = -width / 2, ex = width / 2;

  for (float y = sh; y < eh; y++) {
    for (float x = sx; x < ex; x++) {
      // define a direction for each pixel
      vec3 camera{0., 0., 1.003}; // camera must be above
      vec3 cam_dir = normalize({x, y, -1.});

      float d_traveled = rayMarch(camera, cam_dir);
      char pixel = ' ';
      if (d_traveled <= max_depth) {
        // hits the scene i.e. the sphere
        const vec3 contact_point = add(camera, scaleReal(cam_dir, d_traveled));
        const vec3 contact_normal = sceneNormalAt(contact_point);
        const vec3 light_pos{1., 1., 2.};
        vec3 light_dir = sub(light_pos, contact_point);
        light_dir = normalize(light_dir);
        float diffuse = dot(light_dir, contact_normal);

        diffuse = std::fmin(diffuse, 1.);

        // color
        const float len = (float)strlen(color);
        pixel = color[(int)(diffuse * len)];
      }
      printf("%c", pixel);
    }
    printf("\n");
  }
}

int main() {
  draw();
  return 0;
}
