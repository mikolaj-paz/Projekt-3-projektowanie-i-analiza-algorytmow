/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

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

/// @brief Enumerator stanu SI
enum BotState
{
    IDLING,
    THINKING
};

/// @brief Klasa zarzadzajaca gra
class GameManager
{
    public:
        double mouseX, mouseY;  // Pozycja kursora myszy
        bool buttons[2];        // Wartosci przyciskow myszy

    public:
        /// @brief Konstruktor podstawowy klasy zarzadcy gry
        /// @param width szerokosc okna
        /// @param height wysokosc okna
        GameManager(const unsigned int& width, const unsigned int& height):
            gameState{GAME_ACTIVE}, botState{IDLING}, width{width}, height{height} {}

        ~GameManager();

        /// @brief Inicjuje wszystkie potrzebne procesy do rozpoczecia gry.
        void init();

        /// @brief Zarzadza wejsciami uzytkownika.
        /// @param deltaTime zmiana czasu w porownaniu z poprzednia klatka
        void processInput(float deltaTime);

        /// @brief Aktualizuje stan gry.
        /// @param deltaTime zmiana czasu w porownaniu z poprzednia klatka
        void update(float deltaTime);

        /// @brief Rysuje wszystkie elementy graficzne na ekran.
        void render();

        /// @return Zwraca szerokosc okna gry.
        const unsigned int& getWidth() const
            { return width; }

        /// @return Zwraca wysokosc okna gry.
        const unsigned int& getHeight() const
            { return height; }

    private:
        std::atomic<GameState> gameState;   // Stan gry

        std::atomic<BotState> botState;     // Stan SI

        unsigned int width, height;         // Szerokosc, wysokosc okna gry
};

#endif