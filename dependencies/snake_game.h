#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include<cstdio>
#include<vector>
#include<queue>
#include<unordered_map>
#include<limits.h>
#include<math.h>
#include<boost/unordered/unordered_flat_map.hpp>

#include "state_action_space.h"
#include "enums.h"
#include "custom_exceptions.h"

class SnakeGame {
private:
    // Size of the board
    int BOARD_SIZE;

    // Size of the board squared times 2
    int BOARD_SIZE_SQUARED_TIMES_2;

    // Number of fruit on the board (until there is no more space)
    int NUM_FRUIT;

    // Initial row of the snake head
    int INITIAL_ROW_POS;

    // Initial column of the snake head
    int INITIAL_COL_POS;

    // the board
    std::vector<std::vector<CELL>> board;

    // pair of the total number of empty cells and a vector with the number of empty cells for each row
    std::pair<int, std::vector<int>> emptyCount;

    // characters corresponding to up, down, left, and right
    std::unordered_map<std::string, DIR> userMoveToEnum;

    // the snake
    std::string snake;

    // pointer to snake tail
    int left;

    // Seed of the random number generator
    unsigned int seed;

    /**
     * Checks that all parameters to the constructor are valid
     * @param board_size the board size
     * @param numFruit the number of fruit
     * @param snakeRowStart the starting row of the snake
     * @param snakeColStart the startinc column of the snake
     * @throws CustomException if the parameters are not valid
     */
    void checkParameters(const int board_size, const int numFruit, const int snakeRowStart, const int snakeColStart);

    /**
     * Generates a random fruit location
     *
     * @return random fruit location
     */
    const std::pair<int, int> getNewFruitLoc();

    /**
     * Adds a fruit to the board and adjusts empty counts
     */
    void addFruit();

    /**
     * Moves the snake UP, DOWN, LEFT, or RIGHT
     *
     * @param dir direction the snake will move
     * @return true if move successful, false if not
     */
    bool moveSnake(const DIR dir);

public:
    /**
     * Constructor for the snake game
     *
     * @param board_size the board size
     * @param numFruit the number of fruit on the board
     * @param snakeRowStart the starting row of the snake
     * @param snakeColStart the starting column of the snake
     */
    SnakeGame(const int board_size = 10, const int numFruit = 1, int snakeRowStart = INT_MIN, int snakeColStart = INT_MIN, unsigned int seen = time(NULL));

    /**
     * This moves the snake in the direction given and returns the new state and reward
     * @param direction the direction of the snake to move in
     * @return a tuple of the new state (full snake history and tail) and reward
     */
    std::tuple<std::string, int, int> step(DIR direction);

    /**
     * Plays the snake game in user mode
     * @param maxRounds the max number of rounds
     */
    void playUserSnake(const int maxRounds = 50);

    /**
     * Plays the snake game in computer mode and visualizes the game in the terminal
     * @param qtable the model
     */
    void playCompSnakeVisualize(StateActionSpace* stateActionSpace, const int numRounds);

    /**
     * Plays the snake game in computer mode and returns info about the result
     * @param stateActionSpace the qtable and reflection information
     * @param maxRounds the maximum number of rounds the simulation can run
     * @return
     */
    CompInfo playCompSnakeInfo(StateActionSpace* stateActionSpace, const int maxROunds);

    /**
     * Gets the computer move by taking the argmax of the qtable for the respective state
     * @param qtable the move
     */
    DIR getCompMove(StateActionSpace* stateActionSpace);

    /**
     * Resets the snake game to the default beginning state
     * @return the snake, tail location, and reward (for convienence when training)
     */
    std::tuple<std::string, int, int> resetGame();

    /**
     * Prints the board
     */
    void printBoard();

    /**
     * Prints the empty counts
     */
    void printEmptyCount();

    /**
     * Prints all data. This includes the board and empty counts
     */
    void printAll();
};

#endif