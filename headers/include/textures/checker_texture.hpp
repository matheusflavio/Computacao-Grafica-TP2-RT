#ifndef CHECKER_TEXTURE_HPP
#define CHECKER_TEXTURE_HPP

#include "texture.hpp"

class CheckerTexture : public Texture {
    private:
        TexturePtr odd;
        TexturePtr even;
        double scale = 8.0;

    public:
        CheckerTexture() {}
        CheckerTexture(const TexturePtr& odd, const TexturePtr& even) : odd(odd), even(even) {}
        CheckerTexture(const color& odd, const color& even) : CheckerTexture(make_shared<SolidColor>(odd), make_shared<SolidColor>(even)) {}

        
        void setSize(double s) {
            scale = pi / s;
        }

        virtual color value(vec2 uv, const p3& p) const override {
            double sines = sin(scale * p.x()) * sin(scale * p.y()) * sin(scale * p.z());
            return (sines < 0 ? odd : even)->value(uv, p);
        }
};
typedef shared_ptr<CheckerTexture> CheckerTexturePtr;

#endif // !CHECKER_TEXTURE_HPP