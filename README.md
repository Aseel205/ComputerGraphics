# Computer Graphics Projects

This repository contains the implementations of several computer graphics projects that explore various techniques and concepts in 3D rendering, image processing, and ray tracing. The projects utilize **C++**, OpenGL, and Python to implement advanced graphics algorithms and image processing filters. Below are the details of each project.

## Projects Overview

### 1. **Rubik's Cube Renderer (OpenGL)**
   In this project, we rendered a dynamic Rubik's cube using **OpenGL** and **C++**. The cube was broken down into smaller cubes, and various transformations were applied to simulate rotation and movement of the entire Rubik's cube.
   
   #### Key Features:
   - **Perspective View Camera**: A 45-degree Field of View (FOV) was implemented to simulate a 3D perspective of the Rubik's cube.
   - **Cube Mesh with Textures**: Each cube was rendered with a texture to represent its surface. Translations and rotations were applied to the individual cubes to form the complete Rubik's cube.
   - **Rotation Handling**: Implemented keyboard and mouse controls to rotate the entire Rubik's cube, rotate specific layers, and perform interactive movements using mouse input.
   - **Color Picking and Manipulation**: Mouse interactions were designed to pick cubes for manipulation, allowing rotation and translation of individual cubes.
   
   #### Technologies Used:
   - **C++**
   - **OpenGL**

### 2. **Ray Tracing Implementation**
   This project involved building a ray tracing engine to simulate the rendering of 3D scenes with lighting, shadows, reflections, and refractions. Ray tracing is a rendering technique that simulates the way light interacts with objects in a 3D scene.
   
   #### Key Features:
   - **3D Scene Rendering**: Basic geometric shapes like spheres and planes were rendered using ray tracing.
   - **Lighting Models**: Implemented global ambient light, directional lights, and spotlights.
   - **Material Properties**: Each object in the scene had basic material properties like ambient, diffuse, and specular reflections.
   - **Shadows and Reflection**: Basic hard shadows were implemented, as well as reflection and transparency up to five recursive steps.
   - **Anti-Aliasing**: Multi-sampling was applied for anti-aliasing to reduce jagged edges in the rendered images.

   #### Technologies Used:
   - **C++**
   - **OpenGL** (for visualization)

### 3. **Image Filters and Processing**
   This assignment focused on implementing various image filters and processing algorithms using **C++**. The goal was to apply transformations to a specific image (Lenna) and generate multiple filtered versions.
   
   #### Key Filters Implemented:
   - **Grayscale**: Converted the image into a grayscale format with pixel values ranging from 0 (black) to 255 (white).
   - **Canny Edge Detection**: Applied the Canny edge detection algorithm to highlight edges in the image using techniques such as Gaussian filtering, gradient calculation, non-maximum suppression, and edge tracking by hysteresis.
   - **Halftone Pattern**: Transformed the image into a halftone pattern, enlarging each pixel into a 4x4 pixel block to create a pixelated effect.
   - **Floyd-Steinberg Dithering**: Applied the Floyd-Steinberg dithering algorithm to reduce the number of grayscale levels in the image, producing a pixelated result with a more natural-looking diffusion of error.

   #### Technologies Used:
   - **C++**
   - Image processing libraries (e.g., OpenCV)

## How to Run

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/computer-graphics-projects.git
