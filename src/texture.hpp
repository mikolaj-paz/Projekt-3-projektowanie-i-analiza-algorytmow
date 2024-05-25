#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>

class Texture2D
{
    public:
        Texture2D();
        
        void generate(unsigned int width, unsigned int height, unsigned char* data);
        
        void bind() const;

        void setInternalFormat(const unsigned int& format)
            { Internal_Format = format; }

        void setImageFormat(const unsigned int& format)
            { Image_Format = format; }

        const unsigned int& getID()
            { return ID; }

    private:
        unsigned int ID;
        unsigned int Width, Height;

        unsigned int Internal_Format;
        unsigned int Image_Format;
    
        unsigned int Wrap_S;
        unsigned int Wrap_T;
        unsigned int Filter_Min;
        unsigned int Filter_Max;
};

#endif