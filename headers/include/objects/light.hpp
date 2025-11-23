#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "vec3.hpp"
#include "ray.hpp"
#include "light_material.hpp"
#include "hittable.hpp"

class Light : public Hittable {
    public:
        p3 center;
        shared_ptr<LightMaterial> matPtr;

        Light(p3 center, shared_ptr<LightMaterial> m) : center(center), matPtr(m) {};

        bool hit(const Ray& r, double tMin, double tMax, HitRecord &rec) const override { return false; }
};



#endif // !LIGHT_HPP
