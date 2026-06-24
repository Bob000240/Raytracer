#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <string>
#include "scene.h"
#include "camera.h"

struct LoadResult
{
    Scene scene;
    Camera camera;
    std::string outputFile;
    bool valid = false;
};

class SceneLoader
{
public:
    static LoadResult load(const std::string &filename);
};

#endif
