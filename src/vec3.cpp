#include "vec3.hpp"

double vec3::dot(const vec4 &v) const {
    const vec3 &u = *this;
    return u.p[0] * v.p[0] + u.p[1] * v.p[1] + u.p[2] * v.p[2] + v.p[3] * 1.0;
}

vec3 vec3::refract(const vec3& normal, double etaiOverEtat) {
    double cosTheta = fmin(-(*this).dot(normal), 1.0);
    vec3 rOutPerp = etaiOverEtat * ((*this) + cosTheta * normal);
    vec3 rOutParal = -sqrt(fabs(1.0 - rOutPerp.lengthSquared())) * normal;
    return rOutPerp + rOutParal;
}

vec3 vec3::reflect(const vec3& normal) const {
    return *this - 2 * (dot(normal) * normal);
}