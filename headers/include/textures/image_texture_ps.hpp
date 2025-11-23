#ifndef IMAGE_TEXTUREPS_HPP
#define IMAGE_TEXTUREPS_HPP

#include "texture.hpp"
#include "v_stb_image.h"
#include "vec4.hpp"

using namespace std;

/* Get the pixel value at (u, v) according to the inputs P0 and P1 (4 dimensions vectors) */
class ImageTexturePs : public Texture {

    private:
        unsigned char *data;
        int width, height;
        int bytesPerScanline;

        vec4 P0, P1;

    public: 
        const static int bytesPerPixel = 3;

        ImageTexturePs() : data(NULL), width(0), height(0), bytesPerScanline(0), P0(vec4()), P1(vec4()) {}

        ImageTexturePs(const char* filename, vec4 p0, vec4 p1) : P0(p0), P1(p1) {
            int componentsPerPixel = bytesPerPixel;

            data = stbi_load(filename, &width, &height, &componentsPerPixel, componentsPerPixel);
            if(!data) {
                cerr << "Could not load texture file " << filename << endl;
                height = 0;
                width = 0;
                return;
            }
            cout << filename << ": " << width << " " << height << endl;

            bytesPerScanline = bytesPerPixel * width;
        }

        ~ImageTexturePs() {
            delete data;
        }

        virtual color value(vec2 uv, const vec3& pc) const override {
            //  s = P0.PC e r = P1.PC.

            double u = abs(P0.dot(pc));
            double v = abs(P1.dot(pc));

            int i = (int)(u * width);
            int j = (int)(v * height);
            
            if(i >= width) i = width - 1;
            if(j >= height) j = height - 1;

            const double colorScale = 1.0 / 255.0;
            auto pixel = data + j*bytesPerScanline + i*bytesPerPixel;

            return color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
            
            return color(0, 0, 0);
        }

};
typedef shared_ptr<ImageTexturePs> ImageTexturePsPtr;

#endif // !IMAGE_TEXTUREPS_HPP