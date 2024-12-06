#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "scene.h"
#include "SceneReader.h"

#define WIDTH 800
#define HEIGHT 800



    Scene* SceneReader::readScene(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open scene file.");
    }

    Eye* eye = nullptr;
    Ambient* ambient = nullptr;
    Scene* scene = nullptr;
    std::vector<LightSource*> lights;
    std::vector<Object*> objects;
    std::vector<Material> materials;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        char identifier;
        ss >> identifier;

        if (identifier == 'e') {
            glm::vec3 eyePos;
            float modeFlag;
            ss >> eyePos.x >> eyePos.y >> eyePos.z >> modeFlag;
            eye = new Eye(eyePos, static_cast<int>(modeFlag));
        } 
        else if (identifier == 'a') {
            glm::vec3 ambientColor;
            float unused;
            ss >> ambientColor.x >> ambientColor.y >> ambientColor.z >> unused;
            ambient = new Ambient(ambientColor);
        } 
       else if (identifier == 'd') {
          glm::vec3 direction;
            float flag;
            ss >> direction.x >> direction.y >> direction.z >> flag;

            if (flag == 1.0f) {
                // You need to have position, cutoff, and intensity for Spotlight
                glm::vec3 position(0.0f, 0.0f, 0.0f);  // Set a default position
                float cutoff = 0.9f;  // Set a default cutoff value
                glm::vec3 intensity(1.0f, 1.0f, 1.0f); // Set a default intensity

                lights.push_back(new Spotlight(position, cutoff, intensity));  // Pass all 3 parameters
            } else {
                lights.push_back(new DirectionalLight(direction, glm::vec3(1.0f)));
            }
        } 
        else if (identifier == 'p') { // Spotlight position
            glm::vec3 position;
            float cutoff;
            ss >> position.x >> position.y >> position.z >> cutoff;

            Spotlight* spotlight = nullptr;

            // Check if there's an existing spotlight; if not, create one
            if (!lights.empty() && dynamic_cast<Spotlight*>(lights.back())) {
                spotlight = static_cast<Spotlight*>(lights.back());
            } else {
                // Create a new spotlight with default intensity if none exists
                spotlight = new Spotlight(position , cutoff  , glm::vec3(1.0f) );
                lights.push_back(spotlight);
            }

            // Set spotlight properties
            spotlight->position = position;
            spotlight->cutoff = cutoff;
        }

        else if (identifier == 'i') {
            glm::vec3 intensity;
            float unused;
            ss >> intensity.x >> intensity.y >> intensity.z >> unused;

            if (lights.empty()) {
                throw std::runtime_error("Unexpected 'i' without a preceding light source.");
            }
            lights.back()->intensity = intensity;
        } 
        else if (identifier == 'o' || identifier == 'r' || identifier == 't') {
            glm::vec3 position;
            float size;
            ss >> position.x >> position.y >> position.z >> size;
            glm::vec4 v4 = glm::vec4(position.x, position.y, position.z, size);


            if(size >0)
                objects.push_back(new Sphere(position, size, Material())); // Temporary material
            else 
                objects.push_back(new Plane(v4 ,  Material())); // Temporary material

             


        } 
        else if (identifier == 'c') {
            glm::vec3 color;
            float shininess;
            ss >> color.x >> color.y >> color.z >> shininess;

            materials.emplace_back(color, shininess);
        }
    }

    // Assign materials to objects
    if (materials.size() != objects.size()) {
        throw std::runtime_error("Mismatch between objects and materials.");
    }
    for (size_t i = 0; i < objects.size(); ++i) {
        objects[i]->setMaterial(materials[i]);
    }

    // Build scene
    if (eye && ambient) {
        scene = new Scene(*eye, *ambient);
        for (auto* light : lights) {
            scene->addLight(light);
        }
        for (auto* obj : objects) {
            scene->addObject(obj);
        }
    }

    file.close();
    return scene;
}



    // void generateRays(Scene* scene )
void SceneReader:: generateRays(Scene& scene) {
    float aspect_ratio = float(WIDTH) / float(HEIGHT);  // 800/600 = 1.333
    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height;

    glm::vec3 horizontal(viewport_width, 0.0f, 0.0f);
    glm::vec3 vertical(0.0f, viewport_height, 0.0f);
    glm::vec3 lower_left_corner = scene.eye.position - horizontal / 2.0f - vertical / 2.0f - glm::vec3(0.0f, 0.0f, 1.0f);

    for (int j = 0; j < HEIGHT; ++j) {
        for (int i = 0; i < WIDTH; ++i) {
            float u = float(i) / float(WIDTH);
            float v = float(j) / float(HEIGHT);
            glm::vec3 direction = lower_left_corner + horizontal * u + vertical * v - scene.eye.position;
            scene.rays[j][i] = Ray(scene.eye.position, direction);
        }
    }
}









