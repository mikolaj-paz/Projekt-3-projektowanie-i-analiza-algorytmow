#include "resource_manager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

Shader& ResourceManager::loadShader(const char *vShaderFile, const char *fShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return Shaders[name];
}

Shader& ResourceManager::getShader(std::string name)
{
    return Shaders[name];
}

Texture2D& ResourceManager::loadQuad(const int& width, const int& alpha, std::string name)
{
    Textures[name] = loadEmptyQuad(width, alpha);
    return Textures[name];
}

Texture2D& ResourceManager::loadTexture(const char *file, bool alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D& ResourceManager::getTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.getID());
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.getID());
}

Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode);
    return shader;
}

Texture2D ResourceManager::loadEmptyQuad(const int& width, const int& alpha)
{
    Texture2D texture;
    
    unsigned char data[width * width * 4];
    for (int i = 0; i < width * width * 4; i++)
    {
        if (i % 4 == 3)
            data[i] = alpha;
        else
            data[i] = 255;
    }

    texture.setInternalFormat(GL_RGBA8);
    texture.setImageFormat(GL_RGBA);

    texture.generate(width, width, data);

    return texture;
}

Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.setInternalFormat(GL_RGBA);
        texture.setImageFormat(GL_RGBA);
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}