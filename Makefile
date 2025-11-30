# Arquivos que serão compilados
OBJS = main.cpp src/*.cpp

# Compilador utilizado
CC = g++

# Caminhos para arquivos de cabeçalho necessários para compilação
INCLUDE_PATHS = -I headers/include/ -I headers/include/textures/ -I /opt/local/include/ -I headers/include/objects/ -I headers/include/materials/ -I headers/include/vectors/

# Caminhos para bibliotecas necessárias para compilação
LIBRARY_PATHS = 

# Opções adicionais de compilação
COMPILER_FLAGS = 

# Especifica as bibliotecas que estamos linkando
LINKER_FLAGS = 

# Nome do executável
EXE_NAME = demo

# Este é o alvo que compila o executável
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -std=c++17 -o $(EXE_NAME)

# Limpa arquivos compilados
clean:
	rm -f $(EXE_NAME)
