#ifndef METAL_HPP
#define METAL_HPP

class MetalMaterial : public Material {
    public:
        TexturePtr col;
        double fuziness;
        MetalMaterial(const color& col, double fuziness) : col(make_shared<SolidColor>(col)), fuziness(fuziness < 1.0 ? fuziness : 1.0) {} 
        MetalMaterial(const TexturePtr& col, double fuziness) : col(col), fuziness(fuziness < 1.0 ? fuziness : 1.0) {} 

        virtual bool scatter(const Ray& rIn, const HitRecord& hr, color& attenuation, Ray& scattered, bool &isLight) const override {
            auto reflected = rIn.direction().normalize().reflect(hr.normal);
            scattered = Ray(hr.p, reflected + fuziness * vec3::randomInUnitSphere());
            attenuation = col->value(hr.uv, hr.p);

            // Avoid rays reflecting in the interior of the object
            // Uncommenting this line will make the code faster but will prevent refracted rays from being reflected inside the object
            // return (scattered.direction().sameDirection(hr.normal));
            return true;
        }
};

#endif // !METAL_HPP