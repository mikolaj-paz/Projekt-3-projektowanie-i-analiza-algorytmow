#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

enum GameState
{
    GAME_ACTIVE,
    GAME_WIN
};

class GameManager
{
    public:
        double mouseX, mouseY;
        bool buttons[2];

    public:
        GameManager(const unsigned int& width, const unsigned int& height):
            gameState{GAME_ACTIVE}, width{width}, height{height} {}

        ~GameManager();

        void init();
        void processInput(float deltaTime);
        void update(float deltaTime);
        void render();

        const GameState& getState() const
            { return gameState; }

        const unsigned int& getWidth() const
            { return width; }

        const unsigned int& getHeight() const
            { return height; }

    private:
        GameState gameState;
        unsigned int width, height;
};

#endif