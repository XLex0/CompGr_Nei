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
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Tiempos
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Velocidad-> cambiar este valor en caso de querer mover el auto mas rapido
float velocidad = 0.01f;
float posicionX = 0.0f;
float posicionZ = 0.0f;
float rotacion = 0.0f;


// Velocidad de la cámara
float cameraSpeed = 5.0f; // Aumentar la velocidad de la cámara

bool dia = true;
bool diaKeyPressed = false;
bool primera = false;
bool primeraKeyPressed = false;
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
        glm::vec3(-2.0f,  2.0f, -5.0f),
        glm::vec3(-1.0f,  2.0f, -5.0f),
        glm::vec3(0.0f,   2.0f, -5.0f),
        glm::vec3(1.0f,   2.0f, -5.0f),
        glm::vec3(2.0f,   2.0f, -5.0f),
        glm::vec3(3.0f,   2.0f, -5.0f)
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
    Model ourModel("C:/model/tesla/tesla.obj");

    ///////////////////////////////////////////////////////
    // CRISTIAN
    Model edificio_dos_torres("C:/model/edificio_dos_torres/edificio_dos_torres.obj");
    Model hospital("C:/model/hospital/hospital.obj");

    ///////////////////////////////////////////////////////
    // NICK

	Model edificio_rojo("C:/model/edificio_rojo/edificio_rojo.obj");
    Model shield("C:/model/shield/shieldok.obj");
	Model russian("C:/model/russian/russian.obj");
	Model BMW("C:/model/BMW/BMW.obj");


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
            glm::vec3 modelPosition = glm::vec3(posicionX, 0.0f, posicionZ);
            glm::vec3 offset = glm::vec3(0.0f, 0.8f, 0.0f);

            camera.Position = modelPosition + offset;

            glm::vec3 direction(0.0f, 0.0f, 0.0f);
            direction.x = cos(glm::radians(rotacion));
            direction.z = -sin(glm::radians(rotacion));
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
        model = glm::translate(model, glm::vec3(0.0, 4.8f, 0.0));
        model = glm::scale(model, glm::vec3(100.0f, 10.0f, 100.0f));
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

        // point light 1
        modelShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        modelShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[0].constant", 1.0f);
        modelShader.setFloat("pointLights[0].linear", 0.3f);    // Valor ajustado
        modelShader.setFloat("pointLights[0].quadratic", 0.15f); // Valor ajustado

        // point light 2
        modelShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        modelShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[1].constant", 1.0f);
        modelShader.setFloat("pointLights[1].linear", 0.3f);    // Valor ajustado
        modelShader.setFloat("pointLights[1].quadratic", 0.15f); // Valor ajustado

        // point light 3
        modelShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        modelShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[2].constant", 1.0f);
        modelShader.setFloat("pointLights[2].linear", 0.3f);    // Valor ajustado
        modelShader.setFloat("pointLights[2].quadratic", 0.15f); // Valor ajustado

        // point light 4
        modelShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        modelShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[3].constant", 1.0f);
        modelShader.setFloat("pointLights[3].linear", 0.3f);    // Valor ajustado
        modelShader.setFloat("pointLights[3].quadratic", 0.15f); // Valor ajustado

        // point light 5
        modelShader.setVec3("pointLights[4].position", pointLightPositions[4]);
        modelShader.setVec3("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("pointLights[4].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[4].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[4].constant", 1.0f);
        modelShader.setFloat("pointLights[4].linear", 0.3f);    // Valor ajustado
        modelShader.setFloat("pointLights[4].quadratic", 0.15f); // Valor ajustado

        // point light 6
        modelShader.setVec3("pointLights[5].position", pointLightPositions[5]);
        modelShader.setVec3("pointLights[5].ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("pointLights[5].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[5].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[5].constant", 1.0f);
        modelShader.setFloat("pointLights[5].linear", 0.3f);    // Valor ajustado
        modelShader.setFloat("pointLights[5].quadratic", 0.15f); // Valor ajustado



        if (!dia) {
            modelShader.setVec3("spotLight.position", camera.Position);
            modelShader.setVec3("spotLight.direction", camera.Front);
            modelShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            modelShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            modelShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            modelShader.setFloat("spotLight.constant", 1.0f);
            modelShader.setFloat("spotLight.linear", 0.02);
            modelShader.setFloat("spotLight.quadratic", 0.02);
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
            modelShader.setFloat("spotLight.linear", 0.09f);
            modelShader.setFloat("spotLight.quadratic", 0.0f);
            modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(180.0f))); // Angulo m�ximo posible
            modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(180.0f))); // Angulo m�ximo posible

        }
        // tesla
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(posicionX, 0.0f, posicionZ));
        model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 0.1f, 0.0f));
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
        modelShader.setMat4("model", model);
        ourModel.Draw(modelShader);



		//-------------NICK-------------------

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(30.0f, 0.0f, 30.0f));
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        modelShader.setMat4("model", model);
        edificio_rojo.Draw(modelShader);

        //calle 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(30.0f, 0.0f, 10.0f));
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        modelShader.setMat4("model", model);
        edificio_rojo.Draw(modelShader);

        //calle 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(20.0f, 0.0f, 10.0f));
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        modelShader.setMat4("model", model);
        edificio_rojo.Draw(modelShader);

		//shield
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f, 0.0f, 30.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        modelShader.setMat4("model", model);
        shield.Draw(modelShader);

        //russian
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 30.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        russian.Draw(modelShader);


		//BMW
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f, 0.0f, 25.0f));
       //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4));
                modelShader.setMat4("model", model);
        BMW.Draw(modelShader);


		//-------------NICK-------------------

       


        /////////////////////
        ////dinomcqueen
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0, 0.1f, 24.0f));

        //model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        //modelShader.setMat4("model", model);
        //dinoModel.Draw(modelShader);

        ////estructura 1
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(32.0f, 0.0f, 45.0f));

        //model = glm::scale(model, glm::vec3(45.0f, 45.0f, 45.0f));
        //modelShader.setMat4("model", model);
        //buildingModel.Draw(modelShader);

        ////estructura 2
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(20.0f, 0.0f, 43.0f));

        //model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
        //modelShader.setMat4("model", model);
        //building02.Draw(modelShader);

        ///*for (int i = 0; i < 5; i++) {
        //    model = glm::mat4(1.0f);
        //    model = glm::translate(model, glm::vec3(-30.0f + i * 15.0f, 0.0f, 43.0f));
        //    model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
        //    modelShader.setMat4("model", model);
        //    building02.Draw(modelShader);
        //}*/

        ////casanick
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(45.0f, 0.0f, 45.0f));

        //model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        //modelShader.setMat4("model", model);
        //casanick.Draw(modelShader);

        // ----------------CRISTIAN-----------------------------
        //**EDIFICIOS** = BLOQUE 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(34.0f, 0.0f, -31.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        modelShader.setMat4("model", model);
        edificio_dos_torres.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(21.8f, 0.0f, -45.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        edificio_dos_torres.Draw(modelShader);


        // **HOSPITAL** = BLOQUE 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(9.0f, 0.0f, -42.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        hospital.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, -33.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        modelShader.setMat4("model", model);
        hospital.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -42.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        // Rotación 90 grados en el eje X (y) -> a la izquierda
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelShader.setMat4("model", model);
        hospital.Draw(modelShader);


        
        // ----------------CRISTIAN-----------------------------
        

        


        // -----------------------------------------------------

        ////////////////////////////////////////////////////////////////////////////////////////////////
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 6; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Matriz de modelo


        // Intercambiar buffers y sondear eventos de IO
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

bool colision(float x, float y) {
    bool colision = true;
    if (x >= -1.63f && x <= 1.19f && y >= 23.3f && y <= 27.16f) {
        colision = false;
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





    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !primeraKeyPressed)
    {
        primera = !primera;
        primeraKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)
    {
        primeraKeyPressed = false;
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

