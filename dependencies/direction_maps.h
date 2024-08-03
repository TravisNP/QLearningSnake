#ifndef DIRECTION_MAPS_H
#define DIRECTION_MAPS_H

#include<vector>
#include "enums.h"

// type for maps for reflected states - first gets reflected fruit position, second gives the correct head direction
typedef std::pair<std::pair<int, int> (*)(char, char, int), std::vector<DIR>> function_map;

class DirectionMaps {

    private:
        function_map nothing_map;
        function_map horizontal_map;
        function_map vertical_map;
        function_map diagonal_map;
        function_map horizontal_vertical_map;
        function_map horizontal_diagonal_map;
        function_map vertical_diagonal_map;
        function_map horizontal_vertical_diagonal_map;

    public:
        std::vector<REFLECTION> horizontal_vertical_diagonal_order;
        std::vector<REFLECTION> horizontal_diagonal_order;

        std::vector<function_map> functionMaps;

        DirectionMaps();

};

#endif