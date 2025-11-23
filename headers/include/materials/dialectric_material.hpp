#ifndef DIALECTRIC_MATERIAL_HPP
#define DIALECTRIC_MATERIAL_HPP

class DialectricMaterial : public Material {
    public:
        double indexOfrefraction;
        double fuziness = 0;
        TexturePtr col = nullptr;

        DialectricMaterial(double indexOfrefraction) : indexOfrefraction(indexOfrefraction) {}
        DialectricMaterial(double indexOfrefraction, double fuziness) : indexOfrefraction(indexOfrefraction), fuziness(fuziness) {}
        DialectricMaterial(double indexOfrefraction, TexturePtr col) : indexOfrefraction(indexOfrefraction), col(col) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, color& attenuation, Ray& scattered, bool &isLight) const override {
            attenuation = col == nullptr ? color(1.0, 1.0, 1.0) : col->value(rec.uv, rec.p);
            double refractionRatio = rec.rayComingFromOutside ? (1.0 / indexOfrefraction) : (indexOfrefraction);

            vec3 unitDirection = rIn.direction().normalize();
            double cosTheta = fmin((-unitDirection).dot(rec.normal), 1.0);
            double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

            // There isn't always a solution to Snell's law. So if a solution doesn't exist, the solid doesn't refract at all, therefore it reflects
            bool cannotRefract = refractionRatio * sinTheta > 1.0;
            vec3 direction;

            if(cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble()) {
                direction = unitDirection.reflect(rec.normal);
            }else{
                direction = unitDirection.refract(rec.normal, refractionRatio);
            }

            scattered = Ray(rec.p, direction + fuziness * vec3::randomInUnitSphere());
            return true;
        }
    
    private:
        // Schlicl's approximation of Fresnel's equation for reflectance
        static double reflectance(double cosine, double refractionRatio) {
            double r0 = (1 - refractionRatio) / (1 + refractionRatio);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
};


#endif // !DIALECTRIC_MATERIAL_HPP