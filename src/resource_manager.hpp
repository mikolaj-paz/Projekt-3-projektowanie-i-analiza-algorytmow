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

class ResourceManager
{
    public:
        ResourceManager() = delete;
        
        static Shader& loadShader(const char *vShaderFile, const char *fShaderFile, std::string name);
        
        static Shader& getShader(std::string name);

        static Texture2D& loadQuad(const int& width, std::string name);
        
        static Texture2D& loadTexture(const char *file, bool alpha, std::string name);
        
        static Texture2D& getTexture(std::string name);
        
        static void clear();

    private:
        static std::map<std::string, Shader> Shaders;
        static std::map<std::string, Texture2D> Textures;
        
        static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile);

        static Texture2D loadEmptyQuad(const int& width);

        static Texture2D loadTextureFromFile(const char *file, bool alpha);
};

#endif