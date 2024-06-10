/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"
#include "shader.hpp"

/// @brief Klasa zajmujaca sie wyswietlaniem sprite'ow
class SpriteRenderer
{
    public:
        /// @brief Domyslny konstruktor, inicjuje shader'y, ksztalty.
        /// @param shader shader uzywany do generowania
        SpriteRenderer(Shader &shader);
        
        ~SpriteRenderer();
        
        /// @brief Rysuje zdefiniowanego sprite'a.
        /// @param texture tekstura sprite'a
        /// @param position pozycja na ekranie
        /// @param size rozmiar
        /// @param rotate obrot
        /// @param color kolor
        void drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    private:
        Shader shader;          // Shader przypisany do generatora
        unsigned int quadVAO;   // Vertex array object

        void initRenderData();
};

#endif