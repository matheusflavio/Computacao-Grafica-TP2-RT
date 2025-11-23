#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "common.hpp"

class Texture {
    public:
        virtual vec3 value(vec2 uv, const p3& p) const = 0;
};
typedef shared_ptr<Texture> TexturePtr;

#endif // !TEXTURE_HPP