#include "smallCube.h"

// Constructor
SmallCube::SmallCube(int idx, const glm::vec3& pos)
    : index(idx), position(pos), rotationAxis(glm::vec3(0.0f)), rotationAngle(0.0f) {
    faceColors.fill(glm::vec4(1.0f)); // Default color for all faces (white)
}

SmallCube::SmallCube()
{
}

// Getters
int SmallCube::getIndex() const {
    return index;
}

glm::vec3 SmallCube::getPosition() const {
    return position;
}

glm::vec3 SmallCube::getRotationAxis() const {
    return rotationAxis;
}

float SmallCube::getRotationAngle() const {
    return rotationAngle;
}

glm::vec4 SmallCube::getFaceColor(int faceIndex) const {
    if (faceIndex >= 0 && faceIndex < 6) {
        return faceColors[faceIndex];
    }
    return glm::vec4(0.0f); // Invalid index, return black
}

// Setters
void SmallCube::setPosition(const glm::vec3& pos) {
    position = pos;
}

void SmallCube::setRotation(const glm::vec3& axis, float angle) {
    rotationAxis = axis;
    rotationAngle = angle;
}

void SmallCube::setFaceColor(int faceIndex, const glm::vec4& color) {
    if (faceIndex >= 0 && faceIndex < 6) {
        faceColors[faceIndex] = color;
    }
}

// Update position (e.g., after a rotation)
void SmallCube::updatePosition(const glm::mat4& transformation) {
    glm::vec4 newPosition = transformation * glm::vec4(position, 1.0f);
    position = glm::vec3(newPosition);
}
