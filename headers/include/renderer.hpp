#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "scene.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "objects/hittable.hpp"
#include "materials/light_material.hpp"
#include <string>

// Classe responsável por renderizar uma cena e gerar a imagem final
class Renderer {
public:
    // Renderiza a cena e salva no arquivo de saída especificado
    static void render(const SceneDescription& scene, const std::string& outputFile);
    
private:
    // Constantes de renderização
    static const int maxDepth = 14;        // Profundidade máxima de recursão do ray tracing
    static const bool smoothShadow = true; // Habilita sombras suaves
    
    // Variáveis de controle de progresso
    static int remainingRows;
    
    // Métodos auxiliares de renderização
    static color rayColor(const Ray& r, const ComponentList& componentList, 
                         const std::vector<Light>& lights, int maxDep, color bg);
    
    static void lightMultiplier(const Ray& r, HitRecord hr, 
                               const std::vector<Light>& lights,
                               v3& diffuseColor, v3& specularColor,
                               const ComponentList& componentList);
    
    static void computeFor(int rowFrom, int rowTo, color** img, int imgWidth, int imgHeight,
                          int samplesPerPixel, const Camera& camera, 
                          const ComponentList& componentList, const std::vector<Light>& lights);
    
    static void printRemaining();
};

#endif
