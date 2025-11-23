#include "input_processor.hpp"
#include "objects/sphere.hpp"
#include "objects/polyhedron.hpp"
#include <iostream>

using namespace std;

SceneDescription InputProcessor::processFile(const string& filename) {
    SceneDescription scene;
    ifstream inputFile(filename);
    
    if (!inputFile.is_open()) {
        cerr << "Erro: Não foi possível abrir o arquivo " << filename << endl;
        return scene;
    }
    
    // Processa cada seção do arquivo de entrada
    parseCameraSettings(inputFile, scene);
    parseLights(inputFile, scene);
    parsePigments(inputFile, scene);
    parseMaterials(inputFile, scene);
    parseObjects(inputFile, scene);
    parseFocusSettings(inputFile, scene);
    
    inputFile.close();
    return scene;
}

void InputProcessor::parseCameraSettings(ifstream& file, SceneDescription& scene) {
    string line;
    
    // Lê posição da câmera (lookFrom)
    getline(file, line);
    scene.lookFrom = p3(line);
    
    // Lê ponto para onde a câmera está olhando (lookAt)
    getline(file, line);
    scene.lookAt = p3(line);
    
    // Lê vetor "up" da câmera
    getline(file, line);
    scene.vUp = p3(line);
    
    // Lê campo de visão vertical (vFov)
    getline(file, line);
    scene.vFov = stod(line);
}

void InputProcessor::parseLights(ifstream& file, SceneDescription& scene) {
    string line;
    
    // Lê número de luzes
    getline(file, line);
    int numLights = stoi(line);
    
    // Processa cada luz
    for(int i = 0; i < numLights; i++) {
        getline(file, line);
        vector<string> lightDetails = split(line);
        
        // Posição da luz (x, y, z)
        p3 lightPos = p3(stod(lightDetails[0]), stod(lightDetails[1]), stod(lightDetails[2]));
        
        // Cor da luz (r, g, b)
        p3 lightColor = p3(stod(lightDetails[3]), stod(lightDetails[4]), stod(lightDetails[5]));
        
        // Coeficientes de atenuação da luz
        double attenuationConstant = stod(lightDetails[6]);  // Constante
        double attenuationLinear = stod(lightDetails[7]);    // Linear (proporcional à distância)
        double attenuationQuadratic = stod(lightDetails[8]); // Quadrático (proporcional ao quadrado da distância)
        
        LightMaterialPtr lightMaterial = make_shared<LightMaterial>(
            lightColor, attenuationConstant, attenuationLinear, attenuationQuadratic, true
        );
        scene.lights.push_back(Light(lightPos, lightMaterial));
    }
}

void InputProcessor::parsePigments(ifstream& file, SceneDescription& scene) {
    string line;
    
    // Lê número de pigmentos
    getline(file, line);
    int numPigments = stoi(line);
    
    // Processa cada pigmento
    for(int i = 0; i < numPigments; i++) {
        getline(file, line);
        vector<string> pigmentDetails = split(line);
        
        if(pigmentDetails[0] == "solid") {
            // Pigmento de cor sólida
            color pigmentColor = p3(stod(pigmentDetails[1]), stod(pigmentDetails[2]), stod(pigmentDetails[3]));
            scene.pigments.push_back(make_shared<SolidColor>(pigmentColor));
            
        } else if(pigmentDetails[0] == "textmap" || pigmentDetails[0] == "texmap") {
            // Textura de imagem mapeada
            string image = pigmentDetails[1];
            vector<string> coords;
            
            // Lê primeiro ponto de mapeamento (u, v, s, t)
            getline(file, line);
            coords = split(line);
            vec4 tmP0 = vec4(stod(coords[0]), stod(coords[1]), stod(coords[2]), stod(coords[3]));
            
            // Lê segundo ponto de mapeamento
            getline(file, line);
            coords = split(line);
            vec4 tmP1 = vec4(stod(coords[0]), stod(coords[1]), stod(coords[2]), stod(coords[3]));
            
            scene.pigments.push_back(make_shared<ImageTexturePs>(image.c_str(), tmP0, tmP1));
            
        } else if(pigmentDetails[0] == "checker") {
            // Padrão xadrez (checker)
            color c1 = p3(stod(pigmentDetails[1]), stod(pigmentDetails[2]), stod(pigmentDetails[3]));
            color c2 = p3(stod(pigmentDetails[4]), stod(pigmentDetails[5]), stod(pigmentDetails[6]));
            double size = stod(pigmentDetails[7]);
            
            CheckerTexturePtr checker = make_shared<CheckerTexture>(c1, c2);
            checker->setSize(size);
            scene.pigments.push_back(checker);
        }
    }
}

void InputProcessor::parseMaterials(ifstream& file, SceneDescription& scene) {
    string line;
    
    // Lê número de materiais
    getline(file, line);
    int numMaterials = stoi(line);
    
    // Processa cada material
    for(int i = 0; i < numMaterials; i++) {
        getline(file, line);
        vector<string> materialDetails = split(line);
        
        // Coeficientes do modelo de iluminação
        double ka = stod(materialDetails[0]);    // Coeficiente de luz ambiente
        double kd = stod(materialDetails[1]);    // Coeficiente de luz difusa
        double ks = stod(materialDetails[2]);    // Coeficiente de luz especular
        double alpha = stod(materialDetails[3]); // Expoente para reflexão especular (brilho)
        
        // Propriedades de reflexão e refração
        double kr = stod(materialDetails[4]);  // Coeficiente de reflexão
        double kt = stod(materialDetails[5]);  // Coeficiente de transmissão (refração)
        double ior = stod(materialDetails[6]); // Índice de refração
        
        // Parâmetro de "fuziness" (imperfeição) - opcional
        double fuziness = 0;
        if(materialDetails.size() > 7) {
            fuziness = stod(materialDetails[7]);
        }
        
        GenericMaterialPtr matPtr = make_shared<GenericMaterial>(ka, kd, ks, alpha, kr, kt, ior, fuziness);
        scene.materials.push_back(matPtr);
    }
}

void InputProcessor::parseObjects(ifstream& file, SceneDescription& scene) {
    string line;
    
    // Lê número de objetos
    getline(file, line);
    int numObjects = stoi(line);
    
    // Processa cada objeto
    for(int i = 0; i < numObjects; i++) {
        getline(file, line);
        vector<string> objectDetails = split(line);
        
        // Índices do pigmento e material a serem usados
        int pigmentIndex = stoi(objectDetails[0]);
        int materialIndex = stoi(objectDetails[1]);
        string objectType = objectDetails[2];
        
        // Cria material com o pigmento apropriado
        GenericMaterial mat = *scene.materials[materialIndex];
        mat.col = scene.pigments[pigmentIndex];
        GenericMaterialPtr matPtr = make_shared<GenericMaterial>(mat);
        
        if(objectType == "sphere") {
            // Esfera: centro (x, y, z) e raio
            p3 center = p3(stod(objectDetails[3]), stod(objectDetails[4]), stod(objectDetails[5]));
            double radius = stod(objectDetails[6]);
            scene.componentList.add(make_shared<Sphere>(center, radius, matPtr));
            
        } else if(objectType == "polyhedron") {
            // Poliedro: definido por múltiplas faces planas
            int numFaces = stoi(objectDetails[3]);
            PolyhedronPtr poly = make_shared<Polyhedron>(matPtr);
            
            // Lê cada face do poliedro (plano definido por ax + by + cz + d = 0)
            for(int j = 0; j < numFaces; j++) {
                getline(file, line);
                vector<string> faceDetails = split(line);
                
                double c1 = stod(faceDetails[0]);
                double c2 = stod(faceDetails[1]);
                double c3 = stod(faceDetails[2]);
                double c4 = stod(faceDetails[3]);
                Plane p = Plane(c1, c2, c3, c4);
                poly->addFace(p);
            }
            scene.componentList.add(poly);
        }
    }
}

void InputProcessor::parseFocusSettings(ifstream& file, SceneDescription& scene) {
    string line;
    
    // Lê configurações opcionais de abertura e distância focal
    if(getline(file, line)) {
        vector<string> focusDetails = split(line);
        if(focusDetails.size() >= 2) {
            scene.aperture = stod(focusDetails[0]);
            scene.distToFocus = stod(focusDetails[1]);
        }
    }
    
    // Ignora linhas adicionais no arquivo (se houver)
    while(getline(file, line)) {
        // Apenas consome linhas extras
    }
}
