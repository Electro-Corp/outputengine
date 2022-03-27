/*
* Output Engine...
* Chinmay Tiwari 2022
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H  
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include <string>
//#include <WinUser.h>
#include <Windows.h>
#include <vector>
float newvert[180];
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void cube(float x, float y, float z) {
    for (int i = 0; i < x; i++) {
        newvert[i] =x+i;
    }
    for (int i = 0; i < y; i++) {
        newvert[i] = y + i;
    }
    for (int i = 0; i < z; i++) {
        newvert[i] = z + i;
    }
   
}
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool firstMouse;
float yaw;
float pitch;
float lastX;
float lastY;
//shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n" "out vec4 FragColor;\n" "void main()\n"
 "{\n""FragColor = vec4(1.0f, 0.9f, 0.2f, 1.0f);\n" "}\n";
float vertices[] = {
           -0.9f, -0.9f, -0.9f,  0.0f, 0.0f,
            0.9f, -0.9f, -0.9f,  1.0f, 0.0f,
            0.9f,  0.9f, -0.9f,  1.0f, 1.0f,
            0.9f,  0.9f, -0.9f,  1.0f, 1.0f,
           -0.9f,  0.9f, -0.9f,  0.0f, 1.0f,
           -0.9f, -0.9f, -0.9f,  0.0f, 0.0f,

           -0.9f, -0.9f,  0.9f,  0.0f, 0.0f,
            0.9f, -0.9f,  0.9f,  1.0f, 0.0f,
            0.9f,  0.9f,  0.9f,  1.0f, 1.0f,
            0.9f,  0.9f,  0.9f,  1.0f, 1.0f,
           -0.9f,  0.9f,  0.9f,  0.0f, 1.0f,
           -0.9f, -0.9f,  0.9f,  0.0f, 0.0f,

           -0.9f,  0.9f,  0.9f,  1.0f, 0.0f,
           -0.9f,  0.9f, -0.9f,  1.0f, 1.0f,
           -0.9f, -0.9f, -0.9f,  0.0f, 1.0f,
           -0.9f, -0.9f, -0.9f,  0.0f, 1.0f,
           -0.9f, -0.9f,  0.9f,  0.0f, 0.0f,
           -0.9f,  0.9f,  0.9f,  1.0f, 0.0f,

            0.9f,  0.9f,  0.9f,  1.0f, 0.0f,
            0.9f,  0.9f, -0.9f,  1.0f, 1.0f,
            0.9f, -0.9f, -0.9f,  0.0f, 1.0f,
            0.9f, -0.9f, -0.9f,  0.0f, 1.0f,
            0.9f, -0.9f,  0.9f,  0.0f, 0.0f,
            0.9f,  0.9f,  0.9f,  1.0f, 0.0f,

           -0.9f, -0.9f, -0.9f,  0.0f, 1.0f,
            0.9f, -0.9f, -0.9f,  1.0f, 1.0f,
            0.9f, -0.9f,  0.9f,  1.0f, 0.0f,
            0.9f, -0.9f,  0.9f,  1.0f, 0.0f,
           -0.9f, -0.9f,  0.9f,  0.0f, 0.0f,
           -0.9f, -0.9f, -0.9f,  0.0f, 1.0f,

           -0.9f,  0.9f, -0.9f,  0.0f, 1.0f,
            0.9f,  0.9f, -0.9f,  1.0f, 1.0f,
            0.9f,  0.9f,  0.9f,  1.0f, 0.0f,
            0.9f,  0.9f,  0.9f,  1.0f, 0.0f,
           -0.9f,  0.9f,  0.9f,  0.0f, 0.0f,
           -0.9f,  0.9f, -0.9f,  0.0f, 1.0f
};
// world space positions of our cubes
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
std::string name;
int main()
{
    //cube(10, 3, 3);
    //glm
    MessageBox(NULL, "Enter the name of the texture to use (must be in C:/demo)", "Output Engine", MB_ICONINFORMATION | MB_OK);
    std::cout << "Image name: ";
    std::cin >> name;
    if (name == "null") { name = "wall"; }
    
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 direction;
   
    yaw = -90.0f;
    direction.x = cos(glm::radians(yaw)); // Note that we convert the angle to radians first
    direction.z = sin(glm::radians(yaw));
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //MessageBox(NULL, "Shader not read!", "xd lamo lol rofl xd so funny yeah", MB_ICONERROR | MB_OK);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Output Engine 3d Demo", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    else { std::cout << "all going to plan.. for now.\n"; }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    Shader ourShader("4.1.texture.vs", "4.1.texture.sus");
        glEnable(GL_DEPTH_TEST);
    //vertice for triangle
    
    
    unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    /*glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);*/
    //unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // TEXTURE!!!!
    int width, height, nrChannels;
    std::string bef = name;
    name = "C:\\demo\\" + name + ".jpg"; 
    const char* name2 = name.c_str();
    unsigned char* data = stbi_load(name2, &width, &height, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        
        std::string error = "Texture Find Failure, " + bef + " not found!";
        const char* beef = error.c_str();
        MessageBox(NULL, beef, "Output Engine", MB_ICONERROR | MB_OKCANCEL);

    }

    stbi_image_free(data);
    unsigned char* bruh = stbi_load("C:\\demo\\crude.jpg", &width, &height, &nrChannels, 0);
    unsigned int source;
    glGenTextures(1, &source);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, source);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bruh);
    if (bruh)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bruh);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;

        std::string error = "Texture Find Failure, Intro image not found! OH NO!";
        const char* beef = error.c_str();
        MessageBox(NULL, beef, "Output Engine", MB_ICONERROR | MB_OKCANCEL);

    }

    stbi_image_free(bruh);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    
    
    
  
    /*unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // frag (grenade) shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    /// <summary>
    /// The shader program combines the shaders n stuff so we can use em
    /// </summary>
    /// <returns></returns>
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram(); //create yay
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    //delete shaders  now that we're done
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //vertex stuff
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    unsigned int VAO;
    glGenVertexArrays(1, & VAO);
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);*/

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    PlaySound(TEXT("C:\\demo\\theme.wav"), NULL, SND_FILENAME | SND_ASYNC);
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);
        glfwSetCursorPosCallback(window, mouse_callback);


        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.9f, 1.0f, 0.0f));
        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;

        view = view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations

        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        // pass them to the shaders (3 different ways)

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // bind Texture
        glBindTexture(GL_TEXTURE_2D, source);
        // cam


        //unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        // render container
        ourShader.use();
        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.9f));
            ourShader.setMat4("model", model);
            model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.9f, 1.0f, 0.0f));

            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
       

        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        //std::cout << "goodbye cruel world..";
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

int createCube(float x, float y, float width, float height) {
    return 0;
}