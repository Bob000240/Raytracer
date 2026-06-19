#include <iostream>
#include "scene_loader.h"
#include "renderer.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <inputfile.txt>\n";
        return 1;
    }

    LoadResult result = SceneLoader::load(argv[1]);
    if (!result.valid)
        return 1;

    Renderer renderer(result.camera, result.scene);
    renderer.render(result.outputFile);
    return 0;
}
