#ifndef Camera_h
#define Camera_h

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera {
public:
    glm::vec3 Up;
    glm::vec3 Right;
    
    glm::vec3 Front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Position = glm::vec3(0.0f, 10.0f, 30.0f);
    glm::vec3 WorldUp  = glm::vec3(0.0f, 1.0f, 0.0f);

    float Yaw              = -90.0f;
    float Pitch            = 0.0f;
    float MovementSpeed    = 0.5f;
    float MouseSensitivity = 0.1f;
    float Zoom             = 45.0f;
    
    float lastX = 800 / 2.0f;
    float lastY = 800 / 2.0f;
    bool firstMouse = true;
    
    Camera() { updateCameraVectors(); }
    
    // Return view matrix using lookAt
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }
    
    // Process input from keyboard
    void ProcessKeyboard(std::string direction) {
        if (direction == "UP")
            Position += Front * MovementSpeed;
        if (direction == "DOWN")
            Position -= Front * MovementSpeed;
        if (direction == "LEFT")
            Position -= Right * MovementSpeed;
        if (direction == "RIGHT")
            Position += Right * MovementSpeed;
    }
    
    // Process input form mouse
    void ProcessMouseMovement(double xpos, double ypos) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        
        lastX = xpos;
        lastY = ypos;
        
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
        
        Yaw   += xoffset;
        Pitch += yoffset;
        
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
        
        updateCameraVectors();
    }
    
    // Uptade front, right and up vectors
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

#endif /* Camera_h */
