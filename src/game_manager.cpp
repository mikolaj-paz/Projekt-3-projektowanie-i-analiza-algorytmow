#include "game_manager.hpp"

#define PLAYER_COLOR WHITE

#if PLAYER_COLOR == WHITE
    #define PLAYER_INDEXING 63 - (j * 8 + (7 - i))
    #define PLAYER_INDEXING_DUMMY 63 - (Dummy.y * 8 + (7 - Dummy.x))
    #define BOT_COLOR Piece.black
#else
    #define PLAYER_INDEXING j * 8 + (7 - i)
    #define PLAYER_INDEXING_DUMMY Dummy.y * 8 + (7 - Dummy.x)
    #define BOT_COLOR Piece.white
#endif

#define DARK_SQUARE_COLOR glm::vec3(.45f, .60f, .68f)
#define LIGHT_SQUARE_COLOR glm::vec3(.85f, .88f, .9f) 

#define MOVE_SQUARE_COLOR glm::vec3(2.0f, .5f, .5f) 
#define LAST_MOVE_COLOR glm::vec3(.5f, .9f, 1.0f)

#define WIN_COLOR glm::vec3(.35f, .7f, .45f)
#define DRAW_COLOR glm::vec3(1.0f, .65f, .4f)

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

        // Ladowanie tekstury szachownicy
        ResourceManager::loadTexture("../assets/marble.jpg", false, "marble");

        // Ladowanie pola
        ResourceManager::loadQuad(this->getWidth() / 8, 190, "square");
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

            Dummy.origin = PLAYER_INDEXING_DUMMY;
            Dummy.piece = board->get()[Dummy.origin];
            if (Dummy.piece == 0) 
            {
                Dummy.render = false;
                return;
            }
            else
            {
                if (board->toMove() == (Dummy.piece & Piece.colorMask) && board->toMove() == PLAYER_COLOR)
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

            int target = PLAYER_INDEXING_DUMMY;
            if (Dummy.availableMovesBoard[target])
            {
                board->update(Move(Dummy.origin, target, Dummy.piece, board->get()[target]));
                gameState = MoveGenerator::getBoardState(board);
            }

            delete[] Dummy.availableMovesBoard;
        }
    }
}

void botThink(Board* board, std::atomic<GameState>* gameState, std::atomic<BotState>* botState)
{   
    Bot::makeMove(board);
    *gameState = MoveGenerator::getBoardState(board);
    *botState = IDLING;
}

void GameManager::update(float deltaTime)
{
    if (gameState == GAME_ACTIVE && botState == IDLING
        && board->toMove() == BOT_COLOR     // Zkomentowac, jezeli ma grac BOT vs BOT
    )
    {
        botState = THINKING;
        std::thread(botThink, board, &gameState, &botState).detach();
    }
}

void GameManager::render()
{
    const int sizeY = this->getHeight() / 8;
    const int sizeX = this->getWidth() / 8;

    Renderer->drawSprite(ResourceManager::getTexture("marble"),
                         glm::vec2(.0f,.0f),
                         glm::vec2(this->getHeight(), this->getWidth()));


    const Move* lastMove = &board->getLastMove();

    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            int index = PLAYER_INDEXING;

            auto squareColor = (i + j) % 2 ? DARK_SQUARE_COLOR : LIGHT_SQUARE_COLOR;
            auto specialColor = (lastMove->getOrigin() == index || lastMove->getTarget() == index) && lastMove->getPiece() != 0 ? LAST_MOVE_COLOR : glm::vec3(1.0f);
            
            if (Dummy.render && Dummy.availableMovesBoard[index])
                specialColor = MOVE_SQUARE_COLOR;

            auto finalColor = squareColor * specialColor;

            if (gameState == GAME_WIN && ((1ULL << index) & (board->toMove() == Piece.white ? board->getBlackKingSquare() : board->getWhiteKingSquare())))
                finalColor = WIN_COLOR;
            else if (gameState == GAME_DRAW && ((1ULL << index) & (board->getWhiteKingSquare() | board->getBlackKingSquare())))
                finalColor = DRAW_COLOR;


            // Rysowanie pola
            Renderer->drawSprite(ResourceManager::getTexture("square"), 
                                 glm::vec2(i * sizeX, j * sizeY),
                                 glm::vec2(sizeX, sizeY),
                                 .0f,
                                 finalColor);

            // // Rysowanie adekwatnej figury
            const int* const squares = board->get();
            if (squares[index] != 0 && !(Dummy.render && Dummy.x == i && Dummy.y == j))
                Renderer->drawSprite(ResourceManager::getTexture(std::to_string(squares[index])),
                                     glm::vec2(i * sizeX, j * sizeY),
                                     glm::vec2(sizeX, sizeY));
        }
    }

    if (Dummy.render)
        Renderer->drawSprite(ResourceManager::getTexture(std::to_string(board->get()[PLAYER_INDEXING_DUMMY])),
                             glm::vec2(mouseX - (sizeX / 2), mouseY - (sizeY / 2)),
                             glm::vec2(sizeX, sizeY));
}