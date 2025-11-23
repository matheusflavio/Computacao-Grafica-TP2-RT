#include <iostream>
#include "color.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "common.hpp"
#include "component_list.hpp"
#include "camera.hpp"
#include "lambertian_material.hpp"
#include "metal_material.hpp"
#include "dialectric_material.hpp"
#include <thread>
#include <fstream>
#include "light.hpp"
#include "light_material.hpp"
#include "generic_material.hpp"
#include "polyhedron.hpp"

using namespace std;

char* inputFileName;
char* outputFileName;

color **img;
int imgWidth = 800;
double aspectRatio = 4.0 / 3.0;
int imgHeight = imgWidth / aspectRatio;
int samplesPerPixel = 15;
const int maxDepth = 14;
const bool smoothShadow = true;

int remainingRows = 0;

vector<TexturePtr> pigments;
vector<shared_ptr<GenericMaterial>> materials;

ComponentList componentList;
vector<Light> lights; 
p3 lookFrom(13, 2, 3);
p3 lookAt(0, 0, 0);
vec3 vUp = v3(0, 1, 0);
double vFov = 50;
double distToFocus = 10.0; //(lookFrom-lookAt).length();
auto aperture = 0.00;
Camera camera;

void lightMultiplier(const Ray& r, HitRecord hr, vector<Light> lights, v3& diffuseColor, v3& specularColor) {
    p3 p = hr.p;
    MaterialPtr hitMat = hr.matPtr;

    v3 diffuseC = color(0, 0, 0);
    v3 specularC = color(0, 0, 0);
    for (int i = 1; i < lights.size(); i++) {
        Light light  = lights[i];
        double distance;
        // get vector from p to light.center
        vec3 lightDir = light.center - p;
        double distanceToLight = lightDir.length();

        if(smoothShadow) {
            p = p +  distanceToLight * (vec3::randomInUnitSphere()/500);
            lightDir = light.center - p;
        }
        
        vec3 normalizedLightDir = lightDir.normalize();
        HitRecord hr2;
        bool didHit = componentList.hit(Ray(p, lightDir), 0.001, infinity, hr2, true);

        bool inShadow = true;

        if(!didHit){
            inShadow = false;
        }else{
            double distanceToHit = (hr2.p - p).length();
            if(distanceToHit > distanceToLight){
                inShadow = false;
            }
        }

        if(!inShadow) {
            LightMaterialPtr lightMat = light.matPtr;

            double attenuation = 1.0 /  (
                lightMat->lightAttenuationConstant + 
                    distanceToLight * lightMat->lightAttenuationLinear + 
                    lightMat->lightAttenuationQuadratic * pow(distanceToLight, 2)
            );

            diffuseC += (hitMat->diffuseLightCoefficient * attenuation * lightMat->col);

            v3 reflectionDirection = r.dir.normalize();
            double cosSpec = (normalizedLightDir - reflectionDirection).normalize().dot(hr.normal);
            if(cosSpec < 0.0000001) cosSpec = 0.0;
            cosSpec = pow(cosSpec, hitMat->reflectionLightExponent);
            specularC += (cosSpec * hitMat->specularLightCoefficient * attenuation * lightMat->col);            
        }
    }

    diffuseColor = diffuseC.sqrtv().sqrtv();
    specularColor = specularC;
}

color rayColor(const Ray& r, const ComponentList& componentList, int maxDep, color bg) {

    // ray bounce limit exceeded
    if(maxDep <= 0) {
        // don't change light
        return color(1, 1, 1);
    }

    HitRecord hr;
    if(componentList.hit(r, 0.001, infinity, hr, maxDep < maxDepth)) {
        Ray scattered;
        color attenuation;
        bool isLight = false;

        v3 diffuseColor, specularColor;
        lightMultiplier(r, hr, lights, diffuseColor, specularColor);
        v3 ambientLight = (hr.matPtr->ambientLightCoefficient * lights[0].matPtr->col).sqrtv();

        if(hr.matPtr->scatter(r, hr, attenuation, scattered, isLight)) {
            vec3 target = rayColor(scattered, componentList, maxDep - 1, bg);

            return attenuation * target * (diffuseColor + ambientLight) + specularColor;
        }else{
            return attenuation * (diffuseColor + ambientLight) + specularColor;
        }
    }else{
        return bg;
    }
}

void printRemaining() {
    cerr << "\rLines remaining: " << remainingRows << ' ' << flush;
}

void computeFor(int rowFrom, int rowTo){
    for(int row = rowFrom; row <= rowTo; row++) {
        for(int col = 0; col < imgWidth; ++col) {
            color pixelColor(0, 0, 0);
            for(int s = 0; s < samplesPerPixel; ++s) {
                auto u = double(col + randomDouble()) / (imgWidth-1);
                auto v = double(row + randomDouble()) / (imgHeight-1);
                // One ray for each pixel of the projection plane
                Ray r = camera.getRay(u, v);

                // color background = color(0.62, 0.72, 0.96);
                color background = color(0.31, 0.31, 0.31);
                color c = rayColor(r, componentList, maxDepth, background);
                pixelColor += c;
            }
            img[row][col] += pixelColor;
        }
        remainingRows --;
        printRemaining();
    }
}

bool validateArgs(int argc, char** argv) {
    if(argc < 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return false;
    }
    inputFileName = argv[1];
    outputFileName = argv[2];

    // if outputfile doesn't have .ppm extension, add it
    if(strcmp(outputFileName + strlen(outputFileName) - 4, ".ppm") != 0) {
        char* newOutputFile = new char[strlen(outputFileName) + 5];
        strcpy(newOutputFile, outputFileName);
        strcat(newOutputFile, ".ppm");
        outputFileName = newOutputFile;
    }
    cout << argc << endl;
    if(argc >= 5) {
        char* width = argv[3];
        char* height = argv[4];

        imgWidth = atoi(width);
        imgHeight = atoi(height);
    }

    if(argc >= 6) {
        samplesPerPixel = atoi(argv[5]);
    }

    return true;
}

void processInputFile(ifstream &inputFile) {
    string line;

    // Camera Inputs
    getline(inputFile, line);
    lookFrom = p3(line);

    getline(inputFile, line);
    lookAt = p3(line);

    getline(inputFile, line);
    vUp = p3(line);
    
    getline(inputFile, line);
    vFov = stod(line);

    // Lights Inputs
    getline(inputFile, line);
    int numLights = stoi(line);

    for(int i = 0; i < numLights; i++) {
        getline(inputFile, line);
        vector<string> lightDetails = split(line);
        p3 lightPos = p3(stod(lightDetails[0]), stod(lightDetails[1]), stod(lightDetails[2]));
        p3 lightColor = p3(stod(lightDetails[3]), stod(lightDetails[4]), stod(lightDetails[5]));
        double attenuationConstant = stod(lightDetails[6]);
        double attenuationLinear = stod(lightDetails[7]); // atennuation proportional to distance from light
        double attenuationQuadratic = stod(lightDetails[8]); // atennuation proportional to distance from light squared

        LightMaterialPtr lightMaterial = make_shared<LightMaterial>(lightColor, attenuationConstant, attenuationLinear, attenuationQuadratic, true);
        lights.push_back(Light(lightPos, lightMaterial));
    }
    
    // Pigments Inputs
    getline(inputFile, line);
    int numPigments = stoi(line);
    for(int i = 0; i < numPigments; i++) {
        getline(inputFile, line);
        vector<string> pigmentDetails = split(line);
        if(pigmentDetails[0] == "solid") {
            color pigmentColor = p3(stod(pigmentDetails[1]), stod(pigmentDetails[2]), stod(pigmentDetails[3]));
            pigments.push_back(make_shared<SolidColor>(pigmentColor));
        } else if(pigmentDetails[0] == "textmap" || pigmentDetails[0] == "texmap") {
            string image = pigmentDetails[1];
            vector<string> lightDetails;
            getline(inputFile, line); lightDetails = split(line);
            vec4 tmP0 = vec4(stod(lightDetails[0]), stod(lightDetails[1]), stod(lightDetails[2]), stod(lightDetails[3]));
            getline(inputFile, line); lightDetails = split(line);
            vec4 tmP1 = vec4(stod(lightDetails[0]), stod(lightDetails[1]), stod(lightDetails[2]), stod(lightDetails[3]));

            pigments.push_back(make_shared<ImageTexturePs>(image.c_str(), tmP0, tmP1));
        } else if(pigmentDetails[0] == "checker") {
            color c1 = p3(stod(pigmentDetails[1]), stod(pigmentDetails[2]), stod(pigmentDetails[3]));
            color c2 = p3(stod(pigmentDetails[4]), stod(pigmentDetails[5]), stod(pigmentDetails[6]));
            double size = stod(pigmentDetails[7]);

            CheckerTexturePtr checker = make_shared<CheckerTexture>(c1, c2);
            checker->setSize(size);
            pigments.push_back(checker);
        }
    }

    // Materials Inputs
    getline(inputFile, line);
    int numMaterials = stoi(line);
    for(int i = 0; i < numMaterials; i++) {
        getline(inputFile, line);
        vector<string> materialDetails = split(line);
        double ka = stod(materialDetails[0]); // environment light coefficient //TODO
        double kd = stod(materialDetails[1]); // kd diffuse light coefficient
        double ks = stod(materialDetails[2]); // specular light coefficient
        double alpha = stod(materialDetails[3]); // expoent for specular reflection

        double kr = stod(materialDetails[4]); // reflection coefficient
        double kt = stod(materialDetails[5]); // transmission coefficient
        double ior = stod(materialDetails[6]); // index of refraction

        double fuziness = 0;
        if(materialDetails.size() > 7) {
            fuziness = stod(materialDetails[7]); // index of refraction
        }

        GenericMaterialPtr matPtr = make_shared<GenericMaterial>(ka, kd, ks, alpha, kr, kt, ior, fuziness);
        materials.push_back(matPtr);
    }

    // Objects Inputs
    getline(inputFile, line);
    int numObjects = stoi(line);
    for(int i = 0; i < numObjects; i++) {
        getline(inputFile, line);
        vector<string> objectDetails = split(line);
        int pigmentIndex = stoi(objectDetails[0]);
        int materialIndex = stoi(objectDetails[1]);
        string objectType = objectDetails[2];

        GenericMaterial mat =  *materials[materialIndex];
        mat.col = pigments[pigmentIndex];
        GenericMaterialPtr matPtr = make_shared<GenericMaterial>(mat);

        if(objectType == "sphere") {
            p3 center = p3(stod(objectDetails[3]), stod(objectDetails[4]), stod(objectDetails[5]));
            double radius = stod(objectDetails[6]);
            componentList.add(make_shared<Sphere>(center, radius, matPtr));
        }else if(objectType == "polyhedron") {
            int numFaces = stoi(objectDetails[3]);
            PolyhedronPtr poly = make_shared<Polyhedron>(matPtr);
            for(int i = 0; i < numFaces; i++) {
                getline(inputFile, line);
                vector<string> faceDetails = split(line);

                double c1 = stod(faceDetails[0]);
                double c2 = stod(faceDetails[1]);
                double c3 = stod(faceDetails[2]);
                double c4 = stod(faceDetails[3]);
                Plane p = Plane(c1, c2, c3, c4);
                poly->addFace(p);
            }
            componentList.add(poly);
        }
    }

    if(getline(inputFile, line)) {
        vector<string> objectDetails = split(line);
        aperture = stod(objectDetails[0]);
        distToFocus = stod(objectDetails[1]);
    }

    camera = Camera(lookFrom, lookAt, vUp, vFov, aspectRatio, aperture, distToFocus);

    while (getline(inputFile, line)){
        cout << line << endl;
    }
}

int main(int argc, char** argv) {

    if(!validateArgs(argc, argv))
        return -1;

    remainingRows = imgHeight;
    aspectRatio = (double)imgWidth / (double)imgHeight;

    // create file
    ofstream outputFile;
    ifstream inputFile(inputFileName);
    outputFile.open(outputFileName);

    if (inputFile.is_open()) {
        processInputFile(inputFile);
        inputFile.close();
    }else{
        cout << "Unable to open file" << endl;
        inputFile.close();
        return -1;
    }

    img = new color*[imgHeight];
    for(int i = 0; i < imgHeight; i++) {
        img[i] = new color[imgWidth];
        for(int j = 0; j < imgWidth; j++) {
            img[i][j] = color(0, 0, 0);
        }
    }

    MaterialPtr diffuseGround = make_shared<LambertianMaterial>(color(0.27, 0.16, 0.1));
    MaterialPtr front = make_shared<LambertianMaterial>(color(0.3, 0.3, 0.7));
    MaterialPtr sides = make_shared<MetalMaterial>(color(0.8, 0.8, 0.8), 0.00);
    MaterialPtr top = make_shared<MetalMaterial>(color(0.14, 0.23, 0.8), 0.3);
    MaterialPtr glass = make_shared<DialectricMaterial>(1.5);

    // Render
    outputFile << "P3\n" << imgWidth << " " << imgHeight << "\n255\n";

    vector<thread*> threads;
    
    printRemaining();
    int batchSize = imgHeight / 20;
    for(int row = imgHeight; row >= 0; row -= batchSize) {
        int from = row-batchSize < 0 ? 0 : row-batchSize;
        int to = row - 1 < 0 ? 0 : row - 1;

        thread *th = new thread(computeFor, from, to);
        threads.push_back(th);
    }

    // join all threads
    for(auto th : threads) {
        th->join();
    }

    for(int row = imgHeight-1; row >= 0; --row) {
        for(int col = 0; col < imgWidth; ++col) {
            outputColor(outputFile, img[row][col], samplesPerPixel);
        }
    }
    cerr << "\nDone.\n";

    outputFile.close();
    return 0;
}
