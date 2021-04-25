//
// Created by Skaner on 20.04.2021.
//

#include "IntTypeDecoder.h"

void IntTypeDecoder::Decode() {
    for (auto val=values.begin(); val!=values.end(); ++val)
    {
        if (*val==INT32_MIN)
            res->push_back("");
        res->push_back(std::to_string(*val));
    }
}

IntTypeDecoder::IntTypeDecoder(const unsigned short &blockSize) {
    valuesCount = blockSize;
    values = std::vector<int>();
    res = std::shared_ptr<std::vector<std::string>>(new std::vector<std::string>());
}

void IntTypeDecoder::Read(std::ifstream &infile) {
    int val;
    for (int i=0; i<valuesCount; ++i)
    {
        infile.read((char*)&val, sizeof(val));
        values.push_back(val);
    }
}