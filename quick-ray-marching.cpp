#include <iostream>
#include <cmath>
#include <cstring>

float min_depth = 0;
float max_depth = 100;
float radius = 1.;
float eps = 0.0001;
float width = 50;
float height = 35;
const char *color = "******+++===----:::......  "; // dark ---> bright

// not very interesting ---
struct vec3 {
	float x; float y; float z;
	vec3 (float x, float y, float z) : x(x), y(y), z(z) {}
};
inline float length (vec3 a) { return (float) sqrt(a.x*a.x + a.y*a.y + a.z*a.z); }
inline vec3 scaleReal (vec3 a, float k) { return {a.x * k, a.y * k, a.z * k}; }
inline vec3 add (vec3 a, vec3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
inline vec3 sub (vec3 a, vec3 b) { return add(a, scaleReal(b, -1.)); }
inline float dot (vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
vec3 normalize (vec3 a) {
	float L = length(a); 
	if (L <= eps) return {0., 0., 0.};
	return {a.x / L, a.y / L, a.z / L};
}
// end ----




// core
// distance function for the scene
// let's try a sphere at (0, 0, 0)
float sdTotalScene (vec3 p) {
	return length(p) - radius;
}

// for ray marching the gradient at the contact point is orthogonal
// to the surface
// just an approximation of the gradient vector
vec3 sceneNormalAt (vec3 p) {
  return normalize({
    sdTotalScene ({p.x + eps, p.y, p.z})  -  sdTotalScene ({p.x - eps, p.y, p.z}),
    sdTotalScene ({p.x, p.y + eps, p.z})  -  sdTotalScene ({p.x, p.y - eps, p.z}),
    sdTotalScene ({p.x, p.y, p.z  + eps}) -  sdTotalScene ({p.x, p.y, p.z - eps})
  });
}

float rayMarch (vec3 camera, vec3 cam_dir) {
	float d_traveled = min_depth; // basically at the start of the screen, ie. 0
	int steps = 200;
	while (steps > 0) {
		vec3 current_pos = add (camera, scaleReal(cam_dir, d_traveled));
		float d = sdTotalScene (current_pos);
		d_traveled += d;
		if (d < eps) break;
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
	// float dt = 0.01, elapsedTime = 0;
	// while (true) {
		// draw (elapsedTime);
		// elapsedTime += dt;
	// }
	return 0;
}
