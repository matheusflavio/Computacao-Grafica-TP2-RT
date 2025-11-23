#include "vec4.hpp"

double vec4::dot(const vec3 &v) const {
    const vec4 &u = *this;
    return u.p[0] * v.p[0] + u.p[1] * v.p[1] + u.p[2] * v.p[2] + u.p[3] * 1.0;
}

double vec4::dot(const vec4 &v) const {
    const vec4 &u = *this;
    return u.p[0] * v.p[0] + u.p[1] * v.p[1] + u.p[2] * v.p[2] + u.p[3] * v.p[3];
}

vec4::vec4(vec3 v, double c3) {
    p[0] = v[0];
    p[1] = v[1];
    p[2] = v[2];
    p[3] = c3;
}

double vec4::lengthSquared() const {
    return p[0]*p[0] + p[1]*p[1] + p[2]*p[2] + p[3]*p[3];
}

double vec4::length() const {
    return sqrt(lengthSquared());
}

vec4 vec4::operator*(double t) const{
    return vec4(p[0] * t, p[1] * t, p[2] * t, p[3] * t);
}

vec4 vec4::normalize() {
    double len = length();
    vec4 newV = *this;
    if (len > 0) {
        newV.p[0] /= len;
        newV.p[1] /= len;
        newV.p[2] /= len;
        newV.p[3] /= len;
    }
    return newV;
}