#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "scene.h"
#include "SceneReader.h"
#include <bits/unique_ptr.h>
#include <bits/shared_ptr.h>
#define WIDTH 800
#define HEIGHT 800

Scene *SceneReader::readScene(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open scene file.");
    }

    Eye eye(glm::vec3(0.0f));
    Ambient ambient(glm::vec3(0.0f));
    std::vector<LightSource *> lights;
    std::vector<Object *> objects;

    std::string line;
    std::vector<glm::vec3> lightDirections, lightIntensities;
    std::vector<glm::vec4> spotlightPositions; // x, y, z, cutoff
    std::vector<Material> materials;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "e")
        { // Eye
            glm::vec3 position;
            float modeFlag;
            iss >> position.x >> position.y >> position.z >> modeFlag;
            eye = Eye(position, static_cast<int>(modeFlag));
        }
        else if (token == "a")
        { // Ambient
            glm::vec3 intensity;
            float ignore;
            iss >> intensity.x >> intensity.y >> intensity.z >> ignore;
            ambient = Ambient(intensity);
        }
        else if (token == "d")
        { // Light direction
            glm::vec3 direction;
            float type;
            iss >> direction.x >> direction.y >> direction.z >> type;
            lightDirections.push_back(direction);
        }
        else if (token == "p")
        { // Spotlight position
            glm::vec4 position;
            iss >> position.x >> position.y >> position.z >> position.w;
            spotlightPositions.push_back(position);
        }
        else if (token == "i")
        { // Light intensity
            glm::vec3 intensity;
            float ignore;
            iss >> intensity.x >> intensity.y >> intensity.z >> ignore;
            lightIntensities.push_back(intensity);
        }
        else if (token == "o" || token == "r" || token == "t")
        { // Objects
            glm::vec4 objData;
            iss >> objData.x >> objData.y >> objData.z >> objData.w;
            Material material;
            int status = (token == "o") ? 0 : (token == "r") ? 1
                                                             : 2;
            if (objData.w > 0)
            { // Sphere
                objects.push_back(new Sphere(status, glm::vec3(objData.x, objData.y, objData.z), objData.w));
            }
            else
            { // Plane
                objects.push_back(new Plane(status, objData));
            }
        }
        else if (token == "c")
        { // Materials
            glm::vec3 color;
            float shininess;
            iss >> color.x >> color.y >> color.z >> shininess;
            materials.emplace_back(color, shininess);
        }
    }

    // Associate materials with objects
    for (size_t i = 0; i < objects.size(); ++i)
    {
        if (i < materials.size())
        {
            objects[i]->setMaterial(materials[i]);
        }
    }

    // Create light sources
    size_t spotlightIndex = 0;
    for (size_t i = 0; i < lightDirections.size(); ++i)
    {
        glm::vec3 direction = lightDirections[i];
        glm::vec3 intensity = lightIntensities[i];
        if (i < spotlightPositions.size())
        { // Spotlight
            glm::vec4 position = spotlightPositions[spotlightIndex++];

            lights.push_back(new Spotlight(intensity, direction, position.w, glm::vec3(position.x, position.y, position.z)));
        }
        else
        { // Directional Light

            lights.push_back(new DirectionalLight(intensity, direction));
        }
    }

    // Create the scene
    Scene *scene = new Scene(eye, ambient);
    for (auto light : lights)
    {

        scene->addLight(light);
    }
    int id = 1;
    for (auto object : objects)
    {
        object->ObjectId = id;
        scene->addObject(object);
        id+=1;
    }

    return scene;
}

Ray SceneReader::ConstructRayThroughPixel(int x, int y, Scene &scene)
{
    // Map pixel (x, y) to normalized device coordinates (NDC)
    float ndc_x = (x + 0.5f) / WIDTH; // Add 0.5 to sample pixel center
    float ndc_y = (y + 0.5f) / HEIGHT;

    // Map NDC to screen coordinates (from -1 to 1 on both axes)
    float screen_x = -1.0f + 2.0f * ndc_x;
    float screen_y = -1.0f + 2.0f * ndc_y;

    // Compute the screen point on the z=0 plane
    glm::vec3 screen_point(screen_x, screen_y, 0.0f);

    // Compute the ray origin (camera position) and direction
    glm::vec3 origin = scene.eye.position; // Eye/camera position
    glm::vec3 direction = glm::normalize(screen_point - origin);

    return Ray(origin, direction);
}
