#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <thread>
#include <atomic>

#include <fstream>

#include "resource_manager.hpp"
#include "sprite_renderer.hpp"
#include "move_generator.hpp"
#include "bot.hpp"

#include "board.hpp"

enum BotState
{
    IDLING,
    THINKING
};

class GameManager
{
    public:
        double mouseX, mouseY;
        bool buttons[2];

    public:
        GameManager(const unsigned int& width, const unsigned int& height):
            gameState{GAME_ACTIVE}, botState{IDLING}, width{width}, height{height} {}

        ~GameManager();

        void init();
        void processInput(float deltaTime);
        void update(float deltaTime);
        void render();

        const unsigned int& getWidth() const
            { return width; }

        const unsigned int& getHeight() const
            { return height; }

    private:
        std::atomic<GameState> gameState;

        std::atomic<BotState> botState;

        unsigned int width, height;
};

#endif