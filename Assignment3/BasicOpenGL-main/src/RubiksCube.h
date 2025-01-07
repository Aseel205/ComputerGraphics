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

    int  rotation_direction = 1  ;    // global varibale for rotation direction
    int RotationAngle = 90 ;    // global variable for rotation 
    float sensitivity =0.1;   //  for arrow pressing

    RubiksCube() ;  // Constructor 
    ~RubiksCube();  // Destructor

    void generateSmallCubes();  // Generate all the small cubes
    void render(Shader& shader, VertexArray& va, IndexBuffer& ib, glm::mat4 proj, glm::mat4 view);

    // Getter for small cubes (in case you need to access them directly)
    std::vector<SmallCube> getSmallCubes() { return smallCubes; }

    bool locks[6]; // 0: Front, 1: Back, 2: Left, 3: Right, 4: Up, 5: Down

    std::vector<std::vector<std::vector<SmallCube>>> cubesPosition ; 





    // Rotaions  
    void rotateRightWall() ; 
    void rotateLeftWall() ;
    void rotateUpWall() ;
    void rotateDownWall() ;
    void rotateBackWall() ;
    void rotateFrontWall() ; 

    // locks 
    bool canRotateRightWall() ; 
    bool canRotateLeftWall(); 
    bool canRotateUpWall() ;
    bool canRotateDownWall() ;
    bool canRotateBackWall() ;
    bool canRotateFrontWall() ;

    // arows 
    void UpArrow () ; 
    void DownArrow () ;
    void LeftArrow () ; 
    void RightArrow () ; 

 





private:
    std::vector<SmallCube> smallCubes;  // Store all the small cubes in the Rubik's Cube
    void updateSmallCubePosition(int index, const glm::vec3& position);
    void updateSmallCubeRotation(int index, const glm::mat4& rotationMatrix);
    void rotateLayer(int layerIndex, const glm::mat4& rotationMatrix);






};

#endif // RUBIKSCUBE_H
