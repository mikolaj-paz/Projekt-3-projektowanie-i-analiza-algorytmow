#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    private:
        enum CompileErrorType
        {
            VERTEX,
            FRAGMENT,
            PROGRAM
        };

    public:
        Shader() {}
        
        Shader& use();
        
        void compile(const char *vertexSource, const char *fragmentSource);
        
        void setFloat (const char *name, float value, bool useShader = false);
        void setInteger (const char *name, int value, bool useShader = false);
        void setVector2f (const char *name, float x, float y, bool useShader = false);
        void setVector2f (const char *name, const glm::vec2 &value, bool useShader = false);
        void setVector3f (const char *name, float x, float y, float z, bool useShader = false);
        void setVector3f (const char *name, const glm::vec3 &value, bool useShader = false);
        void setVector4f (const char *name, float x, float y, float z, float w, bool useShader = false);
        void setVector4f (const char *name, const glm::vec4 &value, bool useShader = false);
        void setMatrix4 (const char *name, const glm::mat4 &matrix, bool useShader = false);

        const unsigned int& getID() const
            { return ID; }

    private:
        unsigned int ID;

        void checkCompileErrors(unsigned int object, const CompileErrorType& type);
};
#endif