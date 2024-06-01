#include "game_manager.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"

#define DARK_SQUARE_COLOR glm::vec3(.6f, .38f, .21f) / .82f
#define LIGHT_SQUARE_COLOR glm::vec3(.6f, .38f, .21f) / .55f 

SpriteRenderer *Renderer;

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
    // load textures
    ResourceManager::loadTexture("../assets/00_pieces.png", true, "pieces");
    ResourceManager::loadQuad(this->getWidth() / 8, "square");
}

GameManager::~GameManager()
{
    delete Renderer;
}

void GameManager::processInput(float deltaTime)
{

}

void GameManager::update(float deltaTime)
{

}

void GameManager::render()
{
    int yStep = this->getHeight() / 8;
    int xStep = this->getWidth() / 8;

    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
            Renderer->drawSprite(ResourceManager::getTexture("square"), 
                                 glm::vec2(i * xStep, j * yStep),
                                 glm::vec2(this->getWidth() / 8, this->getHeight() / 8),
                                 .0f,
                                 (i + j) % 2 ? DARK_SQUARE_COLOR : LIGHT_SQUARE_COLOR);
    }

    Renderer->drawSprite(ResourceManager::getTexture("pieces"), glm::vec2(200.0f, 200.0f), glm::vec2(171.0f, 171.0f));
    
}