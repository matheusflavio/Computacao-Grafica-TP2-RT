#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>

// Includes necessários para a estrutura
#include "vectors/vec3.hpp"
#include "camera.hpp"
#include "objects/light.hpp"
#include "component_list.hpp"
#include "textures/texture.hpp"
#include "materials/generic_material.hpp"

// Estrutura que agrupa todos os dados de uma cena para renderização
// Elimina a necessidade de variáveis globais
struct SceneDescription {
    // Parâmetros da câmera
    p3 lookFrom;
    p3 lookAt;
    vec3 vUp;
    double vFov;
    double aperture;
    double distToFocus;
    
    // Listas de componentes da cena
    std::vector<Light> lights;
    std::vector<TexturePtr> pigments;
    std::vector<std::shared_ptr<GenericMaterial>> materials;
    ComponentList componentList;
    
    // Parâmetros de renderização
    int imgWidth;
    int imgHeight;
    double aspectRatio;
    int samplesPerPixel;
    
    // Construtor com valores padrão
    SceneDescription() 
        : lookFrom(13, 2, 3),
          lookAt(0, 0, 0),
          vUp(0, 1, 0),
          vFov(50),
          aperture(0.00),
          distToFocus(10.0),
          imgWidth(800),
          imgHeight(600),
          aspectRatio(4.0 / 3.0),
          samplesPerPixel(15)
    {}
};

#endif
