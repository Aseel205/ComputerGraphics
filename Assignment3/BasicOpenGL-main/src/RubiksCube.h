#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <vector>
#include "SmallCube.h"
#include <glm/glm.hpp>
#include <Shader.h>
#include <VertexArray.h>
#include <IndexBuffer.h>

class RubiksCube {
public:
    RubiksCube();  // Constructor
    ~RubiksCube();  // Destructor

    void generateSmallCubes();  // Generate all the small cubes
    void render(Shader& shader, VertexArray& va, IndexBuffer& ib, glm::mat4 proj, glm::mat4 view);
    void rotateFace(int faceIndex, float angle);  // Rotate a specific face of the Rubik's Cube
    void applyRotationToSmallCubes();  // Apply transformations after a rotation is made

    // Getter for small cubes (in case you need to access them directly)
    std::vector<SmallCube>& getSmallCubes() { return smallCubes; }

    // Rotaions  
    void rotateRightWall() ; 



private:
    std::vector<SmallCube> smallCubes;  // Store all the small cubes in the Rubik's Cube
    void updateSmallCubePosition(int index, const glm::vec3& position);
    void updateSmallCubeRotation(int index, const glm::mat4& rotationMatrix);
    void rotateLayer(int layerIndex, const glm::mat4& rotationMatrix);
};

#endif // RUBIKSCUBE_H
