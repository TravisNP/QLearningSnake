#include "snake_game.h"

void SnakeGame::checkParameters(const int board_size, const int numFruit, const int snakeRowStart, const int snakeColStart) {
    if (board_size < 1)
        throw CustomException("board size must be positive");

    if (snakeRowStart >= board_size)
        throw CustomException("snakeRowStart must be less than board size");
    if (snakeRowStart < 0)
        throw CustomException("snakeRowStart must be non-negative");

    if (snakeColStart >= board_size)
        throw CustomException("snakeColStart must be less than board size");
    if (snakeColStart < 0)
        throw CustomException("snakeColStart must be non-negative");

    if (numFruit >= pow(board_size, 2))
        throw CustomException("numFruit must be one less than the total spaces on the board (board size squared)");
    if (numFruit < 1)
        throw CustomException("numFruit must be positive");
}

const std::pair<int, int> SnakeGame::getNewFruitLoc() {
    const int randCell = rand() % emptyCount.first;
    int counter = -1;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        counter += emptyCount.second[row];
        if (randCell <= counter) {
            for (int col = BOARD_SIZE - 1; col >= 0; --col) {
                if (board[row][col] == EMPTY && randCell == counter--)
                    return std::make_pair(row, col);
            }

            throw CustomException("emptyCount.second[" + std::to_string(row) + "] is more than the actual amount of empty cells in row");
        }
    }

    throw CustomException("emptyCount.first is more than the actual amount of empty cells in board");
}

void SnakeGame::addFruit() {
    std::pair<int, int> newFruitLoc = getNewFruitLoc();
    --emptyCount.first;
    --emptyCount.second[newFruitLoc.first];
    board[newFruitLoc.first][newFruitLoc.second] = FRUIT;
    snake[0] = '0' + newFruitLoc.first;
    snake[1] = '0' + newFruitLoc.second;
}

bool SnakeGame::moveSnake(const DIR dir) {
    // If snake cannot move in the wanted direction, return false
    int snakeRow = snake[snake.size() - 2] - '0';
    int snakeCol = snake.back() - '0';
    if (dir == UP) {
        if (snakeRow == 0 || board[snakeRow - 1][snakeCol] == SNAKE)
            return false;
        --snakeRow;
    } else if (dir == DOWN) {
        if (snakeRow == BOARD_SIZE - 1 || board[snakeRow + 1][snakeCol] == SNAKE)
            return false;
        ++snakeRow;
    } else if (dir == LEFT) {
        if (snakeCol == 0 || board[snakeRow][snakeCol - 1] == SNAKE)
            return false;
        --snakeCol;
    } else if (dir == RIGHT) {
        if (snakeCol == BOARD_SIZE - 1 || board[snakeRow][snakeCol + 1] == SNAKE)
            return false;
        ++snakeCol;
    }

    // If new space is not a fruit, update board space of tail, add one to empty counter of tail row, and pop off tail
    // Also, decrement one to empty counter of head row
    if (board[snakeRow][snakeCol] != FRUIT) {
        board[snake[left] - '0'][snake[left + 1] - '0'] = EMPTY;
        ++emptyCount.second[snake[left] - '0'];
        left += 2;

        --emptyCount.second[snakeRow];
    } else {
        if (emptyCount.first > 0)
            addFruit();
    }

    // Update head of snake and mark board space of head as snake
    snake += std::to_string(snakeRow) + std::to_string(snakeCol);
    board[snakeRow][snakeCol] = SNAKE;

    return true;
}

std::tuple<std::string, int, int> SnakeGame::resetGame() {
    board = std::vector<std::vector<CELL>>(BOARD_SIZE, std::vector<CELL>(BOARD_SIZE, EMPTY));
    emptyCount.first = pow(BOARD_SIZE, 2);
    emptyCount.second = std::vector<int>(BOARD_SIZE, BOARD_SIZE);
    snake = "####";
    left = 4;

    // Put snake on board
    board[INITIAL_ROW_POS][INITIAL_COL_POS] = SNAKE;
    --emptyCount.first;
    --emptyCount.second[INITIAL_ROW_POS];
    snake += std::to_string(INITIAL_ROW_POS) + std::to_string(INITIAL_COL_POS);

    // Add fruit to board
    for (int i = 0; i < NUM_FRUIT; ++i)
        addFruit();

    return std::make_tuple(snake, left, NOTHING);
}

SnakeGame::SnakeGame(const int board_size, const int numFruit, int snakeRowStart, int snakeColStart)
    : BOARD_SIZE(board_size),
    BOARD_SIZE_SQUARED_TIMES_2(2*pow(board_size, 2)),
    INITIAL_ROW_POS(snakeRowStart),
    INITIAL_COL_POS(snakeColStart),
    NUM_FRUIT(numFruit),
    board(board_size, std::vector<CELL>(board_size, EMPTY)),
    emptyCount({pow(board_size, 2), std::vector<int>(board_size, board_size)}),
    userMoveToEnum({{"w", UP}, {"s", DOWN}, {"a", LEFT}, {"d", RIGHT}}),
    snake("####"),
    left(4)

    {
    if (INITIAL_ROW_POS == INT_MIN)
        INITIAL_ROW_POS = (BOARD_SIZE+1)/2 - 1;
    if (INITIAL_COL_POS == INT_MIN)
        INITIAL_COL_POS = (BOARD_SIZE+1)/2 - 1;
    checkParameters(BOARD_SIZE, NUM_FRUIT, INITIAL_ROW_POS, INITIAL_COL_POS);

    srand((unsigned) time(NULL));

    // Put snake on board
    board[INITIAL_ROW_POS][INITIAL_COL_POS] = SNAKE;
    --emptyCount.first;
    --emptyCount.second[INITIAL_ROW_POS];
    snake += std::to_string(INITIAL_ROW_POS) + std::to_string(INITIAL_COL_POS);

    // Add fruit to board
    for (int i = 0; i < NUM_FRUIT; ++i)
        addFruit();
}

std::tuple<std::string, int, int> SnakeGame::step(DIR dir) {
    bool oneSpaceLeft = (emptyCount.first == 0);

    // If snake cannot move in the wanted direction, return false
    int snakeRow = snake[snake.size() - 2] - '0';
    int snakeCol = snake.back() - '0';
    if (dir == UP) {
        if (snakeRow == 0 || board[snakeRow - 1][snakeCol] == SNAKE)
            return std::make_tuple("", left, REWARD_DEATH);
        --snakeRow;
    } else if (dir == DOWN) {
        if (snakeRow == BOARD_SIZE - 1 || board[snakeRow + 1][snakeCol] == SNAKE)
            return std::make_tuple("", left, REWARD_DEATH);
        ++snakeRow;
    } else if (dir == LEFT) {
        if (snakeCol == 0 || board[snakeRow][snakeCol - 1] == SNAKE)
            return std::make_tuple("", left, REWARD_DEATH);
        --snakeCol;
    } else if (dir == RIGHT) {
        if (snakeCol == BOARD_SIZE - 1 || board[snakeRow][snakeCol + 1] == SNAKE)
            return std::make_tuple("", left, REWARD_DEATH);
        ++snakeCol;
    }

    bool isFruit = board[snakeRow][snakeCol] == FRUIT;

    // If new space is not a fruit, update board space of tail, add one to empty counter of tail row, and pop off tail
    // Also, decrement one to empty counter of head row
    if (!isFruit) {
        board[snake[left] - '0'][snake[left + 1] - '0'] = EMPTY;
        ++emptyCount.second[snake[left] - '0'];
        left += 2;

        --emptyCount.second[snakeRow];
    } else {
        if (emptyCount.first > 0)
            addFruit();
    }

    // Update head of snake and mark board space of head as snake
    snake += std::to_string(snakeRow) + std::to_string(snakeCol);
    board[snakeRow][snakeCol] = SNAKE;

    // If there was no empty spaces and snake was able to move (all spaces but one fill by snake, last with fruit), board is complete
    if (oneSpaceLeft)
        return std::make_tuple(snake, left, REWARD_FINISH);

    // If not finish but we ate a fruit
    if (isFruit)
        return std::make_tuple(snake, left, REWARD_FRUIT);

    // If did not die, finish, or eat a fruit, did nothing
    return std::make_tuple(snake, left, REWARD_NOTHING);
}

void SnakeGame::playSingleSnake(const int NUM_ROUNDS) {
    std::string userMove;
    for (int round = 0; round < NUM_ROUNDS; ++round) {
        std::cin >> userMove;
        if (userMoveToEnum.contains(userMove)) {
            if (!moveSnake(userMoveToEnum[userMove]))
                break;
            if ((snake.size() - left) == BOARD_SIZE_SQUARED_TIMES_2)
                return;
        } else {
            --round;
        }

        printAll();
    }
}

void SnakeGame::printBoard() {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col)
            std::cout << board[row][col] << " ";
        std::cout << std::endl;
    }
}

void SnakeGame::printEmptyCount() {
    for (int i = 0; i < BOARD_SIZE; ++i)
        std::cout << emptyCount.second[i] << " ";
    std::cout << std::endl;
}

void SnakeGame::printAll() {
    std::cout << emptyCount.first << " empty cells" << std::endl;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        std::cout << emptyCount.second[row];
        if (emptyCount.second[row] < 10)
            std::cout << " ";
        std::cout << " | ";
        for (int col = 0; col < BOARD_SIZE; ++col)
            std::cout << board[row][col] << " ";
        std::cout << std::endl;
    }
}