#include "input_processor.hpp"
#include "renderer.hpp"
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    // Valida argumentos mínimos da linha de comando
    if(argc < 3) {
        cerr << "Uso: " << argv[0] << " <arquivo_entrada> <arquivo_saida>" << endl;
        cerr << "Parâmetros opcionais: <largura> <altura> <amostras_por_pixel>" << endl;
        return -1;
    }
    
    // Processa argumentos (variáveis locais ao invés de globais)
    string inputFileName = argv[1];
    string outputFileName = argv[2];
    
    // Adiciona extensão .ppm se não estiver presente
    if(outputFileName.size() < 4 || outputFileName.substr(outputFileName.size() - 4) != ".ppm") {
        outputFileName += ".ppm";
    }
    
    // Parâmetros de renderização (valores padrão)
    int imgWidth = 800;
    int imgHeight = 600;
    int samplesPerPixel = 15;
    
    // Processa parâmetros opcionais
    if(argc >= 5) {
        imgWidth = atoi(argv[3]);
        imgHeight = atoi(argv[4]);
    }
    
    if(argc >= 6) {
        samplesPerPixel = atoi(argv[5]);
    }
    
    // Processa arquivo de entrada e carrega a descrição da cena
    cerr << "Processando arquivo de entrada...\n";
    SceneDescription scene = InputProcessor::processFile(inputFileName);
    
    // Aplica parâmetros customizados de renderização
    scene.imgWidth = imgWidth;
    scene.imgHeight = imgHeight;
    scene.aspectRatio = double(imgWidth) / double(imgHeight);
    scene.samplesPerPixel = samplesPerPixel;
    
    // Renderiza a cena e salva no arquivo de saída
    cerr << "Iniciando renderização...\n";
    cerr << "Resolução: " << imgWidth << "x" << imgHeight << endl;
    cerr << "Amostras por pixel: " << samplesPerPixel << endl;
    Renderer::render(scene, outputFileName);
    
    cerr << "Imagem salva em: " << outputFileName << endl;
    
    return 0;
}
