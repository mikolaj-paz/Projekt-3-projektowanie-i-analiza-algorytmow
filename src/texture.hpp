#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>

class Texture2D
{
    public:
        /// @brief Domyslny kontruktor. Dodaje teksture do OpenGL.
        Texture2D();
        
        /// @brief Generuje teksture na podstawie danych o obrazku
        /// @param width szerokosc obrazka
        /// @param height wysokosc obrazka
        /// @param data strumien danych obrazka wygenerowany przez stb_image.
        void generate(unsigned int width, unsigned int height, unsigned char* data);
        
        /// @brief Przypisuje ID tekstury do tekstury 2D w rozumieniu OpenGL.
        void bind() const;

        /// @brief Ustawia format wewnetrzny.
        void setInternalFormat(const unsigned int& format)
            { Internal_Format = format; }

        /// @brief Ustawia format obrazka.
        void setImageFormat(const unsigned int& format)
            { Image_Format = format; }

        /// @return Zwraca ID przypisane do tekstury.
        const unsigned int& getID()
            { return ID; }

    private:
        unsigned int ID;                // ID przypisane do tekstury
        unsigned int Width, Height;     // Szerokosc i wysokosc tekstury

        unsigned int Internal_Format;   // Format wewnetrzny
        unsigned int Image_Format;      // Format obrazka
    
        unsigned int Wrap_S;
        unsigned int Wrap_T;
        unsigned int Filter_Min;
        unsigned int Filter_Max;
};

#endif