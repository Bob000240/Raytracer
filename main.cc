#include <iostream>
#include "raytracer.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <inputfile.txt>\n";
        return 1;
    }

    RayTracer trace(argv[1]);
    trace.renderShapes();
    return 0;
}