#include "direction_maps.h"

std::pair<int, int> nothing_map_func(char a, char b, int board_size) {
    return std::make_pair(a - '0', b - '0');
}

std::pair<int, int> horizontal_map_func(char a, char b, int board_size) {
    return std::make_pair(board_size - 1 - (a - '0'), b - '0');
}

std::pair<int, int> vertical_map_func(char a, char b, int board_size) {
    return std::make_pair(a - '0', board_size - 1 - (b - '0'));
}

std::pair<int, int> diagonal_map_func(char a, char b, int board_size) {
    return std::make_pair(b - '0', a - '0');
}

std::pair<int, int> horizontal_vertical_map_func(char a, char b, int board_size) {
    return std::make_pair(board_size - 1 - (a - '0'), board_size - 1 - (b - '0'));
}

std::pair<int, int> horizontal_diagonal_map_func(char a, char b, int board_size) {
    return std::make_pair(board_size - 1 - (b - '0'), a - '0');
}

std::pair<int, int> vertical_diagonal_map_func(char a, char b, int board_size) {
    return std::make_pair(b - '0', board_size - 1 - (a - '0'));
}

std::pair<int, int> horizontal_vertical_diagonal_map_func(char a, char b, int board_size) {
    return std::make_pair(board_size - 1 - (b - '0'), board_size - 1 - (a - '0'));
}

DirectionMaps::DirectionMaps() {
    nothing_map = std::make_pair(&nothing_map_func, std::vector<DIR>{UP, DOWN, LEFT, RIGHT});
    horizontal_map = std::make_pair(&horizontal_map_func, std::vector<DIR>{DOWN, UP, LEFT, RIGHT});
    vertical_map = std::make_pair(&vertical_map_func, std::vector<DIR>{UP, DOWN, RIGHT, LEFT});
    diagonal_map = std::make_pair(&diagonal_map_func, std::vector<DIR>{LEFT, RIGHT, UP, DOWN});
    horizontal_vertical_map = std::make_pair(&horizontal_vertical_map_func, std::vector<DIR>{DOWN, UP, RIGHT, LEFT});
    horizontal_diagonal_map = std::make_pair(&horizontal_diagonal_map_func, std::vector<DIR>{RIGHT, LEFT, UP, DOWN});
    vertical_diagonal_map = std::make_pair(&vertical_diagonal_map_func, std::vector<DIR>{LEFT, RIGHT, DOWN, UP});
    horizontal_vertical_diagonal_map = std::make_pair(&horizontal_vertical_diagonal_map_func, std::vector<DIR>{RIGHT, LEFT, DOWN, UP});

    horizontal_vertical_diagonal_order = {
        NOTHING,
        HORIZONTAL,
        VERTICAL,
        HORIZONTAL_VERTICAL,
        DIAGONAL,
        HORIZONTAL_DIAGONAL,
        VERTICAL_DIAGONAL,
        HORIZONTAL_VERTICAL_DIAGONAL
    };

    horizontal_diagonal_order = {
        NOTHING,
        HORIZONTAL,
        DIAGONAL,
        HORIZONTAL_DIAGONAL
    };

    functionMaps = {nothing_map, horizontal_map, vertical_map, diagonal_map, horizontal_vertical_map,
                                horizontal_diagonal_map, vertical_diagonal_map, horizontal_vertical_diagonal_map};
}