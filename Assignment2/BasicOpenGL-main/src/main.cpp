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
#include "phong.h"

/* Window size */
const unsigned int width = 800;
const unsigned int height = 800;

void SaveImage(const std::vector<std::vector<std::vector<unsigned char>>> &imageArray, const std::string &imageName, const std::string &outputDirectory);
void RayTrace(Scene &scene, int width, int height, std::string outputImageName, std::string filepath_outputImage);

int main(int argc, char *argv[])
{

    std::string filepath_input = "C:\\Users\\aseel\\OneDrive\\Desktop\\computer graphics\\Assignment2\\BasicOpenGL-main\\src\\sample_scene1.txt";
    std::string filepath_outputImage = "C:\\Users\\aseel\\OneDrive\\Desktop\\computer graphics\\Assignment2\\BasicOpenGL-main\\src\\res\\";
    std::string outputImageName = "raytracing.png";

    // Create SceneReader object and read the scene from the file
    SceneReader reader;
    Scene* scene = reader.readScene(filepath_input);

    

    std::cout << "  we are before the ray trace " << std::endl;

    RayTrace(*(scene), width, height, outputImageName, filepath_outputImage);

    std::cout << "  we are after the ray trace " << std::endl;

    return 0;
}

void RayTrace(Scene &scene, int width, int height, std::string outputImageName, std::string filepath_outputImage)
{
    std::vector<std::vector<std::vector<unsigned char>>> image(
        height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3)));

    // Iterate over height (Y) first for better cache locality (row-major order)
    for (int y = 0; y < height; y++)
    {                                   // Iterate over height (Y)
        int flipped_y = height - 1 - y; // Flip the Y-coordinate to correct vertical orientation

        for (int x = 0; x < width; x++)
        { // Iterate over width (X)
            // Construct the ray for the current pixel (x, flipped_y)
            Ray ray = SceneReader::ConstructRayThroughPixel(x, flipped_y, scene);

            // Calculate the color for the ray at this pixel
            glm::vec3 color = Phong::calcColor(scene, ray);

            // Store the RGB values in the image
            image[y][x][0] = static_cast<unsigned char>(255 * glm::clamp(color.r, 0.0f, 1.0f)); // Red
            image[y][x][1] = static_cast<unsigned char>(255 * glm::clamp(color.g, 0.0f, 1.0f)); // Green
            image[y][x][2] = static_cast<unsigned char>(255 * glm::clamp(color.b, 0.0f, 1.0f)); // Blue
        }
    }

    // Save the image to the output file
    SaveImage(image, outputImageName, filepath_outputImage);
}

void SaveImage(const std::vector<std::vector<std::vector<unsigned char>>> &imageArray, const std::string &imageName, const std::string &outputDirectory)
{
    // Construct the full file path
    std::string filePath = outputDirectory + imageName;

    // Derive image dimensions from the input array
    int height = imageArray.size();         // Number of rows
    int width = imageArray[0].size();       // Number of columns
    int channels = imageArray[0][0].size(); // Number of color channels (e.g., 3 for RGB)

    // Create a buffer to hold the image data in a format suitable for saving
    unsigned char *buffer = new unsigned char[width * height * channels];

    // Copy image data to the buffer
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int index = (y * width + x) * channels;
            for (int c = 0; c < channels; ++c)
            {
                buffer[index + c] = imageArray[y][x][c];
            }
        }
    }

    // Save the image as a PNG file
    int result = stbi_write_png(filePath.c_str(), width, height, channels, buffer, width * channels);
    if (result)
    {
        std::cout << "Image saved successfully to " << filePath << std::endl;
    }
    else
    {
        std::cerr << "Failed to save the image to " << filePath << std::endl;
    }

    // Free the buffer after saving
    delete[] buffer;
}
