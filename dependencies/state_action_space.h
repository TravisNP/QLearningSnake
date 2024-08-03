#ifndef STATE_ACTION_SPACE_H
#define STATE_ACTION_SPACE_H

#include<boost/unordered/unordered_flat_map.hpp>

#include "custom_exceptions.h"
#include "direction_maps.h"
#include "custom_structs.cpp"

class StateActionSpace {
    private:
        // size of the board
        int BOARD_SIZE;

        // Initial qtable value for death spots
        int BAD_QTABLE_VALUE = -20;

        // Initial qtable value for not death spots
        int GOOD_QTABLE_VALUE = 0;

        // when flipping the board horizontally or vertically, this vector holds the flipped row/column indx
        std::vector<char> horizontal_or_vertical_match;

        // Initializes horizontal_match
        void initHorizontalOrVerticalMatch();

        // Initializes the State X Action space
        void initStateActionSpace();

        /**
         * Populates stateToAction with the State X Action space
         * @param headRow row of the head
         * @param headCol col of the head
         * @param row row of the tail
         * @param col col of the tail
         * @param snake string represetion of the snake - head to tail - even is row indices, odd is column
         * @param boardLeft amount of board space left
         * @param notVisited keeps track of which cells are occupied by the snake
         */
        void traverseBoard(const int headRow, const int headCol, const int row, const int col, std::string snake, const int boardLeft, std::vector<std::vector<bool>>& notVisited);

        /**
         * Creates the action space for the state for each direction if direction does not go off the board
         * @param headRow row of the head
         * @param headCol col of the head
         * @param dirProbReward the action space for the state
         * @param notVisited keeps track of which cells are occupied by the snake
         */
        void populateActionSpace(const int headRow, const int headCol, std::vector<double>& dirProbReward, std::vector<std::vector<bool>>& notVisited);

        /**
         * Takes in the snake and reverses it
         * @param snake the snake traversed from head to tail
         * @return the snake travered from tail to head
         */
        const std::string reverseSnake(const std::string& snake);

        /**
         * Given a snake, produces all possible reflections
         * @param snake the original snake
         */
        void initReflectionMapping();

        /**
         * Adds the horizontal or vertical reflections to the end of states
         * @param states the snake states to reflect
         * @param doVertical true if a vertical flip, false if a horizontal flip
         */
        void horizontal_or_vertical(std::vector<std::string>& states, bool doVertical);

        /**
         * Adds the diagonal reflections to the end of states
         * @param states the snake states to reflect
         */
        void diagonal(std::vector<std::string>& states);

        /**
         * Formats the string leaving a dash between every two characters
         * Ex: 111000 would give 11-10-00
         * @param snake the string to be formated
         * @return the formatted string
         */
        std::string formatSnake(const std::string& snake);

        /**
         * Formats the string leaving a dash between every two characters
         * Ex: 111000 would give 11-10-00
         * @param snake the string_view to be formated
         * @return the formatted string
         */
        std::string formatSnake_stringView(const std::string_view& snake);


    public:

        /**
         * @param board_size the board size
         */
        StateActionSpace(int board_size = 5);

        /** Populates stateToOriginalState - the reflected state is the key and the value is the equivalent state in the state space
         * along with a pointer to a vector that gives the correct direction according to the relfection type */
        boost::unordered_flat_map<std::string, std::pair<std::string, REFLECTION>, StringViewHash, StringViewEqual> stateToOriginalState;

        // Holds the fruit mapping functions, and the direction vectors of the reflected snakes
        DirectionMaps direction_mappings;

        /** Indexed by row and then column of fruit. Inside is the State X Action space for that fruit location
         * Key is the state of the snake, value is the action space with associated reward */
        std::vector<std::vector<boost::unordered_flat_map<std::string, std::vector<double>>>> fruitToStates;

        // Prints the State X Action space
        void printStateActionSpace();

        // Prints the reflected states to state space mapping
        void printReflectedStateMapping();

        // Prints the QTable
        void printQTable();
};

#endif