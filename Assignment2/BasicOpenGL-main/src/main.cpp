#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Debugger.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <Texture.h>
#include <Camera.h>

#include <iostream>
#include <../include/stb/stb_image.h>
#include <../include/stb/stb_image_write.h>
#include <SceneReader.h>


/* Window size */
const unsigned int width = 800;
const unsigned int height = 800;

void SaveImage(const std::vector<std::vector<std::vector<unsigned char>>>& imageArray, const std::string& imageName, const std::string& outputDirectory) {
    // Construct the full file path
    std::string filePath = outputDirectory + imageName;

    // Derive image dimensions from the input array
    int height = imageArray.size();                     // Number of rows
    int width = imageArray[0].size();                   // Number of columns
    int channels = imageArray[0][0].size();             // Number of color channels (e.g., 3 for RGB)

    // Create a buffer to hold the image data in a format suitable for saving
    unsigned char* buffer = new unsigned char[width * height * channels];

    // Copy image data to the buffer
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * channels;
            for (int c = 0; c < channels; ++c) {
                buffer[index + c] = imageArray[y][x][c];
            }
        }
    }

    // Save the image as a PNG file
    int result = stbi_write_png(filePath.c_str(), width, height, channels, buffer, width * channels);
    if (result) {
        std::cout << "Image saved successfully to " << filePath << std::endl;
    } else {
        std::cerr << "Failed to save the image to " << filePath << std::endl;
    }

    // Free the buffer after saving
    delete[] buffer;
}


int main(int argc, char* argv[]){

    std::string  filepath_input =  "C:\\Users\\aseel\\OneDrive\\Desktop\\computer graphics\\Assignment2\\BasicOpenGL-main\\src\\sample_scene1.txt" ;
    std::string filepath_outputImage = "C:\\Users\\aseel\\OneDrive\\Desktop\\computer graphics\\Assignment2\\BasicOpenGL-main\\src\\res\\";
    std::string outputImageName = "raytracing.png";
    std::vector<std::vector<std::vector<unsigned char>>> imageArray(800, std::vector<std::vector<unsigned char>>(800, std::vector<unsigned char>(3)));

      // Create SceneReader object and read the scene from the file
    SceneReader reader;
    Scene* scene = reader.readScene(filepath_input);
    reader.generateRays(*scene); 

    // Print Scene data
    std::cout << "Camera Position: " << glm::to_string(scene->eye.position) << std::endl;
    std::cout << "Ambient Intensity: " << glm::to_string(scene->ambient.intensity) << std::endl;
    for(int i = 0  ; i < scene->objects.size() ; i++  ) { 
        scene->lights[i]->print();
         scene->objects[i]->print() ;
    }  



    
    
    glm :: vec3 myvec = glm :: vec3(1.0, 0.0, -5.0) ; 
    Material Mymaterial = Material ( myvec , 4 ) ;
    Sphere TempSphere = Sphere(myvec ,3.0 , Mymaterial ) ; 

    // Initialize pixel data based on ray intersections
    for (int y = 0; y < height; ++y) {
        for ( int x = 0; x < width; ++x) {
             for (Object* obj : scene->objects) {
                if (obj->Intersect(scene->rays[y][x])) {
                // Check intersection for the current ray and object
                    imageArray[y][x][0] = 156; 
                    imageArray[y][x][1] = 88; 
                    imageArray[y][x][2] = 11;  
                }
                else {
                        imageArray[y][x][0] = scene->ambient.intensity[0] ; 
                        imageArray[y][x][1] = scene->ambient.intensity[1] ; 
                        imageArray[y][x][2]  = scene->ambient.intensity[2] ; 


                }

                
            }
        }
    }


   
    SaveImage (imageArray , outputImageName , filepath_outputImage ) ; 

    return 0 ; 


}