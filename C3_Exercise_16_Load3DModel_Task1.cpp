#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <learnopengl/stb_image.h>

// Prototipos de funciones
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);

// Configuración
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Cámara
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Tiempos
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float velocidad = 0.002f;
float posicionX = 0.0f;
float posicionZ = 0.0f;
float rotacion = 0.0f;


bool dia = true;
bool diaKeyPressed = false;

int main()
{
    // glfw: inicialización y configuración
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Crear ventana GLFW
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exercise 16 Task 1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Capturar el mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: cargar todos los punteros de funciones de OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
//Tomar en cuenta al guardar los shaders en sus respectivas carpetas
    // Shaders
    Shader modelShader("shaders/shader_exercise16_mloading.vs", "shaders/shader_exercise16_mloading.fs");
    Shader cupulaShader("shaders/shader_vertex_cupula.vs", "shaders/shader_fragment_cupula.fs");


    float vertices[] = {
            // positions          // normals           // texture coords
            // atrás
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            // delante
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
           
 

            // superior
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.03f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.03f,  0.03f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.03f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.03f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.03f,  0.0f,

            // inferior
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.8f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.8f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.8f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.8f,  0.8f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.8f,

             // izquierda
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
   0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,
   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,

  // derecha
  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
   0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,
   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f

     
         };

    unsigned int VBOn, VAOn;
    glGenVertexArrays(1, &VAOn);
    glGenBuffers(1, &VBOn);

    glBindVertexArray(VAOn);

    glBindBuffer(GL_ARRAY_BUFFER, VBOn);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atributos de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Atributos de coordenadas de textura
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Cargar Texturas
    unsigned int cieloDia = loadTexture("textures/l.jpg");
    unsigned int cieloNoche = loadTexture("textures/m.jpg");

    cupulaShader.use();
    cupulaShader.setInt("texture1", 0);

    // Cargar Modelo
    Model ourModel("model/tesla/tesla.obj");


    // Ciclo de renderizado
    while (!glfwWindowShouldClose(window))
    {
        // Lógica de tiempos por frame
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Entrada
        processInput(window);

        // Renderizar
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.001f, 10000.0f);
        glm::mat4 view = camera.GetViewMatrix();

        cupulaShader.use();

        cupulaShader.setMat4("projection", projection);
        cupulaShader.setMat4("view", view);
        glActiveTexture(GL_TEXTURE0);

        if (dia) {
            glBindTexture(GL_TEXTURE_2D, cieloDia);
        }
        else {
            glBindTexture(GL_TEXTURE_2D, cieloNoche);
        }
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, 4.8f, 0.0));
        model = glm::scale(model, glm::vec3(100.0f, 10.0f, 100.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));


        cupulaShader.setMat4("model", model);
        cupulaShader.setFloat("textureWidth", 1.0f);
        cupulaShader.setFloat("textureHeight", 1.0f);



        glBindVertexArray(VAOn);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Renderizar el modelo cargado
   
        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(posicionX, 0.0f, posicionZ));
        model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
        modelShader.setMat4("model", model);
        ourModel.Draw(modelShader);



        // Matriz de modelo
  

        // Intercambiar buffers y sondear eventos de IO
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminar GLFW
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    float rotacionR = glm::radians(rotacion);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    float rotacionEnRadianes = glm::radians(rotacion);

    // Movimiento
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        posicionX += velocidad * cos(rotacionEnRadianes);
        posicionZ += -velocidad * sin(rotacionEnRadianes);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        posicionX -= velocidad * 0.25f*cos(rotacionEnRadianes);
        posicionZ += velocidad * 0.25f*sin(rotacionEnRadianes);
    }

    // Rotación el objeto en sentido de las agujas del reloj
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS&& glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        rotacion += 30.0f * velocidad;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS&& glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        rotacion -= 30.0f * velocidad;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        rotacion += -30.0f *0.5f* velocidad;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        rotacion -= -30.0f *0.5f* velocidad;
    }


    // Actualiza la rotación en radianes
    rotacionEnRadianes = glm::radians(rotacion);

//Cambia entre el modo día y noche cada vez que se presiona la tecla P
    
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !diaKeyPressed)
    {
        dia = !dia;
        diaKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
    {
        diaKeyPressed = false;
    }
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
