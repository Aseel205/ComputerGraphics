#include "smallCube.h"

// Constructor
SmallCube::SmallCube(const glm::vec3& pos , int index  )
    : index(index) , position(pos) {}

SmallCube::SmallCube()
{
}



glm::vec3 SmallCube::getPosition() const {
    return position;
}




glm::mat4 SmallCube::getModelMatrix() const
{
    return modelMatrix ;    // segma
}

// Setters


void SmallCube::setPosition(const glm::vec3& pos) {
    position = pos;
}


void SmallCube::setModelMatrix(const glm::mat4 &matrix) {

    this->modelMatrix = matrix;
    this->position = glm::vec3( round(matrix[3][0]),  round(matrix[3][1]), round(matrix[3][2]));

}

