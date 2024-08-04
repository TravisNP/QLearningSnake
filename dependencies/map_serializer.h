// MapSerializer class was made with heavy help from chatgpt

#ifndef MAP_VECTOR_SERIALIZER_HPP
#define MAP_VECTOR_SERIALIZER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "custom_structs.h"

class MapSerializer {
public:

    // Serialize the vector of unordered_flat_map to a file
    static void saveQTable(const Model &model, const std::string& filename);

    // Deserialize the vector of unordered_flat_map from a file
    static void loadQTable(Model &model, const std::string& filename);
};

#endif // MAP_VECTOR_SERIALIZER_HPP