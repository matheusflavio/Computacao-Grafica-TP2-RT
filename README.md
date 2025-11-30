# Ray Tracing - Computação Gráfica UFMG

## Visão Geral

Este projeto implementa um **algoritmo de Ray Tracing Distribuído** desenvolvido como Trabalho Prático 2 da disciplina de Computação Gráfica da UFMG. O renderizador é capaz de gerar imagens fotorealísticas através de simulação física de raios de luz, suportando reflexão, refração, sombras suaves e diversos efeitos visuais avançados.

## Observação sobre os exemplos:
Para compilar os exemplos de renderização, foi solicitado ao ChatGPT para utilizar o modelo de input fornecido na especificação do TP para criar outros exemplos que respeitam as  regras de input para testar a implementação. Assim sendo, foram criados mais 7 novos inputs na pasta "inputs", havendo um total de 8 inputs ao contar com o input de exemplo da especificação.

A compilação do código e execução completa de todos os exemplos no meu notebook (Intel Core I5 6200U, 8GB de RAM DDR3) deu-se em 

A compilação do código e execução completa de todos os exemplos no computador mucuri do DCC deu-se em 

## Compilação e Execução

### Requisitos

- Compilador C++ com suporte a C++17 (g++ ou clang++)
- Sistema Linux/Unix

### Compilar

```bash
make
```

### Executar

```bash
./demo <arquivo_entrada> <arquivo_saida> [largura] [altura] [amostras_por_pixel]
```

**Parâmetros:**
- `arquivo_entrada`: Caminho para o arquivo .txt de entrada (obrigatório)
- `arquivo_saida`: Caminho para o arquivo .ppm de saída (obrigatório)
- `largura`: Largura da imagem em pixels (opcional, padrão: 800)
- `altura`: Altura da imagem em pixels (opcional, padrão: 600)
- `amostras_por_pixel`: Número de raios por pixel para anti-aliasing (opcional, padrão: 15)

**Exemplos:**

```bash
# Renderização rápida (baixa qualidade)
./demo inputs/input1.txt output.ppm 400 300 5

# Renderização de alta qualidade
./demo inputs/input1.txt output.ppm 1920 1080 200

# Usar configurações padrão
./demo inputs/input_focused.txt resultado.ppm
```

### Scripts Auxiliares

#### iterateAllInputs.sh

Script que renderiza automaticamente todos os arquivos de entrada da pasta `inputs/` e converte as imagens PPM para PNG usando ffmpeg:

```bash
chmod +x iterateAllInputs.sh
# Testar todos os exemplos
./iterateAllInputs.sh 
```

**O que o script faz:**
1. Remove compilações anteriores
2. Compila o projeto
3. Para cada arquivo .txt em `inputs/`:
   - Renderiza a imagem em formato PPM
   - Converte para PNG usando ffmpeg
4. Organiza as saídas em `exec_out/` (PPM) e `images_out/` (PNG)

## Estrutura do Projeto

O projeto foi organizado em módulos para melhor manutenibilidade:

```
RayTracing_CG/
├── headers/include/          # Arquivos de cabeçalho
│   ├── materials/            # Definições de materiais
│   ├── objects/              # Objetos renderizáveis (esferas, poliedros, luzes)
│   ├── textures/             # Texturas e pigmentos
│   ├── vectors/              # Classes de vetores (vec2, vec3, vec4)
│   ├── camera.hpp            # Sistema de câmera
│   ├── scene.hpp             # Estrutura da cena
│   ├── input_processor.hpp   # Processador de arquivos de entrada
│   └── renderer.hpp          # Motor de renderização
├── src/                      # Implementações
│   ├── input_processor.cpp   # Parsing de arquivos de entrada
│   ├── renderer.cpp          # Lógica de renderização
│   ├── stb_image_impl.cpp    # Implementação da biblioteca de imagens
│   ├── vec3.cpp              # Operações com vetores 3D
│   └── vec4.cpp              # Operações com vetores 4D
├── inputs/                   # Arquivos de entrada de exemplo
├── textures/                 # Texturas de imagem
├── main.cpp                  # Programa principal
├── Makefile                  # Sistema de compilação
└── iterateAllInputs.sh       # Script para renderizar todos os exemplos
```

### Características Principais

**Objetos Suportados:**
- Esferas
- Poliedros (definidos por planos)
- Fontes de luz pontuais

**Pigmentos (Texturas):**
- Cor sólida
- Padrão xadrez (checker)
- Texturas de imagem (mapeamento de textura)

**Materiais:**
- **Difusos** (Lambertian) - superfícies foscas
- **Reflexivos** (metais) - com parâmetro de imperfeição ajustável
- **Refrativos** (vidro, água) - com parâmetro de imperfeição ajustável
- **Mistos** - combinação de propriedades difusas, reflexivas e refrativas

### Módulos Principais

**1. SceneDescription (`scene.hpp`)**
- Estrutura que agrupa todos os dados da cena
- Elimina variáveis globais, melhorando organização do código
- Contém: câmera, luzes, materiais, pigmentos e lista de objetos

**2. InputProcessor (`input_processor.hpp/cpp`)**
- Responsável por processar arquivos de entrada
- Métodos especializados para cada seção:
  - `parseCameraSettings()` - Configuração da câmera
  - `parseLights()` - Luzes da cena
  - `parsePigments()` - Texturas e cores
  - `parseMaterials()` - Propriedades dos materiais
  - `parseObjects()` - Geometria da cena
  - `parseFocusSettings()` - Profundidade de campo

**3. Renderer (`renderer.hpp/cpp`)**
- Motor de renderização com paralelização via threads
- Implementa o algoritmo de ray tracing recursivo
- Funções principais:
  - `rayColor()` - Traça raios e calcula cores recursivamente
  - `lightMultiplier()` - Calcula iluminação (difusa e especular)
  - `computeFor()` - Renderiza um lote de linhas da imagem


## Formato do Arquivo de Entrada

O arquivo de entrada segue uma estrutura específica dividida em seções:

### 1. Configuração da Câmera
```
lookFrom_x lookFrom_y lookFrom_z       # Posição da câmera
lookAt_x lookAt_y lookAt_z             # Ponto para onde a câmera olha
vUp_x vUp_y vUp_z                      # Vetor "up" da câmera
vFov                                   # Campo de visão vertical (graus)
```

### 2. Luzes
```
num_luzes                              # Número de luzes
# Para cada luz:
pos_x pos_y pos_z cor_r cor_g cor_b att_const att_linear att_quad
```

**Atenuação da luz:**
- `att_const`: Atenuação constante
- `att_linear`: Atenuação linear (proporcional à distância)
- `att_quad`: Atenuação quadrática (proporcional ao quadrado da distância)

### 3. Pigmentos (Texturas)
```
num_pigmentos
# Cor sólida:
solid r g b

# Padrão xadrez:
checker r1 g1 b1 r2 g2 b2 tamanho

# Textura de imagem:
texmap arquivo_imagem.jpg
u1 v1 s1 t1                            # Primeiro ponto de mapeamento
u2 v2 s2 t2                            # Segundo ponto de mapeamento
```

### 4. Materiais
```
num_materiais
# Para cada material:
ka kd ks alpha kr kt ior [fuziness]
```

**Coeficientes:**
- `ka`: Luz ambiente
- `kd`: Luz difusa
- `ks`: Luz especular
- `alpha`: Expoente especular (brilho)
- `kr`: Reflexão
- `kt`: Transmissão (refração)
- `ior`: Índice de refração
- `fuziness`: Imperfeição (opcional, 0-1)

### 5. Objetos
```
num_objetos
# Esfera:
idx_pigmento idx_material sphere centro_x centro_y centro_z raio

# Poliedro:
idx_pigmento idx_material polyhedron num_faces
# Para cada face (plano ax + by + cz + d = 0):
a b c d
```

### 6. Profundidade de Campo (Opcional)
```
abertura distancia_foco
```

## Features Extras Implementadas

### 1. Sombras Suaves (Smooth Shadows)
Aplicadas automaticamente, adicionam realismo ao simular penumbra através de pequenas perturbações aleatórias nos raios de sombra.

**Comparação:**
- **Sombra Perfeita**: Bordas nítidas e artificiais
- **Sombra Suave**: Transição gradual, mais natural

### 2. Profundidade de Campo (Depth of Field)
Controla abertura e distância focal da câmera para simular efeito de desfoque.

**Exemplo:**
- `input_noaperture.txt`: Tudo em foco (abertura = 0)
- `input_focused.txt`: Desfoque de fundo (abertura > 0)

### 3. Reflexão Imperfeita
Adiciona rugosidade às superfícies reflexivas através do parâmetro `fuziness`.

**Exemplo:**
- `input_perfect_reflection.txt`: Reflexão especular perfeita (fuziness = 0)
- `input_imperfect_reflection.txt`: Reflexão com perturbações (fuziness > 0)

### 4. Refração Imperfeita
Similar à reflexão imperfeita, mas para materiais transparentes.

**Exemplo:**
- `input_perfect_refraction.txt`: Vidro perfeito
- `input_imperfect_refraction.txt`: Vidro fosco/rugoso

## Otimização de Desempenho

### Amostras por Pixel
O parâmetro `amostras_por_pixel` controla o equilíbrio entre qualidade e tempo de renderização:

- **5-10 amostras**: Renderização rápida para testes (~segundos)
- **50-100 amostras**: Qualidade intermediária (~minutos)
- **200-500 amostras**: Alta qualidade (~dezenas de minutos)

### Paralelização
O renderizador utiliza threads automáticas para dividir o trabalho de renderização entre múltiplos núcleos do processador, acelerando significativamente o processo.

## Limitações Conhecidas

- Formato de saída PPM (pode ser convertido para PNG/JPEG com ffmpeg para visualização mais amigável)
- Renderização pode ser demorada em alta qualidade
- Não suporta iluminação global completa (path tracing)

## Referências

- Peter Shirley - "Ray Tracing in One Weekend" series
- Especificação do Trabalho Prático - "Trabalho Prático - RT.pdf"
- Documentação da biblioteca STB Image
