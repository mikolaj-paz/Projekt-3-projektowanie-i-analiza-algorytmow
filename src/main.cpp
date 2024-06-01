#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "game_manager.hpp"
#include "resource_manager.hpp"

#define INIT_WIDTH 600

// Funkcja wykonujaca sie przy zmianie rozmiaru okna
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GameManager gameManager(INIT_WIDTH, INIT_WIDTH);

int main()
{
    // Podstawowa konfiguracja glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    // Tworzenie okna glfw
    GLFWwindow* window = glfwCreateWindow(INIT_WIDTH, INIT_WIDTH, "Szachy", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Inicjalizacja menendzera wskaznikow funkcji OpenGL - glad, zanim uzyjemy jakiejkolwiek funkcji OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Ustawienie okna roboczego
    glViewport(0, 0, INIT_WIDTH, INIT_WIDTH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Ustawienie funckji wykonujacej sie przy zmianie rozmiaru okna
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gameManager.init();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Petla glowna generowania okna
    while(!glfwWindowShouldClose(window))
    {
        // Kalkulacja zmiany czasu
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // Wejscia
        gameManager.processInput(deltaTime);

        // Odswiezenie klatki
        gameManager.update(deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        gameManager.render();

        // Zamiana buforow
        glfwSwapBuffers(window);
    }

    ResourceManager::clear();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (width >= INIT_WIDTH && height >= INIT_WIDTH)
        glViewport( (width - INIT_WIDTH) / 2,
                    (height - INIT_WIDTH) / 2,
                    INIT_WIDTH, 
                    INIT_WIDTH );
    
    else
    {
        if (width < height)
            glViewport(0, (height - width) / 2, width, width);
        else 
            glViewport((width - height) / 2, 0, height, height);
    }
}