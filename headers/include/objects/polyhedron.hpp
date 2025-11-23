#ifndef POLYHEDRON_HPP
#define POLYHEDRON_HPP

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include <float.h>

class Plane {
    public:
        double a, b, c, d;
        Plane(double a, double b, double c, double d) : a(a), b(b), c(c), d(d) {};
};


class Polyhedron : public Hittable {
    public:
        vector<Plane> faces;
        shared_ptr<Material> matPtr;

        Polyhedron(shared_ptr<Material> m) : matPtr(m) {};

        // check ray colision with polyhedron
        // this function considers the vectorized polyhedron equation and solve it for t, t is the multiplier of the direction on the Ray's formula.
        bool hit(const Ray& r, double tMin, double tMax, HitRecord &rec) const override;

        void addFace(Plane p) {
            faces.push_back(p);
        }

        int facesCount() {
            return faces.size();
        }

        Plane getFace(int i) {
            return faces[i];
        }

        static vec2 getPolyhedronUV(const p3& p);
};


bool Polyhedron::hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const {

    double eps = 1e-6;
    double t;
    // double tMin = 0, tMax = DOUBLE_MAX;
    vec4 nT0, nT1;

    for(int i = 0; i < faces.size(); i++) {
        vec4 pIn = vec4(r.origin(), 0);
        vec4 n = vec4(faces[i].a, faces[i].b, faces[i].c, 0);
        double dn = r.dir.dot(n);
        double val = pIn.dot(n) + faces[i].d;

        if(fabs(dn) < eps) {
            if(val > eps) {
                tMin = -1;
            }
        }

        if(dn > eps) {
            t = -val / dn;
            if(t < tMax) {
                tMax = t;
                nT1 = n;
            }
        }

        if(dn < -eps) {
            t = -val / dn;
            if(t > tMin) {
                tMin = t;
                nT0 = n;
            }
        }
    }

    if(tMax < tMin) return false;

    bool returnTrue = false;

    if(fabs(tMin) <= eps && (tMax >= tMin) && tMax < DBL_MAX) {
        nT1 = (nT1 * -1.0).normalize();
        rec.normal = vec3(nT1[0], nT1[1], nT1[2]);
        rec.t = tMax;
        returnTrue = true;
    }

    if(tMin > eps && tMax >= tMin) {
        nT0 = nT0.normalize();
        rec.normal = vec3(nT0[0], nT0[1], nT0[2]);
        rec.t = tMin;
        returnTrue = true;
    }

    if(!returnTrue) return false;

    rec.p = r.at(rec.t);
    rec.rayComingFromOutside = !(rec.normal.dot(r.direction()) < 0);
    rec.matPtr = matPtr;
    rec.uv = getPolyhedronUV(rec.rayComingFromOutside ? rec.normal : -rec.normal);

    return true;
}


// p is a point on a polyhedron of radius 1
// Returned (u,v) is the texture coordinates for the point p on the polyhedron
vec2 Polyhedron::getPolyhedronUV(const p3& p) {
    double phi = atan2(-p.z(), p.x()) + pi;
    double theta = acos(-p.y());
    return vec2(phi / (2 * pi), theta / pi);
}

typedef shared_ptr<Polyhedron> PolyhedronPtr;

#endif // !POLYHEDRON_HPP