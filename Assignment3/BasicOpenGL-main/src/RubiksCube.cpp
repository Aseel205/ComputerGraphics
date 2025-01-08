#include "RubiksCube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


RubiksCube::RubiksCube() 
    : locks{false, false, false, false, false, false} { // Initialize all faces to true
    generateSmallCubes();  // Automatically generate small cubes on creation
}

RubiksCube::~RubiksCube() {
    for (SmallCube* cube : smallCubes) {
        delete cube;
    }
}

void RubiksCube::generateSmallCubes() {
    cubesPosition.clear(); 
    cubesPosition.resize(3, std::vector<std::vector<SmallCube*>>(3, std::vector<SmallCube*>(3)));


    smallCubes.clear();

    int index = 0;
    const int offset = 1; // Offset to handle negative indices

    // Resize the cubesPosition vector to 3x3x3

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                glm::vec3 position(x, y, z); 
                SmallCube* smallCube = new SmallCube (position, index);
                
                // Update the model matrix for the cube
                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
                smallCube->setModelMatrix(modelMatrix);

                smallCubes.push_back(smallCube);

                // Use offset to index into cubesPosition
                cubesPosition[x + offset][y + offset][z + offset] = smallCube;

                if (index == 13) { 
                    this->centerCube = smallCube ; 
                }
                index++;
            }
        }
    }
}


std::vector<SmallCube*> RubiksCube::getSmallCubes(){

return this->smallCubes ; 

}



   
void RubiksCube::render(Shader& shader, VertexArray& va, IndexBuffer& ib, glm::mat4 proj, glm::mat4 view) {
    for (SmallCube* cube : smallCubes) {
        // Apply transformations (position, rotation) to each small cube


        glm::mat4 model = cube->getModelMatrix() ;    
        

       glm::mat4 mvp =  proj * view * model ;

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



void RubiksCube::rotateRightWall() {
    std::cout << "Rotating Right Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    // Get the position of the centerCube to keep the wall's rotation relative to it
    glm::vec3 centerPosition = this->centerCube->getPosition();

    for (int i = 0; i < smallCubes.size(); ++i) {
        SmallCube* cube = smallCubes[i];
        
        // Check if the cube is part of the right wall (relative to the centerCube)
        if (cube->getPosition().x == centerPosition.x - 1) { // Right of centerCube (considering +1)
            // Debug: Original position
            glm::vec3 originalPos = cube->getPosition();
            std::cout << "Original Position: " << originalPos.x << ", " << originalPos.y << ", " << originalPos.z << "\n";

            
            
            // Debug: New position
            std::cout << "New Position: " << cube->getPosition().x << ", " << cube->getPosition().y << ", " << cube->getPosition().z << "\n";
        }
    }

    // Locking condition check
    if (RotationAngle == std::abs(45)) {
        this->locks[0] = !this->locks[0];
    }
}


void RubiksCube::rotateLeftWall(){

        std::cout << "Rotating Left Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    // Get the position of the centerCube to keep the wall's rotation relative to it
    glm::vec3 centerPosition = this->centerCube->getPosition();

    for (int i = 0; i < smallCubes.size(); ++i) {
        SmallCube* cube = smallCubes[i];
        
        // Check if the cube is part of the right wall (relative to the centerCube)
        if (cube->getPosition().x == centerPosition.x + 1) { 

            glm::vec3 originalPos = cube->getPosition();
            std::cout << "Original Position: " << originalPos.x << ", " << originalPos.y << ", " << originalPos.z << "\n";

            // Step 2: Rotate around the X-axis by the specified angle
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(1.0f, 0.0f, 0.0f));
            cube->setModelMatrix(rotationMatrix * cube->getModelMatrix());

          
            // Debug: New position
            std::cout << "New Position: " << cube->getPosition().x << ", " << cube->getPosition().y << ", " << cube->getPosition().z << "\n";
        }
    }

     if (RotationAngle == std::abs(45)) {
        this->locks[1] = !this->locks[1];
    }
}



void RubiksCube::rotateUpWall() {
    std::cout << "Rotating Up Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

        // Get the position of the centerCube to keep the wall's rotation relative to it
    glm::vec3 centerPosition = this->centerCube->getPosition();

    for (int i = 0; i < smallCubes.size(); ++i) {
        SmallCube* cube = smallCubes[i];
        
        // Check if the cube is part of the down wall (relative to the centerCube)
        if (cube->getPosition().z == centerPosition.z + 1) { // Downward from centerCube (considering -1)
            // Debug: Original position
            glm::vec3 originalPos = cube->getPosition();
            std::cout << "Original Position: " << originalPos.x << ", " << originalPos.y << ", " << originalPos.z << "\n";

            // Rotate around the Z-axis (rotation axis stays the same)
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(0.0f, 0.0f, 1.0f));
            cube->setModelMatrix(rotationMatrix * cube->getModelMatrix());

            // Debug: New position
            std::cout << "New Position: " << cube->getPosition().x << ", " << cube->getPosition().y << ", " << cube->getPosition().z << "\n";
        }
    }

    // Locking condition check
    if (RotationAngle == std::abs(45)) {
        this->locks[2] = !this->locks[2];
    }
}


void RubiksCube::rotateDownWall() {
    std::cout << "Rotating Down Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    // Get the position of the centerCube to keep the wall's rotation relative to it
    glm::vec3 centerPosition = this->centerCube->getPosition();

    for (int i = 0; i < smallCubes.size(); ++i) {
        SmallCube* cube = smallCubes[i];
        
        // Check if the cube is part of the down wall (relative to the centerCube)
        if (cube->getPosition().z == centerPosition.z - 1) { // Downward from centerCube (considering -1)
            // Debug: Original position
            glm::vec3 originalPos = cube->getPosition();
            std::cout << "Original Position: " << originalPos.x << ", " << originalPos.y << ", " << originalPos.z << "\n";

            // Rotate around the Z-axis (rotation axis stays the same)
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(0.0f, 0.0f, 1.0f));
            cube->setModelMatrix(rotationMatrix * cube->getModelMatrix());

            // Debug: New position
            std::cout << "New Position: " << cube->getPosition().x << ", " << cube->getPosition().y << ", " << cube->getPosition().z << "\n";
        }
    }

    // Locking condition check
    if (RotationAngle == std::abs(45)) {
        this->locks[3] = !this->locks[3];
    }
}

void RubiksCube::rotateBackWall() {
    std::cout << "Rotating Back Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    // Get the position of the centerCube to keep the wall's rotation relative to it
    glm::vec3 centerPosition = this->centerCube->getPosition();

    for (int i = 0; i < smallCubes.size(); ++i) {
        SmallCube* cube = smallCubes[i];
        
        // Check if the cube is part of the back wall (relative to the centerCube)
        if (cube->getPosition().y == centerPosition.y - 1) { // Behind the centerCube (considering -1)
            // Debug: Original position
            glm::vec3 originalPos = cube->getPosition();
            std::cout << "Original Position: " << originalPos.x << ", " << originalPos.y << ", " << originalPos.z << "\n";

            // Rotate around the Y-axis (rotation axis stays the same)
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(0.0f, 1.0f, 0.0f));
            cube->setModelMatrix(rotationMatrix * cube->getModelMatrix());

            // Debug: New position
            std::cout << "New Position: " << cube->getPosition().x << ", " << cube->getPosition().y << ", " << cube->getPosition().z << "\n";
        }
    }

    // Locking condition check
    if (RotationAngle == std::abs(45)) {
        this->locks[4] = !this->locks[4];
    }
}


void RubiksCube::rotateFrontWall() {
    std::cout << "Rotating Front Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    // Get the position of the centerCube to keep the wall's rotation relative to it
    glm::vec3 centerPosition = this->centerCube->getPosition();

    for (int i = 0; i < smallCubes.size(); ++i) {
        SmallCube* cube = smallCubes[i];
        
        // Check if the cube is part of the front wall (relative to the centerCube)
        if (cube->getPosition().y == centerPosition.y + 1) { // In front of the centerCube (considering +1)
            // Debug: Original position
            glm::vec3 originalPos = cube->getPosition();
            std::cout << "Original Position: " << originalPos.x << ", " << originalPos.y << ", " << originalPos.z << "\n";

            // Rotate around the Y-axis (rotation axis stays the same)
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(0.0f, 1.0f, 0.0f));
            cube->setModelMatrix(rotationMatrix * cube->getModelMatrix());

            // Debug: New position
            std::cout << "New Position: " << cube->getPosition().x << ", " << cube->getPosition().y << ", " << cube->getPosition().z << "\n";
        }
    }

    // Locking condition check
    if (RotationAngle == std::abs(45)) {
        this->locks[5] = !this->locks[5];
    }
}












// Rotations :


// we are stucj here .. 

/*

void RubiksCube::rotateRightWall() {
    std::cout << "Rotating Right Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    std::vector<std::vector<SmallCube * >> newLayer(3, std::vector<SmallCube *>(3)); // Temporary layer for the rotated positions

  // Step 1: Translate, rotate, and then translate back
      // Step 1: Translate, rotate, and then translate back
    for (int y = 0; y < 3; ++y) {
        for (int z = 0; z < 3; ++z) {

            

            SmallCube* smallCube = cubesPosition[0][y][z]; // The right wall is at x = -1  
            glm::vec3 position = this->getPosition() ;   
                    
            // Step 1: Translate the cube so that the point of rotation is at the origin

            
            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), - position); // Translate by (-x1, -y1, -z1)

            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(1.0f, 0.0f, 0.0f));

            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), + position); 



            // Combine the transformations: First translate to the origin, then rotate, and finally translate back
            glm::mat4 finalMatrix =  translateBack *  rotationMatrix    * translationToOrigin;



            // Update the model matrix for the small cube
            smallCube->setModelMatrix(finalMatrix * smallCube->getModelMatrix());
        }
    }
    // Step2: update the lock
    if (RotationAngle == std::abs(45)) {
        this->locks[0] = !this->locks[0];
    }


    // Step3: if there is no lock then update the cubesPosition  
    if(this->locks[0] == false) {

        for (int y = 0; y < 3; ++y) {
            for (int z = 0; z < 3; ++z) {
                // Calculate the new indices after a 90-degree clockwise rotation
                int newY = z;
                int newZ = 2 - y;

                // Assign the rotated position to the new layer
                newLayer[newY][newZ] = cubesPosition[0][y][z]; // Assuming x = -1 corresponds to index 0
            }
        }

            // Write the rotated layer back to cubesPosition
        for (int y = 0; y < 3; ++y) {
            for (int z = 0; z < 3; ++z) {
                cubesPosition[0][y][z] = newLayer[y][z];
            }
        }

    }
}





void RubiksCube::rotateLeftWall(){

    std::cout << "Rotating Left Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

  
    std::vector<std::vector<SmallCube * >> newLayer(3, std::vector<SmallCube *>(3)); // Temporary layer for the rotated positions

  // Step 1: Translate, rotate, and then translate back
      // Step 1: Translate, rotate, and then translate back
    for (int y = 0; y < 3; ++y) {
        for (int z = 0; z < 3; ++z) {

            SmallCube* smallCube = cubesPosition[2][y][z]; // The left wall is at x = 1 
            glm::vec3 position = this->getPosition() ;   
                    
            // Step 1: Translate the cube so that the point of rotation is at the origin

            
            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), - position); // Translate by (-x1, -y1, -z1)

            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(1.0f, 0.0f, 0.0f));

            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), + position); 



            // Combine the transformations: First translate to the origin, then rotate, and finally translate back
            glm::mat4 finalMatrix =  translateBack *  rotationMatrix    * translationToOrigin;



            // Update the model matrix for the small cube
            smallCube->setModelMatrix(finalMatrix * smallCube->getModelMatrix());
        }
    }
    // Step2: update the lock
    if (RotationAngle == std::abs(45)) {
        this->locks[1] = !this->locks[1];
    }


    // Step3: if there is no lock then update the cubesPosition  
    if(this->locks[1] == false) {

        for (int y = 0; y < 3; ++y) {
            for (int z = 0; z < 3; ++z) {
                // Calculate the new indices after a 90-degree clockwise rotation
                int newY = z;
                int newZ = 2 - y;

                // Assign the rotated position to the new layer
                newLayer[newY][newZ] = cubesPosition[2][y][z]; // Read from x = 1 layer
            }
        }

        // Write the rotated layer back to cubesPosition
        for (int y = 0; y < 3; ++y) {
            for (int z = 0; z < 3; ++z) {
                cubesPosition[2][y][z] = newLayer[y][z]; // Write back to x = 1 layer
            }
        }  

    }

}
// Rotations

void RubiksCube::rotateUpWall(){


     std::cout << "Rotating Up Wall by " << (RotationAngle * rotation_direction) << " degrees\n";


    std::vector<std::vector<SmallCube * >> newLayer(3, std::vector<SmallCube *>(3)); // Temporary layer for the rotated positions

  // Step 1: Translate, rotate, and then translate back
      // Step 1: Translate, rotate, and then translate back
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y< 3; ++y) {

            SmallCube* smallCube = cubesPosition[x][y][2]; // 
            glm::vec3 position = this->getPosition() ;   
                    

            
            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), - position); // Translate by (-x1, -y1, -z1)
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), + position); 

            // Combine the transformations: First translate to the origin, then rotate, and finally translate back
            glm::mat4 finalMatrix =  translateBack *  rotationMatrix    * translationToOrigin;


            // Update the model matrix for the small cube
            smallCube->setModelMatrix(finalMatrix * smallCube->getModelMatrix());
        }
    }
    // Step2: update the lock
    if (RotationAngle == std::abs(45)) 
        this->locks[2] = !this->locks[2];
    


    // Step3: if there is no lock then update the cubesPosition  
    if(this->locks[2] == false) {

        for (int x = 0; x < 3; ++x) {
            for (int y = 0; y < 3; ++y) {
                // Calculate the new indices after a 90-degree clockwise rotation
                int newX = y;
                int newY = 2 - x;

                // Assign the rotated position to the new layer
                newLayer[newX][newY] = cubesPosition[x][y][2]; // Read from Z = 2 layer
            }
        }

    // Write the rotated layer back to cubesPosition
        for (int x = 0; x < 3; ++x) {
            for (int y = 0; y < 3; ++y) {
                cubesPosition[x][y][2] = newLayer[x][y]; // Write back to Z = 2 layer
            }
        }
    }
}


void RubiksCube::rotateDownWall(){

    
     std::cout << "Rotating Down Wall by " << (RotationAngle * rotation_direction) << " degrees\n";


    std::vector<std::vector<SmallCube * >> newLayer(3, std::vector<SmallCube *>(3)); // Temporary layer for the rotated positions

    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y< 3; ++y) {

            SmallCube* smallCube = cubesPosition[x][y][0]; // The down wall is at z = 0 
            glm::vec3 position = this->getPosition() ;   
                    

            
            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), - position); // Translate by (-x1, -y1, -z1)
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), + position); 

            // Combine the transformations: First translate to the origin, then rotate, and finally translate back
            glm::mat4 finalMatrix =  translateBack *  rotationMatrix    * translationToOrigin;


            // Update the model matrix for the small cube
            smallCube->setModelMatrix(finalMatrix * smallCube->getModelMatrix());
        }
    }

    // Step2: update the lock
    if (RotationAngle == std::abs(45)) 
        this->locks[3] = !this->locks[3];
    


    // Step3: if there is no lock then update the cubesPosition  
    if(this->locks[3] == false) {

        for (int x = 0; x < 3; ++x) {
            for (int y = 0; y < 3; ++y) {
                // Calculate the new indices after a 90-degree clockwise rotation
                int newX = y;
                int newY = 2 - x;

                // Assign the rotated position to the new layer
                newLayer[newX][newY] = cubesPosition[x][y][0]; // Read from Z = 0 layer
            }
        }

        // Write the rotated layer back to cubesPosition
        for (int x = 0; x < 3; ++x) {
            for (int y = 0; y < 3; ++y) {
                cubesPosition[x][y][0] = newLayer[x][y]; // Write back to Z = 0 layer
            }
        }
    }

}

void RubiksCube::rotateBackWall(){

      std::cout << "Rotating Back Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

    std::vector<std::vector<SmallCube * >> newLayer(3, std::vector<SmallCube *>(3)); // Temporary layer for the rotated positions

    for (int x = 0; x < 3; ++x) {
        for (int z = 0; z < 3; ++z) {

            SmallCube* smallCube = cubesPosition[x][0][z]; // The down wall is at z = 0 
            glm::vec3 position = this->getPosition() ;   
                    

            
            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), - position); // Translate by (-x1, -y1, -z1)
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), + position); 

            // Combine the transformations: First translate to the origin, then rotate, and finally translate back
            glm::mat4 finalMatrix =  translateBack *  rotationMatrix    * translationToOrigin;


            // Update the model matrix for the small cube
            smallCube->setModelMatrix(finalMatrix * smallCube->getModelMatrix());
        }
    }
    
    // Step2: update the lock
    if (RotationAngle == std::abs(45)) 
        this->locks[4] = !this->locks[4];
    


    // Step3: if there is no lock then update the cubesPosition  
    if(this->locks[4] == false) {

        for (int x = 0; x < 3; ++x) {
            for (int z = 0; z < 3; ++z) {
                // Calculate the new indices after a 90-degree clockwise rotation
                int newX = z;
                int newZ = 2 - x;

                // Assign the rotated position to the new layer
                newLayer[newX][newZ] = cubesPosition[x][0][z]; // Read from y = 0 layer
            }
        } 

    // Write the rotated layer back to cubesPosition
        for (int x = 0; x < 3; ++x) {
            for (int z = 0; z < 3; ++z) {
                cubesPosition[x][0][z] = newLayer[x][z]; // Write back to y = 0 layer
            }
        }

    }
}

void RubiksCube::rotateFrontWall(){


       std::cout << "Rotating Front Wall by " << (RotationAngle * rotation_direction) << " degrees\n";

   
    std::vector<std::vector<SmallCube * >> newLayer(3, std::vector<SmallCube *>(3)); // Temporary layer for the rotated positions

    for (int x = 0; x < 3; ++x) {
        for (int z = 0; z < 3; ++z) {

            SmallCube* smallCube = cubesPosition[x][2][z]; // The down wall is at z = 0 
            glm::vec3 position = this->getPosition() ;   
                    

            
            glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), - position); // Translate by (-x1, -y1, -z1)
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians((float)RotationAngle * rotation_direction), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), + position); 

            // Combine the transformations: First translate to the origin, then rotate, and finally translate back
            glm::mat4 finalMatrix =  translateBack *  rotationMatrix    * translationToOrigin;


            // Update the model matrix for the small cube
            smallCube->setModelMatrix(finalMatrix * smallCube->getModelMatrix());
        }
    }
    
    // Step2: update the lock
    if (RotationAngle == std::abs(45)) 
        this->locks[5] = !this->locks[5];
    


    // Step3: if there is no lock then update the cubesPosition  
    if(this->locks[5] == false) {

        for (int x = 0; x < 3; ++x) {
            for (int z = 0; z < 3; ++z) {
                // Calculate the new indices after a 90-degree clockwise rotation
                int newX = z;
                int newZ = 2 - x;

                // Assign the rotated position to the new layer
                newLayer[newX][newZ] = cubesPosition[x][2][z]; // Read from y = 2 layer
            }
        }

        // Write the rotated layer back to cubesPosition
        for (int x = 0; x < 3; ++x) {
            for (int z = 0; z < 3; ++z) {
                cubesPosition[x][2][z] = newLayer[x][z]; // Write back to y = 2 layer
            }
        }

    }

}

*/



// Locks : 

bool RubiksCube::canRotateRightWall() {
    return !(locks[2] || locks[3] || locks[4] || locks[5]); // All must be false
}

bool RubiksCube::canRotateLeftWall() {
    return !(locks[2] || locks[3] || locks[4] || locks[5]); // Same condition as RightWall
}

bool RubiksCube::canRotateUpWall() {
    return !(locks[0] || locks[1] || locks[4] || locks[5]); // All must be false
}

bool RubiksCube::canRotateDownWall() {
    return !(locks[0] || locks[1] || locks[4] || locks[5]); // Fix inconsistent condition
}

bool RubiksCube::canRotateBackWall() {
    return !(locks[0] || locks[1] || locks[2] || locks[3]); // All must be false
}

bool RubiksCube::canRotateFrontWall() {
    return !(locks[0] || locks[1] || locks[2] || locks[3]); // All must be false
}


// Arrows pressed : 


void RubiksCube::UpArrow() {

    for (SmallCube* cube : this->smallCubes) {                   
        // Create a translation matrix for moving up
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, this->sensitivity, 0.0f));

        // Apply the translation to the model matrix
        glm::mat4 updatedModelMatrix = translationMatrix * cube->getModelMatrix();

        // Set the updated model matrix
        cube->setModelMatrix(updatedModelMatrix);
    }
}



void RubiksCube::DownArrow(){   


     for (SmallCube* cube : this->smallCubes) {                   
        // Create a translation matrix for moving up
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, - this->sensitivity, 0.0f));

        // Apply the translation to the model matrix
        glm::mat4 updatedModelMatrix = translationMatrix * cube->getModelMatrix();

        // Set the updated model matrix
        cube->setModelMatrix(updatedModelMatrix);
    }
}


void RubiksCube::RightArrow(){

    for (SmallCube* cube : this->smallCubes) {                   
        // Create a translation matrix for moving up
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(this->sensitivity), 0.0f ,  0.0f));

        // Apply the translation to the model matrix
        glm::mat4 updatedModelMatrix = translationMatrix * cube->getModelMatrix();

        // Set the updated model matrix
        cube->setModelMatrix(updatedModelMatrix);

    }
}

glm::vec3 RubiksCube::getPosition(){

    return this->cubesPosition[1][1][1]->position ; 
}




void RubiksCube::LeftArrow(){

       for (SmallCube* cube : this->smallCubes) {                   
        // Create a translation matrix for moving up
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3((this->sensitivity), 0.0f ,  0.0f));

        // Apply the translation to the model matrix
        glm::mat4 updatedModelMatrix = translationMatrix * cube->getModelMatrix();

        // Set the updated model matrix
        cube->setModelMatrix(updatedModelMatrix);

    }

     
}
        // Set the updated matrix
   










