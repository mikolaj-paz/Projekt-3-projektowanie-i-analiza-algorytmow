#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "game_manager.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"
#include "move_generator.hpp"

#include "board.hpp"

#define DARK_SQUARE_COLOR glm::vec3(.6f, .38f, .21f) / .82f
#define LIGHT_SQUARE_COLOR glm::vec3(.6f, .38f, .21f) / .55f 
#define MOVE_SQUARE_COLOR glm::vec3(0.85f, .0f, .0f) 

SpriteRenderer *Renderer;
Board *board;

struct
{
    bool render = false;
    bool rendered = false;

    int x, y;
    int origin;

    int piece;

    bool* availableMovesBoard;
} Dummy;

void GameManager::init()
{
    // load shaders
    ResourceManager::loadShader("../shaders/sprite.vert", "../shaders/sprite.frag", "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->getWidth()), 
        static_cast<float>(this->getHeight()), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

    // Tworzenie szachownicy
    board = new Board();

    // Kalkulacja ruchow
    // PrecomputedMoveData::init();

    // Ladowanie tekstur

        // Ladowanie figur
        ResourceManager::loadTexture("../assets/00_pieces.png", true, "6");
        ResourceManager::loadTexture("../assets/01_pieces.png", true, "5");
        ResourceManager::loadTexture("../assets/02_pieces.png", true, "3");
        ResourceManager::loadTexture("../assets/03_pieces.png", true, "2");
        ResourceManager::loadTexture("../assets/04_pieces.png", true, "4");
        ResourceManager::loadTexture("../assets/05_pieces.png", true, "1");
        ResourceManager::loadTexture("../assets/06_pieces.png", true, "14");
        ResourceManager::loadTexture("../assets/07_pieces.png", true, "13");
        ResourceManager::loadTexture("../assets/08_pieces.png", true, "11");
        ResourceManager::loadTexture("../assets/09_pieces.png", true, "10");
        ResourceManager::loadTexture("../assets/10_pieces.png", true, "12");
        ResourceManager::loadTexture("../assets/11_pieces.png", true, "9");

        // Ladowanie pola
        ResourceManager::loadQuad(this->getWidth() / 8, "square");
}

GameManager::~GameManager()
{
    delete Renderer;
    delete board;
}

void GameManager::processInput(float deltaTime)
{
    Dummy.rendered = Dummy.render;
    Dummy.render = buttons[0];

    if (Dummy.render != Dummy.rendered)
    {
        Dummy.y = mouseY / (this->getHeight() / 8);
        Dummy.x = mouseX / (this->getWidth() / 8);

        if (Dummy.render)
        {
            // Drag

            Dummy.origin = 63 - (Dummy.y * 8 + (7 - Dummy.x));
            Dummy.piece = board->get()[Dummy.origin];
            if (Dummy.piece == 0) 
            {
                Dummy.render = false;
                return;
            }
            else
                Dummy.availableMovesBoard = MoveGenerator::getLegalMoves(board, Dummy.origin, Dummy.piece);
        }
        else
        {
            // Drop

            int target = 63 - (Dummy.y * 8 + (7 - Dummy.x));
            if (Dummy.availableMovesBoard[target])
                board->update(Move(Dummy.origin, target, Dummy.piece));

            delete[] Dummy.availableMovesBoard;
        }
    }
}

void GameManager::update(float deltaTime)
{

}

void GameManager::render()
{
    const int sizeY = this->getHeight() / 8;
    const int sizeX = this->getWidth() / 8;

    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            // Rysowanie pola
            Renderer->drawSprite(ResourceManager::getTexture("square"), 
                                 glm::vec2(i * sizeX, j * sizeY),
                                 glm::vec2(sizeX, sizeY),
                                 .0f,
                                 ((i + j) % 2 ? DARK_SQUARE_COLOR : LIGHT_SQUARE_COLOR) * (Dummy.render && Dummy.availableMovesBoard[63 - (j * 8 + (7 - i))] ? MOVE_SQUARE_COLOR : glm::vec3(1)) );
            
            // // Rysowanie adekwatnej figury
            const int* const squares = board->get();
            if (squares[63 - (j * 8 + (7 - i))] != 0 && !(Dummy.render && Dummy.x == i && Dummy.y == j))
                Renderer->drawSprite(ResourceManager::getTexture(std::to_string(squares[63 - (j * 8 + (7 - i))])),
                                     glm::vec2(i * sizeX, j * sizeY),
                                     glm::vec2(sizeX, sizeY));
        }
    }

    if (Dummy.render)
        Renderer->drawSprite(ResourceManager::getTexture(std::to_string(board->get()[63 - (Dummy.y * 8 + (7 - Dummy.x))])),
                             glm::vec2(mouseX - (sizeX / 2), mouseY - (sizeY / 2)),
                             glm::vec2(sizeX, sizeY));
}