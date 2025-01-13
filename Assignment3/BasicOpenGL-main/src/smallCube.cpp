#include "smallCube.h"

// Constructor
SmallCube::SmallCube(const glm::vec3& pos, int index)
    : index(index), modelMatrix(glm::translate(glm::mat4(1.0f), pos)) ,   RotationMatrix(glm::mat4(1.0f)) {}
 

SmallCube::SmallCube()
    : index(0), modelMatrix(glm::mat4(1.0f)) {}

// Getter for position (calculated from modelMatrix)
glm::vec3 SmallCube::getPosition() const {
    return glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
}

// Getter for the model matrix
glm::mat4 SmallCube::getModelMatrix() const {
    return modelMatrix;
}

// Setter for position (updates modelMatrix)
void SmallCube::setPosition(const glm::vec3& pos) {
    modelMatrix[3][0] = pos.x;
    modelMatrix[3][1] = pos.y;
    modelMatrix[3][2] = pos.z;
}

// Setter for the model matrix (position derived from matrix)
void SmallCube::setModelMatrix(const glm::mat4 &matrix) {
    modelMatrix = matrix;
}

void SmallCube :: setRotationMatrix ( const glm::mat4 &matrix) {
        this->RotationMatrix = matrix ; 
}