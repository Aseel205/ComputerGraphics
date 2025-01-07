#include "RubiksCube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


// Colors for each face of the cube
glm::vec4 red   = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);   // Red
glm::vec4 green = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);   // Green
glm::vec4 blue  = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);   // Blue
glm::vec4 yellow = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);  // Yellow
glm::vec4 white  = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);   // White
glm::vec4 orange = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);  // Orange


RubiksCube::RubiksCube() 
    : locks{true, true, true, true, true, true} { // Initialize all faces to true
    generateSmallCubes();  // Automatically generate small cubes on creation
}

RubiksCube::~RubiksCube() {
    // Destructor: Clean up any resources if needed (e.g., buffers or textures)
}

void RubiksCube::generateSmallCubes() {
    smallCubes.clear();

    int index = 0;
    const int offset = 1; // Offset to handle negative indices

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                glm::vec3 position(x, y, z); 
                SmallCube smallCube(position , index);

                // Update the model matrix for the cube
                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
                smallCube.setModelMatrix(modelMatrix);

                smallCubes.push_back(smallCube);

                cubesPosition[x + offset][y + offset][z + offset] =  smallCube;
                index ++ ; 
            }
        }
    }
}
   
void RubiksCube::render(Shader& shader, VertexArray& va, IndexBuffer& ib, glm::mat4 proj, glm::mat4 view) {
    int i = 0 ;
    for (SmallCube& cube : smallCubes) {
        // Apply transformations (position, rotation) to each small cube



        glm::mat4 model = cube.getModelMatrix() ;    
        

       glm::mat4 mvp = proj * view * model;

        glm::vec4 color = glm::vec4(
             1.0f,  // Red based on x position
             1.0f,  // Green based on y position
             1.0f,  // Blue based on z position
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


// here we are stuck .... 

void RubiksCube::rotateRightWall() {
    std::cout << "Rotating Right Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    for (int i = 0; i < smallCubes.size(); ++i) {
        if (smallCubes[i].getPosition().x  == -1 )  { // Approximate check for x == -1
            SmallCube& cube = smallCubes[i];

            // Debug: Original position
            glm::vec3 originalPos = cube.getPosition();
            std::cout << "Original Position: " << originalPos.x << ", " << originalPos.y << ", " << originalPos.z << "\n";

            // Step 2: Rotate around the X-axis by the specified angle
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(1.0f, 0.0f, 0.0f));
            cube.setModelMatrix(rotationMatrix * cube.getModelMatrix());

            // Normalize the position to avoid floating-point errors
            glm::vec3 newPosition = cube.getPosition();
            newPosition.x = round(newPosition.x);
            newPosition.y = round(newPosition.y);
            newPosition.z = round(newPosition.z);
            cube.setPosition(newPosition);

            // Debug: New position
            std::cout << "New Position: " << cube.getPosition().x << ", " << cube.getPosition().y << ", " << cube.getPosition().z << "\n";
        }
    }

    if(RotationAngle == std :: abs (45)) {
        this->locks[0] =  !  this->locks[0] ;      
    }


}

void RubiksCube::rotateLeftWall(){

        std::cout << "Rotating Left Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    for (int i = 0; i < smallCubes.size(); ++i) {
        if (smallCubes[i].getPosition().x  == 1 )  { // Approximate check for x == -1
            SmallCube& cube = smallCubes[i];

            // Debug: Original position
            glm::vec3 originalPos = cube.getPosition();
            std::cout << "Original Position: " << originalPos.x << ", " << originalPos.y << ", " << originalPos.z << "\n";

            // Step 2: Rotate around the X-axis by the specified angle
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(1.0f, 0.0f, 0.0f));
            cube.setModelMatrix(rotationMatrix * cube.getModelMatrix());

            // Normalize the position to avoid floating-point errors
            glm::vec3 newPosition = cube.getPosition();
            newPosition.x = round(newPosition.x);
            newPosition.y = round(newPosition.y);
            newPosition.z = round(newPosition.z);
            cube.setPosition(newPosition);

            // Debug: New position
            std::cout << "New Position: " << cube.getPosition().x << ", " << cube.getPosition().y << ", " << cube.getPosition().z << "\n";
        }
    }

     if(RotationAngle == std :: abs (45)) {
        this->locks[1] =  !  this->locks[1] ;      
    }



}

// Rotations

void RubiksCube::rotateUpWall(){


     std::cout << "Rotating Up Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    for (int i = 0; i < smallCubes.size(); ++i) {
        if (smallCubes[i].getPosition().z  == 1 )  { // Approximate check for x == -1
            SmallCube& cube = smallCubes[i];

            // Debug: Original position
            glm::vec3 originalPos = cube.getPosition();
            std::cout << "Original Position: " << originalPos.x << ", " << originalPos.y << ", " << originalPos.z << "\n";

            // Step 2: Rotate around the X-axis by the specified angle
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(0.0f, 0.0f, 1.0f));
            cube.setModelMatrix(rotationMatrix * cube.getModelMatrix());

            // Normalize the position to avoid floating-point errors
            glm::vec3 newPosition = cube.getPosition();
            newPosition.x = round(newPosition.x);
            newPosition.y = round(newPosition.y);
            newPosition.z = round(newPosition.z);
            cube.setPosition(newPosition);

            // Debug: New position
            std::cout << "New Position: " << cube.getPosition().x << ", " << cube.getPosition().y << ", " << cube.getPosition().z << "\n";
        }
    }

    if(RotationAngle == std :: abs (45)) {
        this->locks[2] =  !  this->locks[2] ;      
    }


}

void RubiksCube::rotateDownWall(){

    std::cout << "Rotating Down Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    for (int i = 0; i < smallCubes.size(); ++i) {
        if (smallCubes[i].getPosition().z  == -1 )  { // Approximate check for x == -1
            SmallCube& cube = smallCubes[i];

            // Debug: Original position
            glm::vec3 originalPos = cube.getPosition();
            std::cout << "Original Position: " << originalPos.x << ", " << originalPos.y << ", " << originalPos.z << "\n";

            // Step 2: Rotate around the X-axis by the specified angle
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(0.0f, 0.0f, 1.0f));
            cube.setModelMatrix(rotationMatrix * cube.getModelMatrix());

            // Normalize the position to avoid floating-point errors
            glm::vec3 newPosition = cube.getPosition();
            newPosition.x = round(newPosition.x);
            newPosition.y = round(newPosition.y);
            newPosition.z = round(newPosition.z);
            cube.setPosition(newPosition);

            // Debug: New position
            std::cout << "New Position: " << cube.getPosition().x << ", " << cube.getPosition().y << ", " << cube.getPosition().z << "\n";
        }

    }

     if(RotationAngle == std :: abs (45)) {
        this->locks[3] =  !  this->locks[3] ;      
    }

}

void RubiksCube::rotateBackWall(){

      std::cout << "Rotating Back Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    for (int i = 0; i < smallCubes.size(); ++i) {
        if (smallCubes[i].getPosition().y  == -1 )  { // Approximate check for x == -1
            SmallCube& cube = smallCubes[i];

            // Debug: Original position
            glm::vec3 originalPos = cube.getPosition();
            std::cout << "Original Position: " << originalPos.x << ", " << originalPos.y << ", " << originalPos.z << "\n";

            // Step 2: Rotate around the X-axis by the specified angle
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(0.0f, 1.0f, 0.0f));
            cube.setModelMatrix(rotationMatrix * cube.getModelMatrix());

            // Normalize the position to avoid floating-point errors
            glm::vec3 newPosition = cube.getPosition();
            newPosition.x = round(newPosition.x);
            newPosition.y = round(newPosition.y);
            newPosition.z = round(newPosition.z);
            cube.setPosition(newPosition);

            // Debug: New position
            std::cout << "New Position: " << cube.getPosition().x << ", " << cube.getPosition().y << ", " << cube.getPosition().z << "\n";
        }
    }

    
     if(RotationAngle == std :: abs (45)) {
        this->locks[4] =  !  this->locks[4] ;      
    }

}

void RubiksCube::rotateFrontWall(){


       std::cout << "Rotating Front Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    for (int i = 0; i < smallCubes.size(); ++i) {
        if (smallCubes[i].getPosition().y  == 1 )  { // Approximate check for x == -1
            SmallCube& cube = smallCubes[i];

            // Debug: Original position
            glm::vec3 originalPos = cube.getPosition();
            std::cout << "Original Position: " << originalPos.x << ", " << originalPos.y << ", " << originalPos.z << "\n";

            // Step 2: Rotate around the X-axis by the specified angle
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(0.0f, 1.0f, 0.0f));
            cube.setModelMatrix(rotationMatrix * cube.getModelMatrix());

            // Normalize the position to avoid floating-point errors
            glm::vec3 newPosition = cube.getPosition();
            newPosition.x = round(newPosition.x);
            newPosition.y = round(newPosition.y);
            newPosition.z = round(newPosition.z);
            cube.setPosition(newPosition);

            // Debug: New position
            std::cout << "New Position: " << cube.getPosition().x << ", " << cube.getPosition().y << ", " << cube.getPosition().z << "\n";
        }
    }

    
     if(RotationAngle == std :: abs (45)) {
        this->locks[5] =  !  this->locks[5] ;      
    }

}



// Locks : 



bool RubiksCube:: canRotateRightWall() {
    return locks[2] && locks[3] && locks[4] && locks[5]  ; 
}
    bool RubiksCube:: canRotateLeftWall() {
    return locks[2] && locks[3] && locks[4] && locks[5]  ; 

}
    bool RubiksCube:: canRotateUpWall() {
    return locks[0] && locks[1] && locks[4] && locks[5]  ; 

}
    bool RubiksCube:: canRotateDownWall() {
    return locks[0] && locks[1] && locks[4] && locks[5]  ; 

}
    bool RubiksCube:: canRotateBackWall() {
    return locks[0] && locks[1] && locks[2] && locks[3]  ; 

}
    bool RubiksCube:: canRotateFrontWall() {
    return locks[0] && locks[1] && locks[2] && locks[3]  ; 
}


// Arrows pressed : 


void RubiksCube::UpArrow(){           

    
    for (SmallCube& cube : this->getSmallCubes()) {
        // Translation vector in homogeneous coordinates
        glm::vec4 translation(0.0f, this->sensitivity ,  0.0f, 1.0f);

        // Multiply the model matrix by the translation vector
        glm::vec4 newPosition = cube.getModelMatrix() * translation;

        // Update the model matrix to include the translation
        glm::mat4 updatedModelMatrix = cube.getModelMatrix();
        updatedModelMatrix[3] = newPosition; // Update the translation part of the matrix

        // Set the updated matrix
        cube.setModelMatrix(updatedModelMatrix);

    }
}

void RubiksCube::DownArrow(){   

     for (SmallCube& cube : this->getSmallCubes()) {
        // Translation vector in homogeneous coordinates
        glm::vec4 translation(0.0f, -(this->sensitivity) ,  0.0f, 1.0f);

        // Multiply the model matrix by the translation vector
        glm::vec4 newPosition = cube.getModelMatrix() * translation;

        // Update the model matrix to include the translation
        glm::mat4 updatedModelMatrix = cube.getModelMatrix();
        updatedModelMatrix[3] = newPosition; // Update the translation part of the matrix

        // Set the updated matrix
        cube.setModelMatrix(updatedModelMatrix);

    }

}


void RubiksCube::RightArrow(){

     for (SmallCube& cube : this->getSmallCubes()) {
        // Translation vector in homogeneous coordinates
        glm::vec4 translation(-(this->sensitivity), 0.0f, 0.0f, 1.0f);

        // Multiply the model matrix by the translation vector
        glm::vec4 newPosition = cube.getModelMatrix() * translation;

        // Update the model matrix to include the translation
        glm::mat4 updatedModelMatrix = cube.getModelMatrix();
        updatedModelMatrix[3] = newPosition; // Update the translation part of the matrix

        // Set the updated matrix
        cube.setModelMatrix(updatedModelMatrix);

    } 

}


void RubiksCube::LeftArrow(){

     for (SmallCube& cube : this->getSmallCubes()) {
        // Translation vector in homogeneous coordinates
        glm::vec4 translation((this->sensitivity), 0.0f, 0.0f, 1.0f);

        // Multiply the model matrix by the translation vector
        glm::vec4 newPosition = cube.getModelMatrix() * translation;

        // Update the model matrix to include the translation
        glm::mat4 updatedModelMatrix = cube.getModelMatrix();
        updatedModelMatrix[3] = newPosition; // Update the translation part of the matrix

        // Set the updated matrix
        cube.setModelMatrix(updatedModelMatrix);

    }

     
}








