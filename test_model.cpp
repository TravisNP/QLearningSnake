#include "dependencies/snake_game.h"
#include "dependencies/state_action_space.h"
#include "dependencies/map_serializer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./test_model <model.dat> <visualize or test>" << std::endl;
        return -1;
    }

    Model model;
    std::string model_name = argv[1];
    MapSerializer::loadQTable(model, model_name);
    int BOARD_SIZE = model.boardSize;

    SnakeGame* snakeGame;
    try {
        snakeGame = new SnakeGame(BOARD_SIZE, 1, INT_MIN, INT_MIN, 0);
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
    std::string visualizeOrTest = argv[2];
    if (argc == 3 && visualizeOrTest == "visualize") {
        snakeGame->playCompSnakeVisualize(stateActionSpace, 1'000);
        return 0;
    }

    const int numSimulations = 10'000;
    double averageLength = 0;
    double winPercentage = 0;
    double drawPercentage = 0;
    double losePercentage = 0;

    CompInfo compInfo;

    for (int simulation = 0; simulation < numSimulations; ++simulation) {
        snakeGame->resetGame();
        compInfo = snakeGame->playCompSnakeInfo(stateActionSpace, 1'000);
        averageLength += compInfo.snakeLength;
        winPercentage += (compInfo.result == 1);
        drawPercentage += (compInfo.result == 0);
        losePercentage += (compInfo.result == -1);
    }

    averageLength /= numSimulations;
    winPercentage /= numSimulations;
    drawPercentage /= numSimulations;
    losePercentage /= numSimulations;

    auto formatDouble = [](double value) {
        std::ostringstream out;
        out << std::defaultfloat << value;
        return out.str();
    };
    std::cout << "Number Simulations: " << numSimulations << std::endl
                << "Avg Length: " << formatDouble(averageLength) << std::endl
                << "Win Percentage: " << formatDouble(winPercentage * 100) << "%" << std::endl
                << "Draw Percentage: " << formatDouble(drawPercentage * 100) << "%" << std::endl
                << "Lose Percentage: " << formatDouble(losePercentage * 100) << "%" << std::endl;

    return 0;
}