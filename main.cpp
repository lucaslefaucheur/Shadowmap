#include "functions.h"

std::vector<GLfloat> vertices;

GLuint VBO, VAO;

// Render the object file
void renderScene(Shader &shader) {
    glm::mat4 model;
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    shader.use();
    shader.setMat4("model", model);
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices.front(), GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main() {
    initialize();
    
    // Link the shaders used for rendering to depth map
    Shader lightViewShader("/Users/lucaslefaucheur/Desktop/371/Assignment2/lightview.vs", "/Users/lucaslefaucheur/Desktop/371/Assignment2/lightview.fs");
    
    // Link the shaders used for rendering as normal
    Shader cameraViewShader("/Users/lucaslefaucheur/Desktop/371/Assignment2/cameraview.vs", "/Users/lucaslefaucheur/Desktop/371/Assignment2/cameraview.fs");
    
    // Load the scene.obj file and order it in the vector
    loadOBJ("/Users/lucaslefaucheur/Desktop/371/obj/scene.obj", vertices);
    
    // Configure depth map FBO
    const GLuint SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // Create depth texture
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glm::mat4 test = camera.GetViewMatrix();
    
    cameraViewShader.use();
    cameraViewShader.setInt("shadowMap", 1);

    // Loop until the window is closed
    while (!glfwWindowShouldClose(window)) {
        float near_plane = 1.0f, far_plane = 1000.0f;
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Define the matrices
        glm::mat4 view, projection, lightProjection, lightView, lightSpaceMatrix;
        lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, near_plane, far_plane);
        
        // Render to depth map
        lightViewShader.use();
        lightViewShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderScene(lightViewShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // Render scene as normal with shadow mapping
        cameraViewShader.use();
        cameraViewShader.setVec3("lightPos", lightPos);
        cameraViewShader.setVec3("viewPos", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z));
        cameraViewShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        cameraViewShader.setFloat("bias", bias);
        cameraViewShader.setMat4("view", view);
        cameraViewShader.setMat4("projection", projection);
        glViewport(0, 0, SCR_WIDTH*2, SCR_HEIGHT*2);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderScene(cameraViewShader);
         
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}
