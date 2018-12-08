#ifndef functions_h
#define functions_h

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/constants.hpp"

#include <vector>
#include <string>
#include <iostream>

#include "Shader.h"
#include "Camera.h"

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 800;

GLFWwindow* window;
Camera camera;
glm::vec3 lightPos = glm::vec3(0.0f, 40.0f, 40.0f);

float bias = 0.00005;

// Update the window, the camera, the light position or the bias depending on the keyboard input
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard("LEFT");     // move the camera to the left
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboard("RIGHT");    // move the camera to the right
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard("UP");       // move the camera up
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard("DOWN");     // move the camera down
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        lightPos.x--;                       // move the light to the left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        lightPos.x++;                       // move the light to the right
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        lightPos.y++;                       // move the light up
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        lightPos.y--;                       // move the light down
    
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        bias+=0.000001;                     // increase the bias
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && bias > 0)
        bias-=0.000001;                     // decrease the bias
}

// Update the camera depending on the mouse position
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    camera.ProcessMouseMovement(xpos, ypos);
}

// Create a window and initialize GLFW and GLEW
void initialize() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(0);
    }
    
    glfwSetCursorPosCallback(window, mouseCallback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    glewExperimental = GL_TRUE;
    glewInit();
}

// Load an obj file and order it in a vector
void loadOBJ(const char * path, std::vector<GLfloat> & vertices) {
    std::vector<GLuint> vertexIndices, normalIndices;
    std::vector<glm::vec3> tempVertices, tempNormals;
    
    printf("Loading OBJ file %s\n", path);
    FILE * file = fopen(path, "r");
    
    if(file == NULL) {
        printf("Impossible to open the file");
        exit(0);
    }
    
    // Loop through the file
    while(1){
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
    
        // If the line describes a vertex
        else if (strcmp(lineHeader, "v") == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            tempVertices.push_back(vertex);
        }
        
        // If the line describes the normal of one vertex
        else if (strcmp(lineHeader, "vn") == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            tempNormals.push_back(normal);
        }
        
        // If the line describes a face
        else if (strcmp(lineHeader, "f") == 0){
            std::string vertex1, vertex2, vertex3;
            int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9){
                printf("File can't be read by our simple parser");
                exit(0);
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        
        // If it is something else, we ignore
        else {
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }
    
    // Put the vertices in order
    for (GLuint i = 0; i < vertexIndices.size(); i++ ){
        GLuint vertexIndex = vertexIndices[i];
        glm::vec3 vertex = tempVertices[vertexIndex-1];
        vertices.push_back(vertex.x);
        vertices.push_back(vertex.y);
        vertices.push_back(vertex.z);
        
        int temp = normalIndices[i] - 1;
        vertices.push_back(tempNormals[temp].x);
        vertices.push_back(tempNormals[temp].y);
        vertices.push_back(tempNormals[temp].z);
    }
}

#endif /* functions_h */
