#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include "C:\Users\aseel\OneDrive\Desktop\BasicOpenGL-main (new)\src\graphicsFilters.h"
#include <iostream>

int main(void)
{
    std::string filepath = "C:\\Users\\aseel\\OneDrive\\Desktop\\BasicOpenGL-main (new)\\Lenna.png";
    int width, height, comps;
    int req_comps = 4;
    unsigned char * buffer = stbi_load(filepath.c_str(), &width, &height, &comps, req_comps);
    int result = stbi_write_png("res/textures/new_plane.png", width, height, req_comps, buffer, width * comps);
    std::cout << result << std::endl;
    return 0;
}