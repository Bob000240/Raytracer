#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include "scene_types.h"

std::string outputFileName(const std::string &name);

class Data
{
private:
    std::string filename;

public:
    std::vector<double> eye;
    std::vector<double> viewdir;
    std::vector<double> updir;
    int vfov = 0;
    std::vector<int> imsize;

    Color bkgcolor{0, 0, 0};
    Color diffuseLight{1, 1, 1};
    Color specularLight{1, 1, 1};
    std::vector<double> coefficients{1, 1, 1};
    int shininess = 0;
    std::vector<Light> lights;

    std::vector<sphere> spheres;
    std::vector<Cone> cone;
    std::vector<cylinder> cylinders;

    Data(const std::string &fname);
    std::string getFilename() const;
};

#endif