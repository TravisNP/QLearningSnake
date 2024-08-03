#include "enums.h"

std::ostream& operator<<(std::ostream& os, const CELL& cell) {
    switch (cell) {
        case EMPTY:
            os << "E";
            break;
        case SNAKE:
            os << "\033[32mS\033[0m";
            break;
        case FRUIT:
            os << "\033[31mF\033[0m";
            break;
        default:
            os << "U";
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const DIR& cell) {
    switch (cell) {
        case UP:
            os << "U";
            break;
        case DOWN:
            os << "D";
            break;
        case LEFT:
            os << "L";
            break;
        case RIGHT:
            os << "R";
            break;
        default:
            os << "?";
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const REFLECTION& cell) {
    switch (cell) {
        case NOTHING:
            os << "N";
            break;
        case HORIZONTAL:
            os << "H";
            break;
        case VERTICAL:
            os << "V";
            break;
        case DIAGONAL:
            os << "D";
            break;
        case HORIZONTAL_VERTICAL:
            os << "HV";
            break;
        case HORIZONTAL_DIAGONAL:
            os << "HD";
            break;
        case VERTICAL_DIAGONAL:
            os << "VD";
            break;
        case HORIZONTAL_VERTICAL_DIAGONAL:
            os << "HVD";
            break;
        default:
            os << "?";
            break;
    }
    return os;
}