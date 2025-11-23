#ifndef LIGHT_MATERIAL_HPP
#define LIGHT_MATERIAL_HPP

#include "common.hpp"


class LightMaterial : public Material {
    public:
        color col;
        double lightAttenuationConstant, lightAttenuationLinear, lightAttenuationQuadratic;

        LightMaterial(const color& col, double lightAttenuationConstant, double lightAttenuationLinear, double lightAttenuationQuadratic, bool ghostMaterial) : col(col), lightAttenuationConstant(lightAttenuationConstant), lightAttenuationLinear(lightAttenuationLinear), lightAttenuationQuadratic(lightAttenuationQuadratic) {
            this->ghostMaterial = ghostMaterial;
        }

        virtual bool scatter(const Ray& rIn, const HitRecord& hr, color& attenuation, Ray& scattered, bool &isLight) const override {
            // Don't reflect any rays
            isLight = true;
            attenuation = lightAttenuationConstant * col;
            return false;
        }
};

typedef shared_ptr<LightMaterial> LightMaterialPtr;

#endif // !LIGHT_MATERIAL_HPP