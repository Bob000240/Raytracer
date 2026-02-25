#include "data.h"
#include <cstdio>
#include <cstring>
#include <iostream>

std::string outputFileName(const std::string &name)
{
    size_t dot = name.find_last_of('.');
    if (dot == std::string::npos)
        return name + ".ppm";
    return name.substr(0, dot) + ".ppm";
}

Data::Data(const std::string &fname) : filename(fname)
{
    FILE *fp = std::fopen(filename.c_str(), "r");
    if (!fp)
    {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }

    char key[128];
    while (std::fscanf(fp, "%s", key) == 1)
    {
        if (std::strcmp(key, "eye") == 0)
        {
            double x, y, z;
            if (std::fscanf(fp, "%lf %lf %lf", &x, &y, &z) != 3)
                break;
            eye = {x, y, z};
        }
        else if (std::strcmp(key, "viewdir") == 0)
        {
            double x, y, z;
            if (std::fscanf(fp, "%lf %lf %lf", &x, &y, &z) != 3)
                break;
            viewdir = {x, y, z};
        }
        else if (std::strcmp(key, "updir") == 0)
        {
            double x, y, z;
            if (std::fscanf(fp, "%lf %lf %lf", &x, &y, &z) != 3)
                break;
            updir = {x, y, z};
        }
        else if (std::strcmp(key, "vfov") == 0)
        {
            if (std::fscanf(fp, "%d", &vfov) != 1)
                break;
        }
        else if (std::strcmp(key, "imsize") == 0)
        {
            int w, h;
            if (std::fscanf(fp, "%d %d", &w, &h) != 2)
                break;
            imsize = {w, h};
        }
        else if (std::strcmp(key, "bkgcolor") == 0)
        {
            double r, g, b;
            if (std::fscanf(fp, "%lf %lf %lf", &r, &g, &b) != 3)
                break;
            bkgcolor.setR(r).setG(g).setB(b);
        }
        else if (std::strcmp(key, "mtlcolor") == 0)
        {
            double dr, dg, db, sr, sg, sb;
            if (std::fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %d",
                            &dr, &dg, &db,
                            &sr, &sg, &sb,
                            &coefficients[0], &coefficients[1], &coefficients[2], &shininess) != 10)
                break;
            diffuseLight.setR(dr).setG(dg).setB(db);
            specularLight.setR(sr).setG(sg).setB(sb);
        }
        else if (std::strcmp(key, "sphere") == 0)
        {
            double cx, cy, cz, radius;
            if (std::fscanf(fp, "%lf %lf %lf %lf", &cx, &cy, &cz, &radius) != 4)
                break;
            spheres.push_back(sphere{cx, cy, cz, radius, diffuseLight, specularLight, coefficients, shininess});
        }
        else if (std::strcmp(key, "cone") == 0)
        {
            double cx, cy, cz, dx, dy, dz, angle, height;
            if (std::fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf %lf",
                            &cx, &cy, &cz, &dx, &dy, &dz, &angle, &height) != 8)
                break;
            cone.push_back(Cone{cx, cy, cz, dx, dy, dz, angle, height, diffuseLight, specularLight, coefficients, shininess});
        }
        else if (std::strcmp(key, "cylinder") == 0)
        {
            double cx, cy, cz, dx, dy, dz, radius, length;
            if (std::fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf %lf",
                            &cx, &cy, &cz, &dx, &dy, &dz, &radius, &length) != 8)
                break;
            cylinders.push_back(cylinder{cx, cy, cz, dx, dy, dz, radius, length, diffuseLight, specularLight, coefficients, shininess});
        }
        else if (std::strcmp(key, "light") == 0)
        {
            double x, y, z, intensity;
            int w;
            if (std::fscanf(fp, "%lf %lf %lf %d %lf", &x, &y, &z, &w, &intensity) != 5)
                break;
            lights.push_back(Light{x, y, z, w, intensity});
        }
        else
        {
            std::cout << "Unknown Command\n";
        }
    }

    std::fclose(fp);
}

std::string Data::getFilename() const { return filename; }