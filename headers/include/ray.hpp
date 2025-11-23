#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

class Ray {
    public:
        p3 orig;
        v3 dir;

        Ray() {}
        Ray(const p3& origin, const v3& direction) : orig(origin), dir(direction) {}

        p3 origin() const { return orig; }
        v3 direction() const { return dir; }

        p3 at(double t) const {
            return orig + t * dir;
        }
};

#endif // !RAY_HPP

