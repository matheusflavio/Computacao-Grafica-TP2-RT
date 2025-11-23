#ifndef VEC4_HPP
#define VEC4_HPP

#include <cmath>
#include "vec3.hpp"

using namespace std;

class vec3;

class vec4 {
    public:
        double p[4];

        vec4() : vec4(0, 0, 0, 0) {}
        vec4(double c0, double c1, double c2, double c3) {
            p[0] = c0;
            p[1] = c1;
            p[2] = c2;
            p[3] = c3;
        }

        vec4(vec3 v, double c3);

        double operator[] (int i) const { return p[i]; }
        double& operator[] (int i) { return p[i]; }

        vec4 normalize();

        double dot(const vec3 &v) const;
        double dot(const vec4 &v) const;

        vec4 operator*(double t) const;

        double lengthSquared() const;
        double length() const;
};

#endif