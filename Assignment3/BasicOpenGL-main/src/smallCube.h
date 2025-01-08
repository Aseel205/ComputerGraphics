#ifndef SMALLCUBE_H
#define SMALLCUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

class SmallCube {
    
    public:
    glm::vec3 position;                 // Current position in 3D space
    glm::mat4 modelMatrix;              // Model matrix for transformations
    int index ; 


    // Constructor
    SmallCube(const glm::vec3& pos , int index);
    SmallCube() ; 

    // Getters
    glm::vec3 getPosition() const;
    glm::mat4 getModelMatrix() const;   // Getter for the model matrix



    // Setters
    void setPosition(const glm::vec3& pos);
    void setModelMatrix(const glm::mat4& matrix); // Setter for the model matrix



};


#endif // SMALLCUBE_H

