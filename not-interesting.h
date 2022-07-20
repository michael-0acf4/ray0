#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>

#define PI 3.1415
#define EPSILON 0.00001f
#define BIG_EPSILON 0.01f

// dark ---> bright
const char *COLOR_STRONG = "@@@@@@@@@@%%%%%%%%#########*********++++++++===:::...  ";
const char *COLOR_LIGHT = "******+++===----:::::::::::...... ";

// not very interesting
struct vec3 {
	float x; float y; float z;
	vec3 (float x, float y, float z) : x(x), y(y), z(z) {}
	inline void show () { printf("vec3 %f %f %f", x, y, z); }
};

// I know it looks ugly
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
	std::vector<std::vector<char>> pixels;
	
	t_screen (float w, float h) {
		this->width = w;
		this->height = h;
		for (int i = 0; i < (int) height; i++) {
			std::vector<char> row;
			for (int j = 0; j < (int) width; j++)
				row.push_back (' ');
			pixels.push_back (row);
		}
	}

    char computeColorGivenDiffuseLight (float light, const char *palette = COLOR_LIGHT) {
        float len = (float) strlen (palette);
        int index = std::min (
            std::max (0, (int) (light * len)),
            (int) (len - 1)
        );
        // reverse
        return palette[(int) len - index - 1];
    }

	void put (int y, int x, char pixel) {
		if (x < 0 || x >= (int) width || y >= (int) height || y < 0)
			return;
		pixels[y][x] = pixel;
	}
	
	void show () {
		for (int i = 0; i < (int) height; i++) {
			for (int j = 0; j < (int) width; std::cout << pixels[i][j], j++);
			std::cout << '\n';
		}
	}

	void saveCursor () {
		std::cout << "\033[s";
	}

	void restoreCursor () {
		std::cout << "\033[u";
	}
	
	void clear () {
		for (int i = 0; i < (int) height; i++)
			for (int j = 0; j < (int) width; pixels[i][j] = ' ', j++);
		#ifdef _WIN32
			system("cls");
		#else
			system("clear");
		#endif
	}

};

inline float length (vec3 a) { return (float) sqrt(a.x*a.x + a.y*a.y + a.z*a.z); }

inline vec3 scaleReal (vec3 a, float k) { return {a.x * k, a.y * k, a.z * k}; }

inline vec3 add (vec3 a, vec3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }

inline vec3 sub (vec3 a, vec3 b) { return add(a, scaleReal(b, -1.)); }

inline float dot (vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

vec3 v_max (vec3 a, vec3 b) {
	return {
		std::max(a.x, b.x),
		std::max(a.y, b.y),
		std::max(a.z, b.z)
	};
}

vec3 v_min (vec3 a, vec3 b) {
	return {
		std::min(a.x, b.x),
		std::min(a.y, b.y),
		std::min(a.z, b.z)
	};
}

vec3 normalize (vec3 a) {
	float L = length(a); 
	if (L <= EPSILON) return {0., 0., 0.};
	return {a.x / L, a.y / L, a.z / L};
}

vec3 applyTransf (mat4 m, vec3 v) {
	float x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * 1.;
	float y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * 1.;
	float z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * 1.;
	float w = m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * 1.;
	vec3 res {x, y, z};
	if (fabs(w) > EPSILON)
		return scaleReal (res, 1. / w);
	return res;
}

// basic transformations
mat4 rotateY (float t) {
    float ct = std::cos (t);
    float st = std::sin (t);
    return mat4 (
        ct, 0, st, 0,
        0, 1,  0, 0,
        -st, 0, ct, 0,
        0, 0, 0, 1
    );
}

mat4 rotateZ (float t) {
    float ct = std::cos (t);
    float st = std::sin (t);
    return mat4 (
        ct, -st, 0, 0,
        st, ct, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

mat4 rotateX (float t) {
    float ct = std::cos (t);
    float st = std::sin (t);
    return mat4 (
        1, 0, 0, 0,
        0, ct, -st, 0,
        0, st, ct, 0,
        0, 0, 0, 1
    );
}

// shapes
inline float sdSphere (vec3 p, float radius) {
    return length(p) - radius;
}

inline float sdBox (vec3 p, vec3 b) {
	return length (vec3( // vmax
		(float) std::max(fabs(p.x) - b.x, 0.),
		(float) std::max(fabs(p.y) - b.y, 0.),
		(float) std::max(fabs(p.z) - b.z, 0.)
	));
}

inline float sdTorus (vec3 p, float tx, float ty) {
    return -ty + sqrt (
        p.y*p.y + std::pow(sqrt(p.x*p.x + p.z*p.z) - tx, 2)
    );
}

inline float sdUnion (float distA, float distB) {
    return std::min (distA, distB);
}

inline float sdInter (float distA, float distB) {
    return std::max (distA, distB);
}

inline float sdDiff (float distA, float distB) {
    return std::max (distA, -distB);
}

vec3 lerp3 (vec3 colorone, vec3 colortwo, float value) {
	return add (
		colorone,
		scaleReal (sub(colortwo, colorone), value)
	);
}

float lerp (float colorone, float colortwo, float value) {
	return lerp3 (vec3(colorone, 0., 0.), vec3(colortwo, 0., 0.), value).x;
}

float clamp (float value, float min, float max) {
	return std::max (min, std::min(max, value));
}

float sdRoundedCylinder (vec3 p, float ra, float rb, float h) {
    vec3 d = vec3(length({p.x, 0., p.z}) - 2.0 * ra + rb, fabs(p.y) - h, 0.);
    return std::min(std::max(d.x, d.y), 0.f) + length(v_max(d, {0., 0., 0.})) - rb;
}

float sdSmoothSubtraction (float d1, float d2, float k) {
    float h = clamp(0.5 - 0.5 * (d2 + d1) / k, 0.0, 1.0);
    return lerp(d2, -d1, h) + k * h * (1.0 - h);
}

float step (float a, float edge) {
    // test sigmoid
    if (a < edge) return 1.;
    return 0.;
}

float fixed_fmod (float a, float n) {
	return std::fmod(std::fmod(a, n) + n, n);
}