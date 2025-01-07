#include <Camera.h>

void Camera::SetOrthographic(float near, float far)
{
    m_Near = near;
    m_Far = far;

    // Rest Projection and View matrices
    m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, near, far);
    m_View = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
}

/////////////////////
// Input Callbacks //
/////////////////////

void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! KeyCallback is skipped" << std::endl;
        return;
    }

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
            case GLFW_KEY_UP:
                std::cout << "UP Pressed" << std::endl;
                break;
            case GLFW_KEY_DOWN:
                std::cout << "DOWN Pressed" << std::endl;
                break;
            case GLFW_KEY_LEFT:
                std::cout << "LEFT Pressed" << std::endl;
                break;
            case GLFW_KEY_RIGHT:
                std::cout << "RIGHT Pressed" << std::endl;
                break;
            default:
                break;
        }
    }
}

void MouseButtonCallback(GLFWwindow* window, double currMouseX, double currMouseY)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        std::cout << "MOUSE LEFT Click" << std::endl;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        std::cout << "MOUSE RIGHT Click" << std::endl;
    }
}

void CursorPosCallback(GLFWwindow* window, double currMouseX, double currMouseY)
{
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! KeyCallback is skipped" << std::endl;
        return;
    }

        float deltaX = camera->m_OldMouseX - currMouseX;
        float deltaY = camera->m_OldMouseY - currMouseY;
        camera->m_OldMouseX = currMouseX;
        camera->m_OldMouseY = currMouseY;


        // Check if left mouse button is pressed to trigger rotation
 if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // Output mouse motion (for debugging)
        std::cout << "MOUSE LEFT Motion: DeltaX = " << deltaX << ", DeltaY = " << deltaY << std::endl;

        // Adjust the cube's rotation angles based on mouse movement
        // Apply sensitivity scaling for smoother rotation
  //      camera->m_RotationAngleX += deltaY * camera->m_RotationSensitivity;
  //      camera->m_RotationAngleY += deltaX * camera->m_RotationSensitivity;

        // Clamp the rotation angles to avoid extreme values
  //      camera->m_RotationAngleX = glm::clamp(camera->m_RotationAngleX, -90.0f, 90.0f); // Prevent camera from flipping upside down
  //      camera->m_RotationAngleY = glm::clamp(camera->m_RotationAngleY, -180.0f, 180.0f); // Limit horizontal rotation range

        // Output the updated rotation angles for debugging
        std::cout << "Updated Rotation Angles: X = " << camera->m_RotationAngleX << ", Y = " << camera->m_RotationAngleY << std::endl;
    }

  // Mouse rigth motion , when you click and mve the mouse  
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
            
        // Handle motion for the right mouse button
        
            float sensitivity = 0.005f; // Adjust movement speed

            // Update camera position based on mouse motion
            camera->m_Position += glm::normalize(glm::cross(camera->m_Orientation, camera->m_Up)) * deltaX * sensitivity; // Left/right
            camera->m_Position += camera->m_Up * deltaY * sensitivity;                                                   // Up/down

            // Update the view matrix
            camera->UpdateViewMatrix();

            // Debugging output
            std::cout << "Camera Position: " 
                    << camera->m_Position.x << ", " 
                    << camera->m_Position.y << ", " 
                    << camera->m_Position.z << std::endl;

    
    }
}

/// SCROLL Motion
void ScrollCallback(GLFWwindow* window, double scrollOffsetX, double scrollOffsetY)
{
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! ScrollCallback is skipped" << std::endl;
        return;
    }

    // Define sensitivity for scroll motion
    float sensitivity = 0.5f;

    // Update camera position along the Z-axis in the direction of the camera's orientation
    camera->m_Position += camera->m_Orientation * static_cast<float>(scrollOffsetY) * sensitivity;

    // Update the view matrix
    camera->UpdateViewMatrix();

    // Debugging output
    std::cout << "Camera Position (Z-axis updated): " 
              << camera->m_Position.x << ", " 
              << camera->m_Position.y << ", " 
              << camera->m_Position.z << std::endl;

    std::cout << "SCROLL Motion" << std::endl;
}

void Camera::EnableInputs(GLFWwindow* window)
{
    // Set camera as the user pointer for the window
    glfwSetWindowUserPointer(window, this);

    // Handle key inputs
    glfwSetKeyCallback(window, (void(*)(GLFWwindow *, int, int, int, int)) KeyCallback);

    // Handle cursor buttons
    glfwSetMouseButtonCallback(window, (void(*)(GLFWwindow *, int, int, int)) MouseButtonCallback);

    // Handle cursor position and inputs on motion
    glfwSetCursorPosCallback(window , (void(*)(GLFWwindow *, double, double)) CursorPosCallback);

    // Handle scroll inputs
    glfwSetScrollCallback(window, (void(*)(GLFWwindow *, double, double)) ScrollCallback);
}



void Camera::UpdateViewMatrix()
{
    // Calculate the forward and right directions after rotation
    glm::vec3 right = glm::normalize(glm::cross(m_Up, m_Orientation)); // Right vector (cross of Up and Forward)
    glm::vec3 up = glm::cross(right, m_Orientation); // Recalculate Up vector

    // Apply pitch and yaw rotations to the orientation (camera's forward vector)
    m_Orientation = glm::rotate(m_Orientation, glm::radians(m_RotationAngleX), right); // Pitch (around X-axis)
    m_Orientation = glm::rotate(m_Orientation, glm::radians(m_RotationAngleY), m_Up); // Yaw (around Y-axis)

    // Update the view matrix
    m_View = glm::lookAt(m_Position, m_Position + m_Orientation, up);
}


void Camera::SetPerspective(float fov, float near, float far)
{
    // Set the FOV, near, and far planes
    m_FOV = fov;
    m_PerspectiveNear = near;
    m_PerspectiveFar = far;

    // Calculate the aspect ratio
    float aspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);

    // Update the Projection matrix using glm::perspective
    m_Projection = glm::perspective(glm::radians(m_FOV), aspectRatio, m_PerspectiveNear, m_PerspectiveFar);

    // Update the View matrix
    UpdateViewMatrix();

    // Debugging output
    std::cout << "Perspective set: FOV=" << m_FOV 
              << ", Near=" << m_PerspectiveNear 
              << ", Far=" << m_PerspectiveFar 
              << ", AspectRatio=" << aspectRatio << std::endl;
}


void Camera::SetPosition(glm::vec3 vec3) { 

    this->m_Position = vec3 ; 
    UpdateViewMatrix() ; 
}
