#include "dependencies/snake_game.h"
#include "dependencies/state_action_space.h"
#include "dependencies/map_serializer.h"

int main() {
    std::string model_name = "models/model_BS3_EP3e+06_R100_MXE1_MNE0.01_DR0.0005_G0.95_LR0.7.dat";
    const int BOARD_SIZE = model_name[15] - '0';

    StateActionSpace* stateActionSpace;

    try {
        stateActionSpace = new StateActionSpace(BOARD_SIZE);
    } catch(CustomException& e) {
        std::cout << "ERROR: " << e.getMessage() << std::endl;
        delete stateActionSpace;
        return -1;
    }

    SnakeGame* snakeGame;
    try {
        snakeGame = new SnakeGame(BOARD_SIZE, 1);
    } catch (CustomException& e) {
        std::cout << "ERROR: " << e.getMessage() << std::endl;
        delete snakeGame;
        return -1;
    }

    MapSerializer::loadQTable(stateActionSpace->fruitToStates, model_name);
    stateActionSpace->printQTable();

    return 0;
}