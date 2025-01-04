#include "RubiksCube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

RubiksCube::RubiksCube() {
    generateSmallCubes();  // Automatically generate small cubes on creation
}

RubiksCube::~RubiksCube() {
    // Destructor: Clean up any resources if needed (e.g., buffers or textures)
}

void RubiksCube::generateSmallCubes() {
    smallCubes.clear();
    
    int index = 0; // Sequential index starting from 0
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                glm::vec3 position(x, y, z);
                SmallCube smallCube(index, position);  
                smallCubes.push_back(smallCube);
                ++index; // Increment index
            }
        }
    }
}
     
void RubiksCube::render(Shader& shader, VertexArray& va, IndexBuffer& ib, glm::mat4 proj, glm::mat4 view) {
    for (SmallCube& cube : smallCubes) {
        // Apply transformations (position, rotation) to each small cube


        glm::mat4 trans = glm::translate(glm::mat4(1.0f), (0.9f *  cube.getPosition()));
        glm::mat4 scl = glm::scale(glm::mat4(1.0f), glm::vec3(0.9f)); 

        // Aseel : I updated this funciton 
        glm::mat4 model = scl * trans;  // Apply scale before translation

       glm::mat4 mvp = proj * view * model;

        glm::vec4 color = glm::vec4(
             0.5f,  // Red based on x position
             0.5f,  // Green based on y position
             0.5f,  // Blue based on z position
            1.0f             // Alpha
        );

        // Set uniform variables for the shader
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniform1i("u_Texture", 0);
        shader.SetUniform4f("u_Color", color);

        // Render the small cube  
        va.Bind();
        ib.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
        shader.Unbind();
    }
}

void RubiksCube::rotateFace(int faceIndex, float angle) {
    // Determine the rotation axis based on the face index (simplified here)
    glm::vec3 rotationAxis;
    if (faceIndex == 0) rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f); // Front face
    else if (faceIndex == 1) rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f); // Right face
    else if (faceIndex == 2) rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f); // Top face

    // Apply rotation to the face
    for (SmallCube& cube : smallCubes) {
        // Check if cube is part of the rotating face
        if (abs(cube.getPosition().x) == 1 || abs(cube.getPosition().y) == 1 || abs(cube.getPosition().z) == 1) {
            cube.setRotation(rotationAxis, angle);
        }
    }

    // Apply this rotation to all small cubes involved in the face
    applyRotationToSmallCubes();
}

void RubiksCube::applyRotationToSmallCubes() {
    // Apply the rotation transformations to the small cubes based on their rotation angles
    for (SmallCube& cube : smallCubes) {
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(cube.getRotationAngle()), cube.getRotationAxis());
        updateSmallCubeRotation(cube.getIndex(), rotationMatrix);
    }
}

void RubiksCube::updateSmallCubePosition(int index, const glm::vec3& position) {
    // Update position of small cube by index
    smallCubes[index].setPosition(position);
}

void RubiksCube::updateSmallCubeRotation(int index, const glm::mat4& rotationMatrix) {
    // Apply rotation transformation to the small cube by index
    SmallCube& cube = smallCubes[index];
    glm::vec3 newPos = glm::vec3(rotationMatrix * glm::vec4(cube.getPosition(), 1.0f));
    cube.setPosition(newPos);
}

void RubiksCube::rotateLayer(int layerIndex, const glm::mat4& rotationMatrix) {
    // Apply rotation to a whole layer (row/column of cubes) based on the layer index
    for (SmallCube& cube : smallCubes) {
        // Apply rotation based on layer (you can implement more complex logic here)
        if (layerIndex == 0) {  // Example: Apply transformation to a specific layer of cubes
            glm::vec3 rotatedPos = glm::vec3(rotationMatrix * glm::vec4(cube.getPosition(), 1.0f));
            cube.setPosition(rotatedPos);
        }
    }
}
// Rotations :

void RubiksCube::rotateRightWall() {
    std::cout << "In rotateRightWall function.\n";

    for (int i = 0; i < smallCubes.size(); ++i) {
        if (smallCubes[i].getPosition().x == 1) { // Only right wall cubes
            SmallCube& cube = smallCubes[i];

            // Print the old position
            std::cout << "Before rotation - Cube Index: " << i 
                      << ", Position: (" << cube.getPosition().x 
                      << ", " << cube.getPosition().y 
                      << ", " << cube.getPosition().z << ")\n";

            // Step 1: Translate the cube to the origin (by subtracting (1, 0, 0))
            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

            // Step 2: Apply rotation around the Y-axis (45 degrees)
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            // Step 3: Translate the cube back to its original position (by adding (1, 0, 0))
            glm::mat4 translationBack = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            // Combine all transformations (translation -> rotation -> back translation)
            glm::mat4 transformation = translationBack * rotationMatrix * translationToOrigin;

            // Apply the transformation to the cube's position
            glm::vec3 newPos = glm::vec3(transformation * glm::vec4(cube.getPosition(), 1.0f));
            cube.setPosition(newPos);

            // Print the new position
            std::cout << "After rotation - Cube Index: " << i 
                      << ", New Position: (" << cube.getPosition().x 
                      << ", " << cube.getPosition().y 
                      << ", " << cube.getPosition().z << ")\n";
        }
    }
}

