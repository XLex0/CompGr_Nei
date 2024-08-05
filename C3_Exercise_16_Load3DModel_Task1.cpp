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

//comentario
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
Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Tiempos
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Velocidad-> cambiar este valor en caso de querer mover el auto mas rapido
float velocidad = 0.05f;
float posicionX = 0.0f;
float posicionZ = 0.0f;
float rotacion = 0.0f;


// Segundo punto
float posicionX2 = 0.0f;
float posicionZ2 = 0.0f;
float rotacion2 = 0.0f;


// Velocidad de la cámara
float cameraSpeed = 5.0f; // Aumentar la velocidad de la cámara

bool dia = true;
bool diaKeyPressed = false;

bool primera = false;
bool primeraKeyPressed = false;

bool segunda = false;
bool segundaKeyPressed= false;

bool desactivarKeyPressed = false;

bool encendida = false;
bool encendidaKeyPressed = false;
bool show = false;
bool Kshow = false;


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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PROYECTO", NULL, NULL);
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


    glEnable(GL_DEPTH_TEST);
    //////////////////////Tomar en cuenta al guardar los shaders en sus respectivas carpetas
        // Shaders
    Shader modelShader("shaders/shader_exercise15t5_casters.vs", "shaders/shader_exercise15t5_casters.fs");

    Shader cupulaShader("shaders/shader_vertex_cupula.vs", "shaders/shader_fragment_cupula.fs");
    Shader lightCubeShader("shaders/shader_exercise15_lightcube.vs", "shaders/shader_exercise15_lightcube.fs");
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    stbi_set_flip_vertically_on_load(true);
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
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.03f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.03f, 0.03f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.03f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.03f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.03f, 0.0f,

        // inferior
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.9f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.9f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.9f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.9f,  0.9f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.9f,

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
    glm::vec3 pointLightPositions[] = {
        glm::vec3(12.79f, 2.0f, 3.25f),
        glm::vec3(12.79f,  2.0f, 11.92f),
        glm::vec3(12.79f,   2.0f, -5.51f),
        glm::vec3(12.79f,   2.0f, -13.99f),

        glm::vec3(4.15f,   2.0f, -14.0f),
        glm::vec3(-4.46f,   2.0f, -14.0f),
        glm::vec3(-12.98f,   2.0f, -14.0f),
        glm::vec3(-13.0f,   2.0f, -5.38f),

         glm::vec3(-13.0f,   2.0f, 3.28f),
          glm::vec3(-13.0f,   2.0f, 11.8f),
           glm::vec3(-4.35f,   2.0f, 11.8f),
           glm::vec3(4.27f,   2.0f, 11.8f),

              glm::vec3(2.58f,   2.0f, -3.53f),
           glm::vec3(1.5f,   2.0f, -4.9f),
             glm::vec3(-2.98f,   2.0f, 0.53f),
           glm::vec3(-1.38f,   2.0f, 2.26f),
           //16

    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVAO);
    // Atributos de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Atributos de coordenadas de textura
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    // Cargar Texturas
    unsigned int cieloDia = loadTexture("textures/dia.jpg");
    unsigned int cieloNoche = loadTexture("textures/noche.jpg");

    cupulaShader.use();
    cupulaShader.setInt("texture1", 0);
    ///////////////////////////////////////////////////////

    stbi_set_flip_vertically_on_load(false);
    // Cargar Modelo
    Model ourModel("model/tesla/tesla.obj");

    ///////////////////////////////////////////////////////
    // CRISTIAN
    Model edificio_dos_torres("model/edificio_dos_torres/edificio_dos_torres.obj");
    Model hospital("model/hospital/hospital.obj");
    Model parque("model/parque/parque.obj");
    Model edificios("model/edificios/edificios.obj");
    Model edificio_chino("model/edificio_chino/edificio_chino.obj");
    Model recta("model/calle/recta.obj");


    ///////////////////////////////////////////////////////
    // NICK

    Model edificio_rojo("model/edificio_rojo/edificio_rojo.obj");
    Model shield("model/shield/shieldok.obj");
    Model russian("model/russian/russian.obj");
    Model BMW("model/BMW/BMW.obj");

    // EMILIO
    Model building("model/building/building.obj");
    Model building02("model/building02/building02.obj");
    Model casanick("model/casanick/casanick.obj");
    Model hall("model/speer_hall/speer_hall.obj");
    Model dinocomcqueen("model/dinocomcqueen/dinocomcqueen.obj");



    //Alejandro
    Model poste("model/poste/poste.obj");

    Model porsche("model/porsche/porsche.obj");
    Model mercedes("model/mercedes/mercedes.obj");
    Model toyota("model/toyota/toyota.obj");


    // Ciclo de renderizado
    while (!glfwWindowShouldClose(window))
    {
        // LOgica de tiempos por frame
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Entrada
        processInput(window);

        // Renderizar
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (show) {
            std::cout << "X: " << posicionX << "   Z: " << posicionZ << std::endl;
        }

        if (primera) {
            glm::vec3 modelPosition = glm::vec3(posicionX, 0.2f, posicionZ);
            glm::vec3 offset = glm::vec3(0.0f, 0.6f, 0.0f);

            camera.Position = modelPosition + offset;

            glm::vec3 direction(0.0f, -0.2f, 0.0f);
            direction.x = cos(glm::radians(rotacion));
            direction.z = -sin(glm::radians(rotacion));
            direction.y = -0.1f; // Mantener la direcci�n en el plano horizontal
            camera.Front = glm::normalize(direction);
        }
        else if (segunda) {
            glm::vec3 modelPosition = glm::vec3(posicionX2, 0.2f, posicionZ2);
            glm::vec3 offset = glm::vec3(0.0f, 0.7f, 0.0f);

            camera.Position = modelPosition + offset;

            glm::vec3 direction(0.0f, -0.2f, 0.0f);
            direction.x = cos(glm::radians(rotacion2));
            direction.z = -sin(glm::radians(rotacion2));
            direction.y = -0.1f; // Mantener la direcci�n en el plano horizontal
            camera.Front = glm::normalize(direction);
        }
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.001f, 10000000.0f);
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
        model = glm::translate(model, glm::vec3(0.0, 9.6f, 0.0));
        model = glm::scale(model, glm::vec3(100.0f, 20.0f, 100.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));


        cupulaShader.setMat4("model", model);
        cupulaShader.setFloat("textureWidth", 1.0f);
        cupulaShader.setFloat("textureHeight", 1.0f);



        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Renderizar el modelo cargado

        modelShader.use();

        modelShader.setVec3("viewPos", camera.Position);
        modelShader.setFloat("material.shininess", 32.0f);
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);


        modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        modelShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
        modelShader.setVec3("dirLight.diffuse", 0.01f, 0.01f, 0.01f);
        modelShader.setVec3("dirLight.specular", 0.05f, 0.05f, 0.05f);

        // Configuración de luces puntuales usando un bucle
        for (int i = 0; i < 16; i++) {
            modelShader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
            modelShader.setVec3("pointLights[" + std::to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
            modelShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", 0.8f, 0.8f, 0.8f);
            modelShader.setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
            modelShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            modelShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.3f);
            modelShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.15f);
        }


        if (!dia && encendida && (primera||segunda)) {
            modelShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
            modelShader.setVec3("spotLight.position", camera.Position);
            modelShader.setVec3("spotLight.direction", camera.Front);
            modelShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            modelShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            modelShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            modelShader.setFloat("spotLight.constant", 1.0f);
            modelShader.setFloat("spotLight.linear", 0.07);
            modelShader.setFloat("spotLight.quadratic", 0.017);
            modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(14.0f)));
            modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(18.0f)));
        }
        else if (!dia) {
            modelShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
            modelShader.setVec3("spotLight.position", camera.Position);
            modelShader.setVec3("spotLight.direction", camera.Front);
            modelShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            modelShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            modelShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            modelShader.setFloat("spotLight.constant", 3.0f);
            modelShader.setFloat("spotLight.linear", 3.0);
            modelShader.setFloat("spotLight.quadratic", 9.02);
            modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(14.0f)));
            modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        }
        else {
            modelShader.setVec3("spotLight.position", camera.Position); // No es necesario si solo desactivas la luz
            modelShader.setVec3("spotLight.direction", camera.Front); // No es necesario si solo desactivas la luz
            modelShader.setVec3("spotLight.ambient", glm::vec3(0.1f));
            modelShader.setVec3("spotLight.diffuse", glm::vec3(1.0f));
            modelShader.setVec3("spotLight.specular", glm::vec3(1.0f));
            modelShader.setFloat("spotLight.constant", 1.0f);
            modelShader.setFloat("spotLight.linear", 0.0f);
            modelShader.setFloat("spotLight.quadratic", 0.0f);
            modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(180.0f))); // Angulo m�ximo posible
            modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(180.0f))); // Angulo m�ximo posible

        }
        ////////////////////____________________main_Characters______________
        // tesla
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(posicionX, 0.0f, posicionZ));
        model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 0.1f, 0.0f));
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
        modelShader.setMat4("model", model);
        ourModel.Draw(modelShader);

  

        //-------------NICK-------------------

               //Edificio rojo
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(28.0f, 0.0f, 30.0f));
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        modelShader.setMat4("model", model);
        edificio_rojo.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40.0f, 0.0f, 30.0f));
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        modelShader.setMat4("model", model);
        edificio_rojo.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(28.0f, 0.0f, 42.0f));
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        modelShader.setMat4("model", model);
        edificio_rojo.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40.0f, 0.0f, 42.0f));
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        modelShader.setMat4("model", model);
        edificio_rojo.Draw(modelShader);


        //Edificio russian
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 45.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        russian.Draw(modelShader);


        //shield
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 30.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        modelShader.setMat4("model", model);
        shield.Draw(modelShader);

        //BMW
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f, 0.0f, 20.0f));
        //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4));
        modelShader.setMat4("model", model);
        BMW.Draw(modelShader);


        //-------------------------------



        // ----------------CRISTIAN-----------------------------
       // **EDIFICIOS** = BLOQUE 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(46.8f, 0.0f, -40.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        edificio_dos_torres.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(39.8f, 0.0f, -40.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        edificio_dos_torres.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(32.8f, 0.0f, -40.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        edificio_dos_torres.Draw(modelShader);

        //--------------------------------------------------------

        // **HOSPITAL** = BLOQUE 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(6.0f, 0.0f, -42.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        hospital.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, -33.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        modelShader.setMat4("model", model);
        hospital.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.0f, 0.0f, -42.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        hospital.Draw(modelShader);

        //-------------------------------------------------
        // **BLOQUE 5 ** -> PARQUE CENTRO 

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.85f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        parque.Draw(modelShader);

        // --------------------------------------------------

        //**BLOQUE 4** -> EDFIICIOS
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(48.0f, 0.0f, -2.0f));
        model = glm::scale(model, glm::vec3(3.5f, 3.0f, 3.5f));
        modelShader.setMat4("model", model);
        edificios.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40.0f, 0.0f, -2.0f));
        model = glm::scale(model, glm::vec3(3.5f, 3.0f, 3.5f));
        modelShader.setMat4("model", model);
        edificios.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(41.0f, 0.0f, 4.5f));
        model = glm::scale(model, glm::vec3(0.09f, 0.075f, 0.09f));
        modelShader.setMat4("model", model);
        edificio_chino.Draw(modelShader);

        //--------------------------------------------------------
        //**********************CALLES****************************
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 0.0f, -25.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.6f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        recta.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-33.0f, 0.0f, -25.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.6f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        recta.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.015f, -1.0f));
        model = glm::scale(model, glm::vec3(0.6f, 0.1f, 0.4f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        recta.Draw(modelShader);


        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.015f, 38.5f));
        model = glm::scale(model, glm::vec3(0.6f, 0.1f, 0.4f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        recta.Draw(modelShader);

        // ----------------------EMILIO------------------------
           // DINOCOMCQUEEN
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f));

        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        modelShader.setMat4("model", model);
        dinocomcqueen.Draw(modelShader);

        // PRIMER BLOQUE EMILIO

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-35.0f, 0.0f, -42.0f));
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        building02.Draw(modelShader);

        // SEGUNDO BLOQUE EMILIO

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-42.0f, 0.0f, 10.0f));
        model = glm::scale(model, glm::vec3(85.0f, 85.0f, 85.0f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        building.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -10.0f));
        model = glm::scale(model, glm::vec3(85.0f, 85.0f, 85.0f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        building.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-25.0f, 0.0f, -5.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        casanick.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-25.0f, 0.0f, 10.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        casanick.Draw(modelShader);

        // TERCER BLOQUE EMILIO
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 39.5f));
        model = glm::scale(model, glm::vec3(0.45f, 0.45f, 0.45f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        hall.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-38.0f, 0.0f, 34.0f));
        model = glm::scale(model, glm::vec3(0.45f, 0.45f, 0.45f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        hall.Draw(modelShader);



        //-------------------ALEJANDRO---------------------------------------
        for (int i = 0; i < 4; ++i) {

            lightCubeShader.use();
            lightCubeShader.setMat4("projection", projection);
            lightCubeShader.setMat4("view", view);

            // we now draw as many light bulbs as we have point lights.
            glBindVertexArray(lightCubeVAO);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-13.0f, 1.777f, -14.0f + i * 8.6f));
            model = glm::scale(model, glm::vec3(0.0428f)); // Make it a smaller cube
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-13.0f, 0.0f, -14.0f + i * 8.6f));
            model = glm::scale(model, glm::vec3(0.002f, 0.002f, 0.002f));
            modelShader.use();
            modelShader.setMat4("model", model);
            poste.Draw(modelShader);
        }

        for (int i = 1; i < 4; ++i) {

            lightCubeShader.use();
            lightCubeShader.setMat4("projection", projection);
            lightCubeShader.setMat4("view", view);

            // we now draw as many light bulbs as we have point lights.
            glBindVertexArray(lightCubeVAO);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-13.0f + i * 8.6, 1.777f, -14.0f + 3 * 8.6f));
            model = glm::scale(model, glm::vec3(0.0428f)); // Make it a smaller cube
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-13.0f + i * 8.6, 0.0f, -14.0f + 3 * 8.6f));
            model = glm::scale(model, glm::vec3(0.002f, 0.002f, 0.002f));
            modelShader.use();
            modelShader.setMat4("model", model);
            poste.Draw(modelShader);
        }

        for (int i = 1; i < 4; ++i) {

            lightCubeShader.use();
            lightCubeShader.setMat4("projection", projection);
            lightCubeShader.setMat4("view", view);

            // we now draw as many light bulbs as we have point lights.
            glBindVertexArray(lightCubeVAO);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-13.0f + 3 * 8.6, 1.777f, -14.0f + i * 8.6f));
            model = glm::scale(model, glm::vec3(0.0428f)); // Make it a smaller cube
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-13.0f + 3 * 8.6, 0.0f, -14.0f + i * 8.6f));
            model = glm::scale(model, glm::vec3(0.002f, 0.002f, 0.002f));
            modelShader.use();
            modelShader.setMat4("model", model);
            poste.Draw(modelShader);
        }

        for (int i = 1; i < 4; ++i) {

            lightCubeShader.use();
            lightCubeShader.setMat4("projection", projection);
            lightCubeShader.setMat4("view", view);

            // we now draw as many light bulbs as we have point lights.
            glBindVertexArray(lightCubeVAO);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-13.0f + i * 8.6, 1.777f, -14.0f));
            model = glm::scale(model, glm::vec3(0.0428f)); // Make it a smaller cube
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-13.0f + i * 8.6, 0.0f, -14.0f));
            model = glm::scale(model, glm::vec3(0.002f, 0.002f, 0.002f));
            modelShader.use();
            modelShader.setMat4("model", model);
            poste.Draw(modelShader);
        }

        //PORSCHE
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(15.0f, 0.235f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        modelShader.setMat4("model", model);
        porsche.Draw(modelShader);



        //MERCEDES
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(posicionX2, 0.4f, posicionZ2));
        model = glm::rotate(model, glm::radians(rotacion2 ), glm::vec3(0.0f, 0.1f, 0.0f));
      
        modelShader.setMat4("model", model);
        mercedes.Draw(modelShader);

        //toyota
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(19.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        modelShader.setMat4("model", model);
        toyota.Draw(modelShader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

bool colision(float x, float z) {
    bool colision = true;
    if (x >= 49.9999f || x <= -49.9999f || z >= 49.9999f || z <= -49.9999f) {
        return false;
    }
    if (x >= -35.67f && x <= -22.68f && z >= 2.6f && z <= 11.45f) {
        return false;
    }
    //CRISTIAN 
    //------------Bloque 1--------------
    if (x >= 32.045f && x <= 48.0f && z >= -13.78 && z <= -6.68f) {
        return false;
    }
    if (x >= 32.7602f && x <= 48.0f && z >= -6.68f && z <= -3.50224f) {
        return false;
    }
    if (x >= 35.1383f && x <= 48.0f && z >= -3.51f && z <= -1.13928f) {
        return false;
    }
    //--------- edificio chino----------
    if (x >= 32.32f && x <= 48.0f && z >= -1.15928f && z <= 11.4712f) {
        return false;
    }
    //------------Bloque 4--------------
    if (x >= 30.7968 && x <= 37.8851f && z >= -43.3505 && z <= -31.6602f) {
        return false;
    }
    if (x >= 37.8465f && x <= 44.8209f && z >= -43.3505 && z <= -31.6602f) {
        return false;
    }
    if (x >= 44.8018f && x <= 49.6287f && z >= -43.3505 && z <= -31.6602f) {
        return false;
    }

    if (z >= 3.46f && z <= 9.86f && x >= -45.5f && x <= -38.73f) {
        return false;
    }


    if (z >= -9.78f && z <= -3.61f && x >= -43.31f && x <= -36.53f) {
        return false;
    }


    if (z >= -12.17f && z <= -3.74f && x >= -35.48f && x <= -23.22f) {
        return false;
    }

    return colision;
}


void processInput(GLFWwindow* window)
{
    float rotacionR = glm::radians(rotacion);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime * cameraSpeed);

    float rotacionEnRadianes = glm::radians(rotacion);

    // Movimiento
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        float posX = posicionX;
        float posZ = posicionZ;

        posicionX += velocidad * cos(rotacionEnRadianes);
        posicionZ += -velocidad * sin(rotacionEnRadianes);
        if (!colision(posicionX, posicionZ)) {
            posicionX = posX;
            posicionZ = posZ;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {

        float posX = posicionX;
        float posZ = posicionZ;

        posicionX -= velocidad * 0.25f * cos(rotacionEnRadianes);
        posicionZ += velocidad * 0.25f * sin(rotacionEnRadianes);

        if (!colision(posicionX, posicionZ)) {
            posicionX = posX;
            posicionZ = posZ;
        }
    }




    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        float newX = posicionX + velocidad * cos(rotacionEnRadianes);
        float newZ = posicionZ - velocidad * sin(rotacionEnRadianes);

        if (colision(newX, newZ)) {
            rotacion += 30.0f * velocidad;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        float newX = posicionX + velocidad * cos(rotacionEnRadianes);
        float newZ = posicionZ - velocidad * sin(rotacionEnRadianes);

        if (colision(newX, newZ)) {
            rotacion -= 30.0f * velocidad;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        float newX = posicionX - velocidad * 0.5f * cos(rotacionEnRadianes);
        float newZ = posicionZ + velocidad * 0.5f * sin(rotacionEnRadianes);

        if (colision(newX, newZ)) {
            rotacion += 30.0f * 0.5f * velocidad;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        float newX = posicionX - velocidad * 0.5f * cos(rotacionEnRadianes);
        float newZ = posicionZ + velocidad * 0.5f * sin(rotacionEnRadianes);

        if (colision(newX, newZ)) {
            rotacion -= 30.0f * 0.5f * velocidad;
        }
    }
    //////////////////////////////////////////////////////////////////////////////
    // 
    // 
      float rotacion2EnRadianes = glm::radians(rotacion2);
    // Movimiento para el segundo punto
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        float posX2 = posicionX2;
        float posZ2 = posicionZ2;

        posicionX2 += velocidad * cos(rotacion2EnRadianes);
        posicionZ2 += -velocidad * sin(rotacion2EnRadianes);
        if (!colision(posicionX2, posicionZ2)) {
            posicionX2 = posX2;
            posicionZ2 = posZ2;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        float posX2 = posicionX2;
        float posZ2 = posicionZ2;

        posicionX2 -= velocidad * 0.25f * cos(rotacion2EnRadianes);
        posicionZ2 += velocidad * 0.25f * sin(rotacion2EnRadianes);

        if (!colision(posicionX2, posicionZ2)) {
            posicionX2 = posX2;
            posicionZ2 = posZ2;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        float newX2 = posicionX2 + velocidad * cos(rotacion2EnRadianes);
        float newZ2 = posicionZ2 - velocidad * sin(rotacion2EnRadianes);

        if (colision(newX2, newZ2)) {
            rotacion2 += 30.0f * velocidad;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        float newX2 = posicionX2 + velocidad * cos(rotacion2EnRadianes);
        float newZ2 = posicionZ2 - velocidad * sin(rotacion2EnRadianes);

        if (colision(newX2, newZ2)) {
            rotacion2 -= 30.0f * velocidad;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        float newX2 = posicionX2 - velocidad * 0.5f * cos(rotacion2EnRadianes);
        float newZ2 = posicionZ2 + velocidad * 0.5f * sin(rotacion2EnRadianes);

        if (colision(newX2, newZ2)) {
            rotacion2 += 30.0f * 0.5f * velocidad;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        float newX2 = posicionX2 - velocidad * 0.5f * cos(rotacion2EnRadianes);
        float newZ2 = posicionZ2 + velocidad * 0.5f * sin(rotacion2EnRadianes);

        if (colision(newX2, newZ2)) {
            rotacion2 -= 30.0f * 0.5f * velocidad;
        }
    }

    // Actualiza la rotación en radianes
    rotacion2EnRadianes = glm::radians(rotacion2);


    // Actualiza la rotación en radianes
    rotacionEnRadianes = glm::radians(rotacion);

    //Cambia entre el modo día y noche cada vez que se presiona la tecla P
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        if (!diaKeyPressed) // Solo cambia el estado si la tecla estaba previamente no presionada
        {
            dia = !dia;
            diaKeyPressed = true;
        }
    }
    else
    {
        diaKeyPressed = false; // Resetea el estado cuando la tecla es liberada
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if (!encendidaKeyPressed) // Solo cambia el estado si la tecla estaba previamente no presionada
        {
            encendida = !encendida;
            encendidaKeyPressed = true;
        }
    }
    else
    {
        encendidaKeyPressed = false; // Resetea el estado cuando la tecla es liberada
    }


    // En el ciclo de renderizado
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !primeraKeyPressed)
    {
        show = !show;
        Kshow = true;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE)
    {
        Kshow = false;
    }




    // Activar `primera` y desactivar `segunda` con la tecla `G`
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !primeraKeyPressed)
    {
        primera = !primera;
        segunda = false; // Desactiva `segunda` cuando `primera` se activa
        primeraKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)
    {
        primeraKeyPressed = false;
    }

    // Activar `segunda` y desactivar `primera` con la tecla `H`
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !segundaKeyPressed)
    {
        segunda = !segunda;
        primera = false; // Desactiva `primera` cuando `segunda` se activa
        segundaKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE)
    {
        segundaKeyPressed = false;
    }

    // Desactivar ambas con la tecla `Y`
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && !desactivarKeyPressed)
    {
        primera = false;
        segunda = false;
        desactivarKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_RELEASE)
    {
        desactivarKeyPressed = false;
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