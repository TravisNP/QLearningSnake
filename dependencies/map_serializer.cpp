#include "map_serializer.h"

template<class Archive>
void serialize(Archive& ar, std::vector<std::vector<boost::unordered::unordered_flat_map<std::string, std::vector<double>>>>& mapVector, const unsigned int version) {
    ar & mapVector;
}

void MapSerializer::saveQTable(const std::vector<std::vector<boost::unordered::unordered_flat_map<std::string, std::vector<double>>>>& mapVector, const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw std::runtime_error("Unable to open file for writing");
    }
    boost::archive::text_oarchive oa(ofs);
    oa << mapVector;
}

void MapSerializer::loadQTable(std::vector<std::vector<boost::unordered::unordered_flat_map<std::string, std::vector<double>>>>& mapVector, const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw std::runtime_error("Unable to open file for reading");
    }
    boost::archive::text_iarchive ia(ifs);
    ia >> mapVector;
}