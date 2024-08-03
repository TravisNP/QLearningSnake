#include "dependencies/snake_game.h"

#define BOARD_SIZE 2
#define NUM_FRUIT 1
#define NUM_ROUNDS 1000

int main()
{
    SnakeGame* snakeGame;
    try {
        snakeGame = new SnakeGame(BOARD_SIZE, NUM_FRUIT);
    } catch (CustomException& e) {
        std::cout << "ERROR: " << e.getMessage() << std::endl;
        delete snakeGame;
        return -1;
    }

    snakeGame->printAll();
    snakeGame->playSingleSnake(NUM_ROUNDS);

    delete snakeGame;

    return 0;
}