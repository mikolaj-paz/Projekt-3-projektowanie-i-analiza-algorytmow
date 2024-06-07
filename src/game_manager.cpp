#include "game_manager.hpp"

#define DARK_SQUARE_COLOR glm::vec3(.6f, .38f, .21f) / .82f
#define LIGHT_SQUARE_COLOR glm::vec3(.6f, .38f, .21f) / .55f 
#define MOVE_SQUARE_COLOR glm::vec3(0.85f, .0f, .0f) 
#define WIN_COLOR glm::vec3(.0f, 1.0f, .0f) 

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
    std::vector<Move> availableMoves;
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
    PrecomputedMoveData::init();

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
            {
                if (board->toMove() == (Dummy.piece & Piece.colorMask) && board->toMove() == Piece.white)
                {
                    unsigned long long bitboard = 0ULL;
                    Dummy.availableMoves = MoveGenerator::getLegalMoves(board, Dummy.origin, &bitboard);

                    Dummy.availableMovesBoard = new bool[64];
                    std::fill(Dummy.availableMovesBoard, Dummy.availableMovesBoard + 64, 0);
                    for (int i = 0; i < 64; i++)
                        Dummy.availableMovesBoard[i] = ((1ULL << i) & bitboard) >> i;
                }
                else
                {
                    Dummy.availableMovesBoard = new bool[64];
                    std::fill(Dummy.availableMovesBoard, Dummy.availableMovesBoard + 64, 0);
                }
            }
        }
        else
        {
            // Drop

            int target = 63 - (Dummy.y * 8 + (7 - Dummy.x));
            if (Dummy.availableMovesBoard[target])
            {
                board->update(Move(Dummy.origin, target, Dummy.piece, board->get()[target]));

                unsigned long long tempBitboard;
                int i = 0;
                do
                    if ((board->get()[i] & Piece.colorMask) == board->toMove() 
                         && MoveGenerator::getLegalMoves(board, i, &tempBitboard).size() != 0)
                        break;
                while (++i < 64);
                if (i == 64)
                    gameState = GAME_WIN;
            }

            delete[] Dummy.availableMovesBoard;
        }
    }
}

void botThink(Board* board, std::atomic<GameState>* gameState, std::atomic<BotState>* botState)
{   
    if (!Bot::makeMove(board))
        *gameState = GAME_WIN;

    std::ofstream output("../data.txt");
    output << Bot::iterations << '\n' << Bot::time / Bot::iterations << std::endl;
    output.close();

    *botState = IDLING;
}

void GameManager::update(float deltaTime)
{
    if (board->toMove() == Piece.black && botState == IDLING)
    {
        botState = THINKING;
        std::thread(botThink, board, &gameState, &botState).detach();

        // if (!Bot::makeMove(board))
        //     gameState = GAME_WIN;
    }
}

void GameManager::render()
{
    const int sizeY = this->getHeight() / 8;
    const int sizeX = this->getWidth() / 8;

    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            int index = 63 - (j * 8 + (7 - i));

            // Rysowanie pola
            Renderer->drawSprite(ResourceManager::getTexture("square"), 
                                 glm::vec2(i * sizeX, j * sizeY),
                                 glm::vec2(sizeX, sizeY),
                                 .0f,
                                 ((i + j) % 2 ? DARK_SQUARE_COLOR : LIGHT_SQUARE_COLOR) * (Dummy.render && Dummy.availableMovesBoard[index] ? MOVE_SQUARE_COLOR : glm::vec3(1)) );

            if (gameState == GAME_WIN && ((1ULL << index) == (board->toMove() == Piece.white ? board->getBlackKingSquare() : board->getWhiteKingSquare())))
                Renderer->drawSprite(ResourceManager::getTexture("square"),
                                     glm::vec2(i * sizeX, j * sizeY),
                                     glm::vec2(sizeX, sizeY),
                                     .0f,
                                     WIN_COLOR);

            
            // // Rysowanie adekwatnej figury
            const int* const squares = board->get();
            if (squares[index] != 0 && !(Dummy.render && Dummy.x == i && Dummy.y == j))
                Renderer->drawSprite(ResourceManager::getTexture(std::to_string(squares[index])),
                                     glm::vec2(i * sizeX, j * sizeY),
                                     glm::vec2(sizeX, sizeY));
        }
    }

    if (Dummy.render)
        Renderer->drawSprite(ResourceManager::getTexture(std::to_string(board->get()[63 - (Dummy.y * 8 + (7 - Dummy.x))])),
                             glm::vec2(mouseX - (sizeX / 2), mouseY - (sizeY / 2)),
                             glm::vec2(sizeX, sizeY));
}