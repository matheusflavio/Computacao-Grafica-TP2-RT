#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"

class Sphere : public Hittable {
    public:
        p3 center;
        double radius;
        shared_ptr<Material> matPtr;

        Sphere(p3 center, double radius, shared_ptr<Material> m) : center(center), radius(radius), matPtr(m) {};

        // check ray colision with sphere
        // this function considers the vectorized sphere equation and solve it for t, t is the multiplier of the direction on the Ray's formula.
        bool hit(const Ray& r, double tMin, double tMax, HitRecord &rec) const override;

        static vec2 getSphereUV(const p3& p);
};


bool Sphere::hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const {
    p3 oc = r.origin() - center;
    auto a = r.direction().lengthSquared();
    auto halfB = oc.dot(r.direction());
    auto c = oc.lengthSquared() - radius * radius;

    auto discriminant = halfB * halfB - a * c;
    if(discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);
    
    double root = (-halfB - sqrt(discriminant)) / a;
    // if root is out of valid range, try the second root
    if(root < tMin || root > tMax) {
        root = (-halfB + sqrt(discriminant)) / a;
        if(root < tMin || root > tMax) return false;
    }

    // save record of sphere hit with its normal
    rec.t = root;
    rec.p = r.at(root);
    v3 outwardNormal = (rec.p - center) / radius;
    rec.rayComingFromOutside = !outwardNormal.sameDirection(r.direction());
    rec.normal = rec.rayComingFromOutside ? outwardNormal : -outwardNormal;
    rec.matPtr = matPtr;
    rec.uv = getSphereUV(outwardNormal);
    return true;
}


// p is a point on a sphere of radius 1
// Returned (u,v) is the texture coordinates for the point p on the sphere
vec2 Sphere::getSphereUV(const p3& p) {
    double phi = atan2(-p.z(), p.x()) + pi;
    double theta = acos(-p.y());
    return vec2(phi / (2 * pi), theta / pi);
}

#endif // !SPHERE_HPP


/*
    Texture coordinates to sphere

    (𝜃,𝜙) are respectively the angle up from -Y and the angle around the Y-Axis.
    We need a way to represent texture coordinates u and v int he interval [0,1], where (0,0) is the bottom left of the texture.
    
    The normalization from (𝜃,𝜙) to (u,v) is done by the following formula:
    u = 𝜙 / (2𝜋)
    v = 𝜃 / (2𝜋)

    For a unit sphere centered at the origin, we have:
    y = - cos(𝜃)
    x = - cos(𝜙) sin(𝜃)
    z = sin(𝜙) sin(𝜃)

    We can solve for 𝜙 with 𝜙 = atan2(z, -x) = atan2(-z, x) + π
    and for 𝜃 with 𝜃 = acos(-y)
*/