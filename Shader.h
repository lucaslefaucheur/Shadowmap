#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>
#include <fstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;
    
    Shader(const char* vertexPath, const char* fragmentPath) {
        // Vertex Shader
        std::ifstream VertexShaderStream(vertexPath, std::ios::in);
        std::string VertexShaderCode = std::string(std::istreambuf_iterator<char>(VertexShaderStream), std::istreambuf_iterator<char>());
        char const * vertexShaderSource = VertexShaderCode.c_str();
        
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        
        // Fragment Shader
        std::ifstream FragmentShaderStream(fragmentPath, std::ios::in);
        std::string FragmentShaderCode = std::string(std::istreambuf_iterator<char>(FragmentShaderStream), std::istreambuf_iterator<char>());
        char const * fragmentShaderSource = FragmentShaderCode.c_str();
        
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        
        // Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void use() { glUseProgram(ID); }
    
    // Update model, view and projection matrices
    void update(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
        GLuint modelLoc = glGetUniformLocation(ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        GLuint viewLoc = glGetUniformLocation(ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        GLuint projectionLoc = glGetUniformLocation(ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }
    
    // set an integer in the shader
    void setInt(const char* name, int value) {
        GLuint loc = glGetUniformLocation(ID, name);
        glUniform1i(loc, value);
    }
    
    // set a float in the shader
    void setFloat(const char* name, float value) {
        GLuint loc = glGetUniformLocation(ID, name);
        glUniform1f(loc, value);
    }
    
    // set a vector of size 3 in the shader
    void setVec3(const char* name, glm::vec3 vector) {
        GLuint loc = glGetUniformLocation(ID, name);
        glUniform3f(loc, vector.x, vector.y, vector.z);
    }
    
    // set a matrix of size 4*4 in the shader
    void setMat4(const char* name, glm::mat4 matrix) const {
        GLuint loc = glGetUniformLocation(ID, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    }
};
#endif
