#include "renderer.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>

using namespace std;

// Inicialização de variáveis estáticas
int Renderer::remainingRows = 0;

void Renderer::render(const SceneDescription& scene, const string& outputFile) {
    // Cria a câmera com os parâmetros da cena
    Camera camera(scene.lookFrom, scene.lookAt, scene.vUp, scene.vFov, 
                 scene.aspectRatio, scene.aperture, scene.distToFocus);
    
    // Aloca memória para a imagem
    color** img = new color*[scene.imgHeight];
    for(int i = 0; i < scene.imgHeight; i++) {
        img[i] = new color[scene.imgWidth];
        for(int j = 0; j < scene.imgWidth; j++) {
            img[i][j] = color(0, 0, 0);
        }
    }
    
    // Abre arquivo de saída
    ofstream output;
    output.open(outputFile);
    
    // Escreve cabeçalho PPM
    output << "P3\n" << scene.imgWidth << " " << scene.imgHeight << "\n255\n";
    
    // Inicializa contador de progresso
    remainingRows = scene.imgHeight;
    
    // Cria threads para renderização paralela
    vector<thread*> threads;
    printRemaining();
    
    // Divide o trabalho em lotes para paralelização
    int batchSize = scene.imgHeight / 20;
    for(int row = scene.imgHeight; row >= 0; row -= batchSize) {
        int from = row - batchSize < 0 ? 0 : row - batchSize;
        int to = row - 1 < 0 ? 0 : row - 1;
        
        thread* th = new thread(computeFor, from, to, img, scene.imgWidth, scene.imgHeight,
                               scene.samplesPerPixel, ref(camera), 
                               ref(scene.componentList), ref(scene.lights));
        threads.push_back(th);
    }
    
    // Aguarda todas as threads terminarem
    for(auto th : threads) {
        th->join();
        delete th;
    }
    
    // Escreve pixels no arquivo de saída (de cima para baixo)
    for(int row = scene.imgHeight - 1; row >= 0; --row) {
        for(int col = 0; col < scene.imgWidth; ++col) {
            outputColor(output, img[row][col], scene.samplesPerPixel);
        }
    }
    
    cerr << "\nConcluído.\n";
    
    // Libera memória
    for(int i = 0; i < scene.imgHeight; i++) {
        delete[] img[i];
    }
    delete[] img;
    
    output.close();
}

void Renderer::lightMultiplier(const Ray& r, HitRecord hr, const vector<Light>& lights,
                               v3& diffuseColor, v3& specularColor, 
                               const ComponentList& componentList) {
    p3 hitPoint = hr.p;
    MaterialPtr hitMat = hr.matPtr;
    
    v3 diffuseC = color(0, 0, 0);
    v3 specularC = color(0, 0, 0);
    
    // Itera sobre todas as luzes (pula a primeira que é a luz ambiente)
    for(size_t i = 1; i < lights.size(); i++) {
        Light light = lights[i];
        
        // Usa uma cópia local do ponto de hit para aplicar jitter
        p3 p = hitPoint;
        
        // Calcula vetor do ponto para o centro da luz
        vec3 lightDir = light.center - p;
        double distanceToLight = lightDir.length();
        
        // Aplica jitter para sombras suaves (em cópia local para não acumular)
        if(smoothShadow) {
            p = p + distanceToLight * (vec3::randomInUnitSphere() / 500);
            lightDir = light.center - p;
        }
        
        vec3 normalizedLightDir = lightDir.normalize();
        HitRecord hr2;
        
        // Verifica se há algum objeto bloqueando a luz (sombra)
        bool didHit = const_cast<ComponentList&>(componentList).hit(
            Ray(p, lightDir), 0.001, infinity, hr2, true
        );
        
        bool inShadow = true;
        if(!didHit) {
            inShadow = false;
        } else {
            double distanceToHit = (hr2.p - p).length();
            if(distanceToHit > distanceToLight) {
                inShadow = false;
            }
        }
        
        // Se não está na sombra, calcula contribuição da luz
        if(!inShadow) {
            LightMaterialPtr lightMat = light.matPtr;
            
            // Calcula atenuação baseada na distância
            double attenuation = 1.0 / (
                lightMat->lightAttenuationConstant + 
                distanceToLight * lightMat->lightAttenuationLinear + 
                lightMat->lightAttenuationQuadratic * pow(distanceToLight, 2)
            );
            
            // Adiciona componente difusa
            diffuseC += (hitMat->diffuseLightCoefficient * attenuation * lightMat->col);
            
            // Adiciona componente especular
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

color Renderer::rayColor(const Ray& r, const ComponentList& componentList,
                         const vector<Light>& lights, int maxDep, color bg) {
    // Limite de profundidade de recursão atingido
    if(maxDep <= 0) {
        return color(1, 1, 1);
    }
    
    HitRecord hr;
    if(const_cast<ComponentList&>(componentList).hit(r, 0.001, infinity, hr, maxDep < maxDepth)) {
        Ray scattered;
        color attenuation;
        bool isLight = false;
        
        // Calcula iluminação (difusa e especular)
        v3 diffuseColor, specularColor;
        lightMultiplier(r, hr, lights, diffuseColor, specularColor, componentList);
        
        // Calcula luz ambiente
        v3 ambientLight = (hr.matPtr->ambientLightCoefficient * lights[0].matPtr->col).sqrtv();
        
        // Se o material espalha o raio (reflexão/refração)
        if(hr.matPtr->scatter(r, hr, attenuation, scattered, isLight)) {
            // Recursivamente traça o raio espalhado
            vec3 target = rayColor(scattered, componentList, lights, maxDep - 1, bg);
            return attenuation * target * (diffuseColor + ambientLight) + specularColor;
        } else {
            return attenuation * (diffuseColor + ambientLight) + specularColor;
        }
    } else {
        // Não acertou nada, retorna cor de fundo
        return bg;
    }
}

void Renderer::computeFor(int rowFrom, int rowTo, color** img, int imgWidth, int imgHeight,
                         int samplesPerPixel, const Camera& camera,
                         const ComponentList& componentList, const vector<Light>& lights) {
    // Renderiza cada pixel do intervalo de linhas atribuído
    for(int row = rowFrom; row <= rowTo; row++) {
        for(int col = 0; col < imgWidth; ++col) {
            color pixelColor(0, 0, 0);
            
            // Anti-aliasing: múltiplas amostras por pixel
            for(int s = 0; s < samplesPerPixel; ++s) {
                auto u = double(col + randomDouble()) / (imgWidth - 1);
                auto v = double(row + randomDouble()) / (imgHeight - 1);
                
                // Lança um raio para este pixel
                Ray r = const_cast<Camera&>(camera).getRay(u, v);
                
                // Cor de fundo (cinza)
                color background = color(0.31, 0.31, 0.31);
                color c = rayColor(r, componentList, lights, maxDepth, background);
                pixelColor += c;
            }
            img[row][col] += pixelColor;
        }
        
        // Atualiza progresso
        remainingRows--;
        printRemaining();
    }
}

void Renderer::printRemaining() {
    cerr << "\rLinhas restantes: " << remainingRows << ' ' << flush;
}
