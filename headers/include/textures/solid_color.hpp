#ifndef SOLID_COLOR_HPP
#define TEXTURSSOLID_COLOR_HPPOLID_COLOR_HPPE_HPP

#include "texture.hpp"

class SolidColor : public Texture {
    private:
        color col;

    public:
        SolidColor() {}
        SolidColor(const color& c) : col(c) {}

        SolidColor(double red, double green, double blue) : SolidColor(color(red, green, blue)) {}

        virtual color value(vec2 uv, const p3& p) const override {
            return col;
        }
};
typedef shared_ptr<SolidColor> SolidColorPtr;

#endif // !TEXTURE_HPP