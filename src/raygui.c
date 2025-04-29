// raygui's implementation causes GCC's static analyzer to fail due to it being "too complex". As
// such, we compile it in this translation unit and exclude this from GCC's static analyzer.

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
