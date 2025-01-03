#ifndef SMALLCUBE_H
#define SMALLCUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

class SmallCube {
private:
    int index;                          // Unique identifier for the cube
    glm::vec3 position;                 // Current position in 3D space
    glm::vec3 rotationAxis;             // Axis of rotation (if any)
    float rotationAngle;                // Current rotation angle
    std::array<glm::vec4, 6> faceColors; // Colors of the six faces

public:
    // Constructor
    SmallCube(int idx, const glm::vec3& pos);

    // Getters
    int getIndex() const;
    glm::vec3 getPosition() const;
    glm::vec3 getRotationAxis() const;
    float getRotationAngle() const;
    glm::vec4 getFaceColor(int faceIndex) const;

    // Setters
    void setPosition(const glm::vec3& pos);
    void setRotation(const glm::vec3& axis, float angle);
    void setFaceColor(int faceIndex, const glm::vec4& color);

    // Update position (e.g., after rotation)
    void updatePosition(const glm::mat4& transformation);
};

#endif // SMALLCUBE_H
