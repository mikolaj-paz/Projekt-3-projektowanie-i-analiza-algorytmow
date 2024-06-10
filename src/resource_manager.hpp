/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.hpp"
#include "shader.hpp"

/// @brief Klasa zajmujaca sie glownie zarzadzaniem teksturami i shaderami.
class ResourceManager
{
    public:
        ResourceManager() = delete;
        
        /// @brief Zaladowuje shader do zbioru shaderow.
        /// @param vShaderFile sciezka do pliku vertex shader'a
        /// @param fShaderFile sciezka do pliku fragment shader'a
        /// @param name nazwa do nadania shader'owi
        /// @return Zwraca referecje do utworzonego shader'a.
        static Shader& loadShader(const char *vShaderFile, const char *fShaderFile, std::string name);
        
        /// @param name nazwa shader'a
        /// @return Zwraca referencje shader'a z podana nazwa.
        static Shader& getShader(std::string name);

        /// @brief Zaladowuje prosty kwadrat do zbioru tekstur.
        /// @param width szerokosc kwadratu
        /// @param alpha wartosc [0,255] przezroczystosci gdzie, 0 to przezroczysty
        /// @param name nazwa do nadania teksturze
        /// @return Zwraca referencje do obiektu tekstury.
        static Texture2D& loadQuad(const int& width, const int& alpha, std::string name);
        
        /// @brief Zaladowuje teksture z pliku do zbioru tekstur.
        /// @param file nazwa pliku w formacie C string
        /// @param alpha czy tekstura ma elementy przezroczyste
        /// @param name nazwa do nadania teksturze
        /// @return Zwraca referencje do obiektu tekstury.
        static Texture2D& loadTexture(const char *file, bool alpha, std::string name);
        
        /// @param name nazwa tekstury
        /// @return Zwraca referencje do konkretnej tekstury ze zbioru tekstur
        static Texture2D& getTexture(std::string name);
        
        /// @brief Metoda czyszczaca zbior tekstur i zbior shader'ow.
        static void clear();

    private:
        static std::map<std::string, Shader> Shaders;       // Zbior shader'ow
        static std::map<std::string, Texture2D> Textures;   // Zbior tekstur
          
        static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile); // Metoda wykonawcza loadShader()
        static Texture2D loadEmptyQuad(const int& width, const int& alpha);                 // Metoda wykonawcza loadQuad()
        static Texture2D loadTextureFromFile(const char *file, bool alpha);                 // Metoda wykonawcza loadTexture()
};

#endif