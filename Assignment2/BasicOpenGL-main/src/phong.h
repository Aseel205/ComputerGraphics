#ifndef PHONG_H
#define PHONG_H

#include <glm/glm.hpp>
#include <vector>
#include <Scene.h>

class Phong
{
public:
    // phong model
    static glm::vec3 calcAmbientColor(Scene &scene, Intersection &hit);
    static glm::vec3 calcEmissionColor(Scene &scene);
    static glm::vec3 calcDiffuseColor(Scene &scene, Intersection &hit, LightSource *light);
    static glm::vec3 calcSpecularColor(Scene &scene, Intersection &hit, LightSource *light);
    static bool occluded(Scene &scene, Intersection &hit, LightSource *light) ; 

    // other static functions
    static glm::vec3 calcColor(Scene &scene, Ray &ray);
    static glm::vec3 checkerboardColor(glm::vec3 rgbColor, glm ::vec3 hitPoint);

    ;

private:
    // Add any private members or helper functions here, if necessary
};

#endif // PHONG_H
