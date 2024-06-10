/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/// @brief Klasa implementujaca shader.
class Shader
{
    private:
        /// @brief Enumerator typow bledow
        enum CompileErrorType
        {
            VERTEX,
            FRAGMENT,
            PROGRAM
        };

    public:
        Shader() {}
        
        /// @brief Ustawienie shadera jako programu w OpenGL
        /// @return Zwraca referencje do shadera (*this).
        Shader& use();
        
        /// @brief Kompiluje shader, wykorzystujac vertex shader i fragment shader.
        /// @param vertexSource sciezka do pliku z vertex shader'em
        /// @param fragmentSource sciezka do pliku z fragment shader'em
        void compile(const char *vertexSource, const char *fragmentSource);
        
        // ===================================================================
        //      Metody pozwalajace na zmiane zmiennych wewnatrz shader'a
        // ===================================================================

        void setFloat (const char *name, float value, bool useShader = false);
        void setInteger (const char *name, int value, bool useShader = false);
        void setVector2f (const char *name, float x, float y, bool useShader = false);
        void setVector2f (const char *name, const glm::vec2 &value, bool useShader = false);
        void setVector3f (const char *name, float x, float y, float z, bool useShader = false);
        void setVector3f (const char *name, const glm::vec3 &value, bool useShader = false);
        void setVector4f (const char *name, float x, float y, float z, float w, bool useShader = false);
        void setVector4f (const char *name, const glm::vec4 &value, bool useShader = false);
        void setMatrix4 (const char *name, const glm::mat4 &matrix, bool useShader = false);

        /// @return Zwraca ID shader'a.
        const unsigned int& getID() const
            { return ID; }

    private:
        unsigned int ID;    // ID shader'a

        /// @brief Sprawdza poprawnosc zaladowania shader'a.
        /// @param object ID shadera
        /// @param type typ sprawdzanych bledow
        void checkCompileErrors(unsigned int object, const CompileErrorType& type);
};
#endif