#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>


float min_depth = 0;
float max_depth = 100;
float radius = 1.;
float eps = 0.0001;
float width = 50;
float height = 25;
float aspect_ratio = width / height;
const char *color = "******+++===----:::......  "; // dark ---> bright
float gtime = 0;

// not very interesting ---
struct vec3 {
	float x; float y; float z;
	vec3 (float x, float y, float z) : x(x), y(y), z(z) {}
	void show () { printf("vec3 %f %f %f", x, y, z); }
};

// I know it's ugly, don't look
struct mat4 {
	float m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33;
	mat4 (float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) :
		m00(m00), m01(m01), m02(m02), m03(m03), 
		m10(m10), m11(m11), m12(m12), m13(m13),
		m20(m20), m21(m21), m22(m22), m23(m23),
		m30(m30), m31(m31), m32(m32), m33(m33) {}
};

struct t_screen {
	float width, height;
	std::vector<std::string> pixels;
	t_screen (float w, float h) {
		this->width = w;
		this->height = h;
		for (int i = 0; i < (int) height; i++) {
			pixels[i] = "dsad";
			// for (int j = 0; j < (int) width; j++)
				// pixels[i] += "X";
		}
	}
	
	void show () {
		for (int i = 0; i < (int) height; i++) {
			std::cout << pixels[i] << '\n';
		}
	}
	
	void clear () {
		for (int i = 0; i < (int) height; i++) {
			pixels[i] = "";
			for (int j = 0; j < (int) width; j++)
				pixels[i] += ' ';
		}
		#ifdef _WIN32
			// system("cls");
		#else
			// system("clear");
		#endif
	}
	
	
};

inline float length (vec3 a) { return (float) sqrt(a.x*a.x + a.y*a.y + a.z*a.z); }
inline vec3 scaleReal (vec3 a, float k) { return {a.x * k, a.y * k, a.z * k}; }
inline vec3 add (vec3 a, vec3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
inline vec3 sub (vec3 a, vec3 b) { return add(a, scaleReal(b, -1.)); }
inline float dot (vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
vec3 normalize (vec3 a) {
	float L = length(a); 
	if (L <= 10e-6) return {0., 0., 0.};
	return {a.x / L, a.y / L, a.z / L};
}
vec3 applyTransf (mat4 m, vec3 v) {
	float x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * 1.;
	float y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * 1.;
	float z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * 1.;
	float w = m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * 1.;
	vec3 res {x, y, z};
	if (fabs(w) > eps)
		return scaleReal (res, 1. / w);
	return res;
}
// end ----




// core

// distance function for the scene
// let's try a sphere at (0, 0, 0)
// shapes
float sdSphere (vec3 p, float radius) {
    return length(p) - radius;
}

float sdBox (vec3 p, vec3 b) {
	vec3 vmax {
		(float) std::max(fabs(p.x) - b.x, 0.),
		(float) std::max(fabs(p.y) - b.y, 0.),
		(float) std::max(fabs(p.z) - b.z, 0.)
	};
	return length(vmax);
}

float sdTorus(vec3 p, float tx, float ty) {
  float ax = sqrt(p.x*p.x + p.z*p.z) - tx,
		by = p.y;
  return sqrt(ax*ax + by*by) - ty;
}

float sdUnion (float distA, float distB) {
    return std::min(distA, distB);
}

float sdInter (float distA, float distB) {
    return std::max(distA, distB);
}

float sdDiff (float distA, float distB) {
    return std::max(distA, -distB);
}

mat4 rotateY (float t) {
    float ct = std::cos(t);
    float st = std::sin(t);
    return mat4 {
        ct, 0, st, 0,
         0, 1,  0, 0,
       -st, 0, ct, 0,
         0, 0, 0, 1
    };
}

mat4 rotateZ(float t) {
    float ct = std::cos(t);
    float st = std::sin(t);
    return mat4(
        ct, -st, 0, 0,
        st, ct, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

mat4 rotateX(float t) {
    float ct = std::cos(t);
    float st = std::sin(t);
    return mat4(
        1, 0, 0, 0,
        0, ct, -st, 0,
        0, st, ct, 0,
        0, 0, 0, 1
    );
}

float sdTotalScene (vec3 p) {
    vec3 transf_p = applyTransf (rotateY(-gtime), p);
    transf_p = applyTransf (rotateX(-gtime), transf_p);
    transf_p = applyTransf (rotateZ(-gtime), transf_p);
    return sdSphere(transf_p, 1.);
}

// for ray marching the gradient at the contact point is orthogonal
// to the surface
// just an approximation of the gradient vector
vec3 sceneNormalAt (vec3 p) {
  return normalize({
    sdTotalScene ({p.x + eps, p.y, p.z}) -  sdTotalScene ({p.x - eps, p.y, p.z}),
    sdTotalScene ({p.x, p.y + eps, p.z}) -  sdTotalScene ({p.x, p.y - eps, p.z}),
    sdTotalScene ({p.x, p.y, p.z + eps}) -  sdTotalScene ({p.x, p.y, p.z - eps})
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

void computeScreenBuffer (t_screen &screen) {
	// motivation, the bigger the screensize, the more the steps
	float dp = 1 / std::max(height, width); // we can also assign an arbitrary step
	
	// normalized coordinates centered at (0., 0.)
	float sy = -0.5, ey = 0.5;
	float sx = -0.5, ex = 0.5;
	float ratio = width / height;
	sx *= ratio; // fix the ratio of the x component accordingly
	
	// iterate through the texture coordinate
	for (float y = sy; y < ey; y += dp) {
		for (float x = sx; x < ex; x += dp) {
			// we define a direction for each pixel
			vec3 camera {0., 0., 1.006}; // the camera must be above
			vec3 cam_dir = normalize ({x, y, -1.});
			
			float d_traveled = rayMarch (camera, cam_dir);
			char pixel = ' ';
			if (d_traveled <= max_depth) {
				// hits the scene i.e. the sphere
				// with light				
				vec3 contact_point = add (camera, scaleReal(cam_dir, d_traveled));
				vec3 contact_normal = sceneNormalAt (contact_point);
				vec3 light_pos {1., 1. , 2.}; // i.e. the sun
				// light_pos = applyTransf(rotateY(-gtime), light_pos); 
				vec3 light_dir = sub (light_pos, contact_point); // dir of the ray that hits the sphere
				light_dir = normalize (light_dir);
				float diffuse = dot (light_dir, contact_normal);
				
				if (diffuse > 1)
					diffuse = 1;

				// color
				float len = (float) strlen(color);		
				pixel = color[(int) (diffuse * len)];
			}
			
			// transform the texture coordinate to a real screen coordinate
			int real_x = (int) ((x + 0.5) * width),
				real_y = (int) ((y + 0.5) * height);
			screen.pixels[real_x][real_y] = pixel;
		}
	}
}

int main () {
	t_screen screen (width, height);
	screen.show();
	// computeScreenBuffer (screen);
	float dt = 0.1;
	while (true) {
		// computeScreenBuffer (screen);
		// screen.show();
		// screen.clear();
		
		gtime += dt;
	}
	return 0;
}
