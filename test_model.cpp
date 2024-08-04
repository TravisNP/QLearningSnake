#include "dependencies/snake_game.h"
#include "dependencies/state_action_space.h"
#include "dependencies/map_serializer.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./test_model <model.dat>" << std::endl;
        return -1;
    }

    Model model;
    std::string model_name = argv[1];
    MapSerializer::loadQTable(model, model_name);
    int BOARD_SIZE = model.boardSize;

    SnakeGame* snakeGame;
    try {
        snakeGame = new SnakeGame(BOARD_SIZE, 1);
    } catch (CustomException& e) {
        std::cout << "ERROR: " << e.getMessage() << std::endl;
        delete snakeGame;
        return -1;
    }

    StateActionSpace* stateActionSpace;
    try {
        stateActionSpace = new StateActionSpace(BOARD_SIZE);
    } catch(CustomException& e) {
        std::cout << "ERROR: " << e.getMessage() << std::endl;
        delete stateActionSpace;
        return -1;
    }


    stateActionSpace->fruitToStates = model.qtable;

    snakeGame->playCompSnakeVisualize(stateActionSpace, 100);

    return 0;
}