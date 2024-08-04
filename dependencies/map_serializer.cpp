#include "map_serializer.h"

void MapSerializer::saveQTable(const Model &model, const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw std::runtime_error("Unable to open file for writing");
    }
    boost::archive::text_oarchive oa(ofs);
    oa << model;
}

void MapSerializer::loadQTable(Model &model, const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw std::runtime_error("Unable to open file for reading");
    }
    boost::archive::text_iarchive ia(ifs);
    ia >> model;
}