#ifndef LAMBERTIAN_MATERIAL_HPP
#define LAMBERTIAN_MATERIAL_HPP

#include "common.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include "solid_color.hpp"
#include "image_texture.hpp"
#include "image_texture_ps.hpp"
#include "checker_texture.hpp"

class LambertianMaterial : public Material {
    public:
        TexturePtr col;
        LambertianMaterial(const color& col) : col(make_shared<SolidColor>(col)) {}
        LambertianMaterial(const TexturePtr& col) : col(col) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& hr, color& attenuation, Ray& scattered, bool &isLight) const override {
            auto scatterDirection = hr.normal.normalize() + vec3::randomUnitVector();

            // If the scatter direction is close to 0, NaN issues would occur.
            if(scatterDirection.nearZero()) 
                scatterDirection = hr.normal;

            scattered = Ray(hr.p, scatterDirection);
            attenuation = col->value(hr.uv, hr.p);
            return true;
        }
};

#endif // !LAMBERTIAN_MATERIAL_HPP