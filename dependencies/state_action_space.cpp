#include "state_action_space.h"

StateActionSpace::StateActionSpace(int board_size) :
    BOARD_SIZE(board_size),
    fruitToStates(board_size, std::vector<boost::unordered_flat_map<std::string, std::vector<double>>>(board_size))

    {
        if (BOARD_SIZE < 1 || BOARD_SIZE > 9)
            throw CustomException("Board size must be between 0 and 10 exclusive");


        initStateActionSpace();

        initHorizontalOrVerticalMatch();

        direction_mappings = DirectionMaps();

        initReflectionMapping();
}

void StateActionSpace::initStateActionSpace() {
    std::string snake = "";
    std::vector<std::vector<bool>> notVisited(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, true));
    const int BOARD_SIZE_SQUARED = pow(BOARD_SIZE, 2);
    for (int row = 0; row < (BOARD_SIZE + 1)/2; ++row)
        for (int col = row; col < (BOARD_SIZE + 1)/2; ++col)
            traverseBoard(row, col, row, col, snake, BOARD_SIZE_SQUARED - 1, notVisited);
}

void StateActionSpace::initHorizontalOrVerticalMatch() {
    for (int i = 0; i < BOARD_SIZE; ++i)
        horizontal_or_vertical_match.push_back('0' + (BOARD_SIZE - 1 - i));
}

void StateActionSpace::traverseBoard(const int headRow, const int headCol, const int row, const int col, std::string snake, const int boardLeft, std::vector<std::vector<bool>>& notVisited) {
    snake += std::to_string(row) + std::to_string(col);
    if (boardLeft == 0) {
        snake.pop_back();
        snake.pop_back();
        return;
    }

    notVisited[row][col] = false;

    // Move tail
    // Up
    if (row > 0 && notVisited[row - 1][col])
        traverseBoard(headRow, headCol, row - 1, col, snake, boardLeft - 1, notVisited);

    // Down
    if (row < BOARD_SIZE - 1 && notVisited[row + 1][col])
        traverseBoard(headRow, headCol, row + 1, col, snake, boardLeft - 1, notVisited);

    // Left
    if (col > 0 && notVisited[row][col - 1])
        traverseBoard(headRow, headCol, row, col - 1, snake, boardLeft - 1, notVisited);

    // Right
    if (col < BOARD_SIZE - 1 && notVisited[row][col + 1])
        traverseBoard(headRow, headCol, row, col + 1, snake, boardLeft - 1, notVisited);



    std::vector<double> qActionSpace(4, BAD_QTABLE_VALUE);
    populateActionSpace(headRow, headCol, qActionSpace, notVisited);

    if (qActionSpace.size() > 0) {
        // Loop through all spots
        for (int r = 0; r < BOARD_SIZE; ++r)
            for (int c = 0; c < BOARD_SIZE; ++c)
                // if spot is available for a fruit, create action space
                if (notVisited[r][c]) {
                    fruitToStates[r][c][reverseSnake(snake)] = qActionSpace;
                }
    }

    notVisited[row][col] = true;
    snake.pop_back();
    snake.pop_back();
}

void StateActionSpace::populateActionSpace(const int headRow, const int headCol, std::vector<double>& qActionSpace, std::vector<std::vector<bool>>& notVisited) {
    // Up
    if (headRow > 0 && notVisited[headRow - 1][headCol])
        qActionSpace[UP] = GOOD_QTABLE_VALUE;

    // Down
    if (headRow < BOARD_SIZE - 1 && notVisited[headRow + 1][headCol])
        qActionSpace[DOWN] = GOOD_QTABLE_VALUE;

    // Left
    if (headCol > 0 && notVisited[headRow][headCol - 1])
        qActionSpace[LEFT] = GOOD_QTABLE_VALUE;

    // Right
    if (headCol < BOARD_SIZE - 1 && notVisited[headRow][headCol + 1])
        qActionSpace[RIGHT] = GOOD_QTABLE_VALUE;
}

const std::string StateActionSpace::reverseSnake(const std::string& snake) {
    std::string reversedSnake = "";
    for (int cell = snake.size() - 1; cell > 0 ; cell -= 2) {
        reversedSnake += snake[cell - 1];
        reversedSnake += snake[cell];
    }

    return reversedSnake;
}

void StateActionSpace::horizontal_or_vertical(std::vector<std::string>& states, bool doVertical) {
    for (int j = 0, statesSize = states.size(); j < statesSize; ++j) {
        std::string reflectedState(states[j]);
        for (int i = doVertical; i < reflectedState.size(); i += 2)
            reflectedState[i] = horizontal_or_vertical_match[reflectedState[i] - '0'];
        states.push_back(reflectedState);
    }
}

void StateActionSpace::diagonal(std::vector<std::string>& states) {
    for (int j = 0, statesSize = states.size(); j < statesSize; ++j) {
        std::string reflectedState(states[j]);
        for (int i = 0; i < reflectedState.size(); i += 2)
            std::swap(reflectedState[i], reflectedState[i+1]);
        states.push_back(reflectedState);
    }
}

void StateActionSpace::initReflectionMapping() {
    // If board size is even
    if (BOARD_SIZE % 2 == 0) {
        int offset;
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                for (auto it = fruitToStates[row][col].begin(); it != fruitToStates[row][col].end(); ++it) {
                    if (stateToOriginalState.contains(it->first))
                        continue;

                    std::vector<std::string> states = {it->first};

                    horizontal_or_vertical(states, false);
                    horizontal_or_vertical(states, true);

                    // If not on the diagonal
                    offset = 0;
                    if ((it->first).back() != (it->first)[(it->first).size() - 2]) {
                        diagonal(states);
                        offset = 4;
                    }

                    for (int i = 0; i < 4 + offset; ++i)
                        stateToOriginalState[states[i]] = make_pair(states[0], direction_mappings.horizontal_vertical_diagonal_order[i]);
                }
            }
        }

    // If board size is odd
    } else {

        // index of the middle column
        const char middleNumChar = '0' + ((BOARD_SIZE + 1)/2 - 1);

        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                for (auto it = fruitToStates[row][col].begin(); it != fruitToStates[row][col].end(); ++it) {
                    if (stateToOriginalState.contains(it->first))
                        continue;

                    std::vector<std::string> states = {it->first};

                    // If on the diagonal
                    if ((it->first).back() == (it->first)[(it->first).size() - 2]) {

                        // If in the very middle, no reflections
                        if ((it->first).back() == middleNumChar) {
                            stateToOriginalState[states[0]] = make_pair(states[0], direction_mappings.horizontal_vertical_diagonal_order[0]);
                            continue;
                        }

                        // if not in the very middle
                        horizontal_or_vertical(states, false);
                        horizontal_or_vertical(states, true);

                        for (int i = 0; i < 4; ++i)
                            stateToOriginalState[states[i]] = make_pair(states[0], direction_mappings.horizontal_vertical_diagonal_order[i]);

                    // if not on diagonal
                    } else {
                        horizontal_or_vertical(states, false);

                        // if not on rightmost column of triangle (middle column)
                        if ((it->first).back() != middleNumChar)
                            horizontal_or_vertical(states, true);

                        diagonal(states);

                        if (states.size() == 8)
                            for (int i = 0; i < 8; ++i)
                                stateToOriginalState[states[i]] = make_pair(states[0], direction_mappings.horizontal_vertical_diagonal_order[i]);
                        else
                            for (int i = 0; i < 4; ++i)
                                stateToOriginalState[states[i]] = make_pair(states[0], direction_mappings.horizontal_diagonal_order[i]);
                    }
                }
            }
        }
    }
}

std::string StateActionSpace::formatSnake(const std::string& snake) {
    std::string formattedSnake = "";
    for (int i = 0; i < snake.size(); i += 2) {
        formattedSnake += snake[i];
        formattedSnake += snake[i+1];
        formattedSnake += '-';
    }

    formattedSnake.pop_back();

    return formattedSnake;
}

void StateActionSpace::printStateActionSpace() {
    int stateActionSpaceCounter = 0;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            stateActionSpaceCounter += fruitToStates[row][col].size();
            std::cout << "Fruit: " << row << col << "--------------------------------------------------------" << std::endl;
            for (auto it = fruitToStates[row][col].begin(); it != fruitToStates[row][col].end(); ++it)
                std::cout << formatSnake(it->first) << std::endl;
        }
    }
    std::cout << "Size of the state X action space: " << stateActionSpaceCounter * 4 << std::endl << std::endl;
}

void StateActionSpace::printReflectedStateMapping() {
    for (auto it = stateToOriginalState.begin(); it != stateToOriginalState.end(); ++it)
        std::cout << formatSnake(it->first) << " --> " << formatSnake((it->second).first) << std::endl;
    std::cout << "Size of reflectedMapping: " << stateToOriginalState.size() << std::endl << std::endl;
}

void StateActionSpace::printQTable() {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            std::cout << "Fruit: " << row << col << "--------------------------------------------------------" << std::endl;
            for (auto it = fruitToStates[row][col].begin(); it != fruitToStates[row][col].end(); ++it) {
                std::string line = "";
                line += formatSnake(it->first) + ": (";
                for (int i = 0; i < 4; ++i) {
                    line += std::to_string(it->second[i]) + ", ";
                }
                line.pop_back();
                line.pop_back();
                std::cout << line << ")" << std::endl;
            }
        }
    }
}