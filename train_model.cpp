#include <random>
#include "dependencies/snake_game.h"
#include "dependencies/state_action_space.h"
#include "dependencies/map_serializer.h"

// size of the square board
#define BOARD_SIZE 4

// number of episoes
#define NUM_EPISODES 100'000'000

// maximum number of rounds per episode
#define NUM_ROUNDS 1000

// Maximum epsilon value
#define MAX_EPSILON 1.0

// Minimum epsilon value
#define MIN_EPSILON 0.01

// Decay rate of epsilon
#define DECAY_RATE .0005

// Hyperparameters
#define GAMMA .95

#define LEARNING_RATE .7

/**
 * Trains the model (qtable)
 * This Q-learning algorithm was inspired from the following tutorial on Q-learning:
 * https://www.datacamp.com/tutorial/introduction-q-learning-beginner-tutorial
 * https://medium.com/analytics-vidhya/a-beginners-guide-to-reinforcement-learning-and-its-basic-implementation-from-scratch-2c0b5444cc49
 * https://medium.com/@tp6145/introduction-to-q-learning-for-the-self-driving-cab-problem-ee1dbf959b99
 */
void train(SnakeGame* snakeGame, StateActionSpace* stateActionSpace) {
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0, 1);

    // Determines if the best action will be chosen or if a random direction will be explored
    double epsilon;

    // The reflected direction the snake will take
    DIR reflectedAction;

    // The direction the snake will take on the board
    DIR boardAction;

    // String view of the snake without the fruit characters - this is done in O(1) time
    std::string_view snakeNoFruit;

    // First is the state which lives in the state space, second is the reflection used to go from the state in the state space to the actual snake
    std::pair<std::string, REFLECTION> stateReflection;

    // First is the state which lives in the state space, second is the reflection used to go from the state in the state space to the actual snake
    std::pair<std::string, REFLECTION> newStateReflection;

    // The fruit location in the state space
    std::pair<int, int> stateFruitLoc;

    // The fruit location in the state space
    std::pair<int, int> newStateFruitLoc;

    // The old state and reward (snake, tail, reward)
    std::tuple<std::string, int, int> stateReward;

    // Represents the maximum of the qtable of the next action
    double newMax;

    // Keeps track if episode is finished
    bool episodeNotFinished;

    for (int episode = 0; episode < NUM_EPISODES; ++episode) {

        epsilon = MIN_EPSILON + (MAX_EPSILON - MIN_EPSILON)*exp(-DECAY_RATE*episode);
        // std::cout << "Epsilon: " << epsilon << std::endl;

        stateReward = snakeGame->resetGame();

        snakeNoFruit = std::string_view(get<0>(stateReward).c_str() + get<1>(stateReward), get<0>(stateReward).size() - get<1>(stateReward));

        stateReflection = stateActionSpace->stateToOriginalState.at(snakeNoFruit);

        stateFruitLoc = stateActionSpace->direction_mappings.functionMaps[stateReflection.second].first(get<0>(stateReward)[0], get<0>(stateReward)[1], BOARD_SIZE);

        episodeNotFinished = true;

        for (int round = 0; round < NUM_ROUNDS && episodeNotFinished; ++round) {


            // Get action from the qtable using epsilon greedy policy
            if (dist(gen) > epsilon) {
                std::vector<double> tempQTable = stateActionSpace->fruitToStates[stateFruitLoc.first][stateFruitLoc.second][stateReflection.first];
                reflectedAction = static_cast<DIR>(std::distance(tempQTable.begin(), std::max_element(tempQTable.begin(), tempQTable.end())));
            } else
                reflectedAction = static_cast<DIR>(dist(gen) * 4);

            boardAction = stateActionSpace->direction_mappings.functionMaps[stateReflection.second]
                            .second[reflectedAction];

            stateReward = snakeGame->step(boardAction);

            /** If snake ate a fruit or moved to an empty space, do if block. If died or finished, do else block
             * Calculates the new state information and gets the max of the new states qtable
             * Note that this code in this block cannot run if the snake dies or finishes as the state does not exist to reduce state space */
            if (get<2>(stateReward) == REWARD_NOTHING || get<2>(stateReward) == REWARD_FRUIT) {
                // String view of the snake without the fruit characters - this is done in O(1) time
                snakeNoFruit = std::string_view(get<0>(stateReward).c_str() + get<1>(stateReward), get<0>(stateReward).size() - get<1>(stateReward));

                newStateReflection = stateActionSpace->stateToOriginalState.at(snakeNoFruit);
                newStateFruitLoc = stateActionSpace->direction_mappings.functionMaps[newStateReflection.second].first(get<0>(stateReward)[0], get<0>(stateReward)[1], BOARD_SIZE);

                // Set the new qtable
                std::vector<double> tempNewQTable = stateActionSpace->fruitToStates[newStateFruitLoc.first][newStateFruitLoc.second][newStateReflection.first];

                newMax = *std::max_element(tempNewQTable.begin(), tempNewQTable.end());
            } else {
                newMax = get<2>(stateReward);
                episodeNotFinished = false;
            }

            // Updates the qtable
            stateActionSpace->fruitToStates[stateFruitLoc.first][stateFruitLoc.second][stateReflection.first][reflectedAction] += LEARNING_RATE * (get<2>(stateReward) + GAMMA * newMax  - stateActionSpace->fruitToStates[stateFruitLoc.first][stateFruitLoc.second][stateReflection.first][reflectedAction]);

            stateReflection = newStateReflection;
            stateFruitLoc = newStateFruitLoc;

        }

        if (episode % 1'000'000 == 0) {
            if (episode != 0)
                std::cout << episode << std::endl;
        }
    }
}

int main()
{
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

    train(snakeGame, stateActionSpace);

    auto formatDouble = [](double value) {
        std::ostringstream out;
        out << std::defaultfloat << value;
        return out.str();
    };

    std::string fileName = "models/BAD_QTABLE_0/model_BS" + formatDouble(BOARD_SIZE) + "_EP" + formatDouble(NUM_EPISODES) + "_R" + formatDouble(NUM_ROUNDS)
                             + "_MXE" + formatDouble(MAX_EPSILON) + "_MNE" + formatDouble(MIN_EPSILON) + "_DR" + formatDouble(DECAY_RATE)
                             + "_G" + formatDouble(GAMMA) + "_LR" + formatDouble(LEARNING_RATE) + ".dat";

    Model model = Model(stateActionSpace->fruitToStates, BOARD_SIZE, NUM_EPISODES, NUM_ROUNDS, MAX_EPSILON, MIN_EPSILON, DECAY_RATE, GAMMA, LEARNING_RATE);
    MapSerializer::saveQTable(model, fileName);

    delete stateActionSpace;
    delete snakeGame;
    return 0;
}