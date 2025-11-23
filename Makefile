#Files that will be compiled
OBJS = main.cpp src/*.cpp

#Used compiler
CC = g++

#Path to aditional files needed to compile
INCLUDE_PATHS = -I headers/include/ -I headers/include/textures/ -I /opt/local/include/ -I headers/include/objects/ -I headers/include/materials/ -I headers/include/vectors/

#Path to libraries needed to compile
LIBRARY_PATHS = 

#Aditional compile options
COMPILER_FLAGS = 

#Specifies which libraries we're linking
LINKER_FLAGS = 

#Executable name
EXE_NAME = demo.o

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -std=c++17 -o $(EXE_NAME)