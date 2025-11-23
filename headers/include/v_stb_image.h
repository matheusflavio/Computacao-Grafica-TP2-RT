#ifndef V_STB_IMAGE_H
#define V_STB_IMAGE_H

// Wrapper para a biblioteca STB Image
// IMPORTANTE: STB_IMAGE_IMPLEMENTATION é definido em src/stb_image_impl.cpp
// para evitar múltiplas definições

// Disable pedantic warnings for this external library.
#ifdef _MSC_VER
    // Microsoft Visual C++ Compiler
    #pragma warning (push, 0)
#endif

// Apenas inclui as declarações, não as implementações
#include <stb_image.h>

// Restore warning levels.
#ifdef _MSC_VER
    // Microsoft Visual C++ Compiler
    #pragma warning (pop)
#endif

#endif