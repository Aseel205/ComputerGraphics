#ifndef SCENE_READER_H
#define SCENE_READER_H
#include <string>
#include <vector>
#include "Scene.h"

class SceneReader {
public:
// Public members to hold the parsed data
Eye* eye;                  // Pointer to Eye object
Ambient* ambient;          // Pointer to Ambient object
std::vector<Object*> objects;
std::vector<LightSource*> lights;

// Constructor
SceneReader() : eye(nullptr), ambient(nullptr) {}

// Function to load scene data from a file
Scene* readScene(const std::string& filename);

void generateRays(Scene& scene) ; 


};

#endif // SCENE_READER_H
