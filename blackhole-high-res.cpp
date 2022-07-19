#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>

#include "not-interesting.h"

// 150x80
float width = 150;
float height = 80;

// black hole configuration
const float RS = .125; // singularity radius
const float PS_RAD = 1.5 * RS; // photon sphere radius
const float ACC_RAD = 3. * RS; // accretion disc radius

float gtime = .1;

// ray marching configuration
const float MAX_DEPTH = 100.;
const int MAX_STEPS = 300;
const float DP = 0.05;

// core
float sdAccretionDisc (vec3 p) {
    p = applyTransf(rotateX(-std::sin((PI / 3.) * 3.2)), p); 
    float p1 = sdRoundedCylinder(p, ACC_RAD, ACC_RAD / 6, .001);
    float p2 = sdSphere(p, ACC_RAD);
    return sdSmoothSubtraction(p2, p1, .5);
}

// rough approximation of how light bends
float interpSpaceDistortion (float sz_rad, float dist_singularity) {
    float distortion_factor = 2.79;
    float dist_ratio = sz_rad / dist_singularity;
    return pow(dist_ratio, distortion_factor);
}

vec3 bendLightDirection (vec3 bl_pos, vec3 ray_pos, vec3 ray_dir) {
    // points at the current direction
    vec3 u = normalize(ray_dir);
    // points at the singularity i.e. center of the black hole
    // (u, v) angle is the maximum deviation angle starting from the current
    // unchanged light direction
    vec3 v = normalize(sub(bl_pos, ray_pos));
    
    // we are not going to use v directly !
    // let's bend u in such a way that it follows v (kinda)
    // also let's consider how close it is in order to bend it more
    float dist_how_close = length(sub(bl_pos, ray_pos));
    float lerp_val = interpSpaceDistortion(RS, dist_how_close);
    
    return normalize(lerp3(u, v, lerp_val));
}

float normalizedNoiseTexture (float x, float y) {
	// noise-ish texture... just like a normalized chess board
	int sx = (int) (x * 10.f);
	int sy = (int) (y * 30.f);
	if ((sx + sy) % 2 == 0) return 0.6;
	return 1.;
}

void computeScreenBuffer (t_screen &screen) {
	// motivation : the bigger the screensize, the more the steps
	float dp = 1 / std::max(height, width); // we can also assign an arbitrary step
	
	// normalized coordinates centered at (0., 0.)
	float sy = -0.5, ey = 0.5;
	float sx = -0.5, ex = 0.5;
	float ratio = width / height;
	sx *= ratio;
	sy *= ratio;
	
	// iterate through the texture coordinate
	for (float y = sy; y < ey; y += dp) {
		for (float x = sx; x < ex; x += dp) {
			// we define a direction for each pixel
			vec3 camera {0., 0., 2.01}; // the camera must be above
			vec3 cam_dir = normalize ({x, y, -1.});

			// ray marching
			float dtravel = 0.05; // basically at the start of the screen, ie. 0
			
			vec3 bl_pos(0., 0., 0.);
			vec3 ray_pos = camera;
			vec3 ray_dir = cam_dir;
			
			for (int i = 0; i < MAX_STEPS; i++) {
				// usual SDF
				// ray_pos = add(ray_pos, scaleReal(cam_dir, DP)); // or just don't change the value of ray_dir
				
				// bending the SDF
				ray_dir = bendLightDirection (bl_pos, ray_pos, ray_dir);
				ray_pos = add(ray_pos, scaleReal(ray_dir, DP));
				
				float d = sdAccretionDisc (ray_pos);
				dtravel += d;
				
				if (d < EPSILON) break;
				if (d > MAX_DEPTH) break;
			}

			// base color
			float diffuse = 0.05;

			// just after the photon sphere
			if (length(vec3(x, y, 0.)) < PS_RAD)
				diffuse = 0.;

			// construct the accretion disk and the photon sphere
			if (dtravel <= MAX_DEPTH) {
				vec3 delta = sub(ray_pos, bl_pos);
				// make it glow more as it approaches the center
				float glow = .3 / std::pow(length(delta), 2.); // create glow and diminish it with distance
				glow = clamp(glow, 0., 1.); // remove artifacts


				// rotation effect
				// the noise-ish texture is normalized and have fixed values
				// rotate the uv coordinate
				float rot_vel = 3.;
				float s = std::sin(gtime * rot_vel), c = std::cos(gtime * rot_vel);
				float tuv_x = ray_pos.x;
				float tuv_y = ray_pos.y;
				float rot_x = c * tuv_x - s * tuv_y;
				float rot_y = s * tuv_x + c * tuv_y;

				float tex_color = normalizedNoiseTexture (rot_x, rot_y);

				// total color
				diffuse += glow * tex_color;
			}
			
			// end ray marching

			char pixel = screen.computeColorGivenDiffuseLight(diffuse, COLOR_STRONG);

			// texture coords ---> screen coords
			// -0.5  0.5     ---> -width/2 width/2
			// -0.5  0.5     ---> -height/2 height/2
			int screen_x = (int) ((x + 0.5) * width),
				screen_y = (int) ((y + 0.5) * height);
			screen.put (screen_y, screen_x, pixel);
		}
	}
}

int main () {
	t_screen screen (width, height);
	screen.clear();
	while (true) {
		screen.saveCursor ();
		// std::cout << "t=" << gtime << "\n";
		computeScreenBuffer (screen);
		screen.show ();
		screen.restoreCursor ();

		gtime += .1;
	}
	return 0;
}