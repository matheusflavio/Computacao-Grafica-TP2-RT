#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "common.hpp"
#include "vec3.hpp"
#include "ray.hpp"

class Camera {
    private:
        p3 origin;
        v3 horizontalAxis, verticalAxis;
        p3 lowerLeftCorner;
        double aspectRatio;
        double lensRadius;
        vec3 w, u, v;

    public:

        Camera() {}
        
        // vFov stands for vertical field of view, in degrees
        Camera(p3 lookFrom, p3 lookAt, vec3 vUp,
                 double vFov, double aspectRatio,
                 double aperture, double focusDist
        ) : aspectRatio(aspectRatio) {
            double theta = degreesToRadians(vFov);
            double h = tan(theta/2);
            double viewportHeight = 2.0 * h;
            double viewportWidth = aspectRatio * viewportHeight;

            w = (lookFrom - lookAt).normalize();
            u = vec3::cross(vUp, w).normalize();
            v = vec3::cross(w, u);

            double focalLength = 1.0; // distance between the projection plane and the projection point, not the same as focal distance

            origin = lookFrom;
            horizontalAxis = focusDist * viewportWidth * u;
            verticalAxis = focusDist * viewportHeight * v;
            lowerLeftCorner = origin - horizontalAxis/2 - verticalAxis/2 - focusDist*w;

            lensRadius = aperture / 2;
        }

        Ray getRay(double hh, double vv) const {
            vec3 rd = lensRadius * vec3::randomInUnitDisk();
            vec3 offset = rd.x() * u + rd.y() * v;
            return Ray(origin + offset, lowerLeftCorner + hh * horizontalAxis + vv * verticalAxis - origin - offset);
        }
};

#endif // !CAMERA_HPP