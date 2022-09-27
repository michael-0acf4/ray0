#include <iostream>
#include <cmath>
#include <cstring>

#include "not-interesting.h"

float min_depth = 0.;
float max_depth = 100.;
float radius = 1.;
float width = 50;
float height = 35;
const char *color = "******+++===----:::......  "; // dark ---> bright


// core
// distance function for the scene
// let's try a sphere at (0, 0, 0)
float sdTotalScene (vec3 p) {
	return length(p) - radius;
}

// for ray marching the gradient at the contact point is orthogonal
// to the contact surface
// let's approximate the gradient vector with that information
vec3 sceneNormalAt (vec3 p) {
  return normalize({
	sdTotalScene ({p.x + EPSILON, p.y, p.z})  - sdTotalScene ({p.x - EPSILON, p.y, p.z}),
	sdTotalScene ({p.x, p.y + EPSILON, p.z})  - sdTotalScene ({p.x, p.y - EPSILON, p.z}),
	sdTotalScene ({p.x, p.y, p.z  + EPSILON}) - sdTotalScene ({p.x, p.y, p.z - EPSILON})
  });
}

float rayMarch (vec3 camera, vec3 cam_dir) {
	float d_traveled = min_depth; // basically at the start of the screen, ie. 0
	int steps = 200;
	while (steps > 0) {
		vec3 current_pos = add (camera, scaleReal(cam_dir, d_traveled));
		float d = sdTotalScene (current_pos);
		d_traveled += d;
		if (d < EPSILON) break;
		if (d_traveled > max_depth) break;
		steps--;
	}
	return d_traveled;
}

void draw (float elapsedTime = 1.) {
	float sh = -height / 2, eh = height / 2;
	float sx = -width / 2, ex = width / 2;
	
	for (float y = sh; y < eh; y++) {
		for (float x = sx; x < ex; x++) {
			// we define a direction for each pixel
			vec3 camera {0., 0., 1.003}; // the camera must be above
			vec3 cam_dir = normalize ({x, y, -1.});
			
			float d_traveled = rayMarch (camera, cam_dir);
			char pixel = ' ';
			if (d_traveled <= max_depth) {
				// hits the scene i.e. the sphere
				// with light				
				vec3 contact_point = add (camera, scaleReal(cam_dir, d_traveled));
				vec3 contact_normal = sceneNormalAt (contact_point);
				vec3 light_pos {1., 1. , 2.}; // i.e. the sun
				vec3 light_dir = sub (light_pos, contact_point); // dir of the ray that hits the sphere
				light_dir = normalize (light_dir);
				float diffuse = dot (light_dir, contact_normal);
				
				if (diffuse > 1)
					diffuse = 1;

				// color
				float len = (float) strlen(color);		
				pixel = color[(int) (diffuse * len)];
			}
			printf("%c", pixel);
		}
		printf("\n");
	}
}

int main () {
	draw();
	return 0;
}
