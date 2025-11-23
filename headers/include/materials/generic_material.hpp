#ifndef GENERIC_MATERIAL_HPP
#define GENERIC_MATERIAL_HPP

#include "dialectric_material.hpp"
#include "lambertian_material.hpp"
#include "metal_material.hpp"

/* Make a mix of Lambertian, Metal and Dialectric */
class GenericMaterial : public Material {
    public:
        double reflectionCoefficient;
        double refractionCoefficient;
        double indexOfrefraction;
        double fuzz;

        TexturePtr col = nullptr;

        GenericMaterial(double ambientLightCoefficient, double diffuseLightCoefficient, double specularLightCoefficient, double reflectionLightExponent, double reflectionCoefficient, double refractionCoefficient, double indexOfrefraction) 
        : reflectionCoefficient(reflectionCoefficient), refractionCoefficient(refractionCoefficient), indexOfrefraction(indexOfrefraction),  col(make_shared<SolidColor>(color(1.0, 1.0, 1.0))) {
            this->ambientLightCoefficient = ambientLightCoefficient;
            this->diffuseLightCoefficient = diffuseLightCoefficient;
            this->specularLightCoefficient = specularLightCoefficient;
            this->reflectionLightExponent = reflectionLightExponent;
        }

        GenericMaterial(double ambientLightCoefficient, double diffuseLightCoefficient, double specularLightCoefficient, double reflectionLightExponent, double reflectionCoefficient, double refractionCoefficient, double indexOfrefraction, double fuzz) 
        : fuzz(fuzz), reflectionCoefficient(reflectionCoefficient), refractionCoefficient(refractionCoefficient), indexOfrefraction(indexOfrefraction),  col(make_shared<SolidColor>(color(1.0, 1.0, 1.0))) {
            this->ambientLightCoefficient = ambientLightCoefficient;
            this->diffuseLightCoefficient = diffuseLightCoefficient;
            this->specularLightCoefficient = specularLightCoefficient;
            this->reflectionLightExponent = reflectionLightExponent;
        }

        GenericMaterial(double ambientLightCoefficient, double diffuseLightCoefficient, double specularLightCoefficient, double reflectionLightExponent, double reflectionCoefficient, double refractionCoefficient, double indexOfrefraction, TexturePtr col) :   reflectionCoefficient(reflectionCoefficient), refractionCoefficient(refractionCoefficient), indexOfrefraction(indexOfrefraction), col(col) {
            this->ambientLightCoefficient = ambientLightCoefficient;
            this->diffuseLightCoefficient = diffuseLightCoefficient;
            this->specularLightCoefficient = specularLightCoefficient;
            this->reflectionLightExponent = reflectionLightExponent;
        }

        GenericMaterial(GenericMaterial &m) : reflectionCoefficient(m.reflectionCoefficient), refractionCoefficient(m.refractionCoefficient), indexOfrefraction(m.indexOfrefraction), col(m.col), fuzz(m.fuzz) {
            this->ambientLightCoefficient = m.ambientLightCoefficient;
            this->diffuseLightCoefficient = m.diffuseLightCoefficient;
            this->specularLightCoefficient = m.specularLightCoefficient;
            this->reflectionLightExponent = m.reflectionLightExponent;
        }

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, color& attenuation, Ray& scattered, bool &isLight) const override {
            double randomCoefficient = randomDouble();

            // small hack to force a the sphere background to not infinetely accumulate the same color when diffusing reflecting
            if(reflectionCoefficient < 0.01 && refractionCoefficient < 0.01  && indexOfrefraction < 0.01) {
                attenuation = col->value(rec.uv, rec.p);
                return false;
            }

            if(randomCoefficient < reflectionCoefficient) {
                MaterialPtr matPtr = make_shared<MetalMaterial>(col, fuzz);
                return matPtr->scatter(rIn, rec, attenuation, scattered, isLight);
            }else if(randomCoefficient < reflectionCoefficient + refractionCoefficient) {
                MaterialPtr matPtr = make_shared<DialectricMaterial>(indexOfrefraction, fuzz);
                return matPtr->scatter(rIn, rec, attenuation, scattered, isLight);
            }else{
                MaterialPtr matPtr = make_shared<LambertianMaterial>(col);
                return matPtr->scatter(rIn, rec, attenuation, scattered, isLight);
            }
            return true;
        }
};

typedef shared_ptr<GenericMaterial> GenericMaterialPtr;


#endif // !GENERIC_MATERIAL_HPP