#ifndef INPUT_PROCESSOR_HPP
#define INPUT_PROCESSOR_HPP

#include "scene.hpp"
#include <string>
#include <fstream>

// Classe responsável por processar arquivos de entrada e popular a SceneDescription
class InputProcessor {
public:
    // Processa um arquivo de entrada completo e retorna a descrição da cena
    static SceneDescription processFile(const std::string& filename);
    
private:
    // Métodos auxiliares para processar cada seção do arquivo
    static void parseCameraSettings(std::ifstream& file, SceneDescription& scene);
    static void parseLights(std::ifstream& file, SceneDescription& scene);
    static void parsePigments(std::ifstream& file, SceneDescription& scene);
    static void parseMaterials(std::ifstream& file, SceneDescription& scene);
    static void parseObjects(std::ifstream& file, SceneDescription& scene);
    static void parseFocusSettings(std::ifstream& file, SceneDescription& scene);
};

#endif
