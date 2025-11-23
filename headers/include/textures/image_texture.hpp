#ifndef IMAGE_TEXTURE_HPP
#define IMAGE_TEXTURE_HPP

#include "../common.hpp"
#include "texture.hpp"
#include "v_stb_image.h"

using namespace std;

class ImageTexture : public Texture {

    private:
        unsigned char *data;
        int width, height;
        int bytesPerScanline;

    public: 
        const static int bytesPerPixel = 3;

        ImageTexture() : data(NULL), width(0), height(0), bytesPerScanline(0) {}

        ImageTexture(const char* filename) {
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

        ~ImageTexture() {
            delete data;
        }

        virtual color value(vec2 uv, const vec3& p) const override {
            if(data == NULL) return color(0, 0, 0);

            double u = clamp(uv.u(), 0.0, 1.0);
            // Flip v to image coordinates
            double v = 1.0 - clamp(uv.v(), 0.0, 1.0);

            int i = (int)(u * width);
            int j = (int)(v * height);
            
            if(i >= width) i = width - 1;
            if(j >= height) j = height - 1;

            const double colorScale = 1.0 / 255.0;
            auto pixel = data + j*bytesPerScanline + i*bytesPerPixel;

            return color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
        }

};
typedef shared_ptr<ImageTexture> ImageTexturePtr;

#endif // !IMAGE_TEXTURE_HPP